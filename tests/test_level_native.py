import copy
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
import level_native as native
import integration
import decomp_report


class NativeTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.home = Path(temporary.name)
        self.root = self.home / "sources"
        self.root.mkdir()
        self.level = "0_test"
        self.source, self.catalog_path, self.review_path = native.paths(self.level)
        self.reference = self.home / "reference.elf"
        self.reference.write_bytes(b"synthetic reference, no retail bytes")
        self.tools = {name: "a" * 64 for name in ("cc1", "cpp", "as", "ld.exe")}
        self.target = {"serial": "SCUS_972.68"}
        self.overlays = {"target": self.target["serial"], "levels": [
            {"level": self.level, "sha256": native.file_hash(self.reference)},
            {"level": "1_other", "sha256": "c" * 64}]}
        self.boot = {"target": self.target["serial"], "flags": ["-O2", "-G0", "-ffunction-sections"],
                     "functions": [{"symbol": "FUN_00001000", "address": 0x1000, "size": 8}]}
        self.fn = {"symbol": native.symbol(self.level, 0x2000), "address": 0x2000, "size": 8}
        self.catalog = {"schema": 1, "kind": "level-native-catalog", "target": self.target["serial"],
                        "program": "levels/" + self.level, "level": self.level, "entry": 0x2000,
                        "reference_sha256": native.file_hash(self.reference), "source": self.source,
                        "flags": self.boot["flags"], "functions": [self.fn], "externals": {}}
        self.shared_fn = {"symbol": "FUN_00001000", "address": 0x2010, "size": 8}
        self.placements = {"target": self.target["serial"], "levels": {
            self.level: {"reference_sha256": native.file_hash(self.reference), "functions": [self.shared_fn]},
            "1_other": {"reference_sha256": "c" * 64, "functions": [self.shared_fn]}}}
        self.write("config/target.json", self.target)
        self.write("config/overlays.json", self.overlays)
        self.write("config/candidate-catalog.json", self.boot)
        self.write("config/level-catalog.json", self.placements)
        self.write("progress/candidates.json", {"tools": self.tools, "object_sha256": "d" * 64})
        self.write(self.catalog_path, self.catalog)
        self.bytes(self.source, (f"int {self.fn['symbol']}(void) {{ return 1; }}\n").encode())
        self.bytes("candidates/boot.c", b"int FUN_00001000(void) { return 0; }\n")
        for file in ("level_native.py", "check_candidates.py", "elf_tools.py", "wsl_chain.py"):
            self.bytes("scripts/" + file, (ROOT / "scripts" / file).read_bytes())
        self.object = self.home / "native.o"
        self.object.write_bytes(b"synthetic object")
        self.review = {"schema": 1, "kind": "level-native-candidate", "target": self.target["serial"],
                       "program": "levels/" + self.level, "reference_sha256": native.file_hash(self.reference),
                       "reference_entry": 0x2000, "candidate_source": self.source,
                       "source_sha256": native.file_hash(self.root / self.source),
                       "catalog_sha256": native.file_hash(self.root / self.catalog_path),
                       "checker_sha256": native.checker_hash(self.root), "flags": self.boot["flags"],
                       "tools": self.tools, "object_sha256": native.file_hash(self.object),
                       "candidate_elf_sha256": "b" * 64, "state": "matched_unintegrated",
                       "functions": [{**self.fn, "matched": True, "produced_size": 8, "different_bytes": 0,
                                      "reference_sha256": "b" * 64, "candidate_sha256": "b" * 64}],
                       "matched_code_bytes": 8}
        self.write(self.review_path, self.review)

    def bytes(self, path, content):
        p = self.root / path
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(content)

    def write(self, path, content):
        self.bytes(path, (json.dumps(content) + "\n").encode())

    def load(self):
        return native.load_catalog(self.level, self.root)

    def test_valid_review(self):
        native.validate_review(self.review, self.load(), self.level, self.root)

    def test_small_data_native_profile_keeps_boot_default_and_binds_review(self):
        self.catalog["flags"] = ["-O2", "-G8", "-ffunction-sections"]
        self.write(self.catalog_path, self.catalog)
        loaded = self.load()
        self.assertEqual(self.boot["flags"], ["-O2", "-G0", "-ffunction-sections"])
        with self.assertRaisesRegex(ValueError, "identity, source, catalog or checker"):
            native.validate_review(self.review, loaded, self.level, self.root)
        self.review["flags"] = loaded["flags"]
        self.review["catalog_sha256"] = loaded["_catalog_sha256"]
        native.validate_review(self.review, loaded, self.level, self.root)
        self.review["functions"][0]["different_bytes"] = 1
        with self.assertRaises(ValueError):
            native.validate_review(self.review, loaded, self.level, self.root)

    def test_other_native_flag_variants_are_rejected(self):
        for flags in (["-O2", "-G4", "-ffunction-sections"],
                      ["-O1", "-G8", "-ffunction-sections"],
                      ["-O2", "-G8"], None):
            with self.subTest(flags=flags):
                self.catalog["flags"] = flags
                self.write(self.catalog_path, self.catalog)
                with self.assertRaisesRegex(ValueError, "compiler flags"):
                    self.load()

    def test_wrong_program_and_boot_catalog_rejected(self):
        for field, value in (("program", "boot"), ("kind", "boot-catalog"), ("reference_sha256", "d" * 64)):
            with self.subTest(field=field):
                broken = {**self.catalog, field: value}
                self.write(self.catalog_path, broken)
                with self.assertRaises(ValueError): self.load()

    def test_namespace_prevents_same_address_boot_overlay_collision(self):
        self.assertNotEqual(native.symbol("0_test", 0x2000), native.symbol("1_other", 0x2000))
        self.catalog["functions"] = [{**self.fn, "symbol": "FUN_00002000"}]
        self.write(self.catalog_path, self.catalog)
        with self.assertRaisesRegex(ValueError, "program and entry"): self.load()

    def test_overlap_is_rejected(self):
        self.catalog["functions"] += [{"symbol": native.symbol(self.level, 0x2004), "address": 0x2004, "size": 8}]
        self.write(self.catalog_path, self.catalog)
        with self.assertRaisesRegex(ValueError, "overlapping"): self.load()

    def test_partial_symbols_or_missing_object_hash_rejected(self):
        for change in ("partial", "missing", "hashes", "counter"):
            proof = copy.deepcopy(self.review)
            if change == "partial": proof["functions"][0]["produced_size"] = 4
            if change == "missing": proof.pop("object_sha256")
            if change == "hashes": proof["functions"][0]["candidate_sha256"] = "f" * 64
            if change == "counter": proof["matched_code_bytes"] = 4
            with self.subTest(change=change), self.assertRaises(ValueError):
                native.validate_review(proof, self.load(), self.level, self.root)

    def test_boot_object_proof_cannot_be_used_for_native(self):
        proof = {**self.review, "program": "boot", "candidate_source": "candidates/boot.c"}
        with self.assertRaises(ValueError): native.validate_review(proof, self.load(), self.level, self.root)

    def test_source_and_catalog_changes_invalidate_review(self):
        self.bytes(self.source, b"changed source")
        with self.assertRaises(ValueError): native.validate_review(self.review, self.load(), self.level, self.root)

    def test_unpinned_header_or_time_dependency_rejected(self):
        for source in (b'#include "local.h"\n', b'const char *s=__DATE__;\n'):
            self.bytes(self.source, source)
            with self.assertRaisesRegex(ValueError, "standalone"): self.load()

    def test_reference_hash_entry_and_executable_owner(self):
        structure = {"type": 2, "entry": 0x2000, "sections": [{"type": 1, "flags": 6, "address": 0x2000, "size": 64}]}
        with mock.patch.object(native, "read_elf", return_value=structure):
            native.validate_reference(self.reference, self.load())
            for field, value in (("entry", 0x2004), ("type", 1)):
                with self.subTest(field=field), mock.patch.object(native, "read_elf", return_value={**structure, field: value}):
                    with self.assertRaises(ValueError): native.validate_reference(self.reference, self.load())
            bad = {**structure, "sections": [{"type": 1, "flags": 2, "address": 0x2000, "size": 64}]}
            with mock.patch.object(native, "read_elf", return_value=bad), self.assertRaises(ValueError):
                native.validate_reference(self.reference, self.load())
        self.reference.write_bytes(b"wrong program ELF")
        with self.assertRaisesRegex(ValueError, "pinned overlay"): native.validate_reference(self.reference, self.load())

    def test_recompiled_object_must_equal_reviewed_object(self):
        changed = self.home / "changed.o"
        changed.write_bytes(b"different source/object pair")
        with mock.patch.object(native, "tool_hashes", return_value=self.tools), \
                mock.patch.object(native, "qualify", return_value=(self.load(), changed, self.review)):
            with self.assertRaisesRegex(ValueError, "source/object pair"):
                native.compile_reviewed(self.reference, self.home / "build", self.home / "tools", self.level, self.root)

    def test_source_change_after_compile_is_rechecked(self):
        def changed(*args):
            catalog = self.load(); self.bytes(self.source, b"source changed during compiler run")
            return catalog, self.object, self.review
        with mock.patch.object(native, "tool_hashes", return_value=self.tools), mock.patch.object(native, "qualify", side_effect=changed):
            with self.assertRaises(ValueError):
                native.compile_reviewed(self.reference, self.home / "build", self.home / "tools", self.level, self.root)

    def test_only_affected_level_cache_is_invalidated(self):
        before = native.dependencies(self.level, self.root)
        other_source = native.paths("1_other")[0]
        self.bytes(other_source, b"other level source changed")
        self.assertEqual(before, native.dependencies(self.level, self.root))
        self.bytes(self.source, b"this level changed")
        self.assertNotEqual(before, native.dependencies(self.level, self.root))

    def integrated(self):
        gate = {"matched": True, "bytes_compared": 64, "segments": 1}
        shared_row = {**self.shared_fn, "matched": True, "integrated": True, "program": self.level,
                      "candidate_source": "candidates/boot.c", "origin": "boot-shared"}
        native_row = {**self.review["functions"][0], "integrated": True, "program": self.level,
                      "candidate_source": self.source, "origin": "level-native"}
        integration_proof = {"source_sha256": native.file_hash(self.root / "candidates/boot.c"), "tools": self.tools}
        shared = {"target": self.target["serial"], "program": self.level, "reference_sha256": native.file_hash(self.reference),
                  "candidate_source": "candidates/boot.c", "source_sha256": integration_proof["source_sha256"],
                  "catalog_sha256": native.file_hash(self.root / "config/level-catalog.json"),
                  "state": "integrated", "full_level_gate": gate, "matched_code_bytes": 8, "tools": self.tools,
                  "c_object_sha256": "d" * 64, "candidate_elf_sha256": "e" * 64}
        proof = {**shared, "schema": 2, "kind": "level-c-integration", "functions": [shared_row, native_row],
                 "reconstruction_tools": {"Ps2EeAs.exe": "a" * 64, "ld.exe": "b" * 64},
                 "reference_entry": 0x2000, "candidate_entry": 0x2000, "matched_code_bytes": 16,
                 "dependency_sha256": native.dependencies(self.level, self.root), "shared": shared,
                 "boot_review_sha256": native.file_hash(self.root / "progress/candidates.json"),
                 "native": {"source": self.source, "catalog_path": self.catalog_path, "review_path": self.review_path,
                            "review_sha256": native.file_hash(self.root / self.review_path),
                            "object_sha256": self.review["object_sha256"], "object_qualification": self.review}}
        progress = {"target": self.target["serial"], "tools": proof["reconstruction_tools"], "g3": [{"level": self.level, "matched": True,
                    "reference_sha256": native.file_hash(self.reference), "bytes_compared": 64,
                    "integrated_c_functions": 2, "integrated_c_bytes": 16}]}
        return proof, progress, integration_proof

    def validate_integration(self, proof, progress, integration_proof):
        with mock.patch.object(decomp_report, "ROOT", self.root):
            return decomp_report.validate_native_level_proof(proof, self.target, self.overlays, progress,
                integration_proof, (self.root / "config/level-catalog.json").read_bytes(), self.boot)

    def test_native_and_shared_union_is_valid(self):
        proof, progress, boot = self.integrated()
        results = self.validate_integration(proof, progress, boot)
        self.assertEqual(sum(x["size"] for x in results), 16)
        self.assertEqual({x["candidate_source"] for x in results}, {"candidates/boot.c", self.source})

    def test_cross_source_overlap_is_rejected(self):
        proof, progress, boot = self.integrated()
        proof["functions"][0]["address"] = self.fn["address"]
        with self.assertRaisesRegex(ValueError, "overlapping"): self.validate_integration(proof, progress, boot)

    def test_explicit_review_is_bound_by_hash(self):
        proof, progress, boot = self.integrated()
        proof["boot_review_sha256"] = "f" * 64
        with self.assertRaisesRegex(ValueError, "different boot review"):
            self.validate_integration(proof, progress, boot)

    def test_reconstruction_instruments_are_required(self):
        original, progress, boot = self.integrated()
        for tools in (None, {"ld.exe": "b" * 64}, {"Ps2EeAs.exe": "invalid", "ld.exe": "b" * 64},
                      {"Ps2EeAs.exe": "f" * 64, "ld.exe": "b" * 64}):
            proof = copy.deepcopy(original)
            proof["reconstruction_tools"] = tools
            with self.subTest(tools=tools), self.assertRaises(ValueError):
                self.validate_integration(proof, progress, boot)

    def test_mixed_inputs_link_to_their_own_objects(self):
        directory = self.home / "mixed-build"
        (directory / "config").mkdir(parents=True)
        (directory / "asm_pp").mkdir()
        def body(address):
            return f".globl func_{address:08X}\nfunc_{address:08X}:\n" + "".join(
                f" /* 00000 {address+n:08X} 00000000 */ nop\n" for n in (0,4))
        source = directory / "asm_pp/text.s"
        source.write_text(integration.HEADER+body(0x2000)+body(0x2010))
        (directory / "config/rac2.ld").write_text("SECTIONS { .text 0x2000 : AT(0x1000) { build/asm/text.s.o(.text); } }")
        objects = {self.source: directory / "build/c/native.o", "candidates/boot.c": directory / "build/c/boot.o"}
        functions = [{**self.fn,"candidate_source":self.source},
                     {**self.shared_fn,"candidate_source":"candidates/boot.c"}]
        integration.replace_inputs(directory,[source],{"functions":functions},objects,relocated=True)
        linker=(directory / "config/rac2.ld").read_text()
        self.assertIn(f"build/c/native.o(.text.{self.fn['symbol']})",linker)
        self.assertIn("build/c/boot.o(.text.FUN_00001000)",linker)
        self.assertIn("AT(0x1000)",linker)

    def compile_with_promoted_helper(self, bound_address=0x2020, corrupt_link=False, corrupt_receipt=False):
        shared_object = self.home / "shared.o"
        shared_object.write_bytes(b"synthetic shared object")
        self.write("progress/candidates.json", {
            "tools": self.tools, "object_sha256": native.file_hash(shared_object)})
        promoted = {"symbol": native.symbol(self.level, 0x2020), "address": 0x2020, "size": 8}
        native_catalog = {**self.catalog, "gp": 0,
                          "externals": {promoted["symbol"]: bound_address, "UnownedData": 0x3000}}
        sd_source, _, sd_review = native.paths(self.level, "smalldata")
        self.write(sd_review, {"tools": self.tools})
        sd_object = self.home / "smalldata.o"
        sd_object.write_bytes(b"synthetic small-data object")
        sd_catalog = {"functions": [promoted], "externals": {"OtherData": 0x3004}}
        sd_proof = {"tools": self.tools, "source_sha256": "e" * 64}
        link_directory = self.home / "build"
        link_directory.mkdir()
        shared_object = link_directory / "shared-raw.o"
        shared_object.write_bytes(b"synthetic shared object")
        link_object = link_directory / "shared-link.o"
        link_object.write_bytes(b"synthetic derived link object")
        adapter_path = link_directory / "shared-link-adapter.json"
        adapter_path.write_text(json.dumps({"compiled_object_sha256": native.file_hash(shared_object),
            "link_object_sha256": native.file_hash(link_object), "relocation_changes": []}))
        descriptor = {"path": link_object.name, "sha256": native.file_hash(link_object),
            "adapter_path": adapter_path.name, "adapter_sha256": native.file_hash(adapter_path),
            "compiled_object_sha256": native.file_hash(shared_object),
            "compiled_path": shared_object.name}
        if corrupt_link: link_object.write_bytes(b"changed derived object")
        if corrupt_receipt: adapter_path.write_bytes(b"changed receipt")
        shared = {"functions": [self.shared_fn], "externals": {},
                  "compiled_source_sha256": native.file_hash(self.root / "candidates/boot.c"),
                  "shared_link_object": descriptor}
        with mock.patch.object(integration, "ROOT", self.root), \
                mock.patch.object(integration, "_compile_shared_level_c",
                                  return_value=(shared, shared_object, self.tools)), \
                mock.patch.object(native, "compile_reviewed", side_effect=[
                    (native_catalog, self.object, self.review), (sd_catalog, sd_object, sd_proof)]), \
                mock.patch.object(native, "has_smalldata", return_value=True), \
                mock.patch.object(native, "dependencies", return_value="f" * 64):
            combined, objects, tools = integration.compile_level_c(
                self.reference, self.home / "build", self.home / "tools", self.level)
        self.assertEqual(objects["candidates/boot.c"], link_object)
        self.assertNotEqual(objects["candidates/boot.c"], shared_object)
        self.assertEqual(native.file_hash(shared_object), json.loads(
            (self.root / "progress/candidates.json").read_bytes())["object_sha256"])
        return combined, promoted

    def test_full_level_route_rejects_changed_derived_object(self):
        with self.assertRaisesRegex(ValueError, "derived link object or adapter receipt changed"):
            self.compile_with_promoted_helper(corrupt_link=True)

    def test_full_level_route_rejects_changed_adapter_receipt(self):
        with self.assertRaisesRegex(ValueError, "derived link object or adapter receipt changed"):
            self.compile_with_promoted_helper(corrupt_receipt=True)

    def test_promoted_small_data_helper_keeps_its_function_definition(self):
        combined, promoted = self.compile_with_promoted_helper()
        self.assertNotIn(promoted["symbol"], combined["externals"])
        self.assertEqual(combined["externals"], {"UnownedData": 0x3000, "OtherData": 0x3004})
        self.assertEqual(len(combined["functions"]), 3)
        directory = self.home / "link"
        (directory / "config").mkdir(parents=True)
        script = directory / "config/undefined_symbols.ld"
        script.write_text("func_00002020 = 0x2020;\n", encoding="ascii")
        integration.add_definitions(directory, combined)
        text = script.read_text(encoding="ascii")
        self.assertIn(f"func_00002020 = {promoted['symbol']};", text)
        self.assertNotIn(f"{promoted['symbol']} =", text)
        self.assertIn("UnownedData = 0x00003000;", text)

    def test_promoted_small_data_helper_rejects_a_conflicting_import(self):
        with self.assertRaisesRegex(ValueError, "External disagrees with an integrated definition"):
            self.compile_with_promoted_helper(bound_address=0x2030)

    def test_full_loaded_gate_and_derived_counts_are_mandatory(self):
        original, progress, boot = self.integrated()
        for change in ("gate", "total", "object", "provenance", "partial", "program", "shared-object"):
            proof = copy.deepcopy(original)
            if change == "gate": proof["shared"]["full_level_gate"]["matched"] = False
            if change == "total": proof["matched_code_bytes"] = 8
            if change == "object": proof["native"]["object_sha256"] = "f" * 64
            if change == "provenance": proof["functions"][0]["candidate_source"] = self.source
            if change == "partial": proof["functions"][1]["size"] = 4
            if change == "program": proof["program"] = "boot"
            if change == "shared-object": proof["shared"]["c_object_sha256"] = "f" * 64
            with self.subTest(change=change), self.assertRaises(ValueError):
                self.validate_integration(proof, progress, boot)


if __name__ == "__main__": unittest.main()
