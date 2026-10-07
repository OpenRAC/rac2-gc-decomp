"""Transactional finalization tests with synthetic action artifacts and tool runners."""
import copy
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import campaign
import campaign_finalize as finalize
import boot_sdk_unit


def sha(data):
    return hashlib.sha256(data).hexdigest()


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(finalize._encoded(value) if not isinstance(value, bytes) else value)


class FinalizeTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.bank = Path(self.temporary.name)
        self.repo = self.bank / "repo"
        self.runtime = self.bank / "runtime"
        self.repo.mkdir()
        self.runtime.mkdir()
        self.action = "a" * 32
        self.output = self.bank / "delivery"
        self.reference_root = self.bank / "references"
        self.build = self.bank / ("campaign-" + self.action)
        self.tools = self.bank / "toolchain"
        self.c_tools = {"cc1": "c" * 64, "cpp": "d" * 64, "as": "e" * 64, "ld.exe": "f" * 64}
        self.levels = ["level_%02d" % index for index in range(27)]
        for directory in ("scripts", "tests", "src", "candidates", "config", "progress", "docs"):
            (self.repo / directory).mkdir()
        scripts = ("check_candidates.py", "wsl_chain.py", "campaign.py", "build_unique_catalog.py", "unique_code_report.py",
                   "relocation_identity.py", "verify_boot_bindings.py", "validate_boot_binding.py", "elf_tools.py")
        for name in scripts:
            write(self.repo / "scripts" / name, b"# maintained tool fixture\n")
        write(self.repo / "tests/test_fixture.py", b"# tool tests fixture\n")
        write(self.repo / "candidates/boot.c", b"void exact(void) {}\n")
        write(self.repo / "config/candidate-catalog.json", {"functions": []})
        write(self.repo / "progress/candidates.json", {"flags": ["-O2"], "tools": self.c_tools})
        write(self.repo / "README.md", b"fixture progress\n")
        write(self.repo / "config/function-catalog/catalog.json", {"old": True})
        write(self.repo / "config/function-catalog/boot.ndjson.gz", finalize._compress(b"[]\n"))
        write(self.repo / "config/function-evidence/boot-bindings.json.gz", finalize._compress(b"{}\n"))
        write(self.repo / "config/function-evidence/pointer-arguments.json.gz", finalize._compress(b"{}\n"))
        pins, references, gates = {}, {}, {}
        for program in ["boot", *self.levels]:
            binary = "boot.elf" if program == "boot" else "overlay.elf"
            data = ("reference-" + program).encode()
            ref = self.reference_root / (binary if program == "boot" else "levels/" + program + "/" + binary)
            write(ref, data)
            pins[program] = sha(data)
            references[program] = {"path": str(ref), "sha256": pins[program]}
            if program != "boot":
                references[program]["level"] = program
            candidate = ("candidate-" + program).encode()
            row = {"matched": True, "reference_sha256": pins[program], "candidate_sha256": sha(candidate),
                   "integrated_c_functions": 1, "integrated_c_bytes": 4}
            gates[program] = row
            proof = {"program": program, "reference_sha256": pins[program], "matched_code_bytes": 4,
                     "state": "integrated", "functions": [{"symbol": "exact", "address": 4096, "size": 4}]}
            write(self.build / program / "integration.json", proof)
            write(self.build / program / "gate.json", row)
            write(self.build / program / "build" / binary, candidate)
            write(self.build / program / "assets" / binary, data)
            write(self.build / program / "asm/body.s", b"private generated assembly\n")
        write(self.repo / "config/target.json", {"serial": "TEST", "boot": {"sha256": pins["boot"]}, "expected_levels": 27})
        write(self.repo / "config/overlays.json", {"levels": [{"level": p, "sha256": pins[p]} for p in self.levels]})
        self.manifest = self.bank / "manifest.json"
        write(self.manifest, {"target": "TEST", "boot": references["boot"], "overlays": [references[p] for p in self.levels]})
        for name in ("Ps2EeAs.exe", "ld.exe"):
            write(self.tools / "ee/bin" / name, name.encode())
        self.instruments = campaign.instrument_hashes(self.repo)
        gate = {"schema": 1, "batch_id": self.action, "target": "TEST", "matched": True, "failures": [],
                "g1": dict(gates["boot"], program="boot"),
                "g3": [dict(gates[p], program=p, level=p) for p in self.levels],
                "tools": {n: sha(n.encode()) for n in ("Ps2EeAs.exe", "ld.exe")},
                "input_sha256": self.instruments, "manifest_sha256": sha(self.manifest.read_bytes()),
                "verified_at": "2026-10-07T12:00:00+00:00"}
        write(self.build / "report.json", gate)
        work = self.runtime / "actions" / self.action
        command = [sys.executable, "scripts/campaign_build.py", "--manifest", str(self.manifest),
                   "--toolchain", str(self.tools), "--c-toolchain", str(self.tools), "--batch-id", self.action]
        write(work / "manifest.json", {"id": self.action, "kind": "integrate", "command": command,
            "instruments": self.instruments, "external_input_sha256": {str(self.manifest): sha(self.manifest.read_bytes())}})
        self.outcome = {"id": self.action, "kind": "integrate", "state": "passed", "returncode": 0,
                        "artifact": str(self.build / "report.json"), "artifact_sha256": sha((self.build / "report.json").read_bytes())}
        write(work / "outcome.json", self.outcome)
        action = dict(self.outcome, directory="runtime:actions/" + self.action)
        action.pop("artifact")
        action["action_manifest_sha256"] = sha((work / "manifest.json").read_bytes())
        action["action_outcome_sha256"] = sha((work / "outcome.json").read_bytes())
        self.register = {"schema": 1, "kind": "rac2-campaign", "revision": 1, "actions": {self.action: action},
                         "tasks": {"candidate": {"kind": "candidate", "state": "exact_private", "targets": []},
                                   "research": {"kind": "research", "state": "stopped", "targets": []}},
                         "trials": {"negative": {"state": "failed", "reason": "retained refusal"}}, "legacy_documents": {}}
        write(self.repo / "config/campaign-register.json", self.register)
        self.store = campaign.Store(self.repo / "config/campaign-register.json", self.runtime)
        self.calls = []
        self.fail_script = None
        self.leak = False
        self.drift = False

    def runner(self, command, **kwargs):
        self.assertIsInstance(command, list)
        self.assertNotIn("shell", kwargs)
        self.calls.append(command)
        mirror = Path(kwargs["cwd"])
        if command[1] == "-c":
            if "_validate_boot_summary" in command[2]:
                write(Path(command[-1]), {"expected_edges": 1, "validated_edges": 1, "programs": 28, "blocked": 0})
            else:
                write(Path(command[-1]), self.c_tools)
            return subprocess.CompletedProcess(command, 0)
        if command[1] == "-m":
            return subprocess.CompletedProcess(command, 0)
        script = Path(command[1]).name
        if script == self.fail_script:
            return subprocess.CompletedProcess(command, 1)
        args = command[2:]
        def option(name):
            return Path(args[args.index(name) + 1])
        check = "--check" in args
        if script == "decomp_report.py":
            write(option("--output"), {"measures": {"matchedCode": "112", "totalCode": "400"}})
        elif script == "readme_progress.py" and not check:
            write(mirror / "progress/decompilation.svg", b"<svg>physical</svg>\n")
            write(mirror / "README.md", b"physical progress\n")
        elif script == "global_function_catalog.py":
            write(option("--output"), {"qualified_extent_failures": [], "functions": [], "summary": {"rows": 28}})
        elif script == "scan_pointer_roles.py":
            write(option("--output"), {"records": []})
        elif script == "build_unique_catalog.py":
            destination = option("--output")
            write(destination / "boot.ndjson.gz", finalize._compress(b"[]\n"))
            write(destination / "catalog.json", {"function_chunks": [{"path": "boot.ndjson.gz"}],
                "input_pins": [], "generation": {"reconstruction_checks": 28}})
            write(option("--diagnostics"), {"failures": []})
        elif script == "verify_boot_bindings.py":
            write(option("--output"), {"blocked": [], "source_catalog_sha256": "a" * 64})
        elif script == "unique_code_report.py" and not check:
            value = {"metrics": {"unique_total_bytes": 100, "unique_matched_bytes": 4}}
            if self.leak:
                value["private"] = "C:/Users/private/build.elf"
            write(option("--output"), value)
            write(option("--objdiff-output"), {"private_export": True})
        elif script == "readme_unique_progress.py" and not check:
            write(mirror / "progress/paired-code-metrics.json", {"physical": 112, "unique": 4})
            write(mirror / "progress/unique-decompilation.svg", b"<svg>unique</svg>\n")
            write(mirror / "README.md", b"physical and unique progress\n")
        elif script == "code_reuse_report.py" and not check:
            write(option("--output"), {"metrics": {"template_total_bytes": 100}})
            write(option("--families-output"), finalize._compress(b"{}\n"))
        elif script == "campaign.py" and not check:
            register = json.loads(option("--registry").read_bytes())
            if "close" in args:
                register["tasks"][args[-1]]["state"] = "integrated"
            else:
                write(mirror / "docs/CAMPAIGN-QUEUE.md", b"generated queue\n")
                write(mirror / "docs/C-NATIVE-EXPERIMENT-REGISTER.md", b"retained history\n")
            register["revision"] += 1
            write(option("--registry"), register)
        elif script == "source_layout.py" and "--inventory-output" in args:
            write(option("--inventory-output"), {"integration_credit_added": 0})
        if self.drift and script == "source_layout.py":
            write(self.repo / "candidates/boot.c", b"concurrent source edit\n")
        return subprocess.CompletedProcess(command, 0)

    def finish(self, **kwargs):
        return finalize.finalize(self.store, self.repo, self.action, manifest=self.manifest,
                                 output=self.output, runner=self.runner, **kwargs)

    def rewrite_gate(self, callback):
        gate = json.loads((self.build / "report.json").read_bytes())
        callback(gate)
        write(self.build / "report.json", gate)
        self.outcome["artifact_sha256"] = sha((self.build / "report.json").read_bytes())
        write(self.runtime / "actions" / self.action / "outcome.json", self.outcome)
        register = self.store.load()
        register["actions"][self.action]["artifact_sha256"] = self.outcome["artifact_sha256"]
        register["actions"][self.action]["action_outcome_sha256"] = sha((self.runtime / "actions" / self.action / "outcome.json").read_bytes())
        write(self.store.path, register)

    def configure_sdk_owners(self):
        """Represent the real keyed boot-owner schema and private SDK closure."""
        owners = {}
        for identifier, spec in boot_sdk_unit.UNITS.items():
            owners[identifier] = {"unit_id": identifier, "source": spec["source"], "module": spec["module"],
                "catalog_path": spec["catalog"], "review_path": spec["review"], "review_sha256": "a" * 64,
                "profile_id": boot_sdk_unit.cp.SDK_PROFILE, "input_section": ".text", "object_proof": {}}
            write(self.repo / spec["catalog"], {"unit_id": identifier})
            for filename in ("invocation.json", "inputs-before.json", "private-binding.json"):
                write(self.build / "boot/build/c/sdk" / identifier / filename, {"unit_id": identifier, "kind": filename})
        integration = json.loads((self.build / "boot/integration.json").read_bytes())
        integration.update(schema=3, kind="boot-c-owner-integration", sdk_units=owners, default={"owner": "default"})
        write(self.build / "boot/integration.json", integration)
        self.instruments = campaign.instrument_hashes(self.repo)
        manifest_path = self.runtime / "actions" / self.action / "manifest.json"
        manifest = json.loads(manifest_path.read_bytes())
        manifest["instruments"] = self.instruments
        write(manifest_path, manifest)
        self.rewrite_gate(lambda gate: gate.update(input_sha256=self.instruments))
        register = self.store.load()
        register["actions"][self.action]["action_manifest_sha256"] = sha(manifest_path.read_bytes())
        write(self.store.path, register)
        return owners

    def test_dry_run_is_read_only_and_runs_all_maintained_checks(self):
        before = finalize._inventory(self.repo)
        result = self.finish(tasks=["candidate", "research"])
        self.assertEqual(result["state"], "prepared")
        self.assertEqual(before, finalize._inventory(self.repo))
        self.assertFalse(list(self.repo.rglob("*.tmp")))
        self.assertEqual(result["closed_candidates"], ["candidate"])
        self.assertEqual(result["retained_research_tasks"], ["research"])
        self.assertTrue(any(call[1:3] == ["-m", "unittest"] for call in self.calls))
        self.assertTrue(any(Path(c[1]).name == "scan_pointer_roles.py" for c in self.calls))
        self.assertTrue(any(Path(c[1]).name == "code_reuse_report.py" and "--check" in c for c in self.calls))

    def test_apply_same_output_is_atomic_idempotent_and_keeps_private_exports_out(self):
        self.finish(tasks=["candidate", "research"])
        calls = len(self.calls)
        result = self.finish(tasks=["candidate", "research"], apply=True)
        self.assertEqual(result["state"], "applied")
        self.assertEqual(len(self.calls), calls + 1)  # Live tool observation only; no recipe rerun.
        register = self.store.load()
        self.assertEqual(register["tasks"]["candidate"]["state"], "integrated")
        self.assertEqual(register["tasks"]["research"]["state"], "stopped")
        self.assertEqual(register["trials"], self.register["trials"])
        self.assertFalse((self.repo / "progress/objdiff.json").exists())
        self.assertFalse((self.repo / "progress/unique-objdiff.json").exists())
        plan = json.loads((self.output / "plan.json").read_bytes())
        for row in plan["changes"]:
            self.assertEqual(sha((self.repo / row["path"]).read_bytes()), row["after_sha256"])
            if row["before_sha256"]:
                self.assertEqual(sha((self.output / "backups" / row["path"]).read_bytes()), row["before_sha256"])
        self.assertEqual(self.finish(tasks=["candidate", "research"], apply=True)["state"], "applied")

    def test_failed_and_incomplete_gates_are_rejected_before_snapshot(self):
        for edit in (lambda gate: gate.update(matched=False), lambda gate: gate["g3"].pop()):
            with self.subTest(edit=edit):
                original = (self.build / "report.json").read_bytes()
                self.rewrite_gate(edit)
                with self.assertRaisesRegex(ValueError, "Incomplete or failed"):
                    self.finish()
                self.assertFalse(self.output.exists())
                self.rewrite_gate(lambda gate: gate.update(json.loads(original)))

    def test_unregistered_or_failed_action_refused(self):
        register = self.store.load()
        register["actions"][self.action]["state"] = "failed"
        write(self.store.path, register)
        with self.assertRaisesRegex(ValueError, "registered passed"):
            self.finish()

    def test_immutable_artifact_and_manifest_pins_checked(self):
        write(self.build / "report.json", {"matched": True})
        with self.assertRaisesRegex(ValueError, "artifact hash"):
            self.finish()

    def test_source_and_profile_drift_before_preparation_refused(self):
        write(self.repo / "progress/candidates.json", {"flags": ["-O3"]})
        with self.assertRaisesRegex(ValueError, "changed after"):
            self.finish()

    def test_source_drift_during_preparation_preserved_and_refused(self):
        self.drift = True
        with self.assertRaisesRegex(ValueError, "changed during private"):
            self.finish()
        self.assertEqual((self.repo / "candidates/boot.c").read_bytes(), b"concurrent source edit\n")
        self.assertFalse((self.output / "plan.json").exists())

    def test_concurrent_new_source_after_plan_refused(self):
        self.finish()
        write(self.repo / "src/new.c", b"int new_source;\n")
        with self.assertRaisesRegex(ValueError, "Concurrent"):
            self.finish(apply=True)

    def test_changed_private_gate_after_plan_refused(self):
        self.finish()
        write(self.build / "boot/gate.json", {"matched": False})
        with self.assertRaisesRegex(ValueError, "Private action evidence changed"):
            self.finish(apply=True)

    def test_failed_maintained_check_never_publishes(self):
        before = finalize._inventory(self.repo)
        self.fail_script = "code_reuse_report.py"
        with self.assertRaisesRegex(ValueError, "check failed"):
            self.finish(apply=True)
        self.assertEqual(before, finalize._inventory(self.repo))
        self.assertTrue((self.output / "failed-checks.json").exists())

    def test_private_leak_in_generated_metadata_refused(self):
        self.leak = True
        with self.assertRaisesRegex(ValueError, "private paths"):
            self.finish(apply=True)

    def test_output_inside_repository_refused(self):
        self.output = self.repo / "work/finalization"
        with self.assertRaisesRegex(ValueError, "outside"):
            self.finish()

    def test_changed_staged_after_image_refused(self):
        self.finish()
        write(self.output / "snapshot/README.md", b"unreviewed stage edit\n")
        with self.assertRaisesRegex(ValueError, "Prepared output changed"):
            self.finish(apply=True)

    def test_failed_atomic_replace_rolls_back_all_written_files(self):
        self.finish()
        before = finalize._inventory(self.repo)
        real_replace = finalize._capture_replace
        attempts = []
        def fail_second_public(destination, replacement, capture):
            destination = Path(destination)
            if destination.is_relative_to(self.repo):
                attempts.append(destination)
                if len(attempts) == 2:
                    raise OSError("injected filesystem failure")
            return real_replace(destination, replacement, capture)
        with patch.object(finalize, "_capture_replace", side_effect=fail_second_public):
            with self.assertRaisesRegex(OSError, "injected"):
                self.finish(apply=True)
        self.assertEqual(before, finalize._inventory(self.repo))
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "rolled_back")
        self.assertFalse(list(self.repo.rglob("*.tmp")))
        self.assertEqual(self.finish(apply=True)["state"], "applied")

    def test_resume_interrupted_replace_infers_after_image(self):
        self.finish()
        plan = json.loads((self.output / "plan.json").read_bytes())
        row = plan["changes"][0]
        name = row["path"]
        if row["before_sha256"]:
            write(self.output / "backups" / name, (self.repo / name).read_bytes())
        write(self.repo / name, (self.output / "snapshot" / name).read_bytes())
        journal = json.loads((self.output / "journal.json").read_bytes())
        journal["state"] = "publishing"
        write(self.output / "journal.json", journal)
        self.assertEqual(self.finish(apply=True)["state"], "applied")

    def test_post_apply_source_edit_invalidates_idempotent_receipt(self):
        self.finish(apply=True)
        write(self.repo / "candidates/boot.c", b"changed\n")
        with self.assertRaisesRegex(ValueError, "Concurrent"):
            self.finish(apply=True)

    def test_compressed_private_leaks_and_credentials_refused(self):
        with self.assertRaisesRegex(ValueError, "private paths"):
            finalize._public_metadata("config/function-evidence/pointer-arguments.json.gz",
                                      finalize._compress(b'{"path":"D:/private/reference.elf"}\n'))
        with self.assertRaisesRegex(ValueError, "credential"):
            finalize._public_metadata("progress/report.json", b'{"access_token":"secret"}\n')

    def test_imported_history_preserved_without_allowing_new_private_paths(self):
        original = {"legacy_documents": {"old": {"original_utf8": "Old D:\\private\\work convention"}}, "tasks": {}}
        data = finalize._encoded(original)
        finalize._public_metadata("config/campaign-register.json", data, data)
        changed = copy.deepcopy(original)
        changed["tasks"]["new"] = {"pointer": "D:/private/new"}
        with self.assertRaisesRegex(ValueError, "private paths"):
            finalize._public_metadata("config/campaign-register.json", finalize._encoded(changed), data)
        changed = copy.deepcopy(original)
        changed["legacy_documents"]["old"]["original_utf8"] += " rewritten"
        with self.assertRaisesRegex(ValueError, "immutable imported history"):
            finalize._public_metadata("config/campaign-register.json", finalize._encoded(changed), data)
        finalize._public_metadata("progress/report.json", b'{"source":"https://example.com/reference","note":"owner/root/clamps"}\n')

    def test_missing_registered_manifest_or_outcome_pin_refused(self):
        register = self.store.load()
        del register["actions"][self.action]["action_outcome_sha256"]
        write(self.store.path, register)
        with self.assertRaisesRegex(ValueError, "registered immutable outcome"):
            self.finish()

    def test_current_compiler_drift_after_preparation_refused(self):
        self.finish()
        self.c_tools = dict(self.c_tools, cc1="0" * 64)
        with self.assertRaisesRegex(ValueError, "C/SDK instruments changed"):
            self.finish(apply=True)

    def test_mid_publication_concurrent_write_is_retained_as_conflict(self):
        self.finish()
        real_atomic = finalize._checked_replace
        changed = []
        def concurrent_write(path, data, expected, bank, kind="replace"):
            real_atomic(path, data, expected, bank, kind)
            if Path(path).is_relative_to(self.repo) and not changed:
                changed.append(Path(path))
                Path(path).write_bytes(b"unreviewed concurrent write\n")
        with patch.object(finalize, "_checked_replace", side_effect=concurrent_write):
            with self.assertRaisesRegex(ValueError, "Concurrent"):
                self.finish(apply=True)
        self.assertEqual(changed[0].read_bytes(), b"unreviewed concurrent write\n")
        journal = json.loads((self.output / "journal.json").read_bytes())
        self.assertEqual(journal["state"], "conflict")
        with self.assertRaisesRegex(ValueError, "concurrent conflicts"):
            self.finish(apply=True)

    def test_output_in_foreign_git_directory_or_worktree_refused(self):
        for kind in ("directory", "worktree-file"):
            with self.subTest(kind=kind):
                foreign = self.bank / kind
                foreign.mkdir()
                if kind == "directory":
                    (foreign / ".git").mkdir()
                else:
                    write(foreign / ".git", b"gitdir: ../owner/.git/worktrees/foreign\n")
                self.output = foreign / "nested/private-output"
                with self.assertRaisesRegex(ValueError, "every Git checkout"):
                    self.finish()
                self.assertFalse(self.output.exists())

    def test_private_reference_git_input_is_allowed(self):
        write(self.reference_root / ".git", b"gitdir: private-reference-metadata\n")
        self.assertEqual(self.finish()["state"], "prepared")

    def test_compressed_ndjson_credentials_and_secret_values_refused(self):
        cases = [b'{"nested":[{"api_key":"secret"}]}\n',
                 finalize._encoded({"note": "ghp_" + "a" * 36}),
                 finalize._encoded(["github_pat_" + "b" * 40]),
                 finalize._encoded({"note": "-----BEGIN PRIVATE KEY-----"})]
        for payload in cases:
            with self.subTest(payload=payload[:30]):
                with self.assertRaisesRegex(ValueError, "credential|secret value"):
                    finalize._public_metadata("config/function-catalog/boot.ndjson.gz", finalize._compress(payload))
        for filename in ("README.md", "progress/report.json"):
            payload = b"-----BEGIN RSA PRIVATE KEY-----\n" if filename.endswith(".md") else b'{"note":"github_pat_aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}\n'
            with self.assertRaisesRegex(ValueError, "secret value"):
                finalize._public_metadata(filename, payload)

    def test_boot_edge_summary_requires_complete_nonvacuous_validated_receipt(self):
        catalog = {"programs": [{"program": "boot"}, {"program": "levels/test"}], "functions": [
            {"id": "callee", "program": "boot", "address": 4096},
            {"id": "caller", "program": "levels/test", "address": 8192,
             "call_dependencies": [{"target": 4096, "offset": 0, "kind": "call"}]}]}
        proof = {"programs": catalog["programs"], "boot_core": {"address": 4096, "size": 16},
                 "blocked": [], "target_bodies": [{"id": "callee"}], "bindings": [{"program": "levels/test"}],
                 "per_program": {"levels/test": {"bindings": 1, "blocked": 0, "static_core_load_disjoint": True}}}
        edges = {("caller", 0, "call", 4096): "callee"}
        self.assertEqual(finalize._check_boot_edges(proof, catalog, edges)["validated_edges"], 1)
        for edit in (lambda p: p["bindings"].clear(),
                     lambda p: p["per_program"]["levels/test"].update(bindings=0),
                     lambda p: p["programs"].pop(), lambda p: p.pop("blocked")):
            candidate = copy.deepcopy(proof)
            edit(candidate)
            with self.assertRaises(ValueError):
                finalize._check_boot_edges(candidate, catalog, edges)
        with self.assertRaisesRegex(ValueError, "vacuous or omits"):
            finalize._check_boot_edges(proof, catalog, {})

    def test_independent_boot_validator_is_mandatory(self):
        self.finish()
        self.assertTrue(any(c[1] == "-c" and "_validate_boot_summary" in c[2] for c in self.calls))

    def retained_versions(self):
        return [path.read_bytes() for path in (self.output / "publication-operations").rglob("*.bin")]

    def test_writer_between_final_guard_and_native_replace_is_captured_and_restored(self):
        self.finish()
        native = finalize._capture_replace
        raced = []
        concurrent = b"writer in the exact guard-to-replace window\n"
        def race(destination, replacement, capture):
            if not raced:
                raced.append(Path(destination))
                Path(destination).write_bytes(concurrent)
            return native(destination, replacement, capture)
        with patch.object(finalize, "_capture_replace", side_effect=race):
            with self.assertRaisesRegex(finalize.PublicationConflict, "Concurrent predecessor"):
                self.finish(apply=True)
        self.assertEqual(raced[0].read_bytes(), concurrent)
        self.assertIn(concurrent, self.retained_versions())
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "conflict")
        self.assertEqual(json.loads((self.output / "receipt.json").read_bytes())["state"], "prepared")

    def test_writer_in_rollback_guard_to_replace_window_is_preserved(self):
        self.finish()
        native = finalize._capture_replace
        calls, raced = [], []
        concurrent = b"writer immediately before rollback replaces README\n"
        def race(destination, replacement, capture):
            calls.append(Path(destination))
            if len(calls) == 2:
                raise OSError("force rollback after the first public replacement")
            if len(calls) == 3:
                raced.append(Path(destination))
                Path(destination).write_bytes(concurrent)
            return native(destination, replacement, capture)
        with patch.object(finalize, "_capture_replace", side_effect=race):
            with self.assertRaisesRegex(OSError, "force rollback"):
                self.finish(apply=True)
        self.assertEqual(raced[0].read_bytes(), concurrent)
        self.assertIn(concurrent, self.retained_versions())
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "conflict")

    def test_initially_absent_destination_is_created_exclusively(self):
        destination = self.repo / "new.json"
        bank = self.bank / "operations"
        native = finalize._move_noreplace
        concurrent = b"writer creates destination before exclusive move\n"
        def race(source, target):
            Path(target).write_bytes(concurrent)
            return native(source, target)
        with patch.object(finalize, "_move_noreplace", side_effect=race):
            with self.assertRaisesRegex(finalize.PublicationConflict, "created the publication"):
                finalize._checked_replace(destination, b"reviewed new metadata\n", None, bank)
        self.assertEqual(destination.read_bytes(), concurrent)

    def test_new_file_rollback_captures_concurrent_predecessor(self):
        destination = self.repo / "new.json"
        installed = b"our newly published metadata\n"
        concurrent = b"writer before new-file rollback move\n"
        destination.write_bytes(installed)
        bank = self.bank / "operations"
        native = finalize._move_noreplace
        raced = []
        def race(source, target):
            if Path(source) == destination and not raced:
                raced.append(True)
                destination.write_bytes(concurrent)
            return native(source, target)
        with patch.object(finalize, "_move_noreplace", side_effect=race):
            with self.assertRaisesRegex(finalize.PublicationConflict, "rollback predecessor"):
                finalize._checked_remove(destination, sha(installed), bank)
        self.assertEqual(destination.read_bytes(), concurrent)
        self.assertIn(concurrent, [p.read_bytes() for p in bank.rglob("*.bin")])

    def test_conflict_restoration_captures_a_second_racing_writer(self):
        destination = self.repo / "README.md"
        original = destination.read_bytes()
        bank = self.bank / "operations"
        native = finalize._capture_replace
        writers = [b"first concurrent writer\n", b"second writer racing restoration\n"]
        count = []
        def race(target, replacement, capture):
            if len(count) < len(writers):
                Path(target).write_bytes(writers[len(count)])
                count.append(True)
            return native(target, replacement, capture)
        with patch.object(finalize, "_capture_replace", side_effect=race):
            with self.assertRaises(finalize.PublicationConflict):
                finalize._checked_replace(destination, b"our metadata\n", sha(original), bank)
        self.assertEqual(destination.read_bytes(), writers[-1])
        retained = [p.read_bytes() for p in bank.rglob("*.bin")]
        for version in writers:
            self.assertIn(version, retained)

    @unittest.skipUnless(sys.platform == "win32", "Windows ReplaceFileW partial-failure contract")
    def test_windows_partial_failure_keeps_displaced_image_and_restores_absence(self):
        destination = self.repo / "README.md"
        original = destination.read_bytes()
        bank = self.bank / "operations"
        def partial_failure(target, replacement, capture):
            os.rename(target, capture)  # ERROR_UNABLE_TO_MOVE_REPLACEMENT_2 documented state.
            raise OSError(1177, "replacement not moved; original is at backup")
        with patch.object(finalize, "_capture_replace", side_effect=partial_failure):
            with self.assertRaisesRegex(finalize.PublicationConflict, "failed after displacing"):
                finalize._checked_replace(destination, b"our metadata\n", sha(original), bank)
        self.assertEqual(destination.read_bytes(), original)
        self.assertIn(original, [p.read_bytes() for p in bank.rglob("*.bin")])

    def test_unsupported_backend_refuses_before_any_public_write(self):
        self.finish()
        before = finalize._inventory(self.repo)
        with patch.object(finalize, "_atomic_backend", side_effect=ValueError("unsupported platform")):
            with self.assertRaisesRegex(ValueError, "unsupported"):
                self.finish(apply=True)
        self.assertEqual(finalize._inventory(self.repo), before)
        self.assertFalse((self.repo / "config/campaign-register.json.lock").exists())

    def test_interrupted_unvalidated_capture_never_becomes_applied(self):
        self.finish()
        plan = json.loads((self.output / "plan.json").read_bytes())
        row = plan["changes"][0]
        destination = self.repo / row["path"]
        data = (self.output / "snapshot" / row["path"]).read_bytes()
        directory, replacement, capture, record = finalize._operation(self.output / "publication-operations", destination,
                                                                     data, row["before_sha256"], "replace")
        concurrent = b"writer displaced just before process termination\n"
        destination.write_bytes(concurrent)
        finalize._capture_replace(destination, replacement, capture)
        with self.assertRaisesRegex(finalize.PublicationConflict, "Interrupted atomic"):
            self.finish(apply=True)
        self.assertEqual(destination.read_bytes(), concurrent)
        self.assertIn(concurrent, self.retained_versions())
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "conflict")

    def test_interrupted_predecessor_equal_to_proposed_bytes_is_still_a_conflict(self):
        self.finish()
        plan = json.loads((self.output / "plan.json").read_bytes())
        row = plan["changes"][0]
        destination = self.repo / row["path"]
        proposed = (self.output / "snapshot" / row["path"]).read_bytes()
        directory, replacement, capture, record = finalize._operation(self.output / "publication-operations", destination,
                                                                     proposed, row["before_sha256"], "replace")
        destination.write_bytes(proposed)  # A concurrent predecessor with identical desired bytes.
        finalize._capture_replace(destination, replacement, capture)
        with self.assertRaisesRegex(finalize.PublicationConflict, "Interrupted atomic"):
            self.finish(apply=True)
        self.assertEqual(destination.read_bytes(), proposed)
        self.assertIn(proposed, self.retained_versions())
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "conflict")

    @unittest.skipUnless(sys.platform == "win32", "Windows ReplaceFileW non-mutating failure contract")
    def test_windows_remove_and_move_failures_do_not_lose_the_original(self):
        for number in (1175, 1176):
            destination = self.repo / "README.md"
            original = destination.read_bytes()
            with self.subTest(error=number), patch.object(finalize, "_capture_replace", side_effect=OSError(number, "native failure")):
                with self.assertRaises(OSError):
                    finalize._checked_replace(destination, b"new metadata\n", sha(original), self.bank / "operations")
            self.assertEqual(destination.read_bytes(), original)

    def test_interrupted_exclusive_create_cannot_claim_identical_concurrent_file(self):
        self.finish()
        plan = json.loads((self.output / "plan.json").read_bytes())
        row = next(row for row in plan["changes"] if row["before_sha256"] is None)
        destination = self.repo / row["path"]
        data = (self.output / "snapshot" / row["path"]).read_bytes()
        directory, replacement, capture, record = finalize._operation(self.output / "publication-operations", destination,
                                                                     data, None, "replace")
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(data)  # Another writer wins with identical bytes.
        with self.assertRaises(FileExistsError):
            finalize._move_noreplace(replacement, destination)
        # Simulate termination before the failure result was recorded.
        with self.assertRaisesRegex(finalize.PublicationConflict, "Interrupted operation"):
            self.finish(apply=True)
        self.assertEqual(destination.read_bytes(), data)
        self.assertTrue(replacement.exists())
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "conflict")

    def test_interrupted_before_native_replace_cannot_claim_identical_concurrent_file(self):
        self.finish()
        plan = json.loads((self.output / "plan.json").read_bytes())
        row = plan["changes"][0]
        destination = self.repo / row["path"]
        data = (self.output / "snapshot" / row["path"]).read_bytes()
        write(self.output / "backups" / row["path"], destination.read_bytes())
        finalize._operation(self.output / "publication-operations", destination, data, row["before_sha256"], "replace")
        destination.write_bytes(data)  # Another writer, before our native call ever occurs.
        with self.assertRaisesRegex(finalize.PublicationConflict, "Interrupted operation"):
            self.finish(apply=True)
        self.assertEqual(destination.read_bytes(), data)
        self.assertEqual(json.loads((self.output / "journal.json").read_bytes())["state"], "conflict")

    def test_keyed_sdk_owners_are_pinned_and_survive_the_complete_finalizer_path(self):
        owners = self.configure_sdk_owners()
        evidence = finalize._preflight(self.store, self.repo, self.action, self.manifest, None, ())
        for identifier in owners:
            for filename in ("invocation.json", "inputs-before.json", "private-binding.json"):
                path = self.build / "boot/build/c/sdk" / identifier / filename
                self.assertEqual(evidence["external_pins"][str(path)], sha(path.read_bytes()))
        self.assertFalse(self.output.exists())
        before = finalize._inventory(self.repo)
        self.assertEqual(self.finish()["state"], "prepared")
        self.assertEqual(finalize._inventory(self.repo), before)
        self.assertEqual(self.finish(apply=True)["state"], "applied")
        published = json.loads((self.repo / "progress/integration.json").read_bytes())
        self.assertEqual(published["sdk_units"], owners)

    def test_sdk_owner_map_key_must_agree_with_its_descriptor(self):
        owners = self.configure_sdk_owners()
        keys = list(owners)
        owners[keys[0]], owners[keys[1]] = owners[keys[1]], owners[keys[0]]
        with self.assertRaisesRegex(ValueError, "map key/descriptor mismatch"):
            finalize._sdk_owners({"sdk_units": owners}, self.repo)

    def test_missing_admitted_sdk_owner_and_list_schema_are_refused(self):
        owners = self.configure_sdk_owners()
        missing = dict(owners)
        missing.pop(next(iter(missing)))
        for invalid in ({}, missing, list(owners.values())):
            with self.subTest(kind=type(invalid).__name__), self.assertRaisesRegex(ValueError, "Owner schema fields mismatch"):
                finalize._sdk_owners({"sdk_units": invalid}, self.repo)

    def test_sdk_runtime_metadata_drift_after_prepare_is_refused(self):
        owners = self.configure_sdk_owners()
        self.finish()
        identifier = next(iter(owners))
        write(self.build / "boot/build/c/sdk" / identifier / "inputs-before.json", {"changed": True})
        with self.assertRaisesRegex(ValueError, "Private action evidence changed"):
            self.finish(apply=True)

    def test_legacy_boot_without_sdk_owners_remains_supported(self):
        self.assertEqual(finalize._sdk_owners({"program": "boot"}, self.repo), {})

    def add_verification_metadata(self):
        values = {}
        for name in finalize.SNAPSHOT_VERIFICATION_INPUTS:
            values[name] = finalize._compress(finalize._encoded({"kind": "retained-verification", "name": name}))
            write(self.repo / name, values[name])
        return values

    def test_maintained_verification_gzip_is_snapshotted_unchanged_and_not_published(self):
        values = self.add_verification_metadata()
        self.assertFalse(finalize.SNAPSHOT_VERIFICATION_INPUTS & finalize.FIXED_OUTPUTS)
        self.finish()
        plan = json.loads((self.output / "plan.json").read_bytes())
        proposed = {row["path"] for row in plan["changes"]}
        for name, data in values.items():
            self.assertEqual((self.output / "snapshot" / name).read_bytes(), data)
            self.assertEqual(plan["before"][name], sha(data))
            self.assertNotIn(name, proposed)
        self.finish(apply=True)
        for name, data in values.items():
            self.assertEqual((self.repo / name).read_bytes(), data)

    def test_unknown_compressed_verification_input_is_still_refused(self):
        write(self.repo / "progress/verification/unknown.json.gz", finalize._compress(b"{}\n"))
        with self.assertRaisesRegex(ValueError, "Unknown compressed input"):
            self.finish()
        self.assertFalse(self.output.exists())

    def test_changed_verification_input_cannot_be_proposed_for_publication(self):
        values = self.add_verification_metadata()
        original = self.runner
        name = next(iter(values))
        def mutate_snapshot(command, **kwargs):
            result = original(command, **kwargs)
            if Path(command[1]).name == "code_reuse_report.py" and "--check" not in command:
                write(Path(kwargs["cwd"]) / name, finalize._compress(b'{"changed":true}\n'))
            return result
        with self.assertRaisesRegex(ValueError, "non-publication file"):
            finalize.finalize(self.store, self.repo, self.action, manifest=self.manifest,
                              output=self.output, runner=mutate_snapshot, apply=True)
        self.assertEqual((self.repo / name).read_bytes(), values[name])


if __name__ == "__main__":
    unittest.main()
