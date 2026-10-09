"""Publish a reviewed, derived research shelf from immutable campaign trials.

This tool never compiles, changes campaign decisions, accepts C or grants credit.
The caller must provide an owner-reviewed publication envelope; compiler output
and lexical guards cannot establish the original ABI or all defined behavior.
"""
from __future__ import annotations

import argparse
from contextlib import contextmanager
from datetime import datetime
import hashlib
import json
from pathlib import Path
import re
import sys
import uuid


SHA = re.compile(r"[0-9a-f]{64}\Z")
TRIAL = re.compile(r"[0-9a-f]{32}\Z")
SAFE = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,119}\Z")
SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]{0,99}\Z")
PROGRAM = re.compile(r"(?:boot|levels/[A-Za-z0-9][A-Za-z0-9_-]{0,63})\Z")
REOPEN_CODES = frozenset({"new_abi_evidence", "new_declaration_evidence", "new_compiler_evidence", "new_structural_evidence"})
REVIEWS = frozenset({"authored_c", "no_embedded_game_data", "no_private_data", "defined_behavior", "abi_reviewed"})
MAX_SOURCE = 128 * 1024
MAX_METADATA = 32 * 1024
MAX_ENTRIES = 1000
PRIVATE = re.compile(rb"(?<![A-Za-z0-9_])[A-Za-z]:[\\/]|\\\\|/(?:root|home|Users|private|tmp|var|mnt|workspace|srv|opt|etc|run)/|(?:github_pat_|ghp_|gho_)[A-Za-z0-9_]+|-----BEGIN [A-Z ]*PRIVATE KEY-----|\b(?:GITHUB_TOKEN|OPENAI_API_KEY|ANTHROPIC_API_KEY)\b", re.I)
METADATA_KEYS = frozenset({"schema", "kind", "status", "integration_credit", "task", "trial", "target", "program", "address", "size", "boundary_status", "symbol", "checked_date", "candidate_size", "different_bytes", "source_sha256", "reference_sha256", "reference_body_sha256", "candidate_body_sha256", "catalog_sha256", "profile_sha256", "manifest_sha256", "outcome_sha256", "semantic_key", "publication_receipt_sha256", "reopen_codes"})


def digest(data):
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    return (json.dumps(value, sort_keys=True, indent=2) + "\n").encode("utf-8")


def require(condition, code):
    if not condition:
        raise ValueError(code)


def match(pattern, value):
    return isinstance(value, str) and pattern.fullmatch(value) is not None


def integer(value, minimum=0, maximum=2**32):
    return type(value) is int and minimum <= value <= maximum


def bounded(path, limit):
    path = Path(path)
    require(path.is_file() and path.stat().st_size <= limit, "missing_or_oversized_input")
    with path.open("rb") as stream:
        data = stream.read(limit + 1)
    require(len(data) <= limit, "oversized_input")
    return data


def inside(root, relative):
    root = Path(root).resolve()
    relative = Path(relative)
    require(not relative.is_absolute() and all(part not in {".", ".."} for part in relative.parts), "invalid_shelf_path")
    path = (root / relative).resolve()
    require(path != root and path.is_relative_to(root), "escaped_shelf_path")
    return path


def publication_envelope(value):
    keys = {"schema", "kind", "task", "trial", "target", "symbol", "source_sha256", "review", "reopen_codes"}
    require(type(value) is dict and set(value) == keys, "invalid_publication_envelope")
    require(type(value["schema"]) is int and value["schema"] == 1 and value["kind"] == "rac2-nonmatching-publication", "invalid_publication_envelope")
    require(match(SAFE, value["task"]) and value["task"] not in {".", ".."} and match(TRIAL, value["trial"]) and match(SAFE, value["target"]) and match(SYMBOL, value["symbol"]) and match(SHA, value["source_sha256"]), "invalid_publication_identity")
    review = value["review"]
    require(type(review) is dict and set(review) == REVIEWS and all(flag is True for flag in review.values()), "owner_review_required")
    codes = value["reopen_codes"]
    require(type(codes) is list and 1 <= len(codes) <= len(REOPEN_CODES) and all(isinstance(code, str) and code in REOPEN_CODES for code in codes) and len(set(codes)) == len(codes), "invalid_reopen_codes")
    result = json.loads(encoded(value))
    result["reopen_codes"] = sorted(result["reopen_codes"])
    return result


def source_guard(source, symbol, externals):
    require(type(source) is bytes and 0 < len(source) <= MAX_SOURCE and b"\0" not in source, "invalid_authored_source")
    try:
        source.decode("utf-8", errors="strict")
    except UnicodeDecodeError:
        raise ValueError("invalid_source_encoding") from None
    require(not PRIVATE.search(source), "private_source_metadata")
    # Reuse the maintained ordinary-C scope policy; do not rewrite source bytes.
    from family_candidates import _source_scope, _scrub
    try:
        _source_scope(source, symbol, set(externals))
    except (ValueError, TypeError, UnicodeError):
        raise ValueError("unsupported_source_scope") from None
    clean = _scrub(source)
    require(not re.search(rb"\b(?:__attribute__|__builtin_unreachable|__builtin_alloca|setjmp|longjmp)\b", clean), "unsupported_source_control")
    # This catches a direct uninitialized-return trick, not general C data flow.
    obvious = rb"\b(?:int|char|short|long|float|double)\s+([A-Za-z_]\w*)\s*;\s*return\s+\1\s*;"
    require(not re.search(obvious, clean), "obvious_uninitialized_return")


def _current_context(repo, catalog_path):
    from unique_code_report import read_catalog, validate_catalog, load_current_credit
    catalog, raw = read_catalog(catalog_path)
    programs, rows = validate_catalog(catalog)
    credit = load_current_credit(repo, catalog)
    return programs, rows, credit, digest(raw)


def _current_target(repo, catalog_path, program, address, size, reference, source_body):
    programs, rows, credit, catalog_pin = _current_context(repo, catalog_path)
    require(program in programs and programs[program]["reference_sha256"] == reference, "current_reference_mismatch")
    candidates = [row for row in rows if row["program"] == program and row["address"] == address and row["size"] == size]
    require(len(candidates) == 1, "unknown_complete_extent")
    row = candidates[0]
    require(row.get("boundary", {}).get("status") == "flow_supported_inferred", "unsupported_research_extent")
    require(row.get("raw_sha256") == source_body, "current_reference_body_mismatch")
    # qualified_complete is emitted for already qualified C and is excluded.
    # Flow-supported boundaries remain inferred even when the complete retained
    # trial symbol was measured; reject all current accepted-C overlap as well.
    require(not any(name == program and address < start + length and start < address + size for name, start, length in credit), "already_current_exact")
    return catalog_pin


def _task_open(registry, task_id):
    task = registry.get("tasks", {}).get(task_id)
    require(type(task) is dict and task.get("kind") == "candidate", "unknown_candidate_task")
    require(task.get("state") in {"queued", "blocked", "stopped"} and not task.get("active_trial"), "task_not_research_open")
    return task


def prepare_entry(store, repo, catalog_path, envelope):
    """Read retained evidence and return public metadata plus exact authored C."""
    envelope = publication_envelope(envelope)
    repo = Path(repo).resolve()
    registry = store.load()
    _task_open(registry, envelope["task"])
    from campaign_diff import review_data, _inside, _pinned
    data = review_data(store, repo, envelope["task"], trial_id=envelope["trial"], target=envelope["target"], symbol=envelope["symbol"])
    require(data["trial_state"] == "mismatch" and not data["historical_instrument_drift"], "unqualified_historical_trial")
    views = [view for view in data["views"] if view["target"] == envelope["target"] and view["symbol"] == envelope["symbol"]]
    require(len(views) == 1, "ambiguous_trial_function")
    view = views[0]
    provenance = next(item for item in data["provenance"] if item["target"] == envelope["target"])
    require(view["mode"] == "linked ET_EXEC" and not view["relative"] and view["candidate_address"] == view["address"] and not provenance["has_relocations"], "incomplete_linked_measurement")
    require(integer(view["candidate_size"], 4, 128 * 1024) and view["candidate_size"] % 4 == 0 and integer(view["size"], 4, 128 * 1024) and view["size"] % 4 == 0 and integer(view["different_bytes"], 1, view["size"] + view["candidate_size"]), "not_a_measured_near_miss")
    require(match(SHA, view["reference_sha256"]) and match(SHA, view["candidate_sha256"]), "missing_body_pins")
    require(data["source_sha256"] == envelope["source_sha256"], "unreviewed_source_hash")
    work = _inside(store.runtime, "trials", envelope["trial"])
    manifest = json.loads(_pinned(work, "manifest.json", data["manifest_sha256"]))
    source = _pinned(work, "source/" + data["source_name"], data["source_sha256"])
    child = _inside(work, "targets", envelope["target"])
    catalog = json.loads(_pinned(child, "catalog.json", provenance["catalog_sha256"]))
    source_guard(source, envelope["symbol"], catalog.get("externals", {}))
    program = catalog.get("program", "boot")
    require(match(PROGRAM, program), "invalid_program")
    _current_target(repo, catalog_path, program, view["address"], view["size"], provenance["reference_elf_sha256"], view["reference_sha256"])
    try:
        checked = datetime.fromisoformat(manifest["created"])
        require(checked.tzinfo is not None, "missing_trial_date")
        checked_date = checked.date().isoformat()
    except (KeyError, TypeError, ValueError):
        raise ValueError("missing_trial_date") from None
    metadata = {"schema": 1, "kind": "rac2-nonmatching-entry", "status": "research-only", "integration_credit": 0,
                "task": envelope["task"], "trial": envelope["trial"], "target": envelope["target"], "program": program,
                "address": view["address"], "size": view["size"], "boundary_status": "flow_supported_inferred", "symbol": envelope["symbol"], "checked_date": checked_date,
                "candidate_size": view["candidate_size"], "different_bytes": view["different_bytes"],
                "source_sha256": data["source_sha256"], "reference_sha256": provenance["reference_elf_sha256"],
                "reference_body_sha256": view["reference_sha256"], "candidate_body_sha256": view["candidate_sha256"],
                "catalog_sha256": provenance["catalog_sha256"], "profile_sha256": data["profile_sha256"],
                "manifest_sha256": data["manifest_sha256"], "outcome_sha256": data["outcome_sha256"], "semantic_key": data["semantic_key"],
                "publication_receipt_sha256": digest(encoded(envelope)), "reopen_codes": sorted(envelope["reopen_codes"])}
    validate_metadata(metadata)
    return metadata, source


def validate_metadata(value):
    require(type(value) is dict and set(value) == METADATA_KEYS, "invalid_shelf_metadata")
    require(type(value["schema"]) is int and value["schema"] == 1 and value["kind"] == "rac2-nonmatching-entry" and value["status"] == "research-only" and type(value["integration_credit"]) is int and value["integration_credit"] == 0, "invalid_shelf_status")
    require(value["boundary_status"] == "flow_supported_inferred", "unsupported_research_extent")
    require(match(SAFE, value["task"]) and match(SAFE, value["target"]) and match(TRIAL, value["trial"]) and match(PROGRAM, value["program"]) and match(SYMBOL, value["symbol"]), "invalid_shelf_identity")
    require(integer(value["address"], 0, 2**32 - 1) and value["address"] % 4 == 0 and integer(value["size"], 4, 128 * 1024) and value["size"] % 4 == 0 and value["address"] + value["size"] <= 2**32 and integer(value["candidate_size"], 4, 128 * 1024) and value["candidate_size"] % 4 == 0 and integer(value["different_bytes"], 1, value["size"] + value["candidate_size"]), "invalid_complete_measurement")
    require(all(match(SHA, value[key]) for key in METADATA_KEYS if key.endswith("sha256") or key == "semantic_key"), "invalid_shelf_pin")
    require(isinstance(value["checked_date"], str) and re.fullmatch(r"\d{4}-\d{2}-\d{2}", value["checked_date"]), "invalid_shelf_date")
    try:
        datetime.fromisoformat(value["checked_date"])
    except ValueError:
        raise ValueError("invalid_shelf_date") from None
    require(type(value["reopen_codes"]) is list and 1 <= len(value["reopen_codes"]) <= len(REOPEN_CODES) and all(isinstance(code, str) and code in REOPEN_CODES for code in value["reopen_codes"]) and len(set(value["reopen_codes"])) == len(value["reopen_codes"]), "invalid_reopen_codes")
    return True


def entry_relative(metadata):
    validate_metadata(metadata)
    return Path(metadata["program"]) / f"{metadata['address']:08x}-{metadata['size']:x}"


def shelf_entries(repo, registry, catalog_path=None):
    """Validate public pins/canonical links; optional current target proof checks."""
    repo = Path(repo).resolve()
    shelf = inside(repo, "nonmatching")
    require(shelf.is_dir(), "missing_shelf")
    paths = sorted(shelf.rglob("*.json"))
    require(len(paths) <= MAX_ENTRIES, "shelf_capacity")
    entries, seen, sources = [], set(), set()
    for path in paths:
        require(path.resolve().is_relative_to(shelf), "escaped_shelf_path")
        value = json.loads(bounded(path, MAX_METADATA))
        validate_metadata(value)
        relative = entry_relative(value)
        expected = inside(shelf, relative.with_suffix(".json"))
        require(expected == path.resolve(), "shelf_identity_path_mismatch")
        key = (value["program"], value["address"], value["size"])
        require(key not in seen, "duplicate_shelf_identity")
        seen.add(key)
        source_path = inside(shelf, relative.with_suffix(".c"))
        source = bounded(source_path, MAX_SOURCE)
        require(digest(source) == value["source_sha256"] and not PRIVATE.search(source), "shelf_source_drift")
        sources.add(source_path)
        _task_open(registry, value["task"])
        registered = registry.get("trials", {}).get(value["trial"], {})
        require(registered.get("id") == value["trial"] and registered.get("task") == value["task"] and registered.get("state") == "mismatch" and registered.get("directory") == "runtime:trials/" + value["trial"], "stale_shelf_trial")
        require(all(registered.get(key) == value[key] for key in ("manifest_sha256", "outcome_sha256", "semantic_key")), "stale_shelf_pins")
        if catalog_path is not None:
            _current_target(repo, catalog_path, value["program"], value["address"], value["size"], value["reference_sha256"], value["reference_body_sha256"])
        entries.append(value)
    require({path.resolve() for path in shelf.rglob("*.c")} == sources, "orphan_shelf_source")
    expected_files = sources | {path.resolve() for path in paths} | {inside(shelf, "README.md")}
    require({path.resolve() for path in shelf.rglob("*") if path.is_file()} <= expected_files, "unexpected_shelf_artifact")
    ordered = sorted(entries, key=lambda value: (value["program"], value["address"]))
    require(not any(left["program"] == right["program"] and left["address"] + left["size"] > right["address"] for left, right in zip(ordered, ordered[1:])), "overlapping_shelf_extents")
    return sorted(entries, key=lambda value: (value["different_bytes"] / max(value["size"], value["candidate_size"]), abs(value["size"] - value["candidate_size"]), value["program"], value["address"]))


def render_index(entries):
    lines = ["# Authored C research shelf", "", "Generated by `python scripts/nonmatching.py index`; do not edit this index by hand.", "", "This shelf is a derived view of `config/campaign-register.json`, not a task queue.", "Every entry is research-only and adds **zero C credit**. Size mismatches retain", "both complete sizes and every missing/extra byte. No source here enters a build.", "", "Reserve work through [shared reservations](../docs/CONTRIBUTOR-RESERVATIONS.md) before resuming", "a task. Reopening still follows the canonical task and its retained history.", "", "Only a coordinator stages owner-reviewed authored C from a finalized pinned", "campaign trial. Workers preserve attempts privately. No assembly, game bytes,", "runtime paths, compiler logs or private evidence files belong in this shelf.", "", "```text", "python scripts/nonmatching.py --runtime <private-bank> stage --envelope <private-json> --backup-root <private-backups>", "python scripts/nonmatching.py index", "python scripts/nonmatching.py check", "python scripts/nonmatching.py --runtime <private-bank> check", "```", "", "The owner-reviewed envelope has schema 1, kind `rac2-nonmatching-publication`,", "task/trial/target/symbol/source_sha256, a `review` object with all five flags", "`authored_c`, `no_embedded_game_data`, `no_private_data`, `defined_behavior`,", "`abi_reviewed` set to true, and `reopen_codes` from `new_abi_evidence`,", "`new_declaration_evidence`, `new_compiler_evidence`, `new_structural_evidence`.", "Review assertions must come from the owner, never model prose. Lexical checks", "do not prove ABI or general C defined behavior. The private backup root retains", "the normalized publication receipt and every replaced public source/metadata pair.", "", "`check` validates public pins, canonical trial links, current complete extents,", "current accepted-C exclusions and index freshness without compiling. Its JSON", "explicitly reports `retained_evidence_verified: false` without a private runtime.", "The runtime form independently verifies all complete retained measurements again.", "`index` performs the public checks and refreshes only this generated file.", "Missing or stale evidence fails closed; this tool does not repair or accept it.", "", "Only flow-supported inferred extents with current normalization/reconstruction", "receipts are eligible. These remain inferred original boundaries; a retained", "complete-symbol measurement does not certify the original ABI or function extent.", "Already qualified C, unsupported inferred extents and ambiguous fragments are", "excluded, as is every overlap with current validated accepted-C placements.", "", "Staging supports one complete function in standalone C with explicit reviewed", "externals/types, using the maintained source-scope guard. It rejects assembly,", "includes, file-scope data, unreviewed helpers, private-path/secret patterns,", "special control intrinsics and direct uninitialized-return tricks. It is not a", "complete C parser or privacy scanner; owner source/ABI/behavior review is required.", "Replacements require a strictly better complete measurement and first retain", "the old public pair in private backups. Old canonical/private attempts remain.", "", f"Visible research entries: **{len(entries)}**.", ""]
    if entries:
        lines.extend(["| Program | Complete extent | Candidate bytes | Different bytes | Task / trial | Checked | Source |", "|---|---:|---:|---:|---|---|---|"])
        for value in entries:
            path = entry_relative(value).with_suffix(".c").as_posix()
            lines.append(f"| `{value['program']}` | `0x{value['address']:08X}` / {value['size']} | {value['candidate_size']} | {value['different_bytes']} | `{value['task']}` / `{value['trial']}` | {value['checked_date']} | [{value['symbol']}]({path}) |")
        lines.append("")
    else:
        lines.extend(["No attempts are staged. Existing private trials have not been imported.", ""])
    return "\n".join(lines).encode("utf-8")


@contextmanager
def publication_lock(backup_root):
    """Persistent private OS lock file; never delete another process's lock."""
    backup_root.mkdir(parents=True, exist_ok=True)
    with (backup_root / ".nonmatching.lock").open("a+b") as stream:
        if stream.tell() == 0:
            stream.write(b"0")
            stream.flush()
        stream.seek(0)
        if sys.platform == "win32":
            import msvcrt
            try:
                msvcrt.locking(stream.fileno(), msvcrt.LK_NBLCK, 1)
            except OSError:
                raise ValueError("shelf_publication_locked") from None
            try:
                yield
            finally:
                stream.seek(0)
                msvcrt.locking(stream.fileno(), msvcrt.LK_UNLCK, 1)
        else:
            import fcntl
            try:
                fcntl.flock(stream.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
            except OSError:
                raise ValueError("shelf_publication_locked") from None
            try:
                yield
            finally:
                fcntl.flock(stream.fileno(), fcntl.LOCK_UN)


def atomic_write(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + "." + uuid.uuid4().hex + ".tmp")
    with temporary.open("xb") as stream:
        stream.write(data)
    temporary.replace(path)


def stage(store, repo, catalog_path, envelope, backup_root):
    """Publish one strictly improving reviewed snapshot; retain prior public pair."""
    repo, backup_root = Path(repo).resolve(), Path(backup_root).resolve()
    require(backup_root != repo and not backup_root.is_relative_to(repo) and not repo.is_relative_to(backup_root), "private_backups_required")
    with publication_lock(backup_root):
        metadata, source = prepare_entry(store, repo, catalog_path, envelope)
        registry = store.load()
        entries = shelf_entries(repo, registry, catalog_path)
        verify_retained_entries(store, repo, catalog_path, entries)
        shelf = inside(repo, "nonmatching")
        relative = entry_relative(metadata)
        source_path, metadata_path = inside(shelf, relative.with_suffix(".c")), inside(shelf, relative.with_suffix(".json"))
        old = next((entry for entry in entries if entry_relative(entry) == relative), None)
        if old is not None:
            require(old["reference_sha256"] == metadata["reference_sha256"], "shelf_reference_changed")
            old_rank = (old["different_bytes"], abs(old["candidate_size"] - old["size"]))
            new_rank = (metadata["different_bytes"], abs(metadata["candidate_size"] - metadata["size"]))
            require(new_rank < old_rank, "not_strictly_better")
            backup = backup_root / uuid.uuid4().hex
            backup.mkdir()
            (backup / "source.c").write_bytes(bounded(source_path, MAX_SOURCE))
            (backup / "metadata.json").write_bytes(bounded(metadata_path, MAX_METADATA))
        else:
            require(len(entries) < MAX_ENTRIES, "shelf_capacity")
        # Re-evaluate immutable evidence immediately before publication.
        final_metadata, final_source = prepare_entry(store, repo, catalog_path, envelope)
        require(final_metadata == metadata and final_source == source, "evidence_changed_during_stage")
        receipt = backup_root / "publication-receipts" / (metadata["publication_receipt_sha256"] + ".json")
        receipt.parent.mkdir(parents=True, exist_ok=True)
        receipt_bytes = encoded(publication_envelope(envelope))
        if receipt.exists():
            require(bounded(receipt, MAX_METADATA) == receipt_bytes, "publication_receipt_drift")
        else:
            with receipt.open("xb") as stream:
                stream.write(receipt_bytes)
        atomic_write(source_path, source)
        atomic_write(metadata_path, encoded(metadata))
        entries = shelf_entries(repo, store.load(), catalog_path)
        atomic_write(inside(shelf, "README.md"), render_index(entries))
        return {"ok": True, "state": "research-only", "task": metadata["task"], "trial": metadata["trial"], "source_sha256": metadata["source_sha256"], "integration_credit": 0}


def verify_retained_entries(store, repo, catalog_path, entries):
    for entry in entries:
        # Reconstruct the normalized, hash-bound prior review receipt; this is
        # verification of an existing publication, never a new owner approval.
        envelope = {"schema": 1, "kind": "rac2-nonmatching-publication", "task": entry["task"], "trial": entry["trial"], "target": entry["target"], "symbol": entry["symbol"], "source_sha256": entry["source_sha256"], "review": {key: True for key in REVIEWS}, "reopen_codes": entry["reopen_codes"]}
        metadata, _ = prepare_entry(store, repo, catalog_path, envelope)
        require(metadata == entry, "shelf_evidence_drift")


def check(store, repo, catalog_path, runtime=False):
    entries = shelf_entries(repo, store.load(), catalog_path)
    if runtime:
        verify_retained_entries(store, repo, catalog_path, entries)
    require(bounded(inside(Path(repo).resolve(), "nonmatching/README.md"), 1024 * 1024) == render_index(entries), "stale_shelf_index")
    return {"ok": True, "entries": len(entries), "retained_evidence_verified": bool(runtime), "integration_credit": 0}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--catalog", type=Path)
    parser.add_argument("--runtime", type=Path)
    commands = parser.add_subparsers(dest="command", required=True)
    stage_parser = commands.add_parser("stage")
    stage_parser.add_argument("--envelope", type=Path, required=True)
    stage_parser.add_argument("--backup-root", type=Path, required=True)
    commands.add_parser("index")
    commands.add_parser("check")
    args = parser.parse_args(argv)
    repo = args.repo.resolve()
    # Import only trusted maintained modules alongside this script.
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    from campaign import Store
    require((repo / "config/campaign-register.json").is_file(), "missing_canonical_register")
    catalog = args.catalog.resolve() if args.catalog else repo / "config/function-catalog/catalog.json"
    runtime = args.runtime.resolve() if args.runtime else repo.parent / "unused-private-runtime"
    store = Store(repo / "config/campaign-register.json", runtime)
    if args.command == "stage":
        require(args.runtime is not None, "private_runtime_required")
        envelope = json.loads(bounded(args.envelope, MAX_METADATA))
        result = stage(store, repo, catalog, envelope, args.backup_root)
    elif args.command == "index":
        entries = shelf_entries(repo, store.load(), catalog)
        atomic_write(inside(repo, "nonmatching/README.md"), render_index(entries))
        result = {"ok": True, "entries": len(entries), "integration_credit": 0}
    else:
        result = check(store, repo, catalog, runtime=args.runtime is not None)
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, TypeError, ImportError, StopIteration):
        print(json.dumps({"ok": False, "reason": "nonmatching_validation_failed", "integration_credit": 0}), file=sys.stderr)
        raise SystemExit(1)
