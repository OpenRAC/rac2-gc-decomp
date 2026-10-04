import contextlib
import importlib.util
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
SPEC = importlib.util.spec_from_file_location("doctor", ROOT / "scripts" / "doctor.py")
doctor = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(doctor)


def touch(root, relative):
    path = root.joinpath(*relative.split("/"))
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b"")
    return path


def gather(argv):
    stream = io.StringIO()
    with contextlib.redirect_stdout(stream):
        code = doctor.doctor(argv)
    return code, stream.getvalue()


class ParsingTests(unittest.TestCase):
    def test_pinned_versions_drops_extras_and_comments(self):
        with tempfile.TemporaryDirectory() as name:
            requirements = Path(name) / "requirements.txt"
            requirements.write_text(
                "# a comment\nsplat64[mips]==0.50.0\nspimdisasm==1.42.4\nPyYAML==6.0.3  # inline\n\n",
                encoding="utf-8")
            pins = doctor.pinned_versions(requirements)
        self.assertEqual(pins["splat64"], "0.50.0")
        self.assertEqual(pins["spimdisasm"], "1.42.4")
        self.assertEqual(pins["PyYAML"], "6.0.3")
        self.assertNotIn("[mips]", pins)

    def test_pinned_versions_of_a_missing_file_is_empty(self):
        self.assertEqual(doctor.pinned_versions(Path("no-such-requirements.txt")), {})

    def test_missing_instruments_lists_every_absent_name(self):
        with tempfile.TemporaryDirectory() as name:
            root = Path(name)
            self.assertEqual(doctor.missing_instruments(root, doctor.C_INSTRUMENTS),
                             list(doctor.C_INSTRUMENTS))
            touch(root, "ee/bin/ld.exe")
            self.assertNotIn("ee/bin/ld.exe",
                             doctor.missing_instruments(root, doctor.C_INSTRUMENTS))

    def test_inside_repository_accepts_the_checkout_and_rejects_a_sibling(self):
        self.assertTrue(doctor.inside_repository(doctor.ROOT / "build"))
        with tempfile.TemporaryDirectory() as name:
            self.assertFalse(doctor.inside_repository(Path(name)))


class ManifestTests(unittest.TestCase):
    def test_no_runtime_has_no_manifest(self):
        self.assertIsNone(doctor.report_manifest([], None))

    def test_a_previous_run_is_found_next_to_latest_json(self):
        with tempfile.TemporaryDirectory() as name:
            runtime = Path(name)
            manifest = runtime / "runs" / "20261001T000000Z-abcdef01" / "manifest.json"
            manifest.parent.mkdir(parents=True)
            manifest.write_text("{}", encoding="utf-8")
            (runtime / "latest.json").write_text(json.dumps({"manifest": str(manifest)}), encoding="utf-8")
            lines = []
            self.assertEqual(doctor.report_manifest(lines, runtime), manifest)
            self.assertIn("disc need not be re-verified", "\n".join(lines))

    def test_a_dangling_manifest_is_reported_not_trusted(self):
        with tempfile.TemporaryDirectory() as name:
            runtime = Path(name)
            (runtime / "latest.json").write_text(
                json.dumps({"manifest": str(runtime / "gone" / "manifest.json")}), encoding="utf-8")
            lines = []
            self.assertIsNone(doctor.report_manifest(lines, runtime))
            self.assertIn("missing manifest", "\n".join(lines))


class DoctorTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.directory = Path(temporary.name)
        self.runtime = self.directory / "runtime"
        manifest = self.runtime / "runs" / "20261001T000000Z-abcdef01" / "manifest.json"
        manifest.parent.mkdir(parents=True)
        manifest.write_text("{}", encoding="utf-8")
        (self.runtime / "latest.json").write_text(json.dumps({"manifest": str(manifest)}), encoding="utf-8")
        self.manifest = manifest
        self.assembly = self.directory / "prodg2"
        for name in doctor.ASSEMBLY_INSTRUMENTS:
            touch(self.assembly, name)
        self.compiler = self.directory / "prodg3"
        for name in doctor.C_INSTRUMENTS:
            touch(self.compiler, name)
        self.wrench = self.directory / "wrenchbuild.exe"
        self.wrench.write_bytes(b"")

    def test_an_empty_environment_still_ends_with_a_command(self):
        # No mock here on purpose: on a bare interpreter (what CI is) the honest next command
        # is the pinned install, and on a prepared one it is setup.py. Both are commands.
        code, output = gather([])
        self.assertEqual(code, 0)
        self.assertIn("build (assembly reconstruction)  not yet", output)
        self.assertIn("C candidates (byte proofs)       not yet", output)
        last = output.rstrip().splitlines()[-1].strip()
        self.assertTrue(last.startswith(("python", "pip")), f"verdict must end with a command, got: {last}")

    def test_a_no_game_environment_points_to_required_setup(self):
        with mock.patch.object(doctor, "installed_version",
                               side_effect=lambda package: doctor.pinned_versions(
                                   doctor.ROOT / "requirements.txt").get(package)):
            code, output = gather([])
        self.assertEqual(code, 0)
        self.assertTrue(output.rstrip().splitlines()[-1].strip().startswith("python scripts/doctor.py --help"))

    def test_contributor_check_cannot_use_prepared_manifest_to_bypass_iso(self):
        with mock.patch.object(doctor, "tool_hashes", return_value=json.loads(
                (doctor.ROOT / "progress/candidates.json").read_bytes())["tools"]):
            code, output = gather(["--contributor-check", "--runtime", str(self.runtime),
                                   "--toolchain", str(self.assembly), "--c-toolchain", str(self.compiler)])
        self.assertEqual(code, 1)
        self.assertIn("contributor prerequisites        incomplete", output)
        self.assertIn("disc image        not checked", output)

    def test_reconstruction_files_with_wrong_hashes_fail_full_setup(self):
        lines = []
        self.assertFalse(doctor.report_assembly_hashes(lines, self.assembly))
        self.assertIn("hash mismatch", "\n".join(lines))

    def test_complete_contributor_presence_and_missing_emulator_are_distinct(self):
        ghidra = touch(self.directory, "ghidra/ghidraRun.bat")
        pcsx2 = touch(self.directory, "pcsx2/pcsx2-qt.exe")
        bios = touch(self.directory, "private/bios.bin")
        iso = touch(self.directory, "private/game.iso")
        args = ["--contributor-check", "--iso", str(iso), "--runtime", str(self.runtime),
                "--wrench", str(self.wrench), "--toolchain", str(self.assembly),
                "--c-toolchain", str(self.compiler), "--ghidra", str(ghidra),
                "--pcsx2", str(pcsx2), "--bios", str(bios)]
        hashes = json.loads((doctor.ROOT / "progress/candidates.json").read_bytes())["tools"]
        with mock.patch.object(doctor, "report_disc", return_value=True), \
                mock.patch.object(doctor, "report_assembly_hashes", return_value=True), \
                mock.patch.object(doctor, "tool_hashes", return_value=hashes), \
                mock.patch.object(doctor, "installed_version", side_effect=lambda name:
                                  doctor.pinned_versions(doctor.ROOT / "requirements.txt").get(name)):
            code, output = gather(args)
            self.assertEqual(code, 0)
            self.assertIn("contributor prerequisites        present", output)
            self.assertIn("require separate verification", output)
            # Put the missing path last so argparse's final occurrence is effective.
            code, output = gather([*args, "--pcsx2", str(self.directory / "missing.exe")])
            self.assertEqual(code, 1)
            self.assertIn("contributor prerequisites        incomplete", output)

    def test_a_complete_environment_reaches_the_candidates(self):
        with mock.patch.object(doctor, "installed_version",
                               side_effect=lambda package: doctor.pinned_versions(
                                   doctor.ROOT / "requirements.txt").get(package)), \
                mock.patch.object(doctor, "tool_hashes", return_value=json.loads(
                    (doctor.ROOT / "progress/candidates.json").read_bytes())["tools"]):
            code, output = gather(["--toolchain", str(self.assembly), "--c-toolchain", str(self.compiler),
                                   "--wrench", str(self.wrench), "--runtime", str(self.runtime)])
        self.assertEqual(code, 0)
        self.assertIn("build (assembly reconstruction)  yes", output)
        self.assertIn("C candidates (byte proofs)       yes", output)
        self.assertIn("check_candidates.py", output.rstrip().splitlines()[-1])
        self.assertIn(str(self.manifest.parent / "reference" / "boot.elf"), output)

    def test_linker_only_and_current_gnu_can_check_candidates_without_asm(self):
        hashes = json.loads((doctor.ROOT / "progress/candidates.json").read_bytes())["tools"]
        with mock.patch.object(doctor, "tool_hashes", return_value=hashes):
            code, output = gather(["--c-toolchain", str(self.compiler), "--runtime", str(self.runtime)])
        self.assertEqual(code, 0)
        self.assertIn("C candidates (byte proofs)       yes", output)
        self.assertIn("full C integration               not yet", output)
        self.assertIn("compilation not tested", output)

    def test_legacy_sn_frontends_without_gnu_are_not_c_ready(self):
        for relative in ("bin/ee-gcc2953.exe", "lib/gcc-lib/ee/2.95.3/cc1.exe"):
            touch(self.compiler, relative)
        with mock.patch.object(doctor, "tool_hashes", side_effect=OSError("WSL missing")):
            code, output = gather(["--c-toolchain", str(self.compiler), "--runtime", str(self.runtime)])
        self.assertEqual(code, 0)
        self.assertIn("C candidates (byte proofs)       not yet", output)
        self.assertIn("WSL missing", output)

    def test_wrong_or_incomplete_current_tool_hashes_are_rejected(self):
        expected = json.loads((doctor.ROOT / "progress/candidates.json").read_bytes())["tools"]
        for actual in ({**expected, "as": "0" * 64}, {"cc1": expected["cc1"]}):
            with mock.patch.object(doctor, "tool_hashes", return_value=actual):
                lines = []
                self.assertFalse(doctor.report_c_chain(lines, self.compiler))
                self.assertIn("hash mismatch", "\n".join(lines))

    def test_tool_probe_timeout_is_reported_without_c_readiness(self):
        with mock.patch.object(doctor, "tool_hashes", side_effect=doctor.subprocess.TimeoutExpired("wsl", 300)):
            self.assertFalse(doctor.report_c_chain([], self.compiler))

    def test_plain_doctor_does_not_probe_wsl(self):
        with mock.patch.object(doctor, "tool_hashes") as probe:
            gather([])
        probe.assert_not_called()

    def test_an_incomplete_toolchain_does_not_count_as_present(self):
        touch(self.directory / "half", "ee/bin/Ps2EeAs.exe")
        code, output = gather(["--toolchain", str(self.directory / "half"),
                               "--runtime", str(self.runtime)])
        self.assertEqual(code, 0)
        self.assertIn("is missing ee/bin/ld.exe", output)
        self.assertIn("not yet", output)

    def test_a_runtime_inside_the_repository_is_refused(self):
        code, output = gather(["--runtime", str(doctor.ROOT / "build")])
        self.assertEqual(code, 0)
        self.assertIn("choose a directory outside it", output)

    def test_a_wrong_package_version_asks_for_the_pinned_install(self):
        with mock.patch.object(doctor, "installed_version", return_value="0.0.1"):
            code, output = gather([])
        self.assertEqual(code, 0)
        self.assertIn("pip install -r requirements.txt", output.rstrip().splitlines()[-1])

    def test_a_missing_disc_image_is_reported(self):
        code, output = gather(["--iso", str(self.directory / "absent.iso")])
        self.assertEqual(code, 0)
        self.assertIn("absent.iso not found", output)

    def test_an_incomplete_checkout_exits_two(self):
        with mock.patch.object(doctor, "ROOT", self.directory):
            code, output = gather([])
        self.assertEqual(code, 2)
        self.assertIn("checkout is incomplete", output)


if __name__ == "__main__":
    unittest.main()
