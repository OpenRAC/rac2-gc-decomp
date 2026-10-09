"""Synthetic tests for operational locks; no canonical proof inputs are used."""

from copy import deepcopy
import importlib.util
from pathlib import Path
import unittest
from unittest.mock import patch


SPEC = importlib.util.spec_from_file_location("reservation_core", Path(__file__).resolve().parents[1] / "scripts" / "reservation_core.py")
core = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(core)


NOW = 2000000000
FAMILY = "f" * 64


def target(address=0x1000, size=16, program="boot", reference="a" * 64, family=None, symbol=None):
    result = {"target": "SCUS_972.68", "program": program, "reference_sha256": reference, "address": address, "size": size}
    if family is not None:
        result["family_id"] = family
    if symbol is not None:
        result["symbol"] = symbol
    return result


class ReservationsTest(unittest.TestCase):
    def setUp(self):
        self.state = core.empty_state()
        self.command = 0

    def apply(self, request, actor="Alice", issue=1, now=NOW, **kwargs):
        self.command += 1
        before = deepcopy(self.state)
        self.state, receipt = core.apply_command(self.state, request, actor=actor, issue=issue, command_id=self.command, now=now, **kwargs)
        self.assertTrue(core.validate_state(before))
        self.assertTrue(core.validate_state(self.state))
        return receipt

    def claim(self, targets=None, **kwargs):
        return self.apply({"op": "claim", "kind": "functions", "targets": targets or [target()]}, **kwargs)

    def test_empty_state_is_fresh_and_strict(self):
        self.assertEqual(self.state, {"schema": 1, "revision": 0, "claims": {}, "issues": {}})
        self.state["claims"]["x"] = {}
        self.assertEqual(core.empty_state()["claims"], {})

    def test_completed_handoffs_keep_locks_without_stopping_long_runs_after_five_hits(self):
        for index in range(7):
            held = self.claim([target(0x1000 + index * 32)], issue=index + 1)
            self.assertTrue(held["ok"])
            reviewed = self.apply({"op": "review", "claim_id": held["claim_id"]}, issue=index + 1)
            self.assertTrue(reviewed["ok"])
        self.assertEqual(len(self.state["claims"]), 7)
        conflict = self.claim([target(0x1000)], actor="Bob", issue=20)
        self.assertEqual(conflict["reason"], "target_conflict")
        active = self.claim([target(0x2000 + index * 32) for index in range(5)], issue=21)
        self.assertTrue(active["ok"])
        self.assertEqual(self.claim([target(0x3000)], issue=22)["reason"], "actor_limit")

    def test_blocked_handoff_never_dispatches_and_does_not_overflow_active_quota(self):
        held = self.claim([target()], issue=1)
        self.apply({"op": "review", "claim_id": held["claim_id"]}, issue=1)
        self.assertTrue(self.claim([target(0x2000 + index * 32) for index in range(5)], issue=2)["ok"])
        self.assertTrue(self.apply({"op": "block", "claim_id": held["claim_id"]}, issue=1)["ok"])
        self.assertFalse(core.check_claim(self.state, held["claim_id"], actor="Alice", targets=[target()], now=NOW)["ok"])
        self.assertTrue(core.validate_state(self.state))

    def test_atomic_claim_and_copy_isolation(self):
        request = {"op": "claim", "kind": "functions", "targets": [target()], "repository": "owner/repo", "branch": "work/topic"}
        before = deepcopy(self.state)
        updated, receipt = core.apply_command(self.state, request, actor="Alice", issue=1, command_id=10, now=NOW)
        self.assertTrue(receipt["ok"])
        self.assertEqual(self.state, before)
        request["targets"][0]["address"] = 0
        self.assertEqual(updated["claims"][receipt["claim_id"]]["targets"][0]["address"], 0x1000)
        receipt["state"] = "private-text"
        self.assertEqual(updated["issues"]["1"]["last_receipt"]["state"], "reserved")

    def test_deterministic_id_and_no_credit_fields(self):
        first = self.claim()
        second_state, second = core.apply_command(core.empty_state(), {"op": "claim", "kind": "functions", "targets": [target()]}, actor="Alice", issue=1, command_id=1, now=NOW)
        self.assertEqual(first, second)
        self.assertEqual(self.state, second_state)
        self.assertEqual(len(first["claim_id"]), 64)
        for forbidden in ("proof", "qualified_complete", "c_credit", "exact", "source", "compiler"):
            self.assertNotIn(forbidden, self.state)
            self.assertNotIn(forbidden, self.state["claims"][first["claim_id"]])

    def test_normalize_target_rejects_malformed_and_untrusted_fields(self):
        bad_values = [None, [], {}, dict(target(), path="C:/private"), dict(target(), symbol="X; rm -rf /"), dict(target(), symbol="é"), dict(target(), symbol="x" * 101), dict(target(), family_id=""), dict(target(), reference_sha256="A" * 64), dict(target(), program="levels/../secret"), dict(target(), program="levels/a/b"), dict(target(), target="SCUS_972.69")]
        for value in bad_values:
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, "^invalid_targets$"):
                core.normalize_target(value)

    def test_target_integer_and_interval_bounds(self):
        for key, value in (("address", True), ("address", 1.0), ("address", -4), ("address", 2), ("address", 2**32), ("size", True), ("size", 0), ("size", 2), ("size", 128 * 1024 + 4)):
            with self.subTest(key=key, value=value), self.assertRaisesRegex(ValueError, "^invalid_targets$"):
                core.normalize_target(dict(target(), **{key: value}))
        with self.assertRaises(ValueError):
            core.normalize_target(target(2**32 - 4, 8))
        self.assertEqual(core.normalize_target(target(2**32 - 4, 4))["size"], 4)
        self.assertEqual(core.normalize_target(target(0, 128 * 1024))["address"], 0)

    def test_unknown_request_fields_and_owner_injection_rejected(self):
        for request in ({"op": "claim", "kind": "functions", "targets": [target()], "owner": "Mallory"}, {"op": "claim", "kind": []}, {"op": "echo-private"}, [], {"op": []}, {"op": "release", "claim_id": "a" * 64, "lease_seconds": 600}):
            with self.subTest(request=request):
                result = self.apply(request)
                self.assertFalse(result["ok"])
                self.assertEqual(result["reason"], "invalid_request")
                self.assertNotIn("Mallory", str(result))
                self.assertNotIn("echo-private", str(result))

    def test_invalid_event_does_not_mutate_or_echo_actor(self):
        for field, value in (("actor", "$(private)"), ("issue", True), ("command_id", False), ("now", 0), ("now", float("inf")), ("maintainer", 1), ("integration_verified", "true")):
            event = dict(actor="Alice", issue=1, command_id=1, now=NOW)
            event[field] = value
            state, receipt = core.apply_command(self.state, {"op": "claim", "kind": "functions", "targets": [target()]}, **event)
            self.assertEqual(state, self.state)
            self.assertEqual(receipt["reason"], "invalid_event")
            self.assertNotIn("$(private)", str(receipt))

    def test_lease_bounds(self):
        for value in (True, 0, 599, 172801, 600.0, "600"):
            with self.subTest(value=value):
                receipt = self.apply({"op": "claim", "kind": "functions", "targets": [target()], "lease_seconds": value})
                self.assertEqual(receipt["reason"], "invalid_lease")
        for value in (600, 172800):
            state, receipt = core.apply_command(core.empty_state(), {"op": "claim", "kind": "functions", "targets": [target()], "lease_seconds": value}, actor="Alice", issue=1, command_id=1, now=NOW)
            self.assertTrue(receipt["ok"])
            self.assertEqual(state["claims"][receipt["claim_id"]]["lease_until"], NOW + value)

    def test_safe_repository_and_branch_metadata(self):
        for field, value in (("repository", "https://github.com/a/b"), ("repository", "a/b/c"), ("branch", "--upload-pack"), ("branch", "a..b"), ("branch", "a//b"), ("branch", "a.lock"), ("branch", "a/.hidden"), ("branch", "foo@{bar}"), ("branch", "a;whoami"), ("branch", "a/")):
            with self.subTest(field=field, value=value):
                result = self.apply(dict(op="claim", kind="functions", targets=[target()], **{field: value}))
                self.assertEqual(result["reason"], "invalid_metadata")

    def test_overlap_alias_conflicts_and_adjacent_is_free(self):
        original = self.claim([target(symbol="FUN_A")])
        result = self.claim([target(0x1008, symbol="FUN_ALIAS")], actor="Bob", issue=2)
        self.assertEqual(result["reason"], "target_conflict")
        self.assertEqual(result["conflicting_claim_id"], original["claim_id"])
        self.assertEqual(result["conflicting_issue"], 1)
        self.assertTrue(self.claim([target(0x1010)], actor="Bob", issue=2)["ok"])

    def test_reference_and_program_identity_keep_addresses_independent(self):
        self.claim()
        self.assertTrue(self.claim([target(program="levels/test_overlay")], actor="Bob", issue=2)["ok"])
        self.assertTrue(self.claim([target(reference="b" * 64)], actor="Bob", issue=3)["ok"])

    def test_batch_rollback_when_one_target_conflicts(self):
        self.claim()
        result = self.claim([target(0x2000), target(0x1004)], actor="Bob", issue=2)
        self.assertFalse(result["ok"])
        self.assertEqual(len(self.state["claims"]), 1)
        self.assertTrue(self.claim([target(0x2000)], actor="Carol", issue=3)["ok"])

    def test_within_batch_aliases_are_invalid(self):
        for values in ([target(), target()], [target(symbol="A"), target(symbol="B")], [target(), target(0x1004)]):
            result = self.claim(values)
            self.assertEqual(result["reason"], "invalid_targets")
        self.assertEqual(self.state["claims"], {})

    def test_five_explicit_targets_per_actor_across_open_claims(self):
        self.assertTrue(self.claim([target(0x1000), target(0x2000), target(0x3000)])["ok"])
        self.assertTrue(self.claim([target(0x4000), target(0x5000)], actor="alice", issue=2)["ok"])
        self.assertEqual(self.claim([target(0x6000)], issue=3)["reason"], "actor_limit")
        self.assertEqual(self.claim([target(0x10000 + i * 16) for i in range(6)], issue=4)["reason"], "invalid_targets")

    def test_issue_has_one_open_claim_and_preserves_history(self):
        first = self.claim()
        self.assertEqual(self.claim([target(0x2000)])["reason"], "issue_occupied")
        self.apply({"op": "release", "claim_id": first["claim_id"]})
        second = self.claim([target(0x2000)])
        self.assertTrue(second["ok"])
        self.assertNotEqual(first["claim_id"], second["claim_id"])
        self.assertEqual(len(self.state["claims"]), 2)
        self.assertEqual(self.state["claims"][first["claim_id"]]["status"], "released")
        self.assertEqual(self.state["issues"]["1"]["claim_id"], second["claim_id"])

    def test_owner_hijack_and_cross_issue_maintainer_refused(self):
        claim = self.claim()["claim_id"]
        for op in ("renew", "release", "review", "block", "integrated"):
            request = {"op": op, "claim_id": claim}
            if op == "integrated":
                request["pr"] = 1
            self.assertEqual(self.apply(request, actor="Bob")["reason"], "not_authorized")
        self.assertEqual(self.apply({"op": "release", "claim_id": claim}, actor="Bob", issue=2, maintainer=True)["reason"], "issue_mismatch")
        self.assertEqual(self.state["claims"][claim]["status"], "reserved")

    def test_maintainer_can_release_after_audit(self):
        claim = self.claim()["claim_id"]
        result = self.apply({"op": "release", "claim_id": claim}, actor="Maintainer", now=NOW + 200000, maintainer=True)
        self.assertTrue(result["ok"])
        retained = self.state["claims"][claim]
        self.assertEqual(retained["owner"], "Alice")
        self.assertEqual(retained["status"], "released")
        self.assertEqual(retained["updated_at"], NOW + 200000)
        self.assertEqual(retained["targets"], [core.normalize_target(target())])
        self.assertTrue(self.claim(actor="Bob", issue=2, now=NOW + 200001)["ok"])

    def test_expired_claim_stays_locked_and_owner_may_renew(self):
        receipt = self.claim()
        claim = receipt["claim_id"]
        expiry = self.state["claims"][claim]["lease_until"]
        self.assertEqual(core.effective_status(self.state["claims"][claim], expiry - 1), "reserved")
        self.assertEqual(core.effective_status(self.state["claims"][claim], expiry), "needs_attention")
        self.assertEqual(self.claim(actor="Bob", issue=2, now=expiry)["reason"], "target_conflict")
        self.assertEqual(core.check_claim(self.state, claim, actor="Alice", targets=[target()], now=expiry)["reason"], "claim_expired")
        self.assertTrue(self.apply({"op": "renew", "claim_id": claim, "lease_seconds": 600}, now=expiry + 1)["ok"])
        self.assertTrue(core.check_claim(self.state, claim, actor="alice", targets=[target()], now=expiry + 1)["ok"])

    def test_expired_claim_still_counts_toward_actor_limit(self):
        self.claim([target(0x1000 + i * 16) for i in range(5)])
        self.assertEqual(self.claim([target(0x3000)], issue=2, now=NOW + 200000)["reason"], "actor_limit")

    def test_review_block_renew_retain_locks_without_dispatch_authority(self):
        claim = self.claim()["claim_id"]
        for op, status in (("review", "in_review"), ("block", "blocked")):
            result = self.apply({"op": op, "claim_id": claim})
            self.assertEqual(result["state"], status)
            self.assertEqual(core.check_claim(self.state, claim, actor="Alice", targets=[target()], now=NOW)["reason"], "claim_not_reserved")
            self.assertEqual(self.claim(actor="Bob", issue=2)["reason"], "target_conflict")
            self.assertEqual(self.apply({"op": "renew", "claim_id": claim})["state"], status)
            self.assertEqual(core.effective_status(self.state["claims"][claim], NOW + 200000), "needs_attention")

    def test_integration_requires_maintainer_and_trusted_verification(self):
        claim = self.claim()["claim_id"]
        request = {"op": "integrated", "claim_id": claim, "pr": 20}
        for privileges in ({}, {"maintainer": True}, {"integration_verified": True}):
            self.assertEqual(self.apply(request, **privileges)["reason"], "integration_unverified")
        self.assertEqual(self.apply(dict(request, pr=True), maintainer=True, integration_verified=True)["reason"], "integration_unverified")
        self.assertEqual(self.apply(request, actor="Maintainer", maintainer=True, integration_verified=True)["state"], "integrated")
        self.assertTrue(self.claim(actor="Bob", issue=2)["ok"])
        self.assertEqual(self.state["claims"][claim]["status"], "integrated")

    def test_closed_claim_cannot_be_renewed_or_released_again(self):
        claim = self.claim()["claim_id"]
        self.apply({"op": "release", "claim_id": claim})
        for op in ("renew", "release", "review", "block"):
            self.assertEqual(self.apply({"op": op, "claim_id": claim})["reason"], "claim_closed")

    def test_check_requires_exact_owner_and_complete_membership(self):
        claim = self.claim([target(), target(0x2000)])["claim_id"]
        self.assertTrue(core.check_claim(self.state, claim, actor="ALICE", targets=[target(0x2000), target()], now=NOW)["ok"])
        self.assertEqual(core.check_claim(self.state, claim, actor="Bob", targets=[target(), target(0x2000)], now=NOW)["reason"], "not_authorized")
        self.assertEqual(core.check_claim(self.state, claim, actor="Alice", targets=[target()], now=NOW)["reason"], "target_mismatch")
        self.assertEqual(core.check_claim(self.state, claim, actor="Alice", targets=[dict(target(), symbol="alias"), target(0x2000)], now=NOW)["reason"], "target_mismatch")

    def test_equal_command_replays_receipt_with_no_revision_or_request_change(self):
        receipt = self.claim()
        before = deepcopy(self.state)
        updated, replay = core.apply_command(self.state, {"op": "release", "claim_id": receipt["claim_id"]}, actor="Alice", issue=1, command_id=1, now=NOW + 1)
        self.assertEqual(replay, receipt)
        self.assertEqual(updated, before)
        replay["state"] = "private"
        self.assertEqual(updated, before)

    def test_old_commands_never_recreate_or_release(self):
        claim = self.claim()["claim_id"]
        release = self.apply({"op": "release", "claim_id": claim})
        before = deepcopy(self.state)
        updated, result = core.apply_command(self.state, {"op": "claim", "kind": "functions", "targets": [target()]}, actor="Alice", issue=1, command_id=1, now=NOW)
        self.assertEqual(result["state"], "ignored")
        self.assertEqual(result["reason"], "older_command")
        self.assertFalse(result["ok"])
        self.assertEqual(updated, before)
        self.assertEqual(self.state["issues"]["1"]["last_receipt"], release)

    def test_refusal_is_monotonic_and_replayable(self):
        self.claim()
        rejected = self.claim(actor="Bob", issue=2)
        before = deepcopy(self.state)
        updated, replay = core.apply_command(self.state, {"op": "claim", "kind": "functions", "targets": [target(0x3000)]}, actor="Bob", issue=2, command_id=2, now=NOW)
        self.assertEqual(rejected, replay)
        self.assertEqual(updated, before)
        self.assertEqual(self.state["revision"], 2)

    def test_out_of_order_admission_after_refusal_cannot_win(self):
        state, rejected = core.apply_command(self.state, {"op": "bad"}, actor="Alice", issue=1, command_id=20, now=NOW)
        updated, ignored = core.apply_command(state, {"op": "claim", "kind": "functions", "targets": [target()]}, actor="Alice", issue=1, command_id=10, now=NOW)
        self.assertEqual(ignored["state"], "ignored")
        self.assertEqual(updated, state)
        self.assertEqual(updated["claims"], {})

    def test_family_claim_blocks_unlisted_members_and_other_overlays(self):
        first = self.apply({"op": "claim", "kind": "family", "targets": [target(family=FAMILY)]})
        self.assertTrue(first["ok"])
        for candidate in (target(0x2000, family=FAMILY), target(0x3000, program="levels/test", family=FAMILY)):
            self.assertEqual(self.claim([candidate], actor="Bob", issue=2)["reason"], "family_conflict")
        self.assertTrue(self.claim([target(0x4000, family="e" * 64)], actor="Bob", issue=3)["ok"])

    def test_prior_individual_member_blocks_family_claim(self):
        self.claim([target(0x2000, family=FAMILY)])
        result = self.apply({"op": "claim", "kind": "family", "targets": [target(family=FAMILY)]}, actor="Bob", issue=2)
        self.assertEqual(result["reason"], "family_conflict")
        self.assertEqual(len(self.state["claims"]), 1)

    def test_family_requires_shared_nonempty_trusted_id(self):
        for values in ([target()], [target(family=FAMILY), target(0x2000, family="e" * 64)]):
            self.assertEqual(self.apply({"op": "claim", "kind": "family", "targets": values})["reason"], "invalid_targets")

    def test_release_frees_family_but_retains_record(self):
        claim = self.apply({"op": "claim", "kind": "family", "targets": [target(family=FAMILY)]})["claim_id"]
        self.apply({"op": "release", "claim_id": claim})
        self.assertTrue(self.claim([target(0x2000, family=FAMILY)], actor="Bob", issue=2)["ok"])
        self.assertEqual(self.state["claims"][claim]["kind"], "family")

    def test_time_reversal_is_refused_without_changing_claim(self):
        claim = self.claim()["claim_id"]
        before = deepcopy(self.state["claims"][claim])
        result = self.apply({"op": "renew", "claim_id": claim}, now=NOW - 1)
        self.assertEqual(result["reason"], "invalid_event")
        self.assertEqual(self.state["claims"][claim], before)
        self.assertEqual(core.check_claim(self.state, claim, actor="Alice", targets=[target()], now=NOW - 1)["reason"], "invalid_event")

    def test_ledger_capacity_fails_closed_with_retained_history(self):
        claim = self.claim()["claim_id"]
        with patch.object(core, "MAX_RECORDS", 1):
            before = deepcopy(self.state)
            result = self.claim([target(0x2000)], actor="Bob", issue=2)
            self.assertEqual(result["reason"], "ledger_capacity")
            self.assertEqual(self.state, before)
            self.apply({"op": "release", "claim_id": claim})
            result = self.claim([target(0x2000)])
            self.assertEqual(result["reason"], "ledger_capacity")
            self.assertEqual(len(self.state["claims"]), 1)
            self.assertEqual(self.state["claims"][claim]["status"], "released")

    def test_corrupted_receipt_and_revision_crosslinks_fail_closed(self):
        claim = self.claim()["claim_id"]
        corruptions = []
        for value in (None, "b" * 64):
            candidate = deepcopy(self.state)
            candidate["issues"]["1"]["last_receipt"]["claim_id"] = value
            corruptions.append(candidate)
        candidate = deepcopy(self.state)
        candidate["issues"]["1"]["last_receipt"]["state"] = "released"
        corruptions.append(candidate)
        candidate = deepcopy(self.state)
        candidate["revision"] = 2
        corruptions.append(candidate)
        candidate = deepcopy(self.state)
        candidate["issues"]["1"]["last_receipt"].update(ok=False, state="ignored", reason="older_command")
        corruptions.append(candidate)
        candidate = deepcopy(self.state)
        candidate["issues"]["1"]["last_receipt"].update(conflicting_claim_id="b" * 64, conflicting_issue=2)
        corruptions.append(candidate)
        candidate = deepcopy(self.state)
        candidate["claims"][claim]["issue"] = 2
        corruptions.append(candidate)
        for candidate in corruptions:
            with self.subTest(candidate=candidate), self.assertRaisesRegex(ValueError, "^invalid_ledger$"):
                core.validate_state(candidate)

    def test_corrupted_family_lock_against_existing_member_is_invalid(self):
        first = self.claim([target(family=FAMILY)])["claim_id"]
        self.claim([target(0x2000, family=FAMILY)], actor="Bob", issue=2)
        candidate = deepcopy(self.state)
        candidate["claims"][first]["kind"] = "family"
        with self.assertRaisesRegex(ValueError, "^invalid_ledger$"):
            core.validate_state(candidate)

    def test_invalid_ledger_fails_closed_and_preserves_input(self):
        receipt = self.claim()
        malformed = []
        for field, value in (("schema", True), ("schema", 2), ("revision", True), ("revision", -1), ("claims", []), ("issues", [])):
            candidate = deepcopy(self.state)
            candidate[field] = value
            malformed.append(candidate)
        candidate = deepcopy(self.state)
        candidate["private"] = "secret"
        malformed.append(candidate)
        for field, value in (("owner", "private/path"), ("status", "completed_exact"), ("last_command_id", True), ("lease_until", NOW - 1), ("targets", [dict(target(), size=True)])):
            candidate = deepcopy(self.state)
            candidate["claims"][receipt["claim_id"]][field] = value
            malformed.append(candidate)
        candidate = deepcopy(self.state)
        candidate["issues"]["1"]["last_receipt"]["reason"] = "private-input"
        malformed.append(candidate)
        candidate = deepcopy(self.state)
        candidate["issues"]["01"] = candidate["issues"].pop("1")
        malformed.append(candidate)
        candidate = deepcopy(self.state)
        candidate["issues"] = {}
        malformed.append(candidate)
        for candidate in malformed:
            with self.subTest(candidate=candidate):
                before = deepcopy(candidate)
                with self.assertRaisesRegex(ValueError, "^invalid_ledger$"):
                    core.validate_state(candidate)
                with self.assertRaisesRegex(ValueError, "^invalid_ledger$"):
                    core.apply_command(candidate, {"op": "bad"}, actor="Alice", issue=1, command_id=100, now=NOW)
                self.assertEqual(candidate, before)

    def test_corrupted_active_overlap_and_actor_limit_rejected(self):
        first = self.claim()
        second = self.claim([target(0x2000)], actor="Bob", issue=2)
        candidate = deepcopy(self.state)
        candidate["claims"][second["claim_id"]]["targets"] = [core.normalize_target(target())]
        with self.assertRaisesRegex(ValueError, "^invalid_ledger$"):
            core.validate_state(candidate)
        candidate = deepcopy(self.state)
        candidate["claims"][first["claim_id"]]["targets"] = [core.normalize_target(target(0x1000 + i * 16)) for i in range(5)]
        candidate["claims"][second["claim_id"]]["owner"] = "Alice"
        with self.assertRaisesRegex(ValueError, "^invalid_ledger$"):
            core.validate_state(candidate)

    def test_nonfinite_effective_status_and_malformed_claim_refused(self):
        claim = {"status": "reserved", "lease_until": NOW + 1}
        for now in (True, 0, -1, float("nan"), float("inf")):
            with self.assertRaisesRegex(ValueError, "^invalid_request$"):
                core.effective_status(claim, now)
        with self.assertRaisesRegex(ValueError, "^invalid_request$"):
            core.effective_status({"status": [], "lease_until": NOW}, NOW)


if __name__ == "__main__":
    unittest.main()
