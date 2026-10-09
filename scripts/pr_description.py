#!/usr/bin/env python3
"""Validate PR descriptions as data; never execute contributor text or code."""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import sys
import urllib.error
import urllib.request

CONTEXT = "PR description"
HEADINGS = ("Summary", "Scope", "Validation", "Matching evidence", "Risks and follow-up", "Checklist")
TYPES = {"matching", "placement", "nonmatching", "tooling", "documentation"}
CHECKS = (
    "Description reflects this head and the validated combined source.",
    "Only authored source and public proof metadata are included; no game bytes, proprietary tools, secrets or private runtime data.",
    "Provenance and reused work are identified; incomplete or skipped validation is disclosed.",
)


def meaningful(value: str) -> bool:
    value = value.strip().strip("`").strip()
    return bool(value) and not re.search(r"\b(TODO|TBD|REPLACE ME)\b", value, re.I) and value.lower() not in {"n/a", "none", "...", "-"} and not re.fullmatch(r"n/a\s*:\s*", value, re.I)


def visible_text(body: str) -> str:
    # Unclosed comments/fences hide the remainder too. Closing fences may be
    # longer than their opening fence, as in GitHub-flavoured Markdown.
    body = re.sub(r"<!--.*?(?:-->|\Z)", "", body or "", flags=re.S)
    result = []
    fence = None
    for line in body.splitlines():
        match = re.match(r"^\s*(`{3,}|~{3,})(.*)$", line)
        if fence:
            if match and match[1][0] == fence[0] and len(match[1]) >= len(fence) and not match[2].strip():
                fence = None
        elif match:
            fence = match[1]
        else:
            result.append(line)
    return "\n".join(result)


def validate(body: str, files: list[str] | None = None) -> list[str]:
    """Structural checks only: statements still require technical review/proofs."""
    body = visible_text(body)
    sections: dict[str, str] = {}
    errors: list[str] = []
    matches = list(re.finditer(r"^## ([^\r\n]+)\s*$", body, re.M))
    for i, match in enumerate(matches):
        name = match.group(1).strip()
        if name in sections:
            errors.append(f"Duplicate section: {name}")
        sections[name] = body[match.end():matches[i + 1].start() if i + 1 < len(matches) else len(body)].strip()
    for name in HEADINGS:
        if name not in sections:
            errors.append(f"Missing section: ## {name}")
    for name in ("Summary", "Risks and follow-up"):
        value = sections.get(name, "")
        if not meaningful(value) and not (name == "Risks and follow-up" and value.lower() == "none"):
            errors.append(f"Fill ## {name} with the actual change or limitations.")

    def field(section: str, name: str) -> str:
        values = re.findall(r"^- " + re.escape(name) + r":[ \t]*([^\r\n]*)$", sections.get(section, ""), re.M)
        if len(values) != 1 or not meaningful(values[0]):
            errors.append(f"Fill exactly one '- {name}:' in ## {section} (N/A needs a reason).")
        return values[0].strip() if values else ""

    kinds = {v.strip() for v in field("Scope", "Type").split(",")}
    if not kinds or kinds - TYPES:
        errors.append("Type must contain only: " + ", ".join(sorted(TYPES)))
    base = field("Scope", "Validated base").strip("`")
    if not re.fullmatch(r"[0-9a-fA-F]{40}", base):
        errors.append("Validated base must be the full upstream commit SHA actually used.")
    reservation = field("Scope", "Reservation")
    field("Scope", "Targets")
    decomp = bool(kinds & {"matching", "placement"})
    if decomp and reservation.lower().startswith("n/a"):
        errors.append("Matching/placement work needs an acknowledged reservation or an explained legacy claim.")
    if files and any(p.startswith(("src/", "candidates/")) and p.endswith((".c", ".h", ".cfrag")) for p in files):
        if not decomp:
            errors.append("Changes to maintained authored/generated C require matching or placement Type; retained nonmatching attempts belong on the shelf.")
    if files and any(p in {"config/candidate-catalog.json", "config/level-catalog.json", "progress/candidates.json", "progress/integration.json"} or p.startswith(("config/level-native/", "config/level-g8/", "config/boot-units/", "config/function-catalog/", "progress/levels/", "progress/level-candidates/", "progress/level-g8/", "progress/boot-units/")) for p in files) and not decomp:
        errors.append("Changes to matching catalogue/proof inputs require matching or placement Type (zero delta is valid).")

    pairs = re.findall(r"^- (Command|Result):[ \t]*([^\r\n]*)$", sections.get("Validation", ""), re.M)
    if not pairs or len(pairs) % 2 or any(name != ("Command" if i % 2 == 0 else "Result") or not meaningful(value) for i, (name, value) in enumerate(pairs)):
        errors.append("Validation needs completed alternating '- Command:' / '- Result:' pairs; disclose skips/failures.")
    for name in ("Proofs", "Physical delta", "Unique delta", "Gates", "ABI review"):
        value = field("Matching evidence", name)
        if decomp and value.lower().startswith("n/a"):
            errors.append(f"Matching/placement work must supply {name}.")
        if name in {"Physical delta", "Unique delta"}:
            if decomp and not re.match(r"[+-]?\d+\b", value):
                errors.append(f"{name} must begin with a signed integer byte delta (zero is valid).")
            if "nonmatching" in kinds and not decomp and not re.match(r"[+]?0\b", value):
                errors.append(f"Nonmatching work must report zero {name.lower()}.")
    checklist = sections.get("Checklist", "")
    for statement in CHECKS:
        if not re.search(r"^- \[[xX]\] " + re.escape(statement) + r"\s*$", checklist, re.M):
            errors.append(f"Confirm checklist: {statement}")
    return errors


def api(method: str, endpoint: str, payload: dict | None = None):
    token = os.environ["GITHUB_TOKEN"]
    request = urllib.request.Request(
        "https://api.github.com/" + endpoint,
        data=json.dumps(payload).encode() if payload is not None else None,
        method=method,
        headers={"Authorization": "Bearer " + token, "Accept": "application/vnd.github+json", "X-GitHub-Api-Version": "2022-11-28", "Content-Type": "application/json"},
    )
    with urllib.request.urlopen(request, timeout=30) as response:
        return json.load(response)


MAX_REST_PAGES = 30


def sha_value(value) -> str:
    if not isinstance(value, str) or not re.fullmatch(r"[0-9a-f]{40}", value):
        raise ValueError("Invalid commit identity")
    return value


def bounded_list(endpoint: str) -> list[dict]:
    """Never accept a truncated, malformed or unbounded REST file/PR list."""
    rows = []
    separator = "&" if "?" in endpoint else "?"
    for page in range(1, MAX_REST_PAGES + 1):
        batch = api("GET", endpoint + f"{separator}per_page=100&page={page}")
        if not isinstance(batch, list) or len(batch) > 100 or any(not isinstance(row, dict) for row in batch):
            raise ValueError("Malformed API list")
        rows.extend(batch)
        if len(batch) < 100:
            return rows
    raise ValueError("Pagination limit reached; refusing incomplete evidence")


def queue_member(repo: str, group: dict, stage_hook=None) -> dict:
    """Bind a single-PR merge group through live refs and exact commit parents.

    No queue-number/ref-message parsing or GraphQL permission dependency. The
    applicable-rules endpoint returns only active rules; exactly one reviewed
    MERGE/ALLGREEN one-entry policy is required. A synthetic commit containing
    an earlier queued commit fails because its first parent is not current RAC2.
    """
    stage = stage_hook or (lambda name: None)
    stage("policy")
    rules = bounded_list(f"repos/{repo}/rules/branches/RAC2")
    if len(rules) > 1000 or any(not isinstance(rule.get("type"), str) or not rule["type"] for rule in rules):
        raise ValueError("Malformed applicable rules")
    queues = [rule for rule in rules if rule.get("type") == "merge_queue"]
    if len(queues) != 1:
        raise ValueError("An active unambiguous merge queue rule is required")
    rule = queues[0]
    params = rule["parameters"]
    if (type(rule["ruleset_id"]) is not int or rule["ruleset_id"] < 1
            or rule["ruleset_source_type"] != "Repository" or rule["ruleset_source"] != repo
            or params["grouping_strategy"] != "ALLGREEN" or params["merge_method"] != "MERGE"
            or any(type(params[key]) is not int or params[key] != 1 for key in (
                "max_entries_to_build", "max_entries_to_merge", "min_entries_to_merge"))):
        raise ValueError("Unsupported queue policy")
    stage("refs")
    for ref, expected in (("refs/heads/RAC2", group["base_sha"]), (group["head_ref"], group["head_sha"])):
        actual = api("GET", f"repos/{repo}/git/ref/{ref.removeprefix('refs/')}")
        if (actual["ref"] != ref or actual["object"]["type"] != "commit"
                or sha_value(actual["object"]["sha"]) != expected):
            raise ValueError("Live queue or protected ref changed")
    stage("ancestry")
    commit = api("GET", f"repos/{repo}/git/commits/{group['head_sha']}")
    parents = commit["parents"]
    if (sha_value(commit["sha"]) != group["head_sha"] or not isinstance(parents, list) or len(parents) != 2
            or any(not isinstance(parent, dict) for parent in parents)):
        raise ValueError("Synthetic commit must have exactly two parents")
    base, source = (sha_value(parent["sha"]) for parent in parents)
    if base != group["base_sha"] or source in {base, group["head_sha"]}:
        raise ValueError("Synthetic ancestry does not bind the current base and source")
    stage("association")
    associated = bounded_list(f"repos/{repo}/commits/{source}/pulls")
    matches = []
    numbers = set()
    for pr in associated:
        number = pr["number"]
        if type(number) is not int or number < 1 or number in numbers:
            raise ValueError("Malformed or duplicate commit association")
        numbers.add(number)
        head = sha_value(pr["head"]["sha"])
        pr_base = sha_value(pr["base"]["sha"])
        if (pr["state"] == "open" and head == source and pr["base"]["ref"] == "RAC2"
                and pr["base"]["repo"]["full_name"] == repo):
            if pr["draft"] is not False:
                raise ValueError("Queued source is draft")
            matches.append({"number": number, "headRefOid": head, "baseRefOid": pr_base,
                            "baseRefName": "RAC2", "repository": {"nameWithOwner": repo}})
    if len(matches) != 1:
        raise ValueError("No unique open RAC2 PR matches the source parent")
    # Cached PR base metadata can be older than current RAC2; retain it separately.
    return {"head": group["head_sha"], "base": base, "source": source,
            "policy": {"ruleset_id": rule["ruleset_id"], "parameters": params}, "pr": matches[0]}


def queued_pr_snapshot(pr: dict, repo: str, member: dict) -> tuple:
    expected = member["pr"]
    if (type(pr["number"]) is not int or pr["number"] != expected["number"]
            or pr["state"] != "open" or pr["draft"] is not False
            or pr["base"]["ref"] != "RAC2" or pr["base"]["repo"]["full_name"] != repo
            or sha_value(pr["base"]["sha"]) != expected["baseRefOid"]
            or sha_value(pr["head"]["sha"]) != expected["headRefOid"]
            or type(pr["changed_files"]) is not int or not 0 <= pr["changed_files"] <= 3000
            or (pr.get("body") is not None and not isinstance(pr["body"], str))
            or not isinstance(pr["updated_at"], str) or not pr["updated_at"]):
        raise ValueError("Queued PR no longer matches its entry")
    return (pr["number"], pr["state"], pr["draft"], pr["head"]["sha"], pr["base"]["sha"],
            pr["base"]["ref"], pr["base"]["repo"]["full_name"], pr.get("body"),
            pr["updated_at"], pr["changed_files"])


def queued_head_duplicates(repo: str, number: int, sha: str) -> bool:
    for row in bounded_list(f"repos/{repo}/pulls?state=open&base=RAC2"):
        if type(row["number"]) is not int or row["number"] < 1:
            raise ValueError("Malformed open PR identity")
        if sha_value(row["head"]["sha"]) == sha and row["number"] != number:
            return True
    return False


def handle_merge_group(event: dict) -> int:
    """Read-only admission; Actions supplies the synthetic named job check.

    Log only fixed stages/classifications, never API errors or contributor prose.
    All freshness checks are repeated; later asynchronous edits still require
    GitHub's PR-head checks and rerunning/requeuing when appropriate.
    """
    phase = "event"

    def stage(name):
        nonlocal phase
        phase = name

    try:
        repo = os.environ["GITHUB_REPOSITORY"]
        original = event["merge_group"]
        if not isinstance(original, dict):
            raise ValueError("Merge group must be an object")
        group = dict(original)
        if isinstance(group.get("head_ref"), str) and group["head_ref"].startswith("gh-readonly-queue/"):
            group["head_ref"] = "refs/heads/" + group["head_ref"]
        if group.get("base_ref") == "RAC2":
            group["base_ref"] = "refs/heads/RAC2"
        if (not re.fullmatch(r"[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+", repo)
                or event.get("action") != "checks_requested"
                or event["repository"]["full_name"] != repo
                or group["base_ref"] != "refs/heads/RAC2"
                or not isinstance(group["head_ref"], str)
                or not re.fullmatch(r"refs/heads/gh-readonly-queue/RAC2/[A-Za-z0-9_-][A-Za-z0-9_./-]*", group["head_ref"])
                or ".." in group["head_ref"] or "//" in group["head_ref"]
                or group["head_ref"].endswith(("/", "."))):
            raise ValueError("Unsupported queue event")
        sha_value(group["head_sha"])
        sha_value(group["base_sha"])
        if group["head_sha"] == group["base_sha"]:
            raise ValueError("Synthetic head cannot equal protected base")
        if "GITHUB_SHA" in os.environ and sha_value(os.environ["GITHUB_SHA"]) != group["head_sha"]:
            raise ValueError("Actions SHA does not match merge group")
        member = queue_member(repo, group, stage)
        number = member["pr"]["number"]
        endpoint = f"repos/{repo}/pulls/{number}"
        stage("pr_snapshot")
        pr = api("GET", endpoint)
        snapshot = queued_pr_snapshot(pr, repo, member)
        stage("files")
        files = bounded_list(endpoint + "/files")
        names = [row["filename"] for row in files]
        if (any(not isinstance(name, str) or not name for name in names)
                or len(names) != pr["changed_files"] or len(set(names)) != len(names)):
            raise ValueError("Incomplete or changing queued PR file list")
        errors = validate(pr.get("body") or "", names)
        stage("duplicate_head")
        duplicate = queued_head_duplicates(repo, number, pr["head"]["sha"])
        if queue_member(repo, group, stage) != member:
            raise ValueError("Group ancestry, policy or PR association changed")
        stage("pr_snapshot_final")
        if queued_pr_snapshot(api("GET", endpoint), repo, member) != snapshot:
            raise ValueError("Queued PR changed during validation")
        stage("duplicate_head_final")
        duplicate = duplicate or queued_head_duplicates(repo, number, pr["head"]["sha"])
        if errors or duplicate:
            print("Description: queued PR description is incomplete or its head is ambiguous.")
            return 1
        print("Description: live single-PR merge group passed the description contract; claims still need review.")
        return 0
    except Exception as error:
        if isinstance(error, urllib.error.HTTPError):
            kind = {401: "api_unauthorized", 403: "api_forbidden", 404: "api_missing",
                    429: "api_rate_limited"}.get(error.code, "api_http_error")
        elif isinstance(error, urllib.error.URLError):
            kind = "api_transport_error"
        elif isinstance(error, (ValueError, KeyError, TypeError)):
            kind = "invalid_or_changed_evidence"
        elif isinstance(error, TimeoutError):
            kind = "api_timeout"
        else:
            kind = "api_error"
        print(f"Description: queue check failed at {phase} ({kind}); rerun required.")
        return 1


def handle_event(event: dict) -> int:
    """Fetch current PR data; publish status to its exact head, using trusted code."""
    if "merge_group" in event or os.environ.get("GITHUB_EVENT_NAME") == "merge_group":
        return handle_merge_group(event)
    repo = os.environ["GITHUB_REPOSITORY"]
    if not re.fullmatch(r"[A-Za-z0-9_.-]+/[A-Za-z0-9_.-]+", repo):
        raise ValueError("Invalid repository")
    number = int(event.get("pull_request", {}).get("number") or event.get("inputs", {}).get("pr_number", 0))
    if number < 1:
        raise ValueError("A positive PR number is required")
    endpoint = f"repos/{repo}/pulls/{number}"
    pr = api("GET", endpoint)
    if pr["state"] != "open" or pr["base"]["ref"] != "RAC2" or pr["base"]["repo"]["full_name"] != repo:
        raise ValueError("Only open PRs targeting this repository's RAC2 branch are supported")
    sha = pr["head"]["sha"]
    status_endpoint = f"repos/{repo}/statuses/{sha}"
    url = f"https://github.com/{repo}/actions/runs/{os.environ['GITHUB_RUN_ID']}"
    api("POST", status_endpoint, {"state": "pending", "context": CONTEXT, "description": "Validating current PR description", "target_url": url})
    try:
        # Commit statuses are shared by all PRs with the same head SHA. Reject
        # ambiguity rather than let one complete body approve another PR's body.
        duplicates = []
        page = 1
        while True:
            batch = api("GET", f"repos/{repo}/pulls?state=open&base=RAC2&per_page=100&page={page}")
            duplicates.extend(item["number"] for item in batch if item["number"] != number and item["head"]["sha"] == sha)
            if len(batch) < 100:
                break
            page += 1
        files = []
        page = 1
        while True:
            batch = api("GET", endpoint + f"/files?per_page=100&page={page}")
            files.extend(item["filename"] for item in batch)
            if len(batch) < 100:
                break
            page += 1
        if len(files) != pr["changed_files"]:
            raise ValueError("Incomplete or changing file list; rerun the description check")
        errors = validate(pr.get("body") or "", files)
        if duplicates:
            errors.append("Multiple open RAC2 PRs share this head SHA: " + ", ".join(f"#{n}" for n in duplicates) + ". Keep one PR per head, then rerun.")
        latest = api("GET", endpoint)
        if (latest["head"]["sha"], latest.get("body"), latest["base"]["sha"], latest["state"], latest.get("updated_at")) != (sha, pr.get("body"), pr["base"]["sha"], "open", pr.get("updated_at")):
            # A newer synchronize/edited event will check the new snapshot. Leave
            # the old status pending rather than publishing stale success.
            print("PR changed during validation; awaiting its newer event.")
            return 1
        state = "failure" if errors else "success"
        api("POST", status_endpoint, {"state": state, "context": CONTEXT, "description": f"{len(errors)} description issue(s); see run log" if errors else "Required description fields are complete (claims still need review)", "target_url": url})
        # Fixed prefix prevents user-controlled text becoming workflow commands.
        for error in errors:
            print("Description: " + error)
        if not errors:
            print("Description contract passed; this is not byte-match or full-image proof.")
        return int(bool(errors))
    except Exception:
        api("POST", status_endpoint, {"state": "error", "context": CONTEXT, "description": "Description validation failed to run; rerun required", "target_url": url})
        raise


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--body-file", type=Path)
    source.add_argument("--event", type=Path)
    args = parser.parse_args()
    if args.event:
        return handle_event(json.loads(args.event.read_text(encoding="utf-8-sig")))
    errors = validate(args.body_file.read_text(encoding="utf-8-sig"))
    for error in errors:
        print(error)
    if not errors:
        print("Description contract passed (technical claims are not verified).")
    return int(bool(errors))


if __name__ == "__main__":
    sys.exit(main())
