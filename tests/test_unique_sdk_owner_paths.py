"""Owner closure guards remain independent from exact C placement credit."""
import importlib.util
from pathlib import Path
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location('unique_sdk_consumer', Path(__file__).resolve().parents[1] / 'scripts/unique_code_report.py')
report = importlib.util.module_from_spec(spec)
spec.loader.exec_module(report)


class OwnerPathsTests(unittest.TestCase):
    def fixture(self, root, paths=None):
        module = types.ModuleType('boot_sdk_unit')
        module.__file__ = str(root / 'scripts/boot_sdk_unit.py')
        values = paths if paths is not None else {'scripts/boot_sdk_unit.py', 'src/sdk/unit.c'}
        for name in ('scripts/boot_sdk_unit.py', 'src/sdk/unit.c'):
            p = root / name
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text('current', encoding='utf8')
        module.current_input_paths = lambda integration, repo: values
        return module

    def call(self, root, module, integration=None):
        union = {'schema': 3, 'kind': 'boot-c-owner-integration', 'sdk_units': {}} if integration is None else integration
        with patch.dict(sys.modules, {'boot_sdk_unit': module}):
            return report.sdk_owner_input_paths(union, root)

    def test_legacy_owner_has_no_extra_closure(self):
        self.assertEqual(report.sdk_owner_input_paths({'schema': 1}, Path('/unused')), set())

    def test_fixed_closure_is_returned_and_fresh_pin_tamper_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            module = self.fixture(root)
            paths = self.call(root, module)
            catalog = {'input_pins': [{'path': n, 'sha256': report.digest((root/n).read_bytes())} for n in sorted(paths)]}
            self.assertEqual(report.validate_pins(catalog, root), paths)
            (root/'src/sdk/unit.c').write_text('changed', encoding='utf8')
            with self.assertRaises(ValueError):
                report.validate_pins(catalog, root)

    def test_owner_validator_refusal_propagates(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            module = self.fixture(root)
            def refuse(*args):
                raise ValueError('SDK exact source or full review drift')
            module.current_input_paths = refuse
            with self.assertRaisesRegex(ValueError, 'exact source'):
                self.call(root, module)

    def test_wrong_kind_or_bool_schema_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            module = self.fixture(root)
            for changes in ({'schema': True}, {'schema': 2}, {'kind': 'partial-sdk-proof'}):
                union = {'schema': 3, 'kind': 'boot-c-owner-integration', 'sdk_units': {}, **changes}
                with self.subTest(changes=changes), self.assertRaises(ValueError):
                    self.call(root, module, union)

    def test_path_closure_rejects_escape_missing_and_omitted_validator(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            for paths in ({'src/sdk/unit.c'}, {'scripts/boot_sdk_unit.py', '../private.json'},
                          {'scripts/boot_sdk_unit.py', 'C:/private.json'},
                          {'scripts/boot_sdk_unit.py', 'src\\sdk\\unit.c'},
                          {'scripts/boot_sdk_unit.py', 'src/sdk/missing.c'},
                          ['scripts/boot_sdk_unit.py']):
                module = self.fixture(root, paths)
                with self.subTest(paths=paths), self.assertRaises(ValueError):
                    self.call(root, module)

    def test_wrong_repository_module_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            module = self.fixture(root)
            module.__file__ = str(root/'elsewhere/boot_sdk_unit.py')
            with self.assertRaisesRegex(ValueError, 'repository mismatch'):
                self.call(root, module)


if __name__ == '__main__':
    unittest.main()
