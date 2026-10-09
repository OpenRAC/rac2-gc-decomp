"""Check the transformer's destructive reapplication guard without SDK sources."""
import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / "scripts/compiler/pad_div_erratum_nops.py"
SPEC = importlib.util.spec_from_file_location("div_padding", SCRIPT)
PADDING = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(PADDING)


class DivisionPaddingGuardTests(unittest.TestCase):
    def test_reapplication_refused_without_modifying_transformed_file(self):
        # A structural fixture exercises the patch anchors; it is not a
        # compiler source or evidence of instruction-level matching.
        original = "\n".join((
            PADDING.STATE_ANCHOR + " int unused));",
            PADDING.HELPER_ANCHOR,
            "append_insn (place, ip, address_expr, reloc_type, unmatched_hi)\n{\n  int nops = 0;\n",
            PADDING.NOP_EMIT_ANCHOR,
            PADDING.PAD_ANCHOR,
            "for (l = insn_labels; l != NULL; l = l->next)\n{}\n",
            PADDING.COUNT_ANCHOR + "}\nstatic void\nmips_emit_delays",
            PADDING.LABEL_ANCHOR,
        ))
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "tc-mips.c"
            path.write_text(original, encoding="utf-8", newline="")
            first = subprocess.run([sys.executable, str(SCRIPT), str(path)],
                                   capture_output=True, text=True)
            self.assertEqual(first.returncode, 0, first.stderr)
            transformed = path.read_bytes()
            self.assertEqual(transformed.count(b"static int\nrac2_div_erratum_p"), 1)
            for flags in ([], ["-O"]):
                again = subprocess.run([sys.executable, *flags, str(SCRIPT), str(path)],
                                       capture_output=True, text=True)
                self.assertNotEqual(again.returncode, 0)
                self.assertIn("already transformed", again.stderr)
                self.assertEqual(path.read_bytes(), transformed)


if __name__ == "__main__":
    unittest.main()
