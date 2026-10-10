"""Authenticated identity and full-suite provenance mutation fixtures."""
import base64
import copy
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import maintainer_test_policy as policy


class Api:
    def __init__(self, values):
        self.values = values
        self.calls = []

    def get(self, path):
        self.calls.append(path)
        return copy.deepcopy(self.values[path])


class PolicyTests(unittest.TestCase):
    def setUp(self):
        self.sha = "a" * 40
        self.repo = {"id": policy.REPOSITORY_ID, "full_name": policy.REPOSITORY}
        self.user = {"id": policy.MAINTAINER_ID, "login": policy.MAINTAINER_LOGIN}
        self.pr = {"number": 10, "state": "open", "user": self.user,
                   "head": {"sha": "b" * 40, "repo": self.repo},
                   "base": {"sha": "c" * 40, "ref": "RAC2", "repo": self.repo}}
        self.event = {"repository": self.repo, "number": 10, "pull_request": copy.deepcopy(self.pr)}
        self.api = Api({"pulls/10": self.pr})

    def test_primary_maintainer_exact_head_can_use_focused_pr_tests(self):
        self.assertEqual(policy.select("pull_request", self.event, self.sha, self.api)["mode"], "targeted")

    def test_spoofed_login_id_fork_base_and_stale_heads_are_not_trusted(self):
        mutations = [("user", "id", 1), ("user", "login", "someone"),
                     ("head.repo", "id", 1), ("base.repo", "id", 1),
                     ("head.repo", "full_name", "fork/project"), ("base", "ref", "other"),
                     ("head", "sha", "d" * 40), ("base", "sha", "d" * 40)]
        for path, key, value in mutations:
            with self.subTest(path=path, key=key):
                pr = copy.deepcopy(self.pr)
                target = pr
                for component in path.split("."):
                    target = target[component]
                target[key] = value
                self.assertEqual(policy.select("pull_request", self.event, self.sha, Api({"pulls/10": pr}))["mode"], "full")

    def test_labels_body_and_event_user_cannot_override_api_identity(self):
        event = copy.deepcopy(self.event)
        event["pull_request"].update(labels=[{"name": "trusted-maintainer"}], body="primary maintainer", user=self.user)
        pr = copy.deepcopy(self.pr)
        pr["user"]["id"] = 123
        self.assertEqual(policy.select("pull_request", event, self.sha, Api({"pulls/10": pr}))["mode"], "full")

    def test_merge_group_never_queries_metadata_or_reuses_tests(self):
        self.assertEqual(policy.select("merge_group", {}, self.sha, self.api)["mode"], "full")
        self.assertEqual(self.api.calls, [])

    def test_unknown_events_and_other_push_branches_require_full_suite(self):
        for event_name in ("workflow_dispatch", "pull_request_target", "unknown"):
            self.assertEqual(policy.select(event_name, {"repository": self.repo}, self.sha, self.api)["mode"], "full")
        self.assertEqual(policy.select("push", {"repository": self.repo, "ref": "refs/heads/topic", "after": self.sha}, self.sha, self.api)["mode"], "full")
        self.assertEqual(self.api.calls, [])

    def push_fixture(self):
        workflow_bytes = b"reviewed unconditional full merge-group command"
        self.pin = hashlib.sha256(workflow_bytes).hexdigest()
        pr = copy.deepcopy(self.pr)
        pr.update(state="closed", merged_at="2026-10-09", merge_commit_sha=self.sha)
        run = {"id": 50, "event": "merge_group", "head_sha": self.sha, "workflow_id": 20,
               "path": policy.WORKFLOW, "repository": self.repo, "head_repository": self.repo,
               "status": "completed", "conclusion": "success", "run_attempt": 1,
               "head_branch": "gh-readonly-queue/RAC2/pr-10-base"}
        job = {"name": policy.FULL_JOB, "run_id": 50, "head_sha": self.sha,
               "status": "completed", "conclusion": "success",
               "steps": [{"name": policy.FULL_STEP, "status": "completed", "conclusion": "success"}]}
        self.values = {"commits/" + self.sha + "/pulls?per_page=100": [pr],
                       "actions/workflows/tests.yml": {"path": policy.WORKFLOW, "state": "active", "id": 20},
                       "contents/" + policy.WORKFLOW + "?ref=" + self.sha:
                           {"path": policy.WORKFLOW, "encoding": "base64", "content": base64.b64encode(workflow_bytes).decode()},
                       "actions/workflows/tests.yml/runs?event=merge_group&head_sha=" + self.sha + "&per_page=100":
                           {"workflow_runs": [run]},
                       "actions/runs/50/attempts/1/jobs?per_page=100": {"total_count": 1, "jobs": [job]}}
        return {"repository": self.repo, "ref": "refs/heads/RAC2", "after": self.sha}

    def test_exact_authenticated_full_queue_suite_can_be_reused(self):
        event = self.push_fixture()
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", self.pin):
            result = policy.select("push", event, self.sha, Api(self.values))
        self.assertEqual(result["mode"], "reuse")
        self.assertEqual(result["queue_run_id"], 50)

    def test_weakened_workflow_is_rejected_even_when_step_name_and_status_match(self):
        event = self.push_fixture()
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", "0" * 64):
            self.assertEqual(policy.select("push", event, self.sha, Api(self.values))["mode"], "full")

    def test_each_queue_provenance_field_is_required(self):
        event = self.push_fixture()
        run_key = next(key for key in self.values if "/runs?" in key)
        mutations = {"event": "push", "head_sha": "b" * 40, "workflow_id": 21,
                     "path": ".github/workflows/fake.yml", "repository": {"id": 1},
                     "head_repository": {"id": 1}, "status": "in_progress", "conclusion": "failure",
                     "head_branch": "gh-readonly-queue/RAC2/pr-11-base", "run_attempt": 0}
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", self.pin):
            for key, value in mutations.items():
                with self.subTest(key=key):
                    values = copy.deepcopy(self.values)
                    values[run_key]["workflow_runs"][0][key] = value
                    self.assertEqual(policy.select("push", event, self.sha, Api(values))["mode"], "full")

    def test_job_and_step_not_skipped_or_mislabeled_or_from_other_run(self):
        event = self.push_fixture()
        key = "actions/runs/50/attempts/1/jobs?per_page=100"
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", self.pin):
            for target, field, value in [("job", "run_id", 51), ("job", "head_sha", "b" * 40),
                                         ("job", "status", "queued"), ("job", "conclusion", "failure"),
                                         ("step", "conclusion", "skipped"), ("step", "name", "lookalike"),
                                         ("step", "status", "in_progress")]:
                with self.subTest(target=target, field=field):
                    values = copy.deepcopy(self.values)
                    row = values[key]["jobs"][0]
                    if target == "step":
                        row = row["steps"][0]
                    row[field] = value
                    self.assertEqual(policy.select("push", event, self.sha, Api(values))["mode"], "full")

    def test_ambiguous_or_nonmaintainer_merged_pr_does_not_reuse(self):
        event = self.push_fixture()
        key = "commits/" + self.sha + "/pulls?per_page=100"
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", self.pin):
            for mutation in ("multiple", "foreign", "different_sha"):
                values = copy.deepcopy(self.values)
                if mutation == "multiple":
                    values[key].append(copy.deepcopy(values[key][0]))
                elif mutation == "foreign":
                    values[key][0]["user"]["id"] = 12
                else:
                    values[key][0]["merge_commit_sha"] = "b" * 40
                self.assertEqual(policy.select("push", event, self.sha, Api(values))["mode"], "full")

    def test_api_error_falls_back_without_logging_exception_credentials(self):
        with tempfile.TemporaryDirectory() as directory:
            event = Path(directory) / "event.json"
            output = Path(directory) / "output"
            event.write_text(json.dumps(self.event))
            with patch.dict("os.environ", {"GITHUB_EVENT_NAME": "pull_request", "GITHUB_SHA": self.sha}), \
                 patch.object(policy.GitHub, "get", side_effect=ValueError("secret-token")), \
                 patch("builtins.print") as printed:
                self.assertEqual(policy.main(["--event", str(event), "--output", str(output)]), 0)
            self.assertEqual(output.read_text(), "mode=full\n")
            self.assertNotIn("secret-token", str(printed.call_args))

    def test_current_workflow_pin_and_unconditional_queue_command(self):
        data = (Path(__file__).resolve().parents[1] / policy.WORKFLOW).read_bytes()
        self.assertEqual(hashlib.sha256(data).hexdigest(), policy.QUALIFIED_WORKFLOW_SHA256)
        text = data.decode()
        self.assertIn("ref: ${{ github.event.pull_request.base.sha || github.sha }}", text)
        self.assertIn("if: github.event_name == 'merge_group' || needs.policy.outputs.mode == 'full'", text)
        self.assertIn("run: python -m unittest discover -s tests -v", text)
        self.assertIn("if: always()", text)
        self.assertIn('test "$POLICY_RESULT" = success', text)

    def retry_fixture(self, responses):
        event = self.push_fixture()
        key = "commits/" + self.sha + "/pulls?per_page=100"
        valid = copy.deepcopy(self.values[key])
        api = Api(self.values)
        base_get = api.get
        count = [0]
        def get(path):
            if path == key:
                value = responses[min(count[0], len(responses) - 1)]
                count[0] += 1
                if isinstance(value, Exception): raise value
                if value == "valid": return valid
                return copy.deepcopy(value)
            return base_get(path)
        api.get = get
        waits, clock = [], [0]
        def wait(seconds):
            waits.append(seconds)
            clock[0] += seconds
        return event, api, waits, wait, lambda: clock[0], count

    def test_transient_empty_then_trusted_pending_then_exact_queue_success(self):
        event = self.push_fixture()
        pending = copy.deepcopy(self.values["commits/" + self.sha + "/pulls?per_page=100"])
        pending[0]["merged_at"] = None
        event, api, waits, wait, clock, count = self.retry_fixture([[], pending, "valid"])
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", self.pin):
            result = policy.select_with_retry("push", event, self.sha, api, wait, clock)
        self.assertEqual(result["mode"], "reuse")
        self.assertEqual(result["commit_sha"], self.sha)
        self.assertEqual(waits, [2, 4])
        self.assertEqual(count[0], 3)

    def test_pending_exhausted_is_full_with_finite_wait_budget(self):
        event, api, waits, wait, clock, count = self.retry_fixture([[]])
        result = policy.select_with_retry("push", event, self.sha, api, wait, clock)
        self.assertEqual(result["mode"], "full")
        self.assertIn("bounded retry window", result["reason"])
        self.assertLessEqual(sum(waits), 30)
        self.assertEqual(count[0], 6)

    def test_wrong_owner_fork_sha_and_ambiguity_never_wait(self):
        event = self.push_fixture()
        row = self.values["commits/" + self.sha + "/pulls?per_page=100"][0]
        for kind in ("owner", "fork", "sha", "ambiguous"):
            with self.subTest(kind=kind):
                altered = copy.deepcopy(row)
                altered["merged_at"] = None
                if kind == "owner": altered["user"]["id"] = 1
                elif kind == "fork": altered["head"]["repo"]["id"] = 1
                elif kind == "sha": altered["merge_commit_sha"] = "d" * 40
                responses = [altered, copy.deepcopy(altered)] if kind == "ambiguous" else [altered]
                event, api, waits, wait, clock, count = self.retry_fixture([responses])
                result = policy.select_with_retry("push", event, self.sha, api, wait, clock)
                self.assertEqual(result["mode"], "full")
                self.assertEqual(waits, [])
                self.assertEqual(count[0], 1)

    def test_api_error_unknown_event_and_other_proof_failures_never_retry(self):
        event, api, waits, wait, clock, count = self.retry_fixture([PermissionError("secret-token")])
        with self.assertRaises(PermissionError):
            policy.select_with_retry("push", event, self.sha, api, wait, clock)
        self.assertEqual(waits, [])
        self.assertEqual(policy.select_with_retry("merge_group", {}, self.sha, api, wait, clock)["mode"], "full")
        event, api, waits, wait, clock, count = self.retry_fixture(["valid"])
        with patch.object(policy, "QUALIFIED_WORKFLOW_SHA256", "0" * 64):
            self.assertEqual(policy.select_with_retry("push", event, self.sha, api, wait, clock)["mode"], "full")
        self.assertEqual(waits, [])


if __name__ == "__main__":
    unittest.main()
