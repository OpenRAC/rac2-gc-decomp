"""One producer for current CI metadata checks and public report artifacts.

Source mode is limited pre-queue metadata validation, never a full-suite receipt.
Full mode keeps the actual expensive checks once. Legal-reference raw gates
remain the maintained local workflow's responsibility.
"""
import argparse
import gzip
import hashlib
import json
import os
from pathlib import Path
import subprocess
import io
import sys
import sysconfig
import importlib.metadata
import re
import tempfile

ROOT = Path(__file__).resolve().parents[1]
FILES = {
    "report.json": "build/decomp/report.json",
    "source-modules.json": "build/decomp/source-modules.json",
    "source-inventory.json": "progress/source-inventory.json",
    "unique-report.json": "build/decomp/unique-report.json",
    "unique-summary.json": "build/decomp/unique-summary.json",
    "code-reuse-report.json": "progress/code-reuse-report.json",
    "code-reuse-families.json.gz": "progress/code-reuse-families.json.gz",
    "objdiff-validation.json": "build/decomp/objdiff-validation.json",
    "unique-objdiff-validation.json": "build/decomp/unique-objdiff-validation.json",
}
CHECKS = ["source-layout-full", "physical-proofs", "unique", "reuse", "objdiff-physical", "objdiff-unique"]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def contained(repo, relative):
    path = repo / relative
    if path.is_symlink() or not path.resolve().is_relative_to(repo.resolve()) or path.resolve() == repo.resolve():
        raise ValueError("CI input/output leaves repository")
    return path


def checkout_guard(repo):
    expected = os.environ.get("GITHUB_SHA", "")
    if not re.fullmatch(r"[0-9a-f]{40}", expected):
        raise ValueError("Exact announced CI commit is missing")
    head = subprocess.check_output(["git", "-C", str(repo), "rev-parse", "--verify", "HEAD"]).decode().strip()
    if head != expected:
        raise ValueError("CI checkout differs from announced commit")
    for args in (("diff", "--quiet", "HEAD", "--"), ("diff", "--quiet", "--cached", "HEAD", "--")):
        result = subprocess.run(["git", "-C", str(repo), *args], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if result.returncode != 0:
            raise ValueError("CI tracked worktree or index changed")


def inputs(repo):
    checkout_guard(repo)
    names = subprocess.check_output(["git", "-C", str(repo), "ls-files", "-z"]).decode().split("\0")
    if not any(names):
        raise ValueError("Tracked CI input closure is empty")
    return {name: sha(contained(repo, name).read_bytes()) for name in sorted(names) if name}


def tools(objdiff):
    result = {"python-executable": sha(Path(sys.executable).read_bytes()),
              "objdiff-cli": sha(Path(objdiff).read_bytes())}
    stdlib = Path(sysconfig.get_path("stdlib"))
    for path in sorted(stdlib.rglob("*")):
        relative = path.relative_to(stdlib)
        if "site-packages" not in relative.parts and path.is_file() and path.suffix in (".py", ".so", ".pyd"):
            result["stdlib/" + relative.as_posix()] = sha(path.read_bytes())
    library = sysconfig.get_config_var("LDLIBRARY")
    if library:
        path = Path(sys.base_prefix) / "lib" / library
        if path.is_file():
            result["python-library"] = sha(path.read_bytes())
    package = importlib.metadata.distribution("rabbitizer")
    if package.version != "1.16.2":
        raise ValueError("Decoder version differs")
    for relative in package.files or []:
        path = Path(package.locate_file(relative))
        if path.is_file() and path.suffix in (".py", ".so", ".pyd"):
            result["rabbitizer/" + str(relative).replace("\\", "/")] = sha(path.read_bytes())
    if not any(k.startswith("rabbitizer/") for k in result):
        raise ValueError("Decoder byte closure is missing")
    return result


def source_metadata(repo):
    """Read current pinned source and render exact recipes, without family scan."""
    import source_layout as layout
    manifest_path = repo / "config/source-layout.json"
    raw = manifest_path.read_bytes()
    manifest = json.loads(raw)
    if (Path(layout.__file__).resolve() != (repo / "scripts/source_layout.py").resolve()
            or manifest["generator_sha256"] != sha((repo / "scripts/source_layout.py").read_bytes())):
        raise ValueError("Source generator freshness changed")
    for relative, expected in manifest["input_sha256"].items():
        if sha(contained(repo, relative).read_bytes()) != expected:
            raise ValueError("Pinned source input changed")
    boot, native, required = layout.load_inputs(repo)
    smalldata = layout.load_smalldata(repo, required)
    from boot_sdk_unit import admitted_units, load_catalog
    sdk = [load_catalog(repo, unit) for unit in admitted_units(repo)]
    expected_sources = {"candidates/boot.c"} | {c["source"] for _, c in native + smalldata} | {c["source"] for c in sdk}
    if any(manifest["input_sha256"].get(name) != value for name, value in required.items()):
        raise ValueError("Current catalogue is absent from source input closure")
    generated, _ = layout.render(repo, manifest, enforce_hashes=True)
    if set(generated) != expected_sources or any(data != contained(repo, name).read_bytes() for name, data in generated.items()):
        raise ValueError("Actual standalone source differs from current recipes")
    inventory = {"schema": 1, "kind": "authored-source-inventory", "source_layout_sha256": sha(raw),
                 "metrics": manifest["metrics"], "integration_credit_added": 0}
    encoded = (json.dumps(inventory, indent=2, sort_keys=True) + "\n").encode()
    if encoded != (repo / "progress/source-inventory.json").read_bytes():
        raise ValueError("Source inventory metadata differs")
    return {"byte_identical_units": len(generated), "scope": "current hashes/render; no fresh family classification"}


def command(repo, *args):
    subprocess.run(args, cwd=repo, check=True)


def physical_display(repo, physical):
    import readme_progress as display
    read = lambda path: json.loads((repo / path).read_bytes())
    levels = [read("progress/levels/" + row["level"] + ".json") for row in read("config/overlays.json")["levels"]]
    native = [f for proof in levels for f in proof["functions"] if f.get("origin") in ("level-native", "level-smalldata")]
    svg = display.render(int(physical["measures"]["matchedCode"]), int(physical["measures"]["totalCode"])).encode()
    text = (repo / "README.md").read_text(encoding="utf8")
    table = display.render_table(physical, native, read("progress/report.json")["verified_at"])
    if svg != (repo / "progress/decompilation.svg").read_bytes() or display.update_readme(text, table) != text:
        raise ValueError("Physical display differs from current validated report")


def unique_and_reuse(repo, output):
    # Shared maintained API inputs: unique.generate runs exactly once, and
    # code_reuse.generate receives that validated primary rather than rerunning it.
    import unique_code_report as uq
    import code_reuse_report as reuse
    path = repo / "config/function-catalog/catalog.json"
    catalog, raw = uq.read_catalog(path)
    credit = uq.load_current_credit(repo, catalog)
    primary = uq.generate(catalog, credit, uq.current_boot_binding(repo, catalog))
    primary["catalog_sha256"] = uq.digest(raw)
    summary = {k: v for k, v in primary.items() if k != "groups"}
    summary_bytes = uq.encoded(summary)
    if summary_bytes != (repo / "progress/unique-code-report.json").read_bytes():
        raise ValueError("Current unique serialization differs")
    (output / "unique-summary.json").write_bytes(summary_bytes)
    (output / "unique-report.json").write_bytes(uq.encoded(uq.objdiff(primary)))
    report, details = reuse.generate(catalog, credit, primary)
    authored, authored_details = reuse.authored_subset(repo, credit)
    report.update(authored_C_reuse_subset=authored, catalog_sha256=reuse.digest(raw),
                  input_sha256=reuse.snapshot(repo, path, catalog))
    payload = reuse.encoded({"schema": 1, "policy": reuse.POLICY, "template_families": details,
                             "authored_C_fragment_families": authored_details})
    buffer = io.BytesIO()
    with gzip.GzipFile(fileobj=buffer, mode="wb", filename="", mtime=0) as stream:
        stream.write(payload)
    families = buffer.getvalue()
    report["families_sha256"] = reuse.digest(families)
    if (reuse.encoded(report) != (repo / "progress/code-reuse-report.json").read_bytes()
            or families != (repo / "progress/code-reuse-families.json.gz").read_bytes()):
        raise ValueError("Current reusable-code serialization differs")


def produce(repo, mode, objdiff=None):
    before = inputs(repo)
    source = source_metadata(repo)
    from progress_module_report import verified_report
    physical = verified_report(repo)  # Original generate + validate_object_proof once.
    physical_display(repo, physical)
    with tempfile.TemporaryDirectory(prefix="rac2-ci-views-", dir=os.environ.get("RUNNER_TEMP")) as directory:
        runtime = Path(directory).resolve()
        canonical = repo.resolve()
        if runtime == canonical or runtime.is_relative_to(canonical) or canonical.is_relative_to(runtime):
            raise ValueError("Campaign runtime must remain outside the repository")
        command(repo, sys.executable, "scripts/campaign.py", "--runtime", str(runtime), "views", "--check")
    if mode == "source":
        if inputs(repo) != before:
            raise ValueError("Source metadata changed during precheck")
        return {"mode": "source", "source": source, "physical": physical["measures"],
                "full_validation": False, "artifacts_published": False}
    if mode != "full" or objdiff is None:
        raise ValueError("Full export requires pinned objdiff")
    if sha(Path(objdiff).read_bytes()) != "c8290281e82114bcc1a06ff73061110d3902a177822e750337de2537188e358f":
        raise ValueError("Objdiff executable pin differs")
    tool_pins = tools(objdiff)
    import source_layout as layout
    layout.verify(repo, repo)  # Full family/source check, not source-mode shortcut.
    from progress_modules import group_report
    grouped, summary = group_report(physical, repo)
    output = repo / "build/decomp"
    output.mkdir(parents=True, exist_ok=True)
    for name, value in (("report.json", grouped), ("source-modules.json", summary)):
        (output / name).write_bytes((json.dumps(value, indent=2) + "\n").encode())
    command(repo, str(objdiff), "report", "changes", str(output / "report.json"), str(output / "report.json"),
            "-o", str(output / "objdiff-validation.json"))
    unique_and_reuse(repo, output)
    command(repo, sys.executable, "scripts/readme_unique_progress.py", "--catalogue-report", "progress/unique-code-report.json",
            "--physical-report", "build/decomp/report.json", "--check")
    from decomp_report import measures
    (output / "empty.json").write_bytes(json.dumps({"version": 2, "measures": measures(0, 0, 0),
                                                  "units": [], "categories": []}).encode())
    command(repo, str(objdiff), "report", "changes", str(output / "empty.json"), str(output / "unique-report.json"),
            "-o", str(output / "unique-objdiff-validation.json"))
    if inputs(repo) != before or tools(objdiff) != tool_pins:
        raise ValueError("CI inputs changed during full export")
    receipt = {"schema": 1, "kind": "verified-ci-full-export", "mode": "full", "checks": CHECKS,
               "commit_sha": os.environ["GITHUB_SHA"], "event": os.environ["GITHUB_EVENT_NAME"],
               "run_id": int(os.environ["GITHUB_RUN_ID"]), "run_attempt": int(os.environ["GITHUB_RUN_ATTEMPT"]),
               "repository": "OpenRAC/rac2-gc-decomp", "repository_id": 1400228215,
               "workflow_path": ".github/workflows/tests.yml", "workflow_sha256": before[".github/workflows/tests.yml"],
               "exporter_sha256": before["scripts/ci_export.py"], "inputs": before,
               "tools": tool_pins,
               "outputs": {name: sha(contained(repo, path).read_bytes()) for name, path in FILES.items()}}
    bundle = output / "verified-export"
    bundle.mkdir(exist_ok=True)
    for name, path in FILES.items():
        (bundle / name).write_bytes(contained(repo, path).read_bytes())
    (bundle / "receipt.json").write_bytes((json.dumps(receipt, sort_keys=True) + "\n").encode())
    return {"mode": "full", "checks": CHECKS, "full_validation": True, "bundle": str(bundle)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mode", choices=("source", "full"), required=True)
    parser.add_argument("--objdiff", type=Path)
    args = parser.parse_args()
    try:
        print(json.dumps(produce(ROOT, args.mode, args.objdiff), sort_keys=True))
    except Exception:
        print("CI export failed: validation", flush=True)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
