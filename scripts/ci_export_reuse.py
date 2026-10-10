"""Read-only, exact-run full export provenance and safe archive restoration.

All failures select a fresh full pipeline. No receipt is relabelled current;
the original queue receipt is retained byte-for-byte in the restored bundle.
"""
import argparse
import hashlib
import io
import json
import os
from pathlib import Path
import re
import stat
import urllib.error
import urllib.parse
import urllib.request
import zipfile

import ci_export as export
import maintainer_test_policy as policy

PRODUCER = "validation"
EXPORT_STEP = "Generate and validate complete exports"
REQUIRED = ("tests", "SCUS_972.68 Progress")
MAX_COMPRESSED = 128 * 1024 * 1024
MAX_FILE = 256 * 1024 * 1024
MAX_EXPANDED = 512 * 1024 * 1024
HASH = re.compile(r"[0-9a-f]{64}\Z")
SHA = re.compile(r"[0-9a-f]{40}\Z")


def need(ok):
    if not ok:
        raise ValueError("unverified-export-provenance")


def positive(value):
    return type(value) is int and value > 0


def successful(value):
    return value.get("status") == "completed" and value.get("conclusion") == "success"


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, request, fp, code, message, headers, newurl):
        return None


class Api(policy.GitHub):
    def archive(self, ident):
        need(positive(ident))
        request = urllib.request.Request("https://api.github.com/repos/" + policy.REPOSITORY
                    + f"/actions/artifacts/{ident}/zip",
                    headers={"Authorization": "Bearer " + self.token, "Accept": "application/vnd.github+json",
                             "X-GitHub-Api-Version": "2022-11-28"})
        try:
            urllib.request.build_opener(NoRedirect()).open(request, timeout=20)
            raise ValueError("missing-artifact-redirect")
        except urllib.error.HTTPError as response:
            need(response.code == 302)
            location = response.headers.get("Location", "")
        target = urllib.parse.urlsplit(location)
        need(target.scheme == "https" and bool(target.hostname) and target.username is None and target.password is None)
        # A trusted API signed download URL is followed WITHOUT API token/cookies.
        with urllib.request.build_opener(NoRedirect()).open(urllib.request.Request(location), timeout=30) as stream:
            data = stream.read(MAX_COMPRESSED + 1)
        need(len(data) <= MAX_COMPRESSED)
        return data


def unpack(data, digest):
    need(type(data) is bytes and len(data) <= MAX_COMPRESSED and type(digest) is str
         and digest.startswith("sha256:") and HASH.fullmatch(digest[7:])
         and export.sha(data) == digest[7:])
    result, total = {}, 0
    allowed = set(export.FILES) | {"receipt.json"}
    with zipfile.ZipFile(io.BytesIO(data)) as archive:
        members = archive.infolist()
        need(len(members) == len(allowed))
        for entry in members:
            name = entry.filename
            mode = entry.external_attr >> 16
            need(name in allowed and name not in result and not entry.is_dir()
                 and not stat.S_ISLNK(mode) and not (entry.flag_bits & 1)
                 and 0 <= entry.file_size <= MAX_FILE)
            total += entry.file_size
            need(total <= MAX_EXPANDED)
            body = archive.read(entry)
            need(len(body) == entry.file_size)
            result[name] = body
    need(set(result) == allowed)
    return result


def provenance(api, route, sha):
    need(route.get("mode") == "reuse" and SHA.fullmatch(sha or "")
         and route.get("commit_sha") == sha)
    ident, attempt = route.get("queue_run_id"), route.get("queue_run_attempt")
    need(positive(ident) and positive(attempt))
    run = api.get(f"actions/runs/{ident}")
    workflow = api.get("actions/workflows/tests.yml")
    need(positive(workflow.get("id")) and workflow.get("path") == policy.WORKFLOW and workflow.get("state") == "active")
    need(run.get("id") == ident and type(run.get("id")) is int and run.get("run_attempt") == attempt
         and type(run.get("run_attempt")) is int and run.get("event") == "merge_group"
         and run.get("head_sha") == sha and type(run.get("workflow_id")) is int and run.get("workflow_id") == workflow["id"]
         and run.get("path") == policy.WORKFLOW and policy.same_repository(run.get("repository"))
         and policy.same_repository(run.get("head_repository")) and successful(run))
    jobs = api.get(f"actions/runs/{ident}/attempts/{attempt}/jobs?per_page=100")
    need(type(jobs.get("total_count")) is int and jobs["total_count"] == len(jobs.get("jobs", []))
         and jobs["total_count"] <= 100)
    selected = {}
    for name in (PRODUCER, *REQUIRED):
        matches = [j for j in jobs["jobs"] if j.get("name") == name]
        need(len(matches) == 1)
        job = matches[0]
        need(type(job.get("run_id")) is int and job.get("run_id") == ident and job.get("head_sha") == sha and successful(job))
        selected[name] = job
    for name in (policy.FULL_STEP, EXPORT_STEP):
        steps = [s for s in selected[PRODUCER].get("steps", []) if s.get("name") == name]
        need(len(steps) == 1 and successful(steps[0]))
    listing = api.get(f"actions/runs/{ident}/artifacts?per_page=100")
    need(type(listing.get("total_count")) is int and listing["total_count"] == len(listing.get("artifacts", []))
         and listing["total_count"] <= 100)
    name = f"SCUS_972.68_verified-export-{attempt}"
    matches = [a for a in listing["artifacts"] if a.get("name") == name]
    need(len(matches) == 1 and positive(matches[0].get("id")))
    artifact = api.get("actions/artifacts/" + str(matches[0]["id"]))
    need(artifact == matches[0] and artifact.get("expired") is False)
    origin = artifact.get("workflow_run", {})
    need(type(origin.get("id")) is int and origin.get("id") == ident and origin.get("head_sha") == sha
         and origin.get("repository_id") == policy.REPOSITORY_ID and origin.get("head_repository_id") == policy.REPOSITORY_ID)
    return {"run": run, "workflow": workflow, "jobs": selected, "artifact": artifact}


def manifest(files, route, current, workflow_pin, exporter_pin):
    need(route.get("workflow_sha256") == workflow_pin)
    receipt = json.loads(files["receipt.json"])
    need(receipt.get("schema") == 1 and type(receipt.get("schema")) is int
         and receipt.get("kind") == "verified-ci-full-export" and receipt.get("mode") == "full"
         and receipt.get("event") == "merge_group" and receipt.get("commit_sha") == route["commit_sha"]
         and receipt.get("run_id") == route["queue_run_id"] and type(receipt.get("run_id")) is int
         and receipt.get("run_attempt") == route["queue_run_attempt"] and type(receipt.get("run_attempt")) is int
         and receipt.get("repository") == policy.REPOSITORY and receipt.get("repository_id") == policy.REPOSITORY_ID
         and receipt.get("workflow_path") == policy.WORKFLOW and receipt.get("workflow_sha256") == workflow_pin
         and receipt.get("exporter_sha256") == exporter_pin and receipt.get("inputs") == current
         and current.get(policy.WORKFLOW) == workflow_pin and current.get("scripts/ci_export.py") == exporter_pin
         and receipt.get("checks") == export.CHECKS and set(receipt.get("outputs", {})) == set(export.FILES))
    for name, expected in receipt["outputs"].items():
        need(type(expected) is str and HASH.fullmatch(expected) and export.sha(files[name]) == expected)
    return receipt


def verified_bundle(api, route, current, workflow_pin, exporter_pin):
    before = provenance(api, route, route["commit_sha"])
    artifact = before["artifact"]
    files = unpack(api.archive(artifact["id"]), artifact.get("digest"))
    receipt = manifest(files, route, current, workflow_pin, exporter_pin)
    need(provenance(api, route, route["commit_sha"]) == before)
    return files, receipt, before


def restore(repo, files, receipt, objdiff):
    need(export.sha(Path(objdiff).read_bytes()) == "c8290281e82114bcc1a06ff73061110d3902a177822e750337de2537188e358f")
    need(export.inputs(repo) == receipt["inputs"])
    need(export.tools(objdiff) == receipt.get("tools"))
    # Stage all files first. Never extract arbitrary ZIP paths onto the checkout.
    stage = repo / "build/decomp/verified-restore"
    stage.mkdir(parents=True, exist_ok=False)
    for name, body in files.items():
        (stage / name).write_bytes(body)
    from decomp_report import measures
    empty = json.dumps({"version": 2, "measures": measures(0, 0, 0), "units": [], "categories": []}).encode()
    (stage / "empty.json").write_bytes(empty)
    export.command(repo, str(objdiff), "report", "changes", str(stage / "report.json"), str(stage / "report.json"), "-o", str(stage / "check-physical.json"))
    export.command(repo, str(objdiff), "report", "changes", str(stage / "empty.json"), str(stage / "unique-report.json"), "-o", str(stage / "check-unique.json"))
    need(export.sha((stage / "check-physical.json").read_bytes()) == receipt["outputs"]["objdiff-validation.json"]
         and export.sha((stage / "check-unique.json").read_bytes()) == receipt["outputs"]["unique-objdiff-validation.json"])
    need(export.inputs(repo) == receipt["inputs"] and export.tools(objdiff) == receipt["tools"])
    for name, relative in export.FILES.items():
        target = export.contained(repo, relative)
        target.parent.mkdir(parents=True, exist_ok=True)
        # Already-tracked metadata is equal by the manifest input guard.
        if relative in receipt["inputs"]:
            need(export.sha(files[name]) == receipt["inputs"][relative])
        target.write_bytes(files[name])
    bundle = repo / "build/decomp/verified-export"
    bundle.mkdir(exist_ok=False)
    for name, body in files.items():
        (bundle / name).write_bytes(body)
    need(export.inputs(repo) == receipt["inputs"])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--event", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--objdiff", type=Path, required=True)
    args = parser.parse_args()
    reused = False
    stage = "identity"
    try:
        api = Api(os.environ.get("GITHUB_TOKEN", ""))
        event = json.loads(args.event.read_bytes())
        route = policy.select(os.environ.get("GITHUB_EVENT_NAME"), event, os.environ.get("GITHUB_SHA"), api)
        stage = "provenance-archive"
        current = export.inputs(export.ROOT)
        files, receipt, snapshot = verified_bundle(api, route, current, policy.QUALIFIED_WORKFLOW_SHA256,
                                                   policy.QUALIFIED_EXPORTER_SHA256)
        stage = "restore-tools-cli"
        restore(export.ROOT, files, receipt, args.objdiff)
        # Live metadata and identity must remain unchanged through restoration.
        stage = "final-live-metadata"
        final = policy.select(os.environ.get("GITHUB_EVENT_NAME"), event, os.environ.get("GITHUB_SHA"), api)
        need(final == route)
        need(provenance(api, route, route["commit_sha"]) == snapshot)
        reused = True
    except Exception as error:
        category = "api" if isinstance(error, (urllib.error.URLError, PermissionError)) else "validation"
        print("CI export reuse: fresh full validation required (" + stage + "/" + category + ")", flush=True)
    with args.output.open("a", encoding="utf8") as stream:
        stream.write("reused=" + ("true" if reused else "false") + "\n")
    print("CI export reuse: " + ("verified" if reused else "not verified"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
