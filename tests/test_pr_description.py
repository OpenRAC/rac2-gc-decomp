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
        # These are PR-target/dispatch fixtures even when the whole suite is
        # itself run by a merge_group workflow. Do not inherit its event kind.
        self.env = patch.dict(os.environ, {"GITHUB_REPOSITORY": "OpenRAC/rac2-gc-decomp", "GITHUB_RUN_ID": "1234",
                                          "GITHUB_EVENT_NAME": "pull_request_target"})
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
                                      "head_ref": "refs/heads/gh-readonly-queue/RAC2/pr-999-untrusted"}}
        self.pr = {"number": 42, "state": "open", "draft": False, "body": body(),
                   "changed_files": 1, "updated_at": "2026-10-09T01:00:00Z",
                   "head": {"sha": "a" * 40},
                   "base": {"ref": "RAC2", "sha": "e" * 40, "repo": {"full_name": self.repo}}}
        self.associated = [copy.deepcopy(self.pr)]
        self.rules = [{"type": "merge_queue", "ruleset_id": 24792387,
                       "ruleset_source_type": "Repository", "ruleset_source": self.repo,
                       "parameters": {"grouping_strategy": "ALLGREEN", "merge_method": "MERGE",
                                      "max_entries_to_build": 1, "max_entries_to_merge": 1,
                                      "min_entries_to_merge": 1}}]
        self.refs = {"refs/heads/RAC2": {"ref": "refs/heads/RAC2", "object": {"type": "commit", "sha": "b" * 40}},
                     self.event["merge_group"]["head_ref"]: {"ref": self.event["merge_group"]["head_ref"],
                                                            "object": {"type": "commit", "sha": "c" * 40}}}
        self.commit = {"sha": "c" * 40, "parents": [{"sha": "b" * 40}, {"sha": "a" * 40}],
                       "message": "PR #999 is only prose, never membership"}
        self.files = [{"filename": "README.md"}]
        self.other = []
        self.latest_pr = self.latest_rules = self.latest_refs = self.latest_commit = self.latest_associated = self.latest_other = None
        self.failure_endpoint = None
        self.calls = []
        self.counts = {}
        self.env = patch.dict(os.environ, {"GITHUB_REPOSITORY": self.repo, "GITHUB_EVENT_NAME": "merge_group",
                                          "GITHUB_SHA": "c" * 40})
        self.env.start()
        self.addCleanup(self.env.stop)

    def fake_api(self, method, endpoint, payload=None):
        self.calls.append((method, endpoint, copy.deepcopy(payload)))
        self.assertEqual(method, "GET", "Queue route attempted an API write/query POST")
        self.assertIsNone(payload)
        if endpoint == self.failure_endpoint:
            raise contract.urllib.error.HTTPError("https://example.invalid", 403, "SECRET ::warning:: payload", None, None)
        self.counts[endpoint] = self.counts.get(endpoint, 0) + 1
        later = self.counts[endpoint] > 1
        if "/rules/branches/RAC2?" in endpoint:
            return copy.deepcopy(self.latest_rules if later and self.latest_rules is not None else self.rules)
        if "/git/ref/" in endpoint:
            refs = self.latest_refs if later and self.latest_refs is not None else self.refs
            return copy.deepcopy(refs["refs/" + endpoint.split("/git/ref/", 1)[1]])
        if "/git/commits/" in endpoint:
            self.assertTrue(endpoint.endswith(self.event["merge_group"]["head_sha"]))
            return copy.deepcopy(self.latest_commit if later and self.latest_commit is not None else self.commit)
        if "/commits/" in endpoint and "/pulls?" in endpoint:
            self.assertIn("/commits/" + self.commit["parents"][1]["sha"] + "/pulls?", endpoint)
            return copy.deepcopy(self.latest_associated if later and self.latest_associated is not None else self.associated)
        if "/files?" in endpoint:
            return copy.deepcopy(self.files)
        if "?state=open" in endpoint:
            other = self.latest_other if later and self.latest_other is not None else self.other
            return copy.deepcopy([self.pr] + other)
        self.assertEqual(endpoint, f"repos/{self.repo}/pulls/{self.pr['number']}")
        return copy.deepcopy(self.latest_pr if later and self.latest_pr is not None else self.pr)

    def run_event(self):
        with patch.object(contract, "api", side_effect=self.fake_api), patch("sys.stdout", new_callable=io.StringIO) as output:
            result = contract.handle_event(self.event)
        self.output = output.getvalue()
        self.assertTrue(all(method == "GET" for method, _, _ in self.calls))
        self.assertTrue(all("statuses" not in endpoint and endpoint != "graphql" for _, endpoint, _ in self.calls))
        self.assertNotIn("SECRET", self.output)
        self.assertNotIn("::warning::", self.output)
        return result

    def reset_calls(self):
        self.calls.clear()
        self.counts.clear()

    def test_live_single_pr_ancestry_and_old_cached_base_pass_without_writes(self):
        self.assertEqual(self.run_event(), 0)
        self.assertTrue(all(count == 2 for endpoint, count in self.counts.items() if "/files?" not in endpoint))
        self.assertTrue(all("/pulls/999" not in endpoint for _, endpoint, _ in self.calls))

    def test_measured_real_queue_commit_shape_and_behind_pr_pass(self):
        base = "6a6fe2c31e2a59984097163b2cf8678c170165b9"
        source = "13234ee8b728d2e5167998c145e644f2f7335153"
        head = "0e1244e843669ea0f57fe3de1563ae67d141fed2"
        self.event["merge_group"].update(base_sha=base, head_sha=head)
        os.environ["GITHUB_SHA"] = head
        self.refs["refs/heads/RAC2"]["object"]["sha"] = base
        self.refs[self.event["merge_group"]["head_ref"]]["object"]["sha"] = head
        self.commit.update(sha=head, parents=[{"sha": base}, {"sha": source}])
        self.pr["number"] = 77
        self.pr["head"]["sha"] = source
        self.pr["base"]["sha"] = "5f3c5e50c79bd6461bfde969c2a39669da30f425"
        self.associated = [copy.deepcopy(self.pr)]
        self.assertEqual(self.run_event(), 0)

    def test_short_and_canonical_webhook_refs_bind_same_live_refs(self):
        self.event["merge_group"]["head_ref"] = self.event["merge_group"]["head_ref"].removeprefix("refs/heads/")
        self.event["merge_group"]["base_ref"] = "RAC2"
        self.assertEqual(self.run_event(), 0)

    def test_actions_sha_mismatch_fails_before_api(self):
        os.environ["GITHUB_SHA"] = "d" * 40
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.calls, [])
        self.assertIn("event (invalid_or_changed_evidence)", self.output)

    def test_optional_actions_sha_absent_is_supported(self):
        os.environ.pop("GITHUB_SHA", None)
        self.assertEqual(self.run_event(), 0)

    def test_invalid_body_and_actual_source_type_fail(self):
        self.pr["body"] = "please merge"
        self.assertEqual(self.run_event(), 1)
        self.pr["body"] = body()
        self.files = [{"filename": "src/boot/new.cfrag"}]
        self.assertEqual(self.run_event(), 1)

    def test_missing_duplicate_or_disabled_applicable_queue_rule_fails(self):
        self.rules = []
        self.assertEqual(self.run_event(), 1)
        self.assertIn("policy", self.output)
        self.rules = [{"type": "required_status_checks"}]
        self.assertEqual(self.run_event(), 1)
        self.rules = [{"type": "merge_queue"}, {"type": "merge_queue"}]
        self.assertEqual(self.run_event(), 1)

    def test_unknown_or_boolean_policy_and_non_merge_method_fail(self):
        params = self.rules[0]["parameters"]
        for field, value in (("grouping_strategy", "HEADGREEN"), ("max_entries_to_build", 2),
                             ("max_entries_to_merge", True), ("min_entries_to_merge", 2),
                             ("merge_method", "SQUASH")):
            with self.subTest(field=field):
                old = params[field]
                params[field] = value
                self.assertEqual(self.run_event(), 1)
                params[field] = old

    def test_rule_repository_and_identity_must_match(self):
        for field, value in (("ruleset_source", "somebody/else"), ("ruleset_source_type", "Organization"),
                             ("ruleset_id", True)):
            with self.subTest(field=field):
                old = self.rules[0][field]
                self.rules[0][field] = value
                self.assertEqual(self.run_event(), 1)
                self.rules[0][field] = old

    def test_refs_bind_exact_base_head_type_and_name(self):
        for ref in self.refs:
            with self.subTest(ref=ref):
                old = copy.deepcopy(self.refs[ref])
                self.refs[ref]["object"]["sha"] = "d" * 40
                self.assertEqual(self.run_event(), 1)
                self.refs[ref] = copy.deepcopy(old)
                self.refs[ref]["object"]["type"] = "tag"
                self.assertEqual(self.run_event(), 1)
                self.refs[ref] = copy.deepcopy(old)
                self.refs[ref]["ref"] = "refs/heads/other"
                self.assertEqual(self.run_event(), 1)
                self.refs[ref] = old

    def test_extra_missing_wrong_order_or_duplicate_parents_fail(self):
        original = copy.deepcopy(self.commit["parents"])
        for parents in ([], original[:1], original + [{"sha": "d" * 40}], original[::-1],
                        [{"sha": "d" * 40}, original[1]], [original[0], original[0]],
                        [original[0], {"sha": "c" * 40}], [original[0], {"sha": "bad"}]):
            with self.subTest(parents=parents):
                self.commit["parents"] = parents
                self.assertEqual(self.run_event(), 1)
                self.assertIn("ancestry", self.output)
        self.commit["parents"] = original

    def test_git_commit_response_sha_must_equal_event_head(self):
        self.commit["sha"] = "d" * 40
        self.assertEqual(self.run_event(), 1)

    def test_missing_foreign_closed_draft_or_wrong_source_association_fails(self):
        self.associated = []
        self.assertEqual(self.run_event(), 1)
        for field, value in (("state", "closed"), ("draft", True), ("head", {"sha": "d" * 40}),
                             ("base", {"sha": "e" * 40, "ref": "other", "repo": {"full_name": self.repo}}),
                             ("base", {"sha": "e" * 40, "ref": "RAC2", "repo": {"full_name": "foreign/repo"}})):
            with self.subTest(field=field):
                candidate = copy.deepcopy(self.pr)
                candidate[field] = value
                self.associated = [candidate]
                self.assertEqual(self.run_event(), 1)

    def test_duplicate_association_or_shared_source_head_fails(self):
        second = copy.deepcopy(self.pr)
        second["number"] = 43
        self.associated.append(second)
        self.assertEqual(self.run_event(), 1)
        self.associated = [copy.deepcopy(self.pr)] * 2
        self.assertEqual(self.run_event(), 1)
        self.associated = [copy.deepcopy(self.pr)]
        self.other = [second]
        self.assertEqual(self.run_event(), 1)

    def test_newly_opened_duplicate_head_before_success_fails(self):
        self.latest_other = [{"number": 43, "head": {"sha": "a" * 40}}]
        self.assertEqual(self.run_event(), 1)

    def test_ancestor_commit_association_cannot_select_pr_with_different_head(self):
        self.associated[0]["head"]["sha"] = "d" * 40
        self.assertEqual(self.run_event(), 1)

    def test_pr_fields_or_cached_base_mismatching_association_fail(self):
        for field, value in (("head", {"sha": "d" * 40}), ("state", "closed"), ("draft", True),
                             ("base", {"ref": "RAC2", "sha": "d" * 40, "repo": {"full_name": self.repo}}),
                             ("changed_files", True), ("body", {})):
            with self.subTest(field=field):
                old = copy.deepcopy(self.pr[field])
                self.pr[field] = value
                self.assertEqual(self.run_event(), 1)
                self.pr[field] = old

    def test_replaced_refs_policy_ancestry_and_association_before_success_fail(self):
        self.latest_refs = copy.deepcopy(self.refs)
        self.latest_refs["refs/heads/RAC2"]["object"]["sha"] = "d" * 40
        self.assertEqual(self.run_event(), 1)
        self.reset_calls()
        self.latest_refs = None
        self.latest_rules = []
        self.assertEqual(self.run_event(), 1)
        self.reset_calls()
        self.latest_rules = None
        self.latest_commit = copy.deepcopy(self.commit)
        self.latest_commit["parents"][1]["sha"] = "d" * 40
        self.assertEqual(self.run_event(), 1)
        self.reset_calls()
        self.latest_commit = None
        self.latest_associated = []
        self.assertEqual(self.run_event(), 1)

    def test_replaced_body_head_base_draft_or_state_before_success_fails(self):
        for field, value in (("body", "edited"), ("head", {"sha": "d" * 40}),
                             ("base", {"sha": "d" * 40}), ("state", "closed"), ("draft", True),
                             ("updated_at", "later"), ("changed_files", 2)):
            with self.subTest(field=field):
                self.reset_calls()
                self.latest_pr = copy.deepcopy(self.pr)
                self.latest_pr[field] = value
                self.assertEqual(self.run_event(), 1)

    def test_incomplete_duplicate_or_malformed_file_list_fails(self):
        self.pr["changed_files"] = 2
        self.assertEqual(self.run_event(), 1)
        self.files *= 2
        self.assertEqual(self.run_event(), 1)
        self.files = [{"filename": {}}]
        self.assertEqual(self.run_event(), 1)

    def test_api_permission_error_logs_stage_and_code_without_raw_exception(self):
        for endpoint, phase in ((f"repos/{self.repo}/rules/branches/RAC2?per_page=100&page=1", "policy"),
                                (f"repos/{self.repo}/git/commits/" + "c" * 40, "ancestry"),
                                (f"repos/{self.repo}/commits/" + "a" * 40 + "/pulls?per_page=100&page=1", "association"),
                                (f"repos/{self.repo}/pulls/42/files?per_page=100&page=1", "files")):
            with self.subTest(phase=phase):
                self.failure_endpoint = endpoint
                self.assertEqual(self.run_event(), 1)
                self.assertIn("at " + phase + " (api_forbidden)", self.output)

    def test_malformed_merge_event_never_falls_through_to_status_writer(self):
        self.event.pop("merge_group")
        self.event["pull_request"] = {"number": 42}
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.calls, [])

    def test_merge_group_pair_list_is_not_accepted_as_a_webhook_object(self):
        self.event["merge_group"] = list(self.event["merge_group"].items())
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.calls, [])

    def test_wrong_action_repo_sha_or_base_ref_fails_without_api(self):
        for field, value in (("action", "destroyed"), ("repository", {"full_name": "foreign/repo"}),
                             ("merge_group", {"head_sha": "../../invalid"})):
            with self.subTest(field=field):
                old = self.event[field]
                self.event[field] = value
                self.assertEqual(self.run_event(), 1)
                self.assertEqual(self.calls, [])
                self.event[field] = old
        self.event["merge_group"]["base_ref"] = "master"
        self.assertEqual(self.run_event(), 1)
        self.assertEqual(self.calls, [])

    def test_ref_path_traversal_empty_or_unsafe_suffix_fails_before_api(self):
        for suffix in ("../RAC2", "", "pr-42//x", "pr-42/", "pr-42.", "pr-42?x=y"):
            with self.subTest(suffix=suffix):
                self.event["merge_group"]["head_ref"] = "refs/heads/gh-readonly-queue/RAC2/" + suffix
                self.assertEqual(self.run_event(), 1)
                self.assertEqual(self.calls, [])

    def test_association_pagination_positive_and_hard_bound(self):
        first = [copy.deepcopy(self.pr)] + [{**copy.deepcopy(self.pr), "number": n, "state": "closed"} for n in range(100, 199)]
        second = [{**copy.deepcopy(self.pr), "number": 199, "state": "closed"}]
        def paged(method, endpoint, payload=None):
            if "/commits/" in endpoint and "/pulls?" in endpoint:
                self.calls.append((method, endpoint, payload))
                self.assertEqual(method, "GET")
                return copy.deepcopy(second if endpoint.endswith("page=2") else first)
            return self.fake_api(method, endpoint, payload)
        with patch.object(contract, "api", side_effect=paged), patch("sys.stdout", new_callable=io.StringIO):
            self.assertEqual(contract.handle_event(self.event), 0)
        self.assertEqual(sum("/commits/" in endpoint and "/pulls?" in endpoint for _, endpoint, _ in self.calls), 4)
        with patch.object(contract, "api", return_value=[{"filename": "same"}] * 100) as mocked:
            with self.assertRaises(ValueError):
                contract.bounded_list("repos/owner/repo/pulls/42/files")
            self.assertEqual(mocked.call_count, contract.MAX_REST_PAGES)

    def test_applicable_policy_pagination_finds_later_rule_and_rejects_hidden_duplicate(self):
        first = [{"type": "deletion"}] * 100
        second = copy.deepcopy(self.rules)
        def paged(method, endpoint, payload=None):
            if "/rules/branches/RAC2?" in endpoint:
                self.calls.append((method, endpoint, payload))
                self.assertEqual(method, "GET")
                return copy.deepcopy(second if endpoint.endswith("page=2") else first)
            return self.fake_api(method, endpoint, payload)
        with patch.object(contract, "api", side_effect=paged), patch("sys.stdout", new_callable=io.StringIO):
            self.assertEqual(contract.handle_event(self.event), 0)
            first = copy.deepcopy(self.rules) + [{"type": "deletion"}] * 99
            self.assertEqual(contract.handle_event(self.event), 1)

    def test_api_malformed_lists_fail_closed(self):
        with patch.object(contract, "api", return_value={"unexpected": True}):
            with self.assertRaises(ValueError):
                contract.bounded_list("repos/owner/repo/pulls")
        self.associated = [{"number": True}]
        self.assertEqual(self.run_event(), 1)

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
        for name in ("tests.yml",):
            required = (ROOT / ".github/workflows" / name).read_text()
            self.assertIn("merge_group:", required)
            self.assertIn("checks_requested", required)
            self.assertIn("contents: read", required)
        self.assertIn("\n  tests:\n", (ROOT / ".github/workflows/tests.yml").read_text())
        self.assertIn("name: SCUS_972.68 Progress", (ROOT / ".github/workflows/tests.yml").read_text())


if __name__ == "__main__":
    unittest.main()
