"""CLI grouping must preserve legacy export and cannot bypass object proof."""
import importlib.util
import io
import json
from pathlib import Path
import sys
import tempfile
import types
import unittest
from unittest.mock import Mock, patch


SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
sys.path.insert(0, str(SCRIPTS))
SCRIPT = SCRIPTS / "progress_module_report.py"
SPEC = importlib.util.spec_from_file_location("progress_module_report_cli", SCRIPT)
REPORT = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(REPORT)


class ReportGroupingCliTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        for name in ("config/progress-scope.json", "config/target.json", "config/overlays.json",
                     "progress/report.json", "progress/integration.json", "progress/candidates.json"):
            path = self.root / name
            path.parent.mkdir(parents=True, exist_ok=True)
            value = {"levels": []} if name == "config/overlays.json" else {}
            path.write_text(json.dumps(value), encoding="utf-8")
        self.output = self.root / "output/report.json"
        self.summary = self.root / "output/modules.json"
        self.legacy = {"version": 2, "measures": {"totalCode": "16", "matchedCode": "4"},
                       "units": [{"name": "legacy-function"}], "categories": []}
        self.mapper = types.ModuleType("progress_modules")
        self.mapper.group_report = Mock(side_effect=AssertionError("Unexpected grouping"))

    def run_cli(self, *arguments, validator=None):
        argv = [str(SCRIPT), "--output", str(self.output), *arguments]
        with patch.object(REPORT, "ROOT", self.root), \
             patch.object(REPORT.decomp_report, "generate", return_value=self.legacy), \
             patch.object(REPORT.decomp_report, "validate_object_proof", side_effect=validator), \
             patch.object(sys, "argv", argv), \
             patch.dict(sys.modules, {"progress_modules": self.mapper}), \
             patch("sys.stdout", new_callable=io.StringIO):
            return REPORT.main()

    def test_failed_object_proof_prevents_grouping_and_output(self):
        def invalid(_integration, _review):
            raise ValueError("Invalid object proof")
        with self.assertRaisesRegex(ValueError, "Invalid object proof"):
            self.run_cli("--source-module-summary",
                         str(self.summary), validator=invalid)
        self.mapper.group_report.assert_not_called()
        self.assertFalse(self.output.exists())
        self.assertFalse(self.summary.exists())

    def test_default_cli_keeps_legacy_report(self):
        core = REPORT.decomp_report
        argv = [str(SCRIPTS / "decomp_report.py"), "--output", str(self.output)]
        with patch.object(core, "ROOT", self.root), \
             patch.object(core, "generate", return_value=self.legacy), \
             patch.object(core, "validate_object_proof"), \
             patch.object(sys, "argv", argv), \
             patch.dict(sys.modules, {"progress_modules": self.mapper}), \
             patch("sys.stdout", new_callable=io.StringIO):
            self.assertEqual(core.main(), 0)
        self.assertEqual(json.loads(self.output.read_bytes()), self.legacy)
        self.mapper.group_report.assert_not_called()
        self.assertFalse(self.summary.exists())

    def test_grouped_cli_writes_summary_only_after_proof_validation(self):
        events = []
        summary = {"proved_function_placements": 1}
        def validator(_integration, _review):
            events.append("proof")
        def mapper(report, repo):
            events.append("group")
            self.assertEqual(report, self.legacy)
            self.assertEqual(repo, self.root)
            return report, summary
        self.mapper.group_report.side_effect = mapper
        self.assertEqual(self.run_cli("--source-module-summary", str(self.summary),
                                     validator=validator), 0)
        self.assertEqual(events, ["proof", "group"])
        self.assertEqual(json.loads(self.output.read_bytes()), self.legacy)
        self.assertEqual(json.loads(self.summary.read_bytes()), summary)


if __name__ == "__main__":
    unittest.main()
