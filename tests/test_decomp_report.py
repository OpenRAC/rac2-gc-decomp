import copy
import hashlib
import importlib.util
import io
import json
from pathlib import Path
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location("decomp_report", ROOT / "scripts" / "decomp_report.py")
report_module = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(report_module)


class DecompReportTests(unittest.TestCase):
    def setUp(self):
        self.scope = json.loads((ROOT / "config" / "progress-scope.json").read_text())
        self.target = json.loads((ROOT / "config" / "target.json").read_text())
        self.overlays = json.loads((ROOT / "config" / "overlays.json").read_text())
        self.progress = json.loads((ROOT / "progress" / "report.json").read_text())
        self.progress["decompiled_functions"] = 0
        self.progress["integrated_functions"] = 0
        self.catalog = json.loads((ROOT / "config" / "candidate-catalog.json").read_text())
        self.source = (ROOT / "candidates" / "boot.c").read_bytes()
        self.object_proof = json.loads((ROOT / "progress" / "candidates.json").read_text())
        self.level_catalog = json.loads((ROOT / "config" / "level-catalog.json").read_text())

    def generate(self):
        return report_module.generate(self.scope, self.target, self.overlays, self.progress)

    def test_assembly_reconstruction_never_counts_as_decompilation(self):
        report = self.generate()
        self.assertTrue(self.progress["g1"]["matched"])
        self.assertEqual(len(self.progress["g3"]), 27)
        self.assertEqual(report["measures"]["matchedCode"], "0")
        self.assertEqual(report["measures"]["completeCode"], "0")
        self.assertEqual(report["measures"]["completeUnits"], 0)
        self.assertTrue(all(not unit["metadata"]["complete"] for unit in report["units"]))

    def test_scope_and_categories_cover_all_measured_programs(self):
        report = self.generate()
        expected_code = sum(section["size"] for program in self.scope["programs"]
                            for section in program["sections"] if section["flags"] & 4)
        self.assertEqual(int(report["measures"]["totalCode"]), expected_code)
        self.assertEqual(sum(int(category["measures"]["totalCode"]) for category in report["categories"]), expected_code)
        self.assertEqual({unit["metadata"]["moduleName"] for unit in report["units"]},
                         {program["name"] for program in self.scope["programs"]})
        self.assertEqual(len(report["units"]), len({unit["name"] for unit in report["units"]}))

    def test_missing_or_duplicated_overlay_is_rejected(self):
        for change in ("missing", "duplicate"):
            with self.subTest(change=change):
                scope = copy.deepcopy(self.scope)
                scope["programs"].pop()
                if change == "duplicate":
                    scope["programs"].append(copy.deepcopy(scope["programs"][-1]))
                with self.assertRaises(ValueError):
                    report_module.generate(scope, self.target, self.overlays, self.progress)

    def test_changed_program_identity_is_rejected(self):
        self.scope["programs"][-1]["sha256"] = "0" * 64
        with self.assertRaises(ValueError):
            self.generate()

    def test_progress_beyond_baseline_requires_integration_proof(self):
        for field in ("decompiled_functions", "integrated_functions"):
            with self.subTest(field=field):
                self.progress[field] = 1
                with self.assertRaises(ValueError):
                    self.generate()
                self.progress[field] = 0

    def test_nobits_and_nonallocated_sections_are_rejected(self):
        for field, value in (("type", 8), ("flags", 0), ("size", 0)):
            with self.subTest(field=field):
                scope = copy.deepcopy(self.scope)
                scope["programs"][0]["sections"][0][field] = value
                with self.assertRaises(ValueError):
                    report_module.generate(scope, self.target, self.overlays, self.progress)

    def integration(self):
        functions = [{"symbol": function["symbol"], "address": function["address"],
                      "size": function["size"], "matched": True, "integrated": True, "program": "boot"}
                     for function in self.catalog["functions"]]
        self.progress["decompiled_functions"] = len(functions)
        self.progress["integrated_functions"] = len(functions)
        return {"target": self.target["serial"], "reference_sha256": self.target["boot"]["sha256"],
                "source_sha256": hashlib.sha256(self.source).hexdigest(),
                "catalog_sha256": hashlib.sha256(self.catalog_bytes()).hexdigest(),
                "candidate_source": "candidates/boot.c", "functions": functions,
                "full_boot_gate": {"matched": True, "bytes_compared": 2521763, "segments": 2},
                "state": "integrated", "tools": copy.deepcopy(self.object_proof["tools"]),
                "matched_code_bytes": sum(function["size"] for function in functions)}

    def catalog_bytes(self):
        return json.dumps(self.catalog).encode("utf-8")

    def generate_integrated(self, integration):
        with patch.object(Path, "read_bytes", side_effect=[self.source, self.catalog_bytes()]):
            return report_module.generate(self.scope, self.target, self.overlays, self.progress, integration)

    def update_catalog_function(self, integration, index, **changes):
        self.catalog["functions"][index].update(changes)
        integration["functions"][index].update(changes)
        integration["catalog_sha256"] = hashlib.sha256(self.catalog_bytes()).hexdigest()

    def level_catalog_bytes(self):
        return json.dumps(self.level_catalog).encode("utf-8")

    def level_proof(self, level="0_aranos_tutorial"):
        gate = next(item for item in self.progress["g3"] if item["level"] == level)
        functions = [{"symbol": function["symbol"], "address": function["address"],
                      "size": function["size"], "matched": True, "integrated": True, "program": level}
                     for function in self.level_catalog["levels"][level]["functions"]]
        return {"target": self.target["serial"], "program": level,
                "reference_sha256": gate["reference_sha256"],
                "source_sha256": hashlib.sha256(self.source).hexdigest(),
                "catalog_sha256": hashlib.sha256(self.level_catalog_bytes()).hexdigest(),
                "candidate_source": "candidates/boot.c", "state": "integrated", "functions": functions,
                "full_level_gate": {"matched": True, "bytes_compared": gate["bytes_compared"], "segments": 1},
                "matched_code_bytes": sum(function["size"] for function in functions),
                "tools": copy.deepcopy(self.object_proof["tools"])}

    def generate_levels(self, levels):
        with patch.object(Path, "read_bytes",
                          side_effect=[self.source, self.catalog_bytes(),
                                       self.level_catalog_bytes(), self.catalog_bytes()]):
            return report_module.generate(self.scope, self.target, self.overlays, self.progress,
                                          self.integration(), levels)

    def test_fuzzy_percent_weights_code_and_data_while_code_percent_uses_only_code(self):
        measures = report_module.measures(80, 120, 1, 40)
        self.assertEqual(measures["matchedCodePercent"], 50)
        self.assertEqual(measures["completeCodePercent"], 50)
        self.assertEqual(measures["fuzzyMatchPercent"], 20)
        self.assertEqual(report_module.measures(0, 120, 1)["fuzzyMatchPercent"], 0)
        self.assertEqual(report_module.measures(0, 0, 0)["fuzzyMatchPercent"], 0)

    def test_complete_c_units_preserve_totals_and_categories(self):
        baseline = self.generate()
        integration = self.integration()
        report = self.generate_integrated(integration)
        measures = report["measures"]
        self.assertEqual(measures["totalCode"], "48788176")
        self.assertEqual(measures["totalData"], baseline["measures"]["totalData"])
        # The reviewed lot as committed: 168 catalogue bodies, 8128 bytes.
        self.assertEqual(measures["completeCode"], "8128")
        self.assertEqual(measures["matchedCode"], "8128")
        self.assertEqual(measures["completeUnits"], 168)
        self.assertEqual(measures["totalUnits"], 344)
        self.assertEqual(measures["matchedCodePercent"], 8128 / 48788176 * 100)
        self.assertEqual(measures["completeCodePercent"], 8128 / 48788176 * 100)
        for aggregate in [measures, *(category["measures"] for category in report["categories"])]:
            self.assertEqual(aggregate["fuzzyMatchPercent"], int(aggregate["matchedCode"])
                             / (int(aggregate["totalCode"]) + int(aggregate["totalData"])) * 100)
        for field in ("totalCode", "totalData", "completeCode", "matchedCode", "totalUnits", "completeUnits"):
            with self.subTest(field=field):
                self.assertEqual(sum(int(unit["measures"][field]) for unit in report["units"]), int(measures[field]))
                self.assertEqual(sum(int(category["measures"][field]) for category in report["categories"]),
                                 int(measures[field]))
        completed = [unit for unit in report["units"] if unit["metadata"]["complete"]]
        self.assertEqual({unit["functions"][0]["name"] for unit in completed},
                         {function["symbol"] for function in integration["functions"]})
        for unit in completed:
            self.assertEqual(unit["metadata"]["sourcePath"], "candidates/boot.c")
            self.assertFalse(unit["metadata"]["autoGenerated"])
            self.assertEqual(unit["measures"]["matchedCodePercent"], 100)
        for category in report["categories"]:
            self.assertEqual(category["measures"]["matchedCode"], "8128" if category["id"] == "boot" else "0")
        original_units = {unit["name"]: unit for unit in baseline["units"]}
        for unit in report["units"]:
            if unit["metadata"]["complete"]:
                continue
            original = original_units[unit["name"]]
            promoted_bytes = sum(function["size"] for function in integration["functions"]
                                 if unit["metadata"]["moduleName"] == "boot"
                                 and int(original["sections"][0]["metadata"]["virtualAddress"]) <= function["address"]
                                 < int(original["sections"][0]["metadata"]["virtualAddress"])
                                 + int(original["sections"][0]["size"]))
            self.assertEqual(int(unit["sections"][0]["size"]),
                             int(original["sections"][0]["size"]) - promoted_bytes)
            self.assertEqual(unit["measures"]["matchedCode"], "0")
        exported = json.dumps(report)
        for private_field in ("sha256", "full_boot_gate", "tools", "state", "meaning", "xrefs"):
            self.assertNotIn(private_field, exported)

    def test_fully_promoted_section_omits_empty_parent(self):
        integration = self.integration()
        section = next(section for section in self.scope["programs"][0]["sections"]
                       if section["address"] == integration["functions"][0]["address"])
        section["size"] = integration["functions"][0]["size"]
        integration["functions"] = integration["functions"][:1]
        integration["matched_code_bytes"] = section["size"]
        self.progress["decompiled_functions"] = self.progress["integrated_functions"] = 1
        report = self.generate_integrated(integration)
        parent_name = f"boot/{section['name']}@{section['address']:08X}"
        self.assertNotIn(parent_name, {unit["name"] for unit in report["units"]})
        self.assertEqual(report["measures"]["totalUnits"], 176)
        self.assertTrue(all(int(unit["sections"][0]["size"]) > 0 for unit in report["units"]))

    def test_integration_identity_source_and_hashes_are_required(self):
        original = self.integration()
        for field, value in (("target", "SCUS_971.142"), ("reference_sha256", "0" * 64),
                             ("source_sha256", "0" * 64), ("catalog_sha256", "0" * 64),
                             ("candidate_source", "candidates/other.c"),
                             ("candidate_source", "../candidates/boot.c"), ("state", "matched_unintegrated"),
                             ("source_sha256", "not-a-hash"), ("tools", {}), ("tools", {"cc": "bad"})):
            with self.subTest(field=field, value=value):
                integration = copy.deepcopy(original)
                integration[field] = value
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)
        for field in original:
            with self.subTest(missing=field):
                integration = copy.deepcopy(original)
                del integration[field]
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)

    def test_changed_source_and_catalog_identity_are_rejected(self):
        integration = self.integration()
        self.source += b"\n"
        with self.assertRaisesRegex(ValueError, "hash mismatch"):
            self.generate_integrated(integration)
        integration["source_sha256"] = hashlib.sha256(self.source).hexdigest()
        for field, value in (("target", "SCUS_971.142"), ("reference_sha256", "0" * 64)):
            with self.subTest(field=field):
                original = self.catalog[field]
                self.catalog[field] = value
                integration["catalog_sha256"] = hashlib.sha256(self.catalog_bytes()).hexdigest()
                with self.assertRaisesRegex(ValueError, "catalog identity"):
                    self.generate_integrated(integration)
                self.catalog[field] = original

    def test_partial_mismatch_duplicate_and_unknown_functions_are_rejected(self):
        original = self.integration()
        for field, value in (("matched", False), ("matched", 1), ("integrated", False),
                             ("integrated", 1), ("program", "levels/0_aranos_tutorial"),
                             ("size", original["functions"][0]["size"] + 4), ("symbol", "unknown"), ("different_bytes", 1)):
            with self.subTest(field=field, value=value):
                integration = copy.deepcopy(original)
                integration["functions"][0][field] = value
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)
        integration = copy.deepcopy(original)
        integration["functions"].append(copy.deepcopy(integration["functions"][0]))
        with self.assertRaisesRegex(ValueError, "duplicate"):
            self.generate_integrated(integration)
        integration = copy.deepcopy(original)
        integration["functions"].pop()
        with self.assertRaises(ValueError):
            self.generate_integrated(integration)

    def test_overlapping_functions_are_rejected_even_with_consistent_catalog(self):
        integration = self.integration()
        self.update_catalog_function(integration, 1, address=integration["functions"][0]["address"] + 4)
        with self.assertRaisesRegex(ValueError, "Overlapping integrated"):
            self.generate_integrated(integration)

    def test_out_of_bounds_and_nonexecutable_functions_are_rejected(self):
        for address in (0x80000000, 1260416, 1135104 - 4, 1135104 + 125264 - 4):
            with self.subTest(address=address):
                integration = self.integration()
                self.update_catalog_function(integration, 0, address=address)
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)

    def test_invalid_instruction_boundaries_are_rejected(self):
        for field, value in (("address", 1135105), ("address", True), ("size", 0),
                             ("size", -4), ("size", 10), ("size", True)):
            with self.subTest(field=field, value=value):
                integration = self.integration()
                self.update_catalog_function(integration, 0, **{field: value})
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)

    def test_full_boot_gate_cannot_be_failed_partial_or_from_another_reference(self):
        original = self.integration()
        for field, value in (("matched", False), ("matched", 1), ("bytes_compared", 72),
                             ("bytes_compared", "2521763"), ("segments", 1),
                             ("reference_sha256", "0" * 64), ("candidate_sha256", "0" * 64)):
            with self.subTest(field=field, value=value):
                integration = copy.deepcopy(original)
                integration["full_boot_gate"][field] = value
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)
        for field, value in (("matched", False), ("bytes_compared", 72), ("reference_sha256", "0" * 64)):
            with self.subTest(progress_field=field):
                before = self.progress["g1"][field]
                self.progress["g1"][field] = value
                with self.assertRaises(ValueError):
                    self.generate_integrated(original)
                self.progress["g1"][field] = before

    def test_progress_and_byte_counts_must_agree_exactly(self):
        integration = self.integration()
        for field in ("integrated_functions", "decompiled_functions"):
            for value in (0, 6, 8, True, "7"):
                with self.subTest(field=field, value=value):
                    self.progress[field] = value
                    with self.assertRaises(ValueError):
                        self.generate_integrated(integration)
            self.progress[field] = 7
        for value in (0, 68, 76, "72", True):
            with self.subTest(bytes=value):
                integration["matched_code_bytes"] = value
                with self.assertRaises(ValueError):
                    self.generate_integrated(integration)

    def test_contradictory_optional_function_hashes_are_rejected(self):
        integration = self.integration()
        integration["functions"][0].update(reference_sha256="1" * 64, candidate_sha256="2" * 64)
        with self.assertRaisesRegex(ValueError, "hashes differ"):
            self.generate_integrated(integration)

    def test_raw_assembly_cannot_be_promoted_even_with_updated_source_hash(self):
        integration = self.integration()
        self.source += b'asm(".word 0");'
        integration["source_sha256"] = hashlib.sha256(self.source).hexdigest()
        with self.assertRaisesRegex(ValueError, "embed assembly"):
            self.generate_integrated(integration)

    def test_duplicate_catalog_and_overlapping_sections_are_rejected(self):
        integration = self.integration()
        self.catalog["functions"].append(copy.deepcopy(self.catalog["functions"][0]))
        integration["catalog_sha256"] = hashlib.sha256(self.catalog_bytes()).hexdigest()
        with self.assertRaisesRegex(ValueError, "Duplicate catalog"):
            self.generate_integrated(integration)
        self.progress["integrated_functions"] = self.progress["decompiled_functions"] = 0
        self.scope["programs"][0]["sections"].append(copy.deepcopy(self.scope["programs"][0]["sections"][0]))
        with self.assertRaisesRegex(ValueError, "Overlapping progress"):
            self.generate()

    def test_level_proof_adds_per_program_bytes_and_units(self):
        integration = self.integration()
        boot_only = self.generate_integrated(integration)
        proof = self.level_proof()
        report = self.generate_levels([proof])
        measures = report["measures"]
        level_bytes = proof["matched_code_bytes"]
        self.assertEqual(level_bytes, 7196)
        self.assertEqual(measures["totalCode"], boot_only["measures"]["totalCode"])
        self.assertEqual(measures["totalData"], boot_only["measures"]["totalData"])
        self.assertEqual(measures["totalUnits"], int(boot_only["measures"]["totalUnits"]) + 148)
        self.assertEqual(measures["matchedCode"], str(8128 + level_bytes))
        self.assertEqual(measures["completeCode"], str(8128 + level_bytes))
        self.assertEqual(measures["completeUnits"], 168 + len(proof["functions"]))
        categories = {category["id"]: category["measures"] for category in report["categories"]}
        self.assertEqual(categories["boot"]["matchedCode"], "8128")
        self.assertEqual(categories["levels"]["matchedCode"], str(level_bytes))
        self.assertEqual(categories["levels"]["completeUnits"], len(proof["functions"]))
        for field in ("totalCode", "totalData", "completeCode", "matchedCode", "totalUnits", "completeUnits"):
            with self.subTest(field=field):
                self.assertEqual(sum(int(unit["measures"][field]) for unit in report["units"]), int(measures[field]))
                self.assertEqual(sum(int(category["measures"][field]) for category in report["categories"]),
                                 int(measures[field]))
        completed = [unit for unit in report["units"] if unit["metadata"]["complete"]]
        self.assertEqual({unit["metadata"]["moduleName"] for unit in completed},
                         {"boot", "levels/" + proof["program"]})
        level_units = [unit for unit in completed if unit["metadata"]["moduleName"] == "levels/" + proof["program"]]
        self.assertEqual(len(level_units), len(proof["functions"]))
        self.assertEqual({unit["functions"][0]["name"] for unit in level_units},
                         {function["symbol"] for function in proof["functions"]})
        for unit in level_units:
            self.assertEqual(unit["metadata"]["sourcePath"], "candidates/boot.c")
            self.assertEqual(unit["metadata"]["progressCategories"], ["levels"])
            self.assertEqual(unit["measures"]["matchedCodePercent"], 100)
        untouched = [unit for unit in report["units"] if unit["metadata"]["moduleName"] == "levels/1_oozla"]
        self.assertTrue(untouched)
        self.assertTrue(all(unit["measures"]["matchedCode"] == "0" for unit in untouched))
        exported = json.dumps(report)
        for private_field in ("sha256", "full_level_gate", "tools", "state", "meaning", "xrefs"):
            self.assertNotIn(private_field, exported)

    def test_level_units_reduce_only_their_own_section(self):
        integration = self.integration()
        boot_only = self.generate_integrated(integration)
        proof = self.level_proof()
        report = self.generate_levels([proof])
        before = {unit["name"]: unit for unit in boot_only["units"]}
        after = {unit["name"]: unit for unit in report["units"]}
        added = set(after) - set(before)
        self.assertEqual(len(added), len(proof["functions"]))
        self.assertTrue(all(after[name]["metadata"]["complete"] for name in added))
        self.assertTrue(all(after[name]["metadata"]["moduleName"] == "levels/" + proof["program"] for name in added))
        changed = {name for name in before if name not in after or before[name] != after[name]}
        self.assertEqual(len(changed), 1)
        parent = changed.pop()
        self.assertTrue(parent.startswith("levels/" + proof["program"] + "/"))
        self.assertEqual(int(after[parent]["sections"][0]["size"]),
                         int(before[parent]["sections"][0]["size"]) - proof["matched_code_bytes"])
        self.assertEqual({unit["name"] for unit in report["units"] if not unit["metadata"]["complete"]},
                         {unit["name"] for unit in boot_only["units"] if not unit["metadata"]["complete"]})

    def test_level_bytes_count_only_when_a_proof_is_provided(self):
        integration = self.integration()
        boot_only = self.generate_integrated(integration)
        levels_category = next(category for category in boot_only["categories"] if category["id"] == "levels")
        self.assertEqual(levels_category["measures"]["matchedCode"], "0")
        self.assertEqual(levels_category["measures"]["completeUnits"], 0)
        self.assertEqual(int(boot_only["measures"]["completeUnits"]), 168)
        placeholders = [unit for unit in boot_only["units"]
                        if unit["metadata"]["moduleName"] == "levels/0_aranos_tutorial"]
        self.assertTrue(placeholders)
        self.assertTrue(all(not unit["metadata"]["complete"] for unit in placeholders))

    def test_level_proof_identity_and_source_are_required(self):
        integration = self.integration()
        original = self.level_proof()
        for field, value in (("target", "SCUS_971.142"), ("program", "9_unknown"), ("program", 5),
                             ("reference_sha256", "0" * 64), ("source_sha256", "0" * 64),
                             ("source_sha256", "not-a-hash"), ("catalog_sha256", "0" * 64),
                             ("candidate_source", "candidates/other.c"), ("state", "matched_unintegrated"),
                             ("tools", {}), ("tools", {"cc": "bad"}), ("tools", {"ld.exe": "0" * 64})):
            with self.subTest(field=field, value=value):
                proof = copy.deepcopy(original)
                proof[field] = value
                with self.assertRaises(ValueError):
                    self.generate_levels([proof])
        for field in original:
            with self.subTest(missing=field):
                proof = copy.deepcopy(original)
                del proof[field]
                with self.assertRaises(ValueError):
                    self.generate_levels([proof])
        forged = copy.deepcopy(original)
        forged["source_sha256"] = hashlib.sha256(self.source + b"\n").hexdigest()
        with self.assertRaisesRegex(ValueError, "verified boot C source"):
            self.generate_levels([forged])

    def test_level_proof_function_and_gate_contradictions_are_rejected(self):
        integration = self.integration()
        original = self.level_proof()
        for field, value in (("matched", False), ("matched", 1), ("integrated", False), ("integrated", 1),
                             ("program", "boot"), ("address", original["functions"][0]["address"] + 4),
                             ("size", original["functions"][0]["size"] + 4), ("symbol", "FUN_99999999"), ("different_bytes", 1)):
            with self.subTest(field=field, value=value):
                proof = copy.deepcopy(original)
                proof["functions"][0][field] = value
                with self.assertRaises(ValueError):
                    self.generate_levels([proof])
        proof = copy.deepcopy(original)
        proof["functions"][0].update(reference_sha256="1" * 64, candidate_sha256="2" * 64)
        with self.assertRaisesRegex(ValueError, "hashes differ"):
            self.generate_levels([proof])
        for change in ("partial", "duplicate", "unknown"):
            with self.subTest(change=change):
                proof = copy.deepcopy(original)
                if change == "partial":
                    proof["functions"].pop()
                elif change == "duplicate":
                    proof["functions"].append(copy.deepcopy(proof["functions"][0]))
                else:
                    proof["functions"].append({**copy.deepcopy(proof["functions"][0]),
                                               "symbol": "FUN_99999999",
                                               "address": proof["functions"][-1]["address"] + 8})
                proof["matched_code_bytes"] = sum(function["size"] for function in proof["functions"])
                with self.assertRaises(ValueError):
                    self.generate_levels([proof])
        for field, value in (("matched", False), ("bytes_compared", 0), ("bytes_compared", "2751616"),
                             ("segments", 0), ("segments", "1")):
            with self.subTest(gate_field=field, value=value):
                proof = copy.deepcopy(original)
                proof["full_level_gate"][field] = value
                with self.assertRaises(ValueError):
                    self.generate_levels([proof])
        proof = copy.deepcopy(original)
        proof["full_level_gate"]["bytes_compared"] = 4096
        with self.assertRaisesRegex(ValueError, "contradicts"):
            self.generate_levels([proof])
        proof = copy.deepcopy(original)
        proof["matched_code_bytes"] = 1
        with self.assertRaisesRegex(ValueError, "byte count mismatch"):
            self.generate_levels([proof])
        recorded = next(item for item in self.progress["g3"] if item["level"] == original["program"])
        before = recorded["bytes_compared"]
        recorded["bytes_compared"] = 1
        with self.assertRaisesRegex(ValueError, "contradicts"):
            self.generate_levels([original])
        recorded["bytes_compared"] = before

    def test_level_proof_placement_must_match_the_reviewed_catalog(self):
        integration = self.integration()
        inventory = copy.deepcopy(self.level_catalog)
        self.level_catalog["target"] = "SCUS_971.142"
        with self.assertRaisesRegex(ValueError, "catalog identity"):
            self.generate_levels([self.level_proof()])
        self.level_catalog = copy.deepcopy(inventory)
        self.level_catalog["levels"]["0_aranos_tutorial"]["functions"][0]["size"] += 4
        with self.assertRaisesRegex(ValueError, "reviewed complete C body"):
            self.generate_levels([self.level_proof()])
        self.level_catalog = copy.deepcopy(inventory)
        self.level_catalog["levels"]["0_aranos_tutorial"]["reference_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "pinned overlay identity"):
            self.generate_levels([self.level_proof()])
        self.level_catalog = copy.deepcopy(inventory)
        self.level_catalog["levels"].pop("1_oozla", None)
        base = self.level_proof()
        gate = next(item for item in self.progress["g3"] if item["level"] == "1_oozla")
        orphan = {**copy.deepcopy(base), "program": "1_oozla", "reference_sha256": gate["reference_sha256"],
                  "full_level_gate": {"matched": True, "bytes_compared": gate["bytes_compared"], "segments": 1},
                  "functions": [{**function, "program": "1_oozla"} for function in base["functions"]]}
        with self.assertRaisesRegex(ValueError, "No reviewed level placement"):
            self.generate_levels([orphan])
        self.level_catalog = copy.deepcopy(inventory)

    def test_duplicate_and_orphan_level_proofs_are_rejected(self):
        integration = self.integration()
        proof = self.level_proof()
        with self.assertRaisesRegex(ValueError, "Duplicate"):
            self.generate_levels([proof, copy.deepcopy(proof)])
        with self.assertRaises(ValueError):
            report_module.generate(self.scope, self.target, self.overlays, self.progress, None, [proof])
        with self.assertRaises(ValueError):
            report_module.generate(self.scope, self.target, self.overlays, self.progress,
                                   self.integration(), {"0_aranos_tutorial": proof})

    def run_main(self, integration, proof=None, level_proofs=None):
        level_proofs = level_proofs or []
        fixtures = {"config/progress-scope.json": self.scope, "config/target.json": self.target,
                    "config/overlays.json": self.overlays, "progress/report.json": self.progress,
                    "progress/integration.json": integration,
                    "progress/candidates.json": proof if proof is not None else self.object_proof}
        argv = ["decomp_report.py", "--output", str(ROOT / "unwritten-report.json")]
        for index, level_proof in enumerate(level_proofs):
            fixtures[f"level-proof-{index}.json"] = level_proof
            argv.extend(["--level-proof", str(ROOT / f"level-proof-{index}.json")])
        reads = [self.source, self.catalog_bytes()]
        if level_proofs:
            reads.extend([self.level_catalog_bytes(), self.catalog_bytes()])
        def read_text(path, **kwargs):
            return json.dumps(fixtures[path.relative_to(ROOT).as_posix()])
        with patch("sys.argv", argv), \
                patch.object(Path, "exists", return_value=integration is not None), \
                patch.object(Path, "read_text", autospec=True, side_effect=read_text), \
                patch.object(Path, "read_bytes", side_effect=reads), \
                patch.object(Path, "mkdir") as mkdir, patch.object(Path, "write_text") as write, \
                patch("sys.stdout", new_callable=io.StringIO) as stdout:
            try:
                status = report_module.main()
            except (ValueError, KeyError):
                mkdir.assert_not_called()
                write.assert_not_called()
                raise
            return status, write, stdout.getvalue()

    def test_main_reads_integration_and_validates_candidate_object_proof_before_writing(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        status, write, stdout = self.run_main(integration)
        self.assertEqual(status, 0)
        report = json.loads(write.call_args.args[0])
        self.assertEqual(report["measures"]["matchedCode"], "8128")
        self.assertEqual(json.loads(stdout)["matched_code"], "8128")

    def test_main_baseline_without_integration_is_unchanged(self):
        status, write, stdout = self.run_main(None)
        self.assertEqual(status, 0)
        self.assertEqual(json.loads(write.call_args.args[0]), self.generate())
        self.assertEqual(json.loads(stdout)["matched_code"], "0")

    def test_main_counts_only_the_level_proofs_it_is_given(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        proof = self.level_proof()
        status, write, stdout = self.run_main(integration)
        self.assertEqual(status, 0)
        self.assertEqual(json.loads(write.call_args.args[0])["measures"]["matchedCode"], "8128")
        status, write, stdout = self.run_main(integration, level_proofs=[proof])
        self.assertEqual(status, 0)
        report = json.loads(write.call_args.args[0])
        self.assertEqual(report["measures"]["matchedCode"], str(8128 + proof["matched_code_bytes"]))
        self.assertEqual(report["measures"]["completeUnits"], 168 + len(proof["functions"]))
        self.assertEqual(json.loads(stdout)["matched_code"], str(8128 + proof["matched_code_bytes"]))
        self.assertEqual(json.loads(stdout)["units"], len(report["units"]))

    def test_main_refuses_a_level_proof_that_does_not_match_the_shipped_catalog(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        proof = self.level_proof()
        proof["catalog_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "catalog hash mismatch"):
            self.run_main(integration, level_proofs=[proof])

    def test_generation_preserves_inputs_and_does_not_depend_on_function_order(self):
        integration = self.integration()
        originals = copy.deepcopy((self.scope, self.target, self.overlays, self.progress, integration))
        report = self.generate_integrated(integration)
        self.assertEqual((self.scope, self.target, self.overlays, self.progress, integration), originals)
        integration["functions"].reverse()
        self.assertEqual(self.generate_integrated(integration), report)

    def test_main_requires_all_candidate_tools_but_allows_additional_boot_instruments(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        missing = copy.deepcopy(integration)
        missing["tools"].pop("cc1")
        with self.assertRaisesRegex(ValueError, "instrument mismatch"):
            self.run_main(missing)
        integration["tools"]["Ps2EeAs.exe"] = "1" * 64
        status, write, stdout = self.run_main(integration)
        self.assertEqual(status, 0)

    def test_main_refuses_stale_optional_object_hash_and_malformed_object_proof(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        integration["object_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "object proof hash mismatch"):
            self.run_main(integration)
        del integration["object_sha256"]
        for proof in ([], {**self.object_proof, "functions": [None]}):
            with self.subTest(proof=proof):
                with self.assertRaises(ValueError):
                    self.run_main(integration, proof)

    def test_full_boot_elf_is_distinct_from_candidate_elf_but_c_object_is_identical(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        integration["candidate_elf_sha256"] = "2" * 64
        integration["c_object_sha256"] = self.object_proof["object_sha256"]
        status, write, stdout = self.run_main(integration)
        self.assertEqual(status, 0)
        integration["c_object_sha256"] = "0" * 64
        with self.assertRaisesRegex(ValueError, "C object proof hash mismatch"):
            self.run_main(integration)

    def test_forged_integration_is_rejected_when_object_proof_is_invalid(self):
        integration = self.integration()
        self.object_proof["catalog_sha256"] = integration["catalog_sha256"]
        for field, value in (("object_sha256", "bad"), ("source_sha256", "0" * 64),
                             ("catalog_sha256", "0" * 64), ("target", "SCUS_971.142"),
                             ("tools", {"ee-gcc2953.exe": "0" * 64})):
            with self.subTest(field=field):
                proof = copy.deepcopy(self.object_proof)
                proof[field] = value
                with self.assertRaises(ValueError):
                    self.run_main(integration, proof)
        for field, value in (("matched", False),
                             ("size", self.object_proof["functions"][0]["size"] + 4), ("different_bytes", 1),
                             ("candidate_sha256", "0" * 64)):
            with self.subTest(function_field=field):
                proof = copy.deepcopy(self.object_proof)
                proof["functions"][0][field] = value
                with self.assertRaises(ValueError):
                    self.run_main(integration, proof)
        proof = copy.deepcopy(self.object_proof)
        proof["functions"].pop(0)
        with self.assertRaises(ValueError):
            self.run_main(integration, proof)
        proof = copy.deepcopy(self.object_proof)
        proof["functions"].append(copy.deepcopy(proof["functions"][0]))
        with self.assertRaises(ValueError):
            self.run_main(integration, proof)
        integration["functions"][0].update(reference_sha256="1" * 64, candidate_sha256="1" * 64)
        with self.assertRaisesRegex(ValueError, "function hashes disagree"):
            self.run_main(integration)


if __name__ == "__main__":
    unittest.main()
