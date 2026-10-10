"""Check the save-block transformer emits the stock order and refuses reapplying.

The fixture is a structural stand-in for the two emission blocks of
`save_restore_insns`; it carries the anchors the transformer searches for and
nothing else. It is not a compiler source and is not evidence of
instruction-level matching.
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest


SCRIPT = Path(__file__).resolve().parents[1] / "scripts/compiler/reorder_save_blocks.py"


def fixture() -> str:
    """The two intact blocks, in the order the released source emits them."""
    return "".join((
        "void\nsave_restore_insns (store_p, ...)\n{\n",
        "  /* Save GP registers if needed.  */\n",
        "  if (mask)\n    {\n",
        "      gp_offset = current_frame_info.gp_sp_offset;\n",
        "      for (regno = GP_REG_FIRST; regno <= GP_REG_LAST; regno++)\n",
        "\tif (BITSET_P (mask, regno - GP_REG_FIRST))\n",
        "\t  {\n\t    emit_gp_store (regno, gp_offset);\n\t  }\n",
        "    }\n\n",
        "  /* Save floating point registers if needed.  */\n",
        "  if (fmask)\n    {\n",
        "      for (regno = FP_REG_LAST; regno >= FP_REG_FIRST;\n",
        "\t   regno -= fp_inc)\n",
        "\tif (BITSET_P (fmask, regno - FP_REG_FIRST))\n",
        "\t  {\n\t    emit_fp_store (regno, fp_offset);\n\t  }\n",
        "    }\n",
        "}\n\f\n",
        "void\nnext_function ()\n{\n}\n",
    ))


def run(path: Path) -> subprocess.CompletedProcess:
    return subprocess.run([sys.executable, "-I", str(SCRIPT), str(path)],
                          capture_output=True, text=True)


class SaveBlockOrderTests(unittest.TestCase):
    def transform(self, source: str) -> str:
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mips.c"
            path.write_text(source, encoding="utf-8", newline="")
            result = run(path)
            self.assertEqual(result.returncode, 0, result.stderr)
            return path.read_text(encoding="utf-8")

    def test_pass_zero_runs_the_gp_block_and_pass_one_the_fp_block(self):
        # The selector must be `rac2_save_pass != 0`: its false branch (pass 0)
        # is the GP block and its true branch (pass 1) is the FP block, in both
        # directions -- the order the stock source and the retail image use.
        out = self.transform(fixture())
        self.assertNotIn("(store_p != 0)", out)
        selector = out.index("if (rac2_save_pass != 0)")
        fp = out.index("emit_fp_store (regno, fp_offset);", selector)
        else_at = out.index("}\n        else\n          {", selector)
        gp = out.index("emit_gp_store (regno, gp_offset);", else_at)
        self.assertLess(selector, fp)
        self.assertLess(fp, else_at)
        self.assertLess(else_at, gp)

    def test_both_blocks_and_the_shared_base_survive(self):
        out = self.transform(fixture())
        self.assertIn("for (regno = GP_REG_FIRST; regno <= GP_REG_LAST; regno++)", out)
        self.assertIn("regno -= fp_inc)", out)
        self.assertIn("base_reg_rtx = 0;", out)
        self.assertIn("base_offset = 0;", out)
        self.assertEqual(out.count("rac2_save_pass"), 5)
        self.assertEqual(out.count("int rac2_save_pass;"), 1)
        # the region was rebuilt, everything outside it is untouched
        self.assertIn("void\nnext_function ()\n{\n}\n", out)
        self.assertIn("void\nsave_restore_insns (store_p, ...)\n{\n", out)

    def test_reapplication_is_refused_without_modifying_the_file(self):
        once = self.transform(fixture())
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mips.c"
            path.write_text(once, encoding="utf-8", newline="")
            for flags in ([], ["-O"], ["-OO"]):
                again = subprocess.run([sys.executable, *flags, str(SCRIPT), str(path)],
                                       capture_output=True, text=True)
                self.assertNotEqual(again.returncode, 0)
                self.assertEqual(path.read_text(encoding="utf-8"), once)

    def test_a_missing_anchor_stops_the_transform(self):
        broken = fixture().replace("  /* Save floating point registers if needed.  */",
                                   "  /* nothing here */")
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "mips.c"
            path.write_text(broken, encoding="utf-8", newline="")
            result = run(path)
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual(path.read_text(encoding="utf-8"), broken)

    def test_the_generated_annotation_names_the_emitted_order(self):
        out = self.transform(fixture())
        self.assertRegex(out, re.compile(r"RAC2 : GP block first in both directions"))
        self.assertNotIn("FPR saves precede GPR saves", out)
        # the two words that name the block inside the annotation moved with it
        self.assertIn("first GP pass needs the base initialized", out)
        self.assertNotIn("first FPR pass needs the base initialized", out)

    def test_each_block_is_emitted_exactly_once(self):
        out = self.transform(fixture())
        self.assertEqual(out.count("emit_gp_store (regno, gp_offset);"), 1)
        self.assertEqual(out.count("emit_fp_store (regno, fp_offset);"), 1)
        self.assertEqual(out.count("gp_offset = current_frame_info.gp_sp_offset;"), 1)


if __name__ == "__main__":
    unittest.main()
