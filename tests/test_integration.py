import importlib.util
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
SPEC = importlib.util.spec_from_file_location("integration", ROOT / "scripts" / "integration.py")
integration = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(integration)


def instruction(address, operation="nop"):
    return f"    /* 00000 {address:08X} 00000000 */ {operation}\n"


def original_function(address, size):
    symbol = f"func_{address:08X}"
    return f".globl {symbol}\n{symbol}:\n" + "".join(
        instruction(address + offset) for offset in range(0, size, 4))


def function(address=0x1000, size=8):
    return {"symbol": f"FUN_{address:08X}", "address": address, "size": size}


class IntegrationTests(unittest.TestCase):
    def test_source_change_after_snapshot_blocks_promotion(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        source = directory / "boot.c"
        source.write_bytes(b"changed source")
        catalog = {"compiled_source_sha256": hashlib.sha256(b"reviewed source").hexdigest()}
        with self.assertRaisesRegex(ValueError, "source changed"):
            integration.validate_integrated(directory / "reference.elf", directory / "candidate.elf", catalog, source)

    def test_unmatched_candidate_never_reaches_compiler(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        (directory / "config").mkdir()
        (directory / "progress").mkdir()
        (directory / "candidates").mkdir()
        reference = directory / "reference.elf"
        reference.write_bytes(b"synthetic reference")
        source = directory / "candidates" / "boot.c"
        source.write_bytes(b"void function(void) {}")
        catalog = {"reference_sha256": integration.file_hash(reference), "functions": [function()]}
        catalog_path = directory / "config" / "candidate-catalog.json"
        catalog_path.write_text(json.dumps(catalog))
        candidate = {"source_sha256": integration.file_hash(source),
                     "catalog_sha256": integration.file_hash(catalog_path),
                     "functions": [{**function(), "matched": False}]}
        (directory / "progress" / "candidates.json").write_text(json.dumps(candidate))
        with mock.patch.object(integration, "ROOT", directory), mock.patch.object(integration, "run") as compiler:
            with self.assertRaisesRegex(ValueError, "complete candidate match"):
                integration.compile_c(reference, directory, directory / "tools")
            compiler.assert_not_called()

    def test_only_reviewed_body_is_removed_and_padding_remains(self):
        content = original_function(0x1000, 8) + instruction(0x1008) + original_function(0x1010, 8)
        pieces = integration.split_assembly(content, [function()])
        self.assertEqual(pieces[0]["kind"], "c")
        remainder = "".join(piece.get("content", "") for piece in pieces)
        self.assertNotIn("func_00001000:", remainder)
        self.assertNotIn("00001004", remainder)
        self.assertIn("00001008", remainder)
        self.assertIn("func_00001010:", remainder)

    def test_fragments_and_c_inputs_preserve_original_order(self):
        content = original_function(0x0FF8, 8) + original_function(0x1000, 8) + instruction(0x1008)
        pieces = integration.split_assembly(content, [function()])
        self.assertEqual([piece["kind"] for piece in pieces], ["asm", "c", "asm"])

    def test_duplicate_and_overlapping_ranges_are_rejected(self):
        content = original_function(0x1000, 12) + original_function(0x1008, 8)
        for selected in ([function(), function()], [function(size=12), function(0x1008)]):
            with self.subTest(selected=selected), self.assertRaises(ValueError):
                integration.split_assembly(content, selected)

    def test_wrong_symbol_or_missing_label_is_rejected(self):
        content = original_function(0x1000, 8)
        selected = function()
        selected["symbol"] = "another_function"
        with self.assertRaises(ValueError):
            integration.split_assembly(content, [selected])
        with self.assertRaises(ValueError):
            integration.split_assembly(content.replace("func_00001000:", "other:"), [function()])

    def test_unknown_inner_label_is_not_silently_deleted(self):
        content = original_function(0x1000, 8).replace(instruction(0x1004), "local_label:\n" + instruction(0x1004))
        with self.assertRaises(ValueError):
            integration.split_assembly(content, [function()])

    def test_disassembly_gap_and_truncated_body_are_rejected(self):
        content = original_function(0x1000, 8)
        with self.assertRaises(ValueError):
            integration.split_assembly(content.replace("00001004", "00001008"), [function()])
        with self.assertRaises(ValueError):
            integration.split_assembly(content.removesuffix(instruction(0x1004)), [function()])

    def test_relocated_placement_keeps_the_reviewed_symbol(self):
        content = original_function(0x1000, 8)
        placement = {"symbol": "FUN_00120BC8", "address": 0x1000, "size": 8}
        # A non-address boot alias must first identify a qualified SDK owner.
        with self.assertRaisesRegex(ValueError, "Unknown source-specific SDK unit"):
            integration.split_assembly(content, [placement])
        pieces = integration.split_assembly(content, [placement], relocated=True)
        self.assertEqual([piece["kind"] for piece in pieces], ["c"])
        self.assertEqual(pieces[0]["function"]["symbol"], "FUN_00120BC8")

    def level_document(self, functions, excluded=None):
        return {"target": "SCUS_972.68", "source": "candidates/boot.c",
                "levels": {"0_aranos_tutorial": {"reference_sha256": "a" * 64, "functions": functions,
                                                 "excluded": excluded or []}}}

    def level_directory(self, functions):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        (directory / "config").mkdir()
        (directory / "config" / "candidate-catalog.json").write_text(json.dumps(
            {"target": "SCUS_972.68", "flags": ["-O2"],
             "functions": [{"symbol": "FUN_00120BC8", "address": 0x120BC8, "size": 8},
                           {"symbol": "FUN_00115200", "address": 0x115200, "size": 8}]}))
        (directory / "config" / "overlays.json").write_text(json.dumps(
            {"levels": [{"level": "0_aranos_tutorial", "sha256": "a" * 64}]}))
        (directory / "config" / "level-catalog.json").write_text(json.dumps(self.level_document(functions)))
        return directory

    def test_level_catalog_only_places_reviewed_bodies_once(self):
        placement = {"symbol": "FUN_00120BC8", "address": 0x2A70E4, "size": 8}
        directory = self.level_directory([placement])
        with mock.patch.object(integration, "ROOT", directory):
            catalog = integration.level_catalog("0_aranos_tutorial")
        self.assertEqual(catalog["functions"], [placement])
        self.assertNotIn("gp", catalog)
        elsewhere = {"symbol": "FUN_00115200", "address": 0x2A70E8, "size": 8}
        broken = {
            "unknown symbol": [{"symbol": "FUN_99999999", "address": 0x2A70E4, "size": 8}],
            "wrong size": [{**placement, "size": 12}],
            "duplicate symbol": [placement, {**placement, "address": 0x2A7100}],
            "duplicate address": [placement, {**elsewhere, "address": placement["address"]}],
            "unaligned address": [{**placement, "address": 0x2A70E2}],
            "empty placement": [],
        }
        for label, functions in broken.items():
            with self.subTest(label=label), mock.patch.object(integration, "ROOT", directory):
                (directory / "config" / "level-catalog.json").write_text(
                    json.dumps(self.level_document(functions)))
                with self.assertRaises(ValueError):
                    integration.level_catalog("0_aranos_tutorial")
        with mock.patch.object(integration, "ROOT", directory):
            (directory / "config" / "level-catalog.json").write_text(json.dumps(self.level_document([placement])))
            with self.assertRaisesRegex(ValueError, "No reviewed C placement catalog"):
                integration.level_catalog("1_oozla")
            (directory / "config" / "overlays.json").write_text(json.dumps(
                {"levels": [{"level": "0_aranos_tutorial", "sha256": "b" * 64}]}))
            with self.assertRaisesRegex(ValueError, "pinned overlay identity"):
                integration.level_catalog("0_aranos_tutorial")

    def test_level_placement_never_invents_a_gp_base(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        (directory / "config").mkdir()
        path = directory / "config" / "undefined_symbols.ld"
        path.write_text("func_002A70E4 = 0x0;\n", encoding="ascii")
        integration.add_definitions(directory, {"functions": [{"symbol": "FUN_00120BC8", "address": 0x2A70E4,
                                                              "size": 8}], "externals": {}})
        content = path.read_text(encoding="ascii")
        self.assertIn("func_002A70E4 = FUN_00120BC8;", content)
        self.assertNotIn("_gp", content)

    def test_shipped_level_catalog_matches_the_reviewed_bodies(self):
        catalog = json.loads((ROOT / "config" / "level-catalog.json").read_text(encoding="utf-8"))
        boot = json.loads((ROOT / "config" / "candidate-catalog.json").read_text(encoding="utf-8"))
        overlays = json.loads((ROOT / "config" / "overlays.json").read_text(encoding="utf-8"))
        reviewed = {function["symbol"]: function["size"] for function in boot["functions"]}
        pinned = {entry["level"]: entry["sha256"] for entry in overlays["levels"]}
        self.assertTrue(catalog["levels"])
        for level, entry in catalog["levels"].items():
            with self.subTest(level=level):
                self.assertEqual(entry["reference_sha256"], pinned[level])
                ordered = sorted(entry["functions"], key=lambda function: function["address"])
                addresses = [function["address"] for function in ordered]
                self.assertEqual(len(addresses), len(set(addresses)))
                for previous, current in zip(ordered, ordered[1:]):
                    self.assertLessEqual(previous["address"] + previous["size"], current["address"])
                for function in entry["functions"]:
                    self.assertEqual(function["size"], reviewed[function["symbol"]])
                for rejected in entry["excluded"]:
                    self.assertEqual(rejected["size"], reviewed[rejected["symbol"]])
                    self.assertGreater(len(rejected["reason"]), 20)

    def test_linker_places_compiled_sections_instead_of_original_object(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        directory = Path(temporary.name)
        (directory / "config").mkdir()
        (directory / "asm_pp").mkdir()
        (directory / "build" / "c").mkdir(parents=True)
        source = directory / "asm_pp" / "1234.s"
        source.write_text(integration.HEADER + original_function(0x1000, 8) + instruction(0x1008))
        script = directory / "config" / "rac2.ld"
        script.write_text("SECTIONS { .text : { build/asm/1234.s.o(.text); build/asm/1234.s.o(.data); } }")
        c_object = directory / "build" / "c" / "boot.c.o"
        result, replacements = integration.replace_inputs(directory, [source], {"functions": [function()]}, c_object)
        updated = script.read_text()
        self.assertNotIn("build/asm/1234.s.o", updated)
        self.assertIn("build/c/boot.c.o(.text.FUN_00001000)", updated)
        self.assertTrue(all(path != source for path in result))
        self.assertEqual(replacements, {"1234.s": ["FUN_00001000"]})


if __name__ == "__main__":
    unittest.main()
