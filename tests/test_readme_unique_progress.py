import unittest
import xml.etree.ElementTree as ET
from pathlib import Path
import sys
import tempfile
import json
from unittest.mock import patch
import readme_unique_progress as renderer

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))

from readme_unique_progress import catalogue_metrics, counts, paired_metrics, render, render_table


def report(done, total):
    return {"measures": {"matchedCode": done, "totalCode": total}}


class PairedDisplayTests(unittest.TestCase):
    def fixture(self):
        return {"catalogue_complete": False,
                "validated_subset": {"matched_c_unique_bytes": 12, "total_unique_bytes": 100},
                "coverage": {"scoped_ee_bytes": 1000, "catalogued_interval_bytes": 900,
                             "verified_boundary_bytes": 800, "unresolved_gap_bytes": 100, "excluded_vu_bytes": 0},
                "provisional_partition": {"representative_bytes": 200, "certified": False}}

    def cli(self, root, extra=()):
        args = ["readme_unique_progress.py", "--repo", str(root), "--catalogue-report", str(root / "catalogue.json"),
                "--physical-report", str(root / "physical.json"), *extra]
        with patch.object(sys, "argv", args):
            return renderer.main()

    def setup_inputs(self, root):
        (root / "catalogue.json").write_text(json.dumps(self.fixture()))
        (root / "physical.json").write_text(json.dumps(report(40, 1000)))

    def test_cli_readme_markers_and_check_stale_outputs(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.setup_inputs(root)
            readme = root / "README.md"
            readme.write_text("Before\n" + renderer.START + "\nold\n" + renderer.END + "\nAfter\n")
            self.assertEqual(self.cli(root), 0)
            updated = readme.read_text()
            self.assertTrue(updated.startswith("Before\n"))
            self.assertTrue(updated.endswith("\nAfter\n"))
            self.assertEqual(updated.count(renderer.START), 1)
            self.assertIn("Verified boundary subset", updated)
            self.assertEqual(self.cli(root, ["--check"]), 0)
            for path in (root / "progress/paired-code-metrics.json", root / "progress/unique-decompilation.svg", readme):
                original = path.read_bytes()
                stale = original.replace(b"Verified boundary subset", b"Stale boundary subset") if path == readme else original + b"stale"
                path.write_bytes(stale)
                with self.subTest(path=path.name), self.assertRaisesRegex(ValueError, "Stale paired display"):
                    self.cli(root, ["--check"])
                self.assertEqual(path.read_bytes(), stale)
                path.write_bytes(original)

    def test_cli_requires_exactly_one_marker_pair(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.setup_inputs(root)
            for text in ("missing", renderer.START, renderer.START + renderer.END + renderer.START + renderer.END):
                (root / "README.md").write_text(text)
                with self.assertRaisesRegex(ValueError, "exactly one"):
                    self.cli(root)
                self.assertFalse((root / "progress").exists())

    def test_cli_private_output_preserves_readme_and_checks_table(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.setup_inputs(root)
            (root / "README.md").write_text("No markers needed for private preview")
            preview = root / "preview"
            self.assertEqual(self.cli(root, ["--output-dir", str(preview)]), 0)
            self.assertEqual((root / "README.md").read_text(), "No markers needed for private preview")
            ET.fromstring((preview / "unique-decompilation.svg").read_text())
            self.assertEqual(self.cli(root, ["--output-dir", str(preview), "--check"]), 0)
            (preview / "progress-table.md").write_text("stale")
            with self.assertRaisesRegex(ValueError, "Stale paired display: progress-table.md"):
                self.cli(root, ["--output-dir", str(preview), "--check"])

    def test_each_ratio_uses_its_own_numerator_and_denominator(self):
        metrics = paired_metrics(report("12", "100"), report("40", "1000"))
        self.assertEqual(metrics["unique_code"]["matched_percent"], 12)
        self.assertEqual(metrics["loaded_code"]["matched_percent"], 4)
        table = render_table(metrics)
        self.assertIn("| 12 | 100 | 12.0000% |", table)
        self.assertIn("| 40 | 1,000 | 4.0000% |", table)
        ET.fromstring(render(metrics))

    def test_reject_invalid_counts(self):
        for done, total in ((True, 100), (1, 0), (-1, 100), (101, 100), (1.5, 100), ("1.5", "100"), ("-1", "100")):
            with self.subTest(done=done, total=total), self.assertRaises(ValueError):
                counts(report(done, total))

    def test_reject_impossible_pair(self):
        for unique in (report(41, 100), report(12, 1001)):
            with self.assertRaises(ValueError):
                paired_metrics(unique, report(40, 1000))

    def test_incomplete_catalogue_cannot_be_displayed_as_global(self):
        catalogue = {"catalogue_complete": False,
                     "validated_subset": {"matched_c_unique_bytes": 12, "total_unique_bytes": 100},
                     "coverage": {"scoped_ee_bytes": 1000, "catalogued_interval_bytes": 900,
                                  "verified_boundary_bytes": 800, "unresolved_gap_bytes": 100,
                                  "excluded_vu_bytes": 0},
                     "provisional_partition": {"representative_bytes": 200, "certified": False}}
        metrics = catalogue_metrics(catalogue, report(40, 1000))
        self.assertEqual(metrics["unique_code"]["scope"], "verified_boundary_subset")
        self.assertIn("VERIFIED BOUNDARY SUBSET", render(metrics))
        self.assertIn("100 EE bytes remain unresolved", render_table(metrics))
        self.assertIn("no global progress percentage", render_table(metrics))
        catalogue["catalogue_complete"] = True
        with self.assertRaises(ValueError):
            catalogue_metrics(catalogue, report(40, 1000))


if __name__ == "__main__":
    unittest.main()
