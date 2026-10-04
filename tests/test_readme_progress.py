"""Regression checks for synchronized proof-derived README progress."""
import contextlib
import io
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import readme_progress as tool


class ReadmeProgressTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.progress = tool.current_progress()

    def test_table_and_bar_use_same_total_and_include_native_only_once(self):
        report, native, date = self.progress
        matched = int(report["measures"]["matchedCode"])
        total = int(report["measures"]["totalCode"])
        table = tool.render_table(report, native, date)
        self.assertIn(f"{matched:,} / {total:,} ({matched / total * 100:.4f}%)", table)
        self.assertIn(f"{matched:,} / {total:,} validated code bytes", tool.render(matched, total))
        self.assertEqual(matched, sum(int(c["measures"]["matchedCode"]) for c in report["categories"]))
        self.assertIn(f"{len(native):,} placements | {sum(f['size'] for f in native):,}", table)

    def test_fresh_bar_cannot_hide_stale_table_and_check_does_not_write(self):
        report, _, _ = self.progress
        svg = tool.render(int(report["measures"]["matchedCode"]), int(report["measures"]["totalCode"]))
        with tempfile.TemporaryDirectory() as directory:
            bar, readme = Path(directory) / "bar.svg", Path(directory) / "README.md"
            bar.write_bytes(svg.encode("utf-8"))
            stale = "intro\n" + tool.START + "\nstale counts\n" + tool.END + "\noutro\n"
            readme.write_text(stale, encoding="utf-8")
            with patch.object(tool, "OUTPUT", bar), patch.object(tool, "README", readme), \
                    patch.object(tool, "current_progress", return_value=self.progress), \
                    patch.object(sys, "argv", ["readme_progress.py", "--check"]):
                with self.assertRaisesRegex(ValueError, "Stale README progress table"):
                    tool.main()
                self.assertEqual(readme.read_text(encoding="utf-8"), stale)
                self.assertEqual(bar.read_text(encoding="utf-8"), svg)
                with patch.object(sys, "argv", ["readme_progress.py"]), contextlib.redirect_stdout(io.StringIO()):
                    self.assertEqual(tool.main(), 0)
                with contextlib.redirect_stdout(io.StringIO()):
                    self.assertEqual(tool.main(), 0)
                self.assertTrue(readme.read_text(encoding="utf-8").startswith("intro\n"))
                self.assertTrue(readme.read_text(encoding="utf-8").endswith("\noutro\n"))

    def test_missing_or_duplicate_markers_fail_closed(self):
        for text in ("no block", tool.START + tool.END + tool.START, tool.START):
            with self.subTest(text=text), self.assertRaises(ValueError):
                tool.update_readme(text, "new counts\n")
