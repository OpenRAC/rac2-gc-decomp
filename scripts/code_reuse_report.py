"""Supplementary lossless binding-template and proved authored-fragment reuse."""
from __future__ import annotations
import argparse
import gzip
import hashlib
import io
import json
from pathlib import Path
import re
import struct
import sys

SUPPORTED = {"qualified_complete", "flow_supported_inferred"}
POLICY = "binding-parameterized-lossless-machine-templates-v1"
CERT_FIELDS = {"raw_sha256", "normalized_sha256", "reconstructed_sha256", "normalizer_sha256", "exact"}


def encoded(value):
    return (json.dumps(value, sort_keys=True, separators=(",", ":")) + "\n").encode()


def digest(data):
    return hashlib.sha256(data).hexdigest()


def require(ok, message):
    if not ok:
        raise ValueError(message)


def member_key(row):
    return row["program"], row["address"], row["size"]


def supported(row):
    return row["boundary"]["status"] in SUPPORTED


def binding_alias_pattern(relocations):
    """Preserve equality of external (role, target) slots without absolute values."""
    identities, pattern = {}, []
    for rel in relocations:
        if rel.get("internal") is True:
            pattern.append(["internal", rel["relative_target"]])
        else:
            identity = (rel["role"], rel["target"])
            identities.setdefault(identity, len(identities))
            pattern.append(["external", identities[identity]])
    return pattern


def template_key(row):
    norm = row.get("normalization")
    if not supported(row):
        return (row["size"], "singleton", row["id"])
    certificate = norm["certificate"]
    require(set(certificate) == CERT_FIELDS and type(certificate["exact"]) is bool,
            "Unknown certificate field or invalid exact flag")
    # The maintained signature includes field roles, internal CFG and aliases.
    # Never recompute an address mask from guesses here.
    return (row["size"], norm["signature_sha256"],
            norm.get("template_sha256", norm["signature_sha256"]),
            json.dumps(binding_alias_pattern(norm["relocations"]), separators=(",", ":")))


def generate(catalog, credit, primary):
    from unique_code_report import validate_catalog, no_overlap, sha
    programs, rows = validate_catalog(catalog)
    for program in programs:
        no_overlap([{"address": a, "size": s} for (p, a, s) in credit if p == program])
    for key, value in credit.items():
        require(type(key) is tuple and len(key) == 3 and type(key[1]) is int
                and type(key[2]) is int and key[2] > 0, "Invalid C credit key")
        sha(value)
    primary_by_id = {}
    for group in primary["groups"]:
        for identity in group["members"]:
            require(identity not in primary_by_id, "Duplicate primary membership")
            primary_by_id[identity] = group["id"]
    require(set(primary_by_id) == {r["id"] for r in rows}, "Primary partition scope mismatch")
    groups = {}
    for row in rows:
        pin = credit.get(member_key(row))
        if pin is not None:
            require(pin == row["raw_sha256"], "Current C proof contradicts raw hash")
        groups.setdefault(template_key(row), []).append(row)
    families, details = [], []
    total = matched = any_c = unknown = supported_physical = data_unknown = graph_split = 0
    for key, members in sorted(groups.items()):
        members.sort(key=lambda r: (r["program"], r["address"], r["id"]))
        size = members[0]["size"]
        valid = all(supported(r) for r in members)
        credited = [member_key(r) in credit for r in members]
        unowned_data = any(rel["kind"] != "j26" for r in members
                           for rel in (r.get("normalization") or {}).get("relocations", []))
        classes = sorted({primary_by_id[r["id"]] for r in members})
        total += size
        matched += size if valid and all(credited) else 0
        any_c += size if valid and any(credited) else 0
        unknown += sum(r["size"] for r in members) if not valid else 0
        supported_physical += sum(r["size"] for r in members) if valid else 0
        data_unknown += size if valid and unowned_data else 0
        graph_split += len(classes) > 1
        family = {"id": digest(encoded(key)), "size": size, "placements": len(members),
                  "supported": valid, "all_placements_matched_c": valid and all(credited),
                  "any_placement_matched_c": valid and any(credited),
                  "unproved_original_data_ownership": unowned_data,
                  "conservative_class_count": len(classes),
                  "primary_split_reason": "static target classes or retained absolute data bindings" if len(classes) > 1 else None}
        families.append(family)
        if len(members) > 1:
            details.append(dict(family, conservative_classes=classes,
                members=[{"id": r["id"], "program": r["program"], "address": r["address"],
                          "size": r["size"], "raw_sha256": r["raw_sha256"],
                          "boundary": r["boundary"]["status"], "matched_c": member_key(r) in credit,
                          "relocations": r["normalization"]["relocations"]} for r in members]))
    physical = sum(s["size"] for p in programs.values() for s in p["ee_sections"])
    covered = sum(r["size"] for r in rows)
    gap = physical - covered
    require(gap >= 0 and primary["metrics"]["physical_total_bytes"] == physical,
            "Physical partition mismatch")
    total += gap
    require(0 <= matched <= any_c <= total <= physical, "Invalid paired totals")
    summary = {"schema": 1, "kind": "supplementary-code-reuse", "target": catalog["target"],
        "policy": POLICY, "metrics": {"template_total_bytes": total,
            "template_all_c_bytes": matched, "template_any_c_bytes": any_c,
            "template_all_c_percent": matched / total * 100 if total else 0,
            "physical_total_bytes": physical},
        "quality": {"supported_physical_bytes": supported_physical,
            "unsupported_singleton_physical_bytes": unknown, "residual_gap_bytes": gap,
            "data_owner_unproved_representative_bytes": data_unknown,
            "metadata_reconstruction_receipts_validated": True,
            "private_raw_replay_performed": False,
            "fresh_address_role_classification_performed": False,
            "address_role_authority": "pinned maintained catalogue/normalizer/pointer-theorem receipts",
            "original_source_equivalence_proven": False,
            "original_data_ownership_proven": False,
            "boundary_evidence_is_structural_not_original_source": True},
        "counts": {"placements": len(rows), "families_including_singletons": len(families),
            "multi_member_families": len(details),
            "families_split_by_conservative_identity": graph_split,
            "all_c_families": sum(f["all_placements_matched_c"] for f in families),
            "partial_c_families": sum(f["any_placement_matched_c"] and not f["all_placements_matched_c"] for f in families)},
        "primary_reference": {k: primary[k] for k in ("metrics", "quality", "group_policy", "data_policy") if k in primary},
        "limitations": ["Lossless placement-bound machine templates are not original source or semantic equivalence.",
            "Different callee classes and unowned data bindings are explicit supplementary parameters; conservative primary remains distinct.",
            "Asset-free validation checks pinned certificates; raw reconstruction needs the separate private replay option."]}
    return summary, details


def authored_subset(repo, credit):
    import source_layout as sl
    from boot_sdk_unit import admitted_units, load_catalog, unit_spec
    manifest = json.loads((repo / "config/source-layout.json").read_bytes())
    sources, _ = sl.render(repo, manifest)
    for name, data in sources.items():
        require(data == (repo / name).read_bytes(), "Stale generated authored source")
    boot, natives, _ = sl.load_inputs(repo)
    catalogs = {"candidates/boot.c": boot}
    catalogs.update({c["source"]: c for _, c in natives})
    for unit in admitted_units(repo):
        catalogs[unit_spec(unit)["source"]] = load_catalog(repo, unit)
    require(set(catalogs) == set(sources), "Authored sources/catalogues differ")
    slots = {}
    for name, catalog in sorted(catalogs.items()):
        piece_spans, at = [], 0
        for piece in manifest["recipes"][name]["pieces"]:
            data = (repo / piece["fragment"]).read_bytes()
            for token, replacement in piece.get("replacements", {}).items():
                # Existing recipe literals (e.g. measured NOSDA attributes) are
                # retained as source variants, never erased by lexical renaming.
                data = data.replace(token.encode(), replacement.encode())
            piece_spans.append((at, at + len(data), piece)); at += len(data)
        per_piece = {}
        for f in catalog["functions"]:
            start, end = sl.function_span(sources[name], f["symbol"])
            owners = [(i, p) for i, (a, b, p) in enumerate(piece_spans) if a <= start and end <= b]
            require(len(owners) == 1, "Function slot crosses canonical fragment boundary")
            index, piece = owners[0]
            per_piece.setdefault(index, []).append((start, f, piece))
        for entries in per_piece.values():
            for ordinal, (_, f, piece) in enumerate(sorted(entries, key=lambda e: e[0])):
                slots[(name, f["symbol"])] = {"fragment": piece["fragment"],
                    "fragment_sha256": piece["sha256"], "definition_slot": ordinal,
                    "source_sha256": digest(sources[name]), "canonical_function_size": f["size"],
                    "literal_recipe_variant": {t: v for t, v in piece.get("replacements", {}).items()
                        if re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", v) is None}}
    overlays = json.loads((repo / "config/overlays.json").read_bytes())
    proofs = [json.loads((repo / "progress/integration.json").read_bytes())]
    proofs += [json.loads((repo / f"progress/levels/{l['level']}.json").read_bytes()) for l in overlays["levels"]]
    groups, seen = {}, set()
    for proof in proofs:
        program = "boot" if proof["program"] == "boot" else "levels/" + proof["program"]
        for f in proof["functions"]:
            key = (program, f["address"], f["size"])
            require(key not in seen and credit.get(key) == f["reference_sha256"], "Unproved/duplicate source placement")
            seen.add(key)
            slot = slots.get((f["candidate_source"], f["symbol"]))
            require(slot is not None, "C placement has no canonical function slot")
            identity = (slot["fragment_sha256"], slot["definition_slot"], digest(encoded(slot["literal_recipe_variant"])))
            groups.setdefault(identity, []).append(dict(slot, program=program, address=f["address"],
                size=f["size"], symbol=f["symbol"], source=f["candidate_source"], raw_sha256=f["reference_sha256"]))
    require(seen == set(credit), "Not every current exact placement has a canonical source association")
    details = []
    for identity, members in sorted(groups.items()):
        members.sort(key=lambda m: (m["program"], m["address"]))
        sizes = [m["size"] for m in members]
        details.append({"id": digest(encoded(identity)), "fragment_sha256": identity[0],
            "definition_slot": identity[1], "literal_recipe_variant_sha256": identity[2],
            "placements": len(members), "min_size": min(sizes),
            "max_size": max(sizes), "source_context_variants": len({m["source_sha256"] for m in members}),
            "members": members})
    return {"known_sources": len(sources), "exact_C_placements_associated": len(seen),
        "canonical_fragment_function_slots": len(details),
        "multi_placement_source_slots": sum(d["placements"] > 1 for d in details),
        "scope": "Current proved authored-fragment reuse including boot-shared placements; independent subset, no global source denominator.",
        "original_source_equivalence_proven": False}, [d for d in details if d["placements"] > 1]


def replay(catalog, references):
    from unique_code_report import validate_catalog
    from relocation_identity import _field_parts, reconstruct, NORMALIZER_SHA256
    from elf_tools import read_elf, _mappings, _mapped_bytes
    programs, rows = validate_catalog(catalog)
    require(catalog["normalizer"]["sha256"] == NORMALIZER_SHA256, "Private replay normalizer drift")
    by_program = {}
    for row in rows:
        by_program.setdefault(row["program"], []).append(row)
    reconstructed = 0
    for program, members in sorted(by_program.items()):
        path = references / ("boot.elf" if program == "boot" else program + "/overlay.elf")
        require(path.resolve().is_relative_to(references.resolve()), "Private reference path escapes root")
        data = path.read_bytes()
        require(digest(data) == programs[program]["reference_sha256"], "Wrong private reference pin")
        mappings = _mappings(read_elf(path))
        for row in members:
            raw = _mapped_bytes(data, mappings, row["address"], row["size"])
            require(digest(raw) == row["raw_sha256"], "Private raw body mismatch")
            norm = row.get("normalization")
            if not supported(row):
                continue
            template = bytearray(raw)
            for rel in norm["relocations"]:
                for offset, mask, _ in _field_parts(rel, row["address"]):
                    word = struct.unpack_from("<I", template, offset)[0]
                    value = rel["relative_target"] >> 2 if rel["kind"] == "j26" and rel.get("internal") else 0
                    struct.pack_into("<I", template, offset, (word & ~mask) | (value & mask))
            template = bytes(template)
            require(digest(template) == norm.get("template_sha256", norm["signature_sha256"]), "Private template hash mismatch")
            schema = [{k: v for k, v in rel.items() if k in ("offset", "kind", "high_offset", "low_offset", "lo_mode", "role", "internal", "relative_target")} for rel in norm["relocations"]]
            signature = digest(b"ee-relocation-template-v1\0" + template + json.dumps(schema, sort_keys=True, separators=(",", ":")).encode())
            require(signature == norm["signature_sha256"], "Private role signature mismatch")
            require(reconstruct(template, row["address"], norm["relocations"]) == raw, "Private reconstruction differs")
            reconstructed += 1
    return {"state": "all_raw_rows_and_supported_template_reconstructions_exact",
            "scope": "raw reference/template/role-signature/reconstruction replay; supplied address roles not reclassified",
            "fresh_address_role_classification_performed": False, "raw_rows": len(rows),
            "supported_reconstructed_rows": reconstructed, "catalogue_reference_pins": {p: v["reference_sha256"] for p, v in sorted(programs.items())}}


def snapshot(repo, catalog_path, catalog):
    import source_layout as sl
    inputs = {p["path"]: p["sha256"] for p in catalog["input_pins"]}
    manifest = json.loads((repo / "config/source-layout.json").read_bytes())
    paths = {"config/source-layout.json", "scripts/source_layout.py", "scripts/unique_code_report.py",
             "scripts/relocation_identity.py", "scripts/boot_sdk_unit.py"}
    paths.update(manifest["recipes"])
    paths.update(p["fragment"] for r in manifest["recipes"].values() for p in r["pieces"])
    for path in paths:
        inputs[path] = digest(sl.contained(repo, path).read_bytes())
    require(catalog_path.resolve().is_relative_to(repo.resolve()), "Catalogue must be inside repository")
    inputs[catalog_path.resolve().relative_to(repo.resolve()).as_posix()] = digest(catalog_path.read_bytes())
    raw_manifest = json.loads(catalog_path.read_bytes())
    for chunk in raw_manifest.get("function_chunks", []):
        path = catalog_path.parent / chunk["path"]
        inputs[path.resolve().relative_to(repo.resolve()).as_posix()] = digest(path.read_bytes())
    inputs["scripts/code_reuse_report.py"] = digest(Path(__file__).read_bytes())
    return dict(sorted(inputs.items()))


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--families-output", type=Path, required=True)
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--references", type=Path, help="Private pinned reference root; optional full raw replay")
    args = parser.parse_args(argv)
    repo = args.repo.resolve(); sys.path.insert(0, str(repo / "scripts"))
    import unique_code_report as uq
    catalog, catalog_bytes = uq.read_catalog(args.catalog)
    credit = uq.load_current_credit(repo, catalog)
    primary = uq.generate(catalog, credit, uq.current_boot_binding(repo, catalog))
    summary, details = generate(catalog, credit, primary)
    authored, authored_details = authored_subset(repo, credit)
    summary["authored_C_reuse_subset"] = authored
    summary["catalog_sha256"] = digest(catalog_bytes)
    summary["input_sha256"] = snapshot(repo, args.catalog, catalog)
    if args.references:
        summary["private_raw_replay"] = replay(catalog, args.references)
        summary["quality"]["private_raw_replay_performed"] = True
    payload = encoded({"schema": 1, "policy": POLICY, "template_families": details,
                       "authored_C_fragment_families": authored_details})
    if args.families_output.suffix == ".gz":
        buffer = io.BytesIO()
        with gzip.GzipFile(fileobj=buffer, mode="wb", filename="", mtime=0) as stream:
            stream.write(payload)
        payload = buffer.getvalue()
    summary["families_sha256"] = digest(payload)
    for path, data in [(args.output, encoded(summary)), (args.families_output, payload)]:
        if args.check:
            require(path.read_bytes() == data, "Stale supplementary reuse output: " + path.name)
        else:
            path.parent.mkdir(parents=True, exist_ok=True); path.write_bytes(data)
    print(json.dumps(summary["metrics"], sort_keys=True))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, TypeError) as error:
        print("Code reuse export failed: " + str(error)); raise SystemExit(2)
