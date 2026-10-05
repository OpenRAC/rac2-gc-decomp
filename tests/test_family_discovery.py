"""Pinned synthetic ELF regressions for metadata-only bounded discovery."""
from __future__ import annotations

import contextlib
import copy
import hashlib
import importlib
import io
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
family_discovery = importlib.import_module("family_discovery")


def words(*values):
    return struct.pack("<" + "I" * len(values), *values)


def synthetic_elf(body, *, section_name=".text", section_type=1, section_flags=6,
                  segment_flags=5, extra_section=False, extra_segment=False,
                  section_size=None, memsz=None, elf_type=2, offset_bias=0):
    # Section bytes and PT_LOAD bytes share the SAME file offsets, like retail.
    count = 2 if extra_segment else 1
    offset = 52 + 32 * count
    image = bytearray(offset)
    image.extend(body)
    names = b"\0" + section_name.encode() + b"\0.shstrtab\0"
    strings = len(image)
    image.extend(names)
    image.extend(bytes((-len(image)) % 4))
    sections_offset = len(image)
    headers = [(0,) * 10,
               (1, section_type, section_flags, 0x1000, offset + offset_bias,
                len(body) if section_size is None else section_size, 0, 0, 4, 0)]
    if extra_section:
        headers.append((1, 1, 6, 0x1004, offset+4, 4, 0, 0, 4, 0))
    names_index = len(headers)
    headers.append((len(section_name)+2, 3, 0, 0, strings, len(names), 0, 0, 1, 0))
    for header in headers:
        image.extend(struct.pack("<10I", *header))
    identification = b"\x7fELF\x01\x01\x01" + bytes(9)
    struct.pack_into("<16sHHIIIIIHHHHHH", image, 0, identification, elf_type, 8, 1,
                     0x1000, 52, sections_offset, 0, 52, 32, count, 40, len(headers), names_index)
    for index in range(count):
        struct.pack_into("<8I", image, 52+32*index, 1, offset, 0x1000, 0x1000,
                         len(body), len(body) if memsz is None else memsz, segment_flags, 4)
    return bytes(image)


class DiscoveryTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.base = Path(temporary.name)
        self.repo = self.base / "repo"
        self.references = self.base / "private" / "references"
        (self.repo / "config").mkdir(parents=True)
        self.body_a = words(0x0C000011, 0, 0x03E00008, 0)
        self.body_b = words(0x0C000022, 0, 0x03E00008, 0)
        self.configure()

    def configure(self, body_a=None, body_b=None, **elf_options):
        self.body_a = self.body_a if body_a is None else body_a
        self.body_b = self.body_b if body_b is None else body_b
        for program, body in [("0_demo", self.body_a), ("1_demo", self.body_b)]:
            path = self.references / "levels" / program / "overlay.elf"
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(synthetic_elf(body, **elf_options))
        target = {"serial": "SCUS_972.68", "iso": {"sha256": "f"*64},
                  "boot": {"sha256": "e"*64}, "expected_levels": 2}
        overlays = {"target": "SCUS_972.68", "levels": [
            {"level": name, "sha256": hashlib.sha256((self.references/"levels"/name/"overlay.elf").read_bytes()).hexdigest()}
            for name in ("0_demo", "1_demo")]}
        (self.repo/"config/target.json").write_text(json.dumps(target), encoding="utf8")
        (self.repo/"config/overlays.json").write_text(json.dumps(overlays), encoding="utf8")
        self.scope = {"schema": 1, "target": "SCUS_972.68", "functions": [
            {"program": "levels/0_demo", "address": 0x1000, "size": len(self.body_a),
             "boundary_evidence": "synthetic complete function A"},
            {"program": "levels/1_demo", "address": 0x1000, "size": len(self.body_b),
             "boundary_evidence": "synthetic complete function B"}]}

    def discover(self, scope=None):
        return family_discovery.discovery(self.repo, self.references, self.scope if scope is None else scope)

    def invoke(self, scope=None, output=None, functions=None):
        scope_path = self.base / "private" / "scope.json" if functions is None else functions
        scope_path.parent.mkdir(parents=True, exist_ok=True)
        scope_path.write_text(json.dumps(self.scope if scope is None else scope), encoding="utf8")
        output = self.base / "private" / "report.json" if output is None else output
        with contextlib.redirect_stderr(io.StringIO()):
            result = family_discovery.main(["--repo", str(self.repo), "--references", str(self.references),
                    "--functions", str(scope_path), "--output", str(output)])
        return result, output

    def test_real_parser_call_shape_is_not_a_match(self):
        result = self.discover()
        self.assertEqual(len(result["families"]), 1)
        group = result["families"][0]
        self.assertEqual(len(group["placements"]), 2)
        self.assertEqual(len({p["body_sha256"] for p in group["placements"]}), 2)
        self.assertFalse(result["matching"])
        self.assertFalse(group["matching"])
        self.assertEqual(group["integration_credit"], 0)
        self.assertEqual(group["size"], 16)
        def keys(value):
            if isinstance(value, dict):
                return set(value) | set().union(*(keys(v) for v in value.values()))
            if isinstance(value, list):
                return set().union(*(keys(v) for v in value))
            return set()
        self.assertTrue(keys(result).isdisjoint({"bytes", "words", "disassembly", "body", "matchedCodePercent"}))

    def test_constants_cop2_gp_hi_lo_and_opcode_are_retained(self):
        pairs = [(0x24020026, 0x24020029), (0x4B8208C0, 0x4B8210C0),
                 (0x33820010, 0x33820020), (0x3C02003F, 0x3C020040),
                 (0x24421234, 0x24421238), (0x08000011, 0x0C000011)]
        for first, second in pairs:
            with self.subTest(words=(first, second)):
                self.configure(words(first, 0x03E00008, 0), words(second, 0x03E00008, 0))
                self.assertEqual(len(self.discover()["families"]), 2)

    def test_trailing_nops_and_complete_sizes_retained(self):
        self.configure(words(0x03E00008, 0), words(0x03E00008, 0, 0))
        self.assertEqual({g["size"] for g in self.discover()["families"]}, {8, 12})

    def test_order_does_not_change_family_ids(self):
        first = self.discover()
        other = copy.deepcopy(self.scope)
        other["functions"].reverse()
        self.assertEqual(first["families"], self.discover(other)["families"])

    def test_optional_full_and_body_pins(self):
        scope = copy.deepcopy(self.scope)
        row = scope["functions"][0]
        row["reference_sha256"] = hashlib.sha256((self.references/row["program"]/"overlay.elf").read_bytes()).hexdigest()
        row["body_sha256"] = hashlib.sha256(self.body_a).hexdigest()
        self.assertEqual(len(self.discover(scope)["families"]), 1)
        for name in ("reference_sha256", "body_sha256"):
            bad = copy.deepcopy(scope)
            bad["functions"][0][name] = "a" * 64
            with self.subTest(pin=name), self.assertRaises(ValueError):
                self.discover(bad)

    def test_full_file_pin_rejects_change_outside_function(self):
        path = self.references/"levels/1_demo/overlay.elf"
        path.write_bytes(path.read_bytes()+b"changed trailing metadata")
        with self.assertRaisesRegex(ValueError, "Full reference"):
            self.discover()

    def test_invalid_target_region_or_unpinned_identity(self):
        scope = copy.deepcopy(self.scope)
        scope["target"] = "SCES_516.07"
        with self.assertRaises(ValueError):
            self.discover(scope)
        target_path = self.repo/"config/target.json"
        target = json.loads(target_path.read_bytes())
        target["expected_levels"] = 3
        target_path.write_text(json.dumps(target), encoding="utf8")
        with self.assertRaisesRegex(ValueError, "not pinned"):
            self.discover()

    def test_nonmatching_region_rejected(self):
        registry = {"schema":1,"default":"usa","regions":{"usa":{"label":"USA", "serial":"SCUS_972.68",
                    "target":"config/target.json","overlays":"config/overlays.json","matching":False}}}
        (self.repo/"config/regions.json").write_text(json.dumps(registry),encoding="utf8")
        with self.assertRaisesRegex(ValueError, "matching region"):
            self.discover()

    def test_overlap_and_alias_duplicates_rejected(self):
        for second in [{"address":0x1000,"size":16}, {"address":0x1004,"size":4}]:
            bad = copy.deepcopy(self.scope)
            bad["functions"].append({**bad["functions"][0], **second})
            with self.subTest(second=second), self.assertRaisesRegex(ValueError, "overlapping"):
                self.discover(bad)

    def test_adjacent_intervals_accepted(self):
        scope = {"schema":1,"target":"SCUS_972.68","functions":[
            {"program":"levels/0_demo","address":0x1000,"size":8,"boundary_evidence":"first complete stub"},
            {"program":"levels/0_demo","address":0x1008,"size":8,"boundary_evidence":"adjacent complete stub"}]}
        self.assertEqual(sum(len(g["placements"]) for g in self.discover(scope)["families"]),2)

    def test_invalid_intervals_evidence_and_fields(self):
        updates = [{"address":True}, {"address":0x1001}, {"size":0}, {"size":6},
                   {"address":0xFFFFFFFC,"size":8}, {"boundary_evidence":""},
                   {"boundary_evidence":"line1\nline2"}, {"body_sha256":"A"*64},
                   {"program":"levels/../../repo"}, {"words":[1,2,3]}]
        for update in updates:
            bad = copy.deepcopy(self.scope)
            bad["functions"][0].update(update)
            with self.subTest(update=update), self.assertRaises(ValueError):
                self.discover(bad)

    def test_only_loaded_progbits_ee_text_is_allowed(self):
        cases = [{"section_name":".vutext"}, {"section_name":".data"}, {"section_type":8},
                 {"section_flags":4}, {"segment_flags":4}, {"elf_type":1},
                 {"extra_section":True}, {"extra_segment":True}, {"offset_bias":4}]
        for options in cases:
            with self.subTest(options=options):
                self.configure(**options)
                with self.assertRaises(ValueError):
                    self.discover()

    def test_boot_layout_and_pin(self):
        boot=self.references/"boot.elf"
        boot.write_bytes(synthetic_elf(self.body_a,section_name="core.text"))
        target_path=self.repo/"config/target.json"
        target=json.loads(target_path.read_bytes())
        target["boot"]["sha256"]=hashlib.sha256(boot.read_bytes()).hexdigest()
        target_path.write_text(json.dumps(target),encoding="utf8")
        scope={"schema":1,"target":"SCUS_972.68","functions":[
            {"program":"boot","address":0x1000,"size":len(self.body_a),"boundary_evidence":"complete boot fixture"}]}
        report=self.discover(scope)
        self.assertEqual(report["families"][0]["placements"][0]["program"],"boot")

    def test_core_text_allowed(self):
        self.configure(section_name="core.text")
        self.assertEqual(len(self.discover()["families"]),1)

    def test_body_cannot_cross_file_backed_segment(self):
        self.configure(section_size=20,memsz=20)
        scope = copy.deepcopy(self.scope)
        scope["functions"][0]["size"] = 20
        with self.assertRaisesRegex(ValueError,"file-backed"):
            self.discover(scope)

    def test_late_invalid_pin_precedes_any_grouping(self):
        bad = copy.deepcopy(self.scope)
        bad["functions"][1]["body_sha256"] = "0"*64
        with mock.patch.object(family_discovery,"_shape_hash") as shape:
            with self.assertRaises(ValueError):
                self.discover(bad)
            shape.assert_not_called()

    def test_private_paths_refuse_repo_or_ancestor(self):
        for path in (self.repo, self.repo/"private", self.base):
            with self.subTest(path=path), self.assertRaises(ValueError):
                family_discovery.discovery(self.repo,path,self.scope)

    def test_cli_success_private_metadata_output(self):
        code,path = self.invoke()
        self.assertEqual(code,0)
        self.assertEqual(json.loads(path.read_bytes())["kind"],"family-discovery")

    def test_cli_late_invalid_body_leaves_no_output_or_parent(self):
        bad = copy.deepcopy(self.scope)
        bad["functions"][1]["body_sha256"] = "0"*64
        path=self.base/"new-output-directory/report.json"
        with self.assertRaises(SystemExit) as caught:
            self.invoke(bad,output=path)
        self.assertEqual(caught.exception.code,2)
        self.assertFalse(path.exists())
        self.assertFalse(path.parent.exists())

    def test_cli_no_overwrite(self):
        path=self.base/"private/existing.json"
        path.write_text("preserve",encoding="utf8")
        with self.assertRaises(SystemExit):
            self.invoke(output=path)
        self.assertEqual(path.read_text(encoding="utf8"),"preserve")

    def test_cli_exclusive_write_preserves_racing_output(self):
        path=self.base/"private/race.json"
        report=self.discover()
        def racing(*args):
            path.write_text("race-original",encoding="utf8")
            return report
        with mock.patch.object(family_discovery,"discovery",side_effect=racing),self.assertRaises(SystemExit):
            self.invoke(output=path)
        self.assertEqual(path.read_text(encoding="utf8"),"race-original")

    def test_cli_scope_and_output_cannot_be_in_repo(self):
        with self.assertRaises(SystemExit):
            self.invoke(output=self.repo/"out.json")
        self.assertFalse((self.repo/"out.json").exists())
        with self.assertRaises(SystemExit):
            self.invoke(functions=self.repo/"scope.json")

    def test_reference_symlink_escape_rejected(self):
        path=self.references/"levels/0_demo/overlay.elf"
        outside=self.base/"elsewhere.elf"
        outside.write_bytes(path.read_bytes())
        path.unlink()
        try:
            path.symlink_to(outside)
        except OSError as error:
            self.skipTest(f"Symlink unavailable: {error}")
        with self.assertRaisesRegex(ValueError,"escapes"):
            self.discover()

    def test_existing_broken_output_symlink_not_used(self):
        path=self.base/"private/broken-output.json"
        target=self.base/"private/target-not-created.json"
        try:
            path.symlink_to(target)
        except OSError as error:
            self.skipTest(f"Symlink unavailable: {error}")
        with self.assertRaises(SystemExit):
            self.invoke(output=path)
        self.assertFalse(target.exists())


if __name__ == "__main__":
    unittest.main(verbosity=2)
