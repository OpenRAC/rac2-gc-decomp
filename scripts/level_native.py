"""Independent C reviews for one pinned overlay, never a replacement boot proof."""
from __future__ import annotations

import hashlib
import json
import re
from pathlib import Path

from check_candidates import compare_function, file_hash, linker_script, run
from elf_tools import assert_fresh, read_elf
from wsl_chain import compile_c, tool_hashes

ROOT = Path(__file__).resolve().parents[1]
HASH = re.compile(r"[0-9a-f]{64}")
LEVEL = re.compile(r"[0-9]+_[a-z0-9_]+")
C_ONLY = re.compile(rb"\b(?:asm|__asm__|__asm|INCLUDE_ASM)\b|\.byte|\.word")


def digest(value: object) -> str:
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def require_hash(value: object) -> None:
    if not isinstance(value, str) or not HASH.fullmatch(value):
        raise ValueError("Invalid native proof hash")


def paths(level: str) -> tuple[str, str, str]:
    if not isinstance(level, str) or not LEVEL.fullmatch(level):
        raise ValueError("Invalid native level identifier")
    return (f"candidates/levels/{level}.c", f"config/level-native/{level}.json",
            f"progress/level-candidates/{level}.json")


def symbol(level: str, address: int) -> str:
    paths(level)
    return f"LVL_{level.upper()}_FUN_{address:08X}"


def ranges(functions: list[dict]) -> None:
    if any(not isinstance(item, dict) or type(item.get("address")) is not int
           or type(item.get("size")) is not int for item in functions):
        raise ValueError("Invalid native integration boundary types")
    seen = set()
    ordered = sorted(functions, key=lambda item: item["address"])
    previous_end = -1
    for item in ordered:
        name, address, size = item.get("symbol"), item.get("address"), item.get("size")
        if (not isinstance(name, str) or name in seen or type(address) is not int
                or type(size) is not int or address < 0 or address % 4
                or size <= 0 or size % 4 or address < previous_end):
            raise ValueError("Invalid, duplicate or overlapping native integration body")
        seen.add(name)
        previous_end = address + size


def checker_hash(root: Path) -> str:
    return hashlib.sha256(b"".join((root / "scripts" / name).read_bytes() for name in
                                  ("level_native.py", "check_candidates.py", "elf_tools.py", "wsl_chain.py"))).hexdigest()


def load_catalog(level: str, root: Path = ROOT) -> dict:
    source_path, catalog_path, _ = paths(level)
    catalog_bytes = (root / catalog_path).read_bytes()
    catalog = json.loads(catalog_bytes)
    target = json.loads((root / "config/target.json").read_bytes())
    overlays = json.loads((root / "config/overlays.json").read_bytes())
    boot = json.loads((root / "config/candidate-catalog.json").read_bytes())
    pinned = {entry["level"]: entry["sha256"] for entry in overlays["levels"]}
    if (catalog.get("schema") != 1 or catalog.get("kind") != "level-native-catalog"
            or catalog.get("target") != target["serial"] or overlays["target"] != target["serial"]
            or catalog.get("level") != level or catalog.get("program") != "levels/" + level
            or catalog.get("reference_sha256") != pinned.get(level)
            or catalog.get("source") != source_path or catalog.get("flags") != boot["flags"]
            or type(catalog.get("entry")) is not int or catalog["entry"] < 0 or catalog["entry"] % 4):
        raise ValueError("Native catalog programme, source, entry or pinned identity mismatch")
    require_hash(catalog["reference_sha256"])
    functions = catalog.get("functions")
    if not isinstance(functions, list) or not functions or any(not isinstance(item, dict) for item in functions):
        raise ValueError("Native catalog requires complete functions")
    ranges(functions)
    for item in functions:
        if item["symbol"] != symbol(level, item["address"]):
            raise ValueError("Native symbol must identify its own program and entry")
    externals = catalog.get("externals", {})
    if not isinstance(externals, dict):
        raise ValueError("Invalid native externals")
    for name, address in externals.items():
        if (not isinstance(name, str) or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", name)
                or type(address) is not int or address < 0 or address > 0xFFFFFFFF or address % 4
                or name in {item["symbol"] for item in functions}):
            raise ValueError("Invalid native external identity")
    gp = catalog.get("gp", 0)
    if type(gp) is not int or gp < 0 or gp > 0xFFFFFFFF or gp % 4:
        raise ValueError("Invalid reviewed native gp")
    source = (root / source_path).read_bytes()
    if C_ONLY.search(source):
        raise ValueError("Native candidates must be genuine C")
    if re.search(rb"(?m)^\s*#\s*include\b|\b__(?:DATE|TIME|TIMESTAMP)__\b", source):
        raise ValueError("Native source must be standalone and reproducible until header dependencies are pinned")
    return {**catalog, "externals": externals, "gp": gp,
            "_catalog_sha256": hashlib.sha256(catalog_bytes).hexdigest(),
            "_source_sha256": hashlib.sha256(source).hexdigest()}


def validate_reference(reference: Path, catalog: dict) -> dict:
    if file_hash(reference) != catalog["reference_sha256"]:
        raise ValueError("Native reference ELF is not the pinned overlay")
    structure = read_elf(reference)
    if structure["type"] != 2 or structure["entry"] != catalog["entry"]:
        raise ValueError("Native reference entry is not the reviewed program entry")
    for function in catalog["functions"]:
        owners = [section for section in structure["sections"] if section["type"] == 1
                  and section["flags"] & 6 == 6 and section["address"] <= function["address"]
                  and function["address"] + function["size"] <= section["address"] + section["size"]]
        if len(owners) != 1:
            raise ValueError("Native function is outside one executable reference section")
    return structure


def validate_review(review: dict, catalog: dict, level: str, root: Path = ROOT) -> None:
    source_path, catalog_path, _ = paths(level)
    expected = {"kind": "level-native-candidate", "schema": 1, "target": catalog["target"],
                "program": "levels/" + level, "reference_sha256": catalog["reference_sha256"],
                "reference_entry": catalog["entry"], "candidate_source": source_path,
                "source_sha256": file_hash(root / source_path), "catalog_sha256": file_hash(root / catalog_path),
                "checker_sha256": checker_hash(root), "flags": catalog["flags"], "state": "matched_unintegrated"}
    if not isinstance(review, dict) or any(review.get(key) != value for key, value in expected.items()):
        raise ValueError("Native review identity, source, catalog or checker mismatch")
    for key in ("source_sha256", "catalog_sha256", "checker_sha256", "object_sha256", "candidate_elf_sha256"):
        require_hash(review.get(key))
    tools = review.get("tools")
    if not isinstance(tools, dict) or set(tools) != {"cc1", "cpp", "as", "ld.exe"}:
        raise ValueError("Native review requires all compiler instruments")
    for value in tools.values():
        require_hash(value)
    expected_functions = {item["symbol"]: (item["address"], item["size"]) for item in catalog["functions"]}
    results = review.get("functions")
    if not isinstance(results, list) or len(results) != len(expected_functions):
        raise ValueError("Native review must cover every complete catalogued body")
    seen = set()
    for result in results:
        if (not isinstance(result, dict) or result.get("symbol") in seen
                or expected_functions.get(result.get("symbol")) != (result.get("address"), result.get("size"))
                or any(type(result.get(key)) is not int for key in ("address", "size", "produced_size", "different_bytes"))
                or result.get("produced_size") != result.get("size") or result.get("matched") is not True
                or result.get("different_bytes") != 0):
            raise ValueError("Native review requires complete exact symbols")
        seen.add(result["symbol"])
        require_hash(result.get("reference_sha256")); require_hash(result.get("candidate_sha256"))
        if result["reference_sha256"] != result["candidate_sha256"]:
            raise ValueError("Native body hashes differ")
    if seen != set(expected_functions):
        raise ValueError("Native review function set mismatch")
    if review.get("matched_code_bytes") != sum(item["size"] for item in results):
        raise ValueError("Native review byte count must be derived")


def qualify(reference: Path, directory: Path, toolchain: Path, level: str, root: Path = ROOT) -> tuple[dict, Path, dict]:
    from check_candidates import function_symbols
    catalog = load_catalog(level, root)
    validate_reference(reference, catalog)
    source_path, catalog_path, _ = paths(level)
    directory.mkdir(parents=True, exist_ok=True)
    snapshot = directory / Path(source_path).name
    snapshot.write_bytes((root / source_path).read_bytes())
    if file_hash(snapshot) != catalog["_source_sha256"]:
        raise ValueError("Native source changed while creating its snapshot")
    object_path = directory / (snapshot.name + ".o")
    before = tool_hashes(toolchain)
    compile_c(snapshot, catalog["flags"], object_path, directory / (snapshot.stem + ".s"), directory / "compile.log")
    assert_fresh(object_path, [snapshot])
    script = directory / "qualification.ld"
    script.write_text(linker_script(catalog).replace("/DISCARD/ : {", "/DISCARD/ : { *(.text.*)"), encoding="ascii")
    qualified = directory / "qualification.elf"
    run([str(toolchain / "ee/bin/ld.exe"), "-T", str(script), "-o", str(qualified), str(object_path)], directory / "link.log")
    assert_fresh(qualified, [object_path, snapshot, script])
    results = []
    symbols = function_symbols(qualified)
    for item in catalog["functions"]:
        result = compare_function(reference, qualified, item["symbol"], item["address"], item["size"])
        result["produced_size"] = symbols.get(item["symbol"], {}).get("size")
        results.append(result)
    after = tool_hashes(toolchain)
    if before != after:
        raise ValueError("Native instruments changed during qualification")
    proof = {"schema": 1, "kind": "level-native-candidate", "target": catalog["target"],
             "program": "levels/" + level, "reference_sha256": file_hash(reference), "reference_entry": catalog["entry"],
             "candidate_source": source_path, "source_sha256": file_hash(snapshot), "catalog_sha256": catalog["_catalog_sha256"],
             "object_sha256": file_hash(object_path), "candidate_elf_sha256": file_hash(qualified),
             "checker_sha256": checker_hash(root), "tools": after, "flags": catalog["flags"],
             "state": "matched_unintegrated", "functions": results,
             "matched_code_bytes": sum(item["size"] for item in results if item.get("matched"))}
    (directory / "object-qualification.json").write_text(json.dumps(proof, indent=2) + "\n", encoding="utf-8")
    validate_review(proof, catalog, level, root)
    return catalog, object_path, proof


def compile_reviewed(reference: Path, directory: Path, toolchain: Path, level: str, root: Path = ROOT) -> tuple[dict, Path, dict]:
    catalog = load_catalog(level, root)
    _, _, review_path = paths(level)
    review = json.loads((root / review_path).read_bytes())
    validate_review(review, catalog, level, root)
    if tool_hashes(toolchain) != review["tools"]:
        raise ValueError("Native integration tools differ from the reviewed object")
    compiled, object_path, proof = qualify(reference, directory, toolchain, level, root)
    validate_review(review, compiled, level, root)
    if file_hash(object_path) != review["object_sha256"]:
        raise ValueError("Native source/object pair differs from the reviewed object")
    return compiled, object_path, proof


def dependencies(level: str, root: Path = ROOT, boot_review: Path | None = None) -> str:
    """Only this overlay's placements and native files; no 27-level native index."""
    source_path, catalog_path, review_path = paths(level)
    placements = json.loads((root / "config/level-catalog.json").read_bytes())["levels"][level]
    native = {path: file_hash(root / path) if (root / path).exists() else None
              for path in (source_path, catalog_path, review_path)}
    return digest({"program": level, "shared_placement": placements, "native": native,
                   "boot_source": file_hash(root / "candidates/boot.c"),
                   "boot_review": file_hash(boot_review or root / "progress/candidates.json"), "checker": checker_hash(root)})
