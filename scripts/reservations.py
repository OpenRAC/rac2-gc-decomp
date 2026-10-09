#!/usr/bin/env python3
"""Reserve small contributor lots through the shared upstream GitHub ledger."""
from __future__ import annotations

import argparse
import base64
import json
import os
from pathlib import Path
import sys
import time

from reservation_core import empty_state, effective_status
from reservation_github import (ApiError, CoordinationError, GitHub, Ledger,
    LEDGER_BRANCH, LEDGER_PATH, MAX_COMMAND, UPSTREAM, decode, encoded,
    handle_event, receipt_with_claim, rpc)


def initialise(api):
    """Explicit maintainer bootstrap; isolated parentless coordination branch."""
    actor = api.user()
    if not api.maintainer(actor):
        raise CoordinationError("maintainer_required")
    for state in ("reserved", "in-review", "blocked", "released", "integrated", "needs-attention", "rejected"):
        try:
            api.post("/labels", {"name": "reservation:" + state, "color": "1d76db",
                                 "description": "Operational coordination only: " + state})
        except ApiError as error:
            if error.status != 422:
                raise
    try:
        api.post("/labels", {"name": "reservation", "color": "5319e7",
                             "description": "Shared contributor function reservation"})
    except ApiError as error:
        if error.status != 422:
            raise
    try:
        api.get("/git/ref/heads/" + LEDGER_BRANCH)
    except ApiError as error:
        if error.status != 404:
            raise
    else:
        state, _ = Ledger(api).read()
        return {"ok": True, "state": "already_initialized", "revision": state["revision"]}
    blob = api.post("/git/blobs", {"encoding": "base64", "content": base64.b64encode(encoded(empty_state())).decode()})
    tree = api.post("/git/trees", {"tree": [{"path": LEDGER_PATH, "mode": "100644", "type": "blob", "sha": blob["sha"]}]})
    commit = api.post("/git/commits", {"message": "coordination: initialize shared reservation ledger",
                                     "tree": tree["sha"], "parents": []})
    try:
        api.post("/git/refs", {"ref": "refs/heads/" + LEDGER_BRANCH, "sha": commit["sha"]})
    except ApiError as error:
        if error.status != 422:
            raise
        # Another maintainer may have bootstrapped it. Validate, never replace.
    state, _ = Ledger(api).read()
    return {"ok": True, "state": "initialized", "revision": state["revision"]}


def targets_file(path):
    data = Path(path).read_bytes()
    if len(data) > MAX_COMMAND:
        raise CoordinationError("targets_file_too_large")
    value = decode(data)
    if type(value) is not list or not 1 <= len(value) <= 5:
        raise CoordinationError("invalid_targets")
    return value


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--timeout", type=int, default=120, help="Maximum acknowledgement wait, 10-240 seconds")
    parser.add_argument("--json", action="store_true", help="All commands already return bounded JSON")
    sub = parser.add_subparsers(dest="action", required=True)
    sub.add_parser("init", help="Maintainer: initialize the isolated operational branch")
    sub.add_parser("list", help="List the public reservations without changing them")
    sub.add_parser("rpc", help="Read one bounded JSON request from stdin; owner automation only")
    event = sub.add_parser("handle-event", help="Trusted GitHub issue_comment workflow only")
    event.add_argument("--event", type=Path, required=True)
    describe = sub.add_parser("describe", help="Resolve a complete pinned target from upstream metadata")
    describe.add_argument("--reference-sha256", required=True)
    describe.add_argument("--address", type=lambda v: int(v, 0), required=True)
    describe.add_argument("--size", type=lambda v: int(v, 0), required=True)
    claim = sub.add_parser("claim", help="Create an issue and reserve one small lot")
    claim.add_argument("--targets", type=Path, required=True)
    claim.add_argument("--kind", choices=("functions", "family"), default="functions")
    claim.add_argument("--repository", required=True, help="Your fork, owner/repository")
    claim.add_argument("--branch", required=True)
    claim.add_argument("--lease-seconds", type=int, default=172800)
    for name in ("check", "renew", "release", "review", "block", "integrated"):
        command = sub.add_parser(name)
        command.add_argument("--claim", dest="claim_id", required=True)
        command.add_argument("--owner")
        command.add_argument("--targets", type=Path, required=name == "check")
        if name == "renew":
            command.add_argument("--lease-seconds", type=int, default=172800)
        if name == "integrated":
            command.add_argument("--pr", type=int, required=True)
    args = parser.parse_args(argv)
    if not 10 <= args.timeout <= 240:
        parser.error("timeout must be between 10 and 240 seconds")
    try:
        if args.action == "handle-event":
            if (os.environ.get("GITHUB_REPOSITORY") != UPSTREAM
                    or os.environ.get("GITHUB_EVENT_NAME") != "issue_comment"
                    or not os.environ.get("GITHUB_TOKEN")):
                raise CoordinationError("trusted_workflow_required")
            raw = args.event.read_bytes()
            if len(raw) > 2 * 1024 * 1024:
                raise CoordinationError("event_too_large")
            result = handle_event(GitHub(os.environ["GITHUB_TOKEN"]), decode(raw))
        else:
            api = GitHub()
            if args.action == "init":
                result = initialise(api)
            elif args.action == "list":
                state, _ = Ledger(api).read()
                result = {"ok": True, "revision": state["revision"], "claims": [
                    {**claim, "effective_status": effective_status(claim, int(time.time()))}
                    for claim in state["claims"].values()]}
            elif args.action == "rpc":
                raw = sys.stdin.buffer.read(MAX_COMMAND + 1)
                if len(raw) > MAX_COMMAND:
                    raise CoordinationError("rpc_too_large")
                result = rpc(api, decode(raw), timeout=args.timeout)
            else:
                request = {"op": args.action}
                for name in ("owner", "claim_id", "kind", "repository", "branch", "lease_seconds", "pr"):
                    if getattr(args, name, None) is not None:
                        request[name] = getattr(args, name)
                if args.action == "describe":
                    request["targets"] = [{"reference_sha256": args.reference_sha256,
                                           "address": args.address, "size": args.size}]
                elif getattr(args, "targets", None):
                    request["targets"] = targets_file(args.targets)
                result = rpc(api, request, timeout=args.timeout)
        print(json.dumps(result, ensure_ascii=True, sort_keys=True))
        if args.action == "handle-event" and result.get("state") in {
                "reserved", "in_review", "blocked", "released", "integrated", "rejected", "ignored"}:
            # A confirmed refusal was processed correctly, not a failed model
            # or compiler run. RPC callers still receive nonzero on refusal.
            return 0
        return 0 if result.get("ok") is True else 2
    except (CoordinationError, ValueError, OSError, KeyError, TypeError, RecursionError) as error:
        reason = str(error) if isinstance(error, (CoordinationError, ValueError)) else "coordination_unavailable"
        allowed = reason and len(reason) <= 100 and all(c.isascii() and (c.isalnum() or c == "_") for c in reason)
        print(json.dumps({"ok": False, "state": "unavailable", "reason": reason if allowed else "coordination_unavailable"}))
        return 2


if __name__ == "__main__":
    raise SystemExit(main())
