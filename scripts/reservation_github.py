"""GitHub-backed operational claims; no compilation or matching credit."""
from __future__ import annotations

import base64
import gzip
import hashlib
import json
import os
import re
import subprocess
import time
import urllib.error
import urllib.parse
import urllib.request

from reservation_core import (apply_command, check_claim, effective_status,
                              empty_state, normalize_target, validate_state)

UPSTREAM = "OpenRAC/rac2-gc-decomp"
LEDGER_BRANCH = "coordination/reservations"
LEDGER_PATH = "reservations.json"
MAX_LEDGER = 2 * 1024 * 1024
MAX_COMMAND = 16384
LOGIN = re.compile(r"[A-Za-z0-9](?:[A-Za-z0-9-]{0,37}[A-Za-z0-9])?\Z", re.ASCII)
HEX = re.compile(r"[0-9a-f]{64}\Z")
OID = re.compile(r"(?:[0-9a-f]{40}|[0-9a-f]{64})\Z")
SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]{0,99}\Z", re.ASCII)


class CoordinationError(RuntimeError):
    """Only fixed codes, never HTTP bodies, credentials or candidate text."""


class ApiError(CoordinationError):
    def __init__(self, status):
        self.status = status
        super().__init__("github_request_failed_" + str(status))


def decode(raw):
    def unique(pairs):
        result = {}
        for key, value in pairs:
            if key in result:
                raise CoordinationError("duplicate_json_key")
            result[key] = value
        return result
    try:
        return json.loads(raw, object_pairs_hook=unique)
    except (ValueError, UnicodeError, RecursionError):
        raise CoordinationError("invalid_json") from None


def encoded(value):
    return (json.dumps(value, ensure_ascii=True, sort_keys=True, separators=(",", ":")) + "\n").encode()


class GitHub:
    """Fixed upstream REST transport. Workflow token or authenticated gh CLI."""
    prefix = "/repos/" + UPSTREAM

    def __init__(self, token=None):
        self.token = token

    def request(self, method, path, value=None, *, raw=False, limit=MAX_LEDGER * 2):
        if not (path in {"/user", self.prefix} or path.startswith(self.prefix + "/")):
            raise CoordinationError("untrusted_api_path")
        accept = "application/vnd.github.raw+json" if raw else "application/vnd.github+json"
        if self.token:
            body = encoded(value) if value is not None else None
            request = urllib.request.Request("https://api.github.com" + path, body,
                {"Authorization": "Bearer " + self.token, "Accept": accept,
                 "X-GitHub-Api-Version": "2022-11-28", "User-Agent": "RAC2-reservations",
                 "Content-Type": "application/json"}, method=method)
            try:
                with urllib.request.urlopen(request, timeout=20) as response:
                    data = response.read(limit + 1)
            except urllib.error.HTTPError as error:
                status = error.code
                error.close()
                raise ApiError(status) from None
            except OSError:
                raise CoordinationError("github_transport_unconfirmed") from None
        else:
            command = ["gh", "api", path.lstrip("/"), "--hostname", "github.com", "--method", method,
                       "-H", "Accept: " + accept, "-H", "X-GitHub-Api-Version: 2022-11-28"]
            if value is not None:
                command += ["--input", "-"]
            try:
                result = subprocess.run(command, input=encoded(value) if value is not None else None,
                                        capture_output=True, timeout=30, check=False)
            except (OSError, subprocess.TimeoutExpired):
                raise CoordinationError("github_transport_unconfirmed") from None
            if result.returncode:
                match = re.search(rb"HTTP (\d{3})", result.stderr[-4096:])
                raise ApiError(int(match[1]) if match else 0)
            data = result.stdout
        if len(data) > limit:
            raise CoordinationError("github_response_too_large")
        return data if raw else decode(data)

    def get(self, path, **kwargs):
        return self.request("GET", self.prefix + path, **kwargs)

    def post(self, path, value):
        return self.request("POST", self.prefix + path, value)

    def put(self, path, value):
        return self.request("PUT", self.prefix + path, value)

    def user(self):
        value = self.request("GET", "/user")
        actor = value.get("login") if isinstance(value, dict) else None
        if not isinstance(actor, str) or not LOGIN.fullmatch(actor):
            raise CoordinationError("github_login_required")
        return actor

    def maintainer(self, actor):
        if not isinstance(actor, str) or not LOGIN.fullmatch(actor):
            raise CoordinationError("invalid_actor")
        try:
            value = self.get("/collaborators/" + actor + "/permission")
        except ApiError as error:
            if error.status == 404:
                return False
            raise
        return value.get("role_name") in {"maintain", "admin"}

    def file(self, path, ref, *, limit=MAX_LEDGER):
        if not OID.fullmatch(ref) and ref != LEDGER_BRANCH:
            raise CoordinationError("untrusted_file_ref")
        name = urllib.parse.quote(path, safe="/")
        query = urllib.parse.urlencode({"ref": ref})
        value = self.get("/contents/" + name + "?" + query, limit=limit * 2 + 4096)
        oid, size = value.get("sha", ""), value.get("size")
        if not OID.fullmatch(oid) or type(size) is not int or not 0 <= size <= limit:
            raise CoordinationError("invalid_repository_file")
        # JSON/base64 also works with gh on Windows, whose raw binary output can
        # pass through a text encoder. Larger Contents responses omit content;
        # resolve their exact immutable Git blob instead of another branch read.
        payload = value if value.get("encoding") == "base64" else self.get("/git/blobs/" + oid, limit=limit * 2 + 4096)
        try:
            if payload.get("encoding") != "base64" or payload.get("sha") != oid:
                raise CoordinationError("invalid_repository_encoding")
            raw = base64.b64decode("".join(payload["content"].split()), validate=True)
        except (KeyError, ValueError, TypeError):
            raise CoordinationError("invalid_repository_encoding") from None
        algorithm = "sha1" if len(oid) == 40 else "sha256"
        actual = hashlib.new(algorithm, b"blob " + str(len(raw)).encode() + b"\0" + raw).hexdigest()
        if len(raw) != size or actual != oid:
            raise CoordinationError("repository_blob_changed")
        return raw


class Catalogue:
    """Only current trusted upstream metadata; downloaded code is never executed."""
    def __init__(self, api):
        self.api = api
        repo = api.request("GET", api.prefix)
        branch = repo.get("default_branch")
        if branch != "RAC2":
            raise CoordinationError("unexpected_default_branch")
        ref = api.get("/git/ref/heads/" + branch)
        self.commit = ref.get("object", {}).get("sha", "")
        if not OID.fullmatch(self.commit):
            raise CoordinationError("invalid_default_commit")
        self.data = decode(api.file("config/function-catalog/catalog.json", self.commit))
        if self.data.get("target") != "SCUS_972.68" or self.data.get("schema") != 1:
            raise CoordinationError("invalid_catalogue")
        self.chunks = {}
        self.credit = {}

    def accepted(self, program, reference_sha256):
        if program not in self.credit:
            path = ("progress/integration.json" if program == "boot" else
                    "progress/levels/" + program.removeprefix("levels/") + ".json")
            pins = [p for p in self.data.get("input_pins", []) if p.get("path") == path]
            if len(pins) != 1:
                raise CoordinationError("integration_pin_missing")
            raw = self.api.file(path, self.commit, limit=8 * 1024 * 1024)
            if hashlib.sha256(raw).hexdigest() != pins[0].get("sha256"):
                raise CoordinationError("integration_proof_changed")
            proof = decode(raw)
            if (proof.get("reference_sha256") != reference_sha256
                    or proof.get("program") != program.removeprefix("levels/")
                    or proof.get("state") != "integrated"):
                raise CoordinationError("invalid_integration_proof")
            rows = proof.get("functions")
            if type(rows) is not list:
                raise CoordinationError("invalid_integration_functions")
            credited = {}
            for row in rows:
                if (type(row) is not dict or row.get("matched") is not True or row.get("integrated") is not True
                        or row.get("different_bytes") != 0 or row.get("state") != "integrated"
                        or type(row.get("address")) is not int or type(row.get("size")) is not int
                        or not isinstance(row.get("reference_sha256"), str)
                        or not HEX.fullmatch(row["reference_sha256"])
                        or row.get("candidate_sha256") != row["reference_sha256"]):
                    raise CoordinationError("invalid_integrated_function")
                key = (row["address"], row["size"])
                if key in credited:
                    raise CoordinationError("duplicate_integrated_function")
                credited[key] = row["reference_sha256"]
            self.credit[program] = credited
        return self.credit[program]

    def rows(self, program):
        if program in self.chunks:
            return self.chunks[program]
        matches = [entry for entry in self.data.get("function_chunks", []) if entry.get("program") == program]
        if len(matches) != 1:
            raise CoordinationError("unknown_program")
        entry = matches[0]
        name = entry.get("path", "")
        maximum = entry.get("max_expanded_bytes")
        if (not re.fullmatch(r"[A-Za-z0-9_-]+\.ndjson\.gz", name)
                or type(maximum) is not int or not 1 <= maximum <= 32 * 1024 * 1024):
            raise CoordinationError("invalid_catalogue_chunk")
        raw = self.api.file("config/function-catalog/" + name, self.commit, limit=4 * 1024 * 1024)
        if hashlib.sha256(raw).hexdigest() != entry.get("sha256"):
            raise CoordinationError("catalogue_chunk_changed")
        # Bound decompression before parsing; never extract an archive or execute it.
        import io
        try:
            with gzip.GzipFile(fileobj=io.BytesIO(raw)) as stream:
                expanded = stream.read(maximum + 1)
        except (OSError, EOFError):
            raise CoordinationError("invalid_catalogue_compression") from None
        if len(expanded) > maximum or hashlib.sha256(expanded).hexdigest() != entry.get("expanded_sha256"):
            raise CoordinationError("catalogue_expanded_changed")
        rows = {}
        for line in expanded.splitlines():
            row = decode(line)
            if type(row) is not list or len(row) < 7 or type(row[0]) is not int or type(row[1]) is not int:
                raise CoordinationError("invalid_catalogue_row")
            rows.setdefault((row[0], row[1]), []).append(row)
        self.chunks[program] = rows
        return rows

    def target(self, query, *, allow_integrated=False):
        if type(query) is not dict or set(query) - {"target", "program", "reference_sha256", "address", "size", "symbol", "family_id"}:
            raise CoordinationError("invalid_target_query")
        if query.get("target", "SCUS_972.68") != "SCUS_972.68":
            raise CoordinationError("wrong_release")
        pins = [p for p in self.data.get("programs", []) if p.get("reference_sha256") == query.get("reference_sha256")]
        if len(pins) != 1:
            raise CoordinationError("unknown_reference")
        program = pins[0]["program"]
        if query.get("program", program) != program:
            raise CoordinationError("wrong_program")
        address, size = query.get("address"), query.get("size")
        if type(address) is not int or type(size) is not int:
            raise CoordinationError("invalid_target_extent")
        rows = self.rows(program).get((address, size), [])
        if len(rows) != 1:
            raise CoordinationError("unknown_complete_extent")
        row = rows[0]
        if row[3] not in {"qualified_complete", "flow_supported_inferred"}:
            raise CoordinationError("unsupported_boundary")
        credit = self.accepted(program, pins[0]["reference_sha256"])
        overlaps = [(start, length) for start, length in credit
                    if start < address + size and address < start + length]
        accepted = credit.get((address, size)) == row[2]
        if overlaps and not allow_integrated:
            raise CoordinationError("already_integrated")
        symbols = sorted(s for s in row[6] if isinstance(s, str) and SYMBOL.fullmatch(s))
        fallback = "FUN_" + format(address, "08X")
        requested_symbol = query.get("symbol")
        if requested_symbol is not None and (not isinstance(requested_symbol, str)
                or not SYMBOL.fullmatch(requested_symbol) or requested_symbol not in [*symbols, fallback]):
            raise CoordinationError("invalid_symbol")
        target = {"target": "SCUS_972.68", "program": program,
                  "reference_sha256": pins[0]["reference_sha256"], "address": address,
                  "size": size, "symbol": requested_symbol or (symbols[0] if symbols else fallback)}
        if isinstance(row[4], str) and HEX.fullmatch(row[4]):
            target["family_id"] = row[4]
        if "family_id" in query and query["family_id"] != target.get("family_id"):
            raise CoordinationError("stale_family")
        return normalize_target(target), accepted


class Ledger:
    def __init__(self, api):
        self.api = api

    def read(self):
        path = "/contents/" + LEDGER_PATH + "?" + urllib.parse.urlencode({"ref": LEDGER_BRANCH})
        value = self.api.get(path)
        oid, size = value.get("sha", ""), value.get("size")
        if not OID.fullmatch(oid) or type(size) is not int or not 1 <= size <= MAX_LEDGER:
            raise CoordinationError("invalid_ledger_file")
        try:
            raw = (base64.b64decode(value["content"], validate=False) if value.get("encoding") == "base64"
                   else self.api.file(LEDGER_PATH, LEDGER_BRANCH))
        except (KeyError, ValueError):
            raise CoordinationError("invalid_ledger_encoding") from None
        algorithm = "sha1" if len(oid) == 40 else "sha256"
        actual = hashlib.new(algorithm, b"blob " + str(len(raw)).encode() + b"\0" + raw).hexdigest()
        if actual != oid or len(raw) != size:
            raise CoordinationError("ledger_read_raced")
        state = decode(raw)
        validate_state(state)
        return state, oid

    def apply(self, request, *, actor, issue, command_id, now, maintainer=False, integration_verified=False):
        for attempt in range(8):
            try:
                state, oid = self.read()
            except CoordinationError as error:
                if str(error) != "ledger_read_raced" or attempt == 7:
                    raise
                continue
            updated, receipt = apply_command(state, request, actor=actor, issue=issue,
                command_id=command_id, now=now, maintainer=maintainer, integration_verified=integration_verified)
            if updated == state:
                return state, receipt
            raw = encoded(updated)
            if len(raw) > MAX_LEDGER:
                raise CoordinationError("ledger_capacity_requires_maintenance")
            try:
                self.api.put("/contents/" + LEDGER_PATH,
                    {"message": "coordination: record reservation command " + str(command_id),
                     "branch": LEDGER_BRANCH, "sha": oid, "content": base64.b64encode(raw).decode()})
                return updated, receipt
            except ApiError as error:
                if error.status not in {409, 422}:
                    raise
            except CoordinationError as error:
                if str(error) != "github_transport_unconfirmed":
                    raise
                # A write may have committed. Re-read and let immutable command
                # IDs prove its outcome; never resubmit a different operation.
            if attempt == 7:
                raise CoordinationError("ledger_write_unconfirmed")
        raise CoordinationError("ledger_write_unconfirmed")


def parse_command(body):
    if not isinstance(body, str) or not body.startswith("/reserve "):
        return None
    if len(body.encode("utf-8")) > MAX_COMMAND:
        raise CoordinationError("command_too_large")
    value = decode(body[len("/reserve "):])
    if type(value) is not dict:
        raise CoordinationError("invalid_command")
    return value


def integration_verified(api, catalogue, request, claim):
    number = request.get("pr")
    if type(number) is not int or number <= 0 or not claim:
        return False
    value = api.get("/pulls/" + str(number))
    if (value.get("merged") is not True or value.get("base", {}).get("ref") != "RAC2"
            or value.get("base", {}).get("repo", {}).get("full_name") != UPSTREAM):
        return False
    for target in claim["targets"]:
        normalized, accepted = catalogue.target(target, allow_integrated=True)
        if not accepted or any(normalized.get(k) != target.get(k) for k in
                               ("target", "program", "reference_sha256", "address", "size")):
            return False
    return True


def handle_event(api, event, *, now=None, catalogue_factory=Catalogue):
    """Authenticated GitHub event data, never model-owned actor or privileges."""
    if (event.get("action") != "created" or event.get("repository", {}).get("full_name") != UPSTREAM
            or "pull_request" in event.get("issue", {}) or event.get("comment", {}).get("user", {}).get("type") == "Bot"):
        return {"ok": True, "state": "ignored"}
    comment = event.get("comment", {})
    request = parse_command(comment.get("body"))
    if request is None:
        return {"ok": True, "state": "ignored"}
    actor = comment.get("user", {}).get("login")
    issue, command_id = event.get("issue", {}).get("number"), comment.get("id")
    if (comment.get("user", {}).get("type") != "User" or not isinstance(actor, str) or not LOGIN.fullmatch(actor)
            or event.get("sender", {}).get("login") != actor
            or type(issue) is not int or issue <= 0 or type(command_id) is not int or command_id <= 0):
        raise CoordinationError("invalid_event_identity")
    ledger = Ledger(api)
    before, _ = ledger.read()
    record = before["issues"].get(str(issue))
    replay = record is not None and command_id <= record["last_command_id"]
    claim = before["claims"].get(request.get("claim_id"))
    privileged = request.get("op") == "integrated" or (claim and claim["owner"] != actor)
    maintainer = api.maintainer(actor) if privileged and not replay else False
    verified = False
    if not replay:
        live_issue = api.get("/issues/" + str(issue))
        if "pull_request" in live_issue:
            raise CoordinationError("pull_request_is_not_reservation_issue")
        if request.get("op") == "claim" and (live_issue.get("state") != "open"
                or not str(live_issue.get("title", "")).startswith("[Reservation]")):
            raise CoordinationError("reservation_issue_required")
    if request.get("op") == "claim" and not replay:
        targets = request.get("targets")
        if type(targets) is not list or not 1 <= len(targets) <= 5:
            raise CoordinationError("invalid_targets")
        catalogue = catalogue_factory(api)
        request = {**request, "targets": [catalogue.target(target)[0] for target in targets]}
    elif request.get("op") == "integrated" and maintainer and not replay:
        verified = integration_verified(api, catalogue_factory(api), request, claim)
    state, receipt = ledger.apply(request, actor=actor, issue=issue, command_id=command_id,
        now=int(time.time()) if now is None else now, maintainer=maintainer, integration_verified=verified)
    # The committed ledger is authoritative; labels/comments are a derived view.
    view = "Reservation command " + str(command_id) + ": **" + receipt["state"] + "**."
    if receipt.get("claim_id"):
        view += " Claim: `" + receipt["claim_id"] + "`."
    if receipt.get("reason"):
        view += " Reason: `" + receipt["reason"] + "`."
    if receipt.get("conflicting_issue"):
        view += " Already reserved in #" + str(receipt["conflicting_issue"]) + "."
    view += "\n\nCoordination only; no matching or integration credit is added."
    claim = state["claims"].get(receipt.get("claim_id"))
    if claim:
        view += "\nOwner: `" + claim["owner"] + "`. Recorded revision: " + str(state["revision"]) + "."
    try:
        api.post("/issues/" + str(issue) + "/comments", {"body": view})
        current_issue = api.get("/issues/" + str(issue))
        labels = [label["name"] for label in current_issue.get("labels", [])
                  if isinstance(label, dict) and isinstance(label.get("name"), str)
                  and not label["name"].startswith("reservation:") and label["name"] != "reservation"]
        status = effective_status(claim, int(time.time()) if now is None else now) if claim else "rejected"
        labels += ["reservation", "reservation:" + status.replace("_", "-")]
        api.put("/issues/" + str(issue) + "/labels", {"labels": labels})
        if claim and receipt["ok"]:
            api.post("/issues/" + str(issue) + "/assignees", {"assignees": [claim["owner"]]})
    except CoordinationError:
        receipt = {**receipt, "issue_view_pending": True}
    return receipt


def receipt_with_claim(state, receipt):
    result = dict(receipt)
    claim = state["claims"].get(receipt.get("claim_id"))
    if claim:
        result.update({key: claim[key] for key in ("owner", "targets", "lease_until", "issue")})
    return result


def wait_ack(ledger, issue, command_id, *, timeout=180, now=time.monotonic, sleep=time.sleep):
    deadline = now() + timeout
    while now() < deadline:
        state, _ = ledger.read()
        record = state["issues"].get(str(issue))
        if record and record["last_command_id"] == command_id:
            return receipt_with_claim(state, record["last_receipt"])
        if record and record["last_command_id"] > command_id:
            raise CoordinationError("reservation_command_superseded")
        sleep(min(2, max(0, deadline - now())))
    raise CoordinationError("reservation_ack_unconfirmed")


def submit_command(api, issue, request, *, timeout=180):
    response = api.post("/issues/" + str(issue) + "/comments", {"body": "/reserve " + encoded(request).decode().strip()})
    command_id = response.get("id")
    if type(command_id) is not int or command_id <= 0:
        raise CoordinationError("reservation_post_unconfirmed")
    return wait_ack(Ledger(api), issue, command_id, timeout=timeout)


def rpc(api, request, *, timeout=180, catalogue_factory=Catalogue):
    if type(request) is not dict:
        raise CoordinationError("invalid_rpc")
    op = request.get("op")
    common = {"op", "owner", "claim_id", "targets"}
    allowed = {"describe": {"op", "targets"}, "claim": {"op", "targets", "kind", "lease_seconds", "repository", "branch"},
               "check": common, "renew": common | {"lease_seconds"}, "release": common,
               "review": common, "block": common, "integrated": common | {"pr"}}
    if op not in allowed or set(request) - allowed[op]:
        raise CoordinationError("invalid_rpc_fields")
    owner = api.user()
    if op == "describe":
        queries = request.get("targets")
        if type(queries) is not list or not 1 <= len(queries) <= 5:
            raise CoordinationError("invalid_targets")
        catalogue = catalogue_factory(api)
        return {"ok": True, "state": "described", "owner": owner,
                "targets": [catalogue.target(query)[0] for query in queries]}
    if op == "claim":
        queries = request.get("targets")
        if type(queries) is not list or not 1 <= len(queries) <= 5:
            raise CoordinationError("invalid_targets")
        catalogue = catalogue_factory(api)
        targets = [catalogue.target(query)[0] for query in queries]
        command = {key: request[key] for key in ("kind", "lease_seconds", "repository", "branch") if key in request}
        command.update(op="claim", kind=request.get("kind", "functions"), targets=targets)
        # Validate metadata without allocating any local or remote claim.
        _, admission = apply_command(empty_state(), command, actor=owner, issue=1, command_id=1, now=int(time.time()))
        if not admission["ok"]:
            return admission
        issue = api.post("/issues", {"title": "[Reservation] " + owner + ": " + str(len(targets)) + " function(s)",
            "body": "Small contributor reservation. Commands and ownership are recorded in the shared ledger.\n\n"
                    "Targets:\n```json\n" + encoded(targets).decode() + "```\n\nNo matching credit is claimed."})
        number = issue.get("number")
        if type(number) is not int or number <= 0:
            raise CoordinationError("reservation_issue_unconfirmed")
        return submit_command(api, number, command, timeout=timeout)
    state, _ = Ledger(api).read()
    claim_id = request.get("claim_id")
    claim = state["claims"].get(claim_id) if isinstance(claim_id, str) else None
    if not claim:
        raise CoordinationError("claim_not_found")
    if request.get("owner", owner) != owner:
        raise CoordinationError("authenticated_owner_differs")
    if op == "check":
        queries = request.get("targets")
        if type(queries) is not list or not 1 <= len(queries) <= 5:
            raise CoordinationError("invalid_targets")
        catalogue = catalogue_factory(api)
        current_targets = [catalogue.target(query)[0] for query in queries]
        # Resolve potentially slow current metadata first. The final authority
        # read must observe a release/block/review that happened during it.
        state, _ = Ledger(api).read()
        result = check_claim(state, claim_id, actor=owner, targets=current_targets, now=int(time.time()))
        return receipt_with_claim(state, {**result, "revision": state["revision"]})
    if op not in {"renew", "release", "review", "block", "integrated"}:
        raise CoordinationError("unknown_operation")
    if request.get("targets") is not None:
        supplied = request["targets"]
        if (type(supplied) is not list or not 1 <= len(supplied) <= 5
                or sorted(encoded(normalize_target(t)) for t in supplied)
                != sorted(encoded(normalize_target(t)) for t in claim["targets"])):
            raise CoordinationError("target_mismatch")
    command = {key: request[key] for key in ("lease_seconds", "pr") if key in request}
    command.update(op=op, claim_id=claim_id)
    return submit_command(api, claim["issue"], command, timeout=timeout)
