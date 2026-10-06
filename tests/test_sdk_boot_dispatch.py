"""SDK boot dispatch and full assembly ownership; no compiler execution."""
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
import integration
import boot_sdk_unit as sdk


class SDKBootDispatch(unittest.TestCase):
    def test_valid_sdk_assembly_alias_requires_exact_owner(self):
        function = {**sdk.FUNCTION, 'unit_id': sdk.UNIT, 'origin': 'boot-sdk',
                    'candidate_source': sdk.SOURCE, 'input_section': '.text'}
        address = function['address']
        text = f'.globl func_{address:08X}\nfunc_{address:08X}:\n'
        text += ''.join(f'/* 000000 {address + i:08X} 00000000 */ nop\n'
                        for i in range(0, 152, 4))
        pieces = integration.split_assembly(text, [function])
        self.assertEqual(len(pieces), 1)
        self.assertEqual(pieces[0]['kind'], 'c')
        for key, value in [('unit_id', 'default-gnu8bed'), ('candidate_source', 'other.c'),
                           ('input_section', '.text.fake'), ('symbol', 'OTHER')]:
            wrong = dict(function, **{key: value})
            with self.assertRaises(ValueError):
                integration.split_assembly(text, [wrong])

    def test_default_boot_symbol_guard_remains(self):
        with self.assertRaises(ValueError):
            integration.split_assembly('', [{'symbol': 'ARBITRARY', 'address': 0x1000, 'size': 8}])

    def test_no_sdk_catalog_preserves_exact_default_dispatch(self):
        expected = ({'functions': []}, Path('default.o'), {'cc1': 'old'})
        with patch.object(integration, 'compile_c', return_value=expected) as default:
            with patch.object(Path, 'exists', return_value=False):
                result = integration.compile_boot_c(Path('reference'), Path('private'), Path('tools'))
        self.assertEqual(result, expected)
        default.assert_called_once()

    def test_overlay_dispatch_does_not_invoke_the_present_sdk_boot_owner(self):
        expected = ({'functions': []}, Path('shared.o'), {'cc1': 'default'})
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            catalog = root / sdk.CATALOG
            catalog.parent.mkdir(parents=True)
            catalog.write_text('{}', encoding='utf8')
            with patch.object(integration, 'ROOT', root), \
                    patch.object(integration, '_compile_shared_level_c', return_value=expected) as shared, \
                    patch.object(sdk, 'compile_reviewed', side_effect=AssertionError('SDK is boot-only')) as sdk_compile:
                result = integration.compile_level_c(Path('reference'), Path('private'), Path('tools'), '0_aranos_tutorial')
            self.assertEqual(result, expected)
            shared.assert_called_once()
            sdk_compile.assert_not_called()


if __name__ == '__main__':
    unittest.main()
