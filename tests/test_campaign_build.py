"""Complete batches must retain failures and never promote a partial result."""
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import campaign_build


class CampaignBuildTests(unittest.TestCase):
    def test_vendored_source_changes_invalidate_batch_inputs(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            source = root / "src/libgcc/fp-bit-ee.c"
            source.parent.mkdir(parents=True)
            source.write_bytes(b"int library_helper(void) { return 1; }\n")
            generated = root / "candidates/boot.c"
            generated.parent.mkdir()
            generated.write_bytes(source.read_bytes())
            before = campaign_build.input_hashes(root)
            source.write_bytes(b"int library_helper(void) { return 2; }\n")
            after = campaign_build.input_hashes(root)
            self.assertNotEqual(before["src/libgcc/fp-bit-ee.c"],
                                after["src/libgcc/fp-bit-ee.c"])
            self.assertEqual(before["candidates/boot.c"], after["candidates/boot.c"])

    def test_region_policy_and_identity_changes_invalidate_provenance(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            identity = root / "config/regions/pal/target.json"
            identity.parent.mkdir(parents=True)
            identity.write_text('{"serial": "SCES_516.07"}')
            policy = root / "config/regions.json"
            policy.write_text('{"default": "ntsc-u"}')
            before = campaign_build.input_hashes(root)
            policy.write_text('{"default": "pal"}')
            changed_policy = campaign_build.input_hashes(root)
            self.assertNotEqual(before["config/regions.json"], changed_policy["config/regions.json"])
            identity.write_text('{"serial": "SCES_516.07", "expected_levels": 27}')
            changed_identity = campaign_build.input_hashes(root)
            self.assertNotEqual(changed_policy["config/regions/pal/target.json"],
                                changed_identity["config/regions/pal/target.json"])

    def batch(self, failing=False):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "repo"
            (root / "config").mkdir(parents=True)
            runtime = Path(temporary) / "runtime"
            runtime.mkdir()
            rows = [{"level": f"{i:02}_test", "sha256": "a" * 64,
                     "path": "unused"} for i in range(27)]
            (root / "config/overlays.json").write_text(json.dumps({"levels": rows}))
            manifest = runtime / "manifest.json"
            manifest.write_text(json.dumps({"target": campaign_build.TARGET["serial"],
                                            "boot": {"sha256": campaign_build.TARGET["boot"]["sha256"],
                                                     "path": "unused"}, "overlays": rows}))
            tools = Path(temporary) / "tools"
            (tools / "ee/bin").mkdir(parents=True)
            for name in ("Ps2EeAs.exe", "ld.exe"):
                (tools / "ee/bin" / name).write_bytes(b"synthetic instrument")
            observed = []

            def rebuild(*args, **kwargs):
                directory = args[2]
                observed.append(directory)
                if failing and directory.name == "04_test":
                    raise ValueError("synthetic byte mismatch")
                if directory.name == "boot":
                    return json.loads((Path(__file__).resolve().parents[1] / "progress/report.json").read_bytes())["g1"]
                return {"matched": True, "bytes_compared": 1}

            versions = {"splat64": "0.50.0", "spimdisasm": "1.42.4", "rabbitizer": "1.16.2"}
            with patch.object(campaign_build, "ROOT", root), \
                    patch.object(campaign_build.importlib.metadata, "version", side_effect=versions.get), \
                    patch.object(campaign_build, "rebuild", side_effect=rebuild):
                path, report = campaign_build.run_campaign(manifest, tools, tools, 4, 2)
                self.assertEqual(json.loads(path.read_bytes()), report)
                self.assertEqual(len(observed), 28)
                self.assertEqual(len(set(observed)), 28)
                second_path, _ = campaign_build.run_campaign(manifest, tools, tools, 2, 1)
                self.assertNotEqual(second_path.parent, path.parent)
                return report

    def test_complete_batch_passes_and_never_reuses_output(self):
        report = self.batch()
        self.assertTrue(report["matched"])
        self.assertEqual(len(report["g3"]), 27)

    def test_batch_boot_metadata_is_compatible_with_existing_exporter(self):
        import decomp_report
        root = Path(__file__).resolve().parents[1]
        read = lambda name: json.loads((root / name).read_bytes())
        report = self.batch()
        functions = decomp_report.validate_integration(read("progress/integration.json"),
                                                       read("config/target.json"), report)
        default = [row for row in functions if row.get("origin") != "boot-sdk"]
        sdk = [row for row in functions if row.get("origin") == "boot-sdk"]
        identity = lambda rows: {(row["symbol"], row["address"], row["size"]) for row in rows}
        self.assertEqual(identity(default), identity(read("config/candidate-catalog.json")["functions"]))
        if sdk:
            catalogs = [json.loads(path.read_bytes()) for path in
                        sorted((root / "config/boot-units").glob("*.json"))]
            expected_sdk = [row for catalog in catalogs for row in catalog["functions"]]
            self.assertEqual(identity(sdk), identity(expected_sdk))
            self.assertEqual(len(sdk), len(expected_sdk))
        self.assertEqual(len(functions), len(default) + len(sdk))

    def test_one_failure_retained_and_blocks_batch(self):
        report = self.batch(failing=True)
        self.assertFalse(report["matched"])
        self.assertEqual(report["failures"], [{"program": "04_test", "error": "synthetic byte mismatch"}])
        self.assertEqual(len(report["g3"]), 26)

    def test_duplicate_overlay_rejected(self):
        rows = [{"level": str(i), "sha256": "a" * 64} for i in range(27)]
        manifest = {"target": campaign_build.TARGET["serial"],
                    "boot": {"sha256": campaign_build.TARGET["boot"]["sha256"]},
                    "overlays": rows[:-1] + [rows[0]]}
        with self.assertRaisesRegex(ValueError, "exactly once"):
            campaign_build.validate_manifest(manifest, {"levels": rows})

    def test_unregenerated_authoritative_source_blocks_all_builds(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary) / "repo"
            (root / "config").mkdir(parents=True)
            (root / "config/source-layout.json").write_text("{}")
            with patch.object(campaign_build, "ROOT", root), \
                    patch("source_layout.verify", side_effect=ValueError("changed source fragment")) as verify, \
                    patch.object(campaign_build, "rebuild") as rebuild:
                with self.assertRaisesRegex(ValueError, "changed source fragment"):
                    campaign_build.run_campaign(Path(temporary) / "manifest.json", root, root, 4, 2)
                verify.assert_called_once_with(root, root)
                rebuild.assert_not_called()
