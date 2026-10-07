"""Prepare and guardedly publish one complete, registered campaign action.

All expensive work happens in a private source snapshot.  Publication replaces
only structural proof metadata, generated displays and the campaign register.
No compiler, matching search, Git mutation or progress-credit calculation lives
here: the maintained validators and exporters remain the authority.
"""
from __future__ import annotations

import copy
import ctypes
import difflib
import gzip
import hashlib
import io
import json
import os
from pathlib import Path, PurePosixPath
import re
import subprocess
import sys
import uuid


PRIVATE_EXPORTS = {"progress/objdiff.json", "progress/unique-objdiff.json"}
# Historical verification metadata is snapshotted and guarded, never regenerated
# or admitted to the publication allowlist by the closing command.
SNAPSHOT_VERIFICATION_INPUTS = {
    "progress/verification/data-gp-audit.json.gz",
    "progress/verification/data-gp-witnesses.json.gz",
    "progress/verification/guarded-switch-audit.json.gz",
}
FIXED_OUTPUTS = {
    "README.md", "config/campaign-register.json", "progress/integration.json",
    "progress/report.json", "progress/decompilation.svg", "progress/source-inventory.json",
    "progress/unique-code-report.json", "progress/paired-code-metrics.json",
    "progress/unique-decompilation.svg", "progress/code-reuse-report.json",
    "progress/code-reuse-families.json.gz", "docs/CAMPAIGN-QUEUE.md",
    "docs/C-NATIVE-EXPERIMENT-REGISTER.md",
    "config/function-evidence/pointer-arguments.json.gz",
    "config/function-evidence/boot-bindings.json.gz",
}
PRIVATE_PATH = re.compile(r"(?<![A-Za-z0-9_./-])(?:[A-Za-z]:[\\/]|/(?:home|root|Users|mnt|tmp|private)/|\\\\[A-Za-z0-9])")
FORBIDDEN_KEY = re.compile(r"^(?:access_token|refresh_token|api_key|password|cookie|authorization|private_key)$", re.I)
SECRET_VALUE = re.compile(r"(?:ghp_[A-Za-z0-9]{20,}|github_pat_[A-Za-z0-9_]{20,}|-----BEGIN (?:RSA |EC |OPENSSH |DSA |ENCRYPTED )?PRIVATE KEY-----)")
MAX_METADATA = 256 * 1024 * 1024


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _read(path):
    return json.loads(Path(path).read_bytes())


def _encoded(value):
    return (json.dumps(value, sort_keys=True, indent=2, ensure_ascii=False) + "\n").encode("utf8")


def _new(path, data):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("xb") as stream:
        stream.write(data)


def _atomic(path, data, failure_bank=None):
    """Use a sibling temporary and retain a complete previous file on failure."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".finalize-" + uuid.uuid4().hex + ".tmp")
    _new(temporary, data)
    try:
        os.replace(temporary, path)
    except BaseException as error:
        if failure_bank is not None and temporary.exists():
            quarantine = Path(failure_bank) / temporary.name
            quarantine.parent.mkdir(parents=True, exist_ok=True)
            try:
                os.replace(temporary, quarantine)
            except OSError:
                error.add_note("Atomic replacement temporary retained for inspection: " + str(temporary))
        raise


def _private(path, repo):
    path, repo = Path(path).resolve(), Path(repo).resolve()
    if path == repo or path.is_relative_to(repo) or repo.is_relative_to(path):
        raise ValueError("Finalization runtime inputs/output must be outside the repository")
    return path


def _private_output(path, repo):
    path = _private(path, repo)
    if any((parent / ".git").exists() for parent in (path, *path.parents)):
        raise ValueError("Finalization output must be outside every Git checkout/worktree")
    return path


def _relative(name):
    value = PurePosixPath(name)
    if (not isinstance(name, str) or value.is_absolute() or ".." in value.parts
            or ":" in name or "\\" in name or str(value) != name):
        raise ValueError("Unsafe repository metadata path")
    return name


def _path(repo, name):
    name = _relative(name)
    path = repo / name
    if path.is_symlink() or any(p.is_symlink() for p in path.parents if p != repo.parent):
        raise ValueError("Symlinked publication or snapshot path")
    if not path.resolve().is_relative_to(repo):
        raise ValueError("Repository path escaped its root")
    return path


def _public_metadata(name, data, baseline=None):
    """Reject paths, secrets and binary payloads even in compressed chunks."""
    if name.endswith(".gz"):
        with gzip.GzipFile(fileobj=io.BytesIO(data)) as stream:
            data = stream.read(MAX_METADATA + 1)
    if len(data) > MAX_METADATA:
        raise ValueError("Public metadata exceeds the size limit")
    text = data.decode("utf8")
    if "\r" in text or "\x00" in text:
        raise ValueError("Public output contains binary bytes or non-LF text: " + name)
    if SECRET_VALUE.search(text):
        raise ValueError("Public output contains a secret value: " + name)
    document, legacy = None, None
    def visit(value):
        if isinstance(value, dict):
            for key, child in value.items():
                if value is document and key == "legacy_documents" and legacy is not None:
                    continue
                if FORBIDDEN_KEY.fullmatch(key):
                    raise ValueError("Public output contains a credential field: " + name)
                visit(child)
        elif isinstance(value, list):
            for child in value:
                visit(child)
        elif isinstance(value, str):
            if PRIVATE_PATH.search(value):
                raise ValueError("Public output contains private paths: " + name)
            if SECRET_VALUE.search(value):
                raise ValueError("Public output contains a secret value: " + name)
    if name.endswith((".json", ".json.gz")):
        document = json.loads(text)
        # Immutable imported history may quote the old private work-root convention.
        # Preserve it verbatim; it never authorizes new paths in tasks or proofs.
        if name == "config/campaign-register.json" and baseline is not None:
            legacy = json.loads(baseline).get("legacy_documents")
            if document.get("legacy_documents") != legacy:
                raise ValueError("Finalization modified immutable imported history")
        visit(document)
    elif name.endswith(".ndjson.gz"):
        if PRIVATE_PATH.search(text):
            raise ValueError("Public output contains private paths: " + name)
        for line in text.splitlines():
            visit(json.loads(line))
    elif PRIVATE_PATH.search(text):
        raise ValueError("Public output contains private paths: " + name)
    return data


def _inventory(repo):
    """Copy only source/documentation/structural metadata; never runtime trees."""
    result = {}
    patterns = {"scripts": {".py"}, "tests": {".py"}, "src": {".c", ".h", ".cfrag"},
                "candidates": {".c"}, "config": {".json", ".gz"},
                "progress": {".json", ".gz", ".md", ".svg"}, "docs": {".md"}}
    for directory, suffixes in patterns.items():
        for path in sorted((repo / directory).rglob("*")):
            name = path.relative_to(repo).as_posix()
            if "__pycache__" in path.parts:
                continue
            if path.is_symlink():
                raise ValueError("Symlink in source snapshot: " + name)
            if not path.is_file() or path.suffix not in suffixes or name in PRIVATE_EXPORTS:
                continue
            if (path.suffix == ".gz" and name not in SNAPSHOT_VERIFICATION_INPUTS
                    and not name.startswith(("config/function-catalog/", "config/function-evidence/", "progress/code-reuse-families"))):
                raise ValueError("Unknown compressed input in source snapshot: " + name)
            result[name] = _sha(_path(repo, name).read_bytes())
    for name in ("README.md", "requirements.txt"):
        if (repo / name).exists():
            result[name] = _sha(_path(repo, name).read_bytes())
    return result


def _pin(path, pins):
    path = Path(path).resolve()
    value = _sha(path.read_bytes())
    if str(path) in pins and pins[str(path)] != value:
        raise ValueError("Private evidence changed during preparation")
    pins[str(path)] = value
    return value


def _argument(command, option, required=True):
    positions = [i for i, value in enumerate(command) if value == option]
    if len(positions) > 1 or (positions and positions[0] + 1 >= len(command)):
        raise ValueError("Ambiguous recorded action option: " + option)
    if not positions:
        if required:
            raise ValueError("Missing recorded action option: " + option)
        return None
    return command[positions[0] + 1]


def _sdk_owners(integration, repo):
    """Use the maintained keyed SDK ownership contract, including admission."""
    from boot_sdk_unit import admitted_units, descriptor, fields
    owners = fields(integration.get("sdk_units", {}), admitted_units(repo))
    for identifier, owner in owners.items():
        descriptor(owner)
        if owner["unit_id"] != identifier:
            raise ValueError("SDK owner map key/descriptor mismatch")
    return owners


def _preflight(store, repo, action_id, manifest, references, tasks):
    """Bind immutable facade outcome, source inputs and all 28 program artifacts."""
    import campaign
    if not re.fullmatch(r"[0-9a-f]{32}", action_id):
        raise ValueError("Finalization needs a registered campaign UUID")
    if store.path.resolve() != repo / "config/campaign-register.json":
        raise ValueError("Finalization publishes the repository's canonical register only")
    registry = store.load()
    action = registry["actions"].get(action_id, {})
    if action.get("kind") not in {"build", "integrate"} or action.get("state") != "passed":
        raise ValueError("Finalization needs a registered passed build/integrate action")
    if action.get("directory") != "runtime:actions/" + action_id:
        raise ValueError("Action directory identity mismatch")
    work = _private(store.runtime / "actions" / action_id, repo)
    pins = {}
    for name in ("manifest.json", "outcome.json"):
        key = "action_manifest_sha256" if name == "manifest.json" else "action_outcome_sha256"
        if action.get(key) != _pin(work / name, pins):
            raise ValueError("Action lacks a matching registered immutable " + name + " SHA256")
    invocation, outcome = _read(work / "manifest.json"), _read(work / "outcome.json")
    if (invocation.get("id") != action_id or invocation.get("kind") != action["kind"]
            or outcome.get("id") != action_id or outcome.get("state") != "passed"
            or outcome.get("returncode") != 0 or outcome.get("artifact_sha256") != action.get("artifact_sha256")):
        raise ValueError("Registered action and immutable outcome disagree")
    if any(action.get(key) != value for key, value in outcome.items()
           if key not in {"artifact", "directory"}):
        raise ValueError("Registered action outcome fields differ from its immutable receipt")
    command = invocation["command"]
    manifest = _private(manifest, repo)
    if Path(_argument(command, "--manifest")).resolve() != manifest:
        raise ValueError("Explicit manifest does not belong to this action")
    if _argument(command, "--batch-id") != action_id:
        raise ValueError("Action invocation has a different batch UUID")
    for path, digest in invocation["external_input_sha256"].items():
        if _pin(_private(path, repo), pins) != digest:
            raise ValueError("Action external input is stale")
    if str(manifest) not in invocation["external_input_sha256"]:
        raise ValueError("Action does not pin its reference manifest")
    before = campaign.instrument_hashes(repo)
    if before != invocation["instruments"]:
        raise ValueError("Source, profile, catalog or checker changed after the action")
    artifact = _private(outcome["artifact"], repo)
    if artifact.name != "report.json" or artifact.parent.name != "campaign-" + action_id:
        raise ValueError("Gate artifact is outside its UUID batch")
    if _pin(artifact, pins) != outcome["artifact_sha256"]:
        raise ValueError("Gate artifact hash differs from immutable outcome")
    gate = _read(artifact)
    overlays = _read(repo / "config/overlays.json")["levels"]
    target = _read(repo / "config/target.json")
    expected = {row["level"]: row["sha256"] for row in overlays}
    levels = gate.get("g3", [])
    if (len(expected) != 27 or len(overlays) != 27 or len(levels) != 27
            or {row.get("level") for row in levels} != set(expected)
            or gate.get("batch_id") != action_id or gate.get("target") != target["serial"]
            or gate.get("matched") is not True or gate.get("failures") != []
            or not isinstance(gate.get("g1"), dict) or gate["g1"].get("matched") is not True
            or any(row.get("matched") is not True for row in levels)):
        raise ValueError("Incomplete or failed boot and 27-overlay gate batch")
    required = {"scripts/check_candidates.py", "scripts/wsl_chain.py", "config/candidate-catalog.json", "candidates/boot.c"}
    gate_inputs = gate.get("input_sha256", {})
    if not required.issubset(gate_inputs) or any(before.get(name) != pin for name, pin in gate_inputs.items()):
        raise ValueError("Gate inputs differ from current action source/tool/profile inputs")
    if gate.get("manifest_sha256") != pins[str(manifest)]:
        raise ValueError("Gate reference manifest pin mismatch")
    toolchain = _private(_argument(command, "--toolchain"), repo)
    for name in ("Ps2EeAs.exe", "ld.exe"):
        if gate.get("tools", {}).get(name) != _pin(toolchain / "ee/bin" / name, pins):
            raise ValueError("Reconstruction instrument changed")
    sdk = _argument(command, "--sdk-binding", False)
    if sdk and gate.get("sdk_binding_sha256") != _pin(_private(sdk, repo), pins):
        raise ValueError("SDK binding changed after action")
    reference_manifest = _read(manifest)
    actual = reference_manifest.get("overlays", [])
    if (reference_manifest.get("target") != target["serial"]
            or reference_manifest.get("boot", {}).get("sha256") != target["boot"]["sha256"]
            or len(actual) != 27 or {row["level"]: row["sha256"] for row in actual} != expected):
        raise ValueError("Manifest does not pin all current programs")
    references = _private(references or Path(reference_manifest["boot"]["path"]).parent, repo)
    program_rows = [("boot", reference_manifest["boot"], gate["g1"])] + [
        (row["level"], row, next(g for g in levels if g["level"] == row["level"])) for row in actual]
    for program, reference, measured in program_rows:
        reference_path = _private(reference["path"], repo)
        conventional = references / ("boot.elf" if program == "boot" else "levels/" + program + "/overlay.elf")
        if _pin(reference_path, pins) != reference["sha256"] or _pin(conventional, pins) != reference["sha256"]:
            raise ValueError("Reference files differ from pinned manifest")
        directory = artifact.parent / program
        for filename in ("integration.json", "gate.json"):
            _pin(directory / filename, pins)
        local_gate, integration = _read(directory / "gate.json"), _read(directory / "integration.json")
        aggregate_gate = {key: value for key, value in measured.items() if key not in {"program", "level"}}
        if local_gate != aggregate_gate or local_gate.get("reference_sha256") != reference["sha256"]:
            raise ValueError("Per-program gate differs from its action aggregate")
        if integration.get("program") != program or integration.get("reference_sha256") != reference["sha256"]:
            raise ValueError("Integration proof program/reference differs from gate")
        if (len(integration.get("functions", [])) != measured.get("integrated_c_functions")
                or integration.get("matched_code_bytes") != measured.get("integrated_c_bytes")):
            raise ValueError("Integration counters differ from measured gate")
        binary = "boot.elf" if program == "boot" else "overlay.elf"
        if _pin(directory / "build" / binary, pins) != measured.get("candidate_sha256"):
            raise ValueError("Fresh reconstructed image pin mismatch")
        if _pin(directory / "assets" / binary, pins) != reference["sha256"]:
            raise ValueError("Fresh reconstruction reference asset changed")
        for assembly in sorted((directory / "asm").glob("*.s")):
            _pin(assembly, pins)
        if program == "boot":
            for identifier in _sdk_owners(integration, repo):
                for name in ("invocation.json", "inputs-before.json", "private-binding.json"):
                    _pin(directory / "build/c/sdk" / identifier / name, pins)
    for task_id in tasks:
        if task_id not in registry["tasks"]:
            raise ValueError("Unknown explicit closing task: " + task_id)
        for descriptor in registry["tasks"][task_id].get("targets", []):
            _pin(campaign.absolute(descriptor["catalog"], repo, store.runtime), pins)
    return {"gate": gate, "build": artifact.parent, "references": references,
            "c_toolchain": _private(_argument(command, "--c-toolchain"), repo),
            "external_pins": pins, "tasks": registry["tasks"]}


def _compress(data):
    buffer = io.BytesIO()
    with gzip.GzipFile(filename="", mode="wb", fileobj=buffer, mtime=0) as stream:
        stream.write(data)
    return buffer.getvalue()


def _tool_command(mirror, c_toolchain, build, destination):
    # Existing helpers observe the actual GNU and every admitted SDK instrument.
    # They do not compile or modify the completed build.
    code = ("import json,sys; from pathlib import Path; sys.path.insert(0,'scripts'); "
            "from wsl_chain import tool_hashes; from boot_sdk_unit import check_final_tool_closure; "
            "check_final_tool_closure(Path(sys.argv[2]),Path.cwd()); "
            "Path(sys.argv[3]).write_bytes((json.dumps(tool_hashes(Path(sys.argv[1])),sort_keys=True)+'\\n').encode())")
    return [sys.executable, "-c", code, str(c_toolchain), str(build / "boot"), str(destination)]


def _check_boot_edges(proof, catalog, validated_edges):
    """Require an exhaustive, nonvacuous static edge receipt, without fixed counts."""
    programs = {row["program"] for row in catalog["programs"]}
    images = proof.get("programs", [])
    if len(images) != len(programs) or {row["program"] for row in images} != programs:
        raise ValueError("Boot binding proof must inventory every current pinned program")
    if proof.get("blocked") != []:
        raise ValueError("Fresh boot static binding proof is blocked or incomplete")
    entries = {(row["program"], row["address"]): row for row in catalog["functions"]}
    core = proof["boot_core"]
    expected = set()
    for row in catalog["functions"]:
        program = row["program"]
        if program == "boot":
            continue
        for edge in row.get("call_dependencies", []):
            target = edge["target"]
            if ((program, target) not in entries and ("boot", target) in entries
                    and core["address"] <= target < core["address"] + core["size"]):
                expected.add((row["id"], edge["offset"], edge["kind"], target))
    if not expected or set(validated_edges) != expected or len(proof.get("bindings", [])) != len(expected):
        raise ValueError("Boot binding receipt is vacuous or omits current static cross-program edges")
    per_program = proof.get("per_program", {})
    if set(per_program) != programs - {"boot"}:
        raise ValueError("Boot binding per-program counters are incomplete")
    observed = {program: 0 for program in per_program}
    for binding in proof["bindings"]:
        observed[binding["program"]] += 1
    for program, counters in per_program.items():
        if (type(counters.get("bindings")) is not int or counters["bindings"] != observed[program]
                or type(counters.get("blocked")) is not int or counters["blocked"] != 0
                or counters.get("static_core_load_disjoint") is not True):
            raise ValueError("Boot binding observed edges disagree with per-program counters")
    return {"expected_edges": len(expected), "validated_edges": len(validated_edges),
            "target_bodies": len(proof["target_bodies"]), "programs": len(programs), "blocked": 0,
            "runtime_preservation_proven": False}


def _validate_boot_summary(repo, catalog_path, proof_path):
    """Use the maintained independent validator before attaching a fresh proof."""
    import unique_code_report
    from validate_boot_binding import validate_boot_binding
    catalog, source = unique_code_report.read_catalog(Path(catalog_path))
    proof = _read(proof_path)
    if proof.get("catalog_sha256") != _sha(source) or proof.get("source_catalog_sha256") != _sha(source):
        raise ValueError("Boot binding proof does not identify its fresh source catalogue")
    pins = {key: _sha((Path(repo) / name).read_bytes()) for key, name in (
        ("proof_source_sha256", "scripts/verify_boot_bindings.py"),
        ("reader_sha256", "scripts/elf_tools.py"),
        ("decoder_source_sha256", "scripts/relocation_identity.py"))}
    validated = validate_boot_binding(proof, catalog["functions"], catalog["programs"], catalog["function_chunks"], pins)
    return _check_boot_edges(proof, catalog, validated.edges)


def _prepare(store, repo, action_id, manifest, output, references, tasks, runner):
    evidence = _preflight(store, repo, action_id, manifest, references, tasks)
    before = _inventory(repo)
    output.mkdir(parents=True, exist_ok=False)
    mirror = output / "snapshot"
    for name, pin in before.items():
        # The maintained catalogue builder requires a fresh directory.
        destination = output / "baseline" / name if name.startswith("config/function-catalog/") else mirror / name
        data = _path(repo, name).read_bytes()
        if _sha(data) != pin:
            raise ValueError("Source changed while snapshotting: " + name)
        _new(destination, data)
    _new(output / "before-pins.json", _encoded(before))
    _new(output / "before-register.json", (repo / "config/campaign-register.json").read_bytes())
    snapshot_runtime = output / "validation-runtime"
    snapshot_runtime.mkdir()
    # Closure reads runtime-relative catalogs, but writes revisions/view backups
    # only into this private validation bank. The original runtime stays intact.
    for task_id in tasks:
        for descriptor in evidence["tasks"][task_id].get("targets", []):
            catalog_pointer = descriptor["catalog"]
            if catalog_pointer.startswith("runtime:"):
                name = _relative(catalog_pointer.removeprefix("runtime:"))
                source = _path(Path(store.runtime).resolve(), name)
                destination = _path(snapshot_runtime, name)
                if not destination.exists():
                    _new(destination, source.read_bytes())
    checks = []
    def run(label, script, arguments, command=None):
        command = command or [sys.executable, str(mirror / "scripts" / script), *map(str, arguments)]
        with (output / (label + ".log")).open("xb") as log:
            result = runner(command, cwd=mirror, stdout=log, stderr=subprocess.STDOUT)
        checks.append({"step": label, "returncode": result.returncode})
        if result.returncode != 0:
            _new(output / "failed-checks.json", _encoded(checks))
            raise ValueError("Finalization check failed: " + label + "; private log retained")
    build, refs, gate = evidence["build"], evidence["references"], evidence["gate"]
    tool_output = output / "current-c-tools.json"
    run("00-current-c-instruments", "", [], _tool_command(mirror, evidence["c_toolchain"], build, tool_output))
    current_c_tools = _read(tool_output)
    default_review = _read(mirror / "progress/candidates.json")
    if current_c_tools != default_review.get("tools"):
        raise ValueError("Current C instruments differ from the qualified default profile")
    levels = [row["level"] for row in gate["g3"]]
    physical = mirror / "progress/objdiff.json"
    explicit = ["--output", physical, "--integration-proof", build / "boot/integration.json", "--progress-proof", build / "report.json"]
    for level in levels:
        explicit += ["--level-proof", build / level / "integration.json"]
    run("01-validate-private-proofs", "decomp_report.py", explicit)
    physical_report = _read(physical)
    boot = _read(build / "boot/integration.json")
    proofs = [_read(build / level / "integration.json") for level in levels]
    total = int(physical_report["measures"]["matchedCode"])
    if total != boot["matched_code_bytes"] + sum(row["matched_code_bytes"] for row in proofs):
        raise ValueError("Independent exporter counters disagree with complete proofs")
    enriched = copy.deepcopy(gate)
    enriched.update(compiler_flags=_read(mirror / "progress/candidates.json")["flags"],
                    compiler_flags_scope="Default boot/shared owner; explicit native and SDK owners retain their qualified flags",
                    matched_candidate_functions=len(boot["functions"]), matched_candidate_bytes=boot["matched_code_bytes"],
                    integrated_code_bytes=total, native_gameplay_verified=False,
                    c_integration_reference="progress/integration.json",
                    status=f"Boot and {len(proofs)} complete overlay loaded-byte/metadata gates passed; {total} integrated C bytes; native gameplay not verified.")
    staged = {"progress/integration.json": boot, "progress/report.json": enriched}
    staged.update({"progress/levels/" + level + ".json": proof for level, proof in zip(levels, proofs)})
    for name, value in staged.items():
        destination = _path(mirror, name)
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(_encoded(value))
    run("02-physical-display", "readme_progress.py", [])
    boundaries = output / "boundaries.json"
    run("03-global-boundaries", "global_function_catalog.py", ["--repo", mirror, "--references", refs, "--build", build, "--output", boundaries])
    boundary = _read(boundaries)
    if boundary.get("qualified_extent_failures"):
        raise ValueError("Fresh global boundary qualification failed")
    # Always rescan: source/eligible-boundary changes cannot retain stale evidence.
    pointer_raw = output / "pointer-arguments.json"
    run("04-pointer-scan", "scan_pointer_roles.py", ["--boundaries", boundaries, "--references", refs,
        "--decoder-source", mirror / "scripts/relocation_identity.py", "--output", pointer_raw])
    pointer = mirror / "config/function-evidence/pointer-arguments.json.gz"
    pointer.parent.mkdir(parents=True, exist_ok=True)
    pointer.write_bytes(_compress(pointer_raw.read_bytes().replace(b"\r\n", b"\n")))
    catalog_dir = mirror / "config/function-catalog"
    run("05-normalization-and-pointer-replay", "build_unique_catalog.py", ["--repo", mirror, "--boundaries", boundaries,
        "--references", refs, "--output", catalog_dir, "--diagnostics", output / "normalizer-diagnostics.json", "--pointer-evidence", pointer])
    catalog_path = catalog_dir / "catalog.json"
    catalog = _read(catalog_path)
    boot_raw = output / "boot-bindings.json"
    run("06-boot-bindings", "verify_boot_bindings.py", ["--repo", mirror, "--references", refs, "--catalog", catalog_path, "--output", boot_raw])
    boot_binding = _read(boot_raw)
    boot_summary_path = output / "boot-binding-validation.json"
    boot_check = ("import json,sys;from pathlib import Path;sys.path.insert(0,'scripts');"
                  "from campaign_finalize import _validate_boot_summary;"
                  "Path(sys.argv[3]).write_bytes((json.dumps(_validate_boot_summary(Path.cwd(),Path(sys.argv[1]),Path(sys.argv[2])),sort_keys=True)+'\\n').encode())")
    run("06b-independent-boot-binding-check", "", [], [sys.executable, "-c", boot_check,
        str(catalog_path), str(boot_raw), str(boot_summary_path)])
    boot_summary = _read(boot_summary_path)
    boot_path = mirror / "config/function-evidence/boot-bindings.json.gz"
    boot_path.write_bytes(_compress(boot_raw.read_bytes().replace(b"\r\n", b"\n")))
    catalog["boot_binding_proof"] = {"path": boot_path.relative_to(mirror).as_posix(), "sha256": _sha(boot_path.read_bytes()),
        "scope": "combined-pinned-reference-images", "runtime_preservation_proven": False,
        "source_catalog_sha256": boot_binding["source_catalog_sha256"]}
    catalog_pins = {row["path"]: row["sha256"] for row in catalog["input_pins"]}
    for name in ("config/function-evidence/boot-bindings.json.gz", "scripts/verify_boot_bindings.py",
                 "scripts/validate_boot_binding.py", "scripts/elf_tools.py", "scripts/relocation_identity.py"):
        catalog_pins[name] = _sha((mirror / name).read_bytes())
    catalog["input_pins"] = [{"path": name, "sha256": pin} for name, pin in sorted(catalog_pins.items())]
    catalog_path.write_bytes(_encoded(catalog))
    unique = mirror / "progress/unique-code-report.json"
    unique_args = ["--repo", mirror, "--catalog", catalog_path, "--output", unique, "--objdiff-output", mirror / "progress/unique-objdiff.json"]
    run("07-unique-primary", "unique_code_report.py", unique_args)
    run("08-unique-check", "unique_code_report.py", [*unique_args, "--check"])
    public_export = ["--output", physical]
    for level in levels:
        public_export += ["--level-proof", mirror / ("progress/levels/" + level + ".json")]
    run("09-physical-staged-export", "decomp_report.py", public_export)
    if _read(physical)["measures"] != physical_report["measures"]:
        raise ValueError("Staged public proofs changed validated physical measures")
    display = ["--repo", mirror, "--catalogue-report", unique, "--physical-report", physical]
    run("10-paired-display", "readme_unique_progress.py", display)
    run("11-paired-display-check", "readme_unique_progress.py", [*display, "--check"])
    reuse = ["--repo", mirror, "--catalog", catalog_path, "--output", mirror / "progress/code-reuse-report.json",
             "--families-output", mirror / "progress/code-reuse-families.json.gz", "--references", refs]
    run("12-supplementary-reuse", "code_reuse_report.py", reuse)
    run("13-supplementary-reuse-check", "code_reuse_report.py", [*reuse, "--check"])
    closed, retained = [], []
    for task_id in tasks:
        if evidence["tasks"][task_id]["kind"] != "candidate":
            retained.append(task_id)
            continue
        run("close-" + task_id, "campaign.py", ["--repo", mirror, "--registry", mirror / "config/campaign-register.json",
            "--runtime", snapshot_runtime, "close", task_id])
        closed.append(task_id)
    campaign_args = ["--repo", mirror, "--registry", mirror / "config/campaign-register.json", "--runtime", snapshot_runtime, "views"]
    run("14-register-views", "campaign.py", campaign_args)
    run("15-register-views-check", "campaign.py", [*campaign_args, "--check"])
    run("16-source-inventory", "source_layout.py", ["--repo", mirror, "--inventory-output", mirror / "progress/source-inventory.json"])
    run("17-source-inventory-check", "source_layout.py", ["--repo", mirror, "--check", "--inventory-check", mirror / "progress/source-inventory.json"])
    run("18-physical-display-check", "readme_progress.py", ["--check"])
    run("19-tool-tests", "", [], [sys.executable, "-m", "unittest", "discover", "-s", str(mirror / "tests"), "-q"])
    # All output chunks are explicitly declared by the independently validated manifest.
    allowed = FIXED_OUTPUTS | {"progress/levels/" + level + ".json" for level in levels} | {"config/function-catalog/catalog.json"}
    for chunk in catalog["function_chunks"]:
        name = _relative(chunk["path"])
        if "/" in name or not name.endswith(".ndjson.gz"):
            raise ValueError("Unexpected generated catalogue chunk")
        allowed.add("config/function-catalog/" + name)
    after = _inventory(mirror)
    changes = []
    diff = []
    for name, pin in after.items():
        if before.get(name) == pin:
            continue
        if name not in allowed:
            raise ValueError("Maintained command changed a non-publication file: " + name)
        data = _path(mirror, name).read_bytes()
        baseline_data = _path(repo, name).read_bytes() if name in before else None
        _public_metadata(name, data, baseline_data)
        changes.append({"path": name, "before_sha256": before.get(name), "after_sha256": pin})
        if not name.endswith(".gz"):
            original = (repo / name).read_bytes().decode("utf8").splitlines(True) if name in before else []
            diff.extend(difflib.unified_diff(original, data.decode("utf8").splitlines(True), fromfile="a/" + name, tofile="b/" + name))
    missing = set(before) - set(after)
    if missing:
        raise ValueError("Finalization would remove existing files: " + ", ".join(sorted(missing)))
    if _inventory(repo) != before:
        raise ValueError("Public source changed during private preparation")
    _verify_external(evidence["external_pins"])
    _new(output / "review.diff", "".join(diff).encode("utf8"))
    plan = {"schema": 1, "action": action_id, "repo": str(repo), "manifest": str(Path(manifest).resolve()),
            "references": str(refs), "runtime": str(store.runtime), "tasks": list(tasks), "before": before,
            "changes": sorted(changes, key=lambda row: row["path"]), "stage_inventory": after,
            "external_pins": evidence["external_pins"], "c_toolchain": str(evidence["c_toolchain"]),
            "c_tools": current_c_tools, "build": str(build),
            "checks": checks, "closed_candidates": closed, "retained_research_tasks": retained,
            "physical_measures": physical_report["measures"], "primary_metrics": _read(unique)["metrics"],
            "normalization": catalog["generation"], "pointer_evidence": "fresh scan and complete theorem replay",
            "boot_binding_validation": boot_summary,
            "runtime_preservation_proven": False, "private_validation_exports": sorted(PRIVATE_EXPORTS)}
    _new(output / "plan.json", _encoded(plan))
    _new(output / "journal.json", _encoded({"state": "prepared", "plan_sha256": _sha(_encoded(plan)), "published": []}))
    return plan


def _verify_external(pins):
    for name, pin in pins.items():
        if not Path(name).is_file() or _sha(Path(name).read_bytes()) != pin:
            raise ValueError("Private action evidence changed: " + name)


def _guard(repo, plan, published):
    expected = dict(plan["before"])
    for row in plan["changes"]:
        if row["path"] in published:
            expected[row["path"]] = row["after_sha256"]
    if _inventory(repo) != expected:
        raise ValueError("Concurrent source, registry, profile or output change; publication refused")
    _verify_external(plan["external_pins"])


def _guard_stage(output, plan):
    if _inventory(output / "snapshot") != plan["stage_inventory"]:
        raise ValueError("Prepared output changed before publication")


class PublicationConflict(ValueError):
    """All displaced versions are retained; automatic publication must stop."""
    def __init__(self, message, destination):
        super().__init__(message)
        self.destination = str(destination)


def _atomic_backend():
    if sys.platform == "win32":
        return "windows"
    if sys.platform.startswith("linux"):
        library = ctypes.CDLL(None, use_errno=True)
        if getattr(library, "renameat2", None) is not None:
            return "linux"
    raise ValueError("Safe atomic predecessor capture is unsupported on this platform")


def _assert_atomic_backend(repo, output):
    _atomic_backend()
    if os.stat(repo).st_dev != os.stat(output).st_dev:
        raise ValueError("Atomic predecessor capture requires private output on the repository volume")


def _rename_linux(source, destination, flags):
    library = ctypes.CDLL(None, use_errno=True)
    operation = library.renameat2
    operation.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_uint]
    operation.restype = ctypes.c_int
    if operation(-100, os.fsencode(source), -100, os.fsencode(destination), flags) != 0:
        number = ctypes.get_errno()
        raise OSError(number, os.strerror(number), str(destination))


def _move_noreplace(source, destination):
    """Atomically move only if the destination is absent; never overwrite it."""
    if _atomic_backend() == "windows":
        library = ctypes.WinDLL("kernel32", use_last_error=True)
        operation = library.MoveFileExW
        operation.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_uint]
        operation.restype = ctypes.c_int
        if not operation(str(source), str(destination), 0):
            raise ctypes.WinError(ctypes.get_last_error())
    else:
        _rename_linux(source, destination, 1)  # RENAME_NOREPLACE


def _capture_replace(destination, replacement, capture):
    """Native atomic replacement retaining the actual displaced predecessor.

    Microsoft ReplaceFileW requires a same-volume named backup. Linux exchange
    leaves the displaced inode at the replacement path. No unknown image is
    removed, including documented Windows partial failures 1175/1176/1177.
    """
    if _atomic_backend() == "windows":
        library = ctypes.WinDLL("kernel32", use_last_error=True)
        operation = library.ReplaceFileW
        operation.argtypes = [ctypes.c_wchar_p, ctypes.c_wchar_p, ctypes.c_wchar_p,
                              ctypes.c_uint, ctypes.c_void_p, ctypes.c_void_p]
        operation.restype = ctypes.c_int
        if not operation(str(destination), str(replacement), str(capture), 0, None, None):
            raise ctypes.WinError(ctypes.get_last_error())
        return capture
    _rename_linux(destination, replacement, 2)  # RENAME_EXCHANGE
    return replacement


def _operation(bank, destination, data, expected, kind):
    directory = bank / uuid.uuid4().hex
    directory.mkdir(parents=True)
    replacement = directory / "replacement.bin"
    if data is not None:
        _new(replacement, data)
    capture = directory / "displaced.bin"
    record = {"kind": kind, "destination": str(destination), "expected_sha256": expected,
              "proposed_sha256": _sha(data) if data is not None else None,
              "replacement": str(replacement), "capture": str(capture), "backend": _atomic_backend()}
    if data is not None:
        identity = replacement.stat()
        record["replacement_identity"] = [identity.st_dev, identity.st_ino]
    _new(directory / "prepared.json", _encoded(record))
    return directory, replacement, capture, record


def _actual_capture(record):
    location = Path(record["capture"] if record["backend"] == "windows" or record["kind"] == "remove"
                    else record["replacement"])
    if not location.exists():
        return None
    # Exchange identity, never byte equality: a racing predecessor may itself
    # contain exactly the proposed bytes, but still differs from the before pin.
    if record["backend"] == "linux" and record["kind"] != "remove":
        identity = location.stat()
        if [identity.st_dev, identity.st_ino] == record.get("replacement_identity"):
            return None
    return location


def _restore_displaced(destination, data, installed_sha, bank):
    """Restore a displaced writer; capture each later racing writer as well.

    A changed public image is left alone. A race after that observation is
    captured by the native primitive and its newer version becomes the next
    restoration candidate. Bounded recovery retains every version if writers
    never quiesce; the enclosing transaction remains a conflict, never applied.
    """
    for _ in range(8):
        observed = _sha(destination.read_bytes()) if destination.exists() else None
        if observed is not None and observed != installed_sha:
            return False
        directory, replacement, capture, record = _operation(bank, destination, data, installed_sha, "restore")
        try:
            if observed is None:
                _move_noreplace(replacement, destination)
                _new(directory / "result.json", _encoded({"state": "restored-absent"}))
                return True
            captured = _capture_replace(destination, replacement, capture)
        except FileExistsError:
            _new(directory / "result.json", _encoded({"state": "newer-writer-retained"}))
            return False
        except OSError as error:
            captured = _actual_capture(record)
            _new(directory / "result.json", _encoded({"state": "native-error", "error": str(error),
                "capture": str(captured) if captured else None}))
            if captured is None:
                return False
            # A partial native failure may have left the destination absent.
            data = captured.read_bytes()
            continue
        displaced = captured.read_bytes()
        displaced_sha = _sha(displaced)
        _new(directory / "result.json", _encoded({"state": "restored" if displaced_sha == installed_sha else "racing-writer-captured",
            "capture": str(captured), "captured_sha256": displaced_sha}))
        if displaced_sha == installed_sha:
            return True
        installed_sha, data = _sha(data), displaced
    return False


def _checked_replace(destination, data, expected, bank, kind="replace"):
    """Check the atomically displaced image, rather than a racy prior read."""
    destination.parent.mkdir(parents=True, exist_ok=True)
    directory, replacement, capture, record = _operation(bank, destination, data, expected, kind)
    if expected is None:
        try:
            _move_noreplace(replacement, destination)
        except FileExistsError as error:
            _new(directory / "result.json", _encoded({"state": "conflict-new-destination"}))
            raise PublicationConflict("Concurrent writer created the publication destination", destination) from error
        _new(directory / "result.json", _encoded({"state": "created-exclusively"}))
        return
    try:
        captured = _capture_replace(destination, replacement, capture)
    except OSError as error:
        captured = _actual_capture(record)
        _new(directory / "result.json", _encoded({"state": "native-error", "error": str(error),
            "capture": str(captured) if captured else None}))
        if captured is not None:
            _restore_displaced(destination, captured.read_bytes(), record["proposed_sha256"], bank)
            raise PublicationConflict("Native replacement failed after displacing an image; versions retained", destination) from error
        if isinstance(error, FileNotFoundError):
            raise PublicationConflict("Concurrent writer removed the publication destination", destination) from error
        raise
    displaced = captured.read_bytes()
    displaced_sha = _sha(displaced)
    if displaced_sha != expected:
        _new(directory / "result.json", _encoded({"state": "conflict", "capture": str(captured), "captured_sha256": displaced_sha}))
        _restore_displaced(destination, displaced, record["proposed_sha256"], bank)
        raise PublicationConflict("Concurrent predecessor differs from its expected SHA256; displaced versions retained", destination)
    _new(directory / "result.json", _encoded({"state": "replaced", "capture": str(captured), "captured_sha256": displaced_sha}))


def _checked_remove(destination, expected, bank):
    """Rollback a newly created file by atomically capturing what is there."""
    directory, _, capture, record = _operation(bank, destination, None, expected, "remove")
    try:
        _move_noreplace(destination, capture)
    except FileNotFoundError as error:
        raise PublicationConflict("Concurrent writer removed a rollback destination", destination) from error
    displaced = capture.read_bytes()
    actual = _sha(displaced)
    _new(directory / "result.json", _encoded({"state": "removed" if actual == expected else "conflict",
        "capture": str(capture), "captured_sha256": actual}))
    if actual != expected:
        _restore_displaced(destination, displaced, None, bank)
        raise PublicationConflict("Concurrent rollback predecessor retained and restored", destination)


def _check_pending_operations(bank, journal, output):
    """A crash between replace and validation cannot hide an unreviewed image."""
    for location in bank.glob("*/prepared.json"):
        record = _read(location)
        result_path = location.parent / "result.json"
        captured = _actual_capture(record)
        result = _read(result_path) if result_path.exists() else {}
        conflict = result.get("state", "").startswith("conflict") or (
            result.get("state") == "native-error" and captured is not None)
        if captured is None:
            destination = Path(record["destination"])
            if not result and destination.exists() and _sha(destination.read_bytes()) == record["proposed_sha256"]:
                if record["expected_sha256"] is None and Path(record["replacement"]).exists():
                    conflict = True  # Exclusive creation did not establish ownership of this name.
                elif record["expected_sha256"] is not None:
                    conflict = True  # No durable receipt or established exchange identity.
            if conflict:
                journal.update(state="conflict", conflicts=[record["destination"]])
                _atomic(output / "journal.json", _encoded(journal))
                raise PublicationConflict("Interrupted operation retains a concurrent destination", Path(record["destination"]))
            continue
        data = captured.read_bytes()
        if conflict or _sha(data) != record["expected_sha256"]:
            destination = Path(record["destination"])
            _restore_displaced(destination, data, record["proposed_sha256"], bank)
            journal.update(state="conflict", conflicts=[record["destination"]])
            _atomic(output / "journal.json", _encoded(journal))
            raise PublicationConflict("Interrupted atomic publication captured a concurrent predecessor", destination)


def _publish(repo, output, plan, journal):
    """Prepared journal supports restart after interruption without blind writes."""
    _assert_atomic_backend(repo, output)
    lock = repo / "config/campaign-register.json.lock"
    try:
        fd = os.open(lock, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
    except FileExistsError as error:
        raise ValueError("Campaign register is locked; no publication attempted") from error
    with os.fdopen(fd, "wb") as stream:
        stream.write(_encoded({"owner": "campaign-finalize", "pid": os.getpid(), "action": plan["action"],
                               "plan_sha256": journal["plan_sha256"]}))
    try:
        changes = plan["changes"]
        operations = output / "publication-operations"
        _check_pending_operations(operations, journal, output)
        # Infer an atomic replace that completed immediately before a crash.
        published = set(journal.get("published", []))
        for row in changes:
            path = _path(repo, row["path"])
            actual = _sha(path.read_bytes()) if path.exists() else None
            if actual == row["after_sha256"]:
                published.add(row["path"])
            elif actual == row["before_sha256"]:
                published.discard(row["path"])
        _guard(repo, plan, published)
        _guard_stage(output, plan)
        for row in changes:
            path = _path(output / "snapshot", row["path"])
            data = path.read_bytes()
            if _sha(data) != row["after_sha256"]:
                raise ValueError("Prepared output changed before publication")
            if row["path"] == "config/campaign-register.json":
                baseline_data = (output / "before-register.json").read_bytes()
                if _sha(baseline_data) != plan["before"][row["path"]]:
                    raise ValueError("Prepared before-register changed")
            else:
                baseline_data = None
            _public_metadata(row["path"], data, baseline_data)
        backup = output / "backups"
        for row in changes:
            name = row["path"]
            if row["before_sha256"] is None:
                continue
            saved = backup / name
            if not saved.exists():
                if name in published:
                    raise ValueError("Interrupted publication lacks its mandatory before backup")
                _new(saved, _path(repo, name).read_bytes())
            if _sha(saved.read_bytes()) != row["before_sha256"]:
                raise ValueError("Before backup hash mismatch")
        journal.update(state="publishing", published=sorted(published), backup_count=sum(r["before_sha256"] is not None for r in changes))
        _atomic(output / "journal.json", _encoded(journal))
        try:
            for row in changes:
                name = row["path"]
                if name in published:
                    continue
                _guard(repo, plan, published)
                _guard_stage(output, plan)
                data = _path(output / "snapshot", name).read_bytes()
                if _sha(data) != row["after_sha256"]:
                    raise ValueError("Prepared output changed immediately before replace")
                _checked_replace(_path(repo, name), data, row["before_sha256"], operations)
                published.add(name)
                journal["published"] = sorted(published)
                _atomic(output / "journal.json", _encoded(journal))
            _guard(repo, plan, published)
            _check_pending_operations(operations, journal, output)
        except BaseException as error:
            # Rollback captures its actual predecessor too; a racy prior hash is
            # never authority to replace or remove another writer's file.
            conflicts = []
            if isinstance(error, PublicationConflict):
                location = Path(error.destination)
                conflicts.append(location.relative_to(repo).as_posix() if location.is_relative_to(repo) else str(location))
            for row in reversed(changes):
                name = row["path"]
                if name not in published:
                    continue
                destination = _path(repo, name)
                try:
                    if row["before_sha256"] is None:
                        _checked_remove(destination, row["after_sha256"], operations)
                    else:
                        _checked_replace(destination, (backup / name).read_bytes(), row["after_sha256"], operations, "rollback")
                except (OSError, ValueError):
                    conflicts.append(name)
                    continue
                published.remove(name)
            actual = _inventory(repo)
            if actual != plan["before"]:
                conflicts.extend(name for name in actual.keys() | plan["before"].keys()
                                 if actual.get(name) != plan["before"].get(name))
            conflicts = sorted(set(conflicts))
            journal.update(state="conflict" if conflicts else "rolled_back", published=sorted(published), conflicts=conflicts)
            _atomic(output / "journal.json", _encoded(journal))
            raise
        journal.update(state="applied", published=sorted(published))
        _atomic(output / "journal.json", _encoded(journal))
    finally:
        lock.unlink()


def finalize(store, repo, action_id, *, manifest, output, tasks=(), apply=False,
             references=None, runner=subprocess.run):
    """Return a private review receipt; ``apply`` publishes the same prepared plan.

    ``manifest`` is the exact reference manifest recorded by build/integrate.
    ``references`` optionally supplies the conventional pinned reference tree.
    Reusing ``output`` resumes its immutable plan; changed inputs are refused.
    Research tasks remain explicit outstanding decisions and gain no false closure.
    """
    repo = Path(repo).resolve()
    output = _private_output(output, repo)
    manifest = _private(manifest, repo)
    tasks = tuple(tasks)
    if len(tasks) != len(set(tasks)) or any(not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_.-]*", task) for task in tasks):
        raise ValueError("Closing tasks must be unique explicit safe identifiers")
    if output.exists():
        plan, journal = _read(output / "plan.json"), _read(output / "journal.json")
        if (journal.get("plan_sha256") != _sha(_encoded(plan)) or plan.get("repo") != str(repo)
                or plan.get("action") != action_id or plan.get("manifest") != str(manifest)
                or plan.get("runtime") != str(store.runtime) or plan.get("tasks") != list(tasks)
                or (references is not None and plan.get("references") != str(Path(references).resolve()))):
            raise ValueError("Existing finalization plan does not match this invocation")
        if journal.get("state") == "conflict":
            raise ValueError("Interrupted publication has concurrent conflicts; inspect retained journal")
    else:
        plan = _prepare(store, repo, action_id, manifest, output, references, tasks, runner)
        journal = _read(output / "journal.json")
    if journal.get("state") == "applied":
        _guard(repo, plan, {row["path"] for row in plan["changes"]})
    elif apply:
        _guard_stage(output, plan)
        # A resumed plan retains expensive normalization/test results, but observes
        # actual compiler tools again before the first public write.
        observation = output / ("apply-c-tools-" + uuid.uuid4().hex + ".json")
        with observation.with_suffix(".log").open("xb") as log:
            completed = runner(_tool_command(output / "snapshot", Path(plan["c_toolchain"]), Path(plan["build"]), observation),
                               cwd=output / "snapshot", stdout=log, stderr=subprocess.STDOUT)
        if completed.returncode != 0 or _read(observation) != plan["c_tools"]:
            raise ValueError("Current C/SDK instruments changed since finalization preparation")
        _publish(repo, output, plan, journal)
        journal = _read(output / "journal.json")
    elif journal.get("state") in {"prepared", "rolled_back"}:
        _guard(repo, plan, set())
    else:
        raise ValueError("Publication interrupted; resume the same output with --apply")
    receipt = {"schema": 1, "action": action_id, "state": journal["state"], "private_only": True,
               "output": str(output), "plan": str(output / "plan.json"), "review_diff": str(output / "review.diff"),
               "changed_files": len(plan["changes"]), "checks": plan["checks"],
               "physical_measures": plan["physical_measures"], "primary_metrics": plan["primary_metrics"],
               "closed_candidates": plan["closed_candidates"], "retained_research_tasks": plan["retained_research_tasks"],
               "backup_count": journal.get("backup_count", 0), "git_mutated": False,
               "integration_credit_added_by_finalizer": 0}
    _atomic(output / "receipt.json", _encoded(receipt))
    return receipt
