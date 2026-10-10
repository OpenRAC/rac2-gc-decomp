"""Exact provenance mutations and unsafe ZIP refusal; no live API or compiler."""
import copy
import hashlib
import io
import json
from pathlib import Path
import stat
import sys
import tempfile
import unittest
from unittest.mock import patch
import zipfile
import urllib.error
import contextlib
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import ci_export as export
import ci_export_reuse as reuse
import maintainer_test_policy as policy


class Api:
    def __init__(self, rows, data):
        self.rows, self.data = rows, data
        self.calls = []
        self.change = None
    def get(self, path):
        self.calls.append(path)
        if self.change:
            self.change(self, path)
        return copy.deepcopy(self.rows[path])
    def archive(self, ident):
        assert ident == 77
        return self.data


def zip_bytes(files):
    stream = io.BytesIO()
    with zipfile.ZipFile(stream, "w") as archive:
        for name, data in files.items():
            archive.writestr(name, data)
    return stream.getvalue()


class ReuseTests(unittest.TestCase):
    def setUp(self):
        self.sha = "a" * 40
        self.wf, self.code = "b" * 64, "c" * 64
        self.inputs = {policy.WORKFLOW: self.wf, "scripts/ci_export.py": self.code, "src/x.c": "d" * 64}
        self.route = {"mode": "reuse", "commit_sha": self.sha, "queue_run_id": 50,
                      "queue_run_attempt": 2, "workflow_sha256": self.wf}
        self.files = {name: b"{}\n" for name in export.FILES}
        receipt = {"schema": 1, "kind": "verified-ci-full-export", "mode": "full", "event": "merge_group",
                   "commit_sha": self.sha, "run_id": 50, "run_attempt": 2, "repository": policy.REPOSITORY,
                   "repository_id": policy.REPOSITORY_ID, "workflow_path": policy.WORKFLOW,
                   "workflow_sha256": self.wf, "exporter_sha256": self.code, "inputs": self.inputs,
                   "checks": export.CHECKS, "tools": {"decoder": "e" * 64},
                   "outputs": {n: export.sha(b) for n, b in self.files.items()}}
        self.files["receipt.json"] = json.dumps(receipt).encode()
        self.data = zip_bytes(self.files)
        repository = {"id": policy.REPOSITORY_ID, "full_name": policy.REPOSITORY}
        artifact = {"id": 77, "name": "SCUS_972.68_verified-export-2", "expired": False,
                    "digest": "sha256:" + export.sha(self.data), "workflow_run": {
                        "id": 50, "head_sha": self.sha, "repository_id": policy.REPOSITORY_ID,
                        "head_repository_id": policy.REPOSITORY_ID}}
        run = {"id": 50, "run_attempt": 2, "workflow_id": 20, "event": "merge_group", "head_sha": self.sha,
               "repository": repository, "head_repository": repository, "path": policy.WORKFLOW,
               "status": "completed", "conclusion": "success"}
        jobs = [{"name": name, "run_id": 50, "head_sha": self.sha, "status": "completed", "conclusion": "success",
                 "steps": [{"name": step, "status": "completed", "conclusion": "success"}
                           for step in (policy.FULL_STEP, reuse.EXPORT_STEP)] if name == reuse.PRODUCER else []}
                for name in (reuse.PRODUCER, *reuse.REQUIRED)]
        self.rows = {"actions/runs/50": run, "actions/workflows/tests.yml": {"id": 20, "path": policy.WORKFLOW, "state": "active"},
                     "actions/runs/50/attempts/2/jobs?per_page=100": {"total_count": 3, "jobs": jobs},
                     "actions/runs/50/artifacts?per_page=100": {"total_count": 1, "artifacts": [artifact]},
                     "actions/artifacts/77": artifact}

    def verify(self, rows=None, files=None, inputs=None):
        data = self.data if files is None else zip_bytes(files)
        rows = copy.deepcopy(self.rows if rows is None else rows)
        digest = "sha256:" + export.sha(data)
        rows["actions/artifacts/77"]["digest"] = digest
        for row in rows["actions/runs/50/artifacts?per_page=100"]["artifacts"]:
            row["digest"] = digest
        return reuse.verified_bundle(Api(rows, data), self.route, self.inputs if inputs is None else inputs, self.wf, self.code)

    def test_exact_success_preserves_original_receipt_and_files(self):
        files, receipt, snapshot = self.verify()
        self.assertEqual(files, self.files)
        self.assertEqual(receipt["run_attempt"], 2)

    def test_every_run_identity_and_attempt_field_is_required(self):
        for key, value in (("event", "push"), ("head_sha", "f" * 40), ("workflow_id", 21), ("run_attempt", 1),
                           ("run_attempt", True), ("id", 51), ("path", ".github/other.yml"),
                           ("repository", {"id": 1}), ("head_repository", {"id": 1}), ("conclusion", "failure")):
            with self.subTest(key=key):
                rows = copy.deepcopy(self.rows)
                rows["actions/runs/50"][key] = value
                with self.assertRaises(ValueError): self.verify(rows=rows)

    def test_producer_and_both_consumers_must_succeed_once(self):
        key = "actions/runs/50/attempts/2/jobs?per_page=100"
        for index in range(3):
            for field, value in (("name", "lookalike"), ("run_id", 51), ("head_sha", "f" * 40),
                                 ("status", "in_progress"), ("conclusion", "skipped")):
                with self.subTest(index=index, field=field):
                    rows = copy.deepcopy(self.rows)
                    rows[key]["jobs"][index][field] = value
                    with self.assertRaises(ValueError): self.verify(rows=rows)
        rows = copy.deepcopy(self.rows)
        rows[key]["jobs"].append(copy.deepcopy(rows[key]["jobs"][0]))
        rows[key]["total_count"] = 4
        with self.assertRaises(ValueError): self.verify(rows=rows)

    def test_complete_suite_and_export_steps_cannot_be_skipped(self):
        key = "actions/runs/50/attempts/2/jobs?per_page=100"
        for index in (0, 1):
            rows = copy.deepcopy(self.rows)
            rows[key]["jobs"][0]["steps"][index]["conclusion"] = "skipped"
            with self.assertRaises(ValueError): self.verify(rows=rows)

    def test_expired_foreign_wrong_attempt_artifact_refused(self):
        for target, field, value in (("artifact", "expired", True), ("artifact", "name", "SCUS_972.68_verified-export-1"),
                                     ("origin", "head_sha", "f" * 40), ("origin", "id", 51),
                                     ("origin", "repository_id", 1)):
            with self.subTest(field=field):
                rows = copy.deepcopy(self.rows)
                artifact = rows["actions/artifacts/77"]
                (artifact if target == "artifact" else artifact["workflow_run"])[field] = value
                rows["actions/runs/50/artifacts?per_page=100"]["artifacts"][0] = copy.deepcopy(artifact)
                with self.assertRaises(ValueError): self.verify(rows=rows)

    def test_manifest_epoch_code_inputs_checks_or_output_tamper_refused(self):
        for key, value in (("mode", "source"), ("event", "push"), ("run_attempt", 1),
                           ("exporter_sha256", "f" * 64), ("workflow_sha256", "f" * 64),
                           ("inputs", {}), ("checks", []), ("outputs", {})):
            with self.subTest(key=key):
                files = copy.deepcopy(self.files)
                receipt = json.loads(files["receipt.json"])
                receipt[key] = value
                files["receipt.json"] = json.dumps(receipt).encode()
                with self.assertRaises(ValueError): self.verify(files=files)
        files = copy.deepcopy(self.files)
        files["report.json"] += b"modified"
        with self.assertRaises(ValueError): self.verify(files=files)
        with self.assertRaises(ValueError): self.verify(inputs={})

    def test_api_change_between_reads_and_errors_refuse(self):
        api = Api(copy.deepcopy(self.rows), self.data)
        calls = 0
        def change(value, path):
            nonlocal calls
            if path == "actions/runs/50":
                calls += 1
                if calls == 2: value.rows[path]["run_attempt"] = 3
        api.change = change
        with self.assertRaises(ValueError): reuse.verified_bundle(api, self.route, self.inputs, self.wf, self.code)
        with patch.object(Api, "get", side_effect=PermissionError("secret-token")):
            with self.assertRaises(PermissionError): self.verify()

    def test_pagination_duplicate_or_missing_artifact_refused(self):
        key = "actions/runs/50/artifacts?per_page=100"
        for value in ({"total_count": 101, "artifacts": []}, {"total_count": 0, "artifacts": []},
                      {"total_count": 2, "artifacts": self.rows[key]["artifacts"] * 2}):
            rows = copy.deepcopy(self.rows)
            rows[key] = value
            with self.assertRaises(ValueError): self.verify(rows=rows)

    def test_archive_digest_and_unsafe_paths_extra_files_duplicate_symlink_refused(self):
        with self.assertRaises(ValueError): reuse.unpack(self.data, "sha256:" + "0" * 64)
        # Assemble synthetic drive/rooted paths only for the hostile ZIP fixture.
        for name in ("../escape", "x/../../escape", "/absolute", "C" + ":/secret", "\\" + "evil", "candidate.elf"):
            files = copy.deepcopy(self.files)
            files[name] = files.pop("report.json")
            data = zip_bytes(files)
            with self.subTest(name=name), self.assertRaises(ValueError): reuse.unpack(data, "sha256:" + export.sha(data))
        stream = io.BytesIO()
        with zipfile.ZipFile(stream, "w") as archive:
            for name, body in self.files.items():
                entry = zipfile.ZipInfo(name)
                if name == "report.json": entry.external_attr = (stat.S_IFLNK | 0o777) << 16
                archive.writestr(entry, body)
        data = stream.getvalue()
        with self.assertRaises(ValueError): reuse.unpack(data, "sha256:" + export.sha(data))

    def test_size_limit_and_empty_or_extra_outputs_refused(self):
        with patch.object(reuse, "MAX_FILE", 1):
            with self.assertRaises(ValueError): reuse.unpack(self.data, "sha256:" + export.sha(self.data))
        for mutation in ("remove", "extra"):
            files = copy.deepcopy(self.files)
            if mutation == "remove": del files["report.json"]
            else: files["secret.env"] = b"private"
            data = zip_bytes(files)
            with self.assertRaises(ValueError): reuse.unpack(data, "sha256:" + export.sha(data))

    def test_signed_download_never_receives_api_authorization(self):
        requests = []
        def opened(request, timeout):
            requests.append(request)
            if len(requests) == 1:
                raise urllib.error.HTTPError(request.full_url, 302, "Found",
                    {"Location": "https://example.invalid/signed?private-download-token"}, None)
            return io.BytesIO(b"archive-data")
        with patch.object(reuse.urllib.request, "build_opener", return_value=SimpleNamespace(open=opened)):
            self.assertEqual(reuse.Api("private-api-token").archive(77), b"archive-data")
        self.assertEqual(requests[0].get_header("Authorization"), "Bearer private-api-token")
        self.assertIsNone(requests[1].get_header("Authorization"))

    def test_permission_failure_falls_back_without_disclosing_error_or_body(self):
        private = Path(__file__).resolve().parents[2]
        with tempfile.TemporaryDirectory(dir=private) as directory:
            event = Path(directory) / "event.json"
            output = Path(directory) / "output"
            event.write_bytes(b"{}")
            stream = io.StringIO()
            with patch.object(policy, "select", side_effect=PermissionError("private-api-token private-prose")), \
                 patch.object(sys, "argv", ["reuse", "--event", str(event), "--output", str(output), "--objdiff", "unused"]), \
                 contextlib.redirect_stdout(stream):
                self.assertEqual(reuse.main(), 0)
            self.assertEqual(output.read_text(), "reused=false\n")
            self.assertIn("identity/api", stream.getvalue())
            self.assertNotIn("private-api-token", stream.getvalue())
            self.assertNotIn("private-prose", stream.getvalue())

    def restore_fixture(self, mismatch_tools=False, bad_cli=False):
        private = Path(__file__).resolve().parents[2]
        temp = tempfile.TemporaryDirectory(dir=private)
        self.addCleanup(temp.cleanup)
        repo = Path(temp.name)
        binary = repo / "objdiff"
        binary.write_bytes(b"fake-objdiff-fixture")
        receipt = json.loads(self.files["receipt.json"])
        original_sha = export.sha
        def digest(body):
            if body == b"fake-objdiff-fixture":
                return "c8290281e82114bcc1a06ff73061110d3902a177822e750337de2537188e358f"
            return original_sha(body)
        def cli(_repo, *args):
            Path(args[-1]).write_bytes(b"changed consumer output" if bad_cli else b"{}\n")
        mocks = (patch.object(export, "sha", side_effect=digest), patch.object(export, "inputs", return_value=self.inputs),
                 patch.object(export, "tools", return_value={} if mismatch_tools else receipt["tools"]),
                 patch.object(export, "command", side_effect=cli),
                 patch.dict(sys.modules, {"decomp_report": SimpleNamespace(measures=lambda *args: {})}))
        with contextlib.ExitStack() as stack:
            for value in mocks: stack.enter_context(value)
            reuse.restore(repo, self.files, receipt, binary)
        return repo

    def test_restore_checks_actual_consumer_outputs_and_keeps_receipt_bytes(self):
        repo = self.restore_fixture()
        for name, target in export.FILES.items():
            self.assertEqual((repo / target).read_bytes(), self.files[name])
        self.assertEqual((repo / "build/decomp/verified-export/receipt.json").read_bytes(), self.files["receipt.json"])

    def test_changed_runtime_tools_or_cli_result_cannot_restore_success(self):
        with self.assertRaises(ValueError): self.restore_fixture(mismatch_tools=True)
        with self.assertRaises(ValueError): self.restore_fixture(bad_cli=True)

    def test_duplicate_zip_names_cannot_be_treated_as_one_verified_file(self):
        stream = io.BytesIO()
        with zipfile.ZipFile(stream, "w") as archive:
            for name, body in self.files.items():
                archive.writestr("report.json" if name == "receipt.json" else name, body)
        data = stream.getvalue()
        with self.assertRaises(ValueError): reuse.unpack(data, "sha256:" + export.sha(data))


if __name__ == "__main__": unittest.main()
