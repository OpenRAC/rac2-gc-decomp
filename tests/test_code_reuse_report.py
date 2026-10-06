"""Asset-free supplementary partition, source association and replay tests."""
import copy
import contextlib
import io
import importlib.util
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import unique_code_report as uq
import code_reuse_report as reuse
H, N, S = "1" * 64, "2" * 64, "3" * 64


def row(identity, program, address, signature=S, status="qualified_complete"):
    return {"id": identity, "program": program, "address": address, "size": 16,
        "raw_sha256": H, "boundary": {"status": status, "evidence": ["synthetic"]},
        "normalization": {"signature_sha256": signature, "relocations": [],
            "certificate": {"raw_sha256": H, "normalized_sha256": signature,
                "reconstructed_sha256": H, "normalizer_sha256": N, "exact": True}}}


def catalog():
    return {"schema": 1, "target": "SCUS_972.68", "normalizer": {"id": "synthetic", "sha256": N},
        "programs": [{"program": p, "reference_sha256": H,
            "ee_sections": [{"address": a, "size": 32}], "excluded_vu_bytes": 8}
            for p, a in [("boot", 256), ("levels/a", 512)]],
        "functions": [row("a", "boot", 256), row("b", "levels/a", 512)]}


def run_report(c, credit=None):
    credit = credit or {}
    return reuse.generate(c, credit, uq.generate(c, credit))


class ReuseTests(unittest.TestCase):
    def test_same_partition_all_any_and_unresolved_gap(self):
        c = catalog()
        r, details = run_report(c, {("boot", 256, 16): H})
        self.assertEqual(r["metrics"]["template_total_bytes"], 48)
        self.assertEqual(r["metrics"]["template_all_c_bytes"], 0)
        self.assertEqual(r["metrics"]["template_any_c_bytes"], 16)
        self.assertEqual(r["quality"]["residual_gap_bytes"], 32)
        self.assertEqual(len(details), 1)
        r, _ = run_report(c, {("boot", 256, 16): H, ("levels/a", 512, 16): H})
        self.assertEqual(r["metrics"]["template_all_c_bytes"], 16)
        self.assertAlmostEqual(r["metrics"]["template_all_c_percent"], 100 / 3)

    def test_constant_register_opcode_template_variants_stay_separate(self):
        # A changed unmasked operand must produce a changed pinned signature/template.
        for label in ("constant", "register", "opcode", "field_offset", "trailing_nop"):
            c = catalog()
            signature = reuse.digest(label.encode())
            c["functions"][1] = row("b", "levels/a", 512, signature=signature)
            r, details = run_report(c)
            self.assertEqual(r["metrics"]["template_total_bytes"], 64)
            self.assertEqual(details, [])

    def test_real_normalizer_retains_scalar_register_and_opcode_bits(self):
        import relocation_identity as ri
        context = {"program": "boot", "reference_sha256": H,
            "sections": [{"address": 256, "size": 1024, "flags": 6, "type": 1}],
            "function_entries": []}
        first = struct.pack("<4I", 0x24020001, 0x03e00008, 0, 0)
        for word in (0x24020002, 0x24030001, 0x34020001):
            c = catalog(); c["normalizer"]["sha256"] = ri.NORMALIZER_SHA256
            bodies = (first, struct.pack("<4I", word, 0x03e00008, 0, 0))
            for member, body in zip(c["functions"], bodies):
                ctx = dict(context, program=member["program"])
                normalized = ri.normalize(body, member["address"], ctx)
                self.assertEqual(normalized["template"], body)
                member["raw_sha256"] = reuse.digest(body)
                member["normalization"] = {"signature_sha256": normalized["signature_sha256"],
                    "template_sha256": reuse.digest(normalized["template"]),
                    "relocations": normalized["relocations"], "certificate": normalized["certificate"]}
            summary, details = run_report(c)
            self.assertEqual(summary["metrics"]["template_total_bytes"], 64)
            self.assertEqual(details, [])

    def test_same_signature_different_template_hash_cannot_merge(self):
        c = catalog(); r = c["functions"][1]; r["normalization"]["template_sha256"] = "4" * 64
        r["normalization"]["certificate"]["normalized_sha256"] = "4" * 64
        self.assertEqual(run_report(c)[0]["metrics"]["template_total_bytes"], 64)

    def test_unsupported_extent_never_collapses_or_gains_credit(self):
        c = catalog(); c["functions"][1]["boundary"]["status"] = "inferred"
        r, _ = run_report(c, {("boot", 256, 16): H, ("levels/a", 512, 16): H})
        self.assertEqual(r["metrics"]["template_total_bytes"], 64)
        self.assertEqual(r["metrics"]["template_all_c_bytes"], 16)
        self.assertEqual(r["quality"]["unsupported_singleton_physical_bytes"], 16)

    def test_malformed_certificate_and_unknown_certificate_field_refused(self):
        for key, value in [("exact", 1), ("raw_sha256", "4" * 64),
                           ("reconstructed_sha256", "4" * 64), ("normalizer_sha256", "4" * 64),
                           ("guessed_mask", True)]:
            c = catalog(); c["functions"][0]["normalization"]["certificate"][key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                run_report(c)

    def test_credit_hash_partial_extent_duplicate_and_boolean_refused(self):
        with self.assertRaises(ValueError):
            run_report(catalog(), {("boot", 256, 16): "4" * 64})
        r, _ = run_report(catalog(), {("boot", 256, 12): H})
        self.assertEqual(r["metrics"]["template_all_c_bytes"], 0)
        with self.assertRaises(ValueError):
            run_report(catalog(), {("boot", 256, 16): H, ("boot", 260, 4): H})
        with self.assertRaises(ValueError):
            run_report(catalog(), {("boot", True, 16): H})

    def test_alias_equality_pattern_prevents_extra_merges(self):
        rels = [{"kind": "j26", "offset": i * 4, "target": t,
                 "role": "call", "evidence": "synthetic"} for i, t in enumerate((0x1000, 0x1000))]
        different = copy.deepcopy(rels); different[1]["target"] = 0x2000
        self.assertNotEqual(reuse.binding_alias_pattern(rels), reuse.binding_alias_pattern(different))
        c = catalog()
        c["functions"][0]["normalization"]["relocations"] = rels
        c["functions"][1]["normalization"]["relocations"] = different
        self.assertEqual(run_report(c)[0]["metrics"]["template_total_bytes"], 64)
        rels[0].update(internal=True, relative_target=4)
        self.assertEqual(reuse.binding_alias_pattern(rels)[0], ["internal", 4])

    def test_relocation_outside_body_and_scalar_unknown_stay_literal(self):
        c = catalog(); c["functions"][0]["normalization"]["relocations"] = [
            {"kind": "j26", "offset": 16, "target": 0x1000, "role": "call", "evidence": "synthetic"}]
        with self.assertRaises(ValueError):
            run_report(c)
        c = catalog(); c["functions"][1]["normalization"]["signature_sha256"] = "5" * 64
        c["functions"][1]["normalization"]["certificate"]["normalized_sha256"] = "5" * 64
        self.assertEqual(run_report(c)[0]["metrics"]["template_total_bytes"], 64)

    def test_primary_graph_splits_are_retained_as_diagnostic(self):
        c = catalog(); primary = uq.generate(c, {})
        primary["groups"] = [{"id": "p1", "members": ["a"]}, {"id": "p2", "members": ["b"]}]
        primary["metrics"]["unique_total_bytes"] = 64
        r, details = reuse.generate(c, {}, primary)
        self.assertEqual(r["metrics"]["template_total_bytes"], 48)
        self.assertEqual(r["primary_reference"]["metrics"]["unique_total_bytes"], 64)
        self.assertEqual(details[0]["conservative_class_count"], 2)
        primary["groups"][1]["members"] = ["a"]
        with self.assertRaises(ValueError):
            reuse.generate(c, {}, primary)

    def test_ordering_is_deterministic_and_no_external_estimate_input(self):
        c = catalog(); first = run_report(c)
        c["functions"].reverse(); c["programs"].reverse()
        self.assertEqual(reuse.encoded(first), reuse.encoded(run_report(c)))
        self.assertNotIn("estimate", first[0]["metrics"])

    def test_775_shape27_members_keeps27_primary_classes(self):
        c = catalog(); c["programs"] = []; c["functions"] = []
        credit = {}; primary_groups = []
        for i in range(27):
            program = "levels/p" + str(i); address = 0x1000
            c["programs"].append({"program": program, "reference_sha256": H,
                "ee_sections": [{"address": address, "size": 76}], "excluded_vu_bytes": 0})
            member = row(str(i), program, address); member["size"] = 76
            c["functions"].append(member); credit[(program, address, 76)] = H
            primary_groups.append({"id": str(i), "members": [str(i)]})
        primary = uq.generate(c, credit); primary["groups"] = primary_groups
        primary["metrics"]["unique_total_bytes"] = 2052
        summary, details = reuse.generate(c, credit, primary)
        self.assertEqual(summary["metrics"]["template_total_bytes"], 76)
        self.assertEqual(summary["metrics"]["template_all_c_bytes"], 76)
        self.assertEqual(details[0]["placements"], 27)
        self.assertEqual(details[0]["conservative_class_count"], 27)
        self.assertEqual(summary["primary_reference"]["metrics"]["unique_total_bytes"], 2052)

    def test_stale_input_pin_refused_by_maintained_api(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); (root / "input").write_bytes(b"current")
            c = {"input_pins": [{"path": "input", "sha256": reuse.digest(b"old")}]}
            with self.assertRaises(ValueError):
                uq.validate_pins(c, root)

    def test_check_detects_changed_snapshot_and_family_payload(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); output = root / "summary.json"; families = root / "families.json.gz"
            argv = ["--repo", str(root), "--catalog", str(root / "catalog.json"),
                    "--output", str(output), "--families-output", str(families)]
            c = catalog()
            with patch.object(uq, "read_catalog", return_value=(c, b"catalog")), \
                    patch.object(uq, "load_current_credit", return_value={}), \
                    patch.object(uq, "current_boot_binding", return_value=None), \
                    patch.object(reuse, "authored_subset", return_value=({"known_sources": 0}, [])), \
                    patch.object(reuse, "snapshot", return_value={"logical-input": H}) as snap, \
                    contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(reuse.main(argv), 0)
                original = families.read_bytes()
                self.assertEqual(reuse.main(argv + ["--check"]), 0)
                snap.return_value = {"logical-input": S}
                with self.assertRaises(ValueError):
                    reuse.main(argv + ["--check"])
                snap.return_value = {"logical-input": H}
                families.write_bytes(original + b"changed")
                with self.assertRaises(ValueError):
                    reuse.main(argv + ["--check"])

    def test_authored_shared_piece_slots_and_unequal_extents_are_separate_subset(self):
        import source_layout as sl
        import boot_sdk_unit as sdk
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); fragment = b"void @@FUNCTION@@(void) { return; }\n"
            files = {"src/shared.cfrag": fragment,
                     "candidates/boot.c": fragment.replace(b"@@FUNCTION@@", b"ONE"),
                     "candidates/a.c": fragment.replace(b"@@FUNCTION@@", b"TWO")}
            for name, data in files.items():
                path = root / name; path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(data)
            recipes = {name: {"pieces": [{"fragment": "src/shared.cfrag", "sha256": reuse.digest(fragment),
                "source_text_bytes": len(fragment), "replacements": {"@@FUNCTION@@": symbol}}]}
                for name, symbol in [("candidates/boot.c", "ONE"), ("candidates/a.c", "TWO")]}
            config = root / "config"; config.mkdir()
            (config / "source-layout.json").write_bytes(reuse.encoded({"recipes": recipes}))
            (config / "overlays.json").write_bytes(reuse.encoded({"levels": [{"level": "a"}]}))
            proofs = [("boot", "ONE", 256, 12, "candidates/boot.c"),
                      ("a", "TWO", 512, 16, "candidates/a.c")]
            credit = {}
            for program, symbol, address, size, source in proofs:
                f = {"symbol": symbol, "address": address, "size": size, "candidate_source": source, "reference_sha256": H}
                path = root / ("progress/integration.json" if program == "boot" else "progress/levels/a.json")
                path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(reuse.encoded({"program": program, "functions": [f]}))
                credit[("boot" if program == "boot" else "levels/a", address, size)] = H
            boot = {"functions": [{"symbol": "ONE", "size": 12}]}
            native = {"source": "candidates/a.c", "functions": [{"symbol": "TWO", "size": 16}]}
            with patch.object(sl, "load_inputs", return_value=(boot, [("catalog", native)], {})), patch.object(sdk, "admitted_units", return_value=[]):
                summary, details = reuse.authored_subset(root, credit)
                self.assertEqual(summary["exact_C_placements_associated"], 2)
                self.assertEqual(len(details), 1)
                self.assertEqual((details[0]["min_size"], details[0]["max_size"]), (12, 16))
                self.assertNotIn("percent", summary)
                files_path = root / "candidates/a.c"; files_path.write_bytes(b"changed")
                with self.assertRaises(ValueError):
                    reuse.authored_subset(root, credit)

    def test_private_replay_detects_opcode_register_constant_changes_without_assets(self):
        from relocation_identity import reconstruct, NORMALIZER_SHA256
        c = catalog(); c["normalizer"]["sha256"] = NORMALIZER_SHA256
        for member in c["functions"]:
            member["normalization"]["certificate"]["normalizer_sha256"] = NORMALIZER_SHA256
        body = struct.pack("<4I", 0x24020001, 0, 0, 0)
        signature = reuse.digest(b"ee-relocation-template-v1\0" + body + b"[]")
        for r in c["functions"]:
            r["raw_sha256"] = reuse.digest(body)
            r["normalization"].update(signature_sha256=signature, template_sha256=reuse.digest(body))
            r["normalization"]["certificate"].update(raw_sha256=reuse.digest(body),
                normalized_sha256=reuse.digest(body), reconstructed_sha256=reuse.digest(body))
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for p in c["programs"]:
                path = root / ("boot.elf" if p["program"] == "boot" else "levels/a/overlay.elf")
                path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(body)
                p["reference_sha256"] = reuse.digest(body)
            with patch("elf_tools.read_elf", return_value={}), patch("elf_tools._mappings", return_value=[]), patch("elf_tools._mapped_bytes", side_effect=lambda data, mappings, a, s: data):
                result = reuse.replay(c, root)
                self.assertEqual(result["supported_reconstructed_rows"], 2)
                # Restoring a fake scalar-as-address field can roundtrip, but
                # injecting it under the original trusted receipt must fail.
                forged = copy.deepcopy(c)
                forged["functions"][0]["normalization"]["relocations"] = [{"kind": "gp16",
                    "offset": 0, "target": 257, "gp": 256, "role": "fake-constant-address",
                    "evidence": "untrusted scalar-mask assertion"}]
                with self.assertRaisesRegex(ValueError, "template hash"):
                    reuse.replay(forged, root)
                c["functions"][0]["normalization"]["signature_sha256"] = "6" * 64
                with self.assertRaises(ValueError):
                    reuse.replay(c, root)


if __name__ == "__main__":
    unittest.main()
