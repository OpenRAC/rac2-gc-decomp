"""Offline contextual review of pinned campaign trials; never acceptance or credit.

Only the requested private HTML file is written. All decoding uses frozen trial
bytes, and row alignment is by exact address (object fallback is explicitly
section-relative). No fuzzy alignment, masks, compiler or registry edits exist.
"""
from __future__ import annotations

from collections import Counter
import hashlib
import json
from pathlib import Path
import re
import struct
import uuid

import elf_tools

SAFE = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]*\Z")
SHA = re.compile(r"[0-9a-f]{64}\Z")
TRIAL = re.compile(r"[0-9a-f]{32}\Z")


def _hash(data):
    return hashlib.sha256(data).hexdigest()


def _encoded(value):
    return (json.dumps(value, sort_keys=True, indent=2, ensure_ascii=False) + "\n").encode("utf-8")


def _safe(value, label):
    if not isinstance(value, str) or not SAFE.fullmatch(value) or value in {".", ".."}:
        raise ValueError(f"Invalid {label}")
    return value


def _inside(root, *parts):
    """Resolve before reading; reject junction/symlink and traversal escapes."""
    root = Path(root).resolve()
    path = root.joinpath(*parts).resolve()
    if path == root or not path.is_relative_to(root):
        raise ValueError("Trial path escaped its immutable directory")
    return path


def _pinned(root, relative, expected):
    if not isinstance(expected, str) or not SHA.fullmatch(expected):
        raise ValueError(f"Missing SHA256 pin: {relative}")
    data = _inside(root, relative).read_bytes()
    if _hash(data) != expected:
        raise ValueError(f"SHA256 drift: {relative}")
    return data


def _symbols(data, elf):
    """Defined global FUNC symbols, including explicit ET_REL section identity."""
    table = struct.unpack_from("<I", data, 32)[0]
    stride = struct.unpack_from("<H", data, 46)[0]
    headers = [struct.unpack_from("<10I", data, table + i * stride)
               for i in range(len(elf["sections"]))]
    result = {}
    for header in headers:
        if header[1] != 2:
            continue
        link = header[6]
        if link >= len(headers) or headers[link][1] != 3 or header[9] != 16 or header[5] % 16:
            raise ValueError("Invalid ELF symbol table")
        strings_header = headers[link]
        strings = data[strings_header[4]:strings_header[4] + strings_header[5]]
        for position in range(header[4], header[4] + header[5], 16):
            name_offset, value, size, info, _, index = struct.unpack_from("<IIIBBH", data, position)
            if info & 15 != 2 or info >> 4 != 1 or index == 0:
                continue
            if index >= len(headers):
                raise ValueError("Function section outside ELF")
            section = elf["sections"][index]
            if section["type"] != 1 or section["flags"] & 6 != 6:
                raise ValueError("Function is not in executable file-backed code")
            offset = value if elf["type"] == elf_tools.ET_REL else value - section["address"]
            if offset < 0 or offset + size > section["size"]:
                raise ValueError("Function symbol outside executable section")
            ending = strings.find(b"\0", name_offset)
            if name_offset >= len(strings) or ending < 0:
                raise ValueError("Invalid ELF symbol string")
            name = strings[name_offset:ending].decode("ascii")
            if name in result:
                raise ValueError("Duplicate function symbol")
            result[name] = {"address": value, "size": size, "section": index,
                            "section_name": section["name"], "file_offset": section["offset"] + offset}
    return result


def _body(data, elf, body):
    if not body or not body["size"]:
        return b""
    if elf["type"] == elf_tools.ET_EXEC:
        return elf_tools._mapped_bytes(data, elf_tools._mappings(elf), body["address"], body["size"])
    offset = body["file_offset"]
    return data[offset:offset + body["size"]]


def _decode(data, address, decoder):
    if not data:
        return "—"
    if len(data) != 4 or address % 4:
        return ".byte " + ", ".join(f"0x{byte:02X}" for byte in data)
    word = int.from_bytes(data, "little")
    instruction = decoder.Instruction(word, address, decoder.InstrCategory.R5900)
    return instruction.disassemble() if instruction.isValid() else f".word 0x{word:08X} (invalid R5900)"


def _rows(reference, candidate, address, candidate_address, decoder, relative=False):
    """Every byte of both complete bodies; never trim to a shared prefix."""
    ref_start = 0 if relative else address
    cand_start = 0 if relative else candidate_address
    def locations(start, length):
        return range(start // 4 * 4, start + length, 4) if length else ()
    addresses = sorted(set(locations(ref_start, len(reference))) |
                       set(locations(cand_start, len(candidate))))
    rows = []
    different_bytes = 0
    for location in addresses:
        left = [reference[location + i - ref_start]
                if 0 <= location + i - ref_start < len(reference) else None for i in range(4)]
        right = [candidate[location + i - cand_start]
                 if 0 <= location + i - cand_start < len(candidate) else None for i in range(4)]
        changes = [a != b for a, b in zip(left, right)]
        different_bytes += sum(changes)
        lb, rb = bytes(x for x in left if x is not None), bytes(x for x in right if x is not None)
        rows.append({"address": location, "reference": left, "candidate": right, "changed": changes,
                     "different": any(changes),
                     "reference_instruction": _decode(lb, location + address if relative else location, decoder),
                     "candidate_instruction": _decode(rb, location + candidate_address if relative else location, decoder)})
    return rows, different_bytes


def _catalog_functions(catalog):
    functions = catalog.get("functions")
    if not isinstance(functions, list) or not functions:
        raise ValueError("Catalog has no complete functions")
    seen = set()
    for function in functions:
        name, address, size = function.get("symbol"), function.get("address"), function.get("size")
        if (not isinstance(name, str) or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name)
                or name in seen or type(address) is not int or not 0 <= address < 2**32 or address % 4
                or type(size) is not int or size <= 0 or size % 4 or address + size > 2**32):
            raise ValueError("Invalid complete function identity")
        seen.add(name)
    ordered = sorted(functions, key=lambda item: item["address"])
    if any(a["address"] + a["size"] > b["address"] for a, b in zip(ordered, ordered[1:])):
        raise ValueError("Overlapping complete functions")
    return functions


def _decoder():
    import rabbitizer
    if rabbitizer.__version__ != "1.16.2":
        raise ValueError("Contextual review requires rabbitizer 1.16.2 R5900")
    return rabbitizer


def review_data(store, repo, task_id, trial_id=None, target=None, symbol=None):
    """Validate and materialize a historical diagnostic without writing anything."""
    repo, runtime = Path(repo).resolve(), Path(store.runtime).resolve()
    if runtime == repo or runtime.is_relative_to(repo) or repo.is_relative_to(runtime):
        raise ValueError("Trial runtime must be private and outside repository")
    registry = store.load()
    task = registry.get("tasks", {}).get(task_id)
    if not task:
        raise ValueError("Unknown campaign task")
    trial_id = trial_id or task.get("last_trial")
    if not isinstance(trial_id, str) or not TRIAL.fullmatch(trial_id):
        raise ValueError("A finalized registered UUID trial is required")
    registered = registry.get("trials", {}).get(trial_id)
    if not registered or registered.get("id") != trial_id or registered.get("task") != task_id:
        raise ValueError("Trial does not belong to this task")
    if registered.get("directory") != "runtime:trials/" + trial_id:
        raise ValueError("Trial directory is not its registered UUID location")
    work = _inside(runtime, "trials", trial_id)
    if not work.is_relative_to(_inside(runtime, "trials")):
        raise ValueError("Trial directory escaped trials")
    manifest_bytes = _pinned(work, "manifest.json", registered.get("manifest_sha256"))
    outcome_bytes = _pinned(work, "outcome.json", registered.get("outcome_sha256"))
    manifest, outcome = json.loads(manifest_bytes), json.loads(outcome_bytes)
    if (manifest.get("id") != trial_id or manifest.get("task") != task_id or outcome.get("id") != trial_id
            or registered.get("state") != outcome.get("state") or registered.get("state") == "running"):
        raise ValueError("Trial identity/state differs from its register pins")
    if manifest.get("kind") != "rac2-candidate-trial" or manifest.get("schema") != 1:
        raise ValueError("Trial has no immutable candidate inputs to review")
    snapshot, semantic = manifest["task_snapshot"], manifest["semantic_inputs"]
    if snapshot.get("id") != task_id or snapshot.get("kind") != "candidate":
        raise ValueError("Task snapshot identity differs")
    key = _hash(_encoded(semantic))
    if key != manifest.get("semantic_key") or key != registered.get("semantic_key"):
        raise ValueError("Trial semantic key drift")
    source_name = _safe(semantic.get("source_name"), "source filename")
    if not source_name.endswith(".c"):
        raise ValueError("Source snapshot must be C")
    source = _pinned(work, "source/" + source_name, semantic.get("source_sha256"))
    if not isinstance(semantic.get("tools"), dict) or any(
            not SHA.fullmatch(semantic["tools"].get(name, "")) for name in ("cc1", "cpp", "as", "ld.exe")):
        raise ValueError("Missing semantic tool pins")
    object_data = None
    if outcome.get("object_sha256"):
        object_data = _pinned(work, "candidate.o", outcome["object_sha256"])
    children = outcome.get("children", [])
    if len({child.get("id") for child in children}) != len(children):
        raise ValueError("Duplicate target outcomes")
    child_map = {child["id"]: child for child in children}
    descriptors = snapshot.get("targets", [])
    if not descriptors or len({item["id"] for item in descriptors}) != len(descriptors):
        raise ValueError("Task needs unique target snapshots")
    if set(child_map) != {item["id"] for item in descriptors}:
        raise ValueError("Outcome targets differ from immutable task snapshot")
    decoder, actual_semantic_targets, views, provenance = _decoder(), [], [], []
    for descriptor in descriptors:
        ident = _safe(descriptor["id"], "target id")
        child_root = _inside(work, "targets", ident)
        # The enclosing manifest pins the catalogs via semantic inputs. Read once.
        catalog_bytes = _inside(child_root, "catalog.json").read_bytes()
        catalog = json.loads(catalog_bytes)
        reference_pin = catalog.get("reference_sha256")
        reference = _pinned(child_root, "reference.elf", reference_pin)
        actual_semantic_targets.append({"program": catalog.get("program", "boot"),
                                        "catalog_sha256": _hash(catalog_bytes), "reference_sha256": _hash(reference)})
        if catalog.get("flags") != semantic.get("flags"):
            raise ValueError("Catalog flags differ from semantic compiler flags")
        functions = _catalog_functions(catalog)
        ref_elf = elf_tools._parse(reference)
        if ref_elf["type"] != elf_tools.ET_EXEC:
            raise ValueError("Reference must be linked ET_EXEC")
        child = child_map[ident]
        child_outcome = _inside(child_root, "outcome.json")
        if child_outcome.exists() and json.loads(child_outcome.read_bytes()) != child:
            raise ValueError("Child outcome differs from pinned parent outcome")
        if child.get("state") not in {"unmeasured"} and not child_outcome.exists():
            raise ValueError("Measured child outcome is missing")
        linked_pin = child.get("candidate_elf_sha256")
        if linked_pin and object_data is None:
            raise ValueError("Linked trial is missing its pinned candidate object")
        candidate = _pinned(child_root, "candidate.elf", linked_pin) if linked_pin else object_data
        mode = "linked ET_EXEC" if linked_pin else "relocatable object diagnostic" if candidate else "unmeasured"
        cand_elf = elf_tools._parse(candidate) if candidate else None
        if cand_elf and cand_elf["type"] != (elf_tools.ET_EXEC if linked_pin else elf_tools.ET_REL):
            raise ValueError("Candidate ELF type differs from pinned artifact role")
        symbols = _symbols(candidate, cand_elf) if candidate else {}
        relocations = bool(cand_elf and any(section["type"] in {4, 9} and section["size"]
                                           for section in cand_elf["sections"]))
        data_exact = not catalog.get("read_only_sections")
        if catalog.get("read_only_sections"):
            # Reuse the maintained complete data-unit rule, independently of body equality.
            from check_candidates import require_exact_readonly
            try:
                require_exact_readonly(catalog, child.get("read_only_sections", []))
                data_exact = True
            except (ValueError, KeyError, TypeError):
                data_exact = False
        provenance.append({"target": ident, "catalog_sha256": _hash(catalog_bytes),
                           "reference_elf_sha256": _hash(reference), "candidate_elf_sha256": linked_pin,
                           "artifact_mode": mode, "has_relocations": relocations,
                           "recorded_child_state": child.get("state"), "data_unit_exact": data_exact})
        for function in functions:
            name, address, size = function["symbol"], function["address"], function["size"]
            original = elf_tools._mapped_bytes(reference, elf_tools._mappings(ref_elf), address, size)
            produced_symbol = symbols.get(name)
            produced = _body(candidate, cand_elf, produced_symbol) if produced_symbol else b""
            candidate_address = produced_symbol["address"] if produced_symbol else address
            relative = bool(candidate and not linked_pin)
            rows, differences = _rows(original, produced, address, candidate_address, decoder, relative)
            eligible = bool(linked_pin and produced_symbol and size == len(produced)
                            and candidate_address == address and candidate_address % 4 == 0
                            and len(produced) % 4 == 0 and not relocations and data_exact)
            status = ("raw exact complete body (historical; viewer adds no credit)" if eligible and original == produced
                      else "complete body mismatch" if eligible else "diagnostic only; not raw exact")
            reasons = []
            if not linked_pin:
                reasons.append("No pinned linked ELF; object offsets are not linked addresses." if candidate
                               else "Compilation did not produce a pinned object; candidate is unmeasured.")
            if not produced_symbol:
                reasons.append("Defined function symbol is missing.")
            elif not produced or len(produced) % 4 or candidate_address % 4:
                reasons.append("Candidate complete symbol has zero or invalid instruction size/alignment.")
            if produced_symbol and size != len(produced):
                reasons.append("Complete symbol size differs; all extra/missing bytes are retained.")
            if linked_pin and candidate_address != address:
                reasons.append("Candidate symbol is misplaced; rows use exact addresses.")
            if relocations:
                reasons.append("Relocations remain unresolved; equality cannot qualify raw exact.")
            if not data_exact:
                reasons.append("Required complete readonly data unit did not qualify exact.")
            views.append({"target": ident, "symbol": name, "address": address, "size": size,
                          "candidate_address": candidate_address, "candidate_size": len(produced),
                          "candidate_section": produced_symbol.get("section_name") if produced_symbol else None,
                          "mode": mode, "relative": relative, "status": status, "reasons": reasons,
                          "different_bytes": differences, "reference_sha256": _hash(original),
                          "candidate_sha256": _hash(produced) if produced_symbol else None, "rows": rows})
    expected = semantic.get("targets", [])
    if Counter(json.dumps(item, sort_keys=True) for item in actual_semantic_targets) != Counter(
            json.dumps(item, sort_keys=True) for item in expected):
        raise ValueError("Catalog/reference snapshots differ from semantic target pins")
    if target is not None and target not in {item["target"] for item in views}:
        raise ValueError("Unknown target selector")
    available = [item for item in views if target is None or item["target"] == target]
    if symbol is not None and symbol not in {item["symbol"] for item in available}:
        raise ValueError("Unknown function selector for target")
    if symbol is not None:
        selected = next(item for item in available if item["symbol"] == symbol)
    else:
        selected_target = target if target is not None else views[0]["target"]
        target_views = [item for item in views if item["target"] == selected_target]
        preferred = next((name for name in snapshot.get("symbols", [])
                          if name in {item["symbol"] for item in target_views}), target_views[0]["symbol"])
        selected = next(item for item in target_views if item["symbol"] == preferred)
    instruments = manifest.get("instruments", {})
    drift = []
    for path, pin in instruments.items():
        if not isinstance(path, str) or Path(path).is_absolute() or ".." in Path(path).parts:
            raise ValueError("Invalid historical instrument path")
        if not SHA.fullmatch(pin):
            raise ValueError("Invalid historical instrument SHA256")
        current = (repo / path).resolve()
        if not current.is_relative_to(repo):
            drift.append(path)
            continue
        if not current.is_file() or _hash(current.read_bytes()) != pin:
            drift.append(path)
    return {"kind": "rac2-historical-contextual-diff", "task": task_id, "trial": trial_id,
            "trial_state": outcome["state"], "current_task_state": task.get("state"),
            "integration_credit": 0, "historical": True, "decoder": "rabbitizer 1.16.2 R5900",
            "source_name": source_name, "source": source.decode("utf-8", errors="replace"),
            "source_sha256": _hash(source), "manifest_sha256": _hash(manifest_bytes),
            "outcome_sha256": _hash(outcome_bytes), "object_sha256": outcome.get("object_sha256"),
            "semantic_key": key, "tools": semantic["tools"], "profile_sha256": manifest.get("profile_sha256"),
            "historical_instrument_sha256": instruments, "historical_instrument_drift": drift,
            "provenance": provenance, "views": views,
            "selected": {"target": selected["target"], "symbol": selected["symbol"]}}


def render_review(store, repo, task_id, trial_id=None, output=None, target=None, symbol=None):
    """Create one new private standalone HTML and return a small JSON receipt."""
    repo, runtime = Path(repo).resolve(), Path(store.runtime).resolve()
    destination = Path(output).resolve() if output else runtime / "reviews" / (uuid.uuid4().hex + ".html")
    if (destination == repo or destination.is_relative_to(repo) or repo.is_relative_to(destination)
            or destination.suffix.lower() != ".html"):
        raise ValueError("Review output must be a private HTML outside repository")
    if any((parent / ".git").exists() for parent in destination.parents):
        raise ValueError("Review output cannot be inside any Git checkout")
    # Any existing input/registry or immutable evidence directory is off limits.
    forbidden = [runtime / name for name in ("trials", "actions", "registry-revisions")]
    if any(destination == root.resolve() or destination.is_relative_to(root.resolve()) for root in forbidden):
        raise ValueError("Review output cannot be inside immutable evidence")
    if destination.exists() or destination == Path(store.path).resolve():
        raise ValueError("Review output must be a new file")
    data = review_data(store, repo, task_id, trial_id, target, symbol)
    serialized = json.dumps(data, ensure_ascii=True, separators=(",", ":"))
    # Script raw text cannot contain markup or JS line separators from source/metadata.
    serialized = serialized.replace("<", "\\u003c").replace(">", "\\u003e").replace("&", "\\u0026")
    html = HTML.replace("__REVIEW_DATA__", serialized)
    destination.parent.mkdir(parents=True, exist_ok=True)
    with destination.open("x", encoding="utf-8", newline="\n") as stream:
        stream.write(html)
    selected_view = next(view for view in data["views"] if view["target"] == data["selected"]["target"]
                         and view["symbol"] == data["selected"]["symbol"])
    return {"output": str(destination), "trial": data["trial"], "task": task_id,
            "selected_target": data["selected"]["target"], "selected_symbol": data["selected"]["symbol"],
            "recorded_trial_state": data["trial_state"], "selected_body_status": selected_view["status"],
            "targets": len(data["provenance"]), "functions": len(data["views"]),
            "historical": True, "integration_credit": 0, "html_sha256": _hash(html.encode("utf-8"))}


HTML = r'''<!doctype html>
<html lang="en"><head><meta charset="utf-8">
<meta http-equiv="Content-Security-Policy" content="default-src 'none'; script-src 'unsafe-inline'; style-src 'unsafe-inline'; connect-src 'none'; img-src 'none'; base-uri 'none'; form-action 'none'">
<meta name="viewport" content="width=device-width,initial-scale=1"><title>RAC2 contextual trial review</title>
<style>
:root{color-scheme:dark;font:15px system-ui;background:#0e1420;color:#e5edf7}*{box-sizing:border-box}body{margin:0}header{padding:24px 28px;background:#152136;border-bottom:1px solid #38506d}h1{font-size:24px;margin:0 0 8px}p{line-height:1.5;margin:8px 0}.muted{color:#b1bfd0}.badge{display:inline-block;padding:5px 10px;background:#34466a;border-radius:5px;margin:4px 5px 4px 0}main{padding:20px 28px}.controls{display:flex;gap:14px;align-items:center;flex-wrap:wrap;position:sticky;top:0;background:#0e1420;padding:12px 0;z-index:2}select,button{font:inherit;background:#24334b;color:inherit;border:1px solid #5e7596;border-radius:5px;padding:8px}button:disabled{opacity:.45}label{display:flex;gap:7px;align-items:center}#status{padding:15px;background:#19273b;border-left:4px solid #71b6ef;line-height:1.6;margin:12px 0}details{margin:15px 0}summary{cursor:pointer;font-weight:600}pre{overflow:auto;white-space:pre;tab-size:4;background:#111b2b;border:1px solid #31445e;padding:16px;font:13px/1.6 ui-monospace,Consolas,monospace}#source{max-height:440px}#table-scroll{overflow:auto;max-height:65vh;border:1px solid #31445e}table{border-collapse:collapse;width:100%;font:13px/1.5 ui-monospace,Consolas,monospace}th{position:sticky;top:0;background:#24334b;text-align:left;padding:10px;z-index:1}td{border-top:1px solid #25354b;padding:7px 10px;white-space:pre}td:nth-child(4){border-left:2px solid #526b8c}tr.diff{background:#38262c}.byte{display:inline-block;min-width:25px;text-align:center}.changed{color:#fff;background:#9c353f;border-radius:2px}.missing{color:#ffcf80}tr.focused{outline:2px solid #ffd16c;outline-offset:-2px}#counter{min-width:180px}.instructions{color:#d2e1f3}footer{padding:24px 0;color:#aebed2}code{overflow-wrap:anywhere}@media(max-width:800px){header,main{padding:16px}table{font-size:12px}.controls{position:static}}
</style></head><body>
<header><h1>RAC2 contextual trial review</h1><p>Historical immutable trial · informational diagnostic · integration credit: 0</p>
<div id="identity"></div><p class="muted">Exact addresses and every complete-body byte. No alignment heuristics, masks, trimming, or acceptance actions.</p></header>
<main><div class="controls"><label>Target <select id="target" aria-label="Target"></select></label><label>Function <select id="function" aria-label="Function"></select></label>
<button id="previous">Previous difference</button><button id="next">Next difference</button><label><input id="only" type="checkbox">Only differences</label><span id="counter" aria-live="polite"></span></div>
<div id="status" role="status"></div><p id="coordinates" class="muted"></p>
<div id="table-scroll"><table><thead><tr><th>Reference address</th><th>Reference bytes</th><th>Reference R5900</th><th>Candidate address</th><th>Candidate bytes</th><th>Candidate R5900</th></tr></thead><tbody id="rows"></tbody></table></div>
<p id="empty" role="status" hidden>No differing instruction rows for this selection. Disable Only differences to view the full body.</p>
<details open><summary>Immutable source snapshot</summary><p id="source-pin" class="muted"></p><pre id="source"></pre></details>
<details><summary>Provenance and recorded trial state</summary><pre id="provenance"></pre></details>
<footer>Raw body equality is a historical diagnostic. Trial state, readonly data-unit checks, current integration and full-image gates are separate. This viewer cannot compile, edit source, modify the register, accept a trial, or add credit. All assets are local to this HTML.</footer></main>
<script id="review-data" type="application/json">__REVIEW_DATA__</script>
<script>
'use strict';
const data=JSON.parse(document.getElementById('review-data').textContent), byId=id=>document.getElementById(id);
const target=byId('target'),func=byId('function'),only=byId('only');let active=null,diffRows=[],cursor=-1;
const hex=value=>'0x'+value.toString(16).toUpperCase().padStart(8,'0');
function option(select,value){const node=document.createElement('option');node.value=value;node.textContent=value;select.appendChild(node);}
byId('identity').textContent='Task '+data.task+' · Trial '+data.trial+' · Recorded trial state: '+data.trial_state+' · Current task state: '+data.current_task_state;
byId('source').textContent=data.source;byId('source-pin').textContent=data.source_name+' · SHA256 '+data.source_sha256+' · read-only snapshot';
const metadata=Object.fromEntries(Object.entries(data).filter(([key])=>!['views','source'].includes(key)));byId('provenance').textContent=JSON.stringify(metadata,null,2);
for(const value of [...new Set(data.views.map(view=>view.target))])option(target,value);target.value=data.selected.target;
function functions(){func.replaceChildren();for(const view of data.views.filter(view=>view.target===target.value))option(func,view.symbol);}
function cell(row,text){const node=document.createElement('td');node.textContent=text;row.appendChild(node);return node;}
function bytes(row,values,changed){const node=cell(row,'');values.forEach((value,index)=>{const byte=document.createElement('span');byte.className='byte'+(changed[index]?' changed':'')+(value===null?' missing':'');byte.textContent=value===null?'—':value.toString(16).toUpperCase().padStart(2,'0');node.appendChild(byte);});}
function draw(){active=data.views.find(view=>view.target===target.value&&view.symbol===func.value);diffRows=[];cursor=-1;const body=byId('rows');body.replaceChildren();
byId('status').textContent=active.status+' · '+active.different_bytes+' different/extra/missing bytes · reference '+active.size+' bytes · candidate '+active.candidate_size+' bytes. '+active.reasons.join(' ');
byId('coordinates').textContent=active.relative?'Object diagnostic: rows align by body offset for context only. Candidate addresses are unplaced section offsets in '+active.candidate_section+'. No linked-address equality is claimed.':'Rows align by exact virtual address. Missing bytes are shown as —. Every red byte differs or is extra/missing.';
for(const item of active.rows){if(only.checked&&!item.different)continue;const row=document.createElement('tr');if(item.different)row.classList.add('diff');
const leftAddress=active.relative?active.address+item.address:item.address,rightAddress=active.relative?active.candidate_address+item.address:item.address;
cell(row,hex(leftAddress));bytes(row,item.reference,item.changed);cell(row,item.reference_instruction).classList.add('instructions');cell(row,hex(rightAddress));bytes(row,item.candidate,item.changed);cell(row,item.candidate_instruction).classList.add('instructions');body.appendChild(row);if(item.different)diffRows.push(row);}
byId('empty').hidden=!(only.checked&&diffRows.length===0);byId('previous').disabled=byId('next').disabled=diffRows.length===0;byId('counter').textContent=diffRows.length+' differing instruction rows';}
function step(delta){if(!diffRows.length)return;if(cursor>=0)diffRows[cursor].classList.remove('focused');cursor=cursor<0?(delta<0?diffRows.length-1:0):(cursor+delta+diffRows.length)%diffRows.length;const row=diffRows[cursor];row.classList.add('focused');row.scrollIntoView({block:'center'});byId('counter').textContent='Difference '+(cursor+1)+' of '+diffRows.length;}
target.addEventListener('change',()=>{functions();draw();});func.addEventListener('change',draw);only.addEventListener('change',draw);byId('previous').addEventListener('click',()=>step(-1));byId('next').addEventListener('click',()=>step(1));functions();func.value=data.selected.symbol;draw();
</script></body></html>
'''
