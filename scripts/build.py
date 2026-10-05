from __future__ import annotations

import argparse
import hashlib
import importlib.metadata
import json
import re
import shutil
import subprocess
import sys
import uuid
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime, timezone
from pathlib import Path

from elf_tools import assert_fresh, compare_loads, read_elf
import region as regions

ROOT = Path(__file__).resolve().parents[1]
TARGET = json.loads((ROOT / "config" / "target.json").read_text(encoding="utf-8"))


def checked(arguments: list[str], directory: Path, log: Path) -> None:
    with log.open("wb") as stream:
        try:
            result = subprocess.run(arguments, cwd=directory, stdout=stream,
                                    stderr=subprocess.STDOUT, timeout=300)
        except subprocess.TimeoutExpired as error:
            raise ValueError(f"Command timed out; see {log}") from error
    if result.returncode:
        raise ValueError(f"Command failed ({result.returncode}); see {log}")


def safe_name(name: str) -> str:
    return name.lstrip(".").replace(".", "_").replace("-", "_")


def generate_config(reference: Path, directory: Path, name: str) -> dict:
    structure = read_elf(reference)
    loads = [segment for segment in structure["segments"] if segment["type"] == 1]
    if not loads:
        raise ValueError("No PT_LOAD segments")
    sections = [section for section in structure["sections"] if section["size"] and section["address"]
                and section["type"] != 8
                and any(load["offset"] <= section["offset"]
                        and section["offset"] + section["size"] <= load["offset"] + load["filesz"]
                        and section["address"] == load["address"] + section["offset"] - load["offset"]
                        for load in loads)]
    sections.sort(key=lambda section: section["offset"])
    if not sections or any(not any(section["offset"] == load["offset"] for section in sections) for load in loads):
        raise ValueError("PT_LOAD beginning is not covered by sections")
    target = directory / "assets" / f"{name}.elf"
    target.parent.mkdir()
    shutil.copy2(reference, target)
    config = directory / "config"
    config.mkdir()
    options = {"platform": "ps2", "base_path": "..", "basename": name,
               "target_path": f"assets/{name}.elf", "asm_path": "asm", "src_path": "src",
               "build_path": "build", "create_asm_dependencies": True,
               "ld_script_path": "config/rac2.ld", "symbol_addrs_path": "config/symbol_addrs.txt",
               "undefined_funcs_auto_path": "config/undefined_funcs_auto.txt",
               "undefined_syms_auto_path": "config/undefined_syms_auto.txt",
               "find_file_boundaries": False, "disassemble_all": True}
    segments = []
    twins = {}
    for section in sections:
        kind = "asm" if section["name"] in (".text", "core.text") else "data"
        section_name = safe_name(section["name"]) + f"_{section['offset']:X}"
        segment = {"name": section_name, "type": "code" if kind == "asm" else "data",
                   "start": section["offset"], "vram": section["address"], "align": 16,
                   "subsegments": [[section["offset"], kind]]}
        following_bss = [other for other in structure["sections"] if other["type"] == 8
                         and other["address"] == section["address"] + section["size"]]
        if following_bss:
            segment["bss_size"] = sum(other["size"] for other in following_bss)
        segments.append(segment)
        if kind == "asm":
            twins[f"build/asm/{section['offset']:X}.s.o"] = f"build/asm/{section_name}.s.o"
        else:
            twins[f"build/asm/data/{section['offset']:X}.data.s.o"] = f"build/asm/data/{section_name}.data.s.o"
    for load in loads:
        ending = load["offset"] + load["filesz"]
        if ending < max(other["offset"] + other["filesz"] for other in loads):
            segments.append({"name": f"unloaded_gap_{ending:X}", "type": "bin", "start": ending})
    segments.sort(key=lambda segment: segment["start"])
    segments.append([max(load["offset"] + load["filesz"] for load in loads)])
    document = {"name": name, "sha1": hashlib.sha1(reference.read_bytes()).hexdigest(),
                "options": options, "segments": segments}
    (config / "splat.yaml").write_text(json.dumps(document, indent=2) + "\n", encoding="utf-8")
    (config / "symbol_addrs.txt").write_text("", encoding="ascii")
    return twins


def adapt_linker(directory: Path, twins: dict) -> None:
    script = directory / "config" / "rac2.ld"
    content = script.read_text(encoding="utf-8")
    content = re.sub(r"^\s*(unloaded_gap_\w+)_ROM_START = .*?\n\s*\1_VRAM_END = \.;\n",
                     "\n", content, flags=re.MULTILINE | re.DOTALL)
    content = re.sub(r"ALIGN\(\s*\.\s*,\s*(\d+)\s*\)", r"ALIGN(\1)", content)
    content = re.sub(r"ALIGN\(\s*([^,()]+?)\s*,\s*(\d+)\s*\)",
                     r"((\1 + \2 - 1) & ~(\2 - 1))", content)
    content = re.sub(r"\n[ \t]*\. = ALIGN\(\d+\);(?=\n[ \t]*\w+_END = \.;)", "", content)
    content = re.sub(r"HIDDEN\(\s*(\w+)\s*=\s*([^)]+)\);", r"\1 = \2;", content)
    content = re.sub(r"\s*SUBALIGN\(\s*\d+\s*\)", "", content)
    addresses = dict(re.findall(r"^\s*(\.\S+)\s+(0x[0-9A-Fa-f]+)\s*:", content, re.MULTILINE))
    content = re.sub(r"=\s*ADDR\(\s*(\.\S+)\s*\)\s*;",
                     lambda match: f"= {addresses.get(match.group(1), '0')};", content)
    for offset_name, named in twins.items():
        source = directory / named.replace("build/asm/", "asm_pp/").removesuffix(".o")
        if source.is_file():
            content = content.replace(offset_name, named)
    reference = next((directory / "assets").glob("*.elf"))
    for name, tail in data_tails(reference).items():
        marker = f"{name}_DATA_END = .;"
        if content.count(marker) != 1:
            raise ValueError("Cannot locate unique linker data-tail insertion point")
        insertion = "\n        ".join(f"BYTE(0x{value:02X});" for value in tail)
        content = content.replace(marker, insertion + "\n        " + marker)
    loads = [segment for segment in read_elf(reference)["segments"] if segment["type"] == 1]
    header = "PHDRS\n{\n" + "".join(f"  load{index} PT_LOAD FLAGS({load['flags']});\n"
                                      for index, load in enumerate(loads)) + "}\n"
    section_pattern = re.compile(r"(\.[^\s]+\s+(0x[0-9A-Fa-f]+)\s*:[^{]*\{.*?\n\s*})([^\n]*)", re.DOTALL)
    def assign_segment(match: re.Match) -> str:
        address = int(match.group(2), 16)
        matching = [index for index, load in enumerate(loads)
                    if load["address"] <= address < load["address"] + load["memsz"]]
        if len(matching) != 1:
            raise ValueError(f"Output section outside unique PT_LOAD: 0x{address:X}")
        return match.group(1) + f" :load{matching[0]}" + match.group(3)
    content = section_pattern.sub(assign_segment, content)
    script.write_text("INCLUDE config/undefined_symbols.ld\n" + header + content, encoding="ascii")


def assemble_source(source: Path, directory: Path, assembler: Path) -> Path:
    relative = source.relative_to(directory / "asm_pp")
    output = directory / "build" / "asm" / relative.parent / (relative.name + ".o")
    output.parent.mkdir(parents=True, exist_ok=True)
    checked([str(assembler), "-o", str(output), str(source)], directory,
            output.with_suffix(".log"))
    if not output.is_file():
        raise ValueError("Assembler returned success without object")
    return output


def data_tails(reference: Path) -> dict:
    structure = read_elf(reference)
    content = reference.read_bytes()
    tails = {}
    for load in (segment for segment in structure["segments"] if segment["type"] == 1):
        ending = load["offset"] + load["filesz"]
        remainder = ending % 4
        if not remainder:
            continue
        sections = [section for section in structure["sections"] if section["type"] != 8
                    and section["offset"] + section["size"] == ending]
        if len(sections) != 1 or sections[0]["name"] in (".text", "core.text"):
            raise ValueError("Unaligned load tail must be a uniquely identified data section")
        section = sections[0]
        name = safe_name(section["name"]) + f"_{section['offset']:X}"
        tails[name] = content[ending - remainder:ending]
    return tails


def resolved_symbols(directory: Path) -> None:
    definitions = {}
    for filename in ("undefined_syms_auto.txt", "undefined_funcs_auto.txt"):
        path = directory / "config" / filename
        if not path.is_file():
            continue
        for line in path.read_text(encoding="utf-8").splitlines():
            line = line.split("//")[0].strip()
            if "=" in line:
                symbol, value = line.rstrip(";").split("=", 1)
                definitions[symbol.strip()] = value.strip()
    references = set()
    labels = set()
    for source in (directory / "asm_pp").rglob("*.s"):
        content = source.read_text(encoding="utf-8")
        references.update(re.findall(r"\bD_[0-9A-F]{5,8}\b", content))
        labels.update(re.findall(r"^\s*(\w+):", content, re.MULTILINE))
    for symbol in references - labels:
        definitions[symbol] = "0x" + symbol[2:]
    (directory / "config" / "undefined_symbols.ld").write_text(
        "".join(f"{symbol} = {value};\n" for symbol, value in sorted(definitions.items())), encoding="ascii")


def rebuild(reference: Path, expected_hash: str, directory: Path, toolchain: Path, jobs: int, name: str,
            c_toolchain: Path | None = None, level: str | None = None,
            candidate_review: Path | None = None) -> dict:
    if hashlib.sha256(reference.read_bytes()).hexdigest() != expected_hash:
        raise ValueError("Reference changed since verified extraction")
    directory.mkdir(parents=True)
    twins = generate_config(reference, directory, name)
    checked([sys.executable, "-m", "splat", "split", "config/splat.yaml"], directory, directory / "split.log")
    checked([sys.executable, str(ROOT / "scripts" / "expand_asm.py"),
             "--src", str(directory / "asm"), "--dst", str(directory / "asm_pp")],
            directory, directory / "expand.log")
    adapt_linker(directory, twins)
    sources = sorted((directory / "asm_pp").rglob("*.s"))
    excluded = {path.removeprefix("build/asm/").removesuffix(".o") for path in twins
                if (directory / twins[path].replace("build/asm/", "asm_pp/").removesuffix(".o")).is_file()}
    sources = [source for source in sources if source.relative_to(directory / "asm_pp").as_posix() not in excluded]
    if not sources:
        raise ValueError("No generated assembly")
    c_object = None
    if c_toolchain is not None:
        from integration import (compile_c, compile_level_c, replace_inputs, add_definitions,
                                 validate_integrated, c_objects)
        if level is None:
            if name != "boot":
                raise ValueError("C integration is qualified for the boot only")
            catalog, c_object, c_hashes = (compile_c(reference, directory, c_toolchain) if candidate_review is None
                                         else compile_c(reference, directory, c_toolchain, candidate_review))
        else:
            if name != "overlay":
                raise ValueError("Level C integration is qualified for one overlay at a time")
            catalog, c_object, c_hashes = (compile_level_c(reference, directory, c_toolchain, level) if candidate_review is None
                                         else compile_level_c(reference, directory, c_toolchain, level, candidate_review))
        sources, replacements = replace_inputs(directory, sources, catalog, c_object,
                                               relocated=level is not None)
    assembler = toolchain / "ee" / "bin" / "Ps2EeAs.exe"
    linker = toolchain / "ee" / "bin" / "ld.exe"
    reconstruction_hashes = ({path.name: hashlib.sha256(path.read_bytes()).hexdigest()
                             for path in (assembler, linker)} if c_object is not None and "native" in catalog else None)
    with ThreadPoolExecutor(max_workers=jobs) as pool:
        objects = list(pool.map(lambda source: assemble_source(source, directory, assembler), sources))
    if c_object is not None:
        objects.extend(c_objects(c_object))
    resolved_symbols(directory)
    if c_object is not None:
        add_definitions(directory, catalog)
    output = directory / "build" / f"{name}.elf"
    entry = read_elf(reference)["entry"]
    arguments = [str(linker), "-T", "config/rac2.ld", "-e", hex(entry),
                 "-Map", f"build/{name}.map", "-o", f"build/{name}.elf"]
    arguments.extend(object_path.relative_to(directory).as_posix() for object_path in objects)
    checked(arguments, directory, directory / "link.log")
    if reconstruction_hashes is not None and reconstruction_hashes != {
            path.name: hashlib.sha256(path.read_bytes()).hexdigest() for path in (assembler, linker)}:
        raise ValueError("Reconstruction instruments changed during native integration")
    assert_fresh(output, objects + [directory / "config" / "rac2.ld", directory / "config" / "undefined_symbols.ld"])
    result = compare_loads(reference, output)
    if not result["matched"]:
        raise ValueError(f"Byte gate failed: {result}")
    result.update({"name": name, "reference_sha256": expected_hash,
                   "candidate_sha256": hashlib.sha256(output.read_bytes()).hexdigest()})
    if c_object is not None:
        program = level or "boot"
        functions = validate_integrated(reference, output, catalog, ROOT / "candidates" / "boot.c",
                                        program=program)
        catalog_name = "level-catalog.json" if level else "candidate-catalog.json"
        gate_name = "full_level_gate" if level else "full_boot_gate"
        proof = {"target": TARGET["serial"], "program": program, "reference_sha256": expected_hash,
                 "source_sha256": hashlib.sha256((ROOT / "candidates" / "boot.c").read_bytes()).hexdigest(),
                 "catalog_sha256": hashlib.sha256((ROOT / "config" / catalog_name).read_bytes()).hexdigest(),
                 "candidate_source": "candidates/boot.c", "state": "integrated", "functions": functions,
                 gate_name: {"matched": True, "bytes_compared": result["bytes_compared"],
                             "segments": sum(segment["type"] == 1 for segment in read_elf(reference)["segments"])},
                 "matched_code_bytes": sum(function["size"] for function in functions), "tools": c_hashes,
                 "candidate_elf_sha256": result["candidate_sha256"],
                 "c_object_sha256": hashlib.sha256(c_objects(c_object)[0].read_bytes()).hexdigest(),
                 "replacement_inputs": replacements}
        if "native" in catalog:
            from level_native import dependencies, file_hash
            if read_elf(output)["entry"] != catalog["reference_entry"]:
                raise ValueError("Native final linked entry differs from the pinned overlay entry")
            if dependencies(level, ROOT, candidate_review) != catalog["dependency_sha256"]:
                raise ValueError("Native integration dependency changed during reconstruction")
            shared = {**proof, "functions": [item for item in functions if item["origin"] == "boot-shared"]}
            shared["matched_code_bytes"] = sum(item["size"] for item in shared["functions"])
            # Final function rows live once in the union. Reconstruct the shared
            # subset for its unchanged validator rather than copying it again.
            shared.pop("functions")
            native = catalog["native"]
            proof.update({"schema": 2, "kind": "level-c-integration", "shared": shared,
                          "reconstruction_tools": reconstruction_hashes,
                          "reference_entry": catalog["reference_entry"], "candidate_entry": read_elf(output)["entry"],
                          "dependency_sha256": catalog["dependency_sha256"],
                          "boot_review_sha256": catalog["boot_review_sha256"],
                          "native": {"source": native["source"], "catalog_path": native["catalog_path"],
                                     "review_path": native["review_path"], "review_sha256": native["review_sha256"],
                                     "object_qualification": native["object_proof"],
                                     "object_sha256": file_hash(c_object[native["source"]])}})
        (directory / "integration.json").write_text(json.dumps(proof, indent=2) + "\n", encoding="utf-8")
        result["integrated_c_functions"] = len(functions)
        result["integrated_c_bytes"] = proof["matched_code_bytes"]
    print(json.dumps(result), flush=True)
    (directory / "gate.json").write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description="RAC2 assembly reconstruction and exact byte gates")
    parser.add_argument("--manifest", required=True, type=Path)
    parser.add_argument("--toolchain", required=True, type=Path)
    parser.add_argument("--all-levels", action="store_true")
    parser.add_argument("--c-toolchain", type=Path, help="integrate reviewed C using the separately qualified SN compiler")
    parser.add_argument("--c-level", help="also integrate the reviewed C into this one level overlay")
    parser.add_argument("--c-all-levels", action="store_true", help="integrate shared and available native C in all overlays")
    parser.add_argument("--candidate-review", type=Path, help="explicit fresh boot object review; does not overwrite prior provenance")
    parser.add_argument("--jobs", type=int, default=8)
    regions.add_argument(parser)
    args = parser.parse_args()
    if args.jobs < 1:
        raise ValueError("jobs must be positive")
    if args.c_all_levels and (not args.c_toolchain or not args.all_levels):
        raise ValueError("All-level C integration requires --all-levels and --c-toolchain")
    if args.candidate_review and not args.c_toolchain:
        raise ValueError("An explicit candidate review requires C integration")
    versions = {"splat64": "0.50.0", "spimdisasm": "1.42.4", "rabbitizer": "1.16.2"}
    for package, version in versions.items():
        if importlib.metadata.version(package) != version:
            raise ValueError(f"Use pinned {package} {version}")
    manifest_path = args.manifest.resolve()
    if ROOT == manifest_path.parent or ROOT in manifest_path.parents:
        raise ValueError("Private manifest and builds must remain outside source repository")
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    try:
        region = regions.by_serial(manifest["target"])
    except ValueError as error:
        raise ValueError("Wrong manifest target") from error
    if args.region and regions.canonical(args.region) != region.name:
        raise ValueError(f"Manifest target {region.serial} belongs to region {region.name}, not {args.region}")
    if (args.c_toolchain or args.c_level or args.candidate_review) and not region.matching:
        region.require_matching("C integration")
    if region.pinned:
        if manifest["boot"]["sha256"] != region.target["boot"]["sha256"]:
            raise ValueError(f"Manifest boot identity is not the pinned {region.label} baseline")
        expected_overlays = region.overlay_pins()
        expected_count = region.expected_levels
    elif manifest.get("pinned") is False and manifest.get("region") == region.name:
        # An unpinned region: setup.py --measure-identity recorded these identities
        # privately. The byte gates still compare every loaded byte, but against an
        # unreviewed reference, so the report says so and no proof is produced.
        print(f"WARNING: {region.label} identities are unpinned; gates are a round trip only", flush=True)
        expected_overlays = None
        expected_count = len(manifest.get("level_wads", []))
    else:
        region.require_pinned("boot identity")
    actual_overlays = {entry["level"]: entry["sha256"] for entry in manifest["overlays"]}
    if any(not re.fullmatch(r"\d+_[a-z0-9_]+", level) for level in actual_overlays):
        raise ValueError("Invalid level identifier")
    if args.all_levels and (len(manifest["overlays"]) != len(actual_overlays)
                            or (expected_overlays is not None and actual_overlays != expected_overlays)):
        raise ValueError(f"Overlay identities do not match the pinned {region.label} baseline")
    toolchain = args.toolchain.resolve()
    instruments = {name: toolchain / "ee" / "bin" / name for name in ("Ps2EeAs.exe", "ld.exe")}
    for instrument in instruments.values():
        if not instrument.is_file():
            raise ValueError(f"Missing instrument: {instrument}")
    builds = args.manifest.resolve().parent / "builds" / uuid.uuid4().hex[:8]
    report = {"verified_at": datetime.now(timezone.utc).isoformat(), "target": manifest["target"],
              "packages": versions, "tools": {name: hashlib.sha256(path.read_bytes()).hexdigest()
                                             for name, path in instruments.items()},
              "g1": None, "g3": [], "decompiled_functions": 0, "compiler_flags": None}
    if not region.matching:
        report.update({"region": region.name, "pinned": region.pinned})
    report["g1"] = rebuild(Path(manifest["boot"]["path"]), manifest["boot"]["sha256"],
                           builds / "boot", toolchain, args.jobs, "boot",
                           args.c_toolchain.resolve() if args.c_toolchain else None, candidate_review=args.candidate_review)
    if args.c_toolchain:
        report["decompiled_functions"] = report["g1"]["integrated_c_functions"]
    c_level = None
    if args.c_level:
        if not args.c_toolchain:
            raise ValueError("Level C integration requires the separately qualified C toolchain")
        matches = [overlay for overlay in manifest["overlays"] if overlay["level"] == args.c_level]
        if len(matches) != 1:
            raise ValueError("Level C integration requires one overlay of the verified manifest")
        c_level = matches[0]
        result = rebuild(Path(c_level["path"]), c_level["sha256"], builds / c_level["level"],
                         toolchain, args.jobs, "overlay", args.c_toolchain.resolve(), level=c_level["level"],
                         candidate_review=args.candidate_review)
        result["level"] = c_level["level"]
        report["g3"].append(result)
    if args.all_levels:
        if not expected_count or len(manifest["overlays"]) != expected_count:
            raise ValueError(f"G3 requires all {expected_count} verified overlays")
        for overlay in manifest["overlays"]:
            if c_level is not None and overlay["level"] == c_level["level"]:
                continue
            result = rebuild(Path(overlay["path"]), overlay["sha256"], builds / overlay["level"],
                             toolchain, args.jobs, "overlay",
                             args.c_toolchain.resolve() if args.c_all_levels else None,
                             level=overlay["level"] if args.c_all_levels else None, candidate_review=args.candidate_review)
            result["level"] = overlay["level"]
            report["g3"].append(result)
    builds.mkdir(exist_ok=True, parents=True)
    (builds / "report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"Verified report: {builds / 'report.json'}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print(f"Build failed: {error}")
        raise SystemExit(2)
