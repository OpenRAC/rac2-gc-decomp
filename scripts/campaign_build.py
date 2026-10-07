"""Build a complete C campaign with bounded program parallelism and fresh outputs."""
from __future__ import annotations

import argparse
import importlib.metadata
import json
import uuid
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timezone
from pathlib import Path

from build import ROOT, TARGET, rebuild
from check_candidates import file_hash
import region as regions


def input_hashes(root: Path) -> dict:
    paths = [root / name for name in (
        "config/target.json", "config/overlays.json", "config/regions.json", "config/candidate-catalog.json",
        "config/level-catalog.json", "config/source-layout.json", "progress/candidates.json",
        "scripts/build.py", "scripts/integration.py", "scripts/level_native.py",
        "scripts/check_candidates.py", "scripts/elf_tools.py", "scripts/wsl_chain.py",
        "scripts/campaign_build.py", "scripts/source_layout.py")]
    for directory, pattern in (("candidates", "*.c"), ("src", "*.cfrag"), ("src", "*.c"),
                               ("config/level-native", "*.json"), ("config/regions", "*.json"),
                               ("progress/level-candidates", "*.json"), ("config/boot-units", "*.json"),
                               ("progress/boot-units", "*.json"), ("config/compiler-profiles", "*.json"),
                               ("progress/compiler-profiles", "*.json")):
        paths.extend((root / directory).rglob(pattern))
    paths.extend((root / "scripts").rglob("*.py"))
    return {path.relative_to(root).as_posix(): file_hash(path)
            for path in sorted(set(paths)) if path.is_file()}


def validate_manifest(manifest: dict, baseline: dict) -> None:
    if manifest.get("target") != TARGET["serial"]:
        try:
            owner = regions.by_serial(manifest.get("target"), ROOT)
        except ValueError:
            owner = None
        if owner is not None and not owner.matching:
            owner.require_matching("A C campaign")
    if (manifest.get("target") != TARGET["serial"]
            or manifest.get("boot", {}).get("sha256") != TARGET["boot"]["sha256"]):
        raise ValueError("Wrong campaign boot identity")
    expected = {row["level"]: row["sha256"] for row in baseline["levels"]}
    actual = {row["level"]: row["sha256"] for row in manifest["overlays"]}
    count = TARGET["expected_levels"]
    if len(actual) != count or len(manifest["overlays"]) != count or actual != expected:
        raise ValueError(f"Campaign requires all {count} pinned overlays exactly once")


def run_campaign(manifest_path: Path, toolchain: Path, c_toolchain: Path,
                 program_jobs: int, jobs: int, candidate_review: Path | None = None,
                 batch_id: str | None = None, sdk_binding: Path | None = None) -> tuple[Path, dict]:
    if program_jobs < 1 or jobs < 1:
        raise ValueError("Parallelism must be positive")
    if (ROOT / "config/source-layout.json").exists():
        from source_layout import verify
        verify(ROOT, ROOT)
    manifest_path = manifest_path.resolve()
    if ROOT == manifest_path.parent or ROOT in manifest_path.parents:
        raise ValueError("Campaign manifest and artifacts must stay outside the repository")
    manifest = json.loads(manifest_path.read_bytes())
    validate_manifest(manifest, json.loads((ROOT / "config/overlays.json").read_bytes()))
    packages = {"splat64": "0.50.0", "spimdisasm": "1.42.4", "rabbitizer": "1.16.2"}
    for name, version in packages.items():
        if importlib.metadata.version(name) != version:
            raise ValueError(f"Use pinned {name} {version}")
    toolchain, c_toolchain = toolchain.resolve(), c_toolchain.resolve()
    instruments = [toolchain / "ee/bin" / name for name in ("Ps2EeAs.exe", "ld.exe")]
    if any(not path.is_file() for path in instruments):
        raise ValueError("Missing reconstruction instruments")
    # Every invocation creates a fresh batch; no existing result is skipped.
    batch_id = batch_id or uuid.uuid4().hex
    if len(batch_id) != 32 or any(c not in "0123456789abcdef" for c in batch_id):
        raise ValueError("Batch id must be a fresh UUID hex identifier")
    directory = manifest_path.parent / "builds" / ("campaign-" + batch_id)
    directory.mkdir(parents=True, exist_ok=False)
    report = {"schema": 1, "batch_id": batch_id, "target": TARGET["serial"],
              "verified_at": datetime.now(timezone.utc).isoformat(),
              "manifest_sha256": file_hash(manifest_path), "packages": packages,
              "program_jobs": program_jobs, "assembler_jobs_per_program": jobs,
              "tools": {path.name: file_hash(path) for path in instruments},
              "g1": None, "g3": [], "failures": [], "matched": False}
    report["input_sha256"] = input_hashes(ROOT)
    sdk_binding_sha256 = file_hash(sdk_binding) if sdk_binding is not None else None
    report["sdk_binding_sha256"] = sdk_binding_sha256
    programs = [("boot", manifest["boot"])] + [
        (row["level"], row) for row in manifest["overlays"]]

    def build_one(name: str, row: dict) -> dict:
        result = rebuild(Path(row["path"]), row["sha256"], directory / name,
                         toolchain, jobs, "boot" if name == "boot" else "overlay",
                         c_toolchain, level=None if name == "boot" else name,
                         candidate_review=candidate_review, sdk_binding=sdk_binding if name == "boot" else None)
        result["program"] = name
        if name != "boot":
            result["level"] = name
        return result

    with ThreadPoolExecutor(max_workers=program_jobs) as pool:
        futures = {pool.submit(build_one, name, row): name for name, row in programs}
        for future in as_completed(futures):
            name = futures[future]
            try:
                result = future.result()
            except Exception as error:
                report["failures"].append({"program": name, "error": str(error)})
            else:
                if name == "boot":
                    report["g1"] = result
                else:
                    report["g3"].append(result)
    report["g3"].sort(key=lambda row: row["level"])
    if sdk_binding is not None and file_hash(sdk_binding) != sdk_binding_sha256:
        report["failures"].append({"program": "boot-sdk", "error": "Private SDK binding changed during batch"})
    if input_hashes(ROOT) != report["input_sha256"]:
        report["failures"].append({"program": "batch", "error": "Campaign inputs changed during the build"})
    report["failures"].sort(key=lambda row: row["program"])
    # The legacy exporter defines these counters on the boot proof, then adds
    # overlay coverage from its separate per-program integration proofs.
    boot_count = (report["g1"] or {}).get("integrated_c_functions", 0)
    report["integrated_functions"] = boot_count
    report["decompiled_functions"] = boot_count
    report["matched"] = (not report["failures"] and report["g1"] is not None
                         and report["g1"].get("matched") is True
                         and len(report["g3"]) == TARGET["expected_levels"]
                         and all(row.get("matched") is True for row in report["g3"]))
    path = directory / "report.json"
    path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8", newline="")
    return path, report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--toolchain", required=True, type=Path)
    parser.add_argument("--c-toolchain", required=True, type=Path)
    parser.add_argument("--candidate-review", type=Path)
    parser.add_argument("--sdk-binding", type=Path, help="private owned SDK paths, required only for an admitted boot SDK unit")
    parser.add_argument("--program-jobs", type=int, default=4)
    parser.add_argument("--jobs", type=int, default=2)
    parser.add_argument("--batch-id", help="fresh action UUID supplied by the campaign facade")
    args = parser.parse_args()
    path, report = run_campaign(args.manifest, args.toolchain, args.c_toolchain,
                                args.program_jobs, args.jobs, args.candidate_review, args.batch_id, args.sdk_binding)
    print(json.dumps({"report": str(path), "matched": report["matched"],
                      "failures": report["failures"]}))
    return 0 if report["matched"] else 1


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print(f"Campaign preparation failed: {error}")
        raise SystemExit(2)
