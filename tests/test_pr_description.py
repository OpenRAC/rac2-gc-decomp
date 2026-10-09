"""Description admission and privileged metadata handler safety regressions."""
import copy
import importlib.util
import os
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
        self.assertIn("github.event.pull_request.base.sha || github.sha", workflow)
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


if __name__ == "__main__":
    unittest.main()
