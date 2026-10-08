"""Atomic registry rename retries never replay an edit or lose its evidence."""
import errno
import importlib.util
import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

SCRIPT = Path(__file__).resolve().parents[1] / "scripts/campaign.py"
SPEC = importlib.util.spec_from_file_location("campaign_atomic", SCRIPT)
campaign = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(campaign)


def windows_permission(code):
    error = PermissionError(errno.EACCES, "synthetic Windows sharing denial")
    error.winerror = code
    return error


class CampaignAtomicTests(unittest.TestCase):
    def setUp(self):
        self.directory = tempfile.TemporaryDirectory()
        self.addCleanup(self.directory.cleanup)
        self.root = Path(self.directory.name)
        self.path = self.root / "registry.json"
        self.runtime = self.root / "runtime"
        self.original = campaign.encoded({"schema": 1, "kind": "rac2-campaign",
                                          "revision": 4, "mutations": 0})
        self.path.write_bytes(self.original)
        self.store = campaign.Store(self.path, self.runtime)

    def mutate(self, callbacks):
        with self.store.edit() as registry:
            callbacks.append("mutation")
            registry["mutations"] += 1

    def assert_retained(self, callbacks):
        self.assertEqual(callbacks, ["mutation"])
        self.assertEqual(self.path.read_bytes(), self.original)
        self.assertFalse(self.path.with_name("registry.json.lock").exists())
        temporary = list(self.root.glob("registry.json.*.tmp"))
        revisions = list((self.runtime / "registry-revisions").glob("*.json"))
        self.assertEqual(len(temporary), 1)
        self.assertEqual(len(revisions), 1)
        self.assertEqual(temporary[0].read_bytes(), revisions[0].read_bytes())
        value = json.loads(temporary[0].read_bytes())
        self.assertEqual(value["revision"], 5)
        self.assertEqual(value["mutations"], 1)

    def test_transient_windows_errors_retry_the_same_written_temp_once(self):
        real_replace = campaign.os.replace
        for code in (5, 32, 33):
            with self.subTest(winerror=code):
                self.path.write_bytes(self.original)
                callbacks, attempts = [], []

                def replace(source, destination):
                    attempts.append((source, destination, Path(source).read_bytes()))
                    self.assertTrue(self.path.with_name("registry.json.lock").exists())
                    if len(attempts) < 4:
                        self.assertEqual(self.path.read_bytes(), self.original)
                        raise windows_permission(code)
                    return real_replace(source, destination)

                before = len(list((self.runtime / "registry-revisions").glob("*.json")))
                with patch.object(campaign.os, "replace", side_effect=replace), \
                        patch.object(campaign.time, "sleep") as sleep:
                    self.mutate(callbacks)
                self.assertEqual(callbacks, ["mutation"])
                self.assertEqual(len(attempts), 4)
                self.assertTrue(all(item == attempts[0] for item in attempts))
                self.assertEqual(sleep.call_args_list, [unittest.mock.call(.05)] * 3)
                self.assertEqual(len(list((self.runtime / "registry-revisions").glob("*.json"))), before + 1)
                self.assertEqual(self.store.load()["revision"], 5)
                self.assertEqual(self.store.load()["mutations"], 1)
                self.assertFalse(list(self.root.glob("registry.json.*.tmp")))
                self.assertFalse(self.path.with_name("registry.json.lock").exists())

    def test_permanent_windows_error_retains_original_temp_and_releases_lock(self):
        callbacks = []
        failure = windows_permission(5)
        with patch.object(campaign.os, "replace", side_effect=failure) as replace, \
                patch.object(campaign.time, "sleep") as sleep:
            with self.assertRaises(PermissionError) as caught:
                self.mutate(callbacks)
        self.assertIs(caught.exception, failure)
        self.assertEqual(replace.call_count, 11)
        self.assertEqual(sleep.call_args_list, [unittest.mock.call(.05)] * 10)
        self.assertTrue(all(item == replace.call_args_list[0] for item in replace.call_args_list))
        self.assert_retained(callbacks)

    def test_posix_permission_error_is_not_retried(self):
        callbacks = []
        failure = PermissionError(errno.EACCES, "synthetic POSIX permission failure")
        with patch.object(campaign.os, "replace", side_effect=failure) as replace, \
                patch.object(campaign.time, "sleep") as sleep:
            with self.assertRaises(PermissionError) as caught:
                self.mutate(callbacks)
        self.assertIs(caught.exception, failure)
        replace.assert_called_once()
        sleep.assert_not_called()
        self.assert_retained(callbacks)

    def test_unrelated_windows_error_is_not_retried(self):
        callbacks = []
        failure = windows_permission(87)
        with patch.object(campaign.os, "replace", side_effect=failure) as replace, \
                patch.object(campaign.time, "sleep") as sleep:
            with self.assertRaises(PermissionError):
                self.mutate(callbacks)
        replace.assert_called_once()
        sleep.assert_not_called()
        self.assert_retained(callbacks)

    def test_edit_failure_never_attempts_rename(self):
        with patch.object(campaign.os, "replace") as replace, \
                patch.object(campaign.time, "sleep") as sleep:
            with self.assertRaisesRegex(ValueError, "mutation failed"):
                with self.store.edit() as registry:
                    registry["mutations"] += 1
                    raise ValueError("mutation failed")
        replace.assert_not_called()
        sleep.assert_not_called()
        self.assertEqual(self.path.read_bytes(), self.original)
        self.assertFalse(self.path.with_name("registry.json.lock").exists())


if __name__ == "__main__":
    unittest.main()
