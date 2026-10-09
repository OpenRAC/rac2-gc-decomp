"""Description admission and privileged metadata handler safety regressions."""
import copy
import importlib.util
import os
import io
from pathlib import Path
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location("pr_description", ROOT / "scripts/pr_description.py")
contract = importlib.util.module_from_spec(spec)
spec.loader.exec_module(contract)


def body(kind="tooling, documentation"):
    return """## Summary
Reject incomplete descriptions so reviewers can locate evidence.
## Scope
- Type: TYPE
- Validated base: b847d8c8a87a2398f42498d4209988545b92a5b6
- Reservation: N/A: no function work
- Targets: scripts/pr_description.py
## Validation
- Command: python -m unittest discover -s tests -p test_pr_description.py -v
- Result: tests pass; game build not run because no game inputs changed
## Matching evidence
- Proofs: N/A: no game proof inputs changed
- Physical delta: N/A: no matching credit
- Unique delta: N/A: no matching credit
- Gates: N/A: no game inputs changed
- ABI review: N/A: no game source changed
## Risks and follow-up
Structural completeness does not prove byte equality.
## Checklist
""".replace("TYPE", kind) + "\n".join("- [x] " + statement for statement in contract.CHECKS)


def matching_body():
    return (body("matching")
            .replace("N/A: no function work", "#42 acknowledged claim; boot setter")
            .replace("scripts/pr_description.py", "FUN_001163A0, boot, src/boot/01-resident-accessors.cfrag")
            .replace("N/A: no game proof inputs changed", "progress/candidates.json, complete 16-byte symbol, pinned source/checker hashes")
            .replace("N/A: no matching credit", "+16 (before -> after)")
            .replace("N/A: no game inputs changed", "boot and 27 overlays matched; complete symbol has zero differences")
            .replace("N/A: no game source changed", "direct callers/globals checked; field role remains unknown"))


class DescriptionTests(unittest.TestCase):
    def test_documentation_does_not_need_game_build(self):
        self.assertEqual(contract.validate(body("documentation")), [])

    def test_matching_complete(self):
        self.assertEqual(contract.validate(matching_body(), ["src/boot/accessor.cfrag"]), [])

    def test_matching_cannot_use_non_applicable_evidence(self):
        errors = contract.validate(body("matching"))
        self.assertTrue(any("must supply Gates" in e for e in errors))
        self.assertTrue(any("reservation" in e for e in errors))

    def test_blank_template_fails(self):
        template = (ROOT / ".github/pull_request_template.md").read_text(encoding="utf-8-sig")
        self.assertTrue(contract.validate(template))

    def test_missing_and_duplicate_sections(self):
        self.assertTrue(any("Missing section" in e for e in contract.validate(body().replace("## Scope", "## Other"))))
        self.assertTrue(any("Duplicate section" in e for e in contract.validate(body() + "\n## Summary\nExtra")))

    def test_comments_and_examples_do_not_satisfy_contract(self):
        self.assertTrue(contract.validate("<!--" + body() + "-->"))
        self.assertTrue(contract.validate("```markdown\n" + body() + "\n```"))
        self.assertTrue(contract.validate("~~~markdown\n" + body() + "\n~~~"))
        self.assertTrue(contract.validate("```markdown\n" + body() + "\n````"))
        self.assertTrue(contract.validate("```markdown\n" + body()))
        self.assertTrue(contract.validate("<!--" + body()))

    def test_empty_or_placeholder_fields_and_base(self):
        for replacement in ("", "TODO", "TBD", "N/A", "N/A:", "..."):
            with self.subTest(replacement=replacement):
                self.assertTrue(contract.validate(body().replace("scripts/pr_description.py", replacement)))
        self.assertTrue(contract.validate(body().replace("b847d8c8a87a2398f42498d4209988545b92a5b6", "RAC2")))

    def test_duplicate_fields_fail(self):
        self.assertTrue(contract.validate(body().replace("- Type: tooling, documentation", "- Type: tooling\n- Type: documentation")))

    def test_command_result_pairing(self):
        self.assertTrue(contract.validate(body().replace("- Result:", "- Command:")))
        self.assertTrue(contract.validate(body().replace("- Command: python -m unittest discover -s tests -p test_pr_description.py -v", "- Command:")))
        self.assertEqual(contract.validate(body().replace("## Matching evidence", "- Command: python scripts/readme_progress.py --check\n- Result: passed\n## Matching evidence")), [])

    def test_unchecked_attestation_fails(self):
        self.assertTrue(contract.validate(body().replace("- [x]", "- [ ]", 1)))

    def test_changed_c_cannot_hide_as_documentation(self):
        for filename in ("src/boot/test.cfrag", "candidates/boot.c", "src/types.h"):
            self.assertTrue(contract.validate(body("documentation"), [filename]))

    def test_nonmatching_gets_zero_credit(self):
        good = body("nonmatching").replace("N/A: no matching credit", "0 (retained attempt only)")
        self.assertEqual(contract.validate(good, ["nonmatching/boot/test.cfrag"]), [])
        self.assertTrue(contract.validate(good.replace("0 (retained attempt only)", "+16")))
        self.assertTrue(contract.validate(good, ["src/boot/test.cfrag"]))

    def test_matching_catalogue_cannot_hide_as_tooling(self):
        self.assertTrue(contract.validate(body(), ["config/candidate-catalog.json"]))
        self.assertTrue(contract.validate(body(), ["config/level-catalog.json"]))
        self.assertTrue(contract.validate(body(), ["progress/levels/oozla.json"]))

    def test_text_is_not_executed(self):
        text = body().replace("python -m unittest discover -s tests -p test_pr_description.py -v", "$(touch /tmp/do-not-create); `echo text`; ${{ secrets.EXAMPLE }}")
        self.assertEqual(contract.validate(text), [])

    def test_privileged_workflow_serializes_all_status_writers(self):
        workflow = (ROOT / ".github/workflows/pr-description.yml").read_text()
        self.assertIn("group: pr-description-RAC2", workflow)
        self.assertIn("cancel-in-progress: false", workflow)
        # A PR may retain a stale base SHA with no validator. The privileged
        # job must use the protected target repo/branch, independent of it.
        self.assertIn("repository: ${{ github.repository }}", workflow)
        self.assertIn("ref: refs/heads/RAC2", workflow)
        self.assertNotIn("github.event.pull_request.base.sha", workflow)
        self.assertNotIn("github.sha", workflow)
        self.assertIn("persist-credentials: false", workflow)
        self.assertNotIn("github.event.pull_request.head.sha", workflow)


class EventTests(unittest.TestCase):
    def setUp(self):
        self.pr = {"number": 42, "state": "open", "body": body(), "changed_files": 1, "updated_at": "2026-10-09T01:00:00Z", "head": {"sha": "a" * 40}, "base": {"ref": "RAC2", "sha": "b" * 40, "repo": {"full_name": "OpenRAC/rac2-gc-decomp"}}}
        self.posts = []
        self.calls = []
        self.other = []
        self.latest = None
        self.fail_files = False
        self.env = patch.dict(os.environ, {"GITHUB_REPOSITORY": "OpenRAC/rac2-gc-decomp", "GITHUB_RUN_ID": "1234"})
        self.env.start()
        self.addCleanup(self.env.stop)

    def fake_api(self, method, endpoint, payload=None):
        self.calls.append((method, endpoint))
        if method == "POST":
            self.posts.append((endpoint, payload))
            return {}
        if "/files?" in endpoint:
            if self.fail_files:
                raise RuntimeError("file API unavailable")
            return [{"filename": "scripts/pr_description.py"}]
        if "?state=open" in endpoint:
            return [self.pr] + self.other
        count = sum(method == "GET" and path.endswith("/pulls/42") for method, path in self.calls)
        return copy.deepcopy(self.latest if self.latest is not None and count > 1 else self.pr)

    def run_event(self, dispatch=False):
        with patch.object(contract, "api", side_effect=self.fake_api):
            return contract.handle_event({"inputs": {"pr_number": "42"}} if dispatch else {"pull_request": {"number": 42, "body": "stale ignored"}})

    def test_fetches_current_body_and_posts_exact_head(self):
        self.assertEqual(self.run_event(), 0)
        self.assertEqual([p[1]["state"] for p in self.posts], ["pending", "success"])
        self.assertTrue(all(endpoint.endswith("a" * 40) for endpoint, _ in self.posts))

    def test_invalid_body_blocks(self):
        self.pr["body"] = "please merge"
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.posts[-1][1]["state"], "failure")

    def test_edited_snapshot_never_publishes_stale_success(self):
        for field in ("body", "head", "base", "updated_at", "state"):
            with self.subTest(field=field):
                self.posts.clear()
                self.calls.clear()
                self.latest = copy.deepcopy(self.pr)
                replacements = {"body": "edited", "head": {"sha": "c" * 40}, "base": {"sha": "d" * 40}, "updated_at": "later", "state": "closed"}
                self.latest[field] = replacements[field]
                self.assertEqual(self.run_event(), 1)
                self.assertEqual([p[1]["state"] for p in self.posts], ["pending"])

    def test_duplicate_head_cannot_borrow_success(self):
        self.other = [{"number": 43, "head": self.pr["head"]}]
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.posts[-1][1]["state"], "failure")

    def test_api_error_fails_closed(self):
        self.fail_files = True
        with self.assertRaises(RuntimeError):
            self.run_event()
        self.assertEqual(self.posts[-1][1]["state"], "error")

    def test_incomplete_file_list_fails_closed(self):
        self.pr["changed_files"] = 2
        with self.assertRaises(ValueError):
            self.run_event()
        self.assertEqual(self.posts[-1][1]["state"], "error")

    def test_dispatch_reads_current_pr(self):
        self.assertEqual(self.run_event(dispatch=True), 0)

    def test_wrong_target_and_closed_rejected(self):
        self.pr["base"]["ref"] = "other"
        with self.assertRaises(ValueError):
            self.run_event()
        self.assertFalse(self.posts)

    def test_invalid_number_rejected_without_api(self):
        with patch.object(contract, "api") as mocked:
            with self.assertRaises(ValueError):
                contract.handle_event({"inputs": {"pr_number": "../../secrets"}})
            mocked.assert_not_called()


class MergeGroupTests(unittest.TestCase):
    def setUp(self):
        self.repo = "OpenRAC/rac2-gc-decomp"
        self.event = {"action": "checks_requested", "repository": {"full_name": self.repo},
                      "merge_group": {"head_sha": "c" * 40, "base_sha": "b" * 40,
                                      "base_ref": "refs/heads/RAC2",
                                      "head_ref": "refs/heads/gh-readonly-queue/RAC2/pr-42-example"}}
        self.pr = {"number": 42, "state": "open", "draft": False, "body": body(),
                   "changed_files": 1, "updated_at": "2026-10-09T01:00:00Z",
                   "head": {"sha": "a" * 40},
                   "base": {"ref": "RAC2", "sha": "b" * 40, "repo": {"full_name": self.repo}}}
        self.entry = {"id": "entry42", "position": 1, "headCommit": {"oid": "c" * 40},
                      "baseCommit": {"oid": "b" * 40},
                      "pullRequest": {"number": 42, "state": "OPEN", "headRefOid": "a" * 40,
                                      "baseRefOid": "b" * 40, "baseRefName": "RAC2",
                                      "repository": {"nameWithOwner": self.repo}}}
        self.queue = {"data": {"repository": {"nameWithOwner": self.repo,
                       "base": {"target": {"oid": "b" * 40}}, "group": {"target": {"oid": "c" * 40}},
                       "mergeQueue": {"id": "queueRAC2", "configuration": {
                           "maximumEntriesToBuild": 1, "maximumEntriesToMerge": 1,
                           "minimumEntriesToMerge": 1, "mergingStrategy": "ALLGREEN"},
                           "entries": {"totalCount": 1, "pageInfo": {"hasNextPage": False, "endCursor": None},
                                       "nodes": [self.entry]}}}}}
        self.files = [{"filename": "scripts/pr_description.py"}]
        self.other = []
        self.latest_pr = None
        self.latest_queue = None
        self.latest_other = None
        self.failure_endpoint = None
        self.calls = []
        self.queue_reads = self.pr_reads = self.open_reads = 0
        self.env = patch.dict(os.environ, {"GITHUB_REPOSITORY": self.repo, "GITHUB_EVENT_NAME": "merge_group"})
        self.env.start()
        self.addCleanup(self.env.stop)

    def fake_api(self, method, endpoint, payload=None):
        self.calls.append((method, endpoint, copy.deepcopy(payload)))
        if endpoint == self.failure_endpoint:
            raise RuntimeError("API outage containing ::warning:: contributor data")
        if method == "POST":
            self.assertEqual(endpoint, "graphql", "Queue handler attempted a mutation")
            self.assertEqual(payload["variables"]["headRef"], self.event["merge_group"]["head_ref"])
            self.assertNotIn("mutation", payload["query"])
            self.queue_reads += 1
            return copy.deepcopy(self.latest_queue if self.latest_queue is not None and self.queue_reads > 1 else self.queue)
        self.assertEqual(method, "GET")
        if "/files?" in endpoint:
            return copy.deepcopy(self.files)
        if "?state=open" in endpoint:
            self.open_reads += 1
            others = self.latest_other if self.latest_other is not None and self.open_reads > 1 else self.other
            return copy.deepcopy([self.pr] + others)
        self.assertEqual(endpoint, f"repos/{self.repo}/pulls/42")
        self.pr_reads += 1
        return copy.deepcopy(self.latest_pr if self.latest_pr is not None and self.pr_reads > 1 else self.pr)

    def run_event(self):
        with patch.object(contract, "api", side_effect=self.fake_api), patch("sys.stdout", new_callable=io.StringIO) as output:
            result = contract.handle_event(self.event)
        self.assertTrue(all(method == "GET" or (method == "POST" and endpoint == "graphql")
                            for method, endpoint, _ in self.calls))
        self.assertNotIn("::warning::", output.getvalue())
        return result

    def test_valid_group_reads_exact_refs_membership_and_snapshots_without_status_write(self):
        # Contradictory prose and PR-looking ref text never select membership.
        self.event["merge_group"]["head_commit"] = {"message": "merge PR #999"}
        self.event["pull_request"] = {"number": 999}
        self.event["merge_group"]["head_ref"] = "refs/heads/gh-readonly-queue/RAC2/pr-999-untrusted"
        self.assertEqual(self.run_event(), 0)
        self.assertEqual((self.queue_reads, self.pr_reads, self.open_reads), (2, 2, 2))
        self.assertTrue(all("999" not in endpoint for _, endpoint, _ in self.calls))

    def test_invalid_description_and_changed_file_type_fail(self):
        self.pr["body"] = "please merge"
        self.assertEqual(self.run_event(), 1)
        self.pr["body"] = body()
        self.files = [{"filename": "src/boot/new.cfrag"}]
        self.assertEqual(self.run_event(), 1)

    def test_behind_pr_metadata_base_can_differ_from_current_queue_base(self):
        self.event["merge_group"]["base_sha"] = "d" * 40
        self.queue["data"]["repository"]["base"]["target"]["oid"] = "d" * 40
        self.entry["baseCommit"]["oid"] = "d" * 40
        # Both PR APIs still report the old b... base. This is the queue's
        # purpose: validate the combined tree on d..., without manual rebase.
        self.assertEqual(self.run_event(), 0)
        self.latest_pr = copy.deepcopy(self.pr)
        self.latest_pr["base"]["sha"] = "d" * 40
        self.pr_reads = self.queue_reads = self.open_reads = 0
        self.assertEqual(self.run_event(), 1)

    def test_missing_or_malformed_queue_fails_closed(self):
        for malformed in (None, {}, {"configuration": None}):
            with self.subTest(queue=malformed):
                self.queue["data"]["repository"]["mergeQueue"] = malformed
                self.assertEqual(self.run_event(), 1)

    def test_missing_and_duplicate_synthetic_membership_fail(self):
        self.entry["headCommit"]["oid"] = "d" * 40
        self.assertEqual(self.run_event(), 1)
        self.entry["headCommit"]["oid"] = "c" * 40
        duplicate = copy.deepcopy(self.entry)
        duplicate.update(id="entry43", position=2)
        q = self.queue["data"]["repository"]["mergeQueue"]["entries"]
        q.update(totalCount=2, nodes=[self.entry, duplicate])
        self.assertEqual(self.run_event(), 1)

    def test_unknown_policy_and_preceding_member_are_not_silently_supported(self):
        self.entry["position"] = 2
        self.assertEqual(self.run_event(), 1)
        self.entry["position"] = 1
        config = self.queue["data"]["repository"]["mergeQueue"]["configuration"]
        config["mergingStrategy"] = "HEADGREEN"
        self.assertEqual(self.run_event(), 1)
        config["mergingStrategy"] = "ALLGREEN"
        config["maximumEntriesToBuild"] = 2
        self.assertEqual(self.run_event(), 1)
        config["maximumEntriesToBuild"] = True
        self.assertEqual(self.run_event(), 1)

    def test_graphql_errors_with_partial_data_fail_closed(self):
        self.queue["errors"] = [{"message": "Forbidden"}]
        self.assertEqual(self.run_event(), 1)

    def test_queue_refs_wrong_base_head_or_repository_fail(self):
        current = self.queue["data"]["repository"]
        current["base"]["target"]["oid"] = "d" * 40
        self.assertEqual(self.run_event(), 1)
        current["base"]["target"]["oid"] = "b" * 40
        current["group"]["target"]["oid"] = "d" * 40
        self.assertEqual(self.run_event(), 1)
        current["group"]["target"]["oid"] = "c" * 40
        current["nameWithOwner"] = "someone/else"
        self.assertEqual(self.run_event(), 1)

    def test_entry_original_head_base_and_repository_are_bound_to_pr(self):
        for field, replacement in (("headRefOid", "d" * 40), ("baseRefOid", "d" * 40),
                                   ("repository", {"nameWithOwner": "someone/else"}),
                                   ("baseRefName", "other"), ("state", "CLOSED"), ("number", True)):
            with self.subTest(field=field):
                old = self.entry["pullRequest"][field]
                self.entry["pullRequest"][field] = replacement
                self.assertEqual(self.run_event(), 1)
                self.entry["pullRequest"][field] = old

    def test_replaced_membership_before_success_fails(self):
        self.latest_queue = copy.deepcopy(self.queue)
        self.latest_queue["data"]["repository"]["mergeQueue"]["entries"]["nodes"][0]["id"] = "replacement"
        self.assertEqual(self.run_event(), 1)

    def test_replaced_live_protected_or_synthetic_ref_before_success_fails(self):
        for ref in ("base", "group"):
            with self.subTest(ref=ref):
                self.pr_reads = self.queue_reads = self.open_reads = 0
                self.latest_queue = copy.deepcopy(self.queue)
                self.latest_queue["data"]["repository"][ref]["target"]["oid"] = "d" * 40
                self.assertEqual(self.run_event(), 1)

    def test_replaced_body_head_base_or_state_before_success_fails(self):
        for field, replacement in (("body", "edited"), ("head", {"sha": "d" * 40}),
                                   ("base", {"sha": "d" * 40}), ("state", "closed"),
                                   ("draft", True), ("updated_at", "later"), ("changed_files", 2)):
            with self.subTest(field=field):
                self.calls.clear()
                self.queue_reads = self.pr_reads = self.open_reads = 0
                self.latest_pr = copy.deepcopy(self.pr)
                self.latest_pr[field] = replacement
                self.assertEqual(self.run_event(), 1)

    def test_duplicate_original_head_initial_or_newly_opened_fails(self):
        duplicate = {"number": 43, "head": {"sha": "a" * 40}}
        self.other = [duplicate]
        self.assertEqual(self.run_event(), 1)
        self.other = []
        self.open_reads = self.queue_reads = self.pr_reads = 0
        self.latest_other = [duplicate]
        self.assertEqual(self.run_event(), 1)

    def test_incomplete_or_duplicate_file_list_fails(self):
        self.pr["changed_files"] = 2
        self.assertEqual(self.run_event(), 1)
        self.files *= 2
        self.assertEqual(self.run_event(), 1)

    def test_api_outage_never_posts_status_or_exposes_payload(self):
        for endpoint in ("graphql", f"repos/{self.repo}/pulls/42", f"repos/{self.repo}/pulls/42/files?per_page=100&page=1"):
            with self.subTest(endpoint=endpoint):
                self.failure_endpoint = endpoint
                self.assertEqual(self.run_event(), 1)

    def test_malformed_event_does_not_fall_through_to_privileged_pr_handler(self):
        self.event.pop("merge_group")
        self.event["pull_request"] = {"number": 42}
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.calls, [])

    def test_wrong_action_ref_repository_or_sha_is_rejected_without_api(self):
        for path, replacement in (("action", "destroyed"), ("repository", {"full_name": "someone/else"}),
                                   ("merge_group", {"head_sha": "../../anything"})):
            with self.subTest(field=path):
                old = self.event[path]
                self.event[path] = replacement
                self.assertEqual(self.run_event(), 1)
                self.assertEqual(self.calls, [])
                self.event[path] = old

    def test_queue_pagination_is_bounded_and_cursor_must_advance(self):
        q = self.queue["data"]["repository"]["mergeQueue"]["entries"]
        q["pageInfo"] = {"hasNextPage": True, "endCursor": "same"}
        self.assertEqual(self.run_event(), 1)
        self.assertLessEqual(self.queue_reads, 2)

    def test_queue_pagination_positive_retains_single_entry_association(self):
        first = copy.deepcopy(self.queue)
        entries = first["data"]["repository"]["mergeQueue"]["entries"]
        entries["totalCount"] = 101
        entries["pageInfo"] = {"hasNextPage": True, "endCursor": "cursor100"}
        entries["nodes"] += [{"id": f"waiting{n}", "position": n, "headCommit": None} for n in range(2, 101)]
        second = copy.deepcopy(first)
        last = second["data"]["repository"]["mergeQueue"]["entries"]
        last["nodes"] = [{"id": "entry101", "position": 101, "headCommit": None}]
        last["pageInfo"] = {"hasNextPage": False, "endCursor": "cursor101"}
        def paged(method, endpoint, payload=None):
            if endpoint == "graphql":
                self.calls.append((method, endpoint, copy.deepcopy(payload)))
                self.assertIn(payload["variables"]["after"], (None, "cursor100"))
                return copy.deepcopy(first if payload["variables"]["after"] is None else second)
            return self.fake_api(method, endpoint, payload)
        with patch.object(contract, "api", side_effect=paged), patch("sys.stdout", new_callable=io.StringIO):
            self.assertEqual(contract.handle_event(self.event), 0)
        self.assertEqual(sum(endpoint == "graphql" for _, endpoint, _ in self.calls), 4)

    def test_ref_path_traversal_or_empty_suffix_fails_before_api(self):
        for suffix in ("../RAC2", "", "pr-42//x", "pr-42/", "pr-42."):
            with self.subTest(suffix=suffix):
                self.event["merge_group"]["head_ref"] = "refs/heads/gh-readonly-queue/RAC2/" + suffix
                self.assertEqual(self.run_event(), 1)
                self.assertEqual(self.calls, [])

    def test_rest_pagination_has_a_hard_limit(self):
        with patch.object(contract, "api", return_value=[{"filename": "same"}] * 100) as mocked:
            with self.assertRaises(ValueError):
                contract.bounded_list("repos/owner/repo/pulls/42/files")
            self.assertEqual(mocked.call_count, contract.MAX_REST_PAGES)

    def test_read_only_queue_workflow_and_stable_required_jobs(self):
        workflow = (ROOT / ".github/workflows/merge-queue-description.yml").read_text()
        self.assertIn("merge_group:", workflow)
        self.assertIn("checks_requested", workflow)
        self.assertIn("name: PR description", workflow)
        self.assertIn("contents: read", workflow)
        self.assertIn("pull-requests: read", workflow)
        self.assertNotIn("statuses: write", workflow)
        self.assertNotIn("checks: write", workflow)
        self.assertIn("repository: ${{ github.repository }}", workflow)
        self.assertIn("ref: refs/heads/RAC2", workflow)
        self.assertIn("persist-credentials: false", workflow)
        self.assertNotIn("github.event.pull_request.head", workflow)
        for name in ("tests.yml", "progress.yml"):
            required = (ROOT / ".github/workflows" / name).read_text()
            self.assertIn("merge_group:", required)
            self.assertIn("checks_requested", required)
            self.assertIn("contents: read", required)
        self.assertIn("\n  tests:\n", (ROOT / ".github/workflows/tests.yml").read_text())
        self.assertIn("name: SCUS_972.68 Progress", (ROOT / ".github/workflows/progress.yml").read_text())


if __name__ == "__main__":
    unittest.main()
