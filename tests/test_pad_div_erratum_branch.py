"""Check the branch-floor transformer against structural fixtures only.

The fixture reproduces the anchors the transformer expects in the generated
`tc-mips.c`; it is not a compiler source and proves nothing about matching.
"""
import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / "scripts/compiler/pad_div_erratum_branch.py"
SPEC = importlib.util.spec_from_file_location("div_branch", SCRIPT)
BRANCH = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(BRANCH)


def fixture() -> str:
    return "\n".join((
        BRANCH.STATE_ANCHOR,
        BRANCH.PAD_ANCHOR,
        BRANCH.COUNT_ANCHOR,
        BRANCH.NOP_EMIT_ANCHOR,
        BRANCH.FORGET_ANCHOR,
    ))


class BranchFloorTransformerTests(unittest.TestCase):
    def test_applied_then_idempotent(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "tc-mips.c"
            path.write_text(fixture(), encoding="utf-8", newline="")
            first = subprocess.run([sys.executable, str(SCRIPT), str(path)],
                                   capture_output=True, text=True)
            self.assertEqual(first.returncode, 0, first.stderr)
            transformed = path.read_bytes()
            self.assertEqual(transformed.count(b"static int\nrac2_div_branch_p (ip)"), 1)
            self.assertIn(b"rac2_div_insns_since_branch", transformed)
            for flags in ([], ["-O"]):
                again = subprocess.run([sys.executable, *flags, str(SCRIPT), str(path)],
                                       capture_output=True, text=True)
                self.assertEqual(again.returncode, 0, again.stderr)
                self.assertIn("already present", again.stdout)
                self.assertEqual(path.read_bytes(), transformed)

    def test_missing_anchor_stops_without_modifying_the_file(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "tc-mips.c"
            original = fixture().replace(BRANCH.FORGET_ANCHOR, "  /* other source */\n")
            path.write_text(original, encoding="utf-8", newline="")
            failed = subprocess.run([sys.executable, str(SCRIPT), str(path)],
                                    capture_output=True, text=True)
            self.assertNotEqual(failed.returncode, 0)
            self.assertEqual(path.read_text(encoding="utf-8"), original)

    def test_refuses_a_file_that_already_carries_the_floor(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "tc-mips.c"
            path.write_text(BRANCH.STATE_ANCHOR + "\nint rac2_div_insns_since_branch;\n",
                            encoding="utf-8", newline="")
            done = subprocess.run([sys.executable, str(SCRIPT), str(path)],
                                  capture_output=True, text=True)
            self.assertEqual(done.returncode, 0, done.stderr)
            self.assertIn("already present", done.stdout)


if __name__ == "__main__":
    unittest.main()
