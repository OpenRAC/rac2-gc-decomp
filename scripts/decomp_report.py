"""Export RAC2 section totals and proven C integrations in objdiff report v2 format.

The boot proof in ``progress/integration.json`` stays the anchor: its source,
catalogue and object hashes were recomputed from disk.  A caller may add one
proof per level overlay (``--level-proof``); each one is validated on its own
identity, reviewed placement and progress gate before its bytes are counted, and
matched bytes and units are summed per program.  Without level proofs the export
is exactly what it has always been.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BOOT_LOAD_BYTES = 2521763
BOOT_LOAD_SEGMENTS = 2


def measures(code_size: int, data_size: int, units: int,
             complete_code: int = 0, complete_units: int = 0) -> dict:
    percent = complete_code / code_size * 100 if code_size else 0
    fuzzy_percent = complete_code / (code_size + data_size) * 100 if code_size + data_size else 0
    return {"totalCode": str(code_size), "matchedCode": str(complete_code), "matchedCodePercent": percent,
            "totalData": str(data_size), "matchedData": "0", "matchedDataPercent": 0,
            "completeCode": str(complete_code), "completeCodePercent": percent, "completeData": "0",
            "completeDataPercent": 0, "fuzzyMatchPercent": fuzzy_percent,
            "totalUnits": units, "completeUnits": complete_units}


def require_hash(value: object) -> None:
    if not isinstance(value, str) or re.fullmatch(r"[0-9a-f]{64}", value) is None:
        raise ValueError("Proof requires SHA-256 hashes")


def require_tools(value: object) -> None:
    if not isinstance(value, dict) or not value:
        raise ValueError("Proof requires instrument hashes")
    for name, digest in value.items():
        if not isinstance(name, str) or not name.strip():
            raise ValueError("Invalid proof instrument")
        require_hash(digest)


def validate_integration(integration: dict, target: dict, progress: dict) -> list[dict]:
    if not isinstance(integration, dict):
        raise ValueError("Invalid integration proof")
    if integration.get("schema") == 3 and integration.get("kind") == "boot-c-owner-integration":
        from boot_sdk_unit import validate_union
        default_review = json.loads((ROOT / "progress/candidates.json").read_bytes())
        def check_default(legacy):
            adapted = dict(progress, integrated_functions=len(legacy["functions"]),
                           decompiled_functions=len(legacy["functions"]))
            validate_integration(legacy, target, adapted)
        validate_union(integration, default_review, ROOT, check_default, validate_object_proof)
        if (type(progress.get("integrated_functions")) is not int
                or type(progress.get("decompiled_functions")) is not int
                or progress["integrated_functions"] != len(integration["functions"])
                or progress["decompiled_functions"] != len(integration["functions"])):
            raise ValueError("Boot union progress count mismatch")
        return sorted(integration["functions"], key=lambda function: function["address"])
    if not isinstance(integration, dict):
        raise ValueError("Invalid integration proof")
    if (integration.get("target") != target["serial"]
            or progress.get("target") != target["serial"]
            or integration.get("reference_sha256") != target["boot"]["sha256"]
            or integration.get("state") != "integrated"
            or integration.get("candidate_source") != "candidates/boot.c"):
        raise ValueError("Integration identity, source or state mismatch")
    for field in ("reference_sha256", "source_sha256", "catalog_sha256"):
        require_hash(integration.get(field))
    require_tools(integration.get("tools"))
    source = (ROOT / "candidates/boot.c").read_bytes()
    catalog_bytes = (ROOT / "config/candidate-catalog.json").read_bytes()
    if (hashlib.sha256(source).hexdigest() != integration["source_sha256"]
            or hashlib.sha256(catalog_bytes).hexdigest() != integration["catalog_sha256"]):
        raise ValueError("Integration source or catalog hash mismatch")
    if re.search(rb"\b(?:asm|__asm__|__asm|INCLUDE_ASM)\b|(?m:^[ \t]*(?:(?:[A-Za-z_.$][A-Za-z0-9_.$]*|[0-9]+):[ \t]*)?\.(?:byte|word)\b(?:[ \t]+(?![ \t]*=)\S|[ \t]*$))", source):
        raise ValueError("Integrated C must not embed assembly or retail bytes")
    catalog = json.loads(catalog_bytes)
    if (catalog["target"] != target["serial"]
            or catalog["reference_sha256"] != target["boot"]["sha256"]):
        raise ValueError("Candidate catalog identity mismatch")
    known = {function["symbol"]: function for function in catalog["functions"]}
    if len(known) != len(catalog["functions"]):
        raise ValueError("Duplicate catalog function")
    gate = integration.get("full_boot_gate")
    if (not isinstance(gate, dict) or gate.get("matched") is not True
            or type(gate.get("bytes_compared")) is not int
            or gate["bytes_compared"] != BOOT_LOAD_BYTES
            or type(gate.get("segments")) is not int or gate["segments"] != BOOT_LOAD_SEGMENTS):
        raise ValueError("Integration requires the complete boot gate")
    boot_gate = progress.get("g1", {})
    if (boot_gate.get("matched") is not True
            or boot_gate.get("reference_sha256") != target["boot"]["sha256"]
            or type(boot_gate.get("bytes_compared")) is not int
            or boot_gate["bytes_compared"] != BOOT_LOAD_BYTES):
        raise ValueError("Progress boot gate contradicts integration")
    for field in ("reference_sha256", "candidate_sha256"):
        if field in gate and gate[field] != boot_gate.get(field):
            raise ValueError("Integration boot gate identity mismatch")
    for field in ("object_sha256", "c_object_sha256", "candidate_elf_sha256", "checker_sha256"):
        if field in integration:
            require_hash(integration[field])
    functions = integration.get("functions")
    if not isinstance(functions, list) or not functions:
        raise ValueError("Integration requires complete C functions")
    seen = set()
    for function in functions:
        if not isinstance(function, dict):
            raise ValueError("Invalid integrated function")
        symbol = function.get("symbol")
        address, size = function.get("address"), function.get("size")
        if not isinstance(symbol, str) or symbol not in known or symbol in seen:
            raise ValueError("Unknown or duplicate integrated function")
        seen.add(symbol)
        if (function.get("matched") is not True or function.get("integrated") is not True
                or function.get("program") != "boot"
                or type(address) is not int or address < 0 or address % 4
                or type(size) is not int or size <= 0 or size % 4
                or address != known[symbol]["address"] or size != known[symbol]["size"]):
            raise ValueError("Integration requires the complete catalogued boot function")
        if "different_bytes" in function and function["different_bytes"] != 0:
            raise ValueError("Integrated function has mismatched bytes")
        if "reference_sha256" in function or "candidate_sha256" in function:
            require_hash(function.get("reference_sha256"))
            require_hash(function.get("candidate_sha256"))
            if function["reference_sha256"] != function["candidate_sha256"]:
                raise ValueError("Integrated function byte hashes differ")
    functions = sorted(functions, key=lambda function: function["address"])
    for previous, current in zip(functions, functions[1:]):
        if previous["address"] + previous["size"] > current["address"]:
            raise ValueError("Overlapping integrated functions")
    for field in ("integrated_functions", "decompiled_functions"):
        if type(progress.get(field)) is not int or progress[field] != len(functions):
            raise ValueError("Progress function count contradicts integration")
    if (type(integration.get("matched_code_bytes")) is not int
            or integration["matched_code_bytes"] != sum(function["size"] for function in functions)):
        raise ValueError("Integration byte count mismatch")
    return functions


def validate_object_proof(integration: dict, proof: dict) -> None:
    if not isinstance(integration, dict):
        raise ValueError("Invalid integration proof")
    if integration.get("schema") == 3 and integration.get("kind") == "boot-c-owner-integration":
        from boot_sdk_unit import validate_union
        target = json.loads((ROOT / "config/target.json").read_bytes())
        progress = json.loads((ROOT / "progress/report.json").read_bytes())
        def check_default(legacy):
            adapted = dict(progress, integrated_functions=len(legacy["functions"]),
                           decompiled_functions=len(legacy["functions"]))
            validate_integration(legacy, target, adapted)
        validate_union(integration, proof, ROOT, check_default, validate_object_proof)
        return
    if not isinstance(proof, dict):
        raise ValueError("Invalid candidate object proof")
    for field in ("target", "reference_sha256", "source_sha256", "catalog_sha256"):
        if proof.get(field) != integration.get(field):
            raise ValueError("Candidate object proof identity mismatch")
    for field in ("object_sha256", "candidate_elf_sha256", "checker_sha256"):
        require_hash(proof.get(field))
        if field != "candidate_elf_sha256" and field in integration and integration[field] != proof[field]:
            raise ValueError("Candidate object proof hash mismatch")
    if "c_object_sha256" in integration and integration["c_object_sha256"] != proof["object_sha256"]:
        raise ValueError("Integrated C object proof hash mismatch")
    require_tools(proof.get("tools"))
    if any(integration["tools"].get(name) != digest for name, digest in proof["tools"].items()):
        raise ValueError("Candidate object proof instrument mismatch")
    results = proof.get("functions")
    if not isinstance(results, list) or any(not isinstance(result, dict) for result in results):
        raise ValueError("Invalid candidate object function proof")
    known = {result["symbol"]: result for result in results}
    if len(known) != len(results):
        raise ValueError("Duplicate candidate object function proof")
    for function in integration["functions"]:
        result = known.get(function["symbol"], {})
        if (result.get("matched") is not True or result.get("different_bytes") != 0
                or type(result.get("address")) is not int or result["address"] != function["address"]
                or type(result.get("size")) is not int or result["size"] != function["size"]):
            raise ValueError("Invalid complete candidate object function proof")
        require_hash(result.get("reference_sha256"))
        require_hash(result.get("candidate_sha256"))
        if result["reference_sha256"] != result["candidate_sha256"]:
            raise ValueError("Candidate object function bytes differ")
        for field in ("reference_sha256", "candidate_sha256"):
            if field in function and function[field] != result[field]:
                raise ValueError("Integration and candidate object function hashes disagree")


def level_placements(document: dict, program: str, target: dict, overlays: dict,
                     boot_catalog: dict) -> dict[str, dict]:
    """The reviewed placement of the boot's own C bodies at one level's addresses.

    A level placement may only reuse a reviewed boot body with its complete
    reviewed size, and two bodies may never claim one address.  Anything else is
    not the reviewed placement and must not be counted.
    """
    pinned = {entry["level"]: entry["sha256"] for entry in overlays["levels"]}
    if document.get("target") != target["serial"]:
        raise ValueError("Level catalog identity mismatch")
    levels = document.get("levels")
    if not isinstance(levels, dict) or program not in levels or program not in pinned:
        raise ValueError("No reviewed level placement catalog for this program")
    entry = levels[program]
    if not isinstance(entry, dict) or entry.get("reference_sha256") != pinned[program]:
        raise ValueError("Level catalog is not the pinned overlay identity")
    reviewed = {function["symbol"]: function["size"] for function in boot_catalog["functions"]}
    functions = entry.get("functions")
    if not isinstance(functions, list) or not functions:
        raise ValueError("Level placement requires at least one reviewed body")
    placements = {}
    addresses = set()
    for function in functions:
        if not isinstance(function, dict):
            raise ValueError("Invalid level placement")
        symbol, address, size = function.get("symbol"), function.get("address"), function.get("size")
        if symbol not in reviewed or reviewed[symbol] != size:
            raise ValueError("Level placement must reuse a reviewed complete C body")
        if type(address) is not int or type(size) is not int or size <= 0 or size % 4 or address % 4:
            raise ValueError("Invalid level placement boundary")
        if symbol in placements or address in addresses:
            raise ValueError("Duplicate level placement symbol or address")
        placements[symbol] = {"symbol": symbol, "address": address, "size": size}
        addresses.add(address)
    return placements


def validate_level_proof(proof: dict, target: dict, overlays: dict, progress: dict,
                         integration: dict, catalog_bytes: bytes, boot_catalog: dict,
                         expected_totals: tuple[int, int] | None = None) -> list[dict]:
    """One level overlay's C integration proof, held to the same rules as the boot.

    The boot proof is the source of truth for the reviewed C: its source hash was
    recomputed from disk, so a level proof may only cite that exact reviewed
    source and those instruments.  The level catalogue is re-read and hashed
    here, its placement is the only admissible set of functions, and the level's
    own recorded progress gate must agree with the proof.
    """
    if not isinstance(proof, dict):
        raise ValueError("Invalid level integration proof")
    program = proof.get("program")
    pinned = {entry["level"]: entry["sha256"] for entry in overlays["levels"]}
    if (proof.get("target") != target["serial"] or progress.get("target") != target["serial"]
            or not isinstance(program, str) or program not in pinned
            or proof.get("reference_sha256") != pinned[program]
            or proof.get("state") != "integrated"
            or proof.get("candidate_source") != "candidates/boot.c"):
        raise ValueError("Level integration identity, source or state mismatch")
    for field in ("reference_sha256", "source_sha256", "catalog_sha256"):
        require_hash(proof.get(field))
    if proof["source_sha256"] != integration["source_sha256"]:
        raise ValueError("Level integration does not reuse the verified boot C source")
    if hashlib.sha256(catalog_bytes).hexdigest() != proof["catalog_sha256"]:
        raise ValueError("Level integration catalog hash mismatch")
    require_tools(proof.get("tools"))
    if any(integration["tools"].get(name) != digest for name, digest in proof["tools"].items()):
        raise ValueError("Level integration instrument mismatch")
    placements = level_placements(json.loads(catalog_bytes), program, target, overlays, boot_catalog)
    functions = proof.get("functions")
    if not isinstance(functions, list) or not functions:
        raise ValueError("Level integration requires complete C functions")
    seen = set()
    for function in functions:
        if not isinstance(function, dict):
            raise ValueError("Invalid level integrated function")
        symbol = function.get("symbol")
        address, size = function.get("address"), function.get("size")
        placement = placements.get(symbol) if isinstance(symbol, str) else None
        if not isinstance(symbol, str) or symbol in seen:
            raise ValueError("Unknown or duplicate level integrated function")
        seen.add(symbol)
        if (function.get("matched") is not True or function.get("integrated") is not True
                or function.get("program") != program
                or type(address) is not int or address < 0 or address % 4
                or type(size) is not int or size <= 0 or size % 4
                or placement is None or address != placement["address"] or size != placement["size"]):
            raise ValueError("Level integration requires the complete catalogued level function")
        if "different_bytes" in function and function["different_bytes"] != 0:
            raise ValueError("Level integrated function has mismatched bytes")
        if "reference_sha256" in function or "candidate_sha256" in function:
            require_hash(function.get("reference_sha256"))
            require_hash(function.get("candidate_sha256"))
            if function["reference_sha256"] != function["candidate_sha256"]:
                raise ValueError("Level integrated function byte hashes differ")
    if seen != set(placements):
        raise ValueError("Level integration does not cover the reviewed placement")
    functions = sorted(functions, key=lambda function: function["address"])
    for previous, current in zip(functions, functions[1:]):
        if previous["address"] + previous["size"] > current["address"]:
            raise ValueError("Overlapping level integrated functions")
    gate = proof.get("full_level_gate")
    if (not isinstance(gate, dict) or gate.get("matched") is not True
            or type(gate.get("bytes_compared")) is not int or gate["bytes_compared"] <= 0
            or type(gate.get("segments")) is not int or gate["segments"] <= 0):
        raise ValueError("Level integration requires the complete level gate")
    gates = progress.get("g3")
    recorded = ([item for item in gates if isinstance(item, dict) and item.get("level") == program]
                if isinstance(gates, list) else [])
    if (len(recorded) != 1 or recorded[0].get("matched") is not True
            or recorded[0].get("reference_sha256") != pinned[program]
            or recorded[0].get("bytes_compared") != gate["bytes_compared"]):
        raise ValueError("Progress level gate contradicts integration")
    count, byte_count = expected_totals or (len(functions), sum(function["size"] for function in functions))
    if (("integrated_c_functions" in recorded[0] and recorded[0]["integrated_c_functions"] != count)
            or ("integrated_c_bytes" in recorded[0]
                and recorded[0]["integrated_c_bytes"] != byte_count)):
        raise ValueError("Progress level byte count contradicts integration")
    for field in ("object_sha256", "c_object_sha256", "candidate_elf_sha256", "checker_sha256"):
        if field in proof:
            require_hash(proof[field])
    if (type(proof.get("matched_code_bytes")) is not int
            or proof["matched_code_bytes"] != sum(function["size"] for function in functions)):
        raise ValueError("Level integration byte count mismatch")
    return functions


def validate_native_level_proof(proof: dict, target: dict, overlays: dict, progress: dict,
                                integration: dict, catalog_bytes: bytes, boot_catalog: dict,
                                candidate_review: Path | None = None) -> list[dict]:
    """Validate both objects and their union; neither a boot proof nor a partial gate suffices."""
    from level_native import load_catalog, validate_review, paths, ranges, dependencies, file_hash
    level = proof.get("program")
    if proof.get("schema") != 2 or proof.get("kind") != "level-c-integration":
        raise ValueError("Invalid native level integration kind")
    reconstruction_tools = proof.get("reconstruction_tools")
    if not isinstance(reconstruction_tools, dict) or set(reconstruction_tools) != {"Ps2EeAs.exe", "ld.exe"}:
        raise ValueError("Native integration requires both reconstruction instrument identities")
    for value in reconstruction_tools.values():
        require_hash(value)
    if any(progress.get("tools", {}).get(name) != value for name, value in reconstruction_tools.items()):
        raise ValueError("Native reconstruction instruments disagree with runtime gates")
    catalog = load_catalog(level, ROOT)
    source_path, native_catalog_path, review_path = paths(level)
    native = proof.get("native")
    if (not isinstance(native, dict) or native.get("source") != source_path
            or native.get("catalog_path") != native_catalog_path or native.get("review_path") != review_path
            or native.get("review_sha256") != file_hash(ROOT / review_path)
            or type(proof.get("reference_entry")) is not int or type(proof.get("candidate_entry")) is not int
            or proof.get("reference_entry") != catalog["entry"] or proof.get("candidate_entry") != catalog["entry"]
            or proof.get("dependency_sha256") != dependencies(level, ROOT, candidate_review)):
        raise ValueError("Native integration source, entry or dependency mismatch")
    review = json.loads((ROOT / review_path).read_bytes())
    validate_review(review, catalog, level, ROOT)
    qualified = native.get("object_qualification")
    validate_review(qualified, catalog, level, ROOT)
    if (native.get("object_sha256") != review["object_sha256"]
            or qualified["object_sha256"] != review["object_sha256"]
            or qualified["tools"] != review["tools"]
            or any(proof.get("tools", {}).get(name) != value for name, value in review["tools"].items())):
        raise ValueError("Native integration source/object or instruments disagree with review")
    functions = proof.get("functions")
    if not isinstance(functions, list) or not functions:
        raise ValueError("Native integration requires complete functions")
    ranges(functions)
    count, total = len(functions), sum(item["size"] for item in functions)
    shared = proof.get("shared")
    if not isinstance(shared, dict):
        raise ValueError("Native integration requires the unchanged shared boot gate")
    review_path = candidate_review or ROOT / "progress/candidates.json"
    boot_review = json.loads(review_path.read_bytes())
    if proof.get("boot_review_sha256") != file_hash(review_path):
        raise ValueError("Native integration cites a different boot review")
    require_hash(proof.get("candidate_elf_sha256"))
    require_hash(shared.get("c_object_sha256"))
    if (shared["c_object_sha256"] != boot_review.get("object_sha256")
            or proof.get("c_object_sha256") != shared["c_object_sha256"]
            or proof.get("candidate_elf_sha256") != shared.get("candidate_elf_sha256")
            or any(proof.get(key) != shared.get(key) for key in
                   ("source_sha256", "catalog_sha256", "candidate_source"))):
        raise ValueError("Shared source/object or full ELF identity changed in native proof")
    shared_rows = [item for item in functions if item.get("origin") == "boot-shared"]
    if any(item.get("candidate_source") != "candidates/boot.c" for item in shared_rows):
        raise ValueError("Shared functions must retain their boot C provenance")
    shared_results = validate_level_proof({**shared, "functions": shared_rows}, target, overlays, progress, integration,
                                          catalog_bytes, boot_catalog, (count, total))
    expected = {item["symbol"]: item for item in catalog["functions"]}
    native_results = [item for item in functions if item.get("origin") == "level-native"]
    if not isinstance(native_results, list) or len(native_results) != len(expected):
        raise ValueError("Native integration is partial")
    checked = {item["symbol"]: item for item in qualified["functions"]}
    for result in native_results:
        item = expected.get(result.get("symbol"))
        object_result = checked.get(result.get("symbol"))
        if (item is None or result.get("address") != item["address"] or result.get("size") != item["size"]
                or result.get("program") != level or result.get("candidate_source") != source_path
                or result.get("origin") != "level-native" or result.get("integrated") is not True
                or result.get("matched") is not True or result.get("different_bytes") != 0
                or result.get("reference_sha256") != object_result["reference_sha256"]
                or result.get("candidate_sha256") != object_result["candidate_sha256"]):
            raise ValueError("Native integration requires the complete reviewed object bodies")
    union = shared_results + native_results
    ranges(union)
    by_symbol = {item["symbol"]: item for item in union}
    if (len(by_symbol) != len(union) or len(functions) != len(union)
            or any(by_symbol.get(item["symbol"]) != item for item in functions)
            or proof.get("matched_code_bytes") != total or proof.get("target") != target["serial"]
            or proof.get("reference_sha256") != catalog["reference_sha256"]
            or proof.get("state") != "integrated" or proof.get("full_level_gate") != shared.get("full_level_gate")):
        raise ValueError("Native/shared union, identity, full gate or derived counts mismatch")
    return functions


def generate(scope: dict, target: dict, overlays: dict, progress: dict,
             integration: dict | None = None, levels: list[dict] | None = None,
             candidate_review: Path | None = None) -> dict:
    if scope["target"] != target["serial"] or overlays["target"] != target["serial"]:
        raise ValueError("Progress scope belongs to another target")
    for field in ("decompiled_functions", "integrated_functions"):
        count = progress.get(field, 0)
        if type(count) is not int or count < 0:
            raise ValueError("Invalid progress function count")
        if count and integration is None:
            raise ValueError("C/C++ progress requires a valid integration proof")
    expected = {"boot": target["boot"]["sha256"],
                **{"levels/" + entry["level"]: entry["sha256"] for entry in overlays["levels"]}}
    programs = scope["programs"]
    actual = {program["name"]: program["sha256"] for program in programs}
    if len(programs) != 28 or len(actual) != len(programs) or actual != expected:
        raise ValueError("Progress scope must cover the pinned boot and all 27 overlays")
    functions = [] if integration is None else validate_integration(integration, target, progress)
    shared_boot = integration
    if integration is not None and integration.get("kind") == "boot-c-owner-integration":
        # The union has already passed both owner validators. Overlay placements
        # reuse only the unchanged default object and its 187-function review.
        from boot_sdk_unit import owner_rows
        defaults, _ = owner_rows(integration["functions"])
        shared_boot = dict(integration["default"], functions=defaults)
    level_proofs = [] if levels is None else levels
    if not isinstance(level_proofs, list):
        raise ValueError("Invalid level integration proofs")
    if level_proofs and integration is None:
        raise ValueError("Level C progress requires the boot integration proof")
    if level_proofs:
        catalog_bytes = (ROOT / "config" / "level-catalog.json").read_bytes()
        boot_catalog = json.loads((ROOT / "config" / "candidate-catalog.json").read_bytes())
        seen_programs = set()
        for proof in level_proofs:
            program = proof.get("program") if isinstance(proof, dict) else None
            if not isinstance(program, str) or program in seen_programs:
                raise ValueError("Duplicate or invalid level integration proof")
            seen_programs.add(program)
            if proof.get("kind") == "level-c-integration":
                functions.extend(validate_native_level_proof(proof, target, overlays, progress, shared_boot,
                                                             catalog_bytes, boot_catalog, candidate_review))
            else:
                functions.extend(validate_level_proof(proof, target, overlays, progress, shared_boot, catalog_bytes, boot_catalog))
    owners = {}
    promoted_by_section = {}
    for program in programs:
        name = program["name"]
        key = name.removeprefix("levels/")
        program_functions = [function for function in functions if function["program"] == key]
        occupied = []
        for section in program["sections"]:
            size, address = section["size"], section["address"]
            if (type(size) is not int or size <= 0 or type(address) is not int or address < 0
                    or type(section["flags"]) is not int or not section["flags"] & 2
                    or section["type"] != 1):
                raise ValueError("Progress scope requires positive, allocated PROGBITS sections")
            if any(address < ending and starting < address + size for starting, ending in occupied):
                raise ValueError("Overlapping progress sections")
            occupied.append((address, address + size))
            if section["flags"] & 4 and program_functions:
                for function in program_functions:
                    if address <= function["address"] and function["address"] + function["size"] <= address + size:
                        owners[(name, function["symbol"])] = section
                        promoted_by_section.setdefault(id(section), []).append(function)
    if len(owners) != len(functions):
        missing = next(function for function in functions
                       if (function["program"] if function["program"] == "boot"
                           else "levels/" + function["program"], function["symbol"]) not in owners)
        raise ValueError(f"Integrated function is outside an executable {missing['program']} section")
    units = []
    total_code = 0
    total_data = 0
    seen_names = set()
    for program in programs:
        for section in program["sections"]:
            size = section["size"]
            name = f"{program['name']}/{section['name']}@{section['address']:08X}"
            if name in seen_names:
                raise ValueError("Duplicate progress unit")
            seen_names.add(name)
            is_code = bool(section["flags"] & 4)
            code_size = size if is_code else 0
            data_size = 0 if is_code else size
            total_code += code_size
            total_data += data_size
            category = "boot" if program["name"] == "boot" else "levels"
            promoted = promoted_by_section.get(id(section), [])
            remaining = size - sum(function["size"] for function in promoted)
            for function in promoted:
                candidate_source = function.get("candidate_source", "candidates/boot.c")
                units.append({"name": f"{program['name']}/{candidate_source}/{function['symbol']}",
                              "measures": measures(function["size"], 0, 1, function["size"], 1),
                              "sections": [{"name": section["name"], "size": str(function["size"]),
                                            "fuzzyMatchPercent": 100,
                                            "metadata": {"virtualAddress": str(function["address"])}}],
                              "functions": [{"name": function["symbol"], "size": str(function["size"]),
                                             "fuzzyMatchPercent": 100,
                                             "metadata": {"virtualAddress": str(function["address"])}}],
                              "metadata": {"complete": True, "autoGenerated": False,
                                           "sourcePath": candidate_source, "moduleName": program["name"],
                                           "progressCategories": [category]}})
            if not remaining:
                continue
            code_size = remaining if is_code else 0
            data_size = 0 if is_code else remaining
            units.append({"name": name, "measures": measures(code_size, data_size, 1),
                          "sections": [{"name": section["name"], "size": str(remaining),
                                        "fuzzyMatchPercent": 0,
                                        "metadata": {"virtualAddress": str(section["address"])}}],
                          "metadata": {"complete": False, "autoGenerated": True,
                                       "moduleName": program["name"], "progressCategories": [category]}})
    if not total_code:
        raise ValueError("Empty executable progress scope")
    categories = []
    for category, title in (("boot", "Boot ELF"), ("levels", "27 level overlays")):
        subset = [unit for unit in units if category in unit["metadata"]["progressCategories"]]
        categories.append({"id": category, "name": title,
                           "measures": measures(sum(int(unit["measures"]["totalCode"]) for unit in subset),
                                                sum(int(unit["measures"]["totalData"]) for unit in subset),
                                                len(subset),
                                                sum(int(unit["measures"]["completeCode"]) for unit in subset),
                                                sum(unit["measures"]["completeUnits"] for unit in subset))})
    return {"version": 2, "measures": measures(total_code, total_data, len(units),
                                               sum(function["size"] for function in functions), len(functions)),
            "units": units, "categories": categories}


def main() -> int:
    parser = argparse.ArgumentParser(description="Export RAC2 C/C++ progress with integration evidence")
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--level-proof", type=Path, action="append", default=[], metavar="PATH",
                        help="reviewed C integration proof for one level overlay (repeatable)")
    parser.add_argument("--candidate-review", type=Path, help="explicit fresh boot object review, preserving the prior review")
    parser.add_argument("--integration-proof", type=Path, help="explicit boot integration proof")
    parser.add_argument("--progress-proof", type=Path, help="explicit verified runtime gates")
    args = parser.parse_args()
    def read(relative: str) -> dict:
        return json.loads((ROOT / relative).read_text(encoding="utf-8"))
    integration = (json.loads(args.integration_proof.read_bytes()) if args.integration_proof else
                   read("progress/integration.json") if (ROOT / "progress/integration.json").exists() else None)
    levels = [json.loads(path.read_text(encoding="utf-8")) for path in args.level_proof]
    if levels and integration is None:
        raise ValueError("Level integration proofs require the boot integration proof")
    progress = json.loads(args.progress_proof.read_bytes()) if args.progress_proof else read("progress/report.json")
    report = generate(read("config/progress-scope.json"), read("config/target.json"),
                      read("config/overlays.json"), progress, integration, levels, args.candidate_review)
    if integration is not None:
        validate_object_proof(integration, json.loads(args.candidate_review.read_bytes()) if args.candidate_review
                              else read("progress/candidates.json"))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"output": str(args.output), "units": len(report["units"]),
                      "total_code": report["measures"]["totalCode"],
                      "matched_code": report["measures"]["matchedCode"]}))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print(f"Report export failed: {error}")
        raise SystemExit(2)
