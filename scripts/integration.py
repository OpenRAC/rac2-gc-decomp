"""Replace reviewed assembly bodies with genuine compiler-produced C sections."""

from __future__ import annotations

import json
import re
from datetime import datetime, timezone
from pathlib import Path

from check_candidates import compare_function, file_hash, run, linker_script
from elf_tools import assert_fresh
from wsl_chain import compile_c as compile_c_source, tool_hashes


ROOT = Path(__file__).resolve().parents[1]
INSTRUCTION = re.compile(r"/\*\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/")
# Branch targets inside a promoted body. They belong to the body and go away
# with it; without accepting them, only label-free leaves could be integrated.
INTERNAL_LABEL = re.compile(r"^\s*\.?L[0-9A-Fa-f]+:\s*$")
HEADER = '.set noat\n.set noreorder\n.section .text, "ax"\n'


def split_assembly(content: str, functions: list[dict], relocated: bool = False) -> list[dict]:
    lines = content.splitlines(keepends=True)
    selected = sorted(functions, key=lambda function: function["address"])
    if len({function["address"] for function in selected}) != len(selected):
        raise ValueError("Duplicate integration address")
    edits = []
    previous_end = 0
    for function in selected:
        address, size = function["address"], function["size"]
        if size <= 0 or size % 4 or address % 4 or address < previous_end:
            raise ValueError("Invalid or overlapping integration boundary")
        # The original assembly names its functions after their own address.
        # A boot catalog entry carries that same name as its C symbol; a level
        # placement keeps the reviewed C symbol and takes the address from the
        # level, so the name/address agreement is only required for the boot.
        if not relocated and function["symbol"] != f"FUN_{address:08X}":
            raise ValueError("Integration symbol does not identify its address")
        previous_end = address + size
        original_symbol = f"func_{address:08X}"
        starts = [index for index, line in enumerate(lines) if line.strip() == f".globl {original_symbol}"]
        if len(starts) != 1:
            raise ValueError(f"Expected one original assembly definition: {original_symbol}")
        start = starts[0]
        if start + 1 >= len(lines) or lines[start + 1].strip() != original_symbol + ":":
            raise ValueError("Assembly function label is not contiguous")
        observed = []
        end = start + 2
        while end < len(lines):
            match = INSTRUCTION.search(lines[end])
            if not match:
                if INTERNAL_LABEL.match(lines[end]):
                    end += 1
                    continue
                raise ValueError("Unexpected directive or label within promoted body")
            instruction_address = int(match.group(1), 16)
            expected_address = address + 4 * len(observed)
            if instruction_address != expected_address:
                raise ValueError("Disassembly addresses disagree with reviewed body")
            observed.append(instruction_address)
            end += 1
            if len(observed) * 4 == size:
                break
        if len(observed) * 4 != size:
            raise ValueError("Incomplete assembly body")
        edits.append((start, end, function))
    pieces = []
    cursor = 0
    for start, end, function in sorted(edits):
        fragment = "".join(lines[cursor:start])
        if INSTRUCTION.search(fragment):
            pieces.append({"kind": "asm", "content": HEADER + fragment})
        pieces.append({"kind": "c", "function": function})
        cursor = end
    remaining = "".join(lines[cursor:])
    if INSTRUCTION.search(remaining):
        pieces.append({"kind": "asm", "content": HEADER + remaining})
    # Every piece becomes its own object, and `.L*` names are local to their
    # object: a label defined in one piece and used in another links as an
    # undefined symbol (measured 2026-10-01: `lw $a0,%lo(.L002C0020)($s0)` in
    # one piece, the definition 54k lines later in another). Such names are
    # promoted to file-global `XL_*` symbols instead. No instruction and no
    # linked byte changes - only which object owns the name.
    fragments = [piece["content"] for piece in pieces if piece["kind"] == "asm"]
    positions = [index for index, piece in enumerate(pieces) if piece["kind"] == "asm"]
    defined: dict[str, int] = {}
    for index, fragment in enumerate(fragments):
        for match in re.finditer(r"(?m)^\s*\.?L([0-9A-Fa-f]+):", fragment):
            defined.setdefault(match.group(1), index)
    crossing: set[str] = set()
    for index, fragment in enumerate(fragments):
        for match in re.finditer(r"\.?L([0-9A-Fa-f]+)\b", fragment):
            if defined.get(match.group(1), index) != index:
                crossing.add(match.group(1))
    for name in sorted(crossing):
        for index, fragment in enumerate(fragments):
            renomme = re.sub(r"\.?L" + re.escape(name) + r"\b", "XL_" + name, fragment)
            if defined.get(name) == index:
                renomme = re.sub(r"(?m)^(\s*)XL_" + re.escape(name) + r":",
                                 r"\1.globl XL_" + name + "\n\\1XL_" + name + ":", renomme, count=1)
            fragments[index] = renomme
    for index, fragment in enumerate(fragments):
        pieces[positions[index]]["content"] = fragment
    return pieces


def linker_instrument(toolchain: Path) -> Path:
    """The chain's linker; the 2.9-ee compiler tools are hashed through WSL."""
    return toolchain / "ee" / "bin" / "ld.exe"


def compile_snapshot(directory: Path, toolchain: Path, flags: list) -> tuple[Path, Path]:
    """Compile the reviewed public source into this build; never edit the reviewed copy."""
    source = ROOT / "candidates" / "boot.c"
    c_directory = directory / "build" / "c"
    c_directory.mkdir(parents=True)
    snapshot = c_directory / "boot.c"
    snapshot.write_bytes(source.read_bytes())
    object_path = c_directory / "boot.c.o"
    # Compiled under its bare name from its own directory, exactly as the
    # candidate checker does: `.file` carries the source spelling, so passing
    # the absolute build path here would hash a different object than the one
    # the candidate gate qualified, and that object would differ per machine.
    compile_c_source(snapshot, flags, object_path, None, directory / "compile-c.log")
    assert_fresh(object_path, [snapshot])
    return snapshot, object_path


def compile_c(reference: Path, directory: Path, toolchain: Path) -> tuple[dict, Path, dict]:
    catalog = json.loads((ROOT / "config" / "candidate-catalog.json").read_text(encoding="utf-8"))
    candidates = json.loads((ROOT / "progress" / "candidates.json").read_text(encoding="utf-8"))
    source = ROOT / "candidates" / "boot.c"
    if file_hash(reference) != catalog["reference_sha256"] or file_hash(source) != candidates["source_sha256"]:
        raise ValueError("Integration inputs changed since candidate review")
    if file_hash(ROOT / "config" / "candidate-catalog.json") != candidates["catalog_sha256"]:
        raise ValueError("Integration catalogue changed since review")
    expected = {(entry["symbol"], entry["address"], entry["size"]) for entry in catalog["functions"]}
    actual = {(entry["symbol"], entry.get("address"), entry.get("size")) for entry in candidates["functions"]
              if entry["matched"]}
    if actual != expected or len(candidates["functions"]) != len(expected):
        raise ValueError("Every integrated function requires a complete candidate match")
    linker = linker_instrument(toolchain)
    hashes = tool_hashes(toolchain)
    if hashes != candidates["tools"]:
        raise ValueError("C integration instruments differ from the qualified candidate run")
    snapshot, object_path = compile_snapshot(directory, toolchain, catalog["flags"])
    if file_hash(snapshot) != candidates["source_sha256"]:
        raise ValueError("C source changed while creating the integration snapshot")
    catalog["compiled_source_sha256"] = candidates["source_sha256"]
    c_directory = object_path.parent
    qualification_script = c_directory / "qualification.ld"
    qualification_script.write_text(linker_script(catalog), encoding="ascii")
    qualified = c_directory / "qualification.elf"
    run([str(linker), "-T", str(qualification_script), "-o", str(qualified), str(object_path)],
        directory / "qualify-c-object.log")
    assert_fresh(qualified, [object_path, snapshot, qualification_script])
    results = [compare_function(reference, qualified, function["symbol"], function["address"], function["size"])
               for function in catalog["functions"]]
    if not all(result["matched"] for result in results):
        raise ValueError("The exact C object used for integration failed its independent candidate gate")
    object_proof = {"target": catalog["target"], "reference_sha256": file_hash(reference),
                    "source_sha256": file_hash(snapshot), "object_sha256": file_hash(object_path),
                    "candidate_elf_sha256": file_hash(qualified),
                    "catalog_sha256": file_hash(ROOT / "config" / "candidate-catalog.json"),
                    "checker_sha256": file_hash(ROOT / "scripts" / "check_candidates.py"),
                    "verified_at": datetime.now(timezone.utc).isoformat(),
                    "tools": hashes, "flags": catalog["flags"], "functions": results,
                    "integrated_functions": 0,
                    "profile_scope": "Independent qualification of the exact object subsequently used in the full boot"}
    (directory / "object-qualification.json").write_text(json.dumps(object_proof, indent=2) + "\n", encoding="utf-8")
    return catalog, object_path, hashes


def level_catalog(level: str) -> dict:
    """The reviewed placement of the SAME C bodies at one level's own addresses.

    A level is a different link of shared engine code, so the boot symbol keeps
    its name and the level's measured address replaces the boot address. Only
    reviewed boot symbols with their complete reviewed size may be placed, and
    two bodies may never claim one address.
    """
    document = json.loads((ROOT / "config" / "level-catalog.json").read_text(encoding="utf-8"))
    levels = document.get("levels")
    if not isinstance(levels, dict) or level not in levels:
        raise ValueError("No reviewed C placement catalog for this level")
    entry = levels[level]
    overlays = json.loads((ROOT / "config" / "overlays.json").read_text(encoding="utf-8"))
    pinned = {item["level"]: item["sha256"] for item in overlays["levels"]}
    if pinned.get(level) != entry.get("reference_sha256"):
        raise ValueError("Level catalog reference is not the pinned overlay identity")
    boot = json.loads((ROOT / "config" / "candidate-catalog.json").read_text(encoding="utf-8"))
    reviewed = {function["symbol"]: function["size"] for function in boot["functions"]}
    functions = entry.get("functions")
    if not isinstance(functions, list) or not functions:
        raise ValueError("Level placement requires at least one reviewed body")
    seen_symbols = set()
    seen_addresses = set()
    for function in functions:
        symbol, address, size = function.get("symbol"), function.get("address"), function.get("size")
        if symbol not in reviewed or reviewed[symbol] != size:
            raise ValueError("Level placement must reuse a reviewed complete C body")
        if type(address) is not int or type(size) is not int or size <= 0 or size % 4 or address % 4:
            raise ValueError("Invalid level placement boundary")
        if symbol in seen_symbols or address in seen_addresses:
            raise ValueError("Duplicate level placement symbol or address")
        seen_symbols.add(symbol)
        seen_addresses.add(address)
    # A placement whose body calls another function carries the address of each callee
    # IN THIS LEVEL ("externals"), measured by masked search: the jal operand is absolute,
    # so it differs between the boot and the overlay while every other byte is identical.
    externals = entry.get("externals", {})
    if not isinstance(externals, dict):
        raise ValueError("Level externals must be an object")
    for name, address in externals.items():
        if type(address) is not int or address <= 0 or address % 4:
            raise ValueError("Invalid level external address")
    return {"target": boot["target"], "level": level, "reference_sha256": entry["reference_sha256"],
            "flags": boot["flags"], "functions": functions, "externals": externals}


def compile_level_c(reference: Path, directory: Path, toolchain: Path, level: str) -> tuple[dict, Path, dict]:
    """Compile the reviewed C again and qualify that exact object at level addresses."""
    catalog = level_catalog(level)
    candidates = json.loads((ROOT / "progress" / "candidates.json").read_text(encoding="utf-8"))
    if file_hash(reference) != catalog["reference_sha256"]:
        raise ValueError("Level reference changed since the placement catalog was measured")
    linker = linker_instrument(toolchain)
    hashes = tool_hashes(toolchain)
    if hashes != candidates["tools"]:
        raise ValueError("C integration instruments differ from the qualified candidate run")
    snapshot, object_path = compile_snapshot(directory, toolchain, catalog["flags"])
    if file_hash(snapshot) != candidates["source_sha256"]:
        raise ValueError("C source changed while creating the level integration snapshot")
    catalog["compiled_source_sha256"] = candidates["source_sha256"]
    c_directory = object_path.parent
    qualification_script = c_directory / "level-qualification.ld"
    # The compiled object also carries the boot-only bodies, whose data
    # references belong to the boot image; only the sections placed in this
    # level may enter this link, and they must resolve nothing external.
    # The qualification link declares gp = 0: a placed body that needed a
    # gp-relative or external reference would fail this gate closed.
    script = linker_script({**catalog, "gp": 0}).replace(
        "/DISCARD/ : { *(.reginfo) }", "/DISCARD/ : { *(.reginfo) *(.text.FUN_*) }")
    qualification_script.write_text(script, encoding="ascii")
    qualified = c_directory / "level-qualification.elf"
    run([str(linker), "-T", str(qualification_script), "-o", str(qualified), str(object_path)],
        directory / "qualify-level-object.log")
    assert_fresh(qualified, [object_path, snapshot, qualification_script])
    results = [compare_function(reference, qualified, function["symbol"], function["address"], function["size"])
               for function in catalog["functions"]]
    if not all(result["matched"] for result in results):
        raise ValueError("The exact C object used for level integration failed its level qualification")
    object_proof = {"target": catalog["target"], "program": level, "reference_sha256": file_hash(reference),
                    "source_sha256": file_hash(snapshot), "object_sha256": file_hash(object_path),
                    "candidate_elf_sha256": file_hash(qualified),
                    "catalog_sha256": file_hash(ROOT / "config" / "level-catalog.json"),
                    "checker_sha256": file_hash(ROOT / "scripts" / "check_candidates.py"),
                    "verified_at": datetime.now(timezone.utc).isoformat(),
                    "tools": hashes, "flags": catalog["flags"], "functions": results,
                    "integrated_functions": 0,
                    "profile_scope": "Independent qualification of the exact object subsequently used in this level"}
    (directory / "level-object-qualification.json").write_text(json.dumps(object_proof, indent=2) + "\n",
                                                               encoding="utf-8")
    return catalog, object_path, hashes


def replace_inputs(directory: Path, sources: list[Path], catalog: dict, c_object: Path,
                   relocated: bool = False) -> tuple[list[Path], dict]:
    script = directory / "config" / "rac2.ld"
    content = script.read_text(encoding="ascii")
    replacements = {}
    unchanged = []
    found = set()
    for source in sources:
        original = source.read_text(encoding="ascii")
        functions = [function for function in catalog["functions"]
                     if re.search(r"^\s*\.globl\s+func_" + f"{function['address']:08X}" + r"\s*$", original, re.MULTILINE)]
        if not functions:
            unchanged.append(source)
            continue
        declarations = re.findall(r"^\s*\.section\s+([^,\s]+)", original, re.MULTILINE)
        if any(name != ".text" for name in declarations):
            raise ValueError("Integration supports a pure text assembly input only")
        relative = source.relative_to(directory / "asm_pp")
        old_object = "build/asm/" + relative.as_posix() + ".o"
        old_text = old_object + "(.text);"
        if content.count(old_text) != 1:
            raise ValueError("Cannot identify one linked original text input")
        new_inputs = []
        pieces = split_assembly(original, functions, relocated=relocated)
        for index, piece in enumerate(pieces):
            if piece["kind"] == "c":
                function = piece["function"]
                found.add(function["symbol"])
                new_inputs.append(c_object.relative_to(directory).as_posix() + f"(.text.{function['symbol']});")
            else:
                fragment = directory / "asm_pp" / "integrated" / f"{source.stem}_{index}.s"
                fragment.parent.mkdir(exist_ok=True)
                fragment.write_text(piece["content"], encoding="ascii")
                unchanged.append(fragment)
                fragment_object = "build/asm/" + fragment.relative_to(directory / "asm_pp").as_posix() + ".o"
                new_inputs.append(fragment_object + "(.text);")
        content = content.replace(old_text, "\n        ".join(new_inputs))
        content = re.sub(re.escape(old_object) + r"\(\.(?:data|rodata|bss)\);", "", content)
        replacements[relative.as_posix()] = [function["symbol"] for function in functions]
    if found != {function["symbol"] for function in catalog["functions"]}:
        raise ValueError("Not all reviewed C bodies replaced original assembly inputs")
    script.write_text(content, encoding="ascii")
    return unchanged, replacements


def add_definitions(directory: Path, catalog: dict) -> None:
    path = directory / "config" / "undefined_symbols.ld"
    content = path.read_text(encoding="ascii")
    for function in catalog["functions"]:
        original = f"func_{function['address']:08X}"
        content = re.sub(r"^" + re.escape(original) + r"\s*=.*?;\s*$", "", content, flags=re.MULTILINE)
        content += f"{original} = {function['symbol']};\n"
    for name, address in catalog["externals"].items():
        content = re.sub(r"^" + re.escape(name) + r"\s*=.*?;\s*$", "", content, flags=re.MULTILINE)
        content += f"{name} = 0x{address:08X};\n"
    # Only the boot catalog carries a gp base. A level placement must not
    # invent one: the reviewed level bodies are self-contained, and any
    # gp-relative or external reference would fail the qualification gate
    # instead of being papered over by an unchecked base value.
    if "gp" in catalog:
        content += f"_gp = 0x{catalog['gp']:08X};\n"
    path.write_text(content, encoding="ascii")


def validate_integrated(reference: Path, candidate: Path, catalog: dict, source: Path,
                       program: str = "boot") -> list[dict]:
    if file_hash(source) != catalog["compiled_source_sha256"]:
        raise ValueError("Public C source changed after the compiled integration snapshot")
    results = [compare_function(reference, candidate, function["symbol"], function["address"], function["size"])
               for function in catalog["functions"]]
    if not all(result["matched"] for result in results):
        raise ValueError("An integrated C body failed the complete post-link comparison")
    for result in results:
        result.update({"integrated": True, "program": program})
        result["state"] = "integrated"
    return results
