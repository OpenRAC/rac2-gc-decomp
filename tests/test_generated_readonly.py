"""Full synthetic switch-table acceptance, including malformed placements."""
import copy
import hashlib
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import check_candidates as check
from test_elf_tools import synthetic_elf


class GeneratedReadonlyTests(unittest.TestCase):
    def setUp(self):
        temp = tempfile.TemporaryDirectory()
        self.addCleanup(temp.cleanup)
        self.home = Path(temp.name)
        self.table = b"\x04\x10\x00\x00\x08\x10\x00\x00"
        self.catalog = {"functions": [{"symbol": "TestSwitch", "address": 0x1000, "size": 12}],
                        "externals": {}, "gp": 0,
                        "read_only_sections": [{"section": ".rodata", "address": 0x2000,
                                                "size": 8, "sha256": hashlib.sha256(self.table).hexdigest()}]}
        self.reference = self.linked("reference.elf", self.table, section=".data", flags=3)
        self.candidate = self.linked("candidate.elf", self.table)
        self.obj = self.object()

    def linked(self, name, data, section=".rodata", flags=2):
        path = self.home / name
        path.write_bytes(synthetic_elf(segments=[{"address": 0x2000, "data": data, "flags": 6}],
                                       sections=[{"name": section, "address": 0x2000, "data": data,
                                                  "flags": flags, "alignment": 4}]))
        return path

    def object(self, flags=2, extra=False, size=8):
        sections = [{"name": ".rodata", "data": self.table, "size": size, "flags": flags, "alignment": 4}]
        if extra:
            sections.append({"name": ".data", "data": b"EXTRA", "flags": 3})
        path = self.home / "input.o"
        path.write_bytes(synthetic_elf(elf_type=1, sections=sections))
        return path

    def compare(self, candidate=None, obj=None):
        return check.compare_readonly(self.reference, candidate or self.candidate, self.catalog, obj or self.obj)

    def test_writable_reference_accepts_entire_generated_readonly_table(self):
        results = self.compare()
        check.require_exact_readonly(self.catalog, results)
        self.assertEqual(results[0]["different_bytes"], 0)

    def test_last_byte_difference_is_a_refusal(self):
        candidate = self.linked("last-byte.elf", self.table[:-1] + b"\x01")
        results = self.compare(candidate=candidate)
        self.assertFalse(results[0]["matched"])
        self.assertEqual(results[0]["different_bytes"], 1)
        with self.assertRaisesRegex(ValueError, "every generated readonly byte"):
            check.require_exact_readonly(self.catalog, results)

    def test_no_prefix_trim_or_additional_data(self):
        for options in ({"size": 4}, {"extra": True}):
            with self.subTest(options=options):
                results = self.compare(obj=self.object(**options))
                self.assertFalse(results[0]["matched"])

    def test_writable_generated_input_or_output_is_refused(self):
        self.assertFalse(self.compare(obj=self.object(flags=3))[0]["matched"])
        self.obj = self.object()
        self.assertFalse(self.compare(candidate=self.linked("writable.elf", self.table, flags=3))[0]["matched"])

    def test_invalid_identity_overlap_and_address_overflow(self):
        for changes in ({"section": ".data"}, {"address": 0x1004}, {"size": True},
                        {"address": 0xFFFFFFFC}, {"sha256": "not-a-pin"}):
            with self.subTest(changes=changes):
                catalog = copy.deepcopy(self.catalog)
                catalog["read_only_sections"][0].update(changes)
                with self.assertRaises(ValueError):
                    check.linker_script(catalog)

    def test_wrong_reference_pin_and_omitted_review_cannot_pass(self):
        self.catalog["read_only_sections"][0]["sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "reference pin"):
            self.compare()
        with self.assertRaisesRegex(ValueError, "omitted or duplicated"):
            check.require_exact_readonly(self.catalog, [])

    def test_catalogs_without_tables_keep_the_original_linker_layout(self):
        self.catalog.pop("read_only_sections")
        self.assertEqual(check.linker_script(self.catalog),
                         "ENTRY(TestSwitch)\nSECTIONS\n{\n"
                         "    .text.TestSwitch 0x00001000 : { *(.text.TestSwitch) }\n"
                         "    .data : { *(.data) *(.rodata) *(.rdata) *(.lit4) *(.lit8) *(.sdata) }\n"
                         "    .bss : { *(.bss) *(.sbss) *(COMMON) }\n"
                         "    /DISCARD/ : { *(.reginfo) }\n}\n_gp = 0x00000000;\n")
        self.assertEqual(check.compare_readonly(self.reference, self.candidate, self.catalog, self.obj), [])


if __name__ == "__main__":
    unittest.main()
