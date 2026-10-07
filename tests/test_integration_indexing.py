"""Keep strict assembly ownership checks intact when definitions are indexed."""
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import integration


def insn(address, operation="nop"):
    return f"    /* 00000 {address:08X} 00000000 */ {operation}\n"


def body(address):
    return f".globl func_{address:08X}\nfunc_{address:08X}:\n" + insn(address) + insn(address + 4)


class IndexedAssemblyOwnershipTests(unittest.TestCase):
    def test_duplicate_assembly_definitions_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "Expected one original assembly definition"):
            integration.split_assembly(body(0x1000) + body(0x1000), [
                {"symbol": "FUN_00001000", "address": 0x1000, "size": 8}])

    def test_a_longer_symbol_is_not_an_original_definition(self):
        content = body(0x1000).replace("func_00001000", "func_00001000_extra")
        with self.assertRaisesRegex(ValueError, "Expected one original assembly definition"):
            integration.split_assembly(content, [
                {"symbol": "FUN_00001000", "address": 0x1000, "size": 8}])

    def test_cross_object_labels_keep_distinct_prefixes_and_local_ownership(self):
        content = (insn(0xFF0, "lw $2,%lo(.L00002000)($3)")
                   + insn(0xFF4, "lw $2,%lo(L000020000)($3)")
                   + ".L00ABC:\n" + insn(0xFF8, "b .L00ABC")
                   + body(0x1000)
                   + ".L00002000:\n" + insn(0x1010)
                   + "L000020000:\n" + insn(0x1014))
        pieces = integration.split_assembly(content, [
            {"symbol": "FUN_00001000", "address": 0x1000, "size": 8}])
        self.assertEqual([piece["kind"] for piece in pieces], ["asm", "c", "asm"])
        left, right = pieces[0]["content"], pieces[2]["content"]
        self.assertIn("%lo(XL_00002000)", left)
        self.assertIn("%lo(XL_000020000)", left)
        self.assertIn(".L00ABC:", left)
        self.assertIn("b .L00ABC", left)
        self.assertNotIn(".globl XL_00ABC", left + right)
        for label in ("00002000", "000020000"):
            self.assertEqual(right.count(f".globl XL_{label}\n"), 1)
            self.assertIn(f"XL_{label}:", right)
            self.assertNotIn(f".globl XL_{label}\n", left)


if __name__ == "__main__":
    unittest.main()
