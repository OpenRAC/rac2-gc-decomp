"""Concurrent GitHub claims and trusted metadata, without network or game data."""
import base64
import copy
import gzip
import hashlib
import json
from pathlib import Path
import sys
import unittest
from unittest.mock import patch
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
from reservation_core import apply_command, empty_state, normalize_target, validate_state
from reservation_github import (ApiError, Catalogue, CoordinationError, GitHub,
    LEDGER_BRANCH, Ledger, UPSTREAM, decode, encoded, handle_event,
    integration_verified, parse_command, rpc, wait_ack)


def target(address=4096, program="boot", reference="a" * 64):
    return {"target": "SCUS_972.68", "program": program, "reference_sha256": reference,
            "address": address, "size": 16, "symbol": "FUN_" + format(address, "08X"), "family_id": "c" * 64}


def blob(raw):
    return hashlib.sha1(b"blob " + str(len(raw)).encode() + b"\0" + raw).hexdigest()


class MemoryAPI:
    prefix = GitHub.prefix
    def __init__(self):
        self.state = empty_state()
        self.calls = []
        self.files = {}
        self.before_put = None
        self.lose_response = False
        self.actor = "alice"
        self.role = "read"
        self.issue = {"state": "open", "title": "[Reservation] fixture"}
        self.pr = {"merged": True, "base": {"ref": "RAC2", "repo": {"full_name": UPSTREAM}}}

    def user(self): return self.actor
    def maintainer(self, actor): return self.role in {"maintain", "admin"}
    def get(self, path, **kwargs):
        self.calls.append(("GET", path))
        if path.startswith("/contents/reservations.json?"):
            raw = encoded(self.state)
            return {"sha": blob(raw), "size": len(raw), "encoding": "base64", "content": base64.b64encode(raw).decode()}
        if path.startswith("/issues/"): return copy.deepcopy(self.issue)
        if path.startswith("/pulls/"): return copy.deepcopy(self.pr)
        if path.startswith("/git/ref/heads/"): return {"object": {"sha": "1" * 40}}
        raise AssertionError(path)

    def request(self, method, path, **kwargs):
        assert method == "GET" and path == self.prefix
        return {"default_branch": "RAC2"}

    def file(self, path, ref, **kwargs):
        assert ref == "1" * 40
        return self.files[path]

    def post(self, path, value):
        self.calls.append(("POST", path, copy.deepcopy(value)))
        return {"id": 123, "number": 11}

    def put(self, path, value):
        self.calls.append(("PUT", path, copy.deepcopy(value)))
        if path.startswith("/issues/") and path.endswith("/labels"):
            self.issue["labels"] = [{"name": name} for name in value["labels"]]
            return {}
        if self.before_put:
            callback, self.before_put = self.before_put, None
            callback(self)
        if value["sha"] != blob(encoded(self.state)):
            raise ApiError(409)
        assert path == "/contents/reservations.json" and value["branch"] == LEDGER_BRANCH
        self.state = decode(base64.b64decode(value["content"]))
        validate_state(self.state)
        if self.lose_response:
            self.lose_response = False
            raise CoordinationError("github_transport_unconfirmed")
        return {"commit": {"sha": "2" * 40}}


class FixedCatalogue:
    def __init__(self, api): pass
    def target(self, value, **kwargs): return normalize_target(value), False


def event(actor="alice", issue=1, command=10, request=None):
    return {"action": "created", "repository": {"full_name": UPSTREAM}, "sender": {"login": actor},
            "issue": {"number": issue}, "comment": {"id": command, "user": {"login": actor, "type": "User"},
            "body": "/reserve " + json.dumps(request or {"op": "claim", "kind": "functions", "targets": [target()]})}}


class GitHubCoordinationTests(unittest.TestCase):
    def test_cli_transport_pins_public_github_host_even_with_an_enterprise_default(self):
        answer = SimpleNamespace(returncode=0, stdout=b'{"login":"alice"}', stderr=b'')
        with patch.dict('os.environ', {'GH_HOST': 'enterprise.invalid'}), patch('reservation_github.subprocess.run', return_value=answer) as execute:
            self.assertEqual(GitHub().user(), 'alice')
        argv = execute.call_args.args[0]
        self.assertEqual(argv[argv.index('--hostname') + 1], 'github.com')

    def test_racing_claims_recheck_after_cas_and_only_one_wins(self):
        api = MemoryAPI()
        request = {"op": "claim", "kind": "functions", "targets": [target()]}
        def race(api):
            api.state, receipt = apply_command(api.state, request, actor="bob", issue=2, command_id=11, now=1000)
            self.assertTrue(receipt["ok"])
        api.before_put = race
        state, receipt = Ledger(api).apply(request, actor="alice", issue=1, command_id=10, now=1000)
        self.assertFalse(receipt["ok"])
        self.assertEqual(receipt["reason"], "target_conflict")
        self.assertEqual([c["owner"] for c in state["claims"].values()], ["bob"])
        self.assertEqual(state, api.state)

    def test_lost_write_response_is_resolved_without_second_write(self):
        api = MemoryAPI()
        api.lose_response = True
        state, receipt = Ledger(api).apply({"op": "claim", "kind": "functions", "targets": [target()]},
                                         actor="alice", issue=1, command_id=10, now=1000)
        self.assertTrue(receipt["ok"])
        self.assertEqual(len([c for c in api.calls if c[0] == "PUT"]), 1)
        self.assertEqual(state["revision"], 1)

    def test_same_event_replay_does_not_duplicate_claim(self):
        api = MemoryAPI()
        first = handle_event(api, event(), now=1000, catalogue_factory=FixedCatalogue)
        second = handle_event(api, event(), now=1100, catalogue_factory=FixedCatalogue)
        self.assertEqual(first, second)
        self.assertEqual(api.state["revision"], 1)

    def test_committed_event_replay_survives_closed_issue_and_unavailable_catalogue(self):
        api = MemoryAPI()
        first = handle_event(api, event(), now=1000, catalogue_factory=FixedCatalogue)
        api.issue["state"] = "closed"
        def unavailable(*args):
            raise AssertionError("No new catalogue admission on immutable event replay")
        second = handle_event(api, event(), now=1100, catalogue_factory=unavailable)
        self.assertEqual(first, second)
        self.assertEqual(api.state["revision"], 1)

    def test_forged_sender_and_unknown_actor_type_are_rejected_before_write(self):
        for changed in ("sender", "type"):
            api, value = MemoryAPI(), event()
            if changed == "sender": value["sender"]["login"] = "bob"
            else: value["comment"]["user"]["type"] = "Organization"
            with self.assertRaises(CoordinationError): handle_event(api, value, catalogue_factory=FixedCatalogue)
            self.assertEqual(api.state["revision"], 0)

    def test_bot_pr_other_repo_and_unrecognized_comments_are_ignored(self):
        cases = []
        for key in ("bot", "pr", "repository", "body"):
            value = event()
            if key == "bot": value["comment"]["user"]["type"] = "Bot"
            if key == "pr": value["issue"]["pull_request"] = {}
            if key == "repository": value["repository"]["full_name"] = "attacker/fork"
            if key == "body": value["comment"]["body"] = "unrelated conversation"
            cases.append(value)
        for value in cases:
            api = MemoryAPI()
            self.assertEqual(handle_event(api, value)["state"], "ignored")
            self.assertFalse(api.calls)

    def test_view_failure_keeps_committed_claim(self):
        api = MemoryAPI()
        api.post = lambda *args: (_ for _ in ()).throw(ApiError(403))
        receipt = handle_event(api, event(), now=1000, catalogue_factory=FixedCatalogue)
        self.assertTrue(receipt["ok"])
        self.assertTrue(receipt["issue_view_pending"])
        self.assertEqual(len(api.state["claims"]), 1)

    def test_nonreservation_or_closed_issue_cannot_admit_claim(self):
        for change in ({"title": "ordinary issue"}, {"state": "closed"}, {"pull_request": {}}):
            api = MemoryAPI()
            api.issue.update(change)
            with self.assertRaises(CoordinationError): handle_event(api, event(), catalogue_factory=FixedCatalogue)
            self.assertEqual(api.state, empty_state())

    def test_wait_does_not_infer_success_from_newer_command(self):
        api = MemoryAPI()
        handle_event(api, event(command=11), now=1000, catalogue_factory=FixedCatalogue)
        with self.assertRaisesRegex(CoordinationError, "superseded"):
            wait_ack(Ledger(api), 1, 10)

    def test_wrong_logged_in_owner_cannot_borrow_a_claim(self):
        api = MemoryAPI()
        receipt = handle_event(api, event(), now=1000, catalogue_factory=FixedCatalogue)
        with self.assertRaisesRegex(CoordinationError, "authenticated_owner_differs"):
            rpc(api, {"op": "check", "owner": "bob", "claim_id": receipt["claim_id"], "targets": [target()]})

    def test_release_block_or_review_during_metadata_fetch_cannot_authorize_check(self):
        for operation in ("release", "block", "review"):
            api = MemoryAPI()
            receipt = handle_event(api, event(), now=2000000000, catalogue_factory=FixedCatalogue)
            class RacingCatalogue(FixedCatalogue):
                def __init__(self, api):
                    api.state, changed = apply_command(api.state, {"op": operation, "claim_id": receipt["claim_id"]},
                        actor="alice", issue=1, command_id=11, now=2000000001)
                    assert changed["ok"]
            result = rpc(api, {"op": "check", "claim_id": receipt["claim_id"], "targets": [target()]},
                         catalogue_factory=RacingCatalogue)
            self.assertFalse(result["ok"])
            self.assertEqual(result["revision"], 2)

    def test_command_json_rejects_duplicate_keys_code_and_oversize(self):
        for body in ('/reserve {"op":"claim","op":"release"}', '/reserve __import__("os")', '/reserve ' + 'x' * 20000):
            with self.assertRaises(CoordinationError): parse_command(body)

    def test_permission_404_is_ordinary_but_other_errors_fail_closed(self):
        api = GitHub()
        for status in (404, 403, 503):
            api.get = lambda *a, status=status, **k: (_ for _ in ()).throw(ApiError(status))
            if status == 404: self.assertFalse(api.maintainer("external-user"))
            else:
                with self.assertRaises(ApiError): api.maintainer("external-user")
        for role, expected in (("write", False), ("maintain", True), ("admin", True)):
            api.get = lambda *a, role=role, **k: {"role_name": role, "permission": "write"}
            self.assertEqual(api.maintainer("external-user"), expected)

    def test_file_transport_preserves_binary_and_checks_blob_identity(self):
        raw = b'\x1f\x8b\x00\xffbinary\r\n'
        api = GitHub()
        oid = blob(raw)
        api.get = lambda *a, **k: {"sha": oid, "size": len(raw), "encoding": "base64", "content": base64.b64encode(raw).decode()}
        self.assertEqual(api.file("fixed.gz", "1" * 40), raw)
        api.get = lambda *a, **k: {"sha": "0" * 40, "size": len(raw), "encoding": "base64", "content": base64.b64encode(raw).decode()}
        with self.assertRaisesRegex(CoordinationError, "repository_blob_changed"):
            api.file("fixed.gz", "1" * 40)

    def test_workflow_runs_only_trusted_code_and_never_interpolates_body(self):
        raw = (Path(__file__).resolve().parents[1] / ".github/workflows/reservations.yml").read_text()
        self.assertNotIn("pull_request_target", raw)
        self.assertNotIn("concurrency:", raw)
        self.assertIn("ref: ${{ github.sha }}", raw)
        self.assertIn("persist-credentials: false", raw)
        run = next(line for line in raw.splitlines() if "run:" in line)
        self.assertNotIn("github.event", run)
        self.assertIn('"$GITHUB_EVENT_PATH"', run)


class CatalogueTests(unittest.TestCase):
    def api(self, *, integrated=False, tier="flow_supported_inferred"):
        api = MemoryAPI()
        row = [4096, 16, "b" * 64, tier, "c" * 64, [], ["FUN_00001000"], [], 1, []]
        expanded = encoded(row)
        compressed = gzip.compress(expanded, mtime=0)
        function = {"address": 4096, "size": 16, "reference_sha256": "b" * 64, "candidate_sha256": "b" * 64,
                    "matched": True, "integrated": True, "different_bytes": 0, "state": "integrated"}
        proof = encoded({"reference_sha256": "a" * 64, "program": "boot", "state": "integrated",
                         "functions": [function] if integrated else []})
        catalog = {"schema": 1, "target": "SCUS_972.68", "programs": [{"program": "boot", "reference_sha256": "a" * 64}],
            "function_chunks": [{"program": "boot", "path": "boot.ndjson.gz", "max_expanded_bytes": len(expanded),
                "sha256": hashlib.sha256(compressed).hexdigest(), "expanded_sha256": hashlib.sha256(expanded).hexdigest()}],
            "input_pins": [{"path": "progress/integration.json", "sha256": hashlib.sha256(proof).hexdigest()}]}
        api.files = {"config/function-catalog/catalog.json": encoded(catalog),
                     "config/function-catalog/boot.ndjson.gz": compressed, "progress/integration.json": proof}
        return api

    def test_current_credit_excludes_integrated_body_and_allows_inferred_research(self):
        for integrated in (False, True):
            catalogue = Catalogue(self.api(integrated=integrated))
            if integrated:
                with self.assertRaisesRegex(CoordinationError, "already_integrated"): catalogue.target(target())
            else:
                normalized, accepted = catalogue.target(target())
                self.assertEqual(normalized, target())
                self.assertFalse(accepted)

    def test_current_credit_not_boundary_tier_controls_integration(self):
        catalogue = Catalogue(self.api(tier="qualified_complete"))
        self.assertFalse(catalogue.target(target(), allow_integrated=True)[1])

    def test_inferred_and_known_fragment_extents_cannot_admit_matching_claims(self):
        for tier in ("inferred", "ambiguous_fragment", "unknown"):
            with self.assertRaisesRegex(CoordinationError, "unsupported_boundary"):
                Catalogue(self.api(tier=tier)).target(target())

    def test_invalid_extent_reference_program_or_family_cannot_be_claimed(self):
        catalogue = Catalogue(self.api())
        for change in ({"address": 4100}, {"size": 12}, {"reference_sha256": "f" * 64},
                       {"program": "levels/other"}, {"family_id": "f" * 64}):
            with self.assertRaises(CoordinationError): catalogue.target({**target(), **change})

    def test_chunk_and_integrated_proof_pins_are_verified(self):
        for path in ("config/function-catalog/boot.ndjson.gz", "progress/integration.json"):
            api = self.api()
            api.files[path] += b'changed'
            with self.assertRaises(CoordinationError): Catalogue(api).target(target())

    def test_only_merged_upstream_rac2_pr_with_current_credit_marks_integrated(self):
        api = self.api(integrated=True)
        claim = {"targets": [target()]}
        self.assertTrue(integration_verified(api, Catalogue(api), {"pr": 3}, claim))
        for change in ({"merged": False}, {"base": {"ref": "other", "repo": {"full_name": UPSTREAM}}},
                       {"base": {"ref": "RAC2", "repo": {"full_name": "attacker/fork"}}}):
            api.pr.update(change)
            self.assertFalse(integration_verified(api, Catalogue(api), {"pr": 3}, claim))


if __name__ == "__main__":
    unittest.main()
