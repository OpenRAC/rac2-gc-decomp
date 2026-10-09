"""Campaign organization over the existing RAC2 checks; no integration credit here."""
from __future__ import annotations

import argparse
import copy
import hashlib
import importlib
import json
import os
import re
import subprocess
import sys
import time
import uuid
from contextlib import contextmanager
from datetime import datetime, timezone
from pathlib import Path

CC1 = "5fed4e239d6fe3ef19d3b18483844eaf5fb1e647c5d8556652c75fc8e9a73bc6"
STATES = {"queued", "blocked", "stopped", "exact_private", "integrated", "done"}
SAFE = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]*\Z")
HASH = re.compile(r"[0-9a-f]{64}\Z")


def now():
    return datetime.now(timezone.utc).isoformat()


def digest(data):
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    return (json.dumps(value, sort_keys=True, indent=2, ensure_ascii=False) + "\n").encode("utf-8")


def read(path):
    return json.loads(Path(path).read_bytes())


def write_new(path, data):
    """Never replace an immutable manifest, snapshot, or final outcome."""
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("xb") as stream:
        stream.write(data)


def private(path, repo):
    path, repo = Path(path).resolve(), Path(repo).resolve()
    if path == repo or repo in path.parents or path in repo.parents:
        raise ValueError("Runtime/evidence must be outside and not contain the repository")
    return path


class Store:
    """One authority; exclusive mutation, atomic replace, retained prior revisions."""
    def __init__(self, path, runtime):
        self.path, self.runtime = Path(path).resolve(), Path(runtime).resolve()

    def load(self):
        value = read(self.path) if self.path.exists() else {
            "schema": 1, "kind": "rac2-campaign", "revision": 0,
            "tasks": {}, "trials": {}, "actions": {}, "legacy_documents": {}, "legacy_rows": {}}
        if value.get("schema") != 1 or value.get("kind") != "rac2-campaign":
            raise ValueError("Unsupported campaign registry")
        return value

    @contextmanager
    def edit(self):
        self.path.parent.mkdir(parents=True, exist_ok=True)
        lock = self.path.with_name(self.path.name + ".lock")
        try:
            fd = os.open(lock, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
        except FileExistsError as error:
            raise ValueError(f"Registry is locked; inspect its owner before recovery: {lock}") from error
        try:
            with os.fdopen(fd, "w", encoding="utf-8") as stream:
                stream.write(json.dumps({"pid": os.getpid(), "created": now()}))
            value = self.load()
            yield value
            value["revision"] += 1
            data = encoded(value)
            revision = self.runtime / "registry-revisions" / (uuid.uuid4().hex + ".json")
            write_new(revision, data)
            temp = self.path.with_name(self.path.name + "." + uuid.uuid4().hex + ".tmp")
            write_new(temp, data)
            # Windows readers can briefly deny delete sharing. Retry only the
            # already-written atomic rename, never the mutation or its evidence.
            for attempt in range(11):
                try:
                    os.replace(temp, self.path)
                    break
                except OSError as error:
                    if getattr(error, "winerror", None) not in (5, 32, 33) or attempt == 10:
                        raise
                    time.sleep(.05)
        finally:
            lock.unlink()


class TrialPublicationError(RuntimeError):
    """A real trial's retained evidence requires explicit registry recovery."""
    def __init__(self, trial_id, outcome_path, reason):
        self.trial_id = trial_id
        self.outcome_path = Path(outcome_path)
        self.reason = str(reason)
        super().__init__(f"Trial {trial_id} final publication failed; "
                         f"outcome path: {self.outcome_path}; reason: {self.reason}. "
                         "Inspect retained evidence and recover the registry; do not replay compilation.")


def normalize_task(task):
    task = copy.deepcopy(task)
    if not isinstance(task, dict) or not SAFE.fullmatch(task.get("id", "")):
        raise ValueError("Task requires a safe unique id")
    kind = task.setdefault("kind", "candidate")
    if kind not in {"candidate", "research"}:
        raise ValueError("Task kind must be candidate or research")
    state = task.setdefault("state", "queued")
    if state not in STATES or (state == "done" and kind != "research"):
        raise ValueError("Invalid task state; candidate exact results remain exact_private")
    if state in {"exact_private", "integrated"}:
        raise ValueError("New tasks cannot assert a match or integration without proof")
    if not isinstance(task.get("next_action"), str) or not task["next_action"].strip():
        raise ValueError("Task requires a concrete next_action")
    task.setdefault("hypothesis", "")
    task.setdefault("reopen_condition", "")
    task.setdefault("pointers", [])
    if kind == "candidate":
        if not isinstance(task.get("source"), str) or not task["source"]:
            raise ValueError("Candidate task requires a source path")
        targets = task.get("targets")
        if not isinstance(targets, list) or not targets:
            raise ValueError("Candidate task requires targets")
        names = []
        for target in targets:
            if not SAFE.fullmatch(target.get("id", "")):
                raise ValueError("Each child target requires a safe id")
            names.append(target["id"])
            if not all(isinstance(target.get(key), str) and target[key] for key in ("reference", "catalog")):
                raise ValueError("Target requires reference and catalog paths")
        if len(set(names)) != len(names):
            raise ValueError("Duplicate target id")
    return task


def plan(store, tasks):
    tasks = [normalize_task(task) for task in (tasks if isinstance(tasks, list) else [tasks])]
    if len({task["id"] for task in tasks}) != len(tasks):
        raise ValueError("Duplicate task in plan")
    with store.edit() as registry:
        for task in tasks:
            if task["id"] in registry["tasks"]:
                raise ValueError("Task already exists; queue state updates preserve its history")
            task.update(created=now(), transitions=[])
            registry["tasks"][task["id"]] = task
    return {"planned": [task["id"] for task in tasks]}


def transition(store, task_id, state, reason, reopen=""):
    if state not in STATES or not reason.strip():
        raise ValueError("State change requires a known state and a reason")
    with store.edit() as registry:
        task = registry["tasks"][task_id]
        if task.get("active_trial"):
            raise ValueError("Cannot change a task with an active trial")
        if state in {"exact_private", "integrated"}:
            raise ValueError("Only a validated trial or integration proof can set a matching state")
        if state == "done" and task["kind"] != "research":
            raise ValueError("Candidate task closure needs integration evidence; no manual credit")
        task["transitions"].append({"at": now(), "from": task["state"], "to": state,
                                    "reason": reason, "reopen_condition": reopen})
        task.update(state=state, reopen_condition=reopen or task.get("reopen_condition", ""))
    return {"task": task_id, "state": state}


def amend(store, task_id, changes, reason):
    allowed = {"source", "targets", "symbols", "pointers", "next_action", "hypothesis",
               "reopen_condition", "priority", "budget", "program", "address", "size",
               "context", "abi", "types", "remaining_difference", "best_source"}
    if not isinstance(changes, dict) or set(changes) - allowed or not reason.strip():
        raise ValueError("Amend requires allowed planning fields and a concrete reason")
    with store.edit() as registry:
        task = registry["tasks"][task_id]
        if task.get("active_trial"):
            raise ValueError("Cannot amend a task with an active trial")
        candidate = {**task, **changes, "state": "queued"}
        normalize_task(candidate)
        task.setdefault("amendments", []).append({"at": now(), "reason": reason,
                                                  "before": {key: task.get(key) for key in changes}, "changes": changes})
        task.update(changes)
        if task["state"] in {"exact_private", "integrated"} and set(changes) & {"source", "targets"}:
            task["transitions"].append({"at": now(), "from": task["state"], "to": "stopped", "reason": reason})
            task["state"] = "stopped"
    return {"task": task_id, "amended": sorted(changes)}


def legacy_import(store, path):
    """Retain all original UTF-8 bytes and every experiment row, without inferred validity."""
    path = Path(path)
    raw = path.read_bytes()
    content, document_id = raw.decode("utf-8"), digest(raw)
    rows, header, section = [], None, ""
    for number, line in enumerate(content.splitlines(), 1):
        if line.startswith("#"):
            section, header = line, None
        if not line.startswith("|"):
            continue
        cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
        if cells and all(re.fullmatch(r":?-+:?", cell.strip()) for cell in cells):
            continue
        if (("Trial" in cells and "Outcome" in cells)
                or ("Trial record" in cells and "Result" in cells)
                or ("Probe" in cells and "Private evidence" in cells)):
            header = cells
            continue
        if header is None or len(cells) != len(header):
            continue
        row = dict(zip(header, cells))
        kind = "runtime_observation" if "Probe" in header else "source_trial"
        evidence = row.get("Evidence directory", row.get("Private evidence", "")).strip("`").replace("\\", "/").rstrip("/")
        row_id = document_id + ":" + str(number)
        rows.append((row_id, {"document": document_id, "line": number, "section": section,
                              "raw": line, "cells": row, "evidence_key": evidence, "kind": kind,
                              "validity": "historical_unverified"}))
    with store.edit() as registry:
        if document_id in registry["legacy_documents"]:
            return {"already_imported": document_id, "rows": 0}
        probable_repo = store.path.parent.parent if store.path.parent.name == "config" else None
        source_name = path.resolve().relative_to(probable_repo).as_posix() if probable_repo and path.resolve().is_relative_to(probable_repo) else path.name
        registry["legacy_documents"][document_id] = {
            "source": source_name, "sha256": document_id, "original_utf8": content,
            "imported": now(), "row_count": len(rows), "credit": 0}
        existing = {(row["kind"], row["raw"], row["evidence_key"]) for row in registry["legacy_rows"].values()}
        fresh_rows = [(ident, row) for ident, row in rows
                      if (row["kind"], row["raw"], row["evidence_key"]) not in existing]
        registry["legacy_rows"].update(fresh_rows)
    return {"document": document_id, "rows": len(fresh_rows), "unique_evidence_directories":
            len({row["evidence_key"] for _, row in rows if row["evidence_key"]})}


def summary(registry):
    trials = list(registry["trials"].values())
    legacy = [row for row in registry["legacy_rows"].values() if row["kind"] == "source_trial"]
    states = {}
    for task in registry["tasks"].values():
        states[task["state"]] = states.get(task["state"], 0) + 1
    return {"task_states": states, "trial_count": len(trials),
            "compilation_attempts": sum(item.get("compile_attempted", False) for item in trials),
            "failed_compilation_trials": sum(item.get("state") == "compile_failed" for item in trials),
            "target_measurements": sum(item.get("measured_functions", 0) for item in trials),
            "pending_trials": [item["id"] for item in trials if item["state"] == "running"],
            "legacy_target_rows": len(legacy),
            "legacy_runtime_observations": sum(row["kind"] == "runtime_observation" for row in registry["legacy_rows"].values()),
            "legacy_unique_evidence_directories": len({row["evidence_key"] for row in legacy if row["evidence_key"]}),
            "legacy_failed_compilation_evidence_directories": len({row["evidence_key"] for row in legacy
                if row["evidence_key"] and "COMPILE" in row["cells"].get("Outcome", row["cells"].get("Result", "")).upper()}),
            "integration_credit": 0}


def instrument_hashes(repo):
    paths = [path for directory in ("scripts", "config", "candidates", "src", "progress")
             for path in (repo / directory).rglob("*")
             if path.is_file() and path.suffix in {".py", ".json", ".c", ".h", ".cfrag"}
             and path.name != "campaign-register.json"]
    return {path.relative_to(repo).as_posix(): digest(path.read_bytes()) for path in sorted(paths)}


class PublicBackend:
    """Use the maintained checker/compiler APIs, without changing them."""
    def __init__(self, repo):
        sys.path.insert(0, str(repo / "scripts"))
        self.check = importlib.import_module("check_candidates")
        self.chain = importlib.import_module("wsl_chain")
        if Path(self.check.__file__).resolve().parent != repo / "scripts":
            raise ValueError("Another repository's checker is already imported")

    def tools(self, toolchain):
        return self.chain.tool_hashes(toolchain)

    def compile(self, source, flags, obj, assembly, log):
        return self.chain.compile_c(source, flags, obj, assembly, log)

    def measure(self, obj, catalog, reference, toolchain, work):
        script = work / "candidate.ld"
        write_new(script, self.check.linker_script(catalog).encode("ascii"))
        linked = work / "candidate.elf"
        self.check.run([str(toolchain / "ee/bin/ld.exe"), "-T", str(script), "-o", str(linked), str(obj)], work / "link.log")
        results = [self.check.compare_function(reference, linked, f["symbol"], f["address"], f["size"])
                   for f in catalog["functions"]]
        data = self.check.compare_readonly(reference, linked, catalog, obj)
        return {"functions": results, "read_only_sections": data,
                "candidate_elf_sha256": digest(linked.read_bytes())}


def absolute(path, repo, runtime=None):
    if str(path).startswith("runtime:"):
        if runtime is None:
            raise ValueError("runtime: path needs an explicit runtime binding")
        resolved = (runtime / str(path)[8:]).resolve()
        if not resolved.is_relative_to(runtime.resolve()):
            raise ValueError("runtime: path escaped its binding")
        return resolved
    path = Path(path)
    return (repo / path).resolve() if not path.is_absolute() else path.resolve()


def _trial(store, repo, task_id, toolchain, profile, repeat_reason="", backend=None):
    """One source compilation, any number of linked/measured child targets."""
    task = copy.deepcopy(store.load()["tasks"][task_id])
    if task["kind"] != "candidate":
        raise ValueError("Research tasks do not run compiler trials")
    if task["state"] in {"blocked", "stopped"} and not repeat_reason.strip():
        raise ValueError("Reopening a stopped task requires --repeat-reason")
    source = absolute(task["source"], repo, store.runtime)
    if not SAFE.fullmatch(source.name) or source.suffix != ".c":
        raise ValueError("Use a plain C filename safe for the existing WSL chain")
    content = source.read_bytes()
    if re.search(rb"\b(?:asm|__asm__|__asm|INCLUDE_ASM)\b|(?m:^[ \t]*(?:(?:[A-Za-z_.$][A-Za-z0-9_.$]*|[0-9]+):[ \t]*)?\.(?:byte|word)\b(?:[ \t]+(?![ \t]*=)\S|[ \t]*$))", content):
        raise ValueError("Candidate embeds assembly or retail bytes")
    if re.search(rb"(?m)^\s*#\s*include\b|\b__(?:DATE|TIME|TIMESTAMP)__\b", content):
        raise ValueError("Candidate must be standalone and reproducible until headers are pinned")
    expected_tools = read(profile)["tools"]
    if expected_tools.get("cc1") != CC1 or any(not HASH.fullmatch(expected_tools.get(name, ""))
                                              for name in ("cc1", "cpp", "as", "ld.exe")):
        raise ValueError("Trials require the pinned current GNU8bed profile")
    targets, flags = [], None
    for descriptor in task["targets"]:
        catalog_path = absolute(descriptor["catalog"], repo, store.runtime)
        reference_path = absolute(descriptor["reference"], repo, store.runtime)
        catalog_bytes, reference_bytes = catalog_path.read_bytes(), reference_path.read_bytes()
        catalog = json.loads(catalog_bytes)
        program = catalog.get("program", "boot")
        # The catalogue names its own release; only that region's pinned identities apply.
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        try:
            owner = importlib.import_module("region").by_serial(catalog.get("target"), repo)
            owner.require_matching("C trials")
            refs = owner.program_pins()
        except ValueError as error:
            raise ValueError(f"Reference/catalog is not a pinned RAC2 program ({error})") from error
        if (program not in refs
                or digest(reference_bytes) != refs[program] or catalog.get("reference_sha256") != refs[program]):
            raise ValueError("Reference/catalog is not a pinned RAC2 program")
        current_flags = catalog.get("flags")
        if not isinstance(current_flags, list) or not current_flags or any(
                not isinstance(flag, str) or not re.fullmatch(r"-[A-Za-z0-9_=.+-]+", flag) for flag in current_flags):
            raise ValueError("Invalid compiler flags")
        if flags is not None and flags != current_flags:
            raise ValueError("One trial cannot compile different flag sets")
        flags = current_flags
        functions = catalog.get("functions", [])
        if not functions or len({f["symbol"] for f in functions}) != len(functions):
            raise ValueError("Catalog needs unique complete functions")
        for function in functions:
            if (not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", function["symbol"])
                    or type(function.get("address")) is not int or not 0 <= function["address"] <= 0xFFFFFFFF or function["address"] % 4
                    or type(function.get("size")) is not int or function["size"] <= 0 or function["size"] % 4):
                raise ValueError("Invalid complete function identity")
        ordered = sorted(functions, key=lambda f: f["address"])
        if any(a["address"] + a["size"] > b["address"] for a, b in zip(ordered, ordered[1:])):
            raise ValueError("Overlapping complete function bodies")
        externals = catalog.setdefault("externals", {})
        if not isinstance(externals, dict) or any(not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name)
                or type(address) is not int or not 0 <= address <= 0xFFFFFFFF
                or name in {f["symbol"] for f in functions} for name, address in externals.items()):
            raise ValueError("Invalid external identity or defined/external symbol collision")
        gp = catalog.setdefault("gp", 0)
        if type(gp) is not int or not 0 <= gp <= 0xFFFFFFFF or gp % 4:
            raise ValueError("Invalid gp")
        targets.append({"id": descriptor["id"], "program": program, "catalog": catalog,
                        "catalog_bytes": catalog_bytes, "reference_bytes": reference_bytes,
                        "catalog_sha256": digest(catalog_bytes), "reference_sha256": digest(reference_bytes)})
    semantic = {"source_sha256": digest(content), "source_name": source.name, "flags": flags,
                "tools": expected_tools, "targets": sorted([{key: target[key] for key in
                    ("program", "catalog_sha256", "reference_sha256")} for target in targets],
                    key=lambda t: (t["program"], t["catalog_sha256"], t["reference_sha256"]))}
    key = digest(encoded(semantic))
    backend = backend or PublicBackend(repo)
    before_tools = backend.tools(toolchain)
    if before_tools != expected_tools:
        raise ValueError("Actual tools differ from the requested current profile")
    instruments = instrument_hashes(repo)
    trial_id = uuid.uuid4().hex
    work = store.runtime / "trials" / trial_id
    with store.edit() as registry:
        active = registry["tasks"][task_id]
        if active.get("active_trial"):
            raise ValueError("Task already has an active trial")
        duplicate = [item["id"] for item in registry["trials"].values() if item["semantic_key"] == key]
        if duplicate and not repeat_reason.strip():
            raise ValueError("Semantic duplicate trial: " + ", ".join(duplicate) + "; provide --repeat-reason")
        if active != task:
            raise ValueError("Task changed while preparing trial")
        budget = active.get("budget")
        used = sum(item["task"] == task_id for item in registry["trials"].values())
        if budget is not None and (type(budget) is not int or budget <= used):
            raise ValueError("Task trial budget exhausted; plan a reviewed follow-up task")
        registry["trials"][trial_id] = {"id": trial_id, "task": task_id, "semantic_key": key,
                                       "state": "running", "started": now(), "directory": "runtime:trials/" + trial_id}
        active["active_trial"] = trial_id
    result = {"id": trial_id, "state": "preparation_failed", "compile_attempted": False,
              "measured_functions": 0, "children": [], "integration_credit": 0}
    try:
        write_new(work / "source" / source.name, content)
        for target in targets:
            child = work / "targets" / target["id"]
            write_new(child / "catalog.json", target["catalog_bytes"])
            write_new(child / "reference.elf", target["reference_bytes"])
        manifest = {"schema": 1, "kind": "rac2-candidate-trial", "id": trial_id, "task": task_id,
                    "created": now(), "semantic_key": key, "semantic_inputs": semantic,
                    "repeat_reason": repeat_reason, "prior_semantic_trials": duplicate,
                    "task_snapshot": task, "profile_sha256": digest(Path(profile).read_bytes()),
                    "instruments": instruments, "scope": "private candidate check; never integration credit"}
        write_new(work / "manifest.json", encoded(manifest))
        result["state"] = "compile_failed"
        result["compile_attempted"] = True
        backend.compile(work / "source" / source.name, flags, work / "candidate.o", work / "candidate.s", work / "compile.log")
        if not (work / "candidate.o").is_file():
            raise ValueError("Compiler returned without a fresh object")
        result["object_sha256"] = digest((work / "candidate.o").read_bytes())
        result["state"] = "mismatch"
        for target in targets:
            child = work / "targets" / target["id"]
            try:
                measured = backend.measure(work / "candidate.o", target["catalog"], child / "reference.elf", toolchain, child)
                expected = {f["symbol"] for f in target["catalog"]["functions"]}
                functions = measured["functions"]
                if len(functions) != len(expected) or {f["symbol"] for f in functions} != expected:
                    raise ValueError("Checker omitted or duplicated a complete target function")
                exact = all(f.get("matched") is True and HASH.fullmatch(f.get("reference_sha256", ""))
                            and f.get("reference_sha256") == f.get("candidate_sha256") for f in functions)
                if target["catalog"].get("read_only_sections"):
                    from check_candidates import require_exact_readonly
                    try:
                        require_exact_readonly(target["catalog"], measured.get("read_only_sections", []))
                    except ValueError:
                        exact = False
                outcome = {"id": target["id"], "state": "exact_private" if exact else "mismatch", **measured}
                result["measured_functions"] += len(functions)
            except (OSError, ValueError, KeyError) as error:
                outcome = {"id": target["id"], "state": "link_or_check_failed", "error": str(error), "functions": []}
            write_new(child / "outcome.json", encoded(outcome))
            result["children"].append(outcome)
        candidate_state = ("exact_private" if all(child["state"] == "exact_private"
                                                  for child in result["children"]) else "mismatch")
        result["state"] = "provenance_check_failed"
        if backend.tools(toolchain) != before_tools or instrument_hashes(repo) != instruments:
            result["state"] = "provenance_changed"
            result["error"] = "Compiler, checker, catalog, or repository source changed during trial"
        else:
            result["state"] = candidate_state
    except (OSError, ValueError, KeyError) as error:
        result["error"] = str(error)
    finally:
        if not result["children"]:
            result["children"] = [{"id": target["id"], "state": "unmeasured",
                                   "reason": "shared unit did not reach target measurement", "functions": []}
                                  for target in targets]
        result["finished"] = now()
        try:
            write_new(work / "outcome.json", encoded(result))
            with store.edit() as registry:
                registry["trials"][trial_id].update({k: result[k] for k in
                    ("state", "compile_attempted", "measured_functions", "finished")})
                registry["trials"][trial_id]["outcome_sha256"] = digest(encoded(result))
                manifest_path = work / "manifest.json"
                registry["trials"][trial_id]["manifest_sha256"] = digest(manifest_path.read_bytes()) if manifest_path.exists() else None
                active = registry["tasks"][task_id]
                active.pop("active_trial", None)
                state = "exact_private" if result["state"] == "exact_private" else "stopped"
                active["transitions"].append({"at": now(), "from": active["state"], "to": state,
                                              "reason": result["state"], "trial": trial_id})
                active.update(state=state, last_trial=trial_id)
        except (OSError, ValueError, KeyError, TypeError) as error:
            # This trial may already have compiled and measured. Its identity
            # must never become a fresh, falsely uncompiled preparation rejection.
            raise TrialPublicationError(trial_id, work / "outcome.json", error) from error
    return result


def trial(store, repo, task_id, toolchain, profile, repeat_reason="", backend=None):
    """Also retain pre-compilation rejections; never alter an existing active trial."""
    try:
        return _trial(store, repo, task_id, toolchain, profile, repeat_reason, backend)
    except (OSError, ValueError, KeyError, TypeError) as error:
        task = copy.deepcopy(store.load()["tasks"].get(task_id, {}))
        rejected_id = uuid.uuid4().hex
        work = store.runtime / "trials" / rejected_id
        available = {}
        paths = [("source", task.get("source")), ("profile", str(profile))] + [
                    (key + "-" + str(i), target.get(key)) for i, target in enumerate(task.get("targets", []))
                    for key in ("catalog", "reference")]
        for name, path in paths:
            if path:
                try:
                    data = absolute(path, repo, store.runtime).read_bytes()
                    available[name] = digest(data)
                    write_new(work / "rejected-inputs" / (name + ".snapshot"), data)
                except (OSError, ValueError):
                    pass
        manifest = {"schema": 1, "kind": "rac2-preparation-rejected", "id": rejected_id,
                    "task": task_id, "task_snapshot": task, "available_sha256": available, "error": str(error)}
        result = {"id": rejected_id, "state": "preparation_rejected", "compile_attempted": False,
                  "measured_functions": 0, "children": [{"id": target["id"], "state": "unmeasured"}
                    for target in task.get("targets", [])], "error": str(error), "finished": now(), "integration_credit": 0}
        write_new(work / "manifest.json", encoded(manifest))
        write_new(work / "outcome.json", encoded(result))
        with store.edit() as registry:
            registry["trials"][rejected_id] = {**result, "task": task_id, "directory": "runtime:trials/" + rejected_id,
                "semantic_key": digest(encoded(available)), "manifest_sha256": digest(encoded(manifest)),
                "outcome_sha256": digest(encoded(result))}
        return result


def packet(store, repo, task_id):
    registry = store.load()
    task = registry["tasks"][task_id]
    symbols = set(task.get("symbols", []))
    if not symbols:
        for target in task.get("targets", []):
            try:
                symbols.update(f["symbol"] for f in read(absolute(target["catalog"], repo, store.runtime))["functions"])
            except (OSError, ValueError, KeyError):
                pass
    symbols.update(match.group() for symbol in list(symbols)
                   if (match := re.search(r"FUN_[0-9A-Fa-f]+", symbol)))
    rows = [(ident, row) for ident, row in registry["legacy_rows"].items()
            if any(symbol in row["raw"] for symbol in symbols)]
    rows.sort(key=lambda item: (registry["legacy_documents"][item[1]["document"]]["imported"], item[1]["line"]))
    latest = registry["trials"].get(task.get("last_trial", ""))
    return {"task": task, "latest_trial": latest, "legacy_row_ids": [ident for ident, _ in rows],
            "recent_legacy_rows": [row for _, row in rows[-8:]],
            "rules": ["Use trial; do not invoke an ad-hoc compiler", "Retain every negative result",
                      "exact_private has zero integration credit", "Reopen only with a concrete new condition"]}


def views(store, repo, check=False):
    """Render disposable queue and historical views from the one authority."""
    registry = store.load()
    lines = ["# Campaign queue", "", "Generated from `config/campaign-register.json` by",
             "`scripts/campaign.py views`. Edit tasks through the CLI, then regenerate.", "",
             "Matching credit remains in the strict integration proofs, never this queue.", ""]
    for task_id, task in sorted(registry["tasks"].items()):
        lines += [f"## {task_id}", "", f"State: `{task['state']}`. Kind: `{task['kind']}`.", "",
                  task["next_action"], ""]
        if task.get("reopen_condition"):
            lines += ["Reopen condition: " + task["reopen_condition"], ""]
        for pointer in task.get("pointers", []):
            lines += ["- `" + pointer + "`"]
        if task.get("pointers"):
            lines += [""]
    history = ["# Historical C experiment view", "",
               "Generated from the immutable legacy documents in",
               "`config/campaign-register.json`. This file is a historical view;",
               "current tasks and new trials live only in that structured register.", "",
               "The original text below is preserved verbatim, including dated terminology.", ""]
    for ident, document in sorted(registry["legacy_documents"].items()):
        history += [f"Original document SHA-256: `{ident}`.", "", document["original_utf8"]]
    outputs = {"docs/CAMPAIGN-QUEUE.md": ("\n".join(lines).rstrip() + "\n").encode(),
               "docs/C-NATIVE-EXPERIMENT-REGISTER.md": ("\n".join(history).rstrip() + "\n").encode()}
    prior_hashes = registry.get("view_sha256", {})
    backups = {}
    for relative, data in outputs.items():
        destination = repo / relative
        if check:
            if not destination.exists() or destination.read_bytes() != data:
                raise ValueError("Stale campaign view: " + relative)
        elif destination.exists() and destination.read_bytes() != data:
            current = destination.read_bytes()
            allowed = {prior_hashes.get(relative)}
            if relative.endswith("C-NATIVE-EXPERIMENT-REGISTER.md"):
                allowed.update(registry["legacy_documents"])
            if digest(current) not in allowed:
                raise ValueError("Unregistered view edit; preserve and import it before regeneration: " + relative)
            backups[relative] = current
    if not check:
        with store.edit() as current:
            if current != registry:
                raise ValueError("Registry changed while preparing views")
            for relative, data in backups.items():
                write_new(store.runtime / "view-backups" / uuid.uuid4().hex / Path(relative).name, data)
                if (repo / relative).read_bytes() != data:
                    raise ValueError("View changed during preflight: " + relative)
            for relative, data in outputs.items():
                destination = repo / relative
                destination.parent.mkdir(parents=True, exist_ok=True)
                destination.write_bytes(data)
            current["view_sha256"] = {relative: digest(data) for relative, data in outputs.items()}
    return {"views": list(outputs), "checked": check}


def seed_history(store, repo):
    """One-time, conservative triage from retained rows and validated public proofs."""
    registry = store.load()
    if registry["tasks"]:
        raise ValueError("History seeding requires an empty task registry; never replace live triage")
    integrated = validated_integrations(repo)
    accepted = {(program, re.search(r"FUN_[0-9A-F]+$", symbol).group())
                for program, symbol, _, _ in integrated}
    groups = {}
    ordered_rows = sorted(registry["legacy_rows"].items(),
                          key=lambda item: (registry["legacy_documents"][item[1]["document"]]["imported"], item[1]["line"]))
    for ident, row in ordered_rows:
        if row["kind"] != "source_trial":
            continue
        cells = row["cells"]
        function = re.search(r"FUN_[0-9A-F]{8}", cells.get("Function", cells.get("Target", "")))
        if function:
            program = cells.get("Program", "24_ship_shack").strip("`")
            if "boot" in program.lower():
                program = "boot"
            groups.setdefault((program, function.group()), []).append((ident, row))
    target = read(repo / "config/target.json")
    pins = {"boot": target["boot"]["sha256"]}
    pins.update({r["level"]: r["sha256"] for r in read(repo / "config/overlays.json")["levels"]})
    tasks = []
    for (program, symbol), rows in sorted(groups.items()):
        done = (program, symbol) in accepted
        winner = program == "24_ship_shack" and symbol == "FUN_002A4468"
        latest = rows[-1][1]
        task = {"id": program + "-" + symbol.lower(), "kind": "research",
                "state": "done" if done else "queued" if winner else "stopped",
                "program": program, "address": int(symbol[4:], 16), "symbols": [symbol],
                "reference_sha256": pins[program], "legacy_row_ids": [i for i, _ in rows],
                "latest_recorded_outcome": latest["cells"].get("Outcome", latest["cells"].get("Result")),
                "hypothesis": "Historical source evidence; no new compilation or matching claim",
                "next_action": ("Already covered by current validated integration proofs; no repeat trial."
                                if done else "Isolate the recorded private 76-byte winner and qualify its complete unit before integration."
                                if winner else "Keep parked; inspect the packet before proposing a measured new hypothesis."),
                "reopen_condition": "A concrete new ABI/type/layout/compiler or algorithm observation; preserve prior refusals",
                "pointers": ["docs/C-NATIVE-EXPERIMENT-REGISTER.md", "private-work:" + latest["evidence_key"]]}
        if done:
            task["pointers"].append("progress/integration.json" if program == "boot" else "progress/levels/" + program + ".json")
        tasks.append(task)
    return plan(store, tasks)


def refresh_history(store):
    """Refresh dated pointers after migration; preserve every task state and decision."""
    with store.edit() as registry:
        rows = sorted(registry["legacy_rows"].items(), key=lambda item: (
            registry["legacy_documents"][item[1]["document"]]["imported"], item[1]["line"]))
        updated = []
        for task in registry["tasks"].values():
            matching = [(ident, row) for ident, row in rows if ident in task.get("legacy_row_ids", [])]
            if not matching:
                continue
            latest = matching[-1][1]
            previous = task.get("latest_recorded_outcome")
            task["latest_recorded_outcome"] = latest["cells"].get("Outcome", latest["cells"].get("Result"))
            task["legacy_row_ids"] = [ident for ident, _ in matching]
            task["legacy_latest_evidence"] = "private-work:" + latest["evidence_key"]
            if previous != task["latest_recorded_outcome"]:
                task.setdefault("amendments", []).append({"at": now(), "reason": "Order historical results by original numeric line and import time",
                                                          "before_outcome": previous, "outcome": task["latest_recorded_outcome"]})
            updated.append(task["id"])
    return {"history_refreshed": len(updated)}


def validated_integrations(repo):
    """Use the existing exporter validators, including source/object/full-image gates."""
    sys.path.insert(0, str(repo / "scripts"))
    exporter = importlib.import_module("decomp_report")
    if Path(exporter.__file__).resolve().parent != repo / "scripts":
        raise ValueError("Another repository's proof exporter is already imported")
    boot = read(repo / "progress/integration.json")
    levels = [read(path) for path in sorted((repo / "progress/levels").glob("*.json"))]
    expected = {row["level"] for row in read(repo / "config/overlays.json")["levels"]}
    if len(levels) != 27 or {row.get("program") for row in levels} != expected:
        raise ValueError("Closure requires all 27 current level proofs")
    exporter.generate(read(repo / "config/progress-scope.json"), read(repo / "config/target.json"),
                      read(repo / "config/overlays.json"), read(repo / "progress/report.json"), boot, levels)
    exporter.validate_object_proof(boot, read(repo / "progress/candidates.json"))
    return {("boot", f["symbol"], f["address"], f["size"]) for f in boot["functions"]} | {
        (row["program"], f["symbol"], f["address"], f["size"]) for row in levels for f in row["functions"]}


def close(store, repo, task_id, validator=validated_integrations):
    task = copy.deepcopy(store.load()["tasks"][task_id])
    if task["kind"] != "candidate" or task.get("active_trial"):
        raise ValueError("Close requires an idle candidate task")
    before = instrument_hashes(repo)
    catalogs = {absolute(d["catalog"], repo, store.runtime): None for d in task["targets"]}
    catalogs = {path: digest(path.read_bytes()) for path in catalogs}
    integrated = validator(repo)
    target = read(repo / "config/target.json")
    pins = {"boot": target["boot"]["sha256"]}
    pins.update({r["level"]: r["sha256"] for r in read(repo / "config/overlays.json")["levels"]})
    for descriptor in task["targets"]:
        catalog = read(absolute(descriptor["catalog"], repo, store.runtime))
        program = catalog.get("program", "boot").removeprefix("levels/")
        if catalog.get("target") != target["serial"] or catalog.get("reference_sha256") != pins.get(program):
            raise ValueError("Closure target catalog does not identify the pinned program")
        functions = catalog.get("functions", [])
        if not functions or any((program, f["symbol"], f["address"], f["size"]) not in integrated
                                for f in functions):
            raise ValueError("Task contains a function without a current complete integration proof")
    proof_paths = [repo / "progress/integration.json", repo / "progress/report.json",
                   repo / "progress/candidates.json", *sorted((repo / "progress/levels").glob("*.json"))]
    hashes = {p.relative_to(repo).as_posix(): digest(p.read_bytes()) for p in proof_paths}
    with store.edit() as registry:
        current = registry["tasks"][task_id]
        if current != task:
            raise ValueError("Task changed during closure verification")
        if (instrument_hashes(repo) != before
                or any(digest(path.read_bytes()) != value for path, value in catalogs.items())):
            raise ValueError("Proof, source, catalog, or checker changed during closure verification")
        current["transitions"].append({"at": now(), "from": current["state"], "to": "integrated",
                                      "reason": "Current published source/object/full-image proofs validated",
                                      "proof_sha256": hashes})
        current["state"] = "integrated"
    return {"task": task_id, "state": "integrated", "integration_credit_added": 0}


def facade(store, repo, command, arguments, runner=subprocess.run):
    """Build/report existing proof pipelines; retain private logs and never publish proofs."""
    action_id = uuid.uuid4().hex
    work = store.runtime / "actions" / action_id
    work.mkdir(parents=True)
    script = "decomp_report.py" if command == "report" else "campaign_build.py"
    args = list(arguments)
    if command in {"build", "integrate"}:
        if "--batch-id" in args:
            raise ValueError("Campaign owns the fresh batch identity")
        args += ["--batch-id", action_id]
        for option, default in (("--program-jobs", "4"), ("--jobs", "2")):
            if option not in args:
                args += [option, default]
    if command == "report":
        if "--output" in args:
            private(Path(args[args.index("--output") + 1]), repo)
        else:
            args += ["--output", str(work / "report.json")]
        if Path(args[args.index("--output") + 1]).exists():
            raise ValueError("Report output already exists; use a new private artifact path")
    else:
        if "--manifest" not in args:
            raise ValueError("Build facade requires --manifest")
        private(Path(args[args.index("--manifest") + 1]), repo)
    before = instrument_hashes(repo)
    external_inputs = {str(Path(args[i + 1]).resolve()): digest(Path(args[i + 1]).read_bytes())
                       for i, option in enumerate(args[:-1]) if option in {
                           "--manifest", "--sdk-binding", "--candidate-review", "--integration-proof", "--progress-proof", "--level-proof"}}
    invocation = [sys.executable, str(repo / "scripts" / script), *args]
    write_new(work / "manifest.json", encoded({"id": action_id, "kind": command, "created": now(),
                                              "command": invocation, "instruments": before, "external_input_sha256": external_inputs}))
    with store.edit() as registry:
        registry["actions"][action_id] = {
            "id": action_id, "kind": command, "state": "running",
            "directory": "runtime:actions/" + action_id,
            "action_manifest_sha256": digest((work / "manifest.json").read_bytes()),
        }
    with (work / "run.log").open("xb") as log:
        try:
            completed = runner(invocation, cwd=repo, stdout=log, stderr=subprocess.STDOUT)
            result = {"id": action_id, "returncode": completed.returncode,
                      "state": "passed" if completed.returncode == 0 else "failed"}
        except OSError as error:
            result = {"id": action_id, "returncode": None, "state": "failed", "error": str(error)}
    if instrument_hashes(repo) != before or any(not Path(path).is_file() or digest(Path(path).read_bytes()) != value
                                               for path, value in external_inputs.items()):
        result["state"] = "provenance_changed"
    if result["state"] == "passed":
        try:
            if command == "report":
                artifact = Path(args[args.index("--output") + 1]).resolve()
            else:
                last = (work / "run.log").read_text(encoding="utf-8").strip().splitlines()[-1]
                announced = json.loads(last)
                artifact = private(announced["report"], repo)
                proof = read(artifact)
                if proof.get("batch_id") != action_id or artifact.parent.name != "campaign-" + action_id:
                    raise ValueError("Gate artifact does not belong to this fresh campaign action")
                expected_levels = {row["level"] for row in read(repo / "config/overlays.json")["levels"]}
                levels = proof.get("g3", [])
                if (proof.get("matched") is not True or proof.get("failures") or len(levels) != 27
                        or {row.get("level") for row in levels} != expected_levels
                        or not isinstance(proof.get("g1"), dict) or proof["g1"].get("matched") is not True
                        or not all(row.get("matched") is True for row in levels)):
                    raise ValueError("Full gate output did not confirm all programs")
                proof_inputs = proof.get("input_sha256", {})
                required_inputs = {"scripts/check_candidates.py", "scripts/wsl_chain.py",
                                   "config/candidate-catalog.json", "candidates/boot.c"}
                if (not required_inputs.issubset(proof_inputs)
                        or any(before.get(path) != value for path, value in proof_inputs.items())):
                    raise ValueError("Full gate source/checker hashes disagree with the campaign inputs")
                manifest_input = str(Path(args[args.index("--manifest") + 1]).resolve())
                if proof.get("manifest_sha256") != external_inputs[manifest_input]:
                    raise ValueError("Full gate reference manifest identity changed")
                if "--toolchain" not in args:
                    raise ValueError("Full gate lacks toolchain identity")
                toolchain = Path(args[args.index("--toolchain") + 1]).resolve()
                for name in ("Ps2EeAs.exe", "ld.exe"):
                    if proof.get("tools", {}).get(name) != digest((toolchain / "ee/bin" / name).read_bytes()):
                        raise ValueError("Full gate reconstruction instrument identity changed")
            result.update(artifact=str(artifact), artifact_sha256=digest(artifact.read_bytes()))
        except (OSError, ValueError, KeyError, IndexError) as error:
            result.update(state="failed", error=f"Missing or invalid fresh proof artifact: {error}")
    result.update(finished=now(), integration_credit=0, directory=str(work))
    write_new(work / "outcome.json", encoded(result))
    with store.edit() as registry:
        public_result = {key: value for key, value in result.items() if key not in {"directory", "artifact"}}
        if result.get("artifact"):
            artifact_path = Path(result["artifact"])
            if artifact_path.is_relative_to(store.runtime):
                public_result["artifact"] = "runtime:" + artifact_path.relative_to(store.runtime).as_posix()
            elif command in {"build", "integrate"}:
                prepared = Path(args[args.index("--manifest") + 1]).resolve().parent
                public_result["artifact"] = "prepared-run:" + artifact_path.relative_to(prepared).as_posix()
            else:
                public_result["artifact"] = "private-artifact:" + artifact_path.name
        registry["actions"][action_id].update(public_result)
        registry["actions"][action_id]["action_outcome_sha256"] = digest((work / "outcome.json").read_bytes())
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--registry", type=Path)
    parser.add_argument("--runtime", required=True, type=Path)
    subs = parser.add_subparsers(dest="command", required=True)
    subs.add_parser("plan").add_argument("spec", type=Path)
    a = subs.add_parser("amend")
    a.add_argument("task")
    a.add_argument("spec", type=Path)
    a.add_argument("--reason", required=True)
    q = subs.add_parser("queue")
    q.add_argument("--set", metavar="TASK")
    q.add_argument("--state", choices=sorted(STATES))
    q.add_argument("--reason", default="")
    q.add_argument("--reopen-condition", default="")
    subs.add_parser("status")
    subs.add_parser("packet").add_argument("task")
    v = subs.add_parser("views")
    v.add_argument("--check", action="store_true")
    subs.add_parser("close").add_argument("task")
    d = subs.add_parser("diff", help="Review an immutable trial without changing its acceptance state")
    d.add_argument("task")
    d.add_argument("--trial", help="Historical trial ID; defaults to the task's last trial")
    d.add_argument("--output", type=Path, help="New private standalone HTML review")
    d.add_argument("--target", help="Initially selected trial target")
    d.add_argument("--symbol", help="Initially selected complete function")
    d.add_argument("--serve", action="store_true", help="Serve the private review read-only on 127.0.0.1")
    d.add_argument("--port", type=int, default=0, help="Local review port; zero chooses an unused port")
    d.add_argument("--open", action="store_true", help="Open the local browser (requires --serve)")
    f = subs.add_parser("finalize", help="Validate and publish a completed batch with guarded backups")
    f.add_argument("action", help="Registered successful build/integration action ID")
    f.add_argument("--manifest", required=True, type=Path, help="The action's pinned preparation manifest")
    f.add_argument("--output", required=True, type=Path, help="Private staging, backups and receipts directory")
    f.add_argument("--references", type=Path, help="Private pinned boot/level reference root when needed")
    f.add_argument("--task", action="append", default=[], help="Candidate to close after validation; repeat as needed")
    f.add_argument("--apply", action="store_true", help="Publish the validated staged files; default only prepares them")
    subs.add_parser("seed-history")
    subs.add_parser("refresh-history")
    subs.add_parser("import-legacy").add_argument("markdown", type=Path)
    t = subs.add_parser("trial")
    t.add_argument("task")
    t.add_argument("--toolchain", type=Path, required=True)
    t.add_argument("--profile", type=Path)
    t.add_argument("--repeat-reason", default="")
    for name in ("build", "integrate", "report"):
        subs.add_parser(name).add_argument("arguments", nargs=argparse.REMAINDER)
    args = parser.parse_args(argv)
    if args.command == "diff" and (args.open or args.port) and not args.serve:
        parser.error("--open and --port require --serve")
    if args.command == "diff" and not 0 <= args.port <= 65535:
        parser.error("Review port must be between 0 and 65535")
    repo = args.repo.resolve()
    store = Store(args.registry or repo / "config/campaign-register.json", private(args.runtime, repo))
    if args.command == "plan":
        result = plan(store, read(args.spec))
    elif args.command == "amend":
        result = amend(store, args.task, read(args.spec), args.reason)
    elif args.command == "import-legacy":
        result = legacy_import(store, args.markdown)
    elif args.command == "status":
        result = summary(store.load())
    elif args.command == "queue":
        result = transition(store, args.set, args.state, args.reason, args.reopen_condition) if args.set else {
            "queued": sorted([task for task in store.load()["tasks"].values() if task["state"] == "queued"],
                             key=lambda task: (task.get("priority", 100), task["id"]))}
    elif args.command == "packet":
        result = packet(store, repo, args.task)
    elif args.command == "views":
        result = views(store, repo, args.check)
    elif args.command == "close":
        result = close(store, repo, args.task)
    elif args.command == "diff":
        from campaign_diff import render_review
        result = render_review(store, repo, args.task, trial_id=args.trial,
                               output=args.output, target=args.target, symbol=args.symbol)
        if args.serve:
            from campaign_view_server import serve_review
            print(json.dumps(result, indent=2), flush=True)
            serve_review(Path(result["output"]), port=args.port, open_browser=args.open)
            return 0
    elif args.command == "finalize":
        from campaign_finalize import finalize
        result = finalize(store, repo, args.action, manifest=args.manifest,
                          output=args.output, tasks=tuple(args.task), apply=args.apply,
                          references=args.references)
    elif args.command == "seed-history":
        result = seed_history(store, repo)
    elif args.command == "refresh-history":
        result = refresh_history(store)
    elif args.command == "trial":
        result = trial(store, repo, args.task, args.toolchain.resolve(),
                       args.profile or repo / "progress/candidates.json", args.repeat_reason)
    else:
        forwarded = args.arguments[1:] if args.arguments[:1] == ["--"] else args.arguments
        result = facade(store, repo, args.command, forwarded)
    print(json.dumps(result, indent=2))
    if args.command == "trial":
        return 0 if result["state"] == "exact_private" else 1
    if args.command in {"build", "integrate", "report"}:
        return 0 if result["state"] == "passed" else 1
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except TrialPublicationError as error:
        print(f"Campaign trial publication failed: {error}", file=sys.stderr)
        raise SystemExit(2)
    except (OSError, ValueError, KeyError, TypeError) as error:
        print(f"Campaign rejected: {error}", file=sys.stderr)
        raise SystemExit(2)
