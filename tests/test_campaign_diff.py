"""Synthetic MIPS ELF trials test historical review and its trust boundary."""
import importlib.util
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
from types import SimpleNamespace
import unittest

SCRIPTS = Path(__file__).resolve().parents[1] / "scripts"
sys.path.insert(0, str(SCRIPTS))
SPEC = importlib.util.spec_from_file_location("campaign_diff", SCRIPTS / "campaign_diff.py")
diff = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(diff)


def elf(payload=b"\0" * 8, *, elf_type=2, address=0x1000, functions=None, relocation=False):
    """Small original fixture: one file-backed code section and global symbols."""
    if functions is None:
        functions = [("FUN_1", address if elf_type == 2 else 0, len(payload))]
    phoff = 52 if elf_type == 2 else 0
    image = bytearray(84 if phoff else 52)
    code_offset = len(image)
    image.extend(payload)
    strings = bytearray(b"\0")
    symbols = bytearray(16)
    for name, value, size in functions:
        name_offset = len(strings)
        strings.extend(name.encode("ascii") + b"\0")
        symbols.extend(struct.pack("<IIIBBH", name_offset, value, size, 0x12, 0, 1))
    strings_offset = len(image)
    image.extend(strings)
    symbols_offset = len(image)
    image.extend(symbols)
    headers = [(0,) * 10,
               (0, 1, 6, address if elf_type == 2 else 0, code_offset, len(payload), 0, 0, 4, 0),
               (0, 3, 0, 0, strings_offset, len(strings), 0, 0, 1, 0),
               (0, 2, 0, 0, symbols_offset, len(symbols), 2, 1, 4, 16)]
    if relocation:
        offset = len(image)
        image.extend(bytes(8))
        headers.append((0, 9, 0, 0, offset, 8, 3, 1, 4, 8))
    shoff = len(image)
    for header in headers:
        image.extend(struct.pack("<10I", *header))
    struct.pack_into("<16sHHIIIIIHHHHHH", image, 0,
                     b"\x7fELF\x01\x01\x01" + bytes(9), elf_type, 8, 1,
                     address if elf_type == 2 else 0, phoff, shoff, 0,
                     52, 32 if phoff else 0, 1 if phoff else 0, 40, len(headers), 0)
    if phoff:
        struct.pack_into("<8I", image, phoff, 1, code_offset, address, address,
                         len(payload), len(payload), 5, 1)
    return bytes(image)


class DiffTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name)
        self.repo, self.runtime = self.base / "repo", self.base / "private"
        self.repo.mkdir()
        self.trial_id = "a" * 32
        self.work = self.runtime / "trials" / self.trial_id
        self.registry_path = self.repo / "register.json"
        self.store = SimpleNamespace(runtime=self.runtime, path=self.registry_path,
                                     load=lambda: json.loads(self.registry_path.read_bytes()))
        self.fixture()

    def put(self, path, value):
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(diff._encoded(value) if isinstance(value, dict) else value)

    def fixture(self, candidate=None, reference=None, *, targets=("boot",), functions=None,
                linked=True, compiled=True, readonly=False, state="mismatch", object_image=None):
        reference = reference or elf()
        candidate = candidate if candidate is not None else elf()
        functions = functions or [{"symbol": "FUN_1", "address": 0x1000, "size": 8}]
        source = b"/* </script><script>alert('source')</script> & metadata */\nint FUN_1(void) {return 0;}\n"
        self.put(self.work / "source/body.c", source)
        tools = {name: "b" * 64 for name in ("cc1", "cpp", "as", "ld.exe")}
        source_hash = diff._hash(source)
        semantic = {"source_name": "body.c", "source_sha256": source_hash,
                    "flags": ["-O2", "-G0"], "tools": tools, "targets": []}
        task = {"id": "unit", "kind": "candidate", "source": "candidates/body.c",
                "state": "queued", "targets": []}
        outcome = {"id": self.trial_id, "state": state, "children": [], "integration_credit": 0}
        if compiled:
            object_image = object_image or elf(elf_type=1)
            self.put(self.work / "candidate.o", object_image)
            outcome["object_sha256"] = diff._hash(object_image)
        for ident in targets:
            root = self.work / "targets" / ident
            catalog = {"target": "SCUS_972.68", "program": "boot", "flags": semantic["flags"],
                       "reference_sha256": diff._hash(reference), "functions": functions}
            if readonly:
                catalog["read_only_sections"] = [{"section": ".rodata", "address": 0x2000,
                                                   "size": 4, "sha256": "c" * 64}]
            self.put(root / "catalog.json", catalog)
            self.put(root / "reference.elf", reference)
            child = {"id": ident, "state": "mismatch" if linked else "link_or_check_failed" if compiled else "unmeasured",
                     "functions": [], "read_only_sections": []}
            if linked:
                self.put(root / "candidate.elf", candidate)
                child["candidate_elf_sha256"] = diff._hash(candidate)
            if child["state"] != "unmeasured":
                self.put(root / "outcome.json", child)
            elif (root / "outcome.json").exists():
                (root / "outcome.json").unlink()
            outcome["children"].append(child)
            task["targets"].append({"id": ident, "catalog": "ignored-current-catalog", "reference": "ignored-current-reference"})
            semantic["targets"].append({"program": "boot", "catalog_sha256": diff._hash(diff._encoded(catalog)),
                                        "reference_sha256": diff._hash(reference)})
        self.manifest = {"schema": 1, "kind": "rac2-candidate-trial", "id": self.trial_id,
                         "task": "unit", "semantic_inputs": semantic, "semantic_key": diff._hash(diff._encoded(semantic)),
                         "task_snapshot": task, "instruments": {}}
        self.outcome = outcome
        self.registry = {"tasks": {"unit": {**task, "state": "stopped", "last_trial": self.trial_id}},
                         "trials": {self.trial_id: {"id": self.trial_id, "task": "unit", "state": state,
                                    "directory": "runtime:trials/" + self.trial_id}}}
        self.seal()

    def seal(self):
        semantic = self.manifest["semantic_inputs"]
        key = diff._hash(diff._encoded(semantic))
        self.manifest["semantic_key"] = key
        self.put(self.work / "manifest.json", self.manifest)
        self.put(self.work / "outcome.json", self.outcome)
        self.registry["trials"][self.trial_id].update(
            semantic_key=key, manifest_sha256=diff._hash(diff._encoded(self.manifest)),
            outcome_sha256=diff._hash(diff._encoded(self.outcome)))
        self.put(self.registry_path, self.registry)

    def data(self, **kwargs):
        return diff.review_data(self.store, self.repo, "unit", **kwargs)

    def test_exact_body_distinct_from_recorded_trial_and_no_writes(self):
        before = {str(p): p.read_bytes() for p in self.base.rglob("*") if p.is_file()}
        data = self.data()
        view = data["views"][0]
        self.assertIn("raw exact complete body", view["status"])
        self.assertIn("historical; viewer adds no credit", view["status"])
        self.assertNotIn("unintegrated", view["status"])
        self.assertEqual(view["different_bytes"], 0)
        self.assertEqual(data["trial_state"], "mismatch")
        self.assertEqual(data["integration_credit"], 0)
        self.assertTrue(data["historical"])
        self.assertEqual(before, {str(p): p.read_bytes() for p in self.base.rglob("*") if p.is_file()})
        receipt = diff.render_review(self.store, self.repo, "unit", output=self.base / "review.html")
        self.assertEqual(receipt["selected_symbol"], "FUN_1")
        self.assertEqual(receipt["recorded_trial_state"], "mismatch")
        self.assertEqual(receipt["integration_credit"], 0)
        for path, content in before.items():
            self.assertEqual(Path(path).read_bytes(), content)
        self.assertEqual(set(str(p) for p in self.base.rglob("*") if p.is_file()) - set(before),
                         {str(self.base / "review.html")})

    def test_every_changed_byte_including_size_extension_and_truncation(self):
        for payload, expected in ((b"\x01\0\0\0" + bytes(8), 5), (bytes(4), 4),
                                  (bytes(4) + b"\xFF\xFE\xFD\xFC", 4)):
            with self.subTest(payload=payload):
                self.fixture(candidate=elf(payload))
                view = self.data()["views"][0]
                self.assertEqual(view["different_bytes"], expected)
                self.assertEqual(sum(sum(row["changed"]) for row in view["rows"]), expected)
                self.assertEqual(len(view["rows"]), max(8, len(payload)) // 4)
                if len(payload) != 8:
                    self.assertNotIn("raw exact complete body", view["status"])
                    self.assertIn("Complete symbol size differs", " ".join(view["reasons"]))

    def test_misplaced_symbol_rows_use_absolute_addresses_without_fuzzy_alignment(self):
        self.fixture(candidate=elf(address=0x1100))
        view = self.data()["views"][0]
        self.assertEqual([row["address"] for row in view["rows"]], [0x1000, 0x1004, 0x1100, 0x1104])
        self.assertEqual(view["different_bytes"], 16)
        self.assertNotIn("raw exact complete body", view["status"])

    def test_unaligned_symbol_and_zero_size_never_raw_exact(self):
        for candidate in (elf(bytes(12), functions=[("FUN_1", 0x1001, 8)]),
                          elf(functions=[("FUN_1", 0x1000, 0)])):
            self.fixture(candidate=candidate)
            view = self.data()["views"][0]
            self.assertNotIn("raw exact complete body", view["status"])
        # Overlapping 4-byte windows never count the same address twice.
        self.fixture(candidate=elf(bytes(12), functions=[("FUN_1", 0x1001, 8)]))
        self.assertEqual(self.data()["views"][0]["different_bytes"], 2)

    def test_all_snapshot_hash_drifts_refused_before_output(self):
        paths = ["manifest.json", "outcome.json", "source/body.c", "candidate.o",
                 "targets/boot/catalog.json", "targets/boot/reference.elf", "targets/boot/candidate.elf"]
        for relative in paths:
            with self.subTest(relative=relative):
                path = self.work / relative
                original = path.read_bytes()
                path.write_bytes(original + b" ")
                destination = self.base / "must-not-exist.html"
                with self.assertRaises(ValueError):
                    diff.render_review(self.store, self.repo, "unit", output=destination)
                self.assertFalse(destination.exists())
                path.write_bytes(original)

    def test_child_outcome_tampering_and_missing_measured_outcome_refused(self):
        path = self.work / "targets/boot/outcome.json"
        self.put(path, {"id": "boot", "state": "exact_private"})
        with self.assertRaisesRegex(ValueError, "Child outcome"):
            self.data()
        path.unlink()
        with self.assertRaisesRegex(ValueError, "Measured child"):
            self.data()

    def test_semantic_key_and_target_pins_are_independently_validated(self):
        self.manifest["semantic_inputs"]["source_name"] = "other.c"
        self.put(self.work / "manifest.json", self.manifest)
        self.registry["trials"][self.trial_id]["manifest_sha256"] = diff._hash(diff._encoded(self.manifest))
        self.put(self.registry_path, self.registry)
        with self.assertRaisesRegex(ValueError, "semantic key"):
            self.data()
        self.fixture()
        self.manifest["semantic_inputs"]["targets"][0]["catalog_sha256"] = "f" * 64
        self.seal()
        with self.assertRaisesRegex(ValueError, "semantic target pins"):
            self.data()

    def test_directory_traversal_and_source_symlink_escape_refused(self):
        self.registry["trials"][self.trial_id]["directory"] = "runtime:trials/../../outside"
        self.put(self.registry_path, self.registry)
        with self.assertRaisesRegex(ValueError, "UUID location"):
            self.data()
        self.fixture()
        self.manifest["semantic_inputs"]["source_name"] = "../body.c"
        self.seal()
        with self.assertRaisesRegex(ValueError, "source filename"):
            self.data()
        self.fixture()
        source_dir = self.work / "source"
        outside = self.base / "outside"
        self.put(outside / "body.c", (source_dir / "body.c").read_bytes())
        (source_dir / "body.c").unlink()
        source_dir.rmdir()
        if os.name == "nt":
            result = subprocess.run(["cmd", "/c", "mklink", "/J", str(source_dir), str(outside)],
                                    capture_output=True, check=False)
            if result.returncode:
                self.skipTest("Host does not permit directory junction fixtures")
        else:
            os.symlink(outside, source_dir, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, "escaped"):
            self.data()

    def test_unregistered_uuid_and_wrong_task_or_running_trial_refused(self):
        for changes in ({"task": "other"}, {"state": "running"}, {"id": "b" * 32}):
            self.fixture()
            self.registry["trials"][self.trial_id].update(changes)
            self.put(self.registry_path, self.registry)
            with self.assertRaises(ValueError):
                self.data()
        with self.assertRaises(ValueError):
            self.data(trial_id="../unsafe")
        with self.assertRaises(ValueError):
            self.data(trial_id="b" * 32)

    def test_private_new_html_and_evidence_output_boundaries(self):
        for path in (self.repo / "unsafe.html", self.work / "unsafe.html",
                     self.runtime / "actions/unsafe.html", self.runtime / "registry-revisions/unsafe.html",
                     self.base / "unsafe.txt"):
            with self.subTest(path=path), self.assertRaises(ValueError):
                diff.render_review(self.store, self.repo, "unit", output=path)
        destination = self.base / "exists.html"
        destination.write_text("preserve", encoding="utf-8")
        with self.assertRaises(ValueError):
            diff.render_review(self.store, self.repo, "unit", output=destination)
        self.assertEqual(destination.read_text(), "preserve")
        receipt = diff.render_review(self.store, self.repo, "unit")
        self.assertTrue(Path(receipt["output"]).is_relative_to(self.runtime / "reviews"))
        other = self.base / "other-checkout"
        self.put(other / ".git", b"gitdir: private-pointer")
        with self.assertRaisesRegex(ValueError, "any Git checkout"):
            diff.render_review(self.store, self.repo, "unit", output=other / "nested/review.html")

    def test_multifunction_target_selection_html_controls_and_xss_escaping(self):
        functions = [{"symbol": "FUN_1", "address": 0x1000, "size": 8},
                     {"symbol": "FUN_2", "address": 0x1008, "size": 8}]
        image = elf(bytes(16), functions=[("FUN_1", 0x1000, 8), ("FUN_2", 0x1008, 8)])
        self.fixture(candidate=image, reference=image, targets=("boot", "overlay"), functions=functions)
        data = self.data(target="overlay", symbol="FUN_2")
        self.assertEqual(len(data["views"]), 4)
        self.assertEqual(data["selected"], {"target": "overlay", "symbol": "FUN_2"})
        for selector in ({"target": "absent"}, {"target": "boot", "symbol": "absent"}):
            with self.assertRaises(ValueError):
                self.data(**selector)
        receipt = diff.render_review(self.store, self.repo, "unit", target="overlay", symbol="FUN_2")
        html = Path(receipt["output"]).read_text(encoding="utf-8")
        self.assertNotIn("</script><script>alert", html)
        self.assertIn(r"\u003c/script\u003e", html)
        self.assertIn(".textContent=data.source", html)
        self.assertNotIn("innerHTML", html)
        self.assertIn("connect-src 'none'", html)
        self.assertIn('id="previous"', html)
        self.assertIn('id="next"', html)
        self.assertIn('id="only"', html)
        start = html.index('<script id="review-data" type="application/json">') + len('<script id="review-data" type="application/json">')
        decoded = json.loads(html[start:html.index("</script>", start)])
        self.assertEqual(decoded["source"], data["source"])
        self.assertEqual(decoded["selected"], data["selected"])
        self.assertIn('id="empty" role="status" hidden', html)
        self.assertIn("No differing instruction rows for this selection", html)
        self.assertIn("byId('empty').hidden=!(only.checked&&diffRows.length===0)", html)

    def test_default_task_snapshot_symbol_and_explicit_override(self):
        functions = [{"symbol": "FUN_1", "address": 0x1000, "size": 8},
                     {"symbol": "FUN_2", "address": 0x1008, "size": 8}]
        image = elf(bytes(16), functions=[("FUN_1", 0x1000, 8), ("FUN_2", 0x1008, 8)])
        self.fixture(candidate=image, reference=image, targets=("boot", "overlay"), functions=functions)
        self.manifest["task_snapshot"]["symbols"] = ["absent", "FUN_2"]
        self.seal()
        self.assertEqual(self.data()["selected"], {"target": "boot", "symbol": "FUN_2"})
        self.assertEqual(self.data(target="overlay")["selected"], {"target": "overlay", "symbol": "FUN_2"})
        self.assertEqual(self.data(target="overlay", symbol="FUN_1")["selected"],
                         {"target": "overlay", "symbol": "FUN_1"})
        self.manifest["task_snapshot"]["symbols"] = ["absent"]
        self.seal()
        self.assertEqual(self.data()["selected"]["symbol"], "FUN_1")

    def test_historical_inputs_survive_current_repo_drift(self):
        self.manifest["instruments"] = {"scripts/historical.py": "f" * 64}
        self.seal()
        self.put(self.repo / "scripts/historical.py", b"changed")
        self.registry["tasks"]["unit"]["source"] = "no-longer-exists.c"
        self.put(self.registry_path, self.registry)
        data = self.data()
        self.assertEqual(data["historical_instrument_drift"], ["scripts/historical.py"])
        self.assertIn("raw exact complete body", data["views"][0]["status"])

    def test_link_failed_pinned_object_fallback_and_unmeasured_compile_failure(self):
        self.fixture(linked=False, object_image=elf(elf_type=1, relocation=True))
        view = self.data()["views"][0]
        self.assertTrue(view["relative"])
        self.assertEqual(view["mode"], "relocatable object diagnostic")
        self.assertNotIn("raw exact complete body", view["status"])
        self.assertEqual([row["address"] for row in view["rows"]], [0, 4])
        self.fixture(linked=False, compiled=False, state="compile_failed")
        view = self.data()["views"][0]
        self.assertEqual(view["mode"], "unmeasured")
        self.assertTrue(all(value is None for row in view["rows"] for value in row["candidate"]))

    def test_linked_unresolved_relocations_and_data_unit_mismatch_never_raw_exact(self):
        self.fixture(candidate=elf(relocation=True))
        view = self.data()["views"][0]
        self.assertEqual(view["different_bytes"], 0)
        self.assertNotIn("raw exact complete body", view["status"])
        self.fixture(readonly=True)
        view = self.data()["views"][0]
        self.assertEqual(view["different_bytes"], 0)
        self.assertNotIn("raw exact complete body", view["status"])
        self.assertIn("readonly data unit", " ".join(view["reasons"]))

    def test_missing_or_invalid_complete_candidate_symbols_fail_closed(self):
        self.fixture(candidate=elf(functions=[]))
        view = self.data()["views"][0]
        self.assertEqual(view["different_bytes"], 8)
        self.assertNotIn("raw exact complete body", view["status"])
        self.fixture(candidate=elf(functions=[("FUN_1", 0x1000, 12)]))
        with self.assertRaisesRegex(ValueError, "outside executable section"):
            self.data()
        self.fixture(candidate=elf(functions=[("FUN_1", 0x1000, 8), ("FUN_1", 0x1000, 8)]))
        with self.assertRaisesRegex(ValueError, "Duplicate function"):
            self.data()


if __name__ == "__main__":
    unittest.main()
