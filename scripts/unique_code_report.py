"""Validate a metadata-only EE function catalogue and export conservative unique C coverage.

Reconstruction certificates are pinned extraction receipts, not a retail-byte check
performed by this asset-free exporter. Only verified extents with exact receipts
enter the validated subset. The subset is never described as a full-game total.
"""
from __future__ import annotations

import argparse
import hashlib
import gzip
import io
import json
from pathlib import Path, PurePosixPath
import re
import sys


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def sha(value: object) -> None:
    require(isinstance(value, str) and re.fullmatch(r"[0-9a-f]{64}", value) is not None,
            "Invalid SHA-256")


def interval(row: dict) -> tuple[int, int]:
    address, size = row.get("address"), row.get("size")
    require(type(address) is int and address >= 0 and address % 4 == 0
            and type(size) is int and size > 0 and size % 4 == 0,
            "Invalid aligned interval")
    return address, address + size


def no_overlap(rows: list[dict]) -> None:
    spans = sorted(interval(row) for row in rows)
    require(all(a[1] <= b[0] for a, b in zip(spans, spans[1:])), "Overlapping intervals")


def validate_relocations(relocations: list, address: int, size: int) -> None:
    """Derive address fields independently and require overlapping bits to agree."""
    assignments = {}
    for relocation in relocations:
        require(isinstance(relocation, dict), "Invalid relocation metadata")
        kind, target = relocation.get("kind"), relocation.get("target")
        require(kind in ("j26", "hi16_lo16", "gp16")
                and type(target) is int and 0 <= target <= 0xffffffff,
                "Invalid relocation kind or target")
        require(isinstance(relocation.get("role"), str) and relocation["role"]
                and isinstance(relocation.get("evidence"), str) and relocation["evidence"],
                "Relocation needs classified address role and evidence")
        offset = relocation.get("offset")
        require(type(offset) is int and 0 <= offset <= size - 4 and offset % 4 == 0,
                "Invalid relocation primary offset")
        if kind == "j26":
            require(target % 4 == 0 and (target & 0xf0000000) == ((address + offset + 4) & 0xf0000000),
                    "Invalid J26 target region or alignment")
            if "internal" in relocation:
                require(type(relocation["internal"]) is bool, "Invalid internal jump classification")
                if relocation["internal"]:
                    require(address <= target < address + size
                            and type(relocation.get("relative_target")) is int
                            and relocation["relative_target"] == target - address,
                            "Invalid internal jump relative target")
            parts = [(offset, 0x03ffffff, (target >> 2) & 0x03ffffff)]
        elif kind == "gp16":
            gp = relocation.get("gp")
            require(type(gp) is int and 0 <= gp <= 0xffffffff, "Invalid GP base")
            delta = target - gp
            require(-32768 <= delta <= 32767, "GP target outside signed immediate range")
            parts = [(offset, 0xffff, delta & 0xffff)]
        else:
            high, low, mode = relocation.get("high_offset"), relocation.get("low_offset"), relocation.get("lo_mode")
            require(mode in ("addiu", "daddiu", "ori", "memory", "none")
                    and type(high) is int and (low is None or type(low) is int)
                    and (low is None) == (mode == "none")
                    and offset in (high, low), "Invalid HI16/LO16 pairing metadata")
            high_value = ((target + 0x8000) >> 16) & 0xffff if mode in ("addiu", "daddiu", "memory") else (target >> 16) & 0xffff
            parts = [(high, 0xffff, high_value)]
            if low is not None:
                require(low != high, "HI16 and LO16 cannot occupy one instruction")
                parts.append((low, 0xffff, target & 0xffff))
        for position, mask, value in parts:
            require(type(position) is int and 0 <= position <= size - 4 and position % 4 == 0,
                    "Relocation field outside complete body")
            prior_mask, prior_value = assignments.get(position, (0, 0))
            require(((prior_value ^ value) & prior_mask & mask) == 0,
                    "Conflicting overlapping relocation fields")
            assignments[position] = (prior_mask | mask, (prior_value & ~mask) | (value & mask))


def validate_pins(catalog: dict, repo: Path) -> set[str]:
    """Refuse traversal, symlinks outside the repository and stale tracked inputs."""
    pins = catalog.get("input_pins")
    require(isinstance(pins, list) and bool(pins), "Missing input pins")
    seen = set()
    for pin in pins:
        name = pin.get("path")
        require(isinstance(name, str) and name and "\\" not in name and ":" not in name,
                "Input pin must be repository-relative")
        relative = PurePosixPath(name)
        require(not relative.is_absolute() and ".." not in relative.parts
                and str(relative) == name and name not in seen, "Invalid or duplicate input pin")
        path = (repo / name).resolve()
        require(path.is_relative_to(repo.resolve()), "Input pin escapes repository")
        sha(pin.get("sha256"))
        require(digest(path.read_bytes()) == pin["sha256"], f"Stale input pin: {name}")
        seen.add(name)
    return seen


def validate_catalog(catalog: dict) -> tuple[dict, list[dict]]:
    require(type(catalog.get("schema")) is int and catalog["schema"] == 1
            and catalog.get("target") == "SCUS_972.68",
            "Unsupported catalogue identity")
    normalizer = catalog.get("normalizer", {})
    section_policy = catalog.get("section_identity_policy")
    require(section_policy in (None, "named-pinned-ee-sections-v1"), "Unknown section identity policy")
    graph_policy = catalog.get("group_policy") == "graph-refined-structural-templates"
    require(catalog.get("group_policy") in (None, "graph-refined-structural-templates"), "Unknown grouping policy")
    if graph_policy:
        require(catalog.get("dependency_policy") == "complete-static-control-dependencies",
                "Graph policy lacks complete static dependency receipt")
    require(isinstance(normalizer.get("id"), str) and bool(normalizer["id"]), "Missing normalizer")
    sha(normalizer.get("sha256"))
    programs = {}
    for program in catalog.get("programs", []):
        name = program.get("program")
        require(isinstance(name, str) and name and name not in programs, "Duplicate program")
        sha(program.get("reference_sha256"))
        sections = program.get("ee_sections")
        require(isinstance(sections, list) and bool(sections), "Missing executable EE scope")
        if section_policy is not None:
            require(all(isinstance(s.get("name"), str) and s["name"] for s in sections),
                    "Named section identity is missing")
            require(len({s["name"] for s in sections}) == len(sections), "Duplicate executable section name")
        no_overlap(sections)
        require(type(program.get("excluded_vu_bytes")) is int and program["excluded_vu_bytes"] >= 0,
                "Missing separate VU extent")
        programs[name] = program
    require(bool(programs), "Empty program scope")
    rows = catalog.get("functions")
    require(isinstance(rows, list) and bool(rows), "Empty function catalogue")
    ids, names = set(), set()
    for row in rows:
        identity, program = row.get("id"), row.get("program")
        require(isinstance(identity, str) and bool(identity) and identity not in ids,
                "Duplicate function id")
        require(program in programs, "Unknown function program")
        ids.add(identity)
        start, end = interval(row)
        dependencies = row.get("call_dependencies")
        if graph_policy:
            require(isinstance(dependencies, list), "Graph policy requires explicit static dependencies for every row")
        if dependencies is not None:
            require(isinstance(dependencies, list), "Invalid static dependency metadata")
            dep_offsets = set()
            for edge in dependencies:
                require(isinstance(edge, dict), "Invalid static dependency entry")
                offset, target = edge.get("offset"), edge.get("target")
                require(type(offset) is int and 0 <= offset <= row["size"] - 4 and offset % 4 == 0
                        and offset not in dep_offsets, "Invalid or duplicate static dependency offset")
                require(type(target) is int and 0 <= target <= 0xffffffff and target % 4 == 0
                        and not start <= target < end and edge.get("kind") in ("call", "jump", "branch"),
                        "Invalid external static dependency kind or target")
                require("target_program" not in edge or edge["target_program"] in programs,
                        "Unknown explicit dependency target program")
                dep_offsets.add(offset)
        require(sum(a <= start and end <= b for a, b in
                    map(interval, programs[program]["ee_sections"])) == 1,
                "Function outside executable EE scope")
        sha(row.get("raw_sha256"))
        aliases = row.get("aliases", [])
        require(isinstance(aliases, list) and all(isinstance(a, str) and a for a in aliases),
                "Invalid alias metadata")
        for alias in [identity, *aliases]:
            require((program, alias) not in names, "Duplicate alias or identity")
            names.add((program, alias))
        boundary = row.get("boundary", {})
        require(boundary.get("status") in ("qualified_complete", "flow_supported_inferred", "inferred", "ambiguous_fragment"), "Unknown boundary status")
        require(isinstance(boundary.get("evidence"), list) and bool(boundary["evidence"])
                and all(isinstance(e, str) and e for e in boundary["evidence"]), "Missing boundary evidence")
        norm = row.get("normalization")
        if norm is None:
            require(boundary["status"] not in ("qualified_complete", "flow_supported_inferred"), "Verified boundary lacks normalization certificate")
            continue
        sha(norm.get("signature_sha256"))
        relocations = norm.get("relocations")
        require(isinstance(relocations, list), "Missing explicit relocation metadata")
        validate_relocations(relocations, row["address"], row["size"])
        if graph_policy:
            for relocation in relocations:
                if relocation["kind"] == "j26" and not row["address"] <= relocation["target"] < row["address"] + row["size"]:
                    require(any(edge["offset"] == relocation["offset"] and edge["target"] == relocation["target"]
                                and edge["kind"] in ("call", "jump") for edge in dependencies),
                            "Static dependency receipt omits a normalized external target")
        receipt = norm.get("certificate")
        if receipt is None:
            require(boundary["status"] not in ("qualified_complete", "flow_supported_inferred"), "Verified boundary lacks reconstruction certificate")
            continue
        for field in ("raw_sha256", "normalized_sha256", "reconstructed_sha256", "normalizer_sha256"):
            sha(receipt.get(field))
        template_hash = norm.get("template_sha256")
        if template_hash is None:
            require(receipt["normalized_sha256"] == norm["signature_sha256"],
                    "Role-bearing signature requires explicit template hash")
            template_hash = norm["signature_sha256"]
        sha(template_hash)
        require(receipt.get("exact") is True and receipt["raw_sha256"] == row["raw_sha256"]
                and receipt["reconstructed_sha256"] == row["raw_sha256"]
                and receipt["normalized_sha256"] == template_hash
                and receipt["normalizer_sha256"] == normalizer["sha256"], "Reconstruction certificate mismatch")
    for program in programs:
        no_overlap([row for row in rows if row["program"] == program])
    return programs, rows


def generate(catalog: dict, credit: dict[tuple[str, int, int], str], boot_binding_proof=None) -> dict:
    """Credit keys must come exclusively from the current validated integration loader."""
    programs, rows = validate_catalog(catalog)
    for program in programs:
        credited = [{"address": address, "size": size} for (name, address, size) in credit if name == program]
        no_overlap(credited)
    for value in credit.values():
        sha(value)
    families = {}
    provisional = {}
    graph = None
    if catalog.get("group_policy") == "graph-refined-structural-templates":
        from call_graph_refinement import refine_call_groups
        graph = refine_call_groups(rows, programs, boot_binding_proof)
    for row in rows:
        norm = row.get("normalization")
        signature = norm["signature_sha256"] if norm else "unresolved:" + row["id"]
        provisional.setdefault((row["size"], signature), []).append(row)
        supported = row["boundary"]["status"] in ("qualified_complete", "flow_supported_inferred")
        key = (row["size"], graph["class_by_id"][row["id"]] if graph else signature if supported else "raw-singleton:" + row["id"])
        families.setdefault(key, []).append(row)
    groups = []
    for (size, signature), members in sorted(families.items()):
        members.sort(key=lambda row: (row["program"], row["address"], row["id"]))
        validated = all(row["boundary"]["status"] in ("qualified_complete", "flow_supported_inferred")
                        and (row.get("normalization") or {}).get("certificate") for row in members)
        matched = []
        for row in members:
            pin = credit.get((row["program"], row["address"], row["size"]))
            if pin is not None:
                require(pin == row["raw_sha256"], "Current C proof contradicts catalogue raw hash")
            matched.append(pin is not None)
        group_id = digest(json.dumps([size, signature], separators=(",", ":")).encode())
        groups.append({"id": group_id, "representative": members[0]["id"], "size": size,
                       "members": [row["id"] for row in members], "validated": bool(validated),
                       "all_placements_matched_c": bool(validated and all(matched)),
                       "any_placement_matched_c": bool(validated and any(matched))})
    valid = [group for group in groups if group["validated"]]
    total = sum(group["size"] for group in valid)
    matched = sum(group["size"] for group in valid if group["all_placements_matched_c"])
    scoped = sum(section["size"] for p in programs.values() for section in p["ee_sections"])
    covered = sum(row["size"] for row in rows)
    verified = sum(row["size"] for row in rows if row["boundary"]["status"] in ("qualified_complete", "flow_supported_inferred"))
    complete = covered == scoped and verified == scoped and len(valid) == len(groups)
    global_total = sum(group["size"] for group in groups) + scoped - covered
    unknown = scoped - verified
    physical_matched = sum(size for (program, address, size) in credit
                           if program in programs and any(a <= address and address + size <= b
                              for a, b in map(interval, programs[program]["ee_sections"])))
    return {"schema": 1, "kind": "unique-code-progress", "target": catalog["target"], "catalogue_complete": complete,
            "metrics": {"unique_total_bytes": global_total, "unique_matched_bytes": matched,
                        "unique_matched_percent": matched / global_total * 100 if global_total else 0,
                        "physical_total_bytes": scoped,
                        "physical_matched_bytes": physical_matched},
            "quality": {"boundary_supported_bytes": verified, "unknown_bytes": unknown,
                        "residual_gap_bytes": scoped - covered,
                        "provisional_total_bytes": sum(key[0] for key in provisional) + scoped - covered,
                        "certified": False, "structural_partition_complete": complete, "provisional_certified": False,
                        "data_policy": catalog.get("data_policy", "legacy-template-only-data-policy"),
                        "original_data_ownership_proven": False,
                        "policy": "unsupported extents and gaps remain uncollapsed; unowned data targets and their field encoding positions remain in primary identity; structural receipts are not original source boundaries"},
            "group_policy": catalog.get("group_policy", "legacy-relocation-template-partition"),
            "data_policy": catalog.get("data_policy", "legacy-template-only-data-policy"),
            "graph_refinement": {key: value for key, value in graph.items() if key not in ("class_by_id", "groups")} if graph else None,
            "policy": "one equal-sized representative per conservative structural class refined by static target classes and retained unowned data address operands; all verified placements require current exact C integration; no semantic, original-data-object or original-source equivalence claim",
            "validated_subset": {"total_unique_bytes": total, "matched_c_unique_bytes": matched,
                                 "any_c_unique_bytes": sum(g["size"] for g in valid if g["any_placement_matched_c"]),
                                 "groups": len(valid), "matched_c_groups": sum(g["all_placements_matched_c"] for g in valid)},
            "provisional_partition": {"representative_bytes": sum(key[0] for key in provisional),
                                      "groups": len(provisional), "certified": False},
            "coverage": {"scoped_ee_bytes": scoped, "catalogued_interval_bytes": covered,
                         "verified_boundary_bytes": verified, "unresolved_gap_bytes": scoped - covered,
                         "excluded_vu_bytes": sum(p["excluded_vu_bytes"] for p in programs.values()),
                         "placements": len(rows), "aliases": sum(len(r.get("aliases", [])) for r in rows)},
            "groups": groups}


def require_current_policy(catalog: dict) -> None:
    require(catalog.get("group_policy") == "graph-refined-structural-templates"
            and catalog.get("dependency_policy") == "complete-static-control-dependencies"
            and catalog.get("data_policy") == "retain-unowned-data-address-operands",
            "Current catalogue requires graph-refined complete static dependencies and retained unowned data operands")


def load_current_credit(repo: Path, catalog: dict) -> dict:
    """Reuse the existing exact proof validators; never credit a candidate or partial range."""
    pins = validate_pins(catalog, repo)
    require(any(pin["sha256"] == catalog["normalizer"]["sha256"] for pin in catalog["input_pins"]),
            "Normalizer instrument lacks a current input pin")
    read = lambda path: json.loads((repo / path).read_bytes())
    overlays = read("config/overlays.json")
    required = {"config/target.json", "config/overlays.json", "config/progress-scope.json",
                "progress/report.json", "progress/integration.json", "progress/candidates.json",
                "scripts/decomp_report.py", "scripts/unique_code_report.py"}
    required.update(f"progress/levels/{row['level']}.json" for row in overlays["levels"])
    require_current_policy(catalog)
    required.add("scripts/call_graph_refinement.py")
    if catalog.get("boot_binding_proof") is not None:
        required.update({"scripts/verify_boot_bindings.py", "scripts/validate_boot_binding.py",
                         "scripts/elf_tools.py", "scripts/relocation_identity.py"})
        descriptor = catalog["boot_binding_proof"]
        require(isinstance(descriptor, dict) and isinstance(descriptor.get("path"), str),
                "Invalid boot binding descriptor")
        required.add(descriptor["path"])
    require(required <= pins, "Catalogue lacks current proof/input freshness pins")
    scope = read("config/progress-scope.json")
    identities = {p["name"]: p["sha256"] for p in scope["programs"]}
    require({p["program"] for p in catalog["programs"]} == set(identities),
            "Catalogue does not cover the pinned program identities")
    for program in catalog["programs"]:
        require(program["reference_sha256"] == identities[program["program"]],
                "Catalogue reference identity mismatch")
        physical = next(p for p in scope["programs"] if p["name"] == program["program"])
        ee = [{"address": s["address"], "size": s["size"]} for s in physical["sections"]
              if s["flags"] & 4 and s["name"] != ".vutext"]
        actual = program["ee_sections"]
        require([{k:s[k] for k in ("address", "size")} for s in actual] == ee,
                "Catalogue EE scope differs from pinned physical scope")
        if catalog.get("section_identity_policy") is not None:
            require([s["name"] for s in actual] == [s["name"] for s in physical["sections"]
                    if s["flags"] & 4 and s["name"] != ".vutext"],
                    "Catalogue executable section names differ from pinned physical scope")
        require(program["excluded_vu_bytes"] == sum(s["size"] for s in physical["sections"]
                                                  if s["name"] == ".vutext"), "VU scope mismatch")
    sys.path.insert(0, str(repo / "scripts"))
    import decomp_report
    require(decomp_report.ROOT.resolve() == repo.resolve(), "Proof validator repository mismatch")
    integration = read("progress/integration.json")
    levels = [read(f"progress/levels/{row['level']}.json") for row in overlays["levels"]]
    decomp_report.generate(scope, read("config/target.json"),
                           overlays, read("progress/report.json"), integration, levels)
    decomp_report.validate_object_proof(integration, read("progress/candidates.json"))
    credit = {}
    for proof in [integration, *levels]:
        program = proof["program"] if proof["program"] == "boot" else "levels/" + proof["program"]
        for function in proof["functions"]:
            key = (program, function["address"], function["size"])
            require(key not in credit, "Duplicate current C placement")
            credit[key] = function["reference_sha256"]
    return credit


def objdiff(report: dict) -> dict:
    """A separate conservative global report; never replace the physical report."""
    from decomp_report import measures
    units = [{"name": g["id"], "measures": measures(g["size"], 0, 1,
              g["size"] if g["all_placements_matched_c"] else 0,
              int(g["all_placements_matched_c"])),
              "metadata": {"complete": g["all_placements_matched_c"], "autoGenerated": True,
                           "progressCategories": ["unique"]}}
             for g in report["groups"]]
    residual = report["coverage"]["unresolved_gap_bytes"]
    if residual:
        units.append({"name": "unresolved-ee-gaps", "measures": measures(residual, 0, 1),
                      "metadata": {"complete": False, "autoGenerated": True,
                                   "progressCategories": ["unique"]}})
    metrics = report["metrics"]
    aggregate = measures(metrics["unique_total_bytes"], 0, len(units),
                         metrics["unique_matched_bytes"], report["validated_subset"]["matched_c_groups"])
    # objdiff v2 rejects unknown top-level fields. Quality/provenance belong
    # in the separate paired summary, not the interchange report.
    return {"version": 2, "measures": aggregate, "units": units,
            "categories": [{"id": "unique", "name": "Conservative unique EE partition", "measures": aggregate}]}


def read_catalog(path: Path) -> tuple[dict, bytes]:
    """Read plain or compressed structural proof metadata, never retail assets.

    Optional compression='gzip' keeps sha256 as the compressed payload pin.
    expanded_sha256 optionally pins the decompressed metadata too. Expanded
    chunks have a hard 256 MiB maximum; max_expanded_bytes may lower that limit.
    """
    data = path.read_bytes()
    catalog = json.loads(data)
    require(type(catalog.get("schema")) is int and catalog["schema"] == 1,
            "Invalid catalogue schema")
    if "function_chunks" not in catalog:
        return catalog, data
    require("functions" not in catalog, "Manifest cannot also supply inline functions")
    rows, seen = [], set()
    for chunk in catalog["function_chunks"]:
        relative = PurePosixPath(chunk["path"])
        require(not relative.is_absolute() and ".." not in relative.parts
                and ":" not in str(relative) and "\\" not in str(relative), "Invalid catalogue chunk path")
        location = (path.parent / str(relative)).resolve()
        require(location.is_relative_to(path.parent.resolve()) and location not in seen, "Duplicate or escaped chunk")
        seen.add(location)
        payload = location.read_bytes()
        sha(chunk.get("sha256"))
        require(digest(payload) == chunk["sha256"], "Stale catalogue chunk")
        limit = chunk.get("max_expanded_bytes", 256 * 1024 * 1024)
        require(type(limit) is int and 0 < limit <= 256 * 1024 * 1024,
                "Invalid expanded chunk size limit")
        compression = chunk.get("compression")
        require(compression in (None, "gzip"), "Unknown catalogue chunk compression")
        if compression == "gzip":
            try:
                with gzip.GzipFile(fileobj=io.BytesIO(payload), mode="rb") as stream:
                    payload = stream.read(limit + 1)
            except (OSError, EOFError) as error:
                raise ValueError("Malformed gzip catalogue metadata") from error
        require(len(payload) <= limit, "Expanded catalogue chunk exceeds size limit")
        if "expanded_sha256" in chunk:
            sha(chunk["expanded_sha256"])
            require(digest(payload) == chunk["expanded_sha256"], "Stale expanded catalogue chunk")
        if chunk.get("format") in ("ndjson", "compact-ndjson-v1"):
            entries = [json.loads(line) for line in payload.splitlines() if line.strip()]
            if chunk["format"] == "compact-ndjson-v1":
                entries = [expand_compact_row(row, chunk.get("program"), catalog["normalizer"]["sha256"])
                           for row in entries]
        else:
            require(chunk.get("format", "json") == "json", "Unknown catalogue chunk format")
            document = json.loads(payload)
            entries = document if isinstance(document, list) else document["functions"]
        require(isinstance(entries, list), "Invalid catalogue chunk rows")
        rows.extend(entries)
    catalog["functions"] = rows
    return catalog, data


def encoded(value: dict) -> bytes:
    return (json.dumps(value, indent=2, sort_keys=True) + "\n").encode()


def expand_compact_row(row: object, program: str, normalizer_sha256: str) -> dict:
    """Expand compact-ndjson-v1 without inventing an exact certificate.

    Nine fields: address, size, raw hash, boundary status, signature (or null),
    relocations, aliases, evidence, certificate. Certificate integer 1 explicitly
    records exact reconstruction equal to raw; 0/null means absent. Alternatively
    {exact: bool, normalized_sha256: template_hash, reconstructed_sha256: hash}
    records the actual result independently of the role-bearing signature.
    An optional tenth field contains every external static control dependency.
    Relocations are dictionaries or [kind, offset, target, role, evidence] for
    j26/gp16. HI16/LO16 pairing always retains explicit dictionary metadata.
    """
    require(isinstance(row, list) and len(row) in (9, 10), "Invalid compact row schema")
    address, size, raw, status, signature, relocations, aliases, evidence, receipt = row[:9]
    require(isinstance(program, str) and bool(program), "Compact chunk requires program")
    require(isinstance(relocations, list), "Invalid compact relocation list")
    expanded_relocations = []
    for relocation in relocations:
        if isinstance(relocation, dict):
            expanded_relocations.append(relocation)
        else:
            require(isinstance(relocation, list) and len(relocation) == 5
                    and relocation[0] in ("j26", "gp16"), "Invalid compact relocation schema")
            expanded_relocations.append(dict(zip(("kind", "offset", "target", "role", "evidence"), relocation)))
    require(type(address) is int and type(size) is int, "Invalid compact address or size")
    result = {"id": f"{program}@{address:08x}:{size}", "program": program,
              "address": address, "size": size, "raw_sha256": raw, "aliases": aliases,
              "boundary": {"status": status, "evidence": [evidence] if isinstance(evidence, str) else evidence},
              "normalization": None}
    if len(row) == 10:
        result["call_dependencies"] = row[9]
    if signature is None:
        require(receipt is None or type(receipt) is int and receipt == 0,
                "Compact certificate cannot exist without signature")
        require(not expanded_relocations, "Compact relocations cannot exist without signature")
        return result
    certificate = None
    if type(receipt) is int:
        require(receipt in (0, 1), "Invalid compact exact flag")
        if receipt == 1:
            certificate = {"raw_sha256": raw, "normalized_sha256": signature,
                           "reconstructed_sha256": raw, "normalizer_sha256": normalizer_sha256, "exact": True}
    elif receipt is not None:
        require(isinstance(receipt, dict) and set(receipt) == {"exact", "normalized_sha256", "reconstructed_sha256"}
                and type(receipt["exact"]) is bool, "Invalid compact certificate schema")
        certificate = {"raw_sha256": raw, "normalized_sha256": receipt["normalized_sha256"],
                       "reconstructed_sha256": receipt["reconstructed_sha256"],
                       "normalizer_sha256": normalizer_sha256, "exact": receipt["exact"]}
    result["normalization"] = {"signature_sha256": signature,
                               "relocations": expanded_relocations, "certificate": certificate}
    if certificate is not None:
        result["normalization"]["template_sha256"] = certificate["normalized_sha256"]
    return result


def current_boot_binding(repo, catalog):
    """Load the pinned static image binding; never infer it from section names."""
    descriptor = catalog.get("boot_binding_proof")
    if descriptor is None:
        return None
    from validate_boot_binding import load_boot_binding
    require(isinstance(descriptor, dict), "Invalid boot binding descriptor")
    require(descriptor.get("scope") == "combined-pinned-reference-images"
            and descriptor.get("runtime_preservation_proven") is False,
            "Boot descriptor must state static reference scope without runtime proof")
    sha(descriptor.get("source_catalog_sha256"))
    name = descriptor.get("path")
    require(isinstance(name, str) and name and ":" not in name and "\\" not in name,
            "Boot binding path must be repository-relative")
    relative = PurePosixPath(name)
    require(not relative.is_absolute() and ".." not in relative.parts and str(relative) == name,
            "Invalid boot binding path")
    path = (repo / name).resolve()
    require(path.is_relative_to(repo.resolve()), "Boot binding path escapes repository")
    sha(descriptor.get("sha256"))
    pins = {key: digest((repo / source).read_bytes()) for key, source in (
        ("proof_source_sha256", "scripts/verify_boot_bindings.py"),
        ("reader_sha256", "scripts/elf_tools.py"),
        ("decoder_source_sha256", "scripts/relocation_identity.py"))}
    result = load_boot_binding(path, descriptor["sha256"], catalog["functions"],
                               catalog["programs"], catalog["function_chunks"], pins)
    require(result.source_catalog_sha256 == descriptor["source_catalog_sha256"],
            "Boot descriptor original source context mismatch")
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--include-groups", action="store_true",
                        help="include derived group membership for private analysis; default output is a small summary")
    parser.add_argument("--objdiff-output", type=Path)
    args = parser.parse_args()
    catalog, catalog_bytes = read_catalog(args.catalog)
    credit = load_current_credit(args.repo, catalog)
    report = generate(catalog, credit, current_boot_binding(args.repo, catalog))
    report["catalog_sha256"] = digest(catalog_bytes)
    summary = report if args.include_groups else {key: value for key, value in report.items() if key != "groups"}
    outputs = [(args.output, encoded(summary))]
    if args.objdiff_output:
        outputs.append((args.objdiff_output, encoded(objdiff(report))))
    for path, data in outputs:
        if args.check:
            require(path.read_bytes() == data, f"Stale unique report: {path.name}")
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(data)
    print(json.dumps(report["validated_subset"], sort_keys=True))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, TypeError) as error:
        print(f"Unique catalogue export failed: {error}")
        raise SystemExit(2)

