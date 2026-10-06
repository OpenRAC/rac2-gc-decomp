"""Synthetic identity-isolated builder tests; no game assets or compiler."""
import contextlib
import copy
import gzip
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch
from unittest.mock import Mock
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import build_unique_catalog as builder


class BuilderTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.repo, self.references, self.output = self.root / "repo", self.root / "references", self.root / "metadata"
        self.repo.mkdir()
        self.references.mkdir()
        self.raw = bytes(16)
        (self.references / "boot.elf").write_bytes(self.raw)
        self.reference_sha = builder.sha(self.raw)
        self.scope = {"target": "SCUS_972.68", "programs": [{"name": "boot", "sha256": self.reference_sha,
                      "sections": [{"name": "core.text", "address": 256, "size": 16, "flags": 6, "type": 1}]}]}
        for name in ("config/target.json", "config/overlays.json", "config/progress-scope.json",
                     "progress/report.json", "progress/integration.json", "progress/candidates.json",
                     "scripts/decomp_report.py", "scripts/unique_code_report.py", "scripts/relocation_identity.py",
                     "scripts/build_unique_catalog.py", "scripts/call_graph_refinement.py"):
            path = self.repo / name
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(b"{}" if name.endswith("json") else b"# synthetic instrument\n")
        self.write_scope()
        self.normalizer_sha = builder.sha((self.repo / "scripts/relocation_identity.py").read_bytes())
        self.boundary = {"schema": 1, "target": "SCUS_972.68", "decoder": "synthetic-identity-fixture",
                         "reference_pins": {"boot": self.reference_sha}, "qualified_extent_failures": [],
                         "contexts": [{"program": "boot", "reference_sha256": self.reference_sha}],
                         "functions": [{"id": "fixture", "program": "boot", "address": 256, "size": 16,
                                        "raw_sha256": self.reference_sha,
                                        "boundary": {"status": "qualified_complete", "evidence": ["synthetic extent"]}}]}
        self.boundary_path = self.root / "boundaries.json"
        self.elf = {"sections": [{"name": "core.text", "type": 1, "flags": 6,
                                  "address": 256, "size": 16, "offset": 0}]}

    def write_scope(self):
        (self.repo / "config/progress-scope.json").write_text(json.dumps(self.scope))

    def normalize(self, data, address, context):
        template_hash = builder.sha(data)
        signature = builder.sha(b"ee-relocation-template-v1\0" + data + b"[]")
        return {"template": data, "signature_sha256": signature, "relocations": [], "unresolved": [],
                "certificate": {"raw_sha256": template_hash, "normalized_sha256": template_hash,
                                "reconstructed_sha256": template_hash, "normalizer_sha256": self.normalizer_sha, "exact": True}}

    def build(self, normalize=None, reconstruct=None, destination=None, pointer_evidence=None):
        self.boundary_path.write_text(json.dumps(self.boundary))
        with patch.object(builder.elf_tools, "_parse", return_value=self.elf), \
             patch.object(builder.identity, "prepare_context", side_effect=lambda c: c), \
             patch.object(builder.identity, "normalize", side_effect=normalize or self.normalize), \
             patch.object(builder.identity, "reconstruct", side_effect=reconstruct or (lambda template, address, relocations: template)), \
             contextlib.redirect_stdout(io.StringIO()):
            return builder.build(self.repo, self.boundary_path, self.references, destination or self.output, pointer_evidence)

    def test_metadata_only_reproducible_gzip_and_current_pins(self):
        manifest, failures = self.build()
        self.assertEqual(failures, [])
        chunk = manifest["function_chunks"][0]
        payload = (self.output / chunk["path"]).read_bytes()
        self.assertEqual(builder.sha(payload), chunk["sha256"])
        expanded = gzip.decompress(payload)
        self.assertEqual(builder.sha(expanded), chunk["expanded_sha256"])
        rows = [json.loads(line) for line in expanded.splitlines()]
        self.assertEqual(rows[0][8], {"exact": True, "normalized_sha256": self.reference_sha,
                                     "reconstructed_sha256": self.reference_sha})
        self.assertEqual(rows[0][2], self.reference_sha)
        self.assertNotIn(str(self.root), expanded.decode())
        self.assertNotIn("template", expanded.decode())
        self.assertEqual(rows[0][9], [])
        self.assertEqual(manifest["group_policy"], "graph-refined-structural-templates")
        self.assertEqual(manifest["dependency_policy"], "complete-static-control-dependencies")
        self.assertEqual(manifest["data_policy"], "retain-unowned-data-address-operands")
        for pin in manifest["input_pins"]:
            self.assertEqual(pin["sha256"], builder.sha((self.repo / pin["path"]).read_bytes()))
        second, _ = self.build(destination=self.root / "metadata2")
        self.assertEqual(manifest, second)
        self.assertEqual(payload, (self.root / "metadata2" / chunk["path"]).read_bytes())

    def test_sdk_owner_closure_enters_manifest_and_checks_freshness(self):
        owner_path = 'src/sdk/fixture.c'
        path = self.repo / owner_path
        path.parent.mkdir(parents=True)
        path.write_bytes(b'qualified source')
        with patch.object(builder.unique_code_report, 'sdk_owner_input_paths', return_value={owner_path}) as closure:
            manifest, _ = self.build()
        closure.assert_called_once_with({}, self.repo)
        self.assertEqual(next(p['sha256'] for p in manifest['input_pins'] if p['path'] == owner_path),
                         builder.sha(b'qualified source'))

    def test_sdk_owner_source_changes_during_normalization_refused(self):
        owner_path = 'src/sdk/fixture.c'
        path = self.repo / owner_path
        path.parent.mkdir(parents=True)
        path.write_bytes(b'qualified source')
        def changed(data, address, context):
            path.write_bytes(b'changed source')
            return self.normalize(data, address, context)
        with patch.object(builder.unique_code_report, 'sdk_owner_input_paths', return_value={owner_path}), \
             self.assertRaisesRegex(ValueError, 'changed'):
            self.build(normalize=changed)
        self.assertFalse(self.output.exists())

    def test_existing_destination_and_public_wrong_parent_rejected(self):
        self.output.mkdir()
        sentinel = self.output / "preserve.txt"
        sentinel.write_text("preserve")
        with self.assertRaisesRegex(ValueError, "destination exists"):
            self.build()
        self.assertEqual(sentinel.read_text(), "preserve")
        with self.assertRaisesRegex(ValueError, "direct config subdirectory"):
            self.build(destination=self.repo / "progress/metadata")

    def test_retail_reference_pin_and_span_hash_rejected_before_output(self):
        (self.references / "boot.elf").write_bytes(b"different")
        with self.assertRaisesRegex(ValueError, "Changed pinned reference"):
            self.build()
        self.assertFalse(self.output.exists())
        (self.references / "boot.elf").write_bytes(self.raw)
        self.boundary["functions"][0]["raw_sha256"] = "4" * 64
        with self.assertRaisesRegex(ValueError, "span differs"):
            self.build()
        self.assertFalse(self.output.exists())

    def test_reconstruction_failure_becomes_uncollapsed_without_fake_receipt(self):
        manifest, failures = self.build(reconstruct=lambda *_: b"wrong")
        self.assertEqual(len(failures), 1)
        self.assertEqual(manifest["generation"]["reconstruction_checks"], 0)
        chunk = manifest["function_chunks"][0]
        row = json.loads(gzip.decompress((self.output / chunk["path"]).read_bytes()))
        self.assertEqual(row[3], "inferred")
        self.assertIsNone(row[4])
        self.assertIsNone(row[8])

    def test_boundary_reference_pin_must_equal_progress_scope(self):
        self.scope["programs"][0]["sha256"] = "4" * 64
        self.write_scope()
        with self.assertRaises(ValueError):
            self.build()
        self.assertFalse(self.output.exists())

    def test_duplicate_program_context_rejected(self):
        self.boundary["contexts"].append(copy.deepcopy(self.boundary["contexts"][0]))
        with self.assertRaises(ValueError):
            self.build()

    def test_input_snapshot_change_rejected_before_output(self):
        def changing(data, address, context):
            (self.repo / "progress/report.json").write_bytes(b"changed during normalization")
            return self.normalize(data, address, context)
        with self.assertRaises(ValueError):
            self.build(normalize=changing)
        self.assertFalse(self.output.exists())

    def test_certificate_signature_consistency_is_checked(self):
        def wrong_certificate(data, address, context):
            result = self.normalize(data, address, context)
            result["certificate"]["normalized_sha256"] = "4" * 64
            return result
        manifest, failures = self.build(normalize=wrong_certificate)
        self.assertEqual(len(failures), 1)
        self.assertEqual(manifest["generation"]["reconstruction_checks"], 0)

    def test_reference_path_inside_repo_rejected(self):
        self.references = self.repo / "references"
        with self.assertRaisesRegex(ValueError, "private outside repository"):
            self.build()
        self.assertFalse(self.output.exists())

    def test_body_scope_and_truncation_guards(self):
        with self.assertRaisesRegex(ValueError, "outside one EE code section"):
            builder.body(self.raw, self.elf, 260, 16)
        ambiguous = {"sections": [*self.elf["sections"], *self.elf["sections"]]}
        with self.assertRaisesRegex(ValueError, "outside one EE code section"):
            builder.body(self.raw, ambiguous, 256, 16)
        with self.assertRaisesRegex(ValueError, "Truncated"):
            builder.body(self.raw[:8], self.elf, 256, 16)

    def test_self_consistent_wrong_template_signature_has_no_receipt(self):
        def wrong_signature(data, address, context):
            result = self.normalize(data, address, context)
            result["signature_sha256"] = result["certificate"]["normalized_sha256"] = "4" * 64
            return result
        manifest, failures = self.build(normalize=wrong_signature)
        self.assertEqual(len(failures), 1)
        self.assertEqual(manifest["generation"]["reconstruction_checks"], 0)

    def test_role_schema_tampering_invalidates_signature_receipt(self):
        def altered_roles(data, address, context):
            result = self.normalize(data, address, context)
            result["relocations"] = [{"kind": "j26", "offset": 0, "target": 0x400,
                                      "role": "different-callee", "evidence": "synthetic entry"}]
            return result
        manifest, failures = self.build(normalize=altered_roles)
        self.assertEqual(len(failures), 1)
        self.assertEqual(manifest["generation"]["reconstruction_checks"], 0)

    def test_boundary_schema_bool_rejected(self):
        self.boundary["schema"] = True
        with self.assertRaisesRegex(ValueError, "identity"):
            self.build()

    def test_truthy_nonboolean_exact_is_not_qualification(self):
        def truthy(data, address, context):
            result = self.normalize(data, address, context)
            result["certificate"]["exact"] = 1
            return result
        manifest, failures = self.build(normalize=truthy)
        self.assertEqual(len(failures), 1)
        self.assertEqual(manifest["generation"]["reconstruction_checks"], 0)

    def test_cli_requires_new_private_diagnostics(self):
        for diagnostics in (self.repo / "work/diagnostics.json", self.root / "existing.json"):
            if diagnostics.name == "existing.json":
                diagnostics.write_text("preserve")
            args = ["build_unique_catalog.py", "--repo", str(self.repo), "--boundaries", str(self.boundary_path),
                    "--references", str(self.references), "--output", str(self.output), "--diagnostics", str(diagnostics)]
            with patch.object(sys, "argv", args), self.assertRaisesRegex(ValueError, "new private file"):
                builder.main()
            self.assertFalse(self.output.exists())

    def pointer_fixture(self, compressed=False):
        for name in ("data_role_evidence_v2.py", "pointer_evidence_loader.py", "scan_pointer_roles.py"):
            (self.repo / "scripts" / name).write_bytes(b"# synthetic pointer instrument\n")
        proof_path = self.repo / "scripts/data_role_evidence_v2.py"
        loader_path = self.repo / "scripts/pointer_evidence_loader.py"
        proof_sha = builder.sha(proof_path.read_bytes())
        record = {"program": "boot", "callee_address": 256, "callee_size": 16,
                  "callee_raw_sha256": self.reference_sha, "reference_sha256": self.reference_sha,
                  "boundary_tier": "qualified_complete", "argument_register": 4,
                  "dereference_instruction_offset": 0, "dereference_offset": 0, "width": 4, "action": "load",
                  "proof_decoder_sha256": proof_sha, "decoder_dependency_sha256": self.normalizer_sha,
                  "proof_status": "verified_incoming_dereference", "evidence": "synthetic theorem"}
        report = {"schema": 1, "target": "SCUS_972.68", "proof_decoder_sha256": proof_sha,
                  "decoder_dependency_sha256": self.normalizer_sha, "records": [record]}
        payload = json.dumps(report).encode()
        path = self.root / ("pointer.json.gz" if compressed else "pointer.json")
        path.write_bytes(gzip.compress(payload, mtime=0) if compressed else payload)
        loader = SimpleNamespace(__file__=str(loader_path), data_role_evidence=SimpleNamespace(__file__=str(proof_path)),
                                 load_pointer_evidence=Mock(return_value={"boot": [record]}))
        return path, loader, record

    def test_pointer_replay_precedes_context_promotion_and_pins_metadata(self):
        evidence, loader, record = self.pointer_fixture(compressed=True)
        def inspected(data, address, context):
            self.assertEqual(context["pointer_argument_roles"], [record])
            self.assertEqual(context["function_pins"]["256"], {"size": 16, "raw_sha256": self.reference_sha,
                                                                  "boundary_tier": "qualified_complete"})
            self.assertTrue(loader.load_pointer_evidence.called)
            return self.normalize(data, address, context)
        with patch.dict(sys.modules, {"pointer_evidence_loader": loader}):
            manifest, failures = self.build(normalize=inspected, pointer_evidence=evidence)
        self.assertFalse(failures)
        loader.load_pointer_evidence.assert_called_once()
        self.assertTrue(loader.load_pointer_evidence.call_args.kwargs["replay"])
        provenance = manifest["provenance"]["pointer_evidence"]
        self.assertEqual(provenance["artifact_sha256"], builder.sha(evidence.read_bytes()))
        self.assertEqual(provenance["replayed_records"], 1)
        self.assertNotIn(str(self.root), json.dumps(manifest))
        pins = {p["path"] for p in manifest["input_pins"]}
        self.assertTrue({"scripts/data_role_evidence_v2.py", "scripts/pointer_evidence_loader.py", "scripts/scan_pointer_roles.py"} <= pins)

    def test_pointer_decoder_header_and_replay_failure_refuse_output(self):
        evidence, loader, _ = self.pointer_fixture()
        report = json.loads(evidence.read_bytes())
        report["decoder_dependency_sha256"] = "4" * 64
        evidence.write_text(json.dumps(report))
        with patch.dict(sys.modules, {"pointer_evidence_loader": loader}), self.assertRaisesRegex(ValueError, "source pin mismatch"):
            self.build(pointer_evidence=evidence)
        self.assertFalse(loader.load_pointer_evidence.called)
        self.assertFalse(self.output.exists())
        report["decoder_dependency_sha256"] = self.normalizer_sha
        evidence.write_text(json.dumps(report))
        loader.load_pointer_evidence.side_effect = ValueError("Stored pointer-role witnesses differ from full theorem replay")
        with patch.dict(sys.modules, {"pointer_evidence_loader": loader}), self.assertRaisesRegex(ValueError, "full theorem replay"):
            self.build(pointer_evidence=evidence)
        self.assertFalse(self.output.exists())

    def test_pointer_artifact_and_decoder_snapshot_changes_refuse_output(self):
        evidence, loader, _ = self.pointer_fixture()
        def mutated(data, address, context):
            evidence.write_bytes(evidence.read_bytes() + b" ")
            return self.normalize(data, address, context)
        with patch.dict(sys.modules, {"pointer_evidence_loader": loader}), self.assertRaisesRegex(ValueError, "Pointer evidence changed"):
            self.build(normalize=mutated, pointer_evidence=evidence)
        self.assertFalse(self.output.exists())
        evidence, loader, _ = self.pointer_fixture()
        def changed_during_replay(*args, **kwargs):
            (self.repo / "scripts/data_role_evidence_v2.py").write_bytes(b"changed proof implementation")
            return {"boot": []}
        loader.load_pointer_evidence.side_effect = changed_during_replay
        with patch.dict(sys.modules, {"pointer_evidence_loader": loader}), self.assertRaisesRegex(ValueError, "inputs changed"):
            self.build(pointer_evidence=evidence)
        self.assertFalse(self.output.exists())

    def test_static_external_targets_include_unmasked_branches_and_regimm(self):
        import struct
        address = 0x1000
        words = [(3 << 26) | (0x2000 >> 2), 0, (4 << 26) | 0x100, 0,
                 (1 << 26) | (16 << 16) | 0x200, 0,
                 (2 << 26) | ((address + 4) >> 2), 0]
        dependencies = builder.static_dependencies(struct.pack('<8I', *words), address)
        self.assertEqual(dependencies, [{"offset": 0, "kind": "call", "target": 0x2000},
                                      {"offset": 8, "kind": "branch", "target": address + 12 + 0x400},
                                      {"offset": 16, "kind": "branch", "target": address + 20 + 0x800}])

    def test_static_cop0_branch_family_is_complete_without_decoder_change(self):
        import struct
        address = 0x1000
        for rt in range(4):
            word = (0x10 << 26) | (8 << 21) | (rt << 16) | 0x100
            self.assertIsNone(builder.identity._control(word, 0, address, 2))
            self.assertEqual(builder.static_dependencies(struct.pack('<2I', word, 0), address),
                             [{"offset": 0, "kind": "branch", "target": address + 4 + 0x400}])


if __name__ == "__main__":
    unittest.main()
