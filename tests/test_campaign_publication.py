"""Final publication failure cannot relabel a compiled trial as preparation."""
import importlib.util
import json
import unittest
from pathlib import Path
from unittest.mock import patch

SPEC = importlib.util.spec_from_file_location(
    "campaign_publication_fixture", Path(__file__).with_name("test_campaign.py"))
fixture = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fixture)
campaign = fixture.campaign


class CampaignPublicationTests(unittest.TestCase):
    def setUp(self):
        self.fixture = fixture.CampaignTests(methodName="runTest")
        self.fixture.setUp()
        self.addCleanup(self.fixture.tearDown)
        campaign.plan(self.fixture.store, self.fixture.task)

    def test_permanent_final_replace_preserves_real_compilation_and_identity(self):
        backend = fixture.Backend(self.fixture.tools, mismatch=True)
        real_replace = campaign.os.replace
        denied, active_id = [], []
        failure = PermissionError("synthetic final registry sharing denial")
        failure.winerror = 5

        def replace(source, destination):
            if backend.compilations:
                denied.append((source, destination))
                current = self.fixture.store.load()
                active_id.append(current["tasks"]["unit-one"]["active_trial"])
                raise failure
            return real_replace(source, destination)

        with patch.object(campaign.os, "replace", side_effect=replace), \
                patch.object(campaign.time, "sleep") as sleep:
            with self.assertRaises(campaign.TrialPublicationError) as caught:
                self.fixture.attempt(backend)
        error = caught.exception
        self.assertIs(error.__cause__, failure)
        self.assertIn(error.trial_id, str(error))
        self.assertIn(str(error.outcome_path), str(error))
        self.assertIn("do not replay compilation", str(error))
        self.assertEqual(backend.compilations, 1)
        self.assertEqual(len(denied), 11)
        self.assertEqual(sleep.call_count, 10)
        self.assertEqual(set(active_id), {error.trial_id})
        self.assertTrue(all(call == denied[0] for call in denied))

        registry = self.fixture.store.load()
        self.assertEqual(set(registry["trials"]), {error.trial_id})
        self.assertEqual(registry["tasks"]["unit-one"]["active_trial"], error.trial_id)
        self.assertEqual(registry["trials"][error.trial_id]["state"], "running")
        self.assertFalse(self.fixture.store.path.with_name("campaign-register.json.lock").exists())
        work = self.fixture.runtime / "trials" / error.trial_id
        self.assertEqual(error.outcome_path, work / "outcome.json")
        outcome_bytes = error.outcome_path.read_bytes()
        outcome = json.loads(outcome_bytes)
        self.assertEqual(outcome["id"], error.trial_id)
        self.assertEqual(outcome["state"], "mismatch")
        self.assertTrue(outcome["compile_attempted"])
        self.assertEqual(outcome["measured_functions"], 1)
        self.assertTrue((work / "candidate.o").exists())
        self.assertTrue((work / "targets/boot/outcome.json").exists())
        self.assertEqual([p.name for p in (self.fixture.runtime / "trials").iterdir()], [error.trial_id])
        pending = list(self.fixture.store.path.parent.glob("campaign-register.json.*.tmp"))
        self.assertEqual(len(pending), 1)
        retained = json.loads(pending[0].read_bytes())
        self.assertEqual(retained["trials"][error.trial_id]["state"], "mismatch")
        self.assertTrue(retained["trials"][error.trial_id]["compile_attempted"])
        self.assertEqual(retained["trials"][error.trial_id]["outcome_sha256"], campaign.digest(outcome_bytes))

    def test_final_outcome_write_failure_also_cannot_create_false_rejection(self):
        backend = fixture.Backend(self.fixture.tools)
        real_write = campaign.write_new

        def write(path, data):
            path = Path(path)
            if path.name == "outcome.json" and path.parent.parent.name == "trials":
                raise PermissionError("synthetic outcome publication denial")
            return real_write(path, data)

        with patch.object(campaign, "write_new", side_effect=write):
            with self.assertRaises(campaign.TrialPublicationError) as caught:
                self.fixture.attempt(backend)
        error = caught.exception
        self.assertEqual(backend.compilations, 1)
        registry = self.fixture.store.load()
        self.assertEqual(set(registry["trials"]), {error.trial_id})
        self.assertEqual(registry["tasks"]["unit-one"]["active_trial"], error.trial_id)
        work = self.fixture.runtime / "trials" / error.trial_id
        self.assertTrue((work / "candidate.o").exists())
        self.assertTrue((work / "targets/boot/outcome.json").exists())
        self.assertFalse(error.outcome_path.exists())
        self.assertEqual([p.name for p in (self.fixture.runtime / "trials").iterdir()], [error.trial_id])


if __name__ == "__main__":
    unittest.main()
