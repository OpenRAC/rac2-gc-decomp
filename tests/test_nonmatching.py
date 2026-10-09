"""Offline synthetic fixtures exercise immutable evidence and public shelf rules."""
import importlib.util
import json
import os
from pathlib import Path
import struct
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
SCRIPTS = Path(os.environ.get("RAC2_MAINTAINED_SCRIPTS", ROOT / "scripts")).resolve()
sys.path.insert(0, str(SCRIPTS))
SPEC = importlib.util.spec_from_file_location("nonmatching", ROOT / "scripts/nonmatching.py")
shelf = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(shelf)
import campaign_diff as diff
from unique_code_report import expand_compact_row, validate_catalog


def elf(payload, linked=True, address=0x1000):
    """Original miniature ELF fixture, never a game artifact."""
    phoff = 52 if linked else 0
    image = bytearray(84 if linked else 52)
    code = len(image)
    image.extend(payload)
    strings = b"\0FUN_1\0"
    strings_at = len(image)
    image.extend(strings)
    symbols_at = len(image)
    image.extend(bytes(16) + struct.pack("<IIIBBH", 1, address if linked else 0, len(payload), 0x12, 0, 1))
    headers_at = len(image)
    headers = [(0,) * 10, (0, 1, 6, address if linked else 0, code, len(payload), 0, 0, 4, 0), (0, 3, 0, 0, strings_at, len(strings), 0, 0, 1, 0), (0, 2, 0, 0, symbols_at, 32, 2, 1, 4, 16)]
    for header in headers:
        image.extend(struct.pack("<10I", *header))
    struct.pack_into("<16sHHIIIIIHHHHHH", image, 0, b"\x7fELF\x01\x01\x01" + bytes(9), 2 if linked else 1, 8, 1, address if linked else 0, phoff, headers_at, 0, 52, 32 if linked else 0, 1 if linked else 0, 40, 4, 0)
    if linked:
        struct.pack_into("<8I", image, phoff, 1, code, address, address, len(payload), len(payload), 5, 1)
    return bytes(image)


class NonmatchingTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        base = Path(self.temp.name)
        self.repo, self.runtime, self.backups = base / "repo", base / "runtime", base / "backups"
        (self.repo / "nonmatching").mkdir(parents=True)
        (self.repo / "config").mkdir()
        self.register_path = self.repo / "config/campaign-register.json"
        self.catalog_path = self.repo / "config/function-catalog/catalog.json"
        self.registry = {"schema": 1, "kind": "rac2-campaign", "revision": 1, "tasks": {}, "trials": {}}
        self.store = SimpleNamespace(runtime=self.runtime, path=self.register_path, load=lambda: json.loads(self.register_path.read_bytes()))
        self.programs, self.rows, self.credit = {}, [], {}
        self.current = patch.object(shelf, "_current_context", side_effect=self.current_context)
        self.current.start()
        self.addCleanup(self.current.stop)
        instruction = SimpleNamespace(isValid=lambda: False)
        self.decoder = patch.object(diff, "_decoder", return_value=SimpleNamespace(Instruction=lambda *args: instruction, InstrCategory=SimpleNamespace(R5900=0)))
        self.decoder.start()
        self.addCleanup(self.decoder.stop)
        self.make_trial()
        self.write(self.repo / "nonmatching/README.md", shelf.render_index([]))

    def current_context(self, *args):
        # Use the real catalogue validator on a real compact-row expansion.
        # Only current repository proof freshness/credit input is substituted.
        catalog = {"schema": 1, "target": "SCUS_972.68", "normalizer": {"id": "synthetic-normalizer", "sha256": "f" * 64}, "programs": list(self.programs.values()), "functions": self.rows}
        programs, rows = validate_catalog(catalog)
        return programs, rows, self.credit, "e" * 64

    def write(self, path, value):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(shelf.encoded(value) if isinstance(value, dict) else value)

    def make_trial(self, trial="a" * 32, candidate=b"\x01" + bytes(7), *, source=b"int FUN_1(void) { return 0; }\n", program="boot", target="unit", linked=True, state="mismatch", address=0x1000):
        self.trial = trial
        self.work = self.runtime / "trials" / trial
        reference = elf(bytes(8), address=address)
        source_pin = shelf.digest(source)
        catalog = {"target": "SCUS_972.68", "program": program, "reference_sha256": shelf.digest(reference), "flags": ["-O2", "-G0"], "functions": [{"symbol": "FUN_1", "address": address, "size": 8}], "externals": {}}
        child = {"id": target, "state": "mismatch" if linked else "link_or_check_failed", "functions": []}
        if linked:
            image = elf(candidate, address=address)
            child["candidate_elf_sha256"] = shelf.digest(image)
            self.write(self.work / f"targets/{target}/candidate.elf", image)
        semantic = {"source_name": "body.c", "source_sha256": source_pin, "flags": catalog["flags"], "tools": {name: "b" * 64 for name in ("cc1", "cpp", "as", "ld.exe")}, "targets": [{"program": program, "catalog_sha256": shelf.digest(shelf.encoded(catalog)), "reference_sha256": shelf.digest(reference)}]}
        task = {"id": "research-unit", "kind": "candidate", "state": "stopped", "source": "runtime:body.c", "targets": [{"id": target, "catalog": "runtime:catalog.json", "reference": "runtime:reference.elf"}], "last_trial": trial}
        manifest = {"schema": 1, "kind": "rac2-candidate-trial", "id": trial, "task": task["id"], "created": "2026-10-08T12:00:00+00:00", "semantic_key": shelf.digest(diff._encoded(semantic)), "semantic_inputs": semantic, "task_snapshot": task, "profile_sha256": "c" * 64, "instruments": {}}
        object_data = elf(candidate, linked=False)
        outcome = {"id": trial, "state": state, "children": [child], "object_sha256": shelf.digest(object_data), "integration_credit": 0}
        self.write(self.work / "source/body.c", source)
        self.write(self.work / "candidate.o", object_data)
        self.write(self.work / f"targets/{target}/catalog.json", catalog)
        self.write(self.work / f"targets/{target}/reference.elf", reference)
        self.write(self.work / f"targets/{target}/outcome.json", child)
        self.write(self.work / "manifest.json", manifest)
        self.write(self.work / "outcome.json", outcome)
        self.registry["tasks"][task["id"]] = task
        self.registry["trials"][trial] = {"id": trial, "task": task["id"], "state": state, "directory": "runtime:trials/" + trial, "semantic_key": manifest["semantic_key"], "manifest_sha256": shelf.digest(shelf.encoded(manifest)), "outcome_sha256": shelf.digest(shelf.encoded(outcome))}
        self.write(self.register_path, self.registry)
        self.programs[program] = {"program": program, "reference_sha256": shelf.digest(reference), "ee_sections": [{"address": 0, "size": 0x10000}], "excluded_vu_bytes": 0}
        row = expand_compact_row([address, 8, shelf.digest(bytes(8)), "flow_supported_inferred", "e" * 64, [], ["FUN_1"], "Synthetic closed-flow and frame/call witness", 1], program, "f" * 64)
        self.rows = [old for old in self.rows if old["program"] != program] + [row]
        self.envelope = {"schema": 1, "kind": "rac2-nonmatching-publication", "task": task["id"], "trial": trial, "target": target, "symbol": "FUN_1", "source_sha256": source_pin, "review": {key: True for key in shelf.REVIEWS}, "reopen_codes": ["new_abi_evidence"]}

    def prepare(self):
        return shelf.prepare_entry(self.store, self.repo, self.catalog_path, self.envelope)

    def stage(self):
        return shelf.stage(self.store, self.repo, self.catalog_path, self.envelope, self.backups)

    def test_empty_index_is_derived_zero_credit_and_no_imports(self):
        self.assertEqual(shelf.check(self.store, self.repo, self.catalog_path), {"ok": True, "entries": 0, "retained_evidence_verified": False, "integration_credit": 0})
        self.assertEqual(list((self.repo / "nonmatching").rglob("*.c")), [])
        self.assertIn(b"zero C credit", (self.repo / "nonmatching/README.md").read_bytes())

    def test_stage_publishes_exact_authored_source_and_structural_metadata(self):
        register_before = self.register_path.read_bytes()
        private_before = {str(path): path.read_bytes() for path in self.runtime.rglob("*") if path.is_file()}
        result = self.stage()
        self.assertEqual(result["state"], "research-only")
        self.assertEqual(result["integration_credit"], 0)
        metadata = json.loads((self.repo / "nonmatching/boot/00001000-8.json").read_bytes())
        self.assertEqual(metadata["different_bytes"], 1)
        self.assertEqual(metadata["candidate_size"], 8)
        self.assertEqual((self.repo / "nonmatching/boot/00001000-8.c").read_bytes(), (self.work / "source/body.c").read_bytes())
        self.assertNotIn(str(self.runtime), json.dumps(metadata))
        self.assertEqual(self.register_path.read_bytes(), register_before)
        self.assertEqual(private_before, {str(path): path.read_bytes() for path in self.runtime.rglob("*") if path.is_file()})
        self.assertTrue(shelf.check(self.store, self.repo, self.catalog_path, runtime=True)["ok"])
        self.assertTrue((self.backups / "publication-receipts" / (metadata["publication_receipt_sha256"] + ".json")).is_file())

    def test_full_size_mismatch_keeps_extra_and_missing_bytes(self):
        for candidate, expected in ((bytes(4), 4), (bytes(12), 4)):
            self.make_trial(candidate=candidate)
            metadata, _ = self.prepare()
            self.assertEqual(metadata["size"], 8)
            self.assertEqual(metadata["candidate_size"], len(candidate))
            self.assertEqual(metadata["different_bytes"], expected)

    def test_object_only_and_raw_exact_never_stage(self):
        for kwargs in ({"linked": False}, {"candidate": bytes(8)}, {"state": "exact_private"}):
            self.make_trial(**kwargs)
            with self.subTest(kwargs=kwargs), self.assertRaises(ValueError):
                self.prepare()
        self.assertEqual(list((self.repo / "nonmatching").rglob("*.c")), [])

    def test_current_integrated_overlap_and_exact_task_excluded(self):
        self.credit[("boot", 0x0FF0, 32)] = "d" * 64
        with self.assertRaisesRegex(ValueError, "already_current_exact"):
            self.prepare()
        self.credit.clear()
        self.registry["tasks"]["research-unit"]["state"] = "exact_private"
        self.write(self.register_path, self.registry)
        with self.assertRaisesRegex(ValueError, "task_not_research_open"):
            self.prepare()

    def test_real_flow_supported_catalogue_row_allows_complete_mismatch(self):
        self.assertEqual(self.rows[0]["boundary"]["status"], "flow_supported_inferred")
        self.assertTrue(self.rows[0]["normalization"]["certificate"]["exact"])
        metadata, source = self.prepare()
        self.assertEqual(metadata["boundary_status"], "flow_supported_inferred")
        self.assertEqual(metadata["different_bytes"], 1)
        self.assertEqual(metadata["status"], "research-only")
        self.assertEqual(metadata["integration_credit"], 0)
        self.assertEqual(shelf.digest(source), metadata["source_sha256"])

    def test_qualified_inferred_and_fragment_catalogue_tiers_excluded(self):
        for status in ("qualified_complete", "inferred", "ambiguous_fragment"):
            self.rows[0]["boundary"]["status"] = status
            with self.subTest(status=status), self.assertRaisesRegex(ValueError, "unsupported_research_extent"):
                self.prepare()

    def test_flow_supported_missing_or_false_certificate_fails_closed(self):
        certificate = self.rows[0]["normalization"]["certificate"]
        self.rows[0]["normalization"]["certificate"] = None
        with self.assertRaises(ValueError):
            self.prepare()
        self.rows[0]["normalization"]["certificate"] = certificate
        certificate["exact"] = False
        with self.assertRaises(ValueError):
            self.prepare()

    def test_same_address_different_program_has_separate_paths(self):
        first, _ = self.prepare()
        self.make_trial(trial="b" * 32, program="levels/test_overlay")
        second, _ = self.prepare()
        self.assertNotEqual(shelf.entry_relative(first), shelf.entry_relative(second))
        self.credit[("boot", 0x1000, 8)] = "d" * 64
        self.assertEqual(self.prepare()[0]["program"], "levels/test_overlay")

    def test_unknown_extent_bad_body_and_changed_reference_fail_closed(self):
        for mutation in (lambda: self.rows.clear(), lambda: self.rows[0].update(size=12), lambda: self.rows[0].update(raw_sha256="d" * 64), lambda: self.programs["boot"].update(reference_sha256="d" * 64), lambda: self.rows[0]["boundary"].update(status="inferred")):
            self.make_trial()
            mutation()
            with self.assertRaises(ValueError):
                self.prepare()

    def test_all_immutable_trial_drifts_refused(self):
        for relative in ("manifest.json", "outcome.json", "source/body.c", "candidate.o", "targets/unit/catalog.json", "targets/unit/reference.elf", "targets/unit/candidate.elf"):
            with self.subTest(relative=relative):
                path = self.work / relative
                before = path.read_bytes()
                path.write_bytes(before + b" ")
                with self.assertRaises(ValueError):
                    self.prepare()
                path.write_bytes(before)

    def test_owner_review_and_no_arbitrary_envelope_fields(self):
        for flag in shelf.REVIEWS:
            self.envelope["review"][flag] = False
            with self.subTest(flag=flag), self.assertRaisesRegex(ValueError, "owner_review_required"):
                self.prepare()
            self.envelope["review"][flag] = True
        self.envelope["review"]["defined_behavior"] = 1
        with self.assertRaises(ValueError):
            self.prepare()
        self.envelope["review"]["defined_behavior"] = True
        self.envelope["private_note"] = "owner private path"
        with self.assertRaisesRegex(ValueError, "invalid_publication_envelope"):
            self.prepare()

    def test_owner_source_hash_binding(self):
        self.envelope["source_sha256"] = "d" * 64
        with self.assertRaisesRegex(ValueError, "unreviewed_source_hash"):
            self.prepare()

    def test_guard_rejects_asm_raw_data_private_paths_and_obvious_uninitialized(self):
        for source in (b'int FUN_1(void) { asm("nop"); return 0; }', b"int FUN_1(void) { return 0; }\n.word 0x12345678\n", b"/* C:/Users/private/evidence */\nint FUN_1(void) { return 0; }", b"/* ghp_123456789secret */\nint FUN_1(void) { return 0; }", b"#include <private.h>\nint FUN_1(void) { return 0; }", b"int FUN_1(void) { int value; return value; }", b"int FUN_1(void) { __builtin_unreachable(); }", b"int payload[4] = {1, 2, 3, 4}; int FUN_1(void) { return 0; }"):
            with self.subTest(source=source), self.assertRaises(ValueError):
                shelf.source_guard(source, "FUN_1", {})

    def test_legitimate_comments_and_later_assignment_preserved(self):
        source = b"/* Research hypothesis, reviewed by the owner. */\nint FUN_1(void) { int value; value = 3; return value; }\n"
        self.make_trial(source=source)
        metadata, published = self.prepare()
        self.assertEqual(published, source)
        self.assertEqual(metadata["source_sha256"], shelf.digest(source))

    def test_https_provenance_comment_accepted_verbatim(self):
        source = b"/* https://github.com/OpenRAC/rac1-decomp */\nint FUN_1(void) { return 0; }\n"
        self.make_trial(source=source)
        metadata, published = self.prepare()
        self.assertEqual(published, source)
        self.assertEqual(metadata["source_sha256"], shelf.digest(source))
        self.stage()
        self.assertEqual((self.repo / "nonmatching/boot/00001000-8.c").read_bytes(), source)
        self.assertTrue(shelf.check(self.store, self.repo, self.catalog_path)["ok"])

    def test_known_absolute_private_namespaces_denied_before_stage(self):
        for namespace in ("root", "mnt", "private", "srv", "opt", "etc", "run", "var"):
            source = f"/* /{namespace}/essai-rac2/private-note */\nint FUN_1(void) {{ return 0; }}\n".encode()
            self.make_trial(source=source)
            with self.subTest(namespace=namespace), self.assertRaisesRegex(ValueError, "private_source_metadata"):
                self.stage()
            self.assertEqual(list((self.repo / "nonmatching").rglob("*.c")), [])

    def test_known_absolute_private_namespaces_denied_by_public_check(self):
        self.stage()
        source_path = self.repo / "nonmatching/boot/00001000-8.c"
        metadata_path = self.repo / "nonmatching/boot/00001000-8.json"
        original_source = source_path.read_bytes()
        original_metadata = json.loads(metadata_path.read_bytes())
        for namespace in ("root", "mnt", "private", "srv", "opt", "etc", "run", "var"):
            source = f"/* /{namespace}/essai-rac2/private-note */\n".encode() + original_source
            source_path.write_bytes(source)
            # Even a matching public source hash cannot bypass the privacy guard.
            metadata = dict(original_metadata, source_sha256=shelf.digest(source))
            self.write(metadata_path, metadata)
            with self.subTest(namespace=namespace), self.assertRaisesRegex(ValueError, "shelf_source_drift"):
                shelf.check(self.store, self.repo, self.catalog_path)

    def test_real_windows_drive_paths_remain_private(self):
        for path in (b"C:/Users/owner/private-note", b"D:\\private-note", b"C:/unlisted-folder/evidence"):
            with self.subTest(path=path), self.assertRaisesRegex(ValueError, "private_source_metadata"):
                shelf.source_guard(b"/* " + path + b" */\nint FUN_1(void) { return 0; }\n", "FUN_1", {})

    def test_duplicate_or_worse_attempt_never_overwrites_best(self):
        self.stage()
        source_path = self.repo / "nonmatching/boot/00001000-8.c"
        before = source_path.read_bytes()
        with self.assertRaisesRegex(ValueError, "not_strictly_better"):
            self.stage()
        self.make_trial(trial="b" * 32, candidate=b"\x01\x02" + bytes(6))
        with self.assertRaisesRegex(ValueError, "not_strictly_better"):
            self.stage()
        self.assertEqual(source_path.read_bytes(), before)
        self.assertTrue(shelf.check(self.store, self.repo, self.catalog_path)["ok"])

    def test_better_attempt_retains_previous_public_pair_privately(self):
        self.make_trial(candidate=b"\x01\x02" + bytes(6))
        self.stage()
        old_source = (self.repo / "nonmatching/boot/00001000-8.c").read_bytes()
        old_metadata = (self.repo / "nonmatching/boot/00001000-8.json").read_bytes()
        self.make_trial(trial="b" * 32, candidate=b"\x01" + bytes(7), source=b"int FUN_1(void) { return 1; }\n")
        self.stage()
        retained = [path for path in self.backups.iterdir() if path.is_dir() and path.name != "publication-receipts"]
        self.assertEqual(len(retained), 1)
        self.assertEqual((retained[0] / "source.c").read_bytes(), old_source)
        self.assertEqual((retained[0] / "metadata.json").read_bytes(), old_metadata)
        self.assertEqual(len(self.store.load()["trials"]), 2)
        self.assertTrue(shelf.check(self.store, self.repo, self.catalog_path, runtime=True)["ok"])

    def test_private_backups_cannot_be_inside_repository(self):
        with self.assertRaisesRegex(ValueError, "private_backups_required"):
            shelf.stage(self.store, self.repo, self.catalog_path, self.envelope, self.repo / "private-backups")

    def test_stale_index_and_source_hash_fail_closed(self):
        self.stage()
        path = self.repo / "nonmatching/README.md"
        original = path.read_bytes()
        path.write_bytes(original + b"stale")
        with self.assertRaisesRegex(ValueError, "stale_shelf_index"):
            shelf.check(self.store, self.repo, self.catalog_path)
        path.write_bytes(original)
        with (self.repo / "nonmatching/boot/00001000-8.c").open("ab") as stream:
            stream.write(b"\n")
        with self.assertRaisesRegex(ValueError, "shelf_source_drift"):
            shelf.check(self.store, self.repo, self.catalog_path)

    def test_stale_register_receipt_and_closed_task_fail_closed(self):
        self.stage()
        self.registry["trials"][self.trial]["manifest_sha256"] = "e" * 64
        self.write(self.register_path, self.registry)
        with self.assertRaisesRegex(ValueError, "stale_shelf_pins"):
            shelf.check(self.store, self.repo, self.catalog_path)
        self.make_trial()
        self.registry["tasks"]["research-unit"]["state"] = "integrated"
        self.write(self.register_path, self.registry)
        with self.assertRaisesRegex(ValueError, "task_not_research_open"):
            shelf.check(self.store, self.repo, self.catalog_path)

    def test_orphan_source_and_unexpected_binary_artifacts_refused(self):
        self.write(self.repo / "nonmatching/orphan.c", b"int orphan(void){return 0;}")
        with self.assertRaisesRegex(ValueError, "orphan_shelf_source"):
            shelf.check(self.store, self.repo, self.catalog_path)
        # The fixture intentionally leaves the orphan intact; no purge occurs.

    def test_unexpected_binary_artifact_refused(self):
        self.write(self.repo / "nonmatching/candidate.o", b"synthetic-unpublished-artifact")
        with self.assertRaisesRegex(ValueError, "unexpected_shelf_artifact"):
            shelf.check(self.store, self.repo, self.catalog_path)

    def test_runtime_check_detects_measurement_metadata_forgery(self):
        self.stage()
        metadata_path = self.repo / "nonmatching/boot/00001000-8.json"
        metadata = json.loads(metadata_path.read_bytes())
        metadata["different_bytes"] = 2
        self.write(metadata_path, metadata)
        entries = shelf.shelf_entries(self.repo, self.store.load(), self.catalog_path)
        self.write(self.repo / "nonmatching/README.md", shelf.render_index(entries))
        public = shelf.check(self.store, self.repo, self.catalog_path)
        self.assertFalse(public["retained_evidence_verified"])
        with self.assertRaisesRegex(ValueError, "shelf_evidence_drift"):
            shelf.check(self.store, self.repo, self.catalog_path, runtime=True)
        with self.assertRaisesRegex(ValueError, "shelf_evidence_drift"):
            self.stage()

    def test_unsorted_owner_reopen_codes_normalize_receipt_stably(self):
        self.envelope["reopen_codes"] = ["new_structural_evidence", "new_abi_evidence"]
        self.stage()
        self.assertTrue(shelf.check(self.store, self.repo, self.catalog_path, runtime=True)["ok"])

    def test_unknown_metadata_and_boolean_measurements_refused(self):
        metadata, _ = self.prepare()
        for key, value in (("size", True), ("different_bytes", False), ("integration_credit", True), ("checked_date", "2026-99-99"), ("reopen_codes", ["C:/private"])):
            candidate = dict(metadata, **{key: value})
            with self.subTest(key=key), self.assertRaises(ValueError):
                shelf.validate_metadata(candidate)
        with self.assertRaisesRegex(ValueError, "invalid_shelf_metadata"):
            shelf.validate_metadata(dict(metadata, queue_state="work next"))


if __name__ == "__main__":
    unittest.main()
