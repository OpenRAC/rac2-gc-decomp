"""Focused selection and local immutable-identity checks with synthetic sources."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import maintainer_tests as tests
import maintainer_test_policy as policy


class MaintainerTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.repo = Path(temporary.name)
        (self.repo / "scripts").mkdir()
        (self.repo / "tests").mkdir()
        for name in ("test_maintainer_test_policy", "test_maintainer_tests", "test_decomp_report_cli", "test_source_layout"):
            self.write("tests/" + name + ".py", "pass\n")

    def write(self, path, data):
        (self.repo / path).write_text(data, encoding="utf8")

    def test_changed_tests_selected_without_transitive_import_suites(self):
        self.write("scripts/one.py", "pass\n")
        self.write("scripts/two.py", "from one import value\n")
        self.write("tests/test_two.py", "import two\n")
        self.write("tests/test_added.py", "pass\n")
        result = tests.select_modules(self.repo, ["scripts/one.py", "tests/test_added.py"])
        self.assertNotIn("test_two", result)
        self.assertIn("test_added", result)

    def test_orphan_script_stays_explicitly_limited_owner_precheck(self):
        self.write("scripts/one.py", "pass\n")
        self.write("scripts/orphan.py", "pass\n")
        self.write("tests/test_one.py", "import one\n")
        self.assertEqual(len(tests.select_modules(self.repo, ["scripts/one.py", "scripts/orphan.py"])), 3)

    def test_deleted_tests_and_nonpython_executables_keep_limited_smoke(self):
        self.assertEqual(len(tests.select_modules(self.repo, ["tests/test_deleted.py"])), 3)
        self.assertEqual(len(tests.select_modules(self.repo, ["scripts/tool.sh"])), 3)

    def test_toolchain_workflows_and_executables_do_not_claim_full_coverage(self):
        for path in ("toolchain/compiler.json", ".github/workflows/progress.yml", "tools/helper.ps1", "misc/tool.py"):
            with self.subTest(path=path):
                self.assertEqual(len(tests.select_modules(self.repo, [path])), 3)

    def test_source_or_docs_changes_keep_integrity_smoke_checks(self):
        result = tests.select_modules(self.repo, ["src/boot/00-types.cfrag", "docs/README.md"])
        self.assertEqual(len(result), 3)

    def test_changed_python_syntax_checked_without_importing(self):
        self.write("scripts/one.py", "raise RuntimeError('must not execute')\n")
        self.assertEqual(tests.check_changed_python(self.repo, ["scripts/one.py"]), ["scripts/one.py"])
        self.write("scripts/one.py", "def broken(:\n")
        with self.assertRaises(SyntaxError):
            tests.select_modules(self.repo, ["scripts/one.py"])

    def test_unchanged_heavy_source_layout_suite_not_selected(self):
        self.write("scripts/source_layout.py", "pass\n")
        result = tests.select_modules(self.repo, ["scripts/source_layout.py"])
        self.assertNotIn("test_source_layout", result)
        self.assertIn("test_source_layout", tests.select_modules(self.repo, ["tests/test_source_layout.py"]))

    def test_nested_changed_test_does_not_select_flat_homonym(self):
        (self.repo / "tests/nested").mkdir()
        self.write("tests/nested/test_same.py", "pass\n")
        self.write("tests/test_same.py", "pass\n")
        result = tests.select_modules(self.repo, ["tests/nested/test_same.py"])
        self.assertNotIn("test_same", result)

    def test_module_paths_and_missing_modules_are_refused(self):
        for modules in ([], ["../test_one"], ["test_missing"], ["os"], ["test_one.py"]):
            with self.subTest(modules=modules), self.assertRaises(ValueError):
                tests.validate_modules(self.repo, modules)

    def test_authenticated_github_user_requires_id_and_login(self):
        user = {"id": policy.MAINTAINER_ID, "login": policy.MAINTAINER_LOGIN}
        result = subprocess.CompletedProcess([], 0, json.dumps(user), "")
        with patch.object(tests.subprocess, "check_output", return_value="https://github.com/" + policy.REPOSITORY + ".git\n"), \
             patch.object(tests.subprocess, "run", return_value=result) as invoked:
            self.assertEqual(tests.authenticate(self.repo), user)
            self.assertEqual(invoked.call_args.args[0], ["gh", "api", "--hostname", "github.com", "user"])
        for bad in ({"id": 1, "login": policy.MAINTAINER_LOGIN}, {"id": policy.MAINTAINER_ID, "login": "fake"}):
            with patch.object(tests.subprocess, "check_output", return_value="https://github.com/" + policy.REPOSITORY + ".git\n"), \
                 patch.object(tests.subprocess, "run", return_value=subprocess.CompletedProcess([], 0, json.dumps(bad), "")), \
                 self.assertRaises(ValueError):
                tests.authenticate(self.repo)

    def test_fork_origin_is_refused_before_authentication(self):
        with patch.object(tests.subprocess, "check_output", return_value="https://github.com/fork/repo.git\n"), \
             patch.object(tests.subprocess, "run") as api, self.assertRaises(ValueError):
            tests.authenticate(self.repo)
        api.assert_not_called()

    def test_full_fallback_is_discovery_and_targeted_is_explicit(self):
        suite = lambda: unittest.TestSuite([unittest.FunctionTestCase(lambda: None)])
        with patch.object(tests.unittest.defaultTestLoader, "discover", return_value=suite()) as discover:
            self.assertEqual(tests.run_modules(self.repo, None), 0)
            discover.assert_called_once_with(str(self.repo / "tests"))
        with patch.object(tests.unittest.defaultTestLoader, "loadTestsFromNames", return_value=suite()) as load:
            self.assertEqual(tests.run_modules(self.repo, ["test_source_layout"]), 0)
            load.assert_called_once_with(["test_source_layout"])

    def test_empty_selection_cannot_report_success(self):
        with patch.object(tests.unittest.defaultTestLoader, "loadTestsFromNames", return_value=unittest.TestSuite()):
            self.assertEqual(tests.run_modules(self.repo, ["test_source_layout"]), 1)


if __name__ == "__main__":
    unittest.main()
