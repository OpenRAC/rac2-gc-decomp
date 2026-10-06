"""Compile and link reviewed RAC2 C candidates, then compare complete symbols."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
import subprocess
import uuid
from datetime import datetime, timezone
from pathlib import Path

from elf_tools import address_bytes, assert_fresh, read_elf
from wsl_chain import compile_c, tool_hashes


ROOT = Path(__file__).resolve().parents[1]


def file_hash(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def function_symbols(path: Path) -> dict:
    structure = read_elf(path)
    if structure["type"] != 2:
        raise ValueError("Candidate must be a linked ET_EXEC ELF")
    data = path.read_bytes()
    section_offset = struct.unpack_from("<I", data, 32)[0]
    section_stride = struct.unpack_from("<H", data, 46)[0]
    headers = [struct.unpack_from("<10I", data, section_offset + index * section_stride)
               for index in range(len(structure["sections"]))]
    symbols = {}
    for header in headers:
        if header[1] != 2:
            continue
        link = header[6]
        if link >= len(headers) or headers[link][1] != 3 or header[9] != 16 or header[5] % 16:
            raise ValueError("Invalid ELF symbol table")
        strings_header = headers[link]
        strings = data[strings_header[4]:strings_header[4] + strings_header[5]]
        for position in range(header[4], header[4] + header[5], 16):
            name_offset, address, size, info, visibility, section_index = struct.unpack_from("<IIIBBH", data, position)
            if info & 15 != 2 or info >> 4 != 1 or not section_index:
                continue
            if section_index >= len(structure["sections"]):
                continue
            section = structure["sections"][section_index]
            if section["type"] != 1 or section["flags"] & 6 != 6:
                continue
            if address < section["address"] or address + size > section["address"] + section["size"]:
                raise ValueError("Function symbol outside its executable section")
            ending = strings.find(b"\0", name_offset)
            if name_offset >= len(strings) or ending < 0:
                raise ValueError("Invalid symbol string")
            name = strings[name_offset:ending].decode("ascii")
            if name in symbols:
                raise ValueError("Duplicate function symbol")
            symbols[name] = {"address": address, "size": size}
    return symbols


def compare_function(reference: Path, candidate: Path, symbol: str, address: int, size: int) -> dict:
    symbols = function_symbols(candidate)
    if symbol not in symbols or symbols[symbol]["address"] != address:
        return {"symbol": symbol, "matched": False, "reason": "missing or misplaced defined function"}
    if size <= 0 or symbols[symbol]["size"] != size:
        return {"symbol": symbol, "matched": False, "reason": "complete symbol size mismatch"}
    original = address_bytes(reference, address, size)
    produced = address_bytes(candidate, address, size)
    return {"symbol": symbol, "address": address, "size": size, "matched": original == produced,
            "reference_sha256": hashlib.sha256(original).hexdigest(),
            "candidate_sha256": hashlib.sha256(produced).hexdigest(),
            "different_bytes": sum(before != after for before, after in zip(original, produced)),
            "state": "matched_unintegrated" if original == produced else "mismatch"}


def run(arguments: list[str], log: Path, directory: Path | None = None) -> None:
    with log.open("wb") as stream:
        try:
            result = subprocess.run(arguments, cwd=directory, stdout=stream,
                                    stderr=subprocess.STDOUT, timeout=120)
        except subprocess.TimeoutExpired as error:
            raise ValueError(f"Tool timed out; see {log}") from error
    if result.returncode:
        raise ValueError(f"Tool failed ({result.returncode}); see {log}")


def readonly_sections(catalog: dict) -> list[dict]:
    """Review one whole compiler-generated table, never a guessed subsection."""
    placements = catalog.get("read_only_sections", [])
    if not isinstance(placements, list) or len(placements) > 1:
        raise ValueError("Only one whole generated readonly section is supported")
    for item in placements:
        if (not isinstance(item, dict) or set(item) != {"section", "address", "size", "sha256"}
                or item["section"] != ".rodata"
                or type(item["address"]) is not int or item["address"] < 0 or item["address"] % 4
                or type(item["size"]) is not int or item["size"] <= 0 or item["size"] % 4
                or item["address"] + item["size"] > 1 << 32
                or not isinstance(item["sha256"], str) or not re.fullmatch(r"[0-9a-f]{64}", item["sha256"])):
            raise ValueError("Invalid whole generated readonly section placement")
        for function in catalog["functions"]:
            if function["address"] < item["address"] + item["size"] and item["address"] < function["address"] + function["size"]:
                raise ValueError("Generated readonly section overlaps a complete function")
    return placements


def compare_readonly(reference: Path, candidate: Path, catalog: dict, obj: Path) -> list[dict]:
    placements = readonly_sections(catalog)
    if not placements:
        return []
    object_elf, linked_elf, reference_elf = (read_elf(path) for path in (obj, candidate, reference))
    if object_elf["type"] != 1 or linked_elf["type"] != 2 or reference_elf["type"] != 2:
        raise ValueError("Generated readonly data needs an unchanged object and linked reference/candidate")
    generated = [section for section in object_elf["sections"]
                 if section["size"] and section["type"] in (1, 8)
                 and section["flags"] & 2 and not section["flags"] & 4]
    results = []
    for item in placements:
        result = {"section": item["section"], "address": item["address"], "size": item["size"], "matched": False}
        owners = [section for section in reference_elf["sections"] if section["type"] == 1
                  and section["address"] <= item["address"] and item["address"] + item["size"] <= section["address"] + section["size"]]
        if len(owners) != 1:
            raise ValueError("Generated readonly reference extent lacks one mapped PROGBITS owner")
        original = address_bytes(reference, item["address"], item["size"])
        reference_hash = hashlib.sha256(original).hexdigest()
        if reference_hash != item["sha256"]:
            raise ValueError("Generated readonly reference pin differs")
        inputs = [section for section in generated if section["name"] == item["section"]]
        outputs = [section for section in linked_elf["sections"] if section["name"] == item["section"]]
        if (len(generated) != 1 or len(inputs) != 1 or len(outputs) != 1
                or inputs[0]["type"] != 1 or inputs[0]["flags"] != 2 or inputs[0]["size"] != item["size"]
                or outputs[0]["type"] != 1 or outputs[0]["flags"] != 2
                or outputs[0]["address"] != item["address"] or outputs[0]["size"] != item["size"]
                or item["address"] % max(1, inputs[0]["alignment"])):
            result["reason"] = "missing, extra, writable, misplaced or incomplete generated readonly section"
        else:
            produced = address_bytes(candidate, item["address"], item["size"])
            result.update(reference_sha256=reference_hash, candidate_sha256=hashlib.sha256(produced).hexdigest(),
                          different_bytes=sum(before != after for before, after in zip(original, produced)),
                          matched=original == produced)
        results.append(result)
    return results


def require_exact_readonly(catalog: dict, results: list[dict]) -> None:
    placements = readonly_sections(catalog)
    if not isinstance(results, list) or len(results) != len(placements):
        raise ValueError("Review omitted or duplicated generated readonly data")
    for item, result in zip(placements, results):
        if (not isinstance(result, dict) or any(result.get(key) != item[key] for key in ("section", "address", "size"))
                or result.get("matched") is not True or type(result.get("different_bytes")) is not int
                or result["different_bytes"] != 0 or result.get("reference_sha256") != item["sha256"]
                or result.get("candidate_sha256") != item["sha256"]):
            raise ValueError("Review requires every generated readonly byte to match")


def linker_script(catalog: dict) -> str:
    functions = sorted(catalog["functions"], key=lambda function: function["address"])
    sections = "".join(f"    .text.{function['symbol']} 0x{function['address']:08X} : "
                       f"{{ *(.text.{function['symbol']}) }}\n" for function in functions)
    definitions = "".join(f"{name} = 0x{address:08X};\n" for name, address in catalog["externals"].items())
    readonly = "".join(f"    {item['section']} 0x{item['address']:08X} : {{ *({item['section']}) }}\n"
                       for item in readonly_sections(catalog))
    return (f"ENTRY({functions[0]['symbol']})\nSECTIONS\n{{\n" + readonly + sections +
            "    .data : { *(.data) *(.rodata) *(.rdata) *(.lit4) *(.lit8) *(.sdata) }\n"
            "    .bss : { *(.bss) *(.sbss) *(COMMON) }\n"
            "    /DISCARD/ : { *(.reginfo) }\n}\n" + definitions +
            f"_gp = 0x{catalog['gp']:08X};\n")


def main() -> int:
    parser = argparse.ArgumentParser(description="Verify the reviewed initial RAC2 C candidate lot")
    parser.add_argument("--reference", required=True, type=Path)
    parser.add_argument("--toolchain", required=True, type=Path)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--source", type=Path, default=ROOT / "candidates" / "boot.c")
    args = parser.parse_args()
    catalog = json.loads((ROOT / "config" / "candidate-catalog.json").read_text(encoding="utf-8"))
    target = json.loads((ROOT / "config" / "target.json").read_text(encoding="utf-8"))
    reference = args.reference.resolve()
    if catalog["reference_sha256"] != target["boot"]["sha256"] or file_hash(reference) != target["boot"]["sha256"]:
        raise ValueError("Wrong RAC2 reference identity")
    runtime = args.runtime.resolve()
    if runtime == ROOT or ROOT in runtime.parents or runtime in ROOT.parents:
        raise ValueError("Keep private candidate builds outside sources")
    source = args.source.resolve()
    content = source.read_text(encoding="utf-8")
    if re.search(r"\b(?:asm|__asm__|__asm|INCLUDE_ASM)\b|(?m:^[ \t]*(?:(?:[A-Za-z_.$][A-Za-z0-9_.$]*|[0-9]+):[ \t]*)?\.(?:byte|word)\b(?:[ \t]+(?![ \t]*=)\S|[ \t]*$))", content):
        raise ValueError("C candidates must not embed assembly or retail bytes")
    work = runtime / "candidate-runs" / uuid.uuid4().hex[:8]
    work.mkdir(parents=True)
    linker = args.toolchain.resolve() / "ee" / "bin" / "ld.exe"
    if not linker.is_file():
        raise ValueError(f"Missing instrument {linker.name}")
    assembly = work / "candidate.s"
    object_path = work / "candidate.o"
    flags = catalog["flags"]
    # The compiler writes the source spelling into `.file`, so the same file
    # compiled as an absolute path and as a bare name produces two different
    # objects. The reconstructed chain compiles the source under its bare name
    # inside WSL (wsl_chain.py): the object identifies the file itself, not the
    # machine or the build directory, and two passes over one source yield one
    # hash.
    compile_c(source, flags, object_path, assembly, work / "compile.log")
    (work / "compiler-version.log").write_text(
        "\n".join(f"{name}: {hash_}" for name, hash_ in tool_hashes(args.toolchain).items()) + "\n",
        encoding="utf-8")
    assert_fresh(object_path, [source])
    functions = sorted(catalog["functions"], key=lambda function: function["address"])
    script = work / "candidate.ld"
    script.write_text(linker_script(catalog), encoding="ascii")
    candidate = work / "candidate.elf"
    run([str(linker), "-T", str(script), "-o", str(candidate), str(object_path)], work / "link.log")
    assert_fresh(candidate, [source, object_path, script])
    results = [compare_function(reference, candidate, function["symbol"], function["address"], function["size"])
               for function in functions]
    data_results = compare_readonly(reference, candidate, catalog, object_path)
    report = {"target": catalog["target"], "verified_at": datetime.now(timezone.utc).isoformat(),
              "reference_sha256": file_hash(reference), "source_sha256": file_hash(source),
              "candidate_elf_sha256": file_hash(candidate), "object_sha256": file_hash(object_path),
              "catalog_sha256": file_hash(ROOT / "config" / "candidate-catalog.json"),
              "checker_sha256": file_hash(Path(__file__)),
              "flags": flags, "tools": tool_hashes(args.toolchain),
            "functions": results, "read_only_sections": data_results, "integrated_functions": 0,
              "profile_scope": "Only the fully matched functions below; not a general RAC2 compiler qualification"}
    (work / "report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"report": str(work / "report.json"), "results": results}, indent=2))
    return 0 if all(result["matched"] for result in results + data_results) else 1


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, struct.error) as error:
        print(f"Candidate preparation failed: {error}")
        raise SystemExit(2)
