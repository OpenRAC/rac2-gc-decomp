"""The five maintained admission gates must distinguish C members from directives."""
import ast
from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[1]
SCRIPTS = ("campaign.py", "check_candidates.py", "level_native.py", "decomp_report.py", "family_candidates.py")


def guard(name):
    tree = ast.parse((ROOT / "scripts" / name).read_text(encoding="utf-8"))
    patterns = [node.args[0].value for node in ast.walk(tree)
                if isinstance(node, ast.Call) and isinstance(node.func, ast.Attribute)
                and node.func.attr in ("search", "compile") and node.args
                and isinstance(node.args[0], ast.Constant)
                and isinstance(node.args[0].value, (str, bytes))
                and "INCLUDE_ASM" in (node.args[0].value.decode() if isinstance(node.args[0].value, bytes) else node.args[0].value)]
    if len(patterns) != 1:
        raise AssertionError(f"Expected one maintained assembly policy in {name}")
    return re.compile(patterns[0].decode() if isinstance(patterns[0], bytes) else patterns[0])


class SourceAdmissionGuards(unittest.TestCase):
    def test_all_gates_accept_ordinary_c_members_and_designators(self):
        cases = ("(const char **)(void *)snapshot.bytes", "return object.word;", "return object.words;",
                 "return object.byte;", "return object.bytes;", "return object->word;",
                 "struct Item value = {\n .word = 7,\n .byte = 1\n};", ".word = 3", ".words = 4")
        for name in SCRIPTS:
            for source in cases:
                with self.subTest(gate=name, source=source):
                    self.assertIsNone(guard(name).search(source))

    def test_all_gates_refuse_real_assembly_and_exact_directive_names(self):
        cases = (".byte 1,2\n", "  .word 0x1234\n", "label: .word symbol + 4\n",
                 "1: .byte 7\n", ".byte\n", ".word\n", 'asm("nop");',
                 '__asm__("nop");', '__asm("nop");', 'INCLUDE_ASM("file.s", function);')
        for name in SCRIPTS:
            for source in cases:
                with self.subTest(gate=name, source=source):
                    self.assertIsNotNone(guard(name).search(source))


if __name__ == "__main__":
    unittest.main()
