"""Select the primary maintainer's CI route from authenticated GitHub metadata.

An absent, ambiguous or stale proof always selects the complete suite. The
merge-group workflow forces that suite independently of this script's output.
"""
import argparse
import base64
import hashlib
import json
import os
from pathlib import Path
import urllib.request

REPOSITORY = "OpenRAC/rac2-gc-decomp"
REPOSITORY_ID = 1400228215
MAINTAINER_ID = 191315338
MAINTAINER_LOGIN = "llesieur99"
WORKFLOW = ".github/workflows/tests.yml"
# Updated only after reviewing the literal, unconditional merge-group suite.
QUALIFIED_WORKFLOW_SHA256 = "5e180fe1ee75bef514d74cfe6b30fc7523c03366e30f40e3ec070e75ff503583"
FULL_STEP = "Run complete tool suite"


def identity(user):
    return (isinstance(user, dict) and user.get("id") == MAINTAINER_ID
            and user.get("login") == MAINTAINER_LOGIN)


def same_repository(value):
    return (isinstance(value, dict) and value.get("id") == REPOSITORY_ID
            and value.get("full_name") == REPOSITORY)


def trusted_pr(pr):
    return (identity(pr.get("user"))
            and same_repository(pr.get("head", {}).get("repo"))
            and same_repository(pr.get("base", {}).get("repo"))
            and pr.get("base", {}).get("ref") == "RAC2")


class GitHub:
    def __init__(self, token):
        self.token = token

    def get(self, path):
        request = urllib.request.Request("https://api.github.com/repos/" + REPOSITORY + "/" + path,
            headers={"Accept": "application/vnd.github+json", "Authorization": "Bearer " + self.token,
                     "X-GitHub-Api-Version": "2022-11-28"})
        with urllib.request.urlopen(request, timeout=20) as stream:
            return json.load(stream)


def full(reason):
    return {"mode": "full", "reason": reason}


def select(event_name, event, sha, api):
    """Only exact-SHA queue evidence can replace a post-merge suite."""
    if event_name == "merge_group":
        return full("merge groups always run the complete suite")
    if not same_repository(event.get("repository")):
        return full("repository identity is not qualified")
    if event_name == "pull_request":
        supplied = event.get("pull_request", {})
        pr = api.get("pulls/" + str(event["number"]))
        if (trusted_pr(pr) and pr.get("state") == "open" and pr.get("number") == event["number"]
                and supplied.get("head", {}).get("sha") == pr.get("head", {}).get("sha")
                and supplied.get("base", {}).get("sha") == pr.get("base", {}).get("sha")):
            return {"mode": "targeted", "reason": "authenticated primary maintainer PR",
                    "base_sha": pr["base"]["sha"], "head_sha": pr["head"]["sha"]}
        return full("PR identity, repository or exact reviewed head is not qualified")
    if event_name != "push" or event.get("ref") != "refs/heads/RAC2" or event.get("after") != sha:
        return full("only an exact protected-branch push can reuse queue tests")
    prs = api.get("commits/" + sha + "/pulls?per_page=100")
    merged = [pr for pr in prs if pr.get("merged_at") and pr.get("merge_commit_sha") == sha]
    if len(prs) >= 100 or len(merged) != 1 or not trusted_pr(merged[0]):
        return full("exact merged PR is absent, ambiguous or not the primary maintainer")
    workflow = api.get("actions/workflows/tests.yml")
    if workflow.get("path") != WORKFLOW or workflow.get("state") != "active":
        return full("workflow identity is not qualified")
    content = api.get("contents/" + WORKFLOW + "?ref=" + sha)
    if content.get("encoding") != "base64" or content.get("path") != WORKFLOW:
        return full("workflow bytes are unavailable")
    digest = hashlib.sha256(base64.b64decode(content["content"], validate=False)).hexdigest()
    if digest != QUALIFIED_WORKFLOW_SHA256:
        return full("workflow bytes differ from the reviewed full-suite workflow")
    runs = api.get("actions/workflows/tests.yml/runs?event=merge_group&head_sha=" + sha + "&per_page=100")
    for run in runs.get("workflow_runs", []):
        if (run.get("event") != "merge_group" or run.get("head_sha") != sha
                or run.get("workflow_id") != workflow.get("id") or run.get("path") != WORKFLOW
                or not same_repository(run.get("repository"))
                or not same_repository(run.get("head_repository"))
                or run.get("status") != "completed" or run.get("conclusion") != "success"
                or not run.get("head_branch", "").startswith(f"gh-readonly-queue/RAC2/pr-{merged[0]['number']}-")):
            continue
        attempt = run.get("run_attempt")
        if not isinstance(attempt, int) or attempt < 1:
            continue
        jobs = api.get(f"actions/runs/{run['id']}/attempts/{attempt}/jobs?per_page=100")
        matches = [job for job in jobs.get("jobs", []) if job.get("name") == "tests"]
        if jobs.get("total_count", 101) > 100 or len(matches) != 1:
            continue
        job = matches[0]
        steps = [step for step in job.get("steps", []) if step.get("name") == FULL_STEP]
        if (job.get("run_id") != run["id"] or job.get("head_sha") != sha
                or job.get("status") != "completed" or job.get("conclusion") != "success"
                or len(steps) != 1 or steps[0].get("status") != "completed"
                or steps[0].get("conclusion") != "success"):
            continue
        return {"mode": "reuse", "reason": "exact-SHA full merge-group suite verified",
                "queue_run_id": run["id"], "queue_run_attempt": attempt,
                "workflow_sha256": digest, "commit_sha": sha}
    return full("no successful exact-SHA full merge-group suite was verified")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--event", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args(argv)
    try:
        event = json.loads(args.event.read_bytes())
        result = select(os.environ.get("GITHUB_EVENT_NAME"), event, os.environ.get("GITHUB_SHA"),
                        GitHub(os.environ.get("GITHUB_TOKEN", "")))
    except Exception:
        # No API exception text: request errors must never disclose credentials.
        result = full("metadata could not be verified; complete suite required")
    with args.output.open("a", encoding="utf8") as stream:
        for key in ("mode", "base_sha"):
            if key in result:
                stream.write(key + "=" + result[key] + "\n")
    print(json.dumps(result, sort_keys=True))
    summary = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary:
        with open(summary, "a", encoding="utf8") as stream:
            stream.write("Tool test route: **" + result["mode"] + "** â€” " + result["reason"] + ".\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
