"""Pure operational reservation ledger; this module grants no proof credit.

All metadata must already have been resolved against the trusted catalogue by
the adapter. No input is executed and no network or filesystem access occurs.
"""

from copy import deepcopy
import hashlib
import json
import re


SCHEMA = 1
MIN_LEASE = 600
MAX_LEASE = 48 * 60 * 60
MAX_TARGETS = 5
MAX_RECORDS = 10000
MAX_INTEGER = 2**63 - 1
STATUSES = frozenset({"reserved", "in_review", "blocked", "released", "integrated"})
OPEN_STATUSES = frozenset({"reserved", "in_review", "blocked"})
_HEX = re.compile(r"[0-9a-f]{64}\Z", re.ASCII)
_LOGIN = re.compile(r"[A-Za-z0-9](?:[A-Za-z0-9-]{0,37}[A-Za-z0-9])?\Z", re.ASCII)
_SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]{0,99}\Z", re.ASCII)
_PROGRAM = re.compile(r"(?:boot|levels/[A-Za-z0-9][A-Za-z0-9_-]{0,63})\Z", re.ASCII)
_REPOSITORY = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,99}/[A-Za-z0-9][A-Za-z0-9_.-]{0,99}\Z", re.ASCII)
_BRANCH = re.compile(r"[A-Za-z0-9][A-Za-z0-9._/-]{0,199}\Z", re.ASCII)
_TARGET_KEYS = {"target", "program", "reference_sha256", "address", "size", "symbol", "family_id"}
_CLAIM_KEYS = {"id", "issue", "owner", "kind", "targets", "repository", "branch", "status", "created_at", "updated_at", "lease_until", "last_command_id"}
_REASONS = frozenset({"invalid_event", "invalid_request", "invalid_targets", "invalid_lease", "invalid_metadata", "issue_occupied", "actor_limit", "target_conflict", "family_conflict", "claim_not_found", "issue_mismatch", "not_authorized", "claim_closed", "integration_unverified", "older_command", "claim_not_reserved", "claim_expired", "target_mismatch", "ledger_capacity"})
_RECEIPT_STATES = STATUSES | {"rejected", "ignored", "needs_attention"}


def _integer(value, minimum=1, maximum=MAX_INTEGER):
    return type(value) is int and minimum <= value <= maximum


def _matches(pattern, value):
    return isinstance(value, str) and pattern.fullmatch(value) is not None


def _fail(code):
    raise ValueError(code)


def empty_state():
    """Return a fresh schema 1 ledger."""
    return {"schema": SCHEMA, "revision": 0, "claims": {}, "issues": {}}


def normalize_target(target):
    """Validate one catalogue target and return an independent canonical dict."""
    if type(target) is not dict or set(target) - _TARGET_KEYS:
        _fail("invalid_targets")
    if not {"target", "program", "reference_sha256", "address", "size"} <= set(target):
        _fail("invalid_targets")
    if target["target"] != "SCUS_972.68" or not _matches(_PROGRAM, target["program"]):
        _fail("invalid_targets")
    if not _matches(_HEX, target["reference_sha256"]):
        _fail("invalid_targets")
    address, size = target["address"], target["size"]
    if not _integer(address, 0, 2**32 - 1) or address % 4:
        _fail("invalid_targets")
    if not _integer(size, 4, 128 * 1024) or size % 4 or address + size > 2**32:
        _fail("invalid_targets")
    if "symbol" in target and not _matches(_SYMBOL, target["symbol"]):
        _fail("invalid_targets")
    if "family_id" in target and not _matches(_HEX, target["family_id"]):
        _fail("invalid_targets")
    return {key: target[key] for key in sorted(target)}


def _target_key(target):
    return json.dumps(target, sort_keys=True, separators=(",", ":"))


def _targets(value):
    if type(value) is not list or not 1 <= len(value) <= MAX_TARGETS:
        _fail("invalid_targets")
    result = [normalize_target(target) for target in value]
    if len({_target_key(target) for target in result}) != len(result):
        _fail("invalid_targets")
    # An alias cannot occupy a second explicit slot within the same batch.
    for index, target in enumerate(result):
        if any(_overlaps(target, earlier) for earlier in result[:index]):
            _fail("invalid_targets")
    return sorted(result, key=_target_key)


def _overlaps(left, right):
    return all(left[key] == right[key] for key in ("target", "program", "reference_sha256")) and left["address"] < right["address"] + right["size"] and right["address"] < left["address"] + left["size"]


def _family(kind, targets):
    if kind != "family":
        return None
    families = {target.get("family_id") for target in targets}
    if len(families) != 1 or None in families:
        _fail("invalid_targets")
    return next(iter(families))


def _conflict(kind, targets, other):
    new_family = _family(kind, targets)
    old_family = _family(other["kind"], other["targets"])
    if (new_family and any(target.get("family_id") == new_family for target in other["targets"])) or (old_family and any(target.get("family_id") == old_family for target in targets)):
        return "family_conflict"
    if any(_overlaps(target, old) for target in targets for old in other["targets"]):
        return "target_conflict"
    return None


def _metadata(repository, branch):
    if repository is not None and not _matches(_REPOSITORY, repository):
        _fail("invalid_metadata")
    if branch is not None:
        if not _matches(_BRANCH, branch) or branch.endswith(("/", ".")) or ".." in branch or "//" in branch or any(part.startswith(".") or part.endswith(".lock") for part in branch.split("/")):
            _fail("invalid_metadata")


def effective_status(claim, now):
    """Expired open claims need attention and continue to hold their locks."""
    if not _integer(now) or type(claim) is not dict or not isinstance(claim.get("status"), str) or claim.get("status") not in STATUSES or not _integer(claim.get("lease_until")):
        _fail("invalid_request")
    if claim["status"] in OPEN_STATUSES and now >= claim["lease_until"]:
        return "needs_attention"
    return claim["status"]


def _validate_receipt(receipt, issue, command_id, revision):
    keys = {"ok", "state", "reason", "issue", "command_id", "revision", "claim_id", "conflicting_claim_id", "conflicting_issue"}
    required = {"ok", "state", "issue", "command_id", "revision", "claim_id"}
    if type(receipt) is not dict or set(receipt) - keys or not required <= set(receipt):
        _fail("invalid_ledger")
    if type(receipt["ok"]) is not bool or receipt["state"] not in _RECEIPT_STATES:
        _fail("invalid_ledger")
    if not _integer(receipt["issue"]) or receipt["issue"] != issue or not _integer(receipt["command_id"]) or receipt["command_id"] != command_id:
        _fail("invalid_ledger")
    if not _integer(receipt["revision"], 1, revision):
        _fail("invalid_ledger")
    if receipt["claim_id"] is not None and not _matches(_HEX, receipt["claim_id"]):
        _fail("invalid_ledger")
    if "reason" in receipt and receipt["reason"] not in _REASONS:
        _fail("invalid_ledger")
    if receipt["ok"] != (receipt["state"] in STATUSES) or (not receipt["ok"] and "reason" not in receipt):
        _fail("invalid_ledger")
    if ("conflicting_claim_id" in receipt) != ("conflicting_issue" in receipt):
        _fail("invalid_ledger")
    if "conflicting_claim_id" in receipt and (not _matches(_HEX, receipt["conflicting_claim_id"]) or not _integer(receipt["conflicting_issue"])):
        _fail("invalid_ledger")


def validate_state(state):
    """Return True for a bounded coherent ledger, else raise a fixed ValueError."""
    try:
        _validate_state(state)
    except (ValueError, TypeError, KeyError, OverflowError):
        _fail("invalid_ledger")
    return True


def _validate_state(state):
    if type(state) is not dict or set(state) != {"schema", "revision", "claims", "issues"} or type(state["schema"]) is not int or state["schema"] != SCHEMA:
        _fail("invalid_ledger")
    if not _integer(state["revision"], 0) or type(state["claims"]) is not dict or type(state["issues"]) is not dict:
        _fail("invalid_ledger")
    if len(state["claims"]) > MAX_RECORDS or len(state["issues"]) > MAX_RECORDS:
        _fail("invalid_ledger")
    intervals, family_holders, family_members = {}, {}, {}
    actor_counts, active_issues = {}, {}
    for claim_id, claim in state["claims"].items():
        if not _matches(_HEX, claim_id) or type(claim) is not dict or set(claim) != _CLAIM_KEYS or claim["id"] != claim_id:
            _fail("invalid_ledger")
        if not _integer(claim["issue"]) or not _matches(_LOGIN, claim["owner"]) or claim["kind"] not in {"functions", "family"} or claim["status"] not in STATUSES:
            _fail("invalid_ledger")
        targets = _targets(claim["targets"])
        _family(claim["kind"], targets)
        _metadata(claim["repository"], claim["branch"])
        if any(not _integer(claim[field]) for field in ("created_at", "updated_at", "lease_until", "last_command_id")):
            _fail("invalid_ledger")
        if not claim["created_at"] <= claim["updated_at"] or not claim["created_at"] <= claim["lease_until"] <= claim["updated_at"] + MAX_LEASE:
            _fail("invalid_ledger")
        if claim["status"] in OPEN_STATUSES:
            if claim["issue"] in active_issues:
                _fail("invalid_ledger")
            active_issues[claim["issue"]] = claim_id
            # Handoffs and explicitly blocked work keep exclusion locks, but
            # only reserved work can dispatch and consumes active slots.
            if claim["status"] == "reserved":
                owner = claim["owner"].lower()
                actor_counts[owner] = actor_counts.get(owner, 0) + len(targets)
                if actor_counts[owner] > MAX_TARGETS:
                    _fail("invalid_ledger")
            family = _family(claim["kind"], targets)
            if family is not None:
                if family in family_holders:
                    _fail("invalid_ledger")
                family_holders[family] = claim_id
            for target in targets:
                identity = tuple(target[key] for key in ("target", "program", "reference_sha256"))
                intervals.setdefault(identity, []).append((target["address"], target["address"] + target["size"]))
                if "family_id" in target:
                    family_members.setdefault(target["family_id"], set()).add(claim_id)
    for placements in intervals.values():
        previous_end = -1
        for address, endpoint in sorted(placements):
            if address < previous_end:
                _fail("invalid_ledger")
            previous_end = endpoint
    for family, holder in family_holders.items():
        if family_members[family] != {holder}:
            _fail("invalid_ledger")
    for issue_key, record in state["issues"].items():
        if not isinstance(issue_key, str) or not issue_key.isascii() or not issue_key.isdecimal() or str(int(issue_key)) != issue_key or not _integer(int(issue_key)):
            _fail("invalid_ledger")
        if type(record) is not dict or set(record) != {"last_command_id", "last_receipt", "claim_id"} or not _integer(record["last_command_id"]):
            _fail("invalid_ledger")
        _validate_receipt(record["last_receipt"], int(issue_key), record["last_command_id"], state["revision"])
        receipt = record["last_receipt"]
        if receipt["claim_id"] != record["claim_id"] or receipt["state"] in {"ignored", "needs_attention"}:
            _fail("invalid_ledger")
        if record["claim_id"] is not None:
            if not _matches(_HEX, record["claim_id"]):
                _fail("invalid_ledger")
            claim = state["claims"].get(record["claim_id"])
            if claim is None or claim["issue"] != int(issue_key):
                _fail("invalid_ledger")
            if receipt["ok"] and receipt["state"] != claim["status"]:
                _fail("invalid_ledger")
        elif receipt["ok"]:
            _fail("invalid_ledger")
        if "conflicting_claim_id" in receipt:
            conflict = state["claims"].get(receipt["conflicting_claim_id"])
            if conflict is None or conflict["issue"] != receipt["conflicting_issue"]:
                _fail("invalid_ledger")
        if int(issue_key) in active_issues and record["claim_id"] != active_issues[int(issue_key)]:
            _fail("invalid_ledger")
    for claim in state["claims"].values():
        record = state["issues"].get(str(claim["issue"]))
        if record is None or claim["last_command_id"] > record["last_command_id"]:
            _fail("invalid_ledger")
    if (state["revision"] == 0) != (not state["issues"]) or len(state["issues"]) > state["revision"]:
        _fail("invalid_ledger")
    receipt_revisions = [record["last_receipt"]["revision"] for record in state["issues"].values()]
    if receipt_revisions and (max(receipt_revisions) != state["revision"] or len(set(receipt_revisions)) != len(receipt_revisions)):
        _fail("invalid_ledger")


def _receipt(ok, status, issue, command_id, revision, claim_id=None, reason=None, conflict=None):
    result = {"ok": ok, "state": status, "issue": issue, "command_id": command_id, "revision": revision, "claim_id": claim_id}
    if reason is not None:
        result["reason"] = reason
    if conflict is not None:
        result.update(conflicting_claim_id=conflict["id"], conflicting_issue=conflict["issue"])
    return result


def check_claim(state, claim_id, *, actor, targets, now):
    """Fail closed unless owner and exact membership hold on a live reservation."""
    validate_state(state)
    def fail(reason):
        return {"ok": False, "state": "rejected", "reason": reason, "claim_id": claim_id if _matches(_HEX, claim_id) else None}
    if not _matches(_HEX, claim_id) or not _matches(_LOGIN, actor) or not _integer(now):
        return fail("invalid_request")
    try:
        normalized = _targets(targets)
    except ValueError:
        return fail("invalid_targets")
    claim = state["claims"].get(claim_id)
    if claim is None:
        return fail("claim_not_found")
    if claim["owner"].lower() != actor.lower():
        return fail("not_authorized")
    if now < claim["updated_at"]:
        return fail("invalid_event")
    if claim["status"] != "reserved":
        return fail("claim_not_reserved")
    if effective_status(claim, now) == "needs_attention":
        return fail("claim_expired")
    if normalized != _targets(claim["targets"]):
        return fail("target_mismatch")
    return {"ok": True, "state": "reserved", "claim_id": claim_id}


def apply_command(state, request, *, actor, issue, command_id, now, maintainer=False, integration_verified=False):
    """Apply one authenticated command atomically; invalid ledgers raise.

    Authenticated refusals are recorded and advance revision/order. Invalid
    event envelopes leave the ledger unchanged. Equal IDs replay the stored
    receipt, older IDs return an ignored receipt without changing the ledger.
    """
    validate_state(state)
    updated = deepcopy(state)
    if not _matches(_LOGIN, actor) or not all(_integer(value) for value in (issue, command_id, now)) or type(maintainer) is not bool or type(integration_verified) is not bool:
        return updated, _receipt(False, "rejected", issue if _integer(issue) else None, command_id if _integer(command_id) else None, state["revision"], reason="invalid_event")
    record = updated["issues"].get(str(issue))
    current_id = record["claim_id"] if record else None
    if record and command_id == record["last_command_id"]:
        return updated, deepcopy(record["last_receipt"])
    if record and command_id < record["last_command_id"]:
        return updated, _receipt(False, "ignored", issue, command_id, state["revision"], current_id, "older_command")
    if state["revision"] == MAX_INTEGER or (record is None and len(updated["issues"]) >= MAX_RECORDS):
        return updated, _receipt(False, "rejected", issue, command_id, state["revision"], current_id, "ledger_capacity")
    revision = updated["revision"] + 1

    def finish(receipt):
        updated["revision"] = revision
        updated["issues"][str(issue)] = {"last_command_id": command_id, "last_receipt": deepcopy(receipt), "claim_id": receipt["claim_id"]}
        return updated, receipt

    def reject(reason, conflict=None):
        return finish(_receipt(False, "rejected", issue, command_id, revision, current_id, reason, conflict))

    if type(request) is not dict or not isinstance(request.get("op"), str):
        return reject("invalid_request")
    op = request["op"]
    allowed = {"claim": {"op", "targets", "kind", "lease_seconds", "repository", "branch"}, "renew": {"op", "claim_id", "lease_seconds"}, "release": {"op", "claim_id"}, "review": {"op", "claim_id"}, "block": {"op", "claim_id"}, "integrated": {"op", "claim_id", "pr"}}
    if op not in allowed or set(request) - allowed[op]:
        return reject("invalid_request")
    lease = request.get("lease_seconds", MAX_LEASE)
    if op in {"claim", "renew"} and (not _integer(lease, MIN_LEASE, MAX_LEASE) or now > MAX_INTEGER - lease):
        return reject("invalid_lease")
    if op == "claim":
        if not isinstance(request.get("kind"), str) or request.get("kind") not in {"functions", "family"}:
            return reject("invalid_request")
        try:
            targets = _targets(request.get("targets"))
            _family(request["kind"], targets)
            _metadata(request.get("repository"), request.get("branch"))
        except ValueError as error:
            return reject(str(error))
        active = [claim for claim in updated["claims"].values() if claim["status"] in OPEN_STATUSES]
        if any(claim["issue"] == issue for claim in active):
            return reject("issue_occupied")
        if sum(len(claim["targets"]) for claim in active
               if claim["owner"].lower() == actor.lower() and claim["status"] == "reserved") + len(targets) > MAX_TARGETS:
            return reject("actor_limit")
        for claim in sorted(active, key=lambda item: item["id"]):
            conflict = _conflict(request["kind"], targets, claim)
            if conflict:
                return reject(conflict, claim)
        if len(updated["claims"]) >= MAX_RECORDS:
            return reject("ledger_capacity")
        claim_id = hashlib.sha256(f"{issue}:{command_id}".encode("ascii")).hexdigest()
        updated["claims"][claim_id] = {"id": claim_id, "issue": issue, "owner": actor, "kind": request["kind"], "targets": targets, "repository": request.get("repository"), "branch": request.get("branch"), "status": "reserved", "created_at": now, "updated_at": now, "lease_until": now + lease, "last_command_id": command_id}
        return finish(_receipt(True, "reserved", issue, command_id, revision, claim_id))
    claim_id = request.get("claim_id")
    if not _matches(_HEX, claim_id):
        return reject("invalid_request")
    claim = updated["claims"].get(claim_id)
    if claim is None:
        return reject("claim_not_found")
    if claim["issue"] != issue:
        return reject("issue_mismatch")
    if actor.lower() != claim["owner"].lower() and not maintainer:
        return reject("not_authorized")
    if claim["status"] not in OPEN_STATUSES:
        return reject("claim_closed")
    if now < claim["updated_at"]:
        return reject("invalid_event")
    if op == "integrated" and (not _integer(request.get("pr")) or not maintainer or not integration_verified):
        return reject("integration_unverified")
    if op == "renew":
        claim["lease_until"] = now + lease
    else:
        claim["status"] = {"release": "released", "review": "in_review", "block": "blocked", "integrated": "integrated"}[op]
    claim["updated_at"] = now
    claim["last_command_id"] = command_id
    return finish(_receipt(True, claim["status"], issue, command_id, revision, claim_id))
