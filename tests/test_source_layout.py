"""Private pilot invariants; no compiler or retail data needed."""
import json
import os
from pathlib import Path
import shutil
import tempfile
import sys
import unittest

HERE = Path(__file__).resolve().parent
REPO = Path(os.environ["RAC2_LAYOUT_REPO"]) if "RAC2_LAYOUT_REPO" in os.environ else next(p for p in [HERE, *HERE.parents] if (p / "candidates/boot.c").exists())
sys.path.insert(0, str(REPO / "scripts"))
import source_layout as tool
from boot_sdk_unit import admitted_units


def small_data_units(repo):
    """Overlays that need a separate small-data unit (config/level-g8)."""
    return sorted((repo / "config/level-g8").glob("*.json"))


class SourceLayoutTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="test-layout-")
        self.addCleanup(self.temp.cleanup)
        self.layout = Path(self.temp.name)
        tool.capture(REPO, self.layout)

    def test_all_catalogued_standalone_sources_are_byte_identical(self):
        result = tool.verify(REPO, self.layout, self.layout / "generated")
        has_sdk = (REPO / "config/boot-units/sdk-sysbit-flush.json").exists()
        self.assertEqual(result["byte_identical_sources"],
                         28 + len(admitted_units(REPO)) + len(small_data_units(REPO)))
        if has_sdk:
            self.assertEqual((self.layout / "generated/candidates/sdk/sysbit_flush.c").read_bytes(),
                             (REPO / "src/sdk/sysbit_flush.c").read_bytes())
            self.assertEqual(result["metrics"]["sdk_authored_functions"], len(admitted_units(REPO)))
        inventory = json.loads((REPO / "progress/source-inventory.json").read_bytes())
        self.assertEqual(result["metrics"]["native_explicit_base_source_families"],
                         inventory["metrics"]["native_explicit_base_source_families"])
        self.assertEqual(result["metrics"]["native_base_family_placements"],
                         inventory["metrics"]["native_base_family_placements"])
        self.assertEqual(result["metrics"]["native_unique_authored_source_variants"],
                         inventory["metrics"]["native_unique_authored_source_variants"])

    def test_changed_fragment_is_rejected(self):
        fragment = self.layout / "src/levels/shared/clear-five-words.cfrag"
        fragment.write_bytes(fragment.read_bytes().replace(b"object[4]=0", b"object[4]=1"))
        with self.assertRaisesRegex(ValueError, "changed source fragment"):
            tool.verify(REPO, self.layout)

    def test_edited_metric_is_rejected(self):
        path = self.layout / "config/source-layout.json"
        manifest = json.loads(path.read_bytes())
        manifest["metrics"]["native_unique_authored_source_variants"] += 1
        path.write_text(json.dumps(manifest))
        with self.assertRaisesRegex(ValueError, "metrics are stale"):
            tool.verify(REPO, self.layout)

    def test_edited_family_membership_is_rejected(self):
        path = self.layout / "config/source-layout.json"
        manifest = json.loads(path.read_bytes())
        manifest["native_source_families"][0]["placements"].pop()
        path.write_text(json.dumps(manifest))
        with self.assertRaisesRegex(ValueError, "family mapping"):
            tool.verify(REPO, self.layout)

    def test_output_escape_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "outside and not contain"):
            tool.verify(REPO, self.layout, REPO)

    def test_changed_output_requires_its_exact_current_hash(self):
        output = self.layout / "generated"
        tool.verify(REPO, self.layout, output)
        destination = output / "candidates/boot.c"
        destination.write_bytes(b"modified generated source\n")
        with self.assertRaisesRegex(ValueError, "explicit current hash"):
            tool.verify(REPO, self.layout, output)
        with self.assertRaisesRegex(ValueError, "explicit current hash"):
            tool.verify(REPO, self.layout, output, {"candidates/boot.c": "0" * 64})
        tool.verify(REPO, self.layout, output, {"candidates/boot.c": tool.digest(destination.read_bytes())})
        self.assertEqual(destination.read_bytes(), (REPO / "candidates/boot.c").read_bytes())

    def test_stale_public_input_is_rejected_using_private_fixture(self):
        snapshot = self.layout / "fixture-repo"
        manifest = json.loads((self.layout / "config/source-layout.json").read_bytes())
        for relative in manifest["input_sha256"]:
            destination = snapshot / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(REPO / relative, destination)
        (snapshot / "candidates/boot.c").write_bytes(b"changed public-input fixture\n")
        with self.assertRaisesRegex(ValueError, "stale public input"):
            tool.verify(snapshot, self.layout)

    def fixture_repo(self):
        snapshot = self.layout / "fixture-repo"
        manifest = json.loads((self.layout / "config/source-layout.json").read_bytes())
        for relative in manifest["input_sha256"]:
            destination = snapshot / relative
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(REPO / relative, destination)
        # Recapture reads every authored G8 module, including modules belonging
        # to other overlays than the one changed by the fixture.
        for path in small_data_units(snapshot):
            module = json.loads(path.read_bytes())["module"]
            destination = snapshot / module
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(REPO / module, destination)
        return snapshot, manifest

    def test_authoring_edits_fragment_and_refreshes_manifest(self):
        repo, manifest = self.fixture_repo()
        fragment = self.layout / "src/boot/01-resident-accessors.cfrag"
        fragment.write_bytes(fragment.read_bytes() + b"\n/* Authored organization test comment. */\n")
        expected = dict(manifest["input_sha256"])
        expected["config/source-layout.json"] = tool.digest((self.layout / "config/source-layout.json").read_bytes())
        result = tool.author(repo, self.layout, None, expected)
        self.assertTrue(result["manifest_refreshed"])
        self.assertFalse(result["compiler_or_retail_gate_run"])
        self.assertIn(b"Authored organization test comment", (repo / "candidates/boot.c").read_bytes())
        self.assertEqual(tool.verify(repo, self.layout)["byte_identical_sources"],
                         28 + len(admitted_units(repo)) + len(small_data_units(repo)))

    def test_authoring_can_add_catalogued_function_without_reslicing_modules(self):
        repo, manifest = self.fixture_repo()
        fragment = self.layout / "src/boot/01-resident-accessors.cfrag"
        fragment.write_bytes(fragment.read_bytes() + b"\nvoid FUN_00ABCDEF(void) {}\n")
        catalogue_path = repo / "config/candidate-catalog.json"
        catalogue = json.loads(catalogue_path.read_bytes())
        catalogue["functions"].append({"symbol": "FUN_00ABCDEF", "address": 0xABCDEF, "size": 8})
        catalogue_path.write_text(json.dumps(catalogue))
        expected = dict(manifest["input_sha256"])
        expected["config/candidate-catalog.json"] = tool.digest(catalogue_path.read_bytes())
        expected["config/source-layout.json"] = tool.digest((self.layout / "config/source-layout.json").read_bytes())
        tool.author(repo, self.layout, None, expected)
        self.assertEqual(tool.verify(repo, self.layout)["metrics"]["boot_authored_functions"],
                         len(catalogue["functions"]))

    def test_authoring_refuses_overwrite_without_current_source_hash(self):
        repo, manifest = self.fixture_repo()
        fragment = self.layout / "src/boot/01-resident-accessors.cfrag"
        fragment.write_bytes(fragment.read_bytes() + b"\n/* edit */\n")
        before = (repo / "candidates/boot.c").read_bytes()
        with self.assertRaisesRegex(ValueError, "explicit current hash"):
            tool.author(repo, self.layout, None, {})
        self.assertEqual((repo / "candidates/boot.c").read_bytes(), before)

    def test_changed_clear_semantics_does_not_merge_original_ship_variant(self):
        repo, manifest = self.fixture_repo()
        fragment = self.layout / "src/levels/shared/clear-five-words.cfrag"
        fragment.write_bytes(fragment.read_bytes().replace(b"object[4]=0", b"object[4]=1"))
        expected = dict(manifest["input_sha256"])
        expected["config/source-layout.json"] = tool.digest((self.layout / "config/source-layout.json").read_bytes())
        tool.author(repo, self.layout, None, expected)
        metrics = tool.verify(repo, self.layout)["metrics"]
        self.assertEqual(metrics["pilot_family_placements"], 26)
        self.assertEqual(metrics["native_unmerged_singletons"],
                         manifest["metrics"]["native_unmerged_singletons"] + 1)


    def test_capture_preserves_complete_multifunction_small_data_module_once(self):
        repo, _ = self.fixture_repo()
        path = small_data_units(repo)[0]
        catalog = json.loads(path.read_bytes())
        (repo / catalog["module"]).parent.mkdir(parents=True, exist_ok=True)
        symbol = "LVL_" + catalog["level"].upper() + "_FUN_00ABCDE0"
        source = (repo / catalog["source"]).read_bytes() + ("\nvoid " + symbol + "(void) {}\n").encode()
        (repo / catalog["module"]).write_bytes(source)
        (repo / catalog["source"]).write_bytes(source)
        catalog["functions"].append({"symbol": symbol, "address": 0xABCDE0, "size": 8})
        path.write_text(json.dumps(catalog))
        recaptured = self.layout / "recaptured"
        manifest = tool.capture(repo, recaptured)
        pieces = manifest["recipes"][catalog["source"]]["pieces"]
        self.assertEqual(len(pieces), 1)
        self.assertNotIn("replacements", pieces[0])
        tool.verify(repo, recaptured, recaptured / "generated")
        self.assertEqual((recaptured / "generated" / catalog["source"]).read_bytes(), source)

    def test_capture_rejects_ambiguous_or_stale_small_data_module(self):
        repo, _ = self.fixture_repo()
        path = small_data_units(repo)[0]
        catalog = json.loads(path.read_bytes())
        (repo / catalog["module"]).parent.mkdir(parents=True, exist_ok=True)
        catalog["functions"].append({"symbol": "LVL_TEST_FUN_00ABCDE0", "address": 0xABCDE0, "size": 8})
        path.write_text(json.dumps(catalog))
        (repo / catalog["module"]).write_bytes(b"void @@FUNCTION@@(void) {}\n")
        with self.assertRaisesRegex(ValueError, "template requires exactly one"):
            tool.capture(repo, self.layout / "ambiguous-template")
        (repo / catalog["module"]).write_bytes((repo / catalog["source"]).read_bytes() + b"/* stale */\n")
        with self.assertRaisesRegex(ValueError, "does not reproduce"):
            tool.capture(repo, self.layout / "stale-module")


if __name__ == "__main__":
    unittest.main()
