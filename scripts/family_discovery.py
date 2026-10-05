"""Discover bounded J/JAL call-shape candidates from private pinned EE bodies.

Discovery is not matching: no bytes, instruction words, disassembly, integration
credit or source-unique progress metric are exported. Complete extents and their
boundary-evidence descriptions must be provided explicitly by the caller.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path

import elf_tools
import region

ROOT = Path(__file__).resolve().parents[1]
TARGET = "SCUS_972.68"
METHOD = "ee-j-jal-target26-v1"
ROW_KEYS = {"program", "address", "size", "boundary_evidence", "reference_sha256", "body_sha256"}


def _hash(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _pin(value: object, label: str) -> str:
    if not isinstance(value, str) or re.fullmatch(r"[0-9a-f]{64}", value) is None:
        raise ValueError(f"{label} must be a lowercase SHA-256")
    return value


def private_path(path: Path, repo: Path, label: str) -> Path:
    """Resolve aliases before refusing paths inside, equal to or containing repo."""
    resolved = Path(path).resolve()
    root = Path(repo).resolve()
    if resolved == root or resolved.is_relative_to(root) or root.is_relative_to(resolved):
        raise ValueError(f"{label} must be private and separate from the repository")
    return resolved


def _shape_hash(body: bytes) -> str:
    # Original narrow implementation: all other bits and the full extent remain.
    shape = bytearray(body)
    for offset in range(0, len(body), 4):
        word = struct.unpack_from("<I", body, offset)[0]
        if word >> 26 in (2, 3):
            struct.pack_into("<I", shape, offset, word & 0xFC000000)
    return _hash(shape)


def _body(data: bytes, elf: dict, address: int, size: int) -> bytes:
    if elf["type"] != elf_tools.ET_EXEC:
        raise ValueError("Reference must be an executable ELF")
    end = address + size
    sections = [s for s in elf["sections"] if s["flags"] & 6 == 6
                and address < s["address"] + s["size"] and s["address"] < end]
    if (len(sections) != 1 or sections[0]["type"] != 1
            or sections[0]["name"] not in (".text", "core.text")
            or address < sections[0]["address"] or end > sections[0]["address"] + sections[0]["size"]):
        raise ValueError("Complete body must belong to one allocated PROGBITS EE .text/core.text section")
    mappings = [s for s in elf["segments"] if s["type"] == elf_tools.PT_LOAD
                and address < s["address"] + s["memsz"] and s["address"] < end]
    if len(mappings) != 1:
        raise ValueError("Complete body must have one unambiguous PT_LOAD mapping")
    mapping, section = mappings[0], sections[0]
    if (not mapping["flags"] & 1 or address < mapping["address"]
            or end > mapping["address"] + mapping["filesz"]):
        raise ValueError("Complete body must be loaded, executable and file-backed")
    offset = mapping["offset"] + address - mapping["address"]
    if offset != section["offset"] + address - section["address"]:
        raise ValueError("Executable section and PT_LOAD file mapping disagree")
    return data[offset:offset + size]


def discovery(repo: Path, reference_root: Path, scope_dict: dict) -> dict:
    """Validate the entire bounded scope, then return metadata-only candidates.

    Reference layout is boot.elf and levels/<id>/overlay.elf. The region registry
    is the authority for every selected full-reference pin. Provided boundary
    evidence is retained as metadata, not certified by discovery or given credit.
    """
    repo = Path(repo).resolve()
    references = private_path(reference_root, repo, "References")
    if not references.is_dir():
        raise ValueError("References must be an existing private directory")
    if (not isinstance(scope_dict, dict) or type(scope_dict.get("schema")) is not int
            or scope_dict["schema"] != 1 or scope_dict.get("target") != TARGET):
        raise ValueError(f"Scope must declare schema 1 and target {TARGET}")
    if set(scope_dict) != {"schema", "target", "functions"}:
        raise ValueError("Unexpected scope fields")
    selected = region.by_serial(TARGET, repo)
    selected.require_pinned("family discovery references")
    selected.require_matching("family discovery")
    pins = selected.program_pins()
    for program, pin in pins.items():
        _pin(pin, f"Region pin for {program}")
    rows = scope_dict["functions"]
    if not isinstance(rows, list) or not rows:
        raise ValueError("Scope functions must be a nonempty list")

    # Phase 1: validate all descriptors and disjoint program intervals.
    descriptors = []
    for row in rows:
        if not isinstance(row, dict) or set(row) - ROW_KEYS:
            raise ValueError("Invalid or unexpected function fields")
        program, address, size = row.get("program"), row.get("address"), row.get("size")
        evidence = row.get("boundary_evidence")
        if (not isinstance(program, str)
                or not (program == "boot" or re.fullmatch(r"levels/[A-Za-z0-9_-]+", program))
                or program not in pins):
            raise ValueError("Function program is not a pinned target program")
        if (type(address) is not int or type(size) is not int or address < 0 or size <= 0
                or address % 4 or size % 4 or address + size > 1 << 32):
            raise ValueError("Function address and complete positive extent must be aligned ELF32 integers")
        if (not isinstance(evidence, str) or not evidence.strip()
                or any(char in evidence for char in "\x00\n\r")):
            raise ValueError("Function requires a nonempty single-line boundary_evidence description")
        for name in ("reference_sha256", "body_sha256"):
            if name in row:
                _pin(row[name], name)
        descriptors.append(dict(row))
    ordered = sorted(descriptors, key=lambda row: (row["program"], row["address"], row["size"]))
    for first, second in zip(ordered, ordered[1:]):
        if first["program"] == second["program"] and second["address"] < first["address"] + first["size"]:
            raise ValueError("Duplicate aliases or overlapping function intervals are forbidden")

    # Phase 2: validate every selected reference and every complete body/pin.
    snapshots = {}
    for program in sorted({row["program"] for row in descriptors}):
        relative = Path("boot.elf") if program == "boot" else Path(program) / "overlay.elf"
        path = (references / relative).resolve()
        if not path.is_relative_to(references):
            raise ValueError("Reference path escapes the private reference root")
        private_path(path, repo, "Reference file")
        data = path.read_bytes()
        if _hash(data) != pins[program]:
            raise ValueError(f"Full reference SHA-256 differs from region pin: {program}")
        # Parse the same immutable read snapshot that was hashed.
        snapshots[program] = (data, elf_tools._parse(data))
    validated = []
    for row in ordered:
        program = row["program"]
        data, elf = snapshots[program]
        if "reference_sha256" in row and row["reference_sha256"] != pins[program]:
            raise ValueError("Provided reference_sha256 differs from the pinned full reference")
        body = _body(data, elf, row["address"], row["size"])
        digest = _hash(body)
        if "body_sha256" in row and row["body_sha256"] != digest:
            raise ValueError("Provided body_sha256 differs from the complete reference body")
        validated.append((row, body, digest))

    # Grouping begins only after every selected input passed validation.
    groups = {}
    for row, body, digest in validated:
        shape = _shape_hash(body)
        key = (row["size"], shape)
        group = groups.setdefault(key, {
            "family_id": f"{METHOD}:{row['size']}:{shape}", "size": row["size"],
            "shape_sha256": shape, "state": "discovery_only", "matching": False,
            "integration_credit": 0, "placements": [],
        })
        group["placements"].append({
            "program": row["program"], "address": row["address"], "size": row["size"],
            "boundary_evidence": row["boundary_evidence"],
            "reference_sha256": pins[row["program"]], "body_sha256": digest,
        })
    families = [groups[key] for key in sorted(groups)]
    canonical_scope = json.dumps(scope_dict, sort_keys=True, separators=(",", ":")).encode("utf8")
    return {
        "schema": 1, "kind": "family-discovery", "target": TARGET,
        "region": selected.name, "method": METHOD, "scope_sha256": _hash(canonical_scope),
        "state": "discovery_only", "matching": False, "integration_credit": 0,
        "reference_sha256": {program: pins[program] for program in sorted(snapshots)},
        "families": families,
        "limits": ["J/JAL destinations can refer to different callees; a shared shape is not identity.",
                   "Constants, GP/HI/LO/COP2 fields and trailing NOP words are retained.",
                   "Boundary descriptions are caller evidence, not a discovery certification.",
                   "Every placement still requires complete exact C checking and full-image integration gates."],
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=ROOT)
    parser.add_argument("--references", type=Path, required=True)
    parser.add_argument("--functions", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args(argv)
    try:
        repo = args.repo.resolve()
        functions = private_path(args.functions, repo, "Function scope")
        output = private_path(args.output, repo, "Output")
        if args.output.exists() or args.output.is_symlink() or output.exists() or output.is_symlink():
            raise FileExistsError("Output already exists; choose a new private path")
        scope = json.loads(functions.read_bytes())
        report = discovery(repo, args.references, scope)
        serialized = json.dumps(report, indent=2) + "\n"
        # No directory/file is created until all scope/reference validation passed.
        output.parent.mkdir(parents=True, exist_ok=True)
        with output.open("x", encoding="utf8") as stream:
            stream.write(serialized)
    except (OSError, ValueError, TypeError) as error:
        parser.error(str(error))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
