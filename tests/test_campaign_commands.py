"""Official review/finalization dispatch and immutable action pinning."""
import contextlib
import io
import json
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import campaign


class CampaignCommandTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.base = Path(self.temp.name)
        self.repo = self.base / "repo"
        self.repo.mkdir()
        self.runtime = self.base / "runtime"
        self.runtime.mkdir()
        self.prefix = ["--repo", str(self.repo), "--runtime", str(self.runtime)]

    def test_diff_dispatch_reads_selected_trial_without_running_compiler(self):
        with mock.patch("campaign_diff.render_review", return_value={"integration_credit": 0, "output": str(self.runtime / "review.html")}) as render, \
                mock.patch.object(campaign, "trial", side_effect=AssertionError("Review must not compile")), \
                contextlib.redirect_stdout(io.StringIO()) as output:
            code = campaign.main([*self.prefix, "diff", "unit-one", "--trial", "a" * 32,
                                  "--target", "level", "--symbol", "FUN_1000"])
        self.assertEqual(code, 0)
        self.assertEqual(json.loads(output.getvalue())["integration_credit"], 0)
        self.assertEqual(render.call_args.args[2], "unit-one")
        self.assertEqual(render.call_args.kwargs, {"trial_id": "a" * 32, "output": None, "target": "level", "symbol": "FUN_1000"})
        self.assertFalse((self.repo / "config/campaign-register.json").exists())

    def test_finalize_dispatch_has_explicit_publish_and_candidate_selection(self):
        manifest, destination, refs = self.runtime / "manifest.json", self.runtime / "final", self.runtime / "references"
        with mock.patch("campaign_finalize.finalize", return_value={"state": "applied"}) as finalize, \
                contextlib.redirect_stdout(io.StringIO()):
            code = campaign.main([*self.prefix, "finalize", "b" * 32, "--manifest", str(manifest),
                                  "--output", str(destination), "--references", str(refs),
                                  "--task", "one", "--task", "two", "--apply"])
        self.assertEqual(code, 0)
        self.assertEqual(finalize.call_args.args[2], "b" * 32)
        self.assertEqual(finalize.call_args.kwargs, {"manifest": manifest, "output": destination,
                                                    "references": refs, "tasks": ("one", "two"), "apply": True,
                                                    "maintainer_tests": ()})

    def test_finalize_dispatch_forwards_explicit_maintainer_modules(self):
        manifest, destination = self.runtime / "manifest.json", self.runtime / "final"
        with mock.patch("campaign_finalize.finalize", return_value={"state": "prepared"}) as finalize, \
                contextlib.redirect_stdout(io.StringIO()):
            code = campaign.main([*self.prefix, "finalize", "b" * 32, "--manifest", str(manifest),
                                  "--output", str(destination), "--maintainer-test", "test_campaign_finalize",
                                  "--maintainer-test", "test_maintainer_test_policy"])
        self.assertEqual(code, 0)
        self.assertEqual(finalize.call_args.kwargs["maintainer_tests"],
                         ("test_campaign_finalize", "test_maintainer_test_policy"))
        self.assertFalse(finalize.call_args.kwargs["apply"])

    def test_browser_open_requires_readonly_server(self):
        with contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as error:
            campaign.main([*self.prefix, "diff", "one", "--open"])
        self.assertEqual(error.exception.code, 2)

    def test_invalid_port_is_rejected_before_creating_review(self):
        for port in (-1, 65536):
            with self.subTest(port=port), mock.patch("campaign_diff.render_review") as render, \
                    contextlib.redirect_stderr(io.StringIO()), self.assertRaises(SystemExit) as error:
                campaign.main([*self.prefix, "diff", "one", "--serve", "--port", str(port)])
            self.assertEqual(error.exception.code, 2)
            render.assert_not_called()

    def test_facade_pins_manifest_and_outcome_without_self_reference(self):
        store = campaign.Store(self.repo / "config/campaign-register.json", self.runtime)
        def runner(args, **kwargs):
            target = Path(args[args.index("--output") + 1])
            target.write_bytes(b'{"verified":true}\n')
            return SimpleNamespace(returncode=0)
        result = campaign.facade(store, self.repo, "report", [], runner=runner)
        self.assertEqual(result["state"], "passed")
        row = store.load()["actions"][result["id"]]
        directory = self.runtime / "actions" / result["id"]
        self.assertEqual(row["action_manifest_sha256"], campaign.digest((directory / "manifest.json").read_bytes()))
        self.assertEqual(row["action_outcome_sha256"], campaign.digest((directory / "outcome.json").read_bytes()))
        self.assertNotIn("action_outcome_sha256", campaign.read(directory / "outcome.json"))
        original = row["action_manifest_sha256"]
        with (directory / "manifest.json").open("ab") as stream:
            stream.write(b"\n")
        self.assertNotEqual(campaign.digest((directory / "manifest.json").read_bytes()), original)
        self.assertEqual(store.load()["actions"][result["id"]]["action_manifest_sha256"], original)

    def test_sdk_binding_change_invalidates_action_even_when_runner_succeeds(self):
        store = campaign.Store(self.repo / "config/campaign-register.json", self.runtime)
        manifest = self.runtime / "manifest.json"
        binding = self.runtime / "sdk-binding.json"
        manifest.write_bytes(b"{}\n")
        binding.write_bytes(b'{"qualified":true}\n')
        original = campaign.digest(binding.read_bytes())
        def runner(args, **kwargs):
            binding.write_bytes(b'{"qualified":false}\n')
            return SimpleNamespace(returncode=0)
        result = campaign.facade(store, self.repo, "integrate", ["--manifest", str(manifest),
                                  "--sdk-binding", str(binding)], runner=runner)
        self.assertEqual(result["state"], "provenance_changed")
        recorded = campaign.read(self.runtime / "actions" / result["id"] / "manifest.json")
        self.assertEqual(recorded["external_input_sha256"][str(binding.resolve())], original)
        self.assertEqual(result["integration_credit"], 0)


if __name__ == "__main__":
    unittest.main()
