"""Region selection: the USA proofs stay pinned, other releases fail closed until measured."""
import contextlib
import importlib
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
region = importlib.import_module("region")
pipeline_setup = importlib.import_module("setup")
pipeline_build = importlib.import_module("build")
doctor = importlib.import_module("doctor")


def clean_environment():
    return mock.patch.dict(os.environ, {key: value for key, value in os.environ.items()
                                        if key != region.ENVIRONMENT}, clear=True)


class RegistryTests(unittest.TestCase):
    def setUp(self):
        patcher = clean_environment()
        patcher.start()
        self.addCleanup(patcher.stop)

    def test_default_is_the_unchanged_usa_matching_target(self):
        usa = region.load()
        self.assertEqual((usa.name, usa.serial, usa.matching, usa.pinned), ("ntsc-u", "SCUS_972.68", True, True))
        self.assertEqual(usa.target, json.loads((ROOT / "config/target.json").read_bytes()))
        self.assertEqual(usa.expected_levels, len(usa.overlay_pins()))
        self.assertEqual(region.matching().name, "ntsc-u")

    def test_aliases_and_environment_select_a_region(self):
        self.assertEqual(region.load("NTSC").name, "ntsc-u")
        self.assertEqual(region.load("europe").name, "pal")
        with mock.patch.dict(os.environ, {region.ENVIRONMENT: "pal"}):
            self.assertEqual(region.load().name, "pal")
            self.assertEqual(region.load("ntsc").name, "ntsc-u")

    def test_pal_is_registered_but_unpinned_and_not_matching(self):
        pal = region.load("pal")
        self.assertEqual(pal.serial, "SCES_516.07")
        self.assertFalse(pal.pinned)
        self.assertFalse(pal.matching)
        with self.assertRaisesRegex(ValueError, "--region pal --measure-identity"):
            pal.require_pinned()
        with self.assertRaisesRegex(ValueError, "not pinned"):
            pal.program_pins()
        with self.assertRaisesRegex(ValueError, "no C catalogues"):
            pal.require_matching("C integration")

    def test_serials_resolve_to_their_owner_only(self):
        self.assertEqual(region.by_serial("SCUS_972.68").name, "ntsc-u")
        self.assertEqual(region.by_serial("SCES_516.07").name, "pal")
        for unknown in ("SCUS_972.69", None, "SLUS_000.00"):
            with self.assertRaisesRegex(ValueError, "No registered region"):
                region.by_serial(unknown)
        with self.assertRaisesRegex(ValueError, "Unknown region"):
            region.load("ntsc-j")

    def test_usa_program_pins_cover_boot_and_every_overlay(self):
        pins = region.load().program_pins()
        self.assertEqual(pins["boot"], region.load().target["boot"]["sha256"])
        self.assertEqual(len(pins), 28)
        self.assertTrue(all(name == "boot" or name.startswith("levels/") for name in pins))

    def test_a_checkout_without_registry_has_only_the_usa_region(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config").mkdir()
            (root / "config/target.json").write_text(json.dumps({"serial": "SCUS_972.68", "boot": {"sha256": "a" * 64}}))
            (root / "config/overlays.json").write_text(json.dumps({"levels": [{"level": "1_oozla", "sha256": "b" * 64}]}))
            self.assertEqual(sorted(region.names(root)), ["ntsc", "ntsc-u"])
            self.assertEqual(region.by_serial("SCUS_972.68", root).program_pins(),
                             {"boot": "a" * 64, "levels/1_oozla": "b" * 64})
            with self.assertRaisesRegex(ValueError, "No registered region"):
                region.by_serial("SCES_516.07", root)

    def test_a_target_that_contradicts_its_registry_serial_is_refused(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config").mkdir()
            (root / "config/target.json").write_text(json.dumps({"serial": "SCES_516.07"}))
            (root / "config/overlays.json").write_text(json.dumps({"target": "SCUS_972.68", "levels": []}))
            with self.assertRaisesRegex(ValueError, "does not declare its serial"):
                region.load(None, root)

    def test_identity_paths_cannot_escape_the_repository(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            (root / "config").mkdir()
            registry = json.loads((ROOT / region.REGISTRY).read_bytes())
            registry["regions"]["pal"]["target"] = "../outside.json"
            (root / region.REGISTRY).write_text(json.dumps(registry))
            with self.assertRaisesRegex(ValueError, "escapes the repository"):
                region.load("pal", root)


class SetupMeasurementTests(unittest.TestCase):
    def test_unmeasured_identities_are_recorded_and_pinned_ones_compared(self):
        measured = {}
        pipeline_setup.check_or_measure({"size": 4, "sha256": "c" * 64}, None, "ISO", measured, "iso")
        pipeline_setup.check_or_measure("LABEL", None, "volume label", measured, "volume_label")
        self.assertEqual(measured, {"iso": {"size": 4, "sha256": "c" * 64}, "volume_label": "LABEL"})
        pipeline_setup.check_or_measure({"size": 4, "sha256": "c" * 64}, {"size": 4}, "ISO", measured, "iso")
        with self.assertRaisesRegex(ValueError, "ISO: wrong sha256"):
            pipeline_setup.check_or_measure({"sha256": "d" * 64}, {"sha256": "c" * 64}, "ISO", {}, "iso")
        with self.assertRaisesRegex(ValueError, "Wrong SYSTEM.CNF version"):
            pipeline_setup.check_or_measure("1.00", "1.01", "SYSTEM.CNF version", {}, "version")

    def test_an_unpinned_region_requires_explicit_measurement(self):
        with clean_environment(), tempfile.TemporaryDirectory() as temporary, \
                mock.patch.object(sys, "argv", ["setup.py", "--iso", "missing.iso", "--runtime", temporary,
                                                "--region", "pal"]):
            with self.assertRaisesRegex(ValueError, "--measure-identity"):
                pipeline_setup.main()
            self.assertEqual(list(Path(temporary).iterdir()), [])


class BuildRegionTests(unittest.TestCase):
    def run_build(self, manifest: dict, *extra: str) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "manifest.json"
            path.write_text(json.dumps(manifest), encoding="utf-8")
            versions = {"splat64": "0.50.0", "spimdisasm": "1.42.4", "rabbitizer": "1.16.2"}
            with clean_environment(), \
                    mock.patch.object(pipeline_build.importlib.metadata, "version", side_effect=versions.get), \
                    mock.patch.object(sys, "argv", ["build.py", "--manifest", str(path),
                                                    "--toolchain", str(Path(temporary) / "none"), *extra]):
                pipeline_build.main()

    def test_pal_manifest_cannot_request_c_integration(self):
        manifest = {"target": "SCES_516.07", "region": "pal", "pinned": False, "boot": {"sha256": "a" * 64},
                    "overlays": [], "level_wads": []}
        with self.assertRaisesRegex(ValueError, "C integration exists only for a matching region"):
            self.run_build(manifest, "--c-toolchain", "c")

    def test_pal_manifest_must_come_from_a_measurement_run(self):
        manifest = {"target": "SCES_516.07", "boot": {"sha256": "a" * 64}, "overlays": []}
        with self.assertRaisesRegex(ValueError, "not pinned yet"):
            self.run_build(manifest)

    def test_measured_pal_manifest_reaches_the_toolchain_check(self):
        manifest = {"target": "SCES_516.07", "region": "pal", "pinned": False, "boot": {"sha256": "a" * 64},
                    "overlays": [], "level_wads": []}
        stream = io.StringIO()
        with contextlib.redirect_stdout(stream), self.assertRaisesRegex(ValueError, "Missing instrument"):
            self.run_build(manifest)
        self.assertIn("round trip only", stream.getvalue())

    def test_explicit_region_must_agree_with_manifest(self):
        manifest = {"target": "SCUS_972.68", "boot": {"sha256": "a" * 64}, "overlays": []}
        with self.assertRaisesRegex(ValueError, "belongs to region ntsc-u, not pal"):
            self.run_build(manifest, "--region", "pal")

    def test_usa_manifest_still_requires_the_pinned_boot(self):
        manifest = {"target": "SCUS_972.68", "boot": {"sha256": "a" * 64}, "overlays": []}
        with self.assertRaisesRegex(ValueError, "not the pinned USA v1.01 baseline"):
            self.run_build(manifest)
        with self.assertRaisesRegex(ValueError, "Wrong manifest target"):
            self.run_build(dict(manifest, target="SLUS_000.00"))


class DoctorRegionTests(unittest.TestCase):
    def gather(self, argv):
        stream = io.StringIO()
        with clean_environment(), contextlib.redirect_stdout(stream), \
                mock.patch.object(doctor, "installed_version", side_effect=lambda package: doctor.pinned_versions(
                    doctor.ROOT / "requirements.txt").get(package)):
            code = doctor.doctor(argv)
        return code, stream.getvalue()

    def test_pal_reports_unmeasured_identity_and_names_itself_in_the_next_command(self):
        with tempfile.TemporaryDirectory() as temporary:
            iso = Path(temporary) / "pal.iso"
            iso.write_bytes(b"")
            code, output = self.gather(["--region", "pal", "--iso", str(iso), "--runtime", str(Path(temporary) / "rt")])
        self.assertEqual(code, 0)
        self.assertIn("SCES_516.07, region pal", output)
        self.assertIn("identity is unpinned", output)
        self.assertIn("C candidates (byte proofs)       no catalogues for this region", output)
        self.assertTrue(output.rstrip().splitlines()[-1].endswith("--region pal --measure-identity"))

    def test_default_region_output_is_unchanged(self):
        code, output = self.gather([])
        self.assertEqual(code, 0)
        self.assertIn("target            Ratchet & Clank: Going Commando USA v1.01 (SCUS_972.68, 27 levels)", output)
        self.assertNotIn("--region", output)


if __name__ == "__main__":
    unittest.main()
