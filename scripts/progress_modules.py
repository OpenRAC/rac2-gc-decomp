"""Read-only presentation of already-proved C records by authored source.

This mapper supplies no proof or matching credit. Call it after the maintained
exporter and object-proof checks. Units are accepted subsets, not retail objects.
"""
from __future__ import annotations

from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import re

BYTE_FIELDS = ("totalCode", "matchedCode", "completeCode", "totalData", "matchedData", "completeData")
DEFAULT = {"schema": 1, "kind": "presentation-only-module-roles", "default_role": "unclassified",
           "roles": {"unclassified": {"label": "Unclassified original role", "basis": "none", "confidence": "unknown"}},
           "assignments": []}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def _path(repo, relative, authored=False):
    if (not isinstance(relative, str) or not relative or "\\" in relative
            or Path(relative).is_absolute() or any(p in {"", ".", ".."} for p in relative.split("/"))
            or ":" in relative or (authored and (not relative.startswith("src/")
                                                 or Path(relative).suffix not in {".c", ".h", ".cfrag"}))):
        raise ValueError("Invalid public source path")
    path = repo / relative
    if path.is_symlink() or not path.resolve().is_relative_to(repo.resolve()):
        raise ValueError("Source path escapes the repository")
    return path


def _uint(value):
    if type(value) is int and value >= 0:
        return value
    if isinstance(value, str) and re.fullmatch(r"[0-9]+", value):
        return int(value)
    raise ValueError("Invalid nonnegative report integer")


def _mask(data):
    """Blank comments and literals byte-for-byte, retaining source coordinates."""
    out = bytearray(data)
    i = 0
    while i < len(data):
        start = i
        if data[i:i + 2] == b"/*":
            end = data.find(b"*/", i + 2)
            if end < 0:
                raise ValueError("Unterminated source comment")
            i = end + 2
        elif data[i:i + 2] == b"//":
            end = data.find(b"\n", i + 2)
            i = len(data) if end < 0 else end
        elif data[i:i + 1] in (b'"', b"'"):
            quote = data[i]
            i += 1
            while i < len(data):
                if data[i] == 92:
                    i += 2
                elif data[i] == quote:
                    i += 1
                    break
                else:
                    i += 1
            else:
                raise ValueError("Unterminated source literal")
        else:
            i += 1
            continue
        for pos in range(start, min(i, len(data))):
            if out[pos] not in (10, 13):
                out[pos] = 32
    return bytes(out)


def _definition_span(masked, symbol):
    # Same body-span contract as source_layout.function_span, with comments and
    # literals masked first so a declaration/example cannot steal attribution.
    if not isinstance(symbol, str) or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
        return None
    pattern = rb"(?m)^[ \t]*[A-Za-z_][^;{}]*?\b" + re.escape(symbol.encode()) + rb"\s*\([^;{}]*?\)\s*\{"
    hits = list(re.finditer(pattern, masked))
    if len(hits) != 1:
        return None
    depth = 0
    for pos in range(hits[0].end() - 1, len(masked)):
        depth += (masked[pos] == 123) - (masked[pos] == 125)
        if not depth:
            return hits[0].start(), pos + 1
    return None


class SourceMap:
    def __init__(self, repo):
        self.repo = Path(repo)
        raw = _path(self.repo, "config/source-layout.json").read_bytes()
        self.layout_sha256 = digest(raw)
        self.layout = json.loads(raw)
        for relative, expected in self.layout.get("input_sha256", {}).items():
            if digest(_path(self.repo, relative).read_bytes()) != expected:
                raise ValueError("Stale source-layout input: " + relative)
        self.sources, self.intervals, self.fragment_hashes = {}, {}, {}
        for source, recipe in self.layout["recipes"].items():
            pieces, intervals, offset = [], [], 0
            for piece in recipe["pieces"]:
                fragment = piece["fragment"]
                data = _path(self.repo, fragment, authored=True).read_bytes()
                if digest(data) != piece["sha256"] or len(data) != piece["source_text_bytes"]:
                    raise ValueError("Stale authored fragment: " + fragment)
                self.fragment_hashes[fragment] = piece["sha256"]
                for token, value in piece.get("replacements", {}).items():
                    if not isinstance(token, str) or not token or not isinstance(value, str):
                        raise ValueError("Invalid source substitution")
                    encoded = token.encode()
                    if data.count(encoded) != 1:
                        raise ValueError("Missing or repeated source token")
                    data = data.replace(encoded, value.encode())
                if b"@@" in data:
                    raise ValueError("Unresolved source token")
                intervals.append((offset, offset + len(data), fragment))
                offset += len(data)
                pieces.append(data)
            data = b"".join(pieces)
            if (digest(data) != recipe["sha256"] or len(data) != recipe["source_text_bytes"]
                    or _path(self.repo, source).read_bytes() != data):
                raise ValueError("Generated source differs from its exact recipe")
            self.sources[source] = _mask(data)
            self.intervals[source] = intervals
        self.catalogue = {}
        self.labels = {row["fragment"]: row["name"] for row in self.layout.get("boot_modules", [])}
        self._add("config/candidate-catalog.json", "boot-default", "boot", "candidates/boot.c")
        for directory, owner in (("config/level-native", "level-native"), ("config/level-g8", "level-smalldata"),
                                 ("config/boot-units", "sdk")):
            for path in sorted((self.repo / directory).glob("*.json")):
                self._add(path.relative_to(self.repo).as_posix(), owner)
        level_path = self.repo / "config/level-catalog.json"
        self.shared = json.loads(level_path.read_bytes()).get("levels", {}) if level_path.is_file() else {}
        self.cache = {}

    def _add(self, path, owner, program=None, source=None):
        data = json.loads(_path(self.repo, path).read_bytes())
        source = source or data["source"]
        program = program or data["program"]
        if owner == "sdk":
            owner = data["unit_id"]
        for row in data["functions"]:
            key = source, row["symbol"]
            if key in self.catalogue:
                raise ValueError("Ambiguous compiled-owner catalogue")
            self.catalogue[key] = (owner, program, row["address"], row["size"], path)

    def resolve(self, program, source, function):
        symbol, address, size = function["name"], _uint(function["metadata"]["virtualAddress"]), _uint(function["size"])
        row = self.catalogue.get((source, symbol))
        owner = row[0] if row else "unresolved-owner"
        if not row or row[3] != size:
            return owner, None, "catalogue-binding-unresolved"
        if row[1] == "boot" and program != "boot" and owner == "boot-default":
            placements = self.shared.get(program.removeprefix("levels/"), {}).get("functions", [])
            if not any(p["symbol"] == symbol and p["address"] == address and p["size"] == size for p in placements):
                return "boot-shared", None, "shared-placement-unresolved"
            owner = "boot-shared"
        elif row[1] != program or row[2] != address:
            return owner, None, "catalogue-binding-unresolved"
        key = source, symbol
        if key not in self.cache:
            span = _definition_span(self.sources.get(source, b""), symbol)
            matches = [fragment for start, end, fragment in self.intervals.get(source, [])
                       if span and start <= span[0] and span[1] <= end]
            self.cache[key] = matches[0] if len(matches) == 1 else None
        fragment = self.cache[key]
        return owner, fragment, "verified" if fragment else "definition-span-unresolved"


def _descriptor(repo, descriptor):
    if descriptor is None:
        path = Path(repo) / "config/progress-modules.json"
        descriptor = json.loads(path.read_bytes()) if path.is_file() else copy.deepcopy(DEFAULT)
    elif isinstance(descriptor, (str, Path)):
        descriptor = json.loads(Path(descriptor).read_bytes())
    if (not isinstance(descriptor, dict) or set(descriptor) != set(DEFAULT)
            or type(descriptor["schema"]) is not int or descriptor["schema"] != 1 or descriptor["kind"] != DEFAULT["kind"]
            or descriptor["default_role"] != "unclassified" or not isinstance(descriptor["roles"], dict)
            or not isinstance(descriptor["assignments"], list)):
        raise ValueError("Invalid presentation-only role descriptor")
    roles = descriptor["roles"]
    if "unclassified" not in roles:
        raise ValueError("Missing unknown role")
    for role, info in roles.items():
        if (not isinstance(role, str) or not re.fullmatch(r"[a-z][a-z0-9_-]*", role)
                or not isinstance(info, dict) or set(info) != {"label", "basis", "confidence"}
                or not isinstance(info["label"], str) or not info["label"]
                or info["basis"] not in {"none", "authored-label", "observed-effect", "documented-sdk-source"}
                or info["confidence"] not in {"unknown", "observed", "reviewed"}
                or info["label"].startswith(("/", "\\"))
                or re.search(r"\b[A-Za-z]:[/\\]|(?:^|\s)/(?:home|Users|mnt|root|private|tmp|var|Volumes|work|data)(?:/|$)", info["label"])
                or ((info["basis"] == "none") != (info["confidence"] == "unknown"))):
            raise ValueError("Invalid public role confidence")
    if roles["unclassified"]["basis"] != "none" or roles["unclassified"]["confidence"] != "unknown":
        raise ValueError("Unknown role must remain explicitly unknown")
    return descriptor


def _program(unit):
    name = unit["metadata"]["moduleName"]
    if name == "boot" or re.fullmatch(r"levels/[A-Za-z0-9_-]+", name):
        return name
    raise ValueError("Expected physical program identity in legacy report")


def _sum_measures(units):
    first = copy.deepcopy(units[0]["measures"])
    for field in BYTE_FIELDS:
        first[field] = str(sum(_uint(unit["measures"][field]) for unit in units))
    code, data = _uint(first["totalCode"]), _uint(first["totalData"])
    for field, count, total in (("matchedCodePercent", "matchedCode", code), ("completeCodePercent", "completeCode", code),
                                ("matchedDataPercent", "matchedData", data), ("completeDataPercent", "completeData", data)):
        first[field] = _uint(first[count]) / total * 100 if total else 0
    first["fuzzyMatchPercent"] = _uint(first["matchedCode"]) / (code + data) * 100 if code + data else 0
    first["totalUnits"] = len(units)
    first["completeUnits"] = sum(unit["metadata"].get("complete") is True for unit in units)
    return first


def group_report(report, repo, descriptor=None):
    """Return (grouped objdiff-v2 report, separate provenance/confidence summary)."""
    if report.get("version") != 2:
        raise ValueError("Expected objdiff report version 2")
    mapping, desc = SourceMap(repo), _descriptor(repo, descriptor)
    assignments = {}
    for row in desc["assignments"]:
        if not isinstance(row, dict) or set(row) != {"program", "symbol", "address", "size", "source_path", "source_sha256", "role"}:
            raise ValueError("Invalid role assignment; proof/credit overrides are forbidden")
        key = row["program"], row["symbol"], row["address"], row["size"]
        if key in assignments or row["role"] not in desc["roles"]:
            raise ValueError("Duplicate or unknown role assignment")
        _path(Path(repo), row["source_path"], authored=True)
        assignments[key] = row
    groups, unchanged, lineage, used = {}, [], [], set()
    original_names = set()
    for unit in report["units"]:
        if unit["name"] in original_names:
            raise ValueError("Duplicate input report unit")
        original_names.add(unit["name"])
        functions = unit.get("functions", [])
        if not functions or unit["metadata"].get("complete") is not True or unit["metadata"].get("autoGenerated") is not False:
            unchanged.append(copy.deepcopy(unit))
            continue
        if len(functions) != 1:
            raise ValueError("Expected legacy single-function proven units")
        function, values = functions[0], unit["measures"]
        size = _uint(function["size"])
        if (not size or any(_uint(values[field]) != size for field in ("totalCode", "matchedCode", "completeCode"))
                or any(_uint(values[field]) for field in ("totalData", "matchedData", "completeData"))
                or function["fuzzyMatchPercent"] != 100):
            raise ValueError("Presentation cannot promote incomplete function evidence")
        program, source = _program(unit), unit["metadata"].get("sourcePath")
        owner, fragment, state = mapping.resolve(program, source, function)
        identity = program, function["name"], _uint(function["metadata"]["virtualAddress"]), size
        role = "unclassified"
        if identity in assignments:
            row = assignments[identity]
            if fragment != row["source_path"] or mapping.fragment_hashes.get(fragment) != row["source_sha256"]:
                raise ValueError("Stale or unresolved role source binding")
            used.add(identity)
            role = row["role"]
        key = (program, owner, fragment, role) if fragment else (program, owner, None, unit["name"])
        groups.setdefault(key, []).append(unit)
        lineage.append({"old_unit_name": unit["name"], "program": program, "compiled_source": source,
                        "proof_owner": owner, "authored_source": fragment, "mapping_state": state,
                        "role_id": role, "role_confidence": desc["roles"][role]["confidence"],
                        "symbols": [function["name"]], "_key": key})
    if used != set(assignments):
        raise ValueError("Orphan role assignment is outside proved function records")
    categories = {item["id"]: copy.deepcopy(item) for item in report["categories"]}
    units, modules = unchanged, []
    for key, members in sorted(groups.items(), key=lambda item: tuple(str(v) for v in item[0])):
        members = sorted(members, key=lambda unit: (_uint(unit["functions"][0]["metadata"]["virtualAddress"]), unit["name"]))
        program, owner, fragment, role_key = key
        if not fragment:
            unit = copy.deepcopy(members[0])
            unit["metadata"].pop("sourcePath", None)
            group_name = unit["name"]
        else:
            group_name = f"{program}/accepted/{owner}/{fragment}/{role_key}"
            unit = {"name": group_name, "measures": _sum_measures(members),
                    "functions": [copy.deepcopy(f) for m in members for f in m["functions"]],
                    "sections": [copy.deepcopy(s) for m in members for s in m.get("sections", [])],
                    "metadata": {"complete": True, "autoGenerated": False, "sourcePath": fragment,
                                 "moduleName": f"{program}/{owner}/{mapping.labels.get(fragment, Path(fragment).stem)} (accepted subset)",
                                 "progressCategories": list(members[0]["metadata"]["progressCategories"])}}
            unit["measures"]["totalUnits"] = unit["measures"]["completeUnits"] = 1
            parent = "boot" if program == "boot" else "levels"
            extra = {f"{parent}.{owner}": owner + " (accepted subset)",
                     f"{parent}.roles.{role_key}": desc["roles"][role_key]["label"] + " (accepted subset)",
                     f"{parent}.module_{digest(fragment.encode())[:16]}": "Authored source: " + fragment + " (accepted subset)"}
            for category, title in extra.items():
                unit["metadata"]["progressCategories"].append(category)
                categories.setdefault(category, {"id": category, "name": title})
        units.append(unit)
        modules.append({"unit_name": group_name, "program": program, "proof_owner": owner,
                        "compiled_source": members[0]["metadata"].get("sourcePath"), "authored_source": fragment,
                        "role_id": role_key if fragment else "unclassified", "scope": "accepted authored subset",
                        "function_placements": len(members), "matched_code": sum(_uint(m["measures"]["matchedCode"]) for m in members)})
        for row in lineage:
            if row["_key"] == key:
                row["group_unit_name"] = group_name
    for row in lineage:
        row.pop("_key")
    # A planet denominator is all scoped physical code/data, not just the
    # accepted source subset. Retain opaque records except this category tag.
    for unit in units:
        program = (unit["name"].split("/accepted/", 1)[0] if "/accepted/" in unit["name"]
                   else _program(unit))
        if program.startswith("levels/"):
            category = "levels.level_" + program.split("/")[1]
            if category not in unit["metadata"]["progressCategories"]:
                unit["metadata"]["progressCategories"].append(category)
            categories.setdefault(category, {"id": category, "name": program + " (all scoped EE code and data)"})
    result = copy.deepcopy(report)
    result["units"] = sorted(units, key=lambda unit: unit["name"])
    result["measures"]["totalUnits"] = len(units)
    result["measures"]["completeUnits"] = sum(unit["metadata"].get("complete") is True for unit in units)
    for category, item in categories.items():
        subset = [u for u in units if category in u["metadata"].get("progressCategories", [])]
        if category in {c["id"] for c in report["categories"]}:
            item["measures"]["totalUnits"] = len(subset)
            item["measures"]["completeUnits"] = sum(u["metadata"].get("complete") is True for u in subset)
        elif subset:
            item["measures"] = _sum_measures(subset)
    result["categories"] = [categories[key] for key in sorted(categories)]
    before = Counter(json.dumps(f, sort_keys=True) for u in report["units"] for f in u.get("functions", []))
    after = Counter(json.dumps(f, sort_keys=True) for u in units for f in u.get("functions", []))
    before_sections = Counter(json.dumps(s, sort_keys=True) for u in report["units"] for s in u.get("sections", []))
    after_sections = Counter(json.dumps(s, sort_keys=True) for u in units for s in u.get("sections", []))
    if before != after or before_sections != after_sections:
        raise ValueError("Grouping changed function/section records")
    if any(result["measures"][key] != report["measures"][key] for key in report["measures"] if key not in {"totalUnits", "completeUnits"}):
        raise ValueError("Grouping changed proved measures")
    summary = {"schema": 1, "kind": "accepted-source-module-presentation", "integration_credit_added": 0,
               "source_layout_sha256": mapping.layout_sha256, "function_placements": len(lineage),
               "mapped_function_placements": sum(row["mapping_state"] == "verified" for row in lineage),
               "display_units": len(units), "original_units": len(report["units"]),
               "roles": desc["roles"], "modules": modules, "lineage": sorted(lineage, key=lambda row: row["old_unit_name"])}
    return result, summary
