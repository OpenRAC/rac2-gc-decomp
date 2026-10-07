"""Organization invariants, using synthetic references and a fake compiler."""
import contextlib
import importlib.util
import io
import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/campaign.py"
SPEC = importlib.util.spec_from_file_location("campaign", SCRIPT)
campaign = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(campaign)


class Backend:
    def __init__(self, tools, compile_failure=False, mismatch=False, link_failure=False):
        self.hashes = tools
        self.compile_failure, self.mismatch, self.link_failure = compile_failure, mismatch, link_failure
        self.compilations = 0

    def tools(self, path):
        return self.hashes

    def compile(self, source, flags, obj, assembly, log):
        self.compilations += 1
        if self.compile_failure:
            raise ValueError("synthetic compiler failure")
        obj.write_bytes(b"fake-object")
        assembly.write_bytes(b"fake-assembly")
        log.write_text("fake compiler\n")

    def measure(self, obj, catalog, reference, toolchain, work):
        if self.link_failure:
            raise ValueError("synthetic link failure")
        return {"functions": [{"symbol": f["symbol"], "matched": not self.mismatch,
                               "reference_sha256": "a" * 64,
                               "candidate_sha256": ("b" if self.mismatch else "a") * 64}
                              for f in catalog["functions"]], "candidate_elf_sha256": "c" * 64}


class CampaignTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.base = Path(self.temp.name)
        self.repo, self.runtime = self.base / "repo", self.base / "private"
        for directory in ("config", "scripts", "candidates", "progress"):
            (self.repo / directory).mkdir(parents=True)
        self.store = campaign.Store(self.repo / "config/campaign-register.json", self.runtime)
        self.reference = self.base / "reference.elf"
        self.reference.write_bytes(b"synthetic-reference")
        reference_hash = campaign.digest(self.reference.read_bytes())
        self.dump(self.repo / "config/target.json", {"serial": "SCUS_972.68", "boot": {"sha256": reference_hash}})
        self.dump(self.repo / "config/overlays.json", {"levels": []})
        self.source = self.repo / "candidates/body.c"
        self.source.write_bytes(b"int FUN_1(void) { return 1; }\n")
        self.catalog = self.repo / "config/candidate-catalog.json"
        self.dump(self.catalog, {"target": "SCUS_972.68", "reference_sha256": reference_hash,
                               "flags": ["-O2", "-G0", "-ffunction-sections"], "gp": 0,
                               "externals": {}, "functions": [{"symbol": "FUN_1", "address": 4096, "size": 8}]})
        self.tools = {"cc1": campaign.CC1, "cpp": "d" * 64, "as": "e" * 64, "ld.exe": "f" * 64}
        self.profile = self.repo / "progress/candidates.json"
        self.dump(self.profile, {"tools": self.tools})
        self.task = {"id": "unit-one", "source": "candidates/body.c", "next_action": "Compare one changed return",
                     "targets": [{"id": "boot", "reference": str(self.reference), "catalog": str(self.catalog)}]}

    def tearDown(self):
        self.temp.cleanup()

    @staticmethod
    def dump(path, value):
        path.write_text(json.dumps(value), encoding="utf-8")

    def attempt(self, backend, reason=""):
        return campaign.trial(self.store, self.repo, "unit-one", self.base, self.profile, reason, backend)

    def test_one_compile_many_children_preserves_inputs_and_no_credit(self):
        self.task["targets"].append({**self.task["targets"][0], "id": "second-placement"})
        campaign.plan(self.store, self.task)
        backend = Backend(self.tools)
        result = self.attempt(backend)
        self.assertEqual(backend.compilations, 1)
        self.assertEqual(result["state"], "exact_private")
        self.assertEqual(len(result["children"]), 2)
        self.assertEqual(result["integration_credit"], 0)
        work = campaign.absolute(self.store.load()["trials"][result["id"]]["directory"], self.repo, self.runtime)
        self.assertEqual((work / "source/body.c").read_bytes(), self.source.read_bytes())
        manifest_bytes = (work / "manifest.json").read_bytes()
        with self.assertRaises(FileExistsError):
            campaign.write_new(work / "manifest.json", b"overwrite")
        self.assertEqual((work / "manifest.json").read_bytes(), manifest_bytes)
        self.assertEqual(campaign.summary(self.store.load())["compilation_attempts"], 1)

    def test_nonmatching_region_is_rejected_before_compilation(self):
        self.dump(self.repo / "config/regions.json", {
            "schema": 1, "default": "ntsc-u", "regions": {"ntsc-u": {
                "serial": "SCUS_972.68", "label": "Unqualified region",
                "target": "config/target.json", "overlays": "config/overlays.json",
                "matching": False}}})
        campaign.plan(self.store, self.task)
        backend = Backend(self.tools)
        result = self.attempt(backend)
        self.assertEqual(result["state"], "preparation_rejected")
        self.assertIn("C trials exists only for a matching region", result["error"])
        self.assertEqual(backend.compilations, 0)

    def test_compile_failure_is_one_failure_not_two_body_measurements(self):
        self.task["targets"].append({**self.task["targets"][0], "id": "second-placement"})
        campaign.plan(self.store, self.task)
        result = self.attempt(Backend(self.tools, compile_failure=True))
        self.assertEqual(result["state"], "compile_failed")
        self.assertTrue(all(child["state"] == "unmeasured" for child in result["children"]))
        status = campaign.summary(self.store.load())
        self.assertEqual(status["failed_compilation_trials"], 1)
        self.assertEqual(status["target_measurements"], 0)

    def test_duplicate_requires_reason_and_renaming_task_target_cannot_bypass(self):
        campaign.plan(self.store, self.task)
        self.attempt(Backend(self.tools))
        backend = Backend(self.tools)
        rejected = self.attempt(backend)
        self.assertEqual(rejected["state"], "preparation_rejected")
        self.assertIn("Semantic duplicate", rejected["error"])
        self.assertEqual(backend.compilations, 0)
        self.task["id"] = "unit-two"
        self.task["targets"][0]["id"] = "different-label"
        campaign.plan(self.store, self.task)
        rejected = campaign.trial(self.store, self.repo, "unit-two", self.base, self.profile, backend=backend)
        self.assertIn("Semantic duplicate", rejected["error"])
        repeated = self.attempt(backend, "Verify deterministic output after reviewed change")
        self.assertEqual(repeated["state"], "exact_private")
        self.assertEqual(campaign.summary(self.store.load())["compilation_attempts"], 2)

    def test_mismatch_and_link_failure_are_retained(self):
        campaign.plan(self.store, self.task)
        result = self.attempt(Backend(self.tools, mismatch=True))
        self.assertEqual(result["state"], "mismatch")
        self.assertEqual(result["children"][0]["functions"][0]["candidate_sha256"], "b" * 64)
        linked = self.attempt(Backend(self.tools, link_failure=True), "Test linker failure recording")
        self.assertEqual(linked["children"][0]["state"], "link_or_check_failed")
        self.assertEqual(linked["measured_functions"], 0)

    def test_profile_pin_and_input_assembly_refused_before_compiler(self):
        campaign.plan(self.store, self.task)
        self.dump(self.profile, {"tools": {**self.tools, "cc1": "0" * 64}})
        backend = Backend(self.tools)
        self.assertIn("GNU8bed", self.attempt(backend)["error"])
        self.dump(self.profile, {"tools": self.tools})
        self.source.write_bytes(b"__asm__(\"nop\");")
        self.assertIn("assembly", self.attempt(backend)["error"])
        self.assertEqual(backend.compilations, 0)

    def test_c_member_names_reach_the_compiler_without_becoming_directives(self):
        self.source.write_bytes(b'''struct Snapshot { unsigned char bytes[24]; int word; int words; };
struct Snapshot snapshot = {
    .word = 2,
    .words = 3
};
int FUN_1(void) { const struct Snapshot *object = &snapshot;
    return snapshot.bytes[0] + object->word + object->words + snapshot.word;
}
''')
        campaign.plan(self.store, self.task)
        backend = Backend(self.tools)
        result = self.attempt(backend)
        self.assertEqual(result["state"], "exact_private")
        self.assertEqual(backend.compilations, 1)

    def test_real_assembly_tokens_and_directives_are_rejected_before_compiler(self):
        campaign.plan(self.store, self.task)
        for content in (b'.byte 1, 2\n', b'.word 0x1234\n', b'label: .word 1\n',
                        b'1: .byte 2\n', b'asm("nop");', b'__asm__("nop");',
                        b'__asm("nop");', b'INCLUDE_ASM("unit.s", FUN_1);'):
            with self.subTest(content=content):
                self.source.write_bytes(content)
                backend = Backend(self.tools)
                result = self.attempt(backend, reason="source admission regression fixture")
                self.assertEqual(result["state"], "preparation_rejected")
                self.assertIn("assembly", result["error"])
                self.assertFalse(result["compile_attempted"])
                self.assertEqual(backend.compilations, 0)

    def test_member_admission_does_not_bypass_standalone_reproducibility(self):
        campaign.plan(self.store, self.task)
        for content in (b'#include "header.h"\nint FUN_1(void) { return object.word; }',
                        b'const char *stamp = __DATE__;\nint FUN_1(void) { return object.words; }'):
            with self.subTest(content=content):
                self.source.write_bytes(content)
                backend = Backend(self.tools)
                result = self.attempt(backend, reason="standalone admission regression fixture")
                self.assertEqual(result["state"], "preparation_rejected")
                self.assertIn("standalone", result["error"])
                self.assertEqual(backend.compilations, 0)

    def test_changed_instrument_invalidates_exact(self):
        campaign.plan(self.store, self.task)
        backend = Backend(self.tools)
        original = backend.measure
        def changed(*args):
            result = original(*args)
            (self.repo / "scripts/checker.py").write_text("changed")
            return result
        backend.measure = changed
        result = self.attempt(backend)
        self.assertEqual(result["state"], "provenance_changed")
        self.assertEqual(self.store.load()["tasks"]["unit-one"]["state"], "stopped")

    def test_legacy_lossless_grouping_and_idempotence(self):
        legacy = self.base / "legacy.md"
        original = ("# Register\r\nNote preserved.\r\n| Trial | Outcome | Evidence directory |\r\n"
                    "|---|---|---|\r\n| one:A | COMPILE failure | `shared/run` |\r\n"
                    "| one:B | COMPILE failure | `shared/run` |\r\n## PCSX2\r\nOpaque historical observation.\r\n")
        legacy.write_bytes(original.encode("utf-8"))
        imported = campaign.legacy_import(self.store, legacy)
        self.assertEqual(imported["rows"], 2)
        status = campaign.summary(self.store.load())
        self.assertEqual(status["legacy_target_rows"], 2)
        self.assertEqual(status["legacy_unique_evidence_directories"], 1)
        self.assertEqual(status["trial_count"], 0)
        registry = self.store.load()
        document = registry["legacy_documents"][imported["document"]]
        self.assertEqual(document["original_utf8"].encode("utf-8"), legacy.read_bytes())
        self.assertEqual(campaign.legacy_import(self.store, legacy)["rows"], 0)
        self.assertTrue(all(row["validity"] == "historical_unverified" for row in registry["legacy_rows"].values()))

    def test_queue_research_and_explicit_transition_no_integration_credit(self):
        campaign.plan(self.store, {"id": "anchors", "kind": "research", "next_action": "Check fresh mapping",
                                  "pointers": ["docs/MAP.md"]})
        campaign.transition(self.store, "anchors", "blocked", "Needs dated evidence", "Reference is available")
        self.assertEqual(self.store.load()["tasks"]["anchors"]["reopen_condition"], "Reference is available")
        campaign.transition(self.store, "anchors", "done", "Evidence reviewed")
        campaign.plan(self.store, self.task)
        with self.assertRaises(ValueError):
            campaign.transition(self.store, "unit-one", "exact_private", "Claim")
        with self.assertRaises(ValueError):
            campaign.transition(self.store, "unit-one", "done", "Claim")

    def test_store_lock_and_failed_edit_keep_registry(self):
        campaign.plan(self.store, self.task)
        original = self.store.path.read_bytes()
        with self.assertRaises(RuntimeError):
            with self.store.edit() as value:
                value["tasks"].clear()
                raise RuntimeError("abort")
        self.assertEqual(original, self.store.path.read_bytes())
        lock = self.store.path.with_name(self.store.path.name + ".lock")
        lock.write_text("existing owner")
        with self.assertRaisesRegex(ValueError, "locked"):
            campaign.plan(self.store, {"id": "research", "kind": "research", "next_action": "Inspect"})
        self.assertEqual(lock.read_text(), "existing owner")

    def test_facade_forwards_all_levels_and_failure_no_publication(self):
        calls = []
        manifest = self.runtime / "baseline.json"
        manifest.parent.mkdir(parents=True, exist_ok=True)
        self.dump(manifest, {"synthetic": True})
        def fake(args, **kwargs):
            calls.append(args)
            return SimpleNamespace(returncode=7)
        result = campaign.facade(self.store, self.repo, "integrate", ["--manifest", str(manifest),
                                   "--c-toolchain", "toolchain"], runner=fake)
        self.assertEqual(Path(calls[0][1]).name, "campaign_build.py")
        self.assertEqual(calls[0][calls[0].index("--program-jobs") + 1], "4")
        self.assertEqual(calls[0][calls[0].index("--jobs") + 1], "2")
        self.assertEqual(result["state"], "failed")
        self.assertEqual(result["integration_credit"], 0)
        with self.assertRaisesRegex(ValueError, "outside"):
            campaign.facade(self.store, self.repo, "report", ["--output", str(self.repo / "report.json")], runner=fake)

    def test_cli_pending_queue_and_packet(self):
        campaign.plan(self.store, self.task)
        options = ["--repo", str(self.repo), "--runtime", str(self.runtime)]
        with contextlib.redirect_stdout(io.StringIO()) as output:
            self.assertEqual(campaign.main([*options, "queue"]), 0)
        self.assertEqual(json.loads(output.getvalue())["queued"][0]["id"], "unit-one")
        with contextlib.redirect_stdout(io.StringIO()) as output:
            self.assertEqual(campaign.main([*options, "packet", "unit-one"]), 0)
        self.assertEqual(json.loads(output.getvalue())["task"]["source"], "candidates/body.c")

    def test_private_runtime_guard(self):
        for path in (self.repo, self.repo / "work", self.base):
            with self.assertRaises(ValueError):
                campaign.private(path, self.repo)
        self.assertEqual(campaign.private(self.runtime, self.repo), self.runtime)

    def test_boot_and_runtime_legacy_tables_are_preserved_separately(self):
        legacy = self.base / "mixed.md"
        legacy.write_text("| Target | Trial record | Result | Evidence directory |\n|---|---|---|---|\n"
                          "| FUN_A | run:A | EXACT | `boot/run` |\n"
                          "## PCSX2\n| Probe | Instrument | Outcome | Private evidence |\n|---|---|---|---|\n"
                          "| Retail | PCSX2 | No breakpoint | `probe/result.json` |\n", encoding="utf-8")
        campaign.legacy_import(self.store, legacy)
        status = campaign.summary(self.store.load())
        self.assertEqual(status["legacy_target_rows"], 1)
        self.assertEqual(status["legacy_runtime_observations"], 1)

    def test_zero_exit_without_fresh_artifact_is_failure(self):
        result = campaign.facade(self.store, self.repo, "report", [],
                                 runner=lambda *args, **kwargs: SimpleNamespace(returncode=0))
        self.assertEqual(result["state"], "failed")
        self.assertIn("fresh proof artifact", result["error"])

    def test_final_tool_read_failure_cannot_report_exact(self):
        campaign.plan(self.store, self.task)
        backend = Backend(self.tools)
        calls = []

        def tools(_):
            calls.append(1)
            if len(calls) > 1:
                raise OSError("Final tool recheck unavailable")
            return self.tools

        backend.tools = tools
        result = self.attempt(backend)
        self.assertEqual(result["state"], "provenance_check_failed")
        self.assertEqual(self.store.load()["tasks"]["unit-one"]["state"], "stopped")

    def test_plan_cannot_assert_a_matching_state(self):
        for state in ("exact_private", "integrated"):
            with self.assertRaisesRegex(ValueError, "without proof"):
                campaign.plan(self.store, {**self.task, "state": state})

    def test_views_reject_unregistered_concurrent_edit(self):
        campaign.plan(self.store, self.task)
        campaign.views(self.store, self.repo)
        history = self.repo / "docs/C-NATIVE-EXPERIMENT-REGISTER.md"
        history.write_bytes(history.read_bytes() + b"\nNew unimported negative result\n")
        with self.assertRaisesRegex(ValueError, "Unregistered view edit"):
            campaign.views(self.store, self.repo)
        self.assertIn(b"New unimported negative result", history.read_bytes())

    def test_closure_requires_complete_proof_and_stable_inputs(self):
        campaign.plan(self.store, self.task)
        for name in ("integration.json", "report.json"):
            self.dump(self.repo / "progress" / name, {})
        with self.assertRaisesRegex(ValueError, "without a current"):
            campaign.close(self.store, self.repo, "unit-one", validator=lambda _: set())

        def changed(_):
            self.source.write_bytes(b"changed during validation")
            return {("boot", "FUN_1", 4096, 8)}

        with self.assertRaisesRegex(ValueError, "changed during closure"):
            campaign.close(self.store, self.repo, "unit-one", validator=changed)
        self.assertEqual(self.store.load()["tasks"]["unit-one"]["state"], "queued")

    def test_successful_closure_adds_no_matching_credit(self):
        campaign.plan(self.store, self.task)
        for name in ("integration.json", "report.json"):
            self.dump(self.repo / "progress" / name, {})
        result = campaign.close(self.store, self.repo, "unit-one", validator=lambda _: {("boot", "FUN_1", 4096, 8)})
        self.assertEqual(result["state"], "integrated")
        self.assertEqual(result["integration_credit_added"], 0)

    def test_amendment_retains_history_and_cannot_set_state(self):
        campaign.plan(self.store, self.task)
        campaign.amend(self.store, "unit-one", {"pointers": ["verified.json"]}, "Verified replacement pointer")
        task = self.store.load()["tasks"]["unit-one"]
        self.assertEqual(task["pointers"], ["verified.json"])
        self.assertEqual(len(task["amendments"]), 1)
        with self.assertRaises(ValueError):
            campaign.amend(self.store, "unit-one", {"state": "integrated"}, "Manual assertion")

    def test_overlapping_legacy_documents_keep_text_without_double_counting_rows(self):
        path = self.base / "legacy.md"
        header = "| Target | Function | Trial | Outcome | Evidence directory |\n|---|---|---|---|---|\n"
        row = "| 0x1 | FUN_1 | first | MISMATCH | bank/first |\n"
        path.write_text(header + row)
        campaign.legacy_import(self.store, path)
        path.write_text(header + row + "| 0x1 | FUN_1 | second | EXACT | bank/second |\n")
        result = campaign.legacy_import(self.store, path)
        self.assertEqual(result["rows"], 1)
        self.assertEqual(len(self.store.load()["legacy_documents"]), 2)
        self.assertEqual(len(self.store.load()["legacy_rows"]), 2)

    def test_build_rejects_old_receipt_even_with_zero_exit(self):
        manifest = self.base / "manifest.json"
        self.dump(manifest, {})
        old = self.runtime / ("campaign-" + "0" * 32) / "report.json"
        old.parent.mkdir(parents=True)
        self.dump(old, {"batch_id": "0" * 32})

        def runner(*args, **kwargs):
            kwargs["stdout"].write((json.dumps({"report": str(old)}) + "\n").encode())
            return SimpleNamespace(returncode=0)

        result = campaign.facade(self.store, self.repo, "build", ["--manifest", str(manifest)], runner=runner)
        self.assertEqual(result["state"], "failed")
        self.assertIn("fresh campaign action", result["error"])


if __name__ == "__main__":
    unittest.main()
