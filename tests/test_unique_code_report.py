"""Synthetic metadata tests: no retail assets or compiler required."""
import copy
import importlib.util
import tempfile
import unittest
import json
import gzip
import sys
from unittest.mock import patch
import contextlib
import io
from pathlib import Path

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/unique_code_report.py"
spec = importlib.util.spec_from_file_location("unique_code_report", SCRIPT)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
H, S, N = "1" * 64, "2" * 64, "3" * 64


def row(identity, program, address, size=16, signature=S, status="qualified_complete"):
    return {"id": identity, "program": program, "address": address, "size": size,
            "raw_sha256": H, "aliases": [], "boundary": {"status": status, "evidence": ["synthetic complete extent"]},
            "normalization": {"signature_sha256": signature, "relocations": [],
                              "certificate": {"raw_sha256": H, "normalized_sha256": signature,
                                              "reconstructed_sha256": H, "normalizer_sha256": N, "exact": True}}}


def catalog():
    return {"schema": 1, "target": "SCUS_972.68", "normalizer": {"id": "test", "sha256": N},
            "programs": [{"program": p, "reference_sha256": H,
                          "ee_sections": [{"address": a, "size": 32}], "excluded_vu_bytes": 8}
                         for p, a in [("boot", 256), ("levels/a", 512)]],
            "functions": [row("a", "boot", 256), row("b", "levels/a", 512)]}


class UniqueReportTests(unittest.TestCase):
    def test_paired_all_vs_any_and_gaps(self):
        result = module.generate(catalog(), {("boot", 256, 16): H})
        self.assertEqual(result["validated_subset"]["total_unique_bytes"], 16)
        self.assertEqual(result["validated_subset"]["matched_c_unique_bytes"], 0)
        self.assertEqual(result["validated_subset"]["any_c_unique_bytes"], 16)
        self.assertEqual(result["metrics"]["unique_total_bytes"], 48)
        self.assertEqual(result["coverage"]["unresolved_gap_bytes"], 32)
        self.assertFalse(result["quality"]["certified"])
        both = module.generate(catalog(), {("boot", 256, 16): H, ("levels/a", 512, 16): H})
        self.assertEqual(both["metrics"]["unique_matched_bytes"], 16)

    def test_partial_c_range_gets_no_unique_credit(self):
        result = module.generate(catalog(), {("boot", 256, 12): H, ("levels/a", 512, 16): H})
        self.assertEqual(result["metrics"]["unique_matched_bytes"], 0)

    def test_different_constants_stay_separate(self):
        c = catalog()
        c["functions"][1] = row("b", "levels/a", 512, signature="4" * 64)
        self.assertEqual(module.generate(c, {})["validated_subset"]["total_unique_bytes"], 32)

    def test_inferred_extent_is_uncollapsed(self):
        c = catalog()
        c["functions"][1]["boundary"]["status"] = "inferred"
        r = module.generate(c, {})
        self.assertEqual(r["metrics"]["unique_total_bytes"], 64)
        self.assertEqual(r["quality"]["provisional_total_bytes"], 48)

    def test_aliases_do_not_add_placements_or_bytes(self):
        c = catalog()
        c["functions"][0]["aliases"] = ["alternate"]
        r = module.generate(c, {})
        self.assertEqual(r["coverage"]["placements"], 2)
        self.assertEqual(r["coverage"]["aliases"], 1)
        c["functions"][0]["aliases"] = ["a"]
        with self.assertRaises(ValueError):
            module.generate(c, {})

    def test_overlap_is_rejected(self):
        c = catalog()
        c["functions"].append(row("overlap", "boot", 260))
        with self.assertRaisesRegex(ValueError, "Overlapping"):
            module.generate(c, {})

    def test_disjoint_section_scope(self):
        c = catalog()
        c["programs"][0]["ee_sections"] = [{"address": 256, "size": 8}, {"address": 272, "size": 8}]
        with self.assertRaisesRegex(ValueError, "outside"):
            module.generate(c, {})

    def test_missing_and_tampered_reconstruction(self):
        for field, value in [("exact", False), ("reconstructed_sha256", "4" * 64),
                             ("normalizer_sha256", "4" * 64)]:
            c = catalog()
            c["functions"][0]["normalization"]["certificate"][field] = value
            with self.assertRaisesRegex(ValueError, "certificate"):
                module.generate(c, {})
        c = catalog()
        del c["functions"][0]["normalization"]["certificate"]
        with self.assertRaisesRegex(ValueError, "certificate"):
            module.generate(c, {})

    def test_current_c_hash_must_equal_catalogue(self):
        with self.assertRaisesRegex(ValueError, "contradicts"):
            module.generate(catalog(), {("boot", 256, 16): "5" * 64})

    def test_membership_changes_report(self):
        c = catalog()
        before = module.encoded(module.generate(c, {}))
        c["functions"][1]["id"] = "renamed"
        self.assertNotEqual(before, module.encoded(module.generate(c, {})))

    def test_input_freshness_and_traversal(self):
        with tempfile.TemporaryDirectory() as directory:
            repo = Path(directory)
            (repo / "proof.json").write_bytes(b"current")
            c = {"input_pins": [{"path": "proof.json", "sha256": module.digest(b"current")}]}
            self.assertEqual(module.validate_pins(c, repo), {"proof.json"})
            (repo / "proof.json").write_bytes(b"stale")
            with self.assertRaisesRegex(ValueError, "Stale"):
                module.validate_pins(c, repo)
            c["input_pins"][0]["path"] = "../proof.json"
            with self.assertRaises(ValueError):
                module.validate_pins(c, repo)

    def test_manifest_membership_and_tampering(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            c = catalog()
            payload = b"\n".join(json.dumps(r).encode() for r in c.pop("functions"))
            (root / "boot.ndjson").write_bytes(payload)
            c["function_chunks"] = [{"path": "boot.ndjson", "sha256": module.digest(payload), "format": "ndjson"}]
            (root / "manifest.json").write_text(json.dumps(c))
            loaded, _ = module.read_catalog(root / "manifest.json")
            self.assertEqual(len(loaded["functions"]), 2)
            (root / "boot.ndjson").write_bytes(payload + b" ")
            with self.assertRaisesRegex(ValueError, "Stale catalogue chunk"):
                module.read_catalog(root / "manifest.json")

    def test_global_objdiff_uses_same_partition(self):
        sys.path.insert(0, str(SCRIPT.parent))
        report = module.generate(catalog(), {("boot", 256, 16): H, ("levels/a", 512, 16): H})
        exported = module.objdiff(report)
        self.assertEqual(int(exported["measures"]["totalCode"]), report["metrics"]["unique_total_bytes"])
        self.assertEqual(int(exported["measures"]["matchedCode"]), report["metrics"]["unique_matched_bytes"])
        self.assertEqual(sum(int(u["measures"]["totalCode"]) for u in exported["units"]), 48)

    def test_objdiff_root_fields_match_pinned_cli_report_schema(self):
        sys.path.insert(0, str(SCRIPT.parent))
        exported = module.objdiff(module.generate(catalog(), {}))
        self.assertEqual(set(exported), {"version", "measures", "units", "categories"})
        self.assertEqual(exported["version"], 2)
        self.assertEqual(int(exported["measures"]["totalCode"]),
                         sum(int(unit["measures"]["totalCode"]) for unit in exported["units"]))

    def test_current_credit_overlap_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "Overlapping"):
            module.generate(catalog(), {("boot", 256, 16): H, ("boot", 260, 12): H})

    def test_compact_expansion_preserves_metrics(self):
        c = catalog()
        compact = copy.deepcopy(c)
        compact["functions"] = [module.expand_compact_row(
            [r["address"], r["size"], H, r["boundary"]["status"], S, [], [], "synthetic complete extent", 1],
            r["program"], N) for r in c["functions"]]
        a, b = module.generate(c, {}), module.generate(compact, {})
        for field in ("metrics", "quality", "coverage", "validated_subset", "provisional_partition"):
            self.assertEqual(a[field], b[field])

    def test_compact_exact_flag_and_hash_tampering(self):
        base = [256, 16, H, "qualified_complete", S, [], [], "synthetic complete extent", 1]
        for receipt in (True, "1", 2, {"exact": "true", "reconstructed_sha256": H}):
            altered = [*base[:-1], receipt]
            with self.assertRaises(ValueError):
                module.expand_compact_row(altered, "boot", N)
        for receipt in (0, None, {"exact": True, "normalized_sha256": S, "reconstructed_sha256": "4" * 64}):
            c = catalog()
            c["functions"][0] = module.expand_compact_row([*base[:-1], receipt], "boot", N)
            with self.assertRaises(ValueError):
                module.generate(c, {})
        with self.assertRaisesRegex(ValueError, "schema"):
            module.expand_compact_row(base[:-1], "boot", N)

    def test_compact_manifest_and_duplicate_program_row(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            c = catalog()
            del c["functions"]
            payload = json.dumps([256, 16, H, "qualified_complete", S, [], [], "synthetic extent", 1]).encode()
            (root / "boot.ndjson").write_bytes(payload + b"\n" + payload)
            c["function_chunks"] = [{"path": "boot.ndjson", "program": "boot", "format": "compact-ndjson-v1",
                                     "sha256": module.digest(payload + b"\n" + payload)}]
            (root / "manifest.json").write_text(json.dumps(c))
            loaded, _ = module.read_catalog(root / "manifest.json")
            with self.assertRaisesRegex(ValueError, "Duplicate function id"):
                module.generate(loaded, {})
            c["function_chunks"][0]["format"] = "compact-ndjson-v2"
            (root / "manifest.json").write_text(json.dumps(c))
            with self.assertRaisesRegex(ValueError, "Unknown catalogue chunk format"):
                module.read_catalog(root / "manifest.json")

    def test_gzip_metadata_pins_malformed_and_limit(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            c = catalog()
            payload = json.dumps(c.pop("functions")).encode()
            compressed = gzip.compress(payload, mtime=0)
            (root / "metadata.json.gz").write_bytes(compressed)
            chunk = {"path": "metadata.json.gz", "format": "json", "compression": "gzip",
                     "sha256": module.digest(compressed), "expanded_sha256": module.digest(payload)}
            c["function_chunks"] = [chunk]
            def save():
                (root / "manifest.json").write_text(json.dumps(c))
            save()
            loaded, _ = module.read_catalog(root / "manifest.json")
            self.assertEqual(loaded["functions"], json.loads(payload))
            chunk["expanded_sha256"] = "4" * 64
            save()
            with self.assertRaisesRegex(ValueError, "Stale expanded"):
                module.read_catalog(root / "manifest.json")
            chunk["expanded_sha256"] = module.digest(payload)
            chunk["max_expanded_bytes"] = len(payload) - 1
            save()
            with self.assertRaisesRegex(ValueError, "exceeds size limit"):
                module.read_catalog(root / "manifest.json")
            del chunk["max_expanded_bytes"]
            (root / "metadata.json.gz").write_bytes(b"invalid gzip")
            chunk["sha256"] = module.digest(b"invalid gzip")
            save()
            with self.assertRaisesRegex(ValueError, "Malformed gzip"):
                module.read_catalog(root / "manifest.json")
            chunk["sha256"] = module.digest(compressed)
            save()
            with self.assertRaisesRegex(ValueError, "Stale catalogue chunk"):
                module.read_catalog(root / "manifest.json")

    def test_boolean_schema_is_rejected(self):
        c = catalog()
        c["schema"] = True
        with self.assertRaisesRegex(ValueError, "identity"):
            module.generate(c, {})
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "manifest.json"
            path.write_text(json.dumps(c))
            with self.assertRaisesRegex(ValueError, "schema"):
                module.read_catalog(path)

    def test_cli_summary_check_and_private_groups(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source, output = root / "catalog.json", root / "summary.json"
            source.write_text(json.dumps(catalog()))
            argv = ["unique_code_report.py", "--catalog", str(source), "--output", str(output)]
            def run(extra=()):
                with patch.object(sys, "argv", [*argv, *extra]), patch.object(module, "load_current_credit", return_value={}), contextlib.redirect_stdout(io.StringIO()):
                    return module.main()
            self.assertEqual(run(), 0)
            summary = json.loads(output.read_bytes())
            self.assertNotIn("groups", summary)
            self.assertEqual(run(["--check"]), 0)
            self.assertEqual(run(["--include-groups"]), 0)
            full = json.loads(output.read_bytes())
            self.assertIn("groups", full)
            self.assertEqual(summary, {key: value for key, value in full.items() if key != "groups"})
            self.assertEqual(run(["--include-groups", "--check"]), 0)
            with self.assertRaisesRegex(ValueError, "Stale unique report"):
                run(["--check"])
            self.assertEqual(run(), 0)
            summary["metrics"]["unique_total_bytes"] += 4
            output.write_text(json.dumps(summary))
            with self.assertRaisesRegex(ValueError, "Stale unique report"):
                run(["--check"])

    def test_relocation_shared_hi_and_pair_field_roles(self):
        first = {"kind": "hi16_lo16", "offset": 8, "high_offset": 0, "low_offset": 8,
                 "target": 0x1bca40, "lo_mode": "addiu", "role": "field-addend48", "evidence": "synthetic address use"}
        same = {**first, "role": "field-addend52"}
        shared_high = {**first, "offset": 12, "low_offset": 12, "target": 0x1bca44, "role": "other pointer"}
        module.validate_relocations([first, same, shared_high], 0x100, 16)
        conflicting_low = {**same, "target": first["target"] + 4}
        with self.assertRaisesRegex(ValueError, "Conflicting overlapping"):
            module.validate_relocations([first, conflicting_low], 0x100, 16)
        conflicting_high = {**shared_high, "target": first["target"] + 0x10000}
        with self.assertRaisesRegex(ValueError, "Conflicting overlapping"):
            module.validate_relocations([first, conflicting_high], 0x100, 16)

    def test_relocation_jump_and_gp_conflict_and_metadata(self):
        jump = {"kind": "j26", "offset": 0, "target": 0x400, "role": "call", "evidence": "synthetic entry"}
        module.validate_relocations([jump, {**jump, "role": "duplicate description"}], 0x100, 16)
        with self.assertRaisesRegex(ValueError, "Conflicting overlapping"):
            module.validate_relocations([jump, {**jump, "target": 0x404}], 0x100, 16)
        gp = {"kind": "gp16", "offset": 4, "target": 0x104, "gp": 0x100,
              "role": "data", "evidence": "synthetic GP proof"}
        module.validate_relocations([gp], 0x100, 16)
        for modified in ({**gp, "gp": True}, {**gp, "target": 0x10000},
                         {**jump, "target": 0x10000400}, {**jump, "target": 0x402},
                         {**jump, "role": ""}, {**jump, "offset": 16}):
            with self.assertRaises(ValueError):
                module.validate_relocations([modified], 0x100, 16)

    def test_relocation_hi16_carry_and_field_scope(self):
        pair = {"kind": "hi16_lo16", "offset": 4, "high_offset": 0, "low_offset": 4,
                "target": 0x18000, "lo_mode": "addiu", "role": "data", "evidence": "synthetic pair"}
        module.validate_relocations([pair], 0x100, 16)
        with self.assertRaisesRegex(ValueError, "Conflicting overlapping"):
            module.validate_relocations([pair, {**pair, "lo_mode": "ori"}], 0x100, 16)
        with self.assertRaisesRegex(ValueError, "outside complete body"):
            module.validate_relocations([{**pair, "high_offset": 16}], 0x100, 16)

    def test_template_hash_is_independent_of_role_bearing_signature(self):
        c = catalog()
        for r in c["functions"]:
            r["normalization"]["template_sha256"] = "6" * 64
            r["normalization"]["certificate"]["normalized_sha256"] = "6" * 64
        result = module.generate(c, {})
        self.assertEqual(result["validated_subset"]["total_unique_bytes"], 16)
        del c["functions"][0]["normalization"]["template_sha256"]
        with self.assertRaisesRegex(ValueError, "explicit template hash"):
            module.generate(c, {})

    def test_compact_explicit_template_hash_roundtrip_and_tamper(self):
        r = module.expand_compact_row([256, 16, H, "qualified_complete", S, [], [], "synthetic extent",
              {"exact": True, "normalized_sha256": "6" * 64, "reconstructed_sha256": H}], "boot", N)
        self.assertEqual(r["normalization"]["signature_sha256"], S)
        self.assertEqual(r["normalization"]["template_sha256"], "6" * 64)
        c = catalog()
        c["functions"][0] = r
        module.generate(c, {})
        r["normalization"]["template_sha256"] = "7" * 64
        with self.assertRaisesRegex(ValueError, "certificate mismatch"):
            module.generate(c, {})

    def graph_catalog(self, equal_callees=False):
        c = catalog()
        c["group_policy"] = "graph-refined-structural-templates"
        c["dependency_policy"] = "complete-static-control-dependencies"
        c["functions"] += [row("callee-a", "boot", 272, signature="4" * 64),
                           row("callee-b", "levels/a", 528, signature=("4" if equal_callees else "5") * 64)]
        for r in c["functions"]:
            r["call_dependencies"] = []
        c["functions"][0]["call_dependencies"] = [{"offset": 0, "kind": "call", "target": 272}]
        c["functions"][1]["call_dependencies"] = [{"offset": 0, "kind": "call", "target": 528}]
        return c

    def test_graph_primary_splits_distinct_callees_and_retains_provisional_templates(self):
        c = self.graph_catalog()
        r = module.generate(c, {("boot", 256, 16): H, ("levels/a", 512, 16): H})
        self.assertEqual(r["metrics"]["unique_total_bytes"], 64)
        self.assertEqual(r["metrics"]["unique_matched_bytes"], 32)
        self.assertEqual(r["provisional_partition"]["representative_bytes"], 48)
        self.assertEqual(r["graph_refinement"]["final_class_count"], 4)
        self.assertEqual(r["graph_refinement"]["external_static_edge_count"], 2)
        self.assertFalse(r["quality"]["certified"])
        same = module.generate(self.graph_catalog(equal_callees=True), {})
        self.assertEqual(same["metrics"]["unique_total_bytes"], 32)

    def test_graph_unmasked_branch_targets_and_unresolved_targets_split(self):
        c = self.graph_catalog(equal_callees=True)
        c["functions"][0]["call_dependencies"][0].update(kind="branch", target=0x900)
        c["functions"][1]["call_dependencies"][0].update(kind="branch", target=0x900)
        r = module.generate(c, {})
        self.assertEqual(r["metrics"]["unique_total_bytes"], 48)
        self.assertEqual(r["graph_refinement"]["unresolved_edge_count"], 2)

    def test_graph_policy_requires_explicit_complete_static_dependency_lists(self):
        c = self.graph_catalog()
        del c["functions"][0]["call_dependencies"]
        with self.assertRaisesRegex(ValueError, "explicit static dependencies"):
            module.generate(c, {})
        c = self.graph_catalog()
        c["dependency_policy"] = "only-masked-j26"
        with self.assertRaisesRegex(ValueError, "complete static"):
            module.generate(c, {})
        for alteration in ({"target": True}, {"offset": 16}, {"offset": 2}, {"kind": "indirect-call"},
                           {"target": 256}, {"target_program": "missing"}):
            c = self.graph_catalog()
            c["functions"][0]["call_dependencies"][0].update(alteration)
            with self.subTest(alteration=alteration), self.assertRaises(ValueError):
                module.generate(c, {})

    def test_compact_tenth_field_preserves_complete_dependency_metadata(self):
        dependencies = [{"offset": 0, "kind": "branch", "target": 0x900}]
        r = module.expand_compact_row([256, 16, H, "qualified_complete", S, [], [], "synthetic extent", 1, dependencies], "boot", N)
        self.assertEqual(r["call_dependencies"], dependencies)
        c = self.graph_catalog()
        c["functions"][0] = r
        module.generate(c, {})

    def test_graph_receipt_must_include_normalized_external_control_target(self):
        c = self.graph_catalog()
        c["functions"][0]["normalization"]["relocations"] = [
            {"kind": "j26", "offset": 0, "target": 0x900, "role": "callee", "evidence": "synthetic entry"}]
        with self.assertRaisesRegex(ValueError, "omits a normalized external target"):
            module.generate(c, {})

    def test_current_policy_requires_unowned_data_operand_guard(self):
        c = self.graph_catalog()
        with self.assertRaisesRegex(ValueError, "retained unowned data operands"):
            module.require_current_policy(c)
        c["data_policy"] = "retain-unowned-data-address-operands"
        module.require_current_policy(c)
        c["data_policy"] = "erase-mapped-data-addresses"
        with self.assertRaisesRegex(ValueError, "retained unowned data operands"):
            module.require_current_policy(c)

    def test_primary_preserves_unowned_folded_data_offsets(self):
        c = self.graph_catalog(equal_callees=True)
        c["data_policy"] = "retain-unowned-data-address-operands"
        for index, target in enumerate((0x18004, 0x18008)):
            r = c["functions"][index]
            r["raw_sha256"] = ("8" if index == 0 else "9") * 64
            r["normalization"]["certificate"].update(raw_sha256=r["raw_sha256"], reconstructed_sha256=r["raw_sha256"])
            r["normalization"]["relocations"] = [{"kind": "hi16_lo16", "offset": 12,
                "high_offset": 8, "low_offset": 12, "lo_mode": "addiu", "target": target,
                "role": "unowned-pointer", "evidence": "synthetic mapped pointer use"}]
        result = module.generate(c, {})
        self.assertEqual(result["metrics"]["unique_total_bytes"], 48)
        self.assertEqual(result["provisional_partition"]["representative_bytes"], 32)
        self.assertFalse(result["quality"]["original_data_ownership_proven"])
        self.assertEqual(result["quality"]["data_policy"], "retain-unowned-data-address-operands")


if __name__ == "__main__":
    unittest.main()
