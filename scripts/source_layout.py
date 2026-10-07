#!/usr/bin/env python3
"""Assemble authoritative source fragments into standalone C units (stdlib only)."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re

BOOT_PLAN = [
    ("types-and-layouts", b"typedef float f32;"),
    ("resident-accessors", b"void *FUN_00115200(void)"),
    ("hud-and-math", b"int FUN_0028B740(HudElem *rec"),
    ("ported-utilities", b"/* Third lot:"),
    ("leaf-field-accessors", b"typedef struct { unsigned long long lo;"),
    ("small-field-transforms", b"/* SECOND LOT --"),
    ("call-bearing-helpers", b"/* The campaign:"),
    ("object-operations", b"s32 FUN_00288A00(u8 *a0"),
    ("resident-io", b"extern u8 D_001A63A8[];"),
    ("object-and-table-operations", b"s32 FUN_0029AE78(s32 *a0)"),
    ("packed-pixel-decoder", b"void FUN_00297550(u8 *out"),
    ("resident-state-and-callbacks", b"typedef struct { u8 before[0x68];"),
    ("aligned-clear", b"/* Clear one aligned 128-bit object"),
]
PILOT_FAMILY = "native-clear-five-words"
FUNCTION_PLACEHOLDER = b"@@FUNCTION@@"
CLEAR_CANONICAL_BODY = b"void @@FUNCTION@@(s32 *object) {\n    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;\n}"
SHIP_CLEAR_VARIANT = b"void @@FUNCTION@@(int *object) {\n    object[0] = 0;\n    object[1] = 0;\n    object[2] = 0;\n    object[3] = 0;\n    object[4] = 0;\n}"
# One reviewed placement anchors each shared source family: the normalized
# authored body is the family identity, so any program may anchor one. The
# legacy anchor program keeps its historical family id spelling.
BASE_SEED_PLACEMENTS = (
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_003B2900"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002ADE68"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002ADEA0"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002ADFD0"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002AEAC0"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002D68E8"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002D7940"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002E3A68"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002F3DD0"),
    ("15_gorn", "LVL_15_GORN_FUN_002FB958"),
    ("3_endako", "LVL_3_ENDAKO_FUN_0043D650"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002A7070"),
    ("15_gorn", "LVL_15_GORN_FUN_0031EC90"),
    ("11_joba", "LVL_11_JOBA_FUN_0035AC48"),
    ("13_boldan", "LVL_13_BOLDAN_FUN_004407D8"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_00346330"),
    ("25_wupash_nebula", "LVL_25_WUPASH_NEBULA_FUN_002F5CD8"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_0032E768"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_003EA860"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_0042C6E0"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_0035F100"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002F36D8"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_003A2B28"),
    ("13_boldan", "LVL_13_BOLDAN_FUN_003876E0"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_0041AF60"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_003883B8"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_003C5B18"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_0042C768"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_00409420"),
    ("13_boldan", "LVL_13_BOLDAN_FUN_0037F258"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_00375940"),
    ("13_boldan", "LVL_13_BOLDAN_FUN_0034FDD8"),
    ("10_hrugis_cloud", "LVL_10_HRUGIS_CLOUD_FUN_003E5E90"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002D1F78"),
    ("11_joba", "LVL_11_JOBA_FUN_0048E038"),
    ("17_smolg", "LVL_17_SMOLG_FUN_0031C1E0"),
    ("13_boldan", "LVL_13_BOLDAN_FUN_00440860"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_002D3068"),
    ("11_joba", "LVL_11_JOBA_FUN_0048E0C0"),
    ("3_endako", "LVL_3_ENDAKO_FUN_00347A08"),
    ("0_aranos_tutorial", "LVL_0_ARANOS_TUTORIAL_FUN_00304FF0"),
)
BASE_SEED_SYMBOLS = tuple(symbol for _, symbol in BASE_SEED_PLACEMENTS)


def seed_family_id(level: str, symbol: str) -> str:
    """Stable legacy ids for the original anchor program; explicit ids elsewhere."""
    if symbol.endswith("002D7940"):
        return PILOT_FAMILY
    address = symbol.split("_FUN_")[1].lower()
    return "native-" + address if level == "0_aranos_tutorial" else f"native-{level}-{address}"


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def contained(root: Path, relative: str) -> Path:
    path = (root / relative).resolve()
    if not path.is_relative_to(root.resolve()) or path == root.resolve():
        raise ValueError(f"path leaves declared root: {relative}")
    return path


def private_output(repo: Path, output: Path) -> None:
    repo, output = repo.resolve(), output.resolve()
    if output == repo or repo in output.parents or output in repo.parents:
        raise ValueError("generated preview must stay outside and not contain the repository")


def write_new(path: Path, data: bytes) -> None:
    if path.exists() and path.read_bytes() != data:
        raise ValueError(f"refusing to overwrite changed draft: {path}")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def function_span(data: bytes, name: str) -> tuple[int, int]:
    """Locate a catalogued definition and lex braces outside comments/strings."""
    pattern = rb"(?m)^[A-Za-z_][^;{}]*?\b" + re.escape(name.encode()) + rb"\s*\([^;{}]*?\)\s*\{"
    hits = list(re.finditer(pattern, data))
    if len(hits) != 1:
        raise ValueError(f"expected one source definition for {name}, got {len(hits)}")
    start = hits[0].start()
    pos = hits[0].end() - 1
    depth = 0
    state = "code"
    while pos < len(data):
        char = data[pos:pos + 1]
        pair = data[pos:pos + 2]
        if state == "comment":
            if pair == b"*/":
                state, pos = "code", pos + 2
                continue
        elif state == "line":
            if char == b"\n":
                state = "code"
        elif state in ("string", "char"):
            if char == b"\\":
                pos += 2
                continue
            if char == (b'"' if state == "string" else b"'"):
                state = "code"
        elif pair == b"/*":
            state, pos = "comment", pos + 2
            continue
        elif pair == b"//":
            state, pos = "line", pos + 2
            continue
        elif char in (b'"', b"'"):
            state = "string" if char == b'"' else "char"
        elif char == b"{":
            depth += 1
        elif char == b"}":
            depth -= 1
            if depth == 0:
                return start, pos + 1
        pos += 1
    raise ValueError(f"unterminated definition: {name}")


def normalized_body(data: bytes, name: str, externals: dict) -> tuple[bytes, dict]:
    """Only rename the definition and explicitly catalogued external tokens."""
    substitutions = {name: "@@FUNCTION@@"}
    for symbol, address in externals.items():
        if re.search(rb"\b" + re.escape(symbol.encode()) + rb"\b", data):
            substitutions[symbol] = f"@@EXTERNAL_{address:08X}@@"
    pattern = rb"\b(?:" + b"|".join(re.escape(k.encode()) for k in substitutions) + rb")\b"
    return re.sub(pattern, lambda m: substitutions[m[0].decode()].encode(), data), substitutions


def checked_fragment(layout: Path, name: str, data: bytes, write: bool = True) -> dict:
    if write:
        write_new(contained(layout, name), data)
    return {"fragment": name, "sha256": digest(data), "source_text_bytes": len(data)}


def load_inputs(repo: Path) -> tuple[dict, list[tuple[str, dict]], dict]:
    boot_name = "config/candidate-catalog.json"
    boot_bytes = contained(repo, boot_name).read_bytes()
    boot = json.loads(boot_bytes)
    inputs = {boot_name: digest(boot_bytes)}
    native = []
    for path in sorted((repo / "config/level-native").glob("*.json")):
        relative = path.relative_to(repo).as_posix()
        data = path.read_bytes()
        catalog = json.loads(data)
        if catalog["target"] != "SCUS_972.68":
            raise ValueError(f"wrong target: {relative}")
        inputs[relative] = digest(data)
        native.append((relative, catalog))
    if len(native) != 27 or boot["target"] != "SCUS_972.68":
        raise ValueError("pilot expects this target and all 27 native catalogues")
    return boot, native, inputs


def capture(repo: Path, layout: Path, write: bool = True, expected_manifest_hash: str | None = None) -> dict:
    repo, layout = repo.resolve(), layout.resolve()
    if write and layout.is_relative_to(repo):
        raise ValueError("draft must be outside the public repository")
    boot, native, inputs = load_inputs(repo)
    fragment = lambda name, data: checked_fragment(layout, name, data, write=write)
    boot_data = contained(repo, "candidates/boot.c").read_bytes()
    inputs["candidates/boot.c"] = digest(boot_data)
    boot_spans = {f["symbol"]: function_span(boot_data, f["symbol"]) for f in boot["functions"]}
    boundaries = []
    for name, marker in BOOT_PLAN:
        positions = [m.start() for m in re.finditer(re.escape(marker), boot_data)]
        if not positions:
            raise ValueError(f"boot boundary disappeared: {name}")
        offset = positions[0]
        if any(start < offset < end for start, end in boot_spans.values()):
            raise ValueError(f"boot boundary splits a function: {name}")
        boundaries.append((name, offset))
    if boundaries[0][1] != 0 or [o for _, o in boundaries] != sorted(set(o for _, o in boundaries)):
        raise ValueError("boot boundary plan is not a contiguous ordered partition")
    boot_recipe = []
    boot_modules = []
    for index, (name, start) in enumerate(boundaries):
        end = boundaries[index + 1][1] if index + 1 < len(boundaries) else len(boot_data)
        piece = fragment(f"src/boot/{index:02d}-{name}.cfrag", boot_data[start:end])
        boot_recipe.append(piece)
        boot_modules.append({"name": name, "start_byte": start, "end_byte": end,
                             "functions": [n for n, (s, e) in boot_spans.items() if start <= s and e <= end],
                             "boundary_evidence": "reviewed organization boundary; not an original object boundary"})

    catalogs = {c["level"]: (name, c) for name, c in native}
    base_templates = {}
    for seed_level, seed_symbol in BASE_SEED_PLACEMENTS:
        if seed_level not in catalogs:
            raise ValueError(f"unknown anchor program: {seed_level}")
        _, seed = catalogs[seed_level]
        seed_data = contained(repo, seed["source"]).read_bytes()
        f = next(f for f in seed["functions"] if f["symbol"] == seed_symbol)
        start, end = function_span(seed_data, f["symbol"])
        normalized, _ = normalized_body(seed_data[start:end], f["symbol"], seed["externals"])
        family = PILOT_FAMILY if f["meaning"].startswith("clear five object fields") else seed_family_id(seed_level, f["symbol"])
        key = digest(normalized)
        if key in base_templates:
            raise ValueError("ambiguous base source template")
        base_templates[key] = {"id": family, "template_sha256": key, "catalogued_size": f["size"],
                               "meaning": f["meaning"], "placements": [], "normalized_source": normalized}
    pilot = next(f for f in base_templates.values() if f["id"] == PILOT_FAMILY)
    if pilot["normalized_source"].count(FUNCTION_PLACEHOLDER) != 1:
        raise ValueError("pilot must contain exactly one definition token")
    pilot_piece = fragment("src/levels/shared/clear-five-words.cfrag", pilot["normalized_source"])
    ship_piece = fragment("src/levels/shared/clear-five-words-ship-variant.cfrag", SHIP_CLEAR_VARIANT)
    recipes = {"candidates/boot.c": {"sha256": digest(boot_data), "source_text_bytes": len(boot_data), "pieces": boot_recipe}}
    singleton_entries = []
    shared_preludes = {}
    for catalog_name, catalog in native:
        relative = catalog["source"]
        data = contained(repo, relative).read_bytes()
        inputs[relative] = digest(data)
        functions = {}
        pilot_symbol = None
        current_pilot_piece = pilot_piece
        for f in catalog["functions"]:
            start, end = function_span(data, f["symbol"])
            functions[f["symbol"]] = (start, end)
            normalized, renames = normalized_body(data[start:end], f["symbol"], catalog["externals"])
            key = digest(normalized)
            placement = {"level": catalog["level"], "program": catalog["program"], "source": relative,
                         "catalogue": catalog_name, "symbol": f["symbol"], "address": f["address"],
                         "catalogued_size": f["size"], "body_sha256": digest(data[start:end]),
                         "normalized_body_sha256": key, "explicit_symbol_substitutions": renames}
            family = base_templates.get(key)
            if catalog["level"] == "24_ship_shack" and f["symbol"] == "LVL_24_SHIP_SHACK_FUN_002D6960":
                if normalized != SHIP_CLEAR_VARIANT or f["size"] != 24:
                    raise ValueError("explicit Ship Shack source variant changed")
                family = pilot
                current_pilot_piece = ship_piece
                placement["source_variant"] = "ship-shack-int-and-original-format"
                placement["relationship_evidence"] = "reviewed five exact zero assignments; int and s32 declarations retained; different source text, independently catalogued placement"
            if family is not None:
                if normalized != family["normalized_source"] or f["size"] != family["catalogued_size"]:
                    if placement.get("source_variant") != "ship-shack-int-and-original-format":
                        raise ValueError("source-equivalent family has incompatible size")
                family["placements"].append(placement)
                if family["id"] == PILOT_FAMILY:
                    pilot_symbol = f["symbol"]
            else:
                # Leave every unproved equivalence distinct, even if sizes agree.
                singleton_entries.append(placement)
        if pilot_symbol is None:
            raise ValueError(f"pilot family missing: {relative}")
        start, end = functions[pilot_symbol]
        prelude_end = 0 if catalog["level"] == "24_ship_shack" else data.index(b"extern ")
        if prelude_end > start:
            raise ValueError("unexpected prelude order")
        prelude = data[:prelude_end]
        prelude_key = digest(prelude)
        pieces = []
        if prelude:
            if prelude_key not in shared_preludes:
                shared_preludes[prelude_key] = fragment(f"src/levels/shared/prelude-{prelude_key[:12]}.cfrag", prelude)
            pieces.append(shared_preludes[prelude_key])
        pieces.extend([fragment(f"src/levels/placements/{catalog['level']}-before.cfrag", data[prelude_end:start]),
                       dict(current_pilot_piece, replacements={"@@FUNCTION@@": pilot_symbol}),
                       fragment(f"src/levels/placements/{catalog['level']}-after.cfrag", data[end:])])
        recipes[relative] = {"sha256": digest(data), "source_text_bytes": len(data), "pieces": pieces}
    from boot_sdk_unit import admitted_units, load_catalog, unit_spec
    sdk_catalogs = [load_catalog(repo, unit) for unit in admitted_units(repo)]
    if sdk_catalogs:
        from boot_sdk_unit import PROFILE, CONTROLS
        for dependency in ('config/target.json', PROFILE, CONTROLS):
            inputs[dependency] = digest(contained(repo, dependency).read_bytes())
    for sdk in sdk_catalogs:
        data = contained(repo, sdk["source"]).read_bytes()
        piece = fragment(sdk["module"], data)
        recipes[sdk["source"]] = {"sha256": digest(data), "source_text_bytes": len(data), "pieces": [piece]}
        catalog_path = unit_spec(sdk["unit_id"])["catalog"]
        inputs[catalog_path] = digest(contained(repo, catalog_path).read_bytes())
        inputs[sdk["source"]] = inputs[sdk["module"]] = digest(data)
    families = [{k: v for k, v in family.items() if k != "normalized_source"} for family in base_templates.values()]
    native_functions = sum(len(c["functions"]) for _, c in native)
    native_bytes = sum(f["size"] for _, c in native for f in c["functions"])
    unique_native = len(families) + len(singleton_entries)
    unique_native_bytes = sum(f["catalogued_size"] for f in families) + sum(f["catalogued_size"] for f in singleton_entries)
    metrics = {
        "scope": "authored boot and native candidate catalogues only; excludes replicated boot/common overlay coverage",
        "identity_basis": "catalogued definitions compared as exact source bytes after recorded symbol-only substitutions; not a new machine proof",
        "boot_authored_functions": len(boot["functions"]), "boot_catalogued_machine_bytes": sum(f["size"] for f in boot["functions"]),
        "native_explicit_base_source_families": len(families), "native_base_family_placements": sum(len(f["placements"]) for f in families),
        "native_unmerged_singletons": len(singleton_entries), "native_unique_family_or_singleton_work_items": unique_native,
        "native_unique_authored_source_variants": unique_native + 1,
        "native_unique_representative_catalogued_bytes": unique_native_bytes,
        "native_authored_variant_catalogued_bytes_upper_bound": unique_native_bytes + 24,
        "native_replicated_placements": native_functions, "native_replicated_catalogued_bytes": native_bytes,
        "total_unique_authored_source_variants_in_scope": len(boot["functions"]) + unique_native + 1,
        "pilot_family_placements": len(pilot["placements"]), "pilot_family_unique_catalogued_bytes": pilot["catalogued_size"],
        "pilot_family_replicated_catalogued_bytes": sum(p["catalogued_size"] for p in pilot["placements"]),
        "pilot_family_authored_source_variants": 2,
        "shared_native_prelude_variants": len(shared_preludes),
        "warning": "Representative catalogued bytes are an organization metric, not loaded-byte progress or a new accepted match. Do not add this numerator to the public report.",
    }
    if sdk_catalogs:
        metrics["sdk_authored_functions"] = len(sdk_catalogs)
        metrics["sdk_catalogued_machine_bytes"] = sum(f["size"] for c in sdk_catalogs for f in c["functions"])
        metrics["total_unique_authored_source_variants_in_scope"] += len(sdk_catalogs)
        metrics["scope"] = "authored default boot, separate SDK boot and native catalogues; excludes replicated common overlay coverage"
    manifest = {"schema": 1, "target": "SCUS_972.68", "input_sha256": inputs,
                "generator_sha256": digest(Path(__file__).read_bytes()),
                "boot_modules": boot_modules, "recipes": recipes, "native_source_families": families,
                "native_unmerged_singletons": singleton_entries, "metrics": metrics,
                "promotion": "Pilot only. Generated C must stay byte-identical; compiler/checker unchanged. Parent reruns full gates if public integration changes any proof input."}
    if write:
        destination = layout / "config/source-layout.json"
        data = (json.dumps(manifest, indent=2, sort_keys=True) + "\n").encode()
        if destination.exists() and destination.read_bytes() != data:
            if expected_manifest_hash is None or digest(destination.read_bytes()) != expected_manifest_hash:
                raise ValueError("refusing manifest replacement without its explicit current SHA256")
            destination.write_bytes(data)
        else:
            write_new(destination, data)
    return manifest


def render(layout: Path, manifest: dict, enforce_hashes: bool = True) -> tuple[dict, dict]:
    sources, recipes = {}, {}
    for relative, recipe in manifest["recipes"].items():
        assembled = []
        pieces = []
        for piece in recipe["pieces"]:
            data = contained(layout, piece["fragment"]).read_bytes()
            if enforce_hashes and (digest(data) != piece["sha256"] or len(data) != piece["source_text_bytes"]):
                raise ValueError(f"changed source fragment: {piece['fragment']}")
            pieces.append(dict(piece, sha256=digest(data), source_text_bytes=len(data)))
            for token, replacement in piece.get("replacements", {}).items():
                if data.count(token.encode()) != 1:
                    raise ValueError(f"missing or repeated placement token: {token}")
                data = data.replace(token.encode(), replacement.encode())
            if b"@@" in data:
                raise ValueError("unresolved source placement token")
            assembled.append(data)
        result = b"".join(assembled)
        sources[relative] = result
        recipes[relative] = {"sha256": digest(result), "source_text_bytes": len(result), "pieces": pieces}
    return sources, recipes


def analyze(repo: Path, sources: dict, recipes: dict) -> dict:
    """Recompute source inventories without re-slicing authoritative modules."""
    boot, native, inputs = load_inputs(repo)
    from boot_sdk_unit import admitted_units, load_catalog, unit_spec
    sdk_catalogs = [load_catalog(repo, unit) for unit in admitted_units(repo)]
    if sdk_catalogs:
        from boot_sdk_unit import PROFILE, CONTROLS
        for dependency in ('config/target.json', PROFILE, CONTROLS):
            inputs[dependency] = digest(contained(repo, dependency).read_bytes())
    for sdk_catalog in sdk_catalogs:
        path = unit_spec(sdk_catalog["unit_id"])["catalog"]
        inputs[path] = digest(contained(repo, path).read_bytes())
        inputs[sdk_catalog["module"]] = digest(contained(repo, sdk_catalog["module"]).read_bytes())
    expected_sources = {"candidates/boot.c"} | {c["source"] for _, c in native} | {c["source"] for c in sdk_catalogs}
    if set(sources) != expected_sources:
        raise ValueError("recipe source inventory differs from the catalogues")
    definition_pattern = rb"(?m)^[A-Za-z_][^;{}]*?\b((?:LVL_[A-Z0-9_]+_)?FUN_[0-9A-F]+)\s*\([^;{}]*?\)\s*\{"
    for catalog in [dict(boot, source="candidates/boot.c")] + [c for _, c in native] + sdk_catalogs:
        data = sources[catalog["source"]]
        defined = {m[1].decode() for m in re.finditer(definition_pattern, data)}
        catalogued = {f["symbol"] for f in catalog["functions"]}
        if catalog in sdk_catalogs:
            spec = unit_spec(catalog["unit_id"])
            if digest(data) != spec["source_sha256"] or catalogued != {spec["function"]["symbol"]}:
                raise ValueError("Fixed SDK whole source/function changed")
            # Exact reviewed source hash bounds definitions including names outside the default regex.
            defined = catalogued
        if defined != catalogued:
            raise ValueError(f"source/catalogue definitions differ: {catalog['source']}")
        for f in catalog["functions"]:
            function_span(data, f["symbol"])
    inputs.update({relative: digest(data) for relative, data in sources.items()})
    catalogs_by_level = {c["level"]: c for _, c in native}
    templates = {}
    for seed_level, symbol in BASE_SEED_PLACEMENTS:
        seed = catalogs_by_level.get(seed_level)
        if seed is None:
            raise ValueError(f"unknown anchor program: {seed_level}")
        f = next(f for f in seed["functions"] if f["symbol"] == symbol)
        start, end = function_span(sources[seed["source"]], symbol)
        body, _ = normalized_body(sources[seed["source"]][start:end], symbol, seed["externals"])
        family_id = PILOT_FAMILY if symbol.endswith("002D7940") else seed_family_id(seed_level, symbol)
        if digest(body) in templates:
            raise ValueError("ambiguous normalized base family")
        templates[digest(body)] = {"id": family_id, "template_sha256": digest(body), "catalogued_size": f["size"],
                                   "meaning": f["meaning"], "placements": []}
    pilot = next(f for f in templates.values() if f["id"] == PILOT_FAMILY)
    singletons = []
    variant_keys = {f["id"]: set() for f in templates.values()}
    for catalog_name, catalog in native:
        data = sources[catalog["source"]]
        for f in catalog["functions"]:
            start, end = function_span(data, f["symbol"])
            normalized, renames = normalized_body(data[start:end], f["symbol"], catalog["externals"])
            key = digest(normalized)
            placement = {"level": catalog["level"], "program": catalog["program"], "source": catalog["source"],
                         "catalogue": catalog_name, "symbol": f["symbol"], "address": f["address"],
                         "catalogued_size": f["size"], "body_sha256": digest(data[start:end]),
                         "normalized_body_sha256": key, "explicit_symbol_substitutions": renames}
            family = templates.get(key)
            if (catalog["level"] == "24_ship_shack" and f["symbol"] == "LVL_24_SHIP_SHACK_FUN_002D6960"
                    and normalized == SHIP_CLEAR_VARIANT and pilot["template_sha256"] == digest(CLEAR_CANONICAL_BODY)):
                # This is an explicit reviewed source variant, never a size-based merge.
                family = pilot
                placement["source_variant"] = "ship-shack-int-and-original-format"
                placement["relationship_evidence"] = "reviewed five exact zero assignments; int and s32 declarations retained; different source text, independently catalogued placement"
            if family is None:
                singletons.append(placement)
            else:
                if family["catalogued_size"] != f["size"]:
                    raise ValueError("source family has incompatible catalogue sizes")
                family["placements"].append(placement)
                variant_keys[family["id"]].add(key)
    families = list(templates.values())
    unique_items = len(families) + len(singletons)
    variants = sum(len(v) for v in variant_keys.values()) + len(singletons)
    unique_bytes = sum(f["catalogued_size"] for f in families) + sum(f["catalogued_size"] for f in singletons)
    variant_bytes = sum(len(variant_keys[f["id"]]) * f["catalogued_size"] for f in families) + sum(f["catalogued_size"] for f in singletons)
    prelude_paths = {p["fragment"] for r in recipes.values() for p in r["pieces"] if "/shared/prelude-" in p["fragment"]}
    metrics = {
        "scope": "authored boot and native candidate catalogues only; excludes replicated boot/common overlay coverage",
        "identity_basis": "catalogued definitions compared as exact source bytes after recorded symbol-only substitutions; not a new machine proof",
        "boot_authored_functions": len(boot["functions"]), "boot_catalogued_machine_bytes": sum(f["size"] for f in boot["functions"]),
        "native_explicit_base_source_families": len(families), "native_base_family_placements": sum(len(f["placements"]) for f in families),
        "native_unmerged_singletons": len(singletons), "native_unique_family_or_singleton_work_items": unique_items,
        "native_unique_authored_source_variants": variants, "native_unique_representative_catalogued_bytes": unique_bytes,
        "native_authored_variant_catalogued_bytes_upper_bound": variant_bytes,
        "native_replicated_placements": sum(len(c["functions"]) for _, c in native),
        "native_replicated_catalogued_bytes": sum(f["size"] for _, c in native for f in c["functions"]),
        "total_unique_authored_source_variants_in_scope": len(boot["functions"]) + variants,
        "pilot_family_placements": len(pilot["placements"]), "pilot_family_unique_catalogued_bytes": pilot["catalogued_size"],
        "pilot_family_replicated_catalogued_bytes": sum(p["catalogued_size"] for p in pilot["placements"]),
        "pilot_family_authored_source_variants": len(variant_keys[PILOT_FAMILY]), "shared_native_prelude_variants": len(prelude_paths),
        "warning": "Representative catalogued bytes are an organization metric, not loaded-byte progress or a new accepted match. Do not add this numerator to the public report.",
    }
    if sdk_catalogs:
        metrics["sdk_authored_functions"] = len(sdk_catalogs)
        metrics["sdk_catalogued_machine_bytes"] = sum(f["size"] for c in sdk_catalogs for f in c["functions"])
        metrics["total_unique_authored_source_variants_in_scope"] += len(sdk_catalogs)
        metrics["scope"] = "authored default boot, separate SDK boot and native catalogues; excludes replicated common overlay coverage"
    return {"input_sha256": inputs, "generator_sha256": digest(Path(__file__).read_bytes()), "recipes": recipes,
            "native_source_families": families, "native_unmerged_singletons": singletons, "metrics": metrics}


def verify(repo: Path, layout: Path, output: Path | None = None, expected_output_hashes: dict | None = None) -> dict:
    manifest = json.loads((layout / "config/source-layout.json").read_bytes())
    for relative, expected in manifest["input_sha256"].items():
        if digest(contained(repo, relative).read_bytes()) != expected:
            raise ValueError(f"stale public input: {relative}")
    sources, recipes = render(layout, manifest)
    derived = analyze(repo, sources, recipes)
    if any(manifest.get(k) != v for k, v in derived.items()):
        raise ValueError("manifest, generator, family mapping, or metrics are stale")
    if output is not None:
        private_output(repo, output)
    for relative, result in sources.items():
        if result != contained(repo, relative).read_bytes():
            raise ValueError(f"regenerated standalone source differs: {relative}")
    if output is not None:
        preflight_destinations(output, sources, expected_output_hashes)
        write_sources(output, sources)
    return {"schema": 1, "byte_identical_sources": len(sources),
            "sources": [{"source": n, "sha256": digest(d), "source_text_bytes": len(d)} for n, d in sources.items()],
            "metrics": manifest["metrics"],
            "compiler_or_retail_gate_run": False}


def preflight_destinations(output: Path, sources: dict, expected_hashes: dict | None) -> None:
    for relative, data in sources.items():
        path = contained(output, relative)
        if path.exists() and path.read_bytes() != data:
            expected = (expected_hashes or {}).get(relative)
            if expected is None or digest(path.read_bytes()) != expected:
                raise ValueError(f"refusing changed output without its explicit current hash: {relative}")


def write_sources(output: Path, sources: dict) -> None:
    for relative, data in sources.items():
        path = contained(output, relative)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)


def author(repo: Path, layout: Path, output: Path | None, expected_hashes: dict | None) -> dict:
    """Render edited authoritative fragments; preflight all destinations before writes."""
    path = layout / "config/source-layout.json"
    manifest = json.loads(path.read_bytes())
    for relative, old_hash in manifest["input_sha256"].items():
        current = digest(contained(repo, relative).read_bytes())
        if current != old_hash and (expected_hashes or {}).get(relative) != current:
            raise ValueError(f"changed input requires explicit current hash: {relative}")
    sources, recipes = render(layout, manifest, enforce_hashes=False)
    derived = analyze(repo, sources, recipes)
    destination = output.resolve() if output is not None else repo.resolve()
    if output is not None:
        private_output(repo, destination)
    preflight_destinations(destination, sources, expected_hashes)
    if output is None:
        expected_manifest = (expected_hashes or {}).get("config/source-layout.json")
        if expected_manifest != digest(path.read_bytes()):
            raise ValueError("source write requires the explicit current manifest hash")
    write_sources(destination, sources)
    if output is None:
        manifest.update(derived)
        # Historical bootstrap offsets are intentionally not used to re-slice sources.
        manifest["boot_modules"] = [{"name": Path(p["fragment"]).stem, "fragment": p["fragment"],
                                     "boundary_evidence": "authoritative contiguous source fragment; not an original object boundary"}
                                    for p in recipes["candidates/boot.c"]["pieces"]]
        manifest["machine_proof_status"] = "unverified after authoring; regenerate all affected reviews and loaded-byte gates"
        path.write_text(json.dumps(manifest, indent=2, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    return {"schema": 1, "rendered_sources": len(sources), "metrics": derived["metrics"],
            "manifest_refreshed": output is None, "compiler_or_retail_gate_run": False}


def main() -> None:
    root = Path(__file__).resolve().parent
    if root.name == "scripts":
        root = root.parent
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("action", nargs="?", default="verify", choices=("capture", "verify"))
    parser.add_argument("--repo", type=Path, default=root)
    parser.add_argument("--layout", type=Path, default=root)
    parser.add_argument("--output", type=Path)
    parser.add_argument("--inventory-output", type=Path, help="write a separate authored-source inventory")
    parser.add_argument("--inventory-check", type=Path, help="check an existing inventory without writing")
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--check", action="store_true", help="verify inputs, fragments, mappings and metrics without writes")
    mode.add_argument("--write", action="store_true", help="render authoritative source fragments; actual source replacement requires current hashes")
    parser.add_argument("--expected-output-hashes", type=Path, help="JSON object of explicit current SHA256 hashes allowing replacement of changed generated outputs")
    parser.add_argument("--expected-manifest-sha256", help="explicit current manifest SHA256 required to recapture a changed draft")
    args = parser.parse_args()
    if args.check and (args.output or args.inventory_output or args.action == "capture"):
        parser.error("--check performs no writes and cannot capture or use --output")
    expected = json.loads(args.expected_output_hashes.read_bytes()) if args.expected_output_hashes else None
    if args.action == "capture":
        result = capture(args.repo, args.layout, expected_manifest_hash=args.expected_manifest_sha256)
    elif args.write:
        result = author(args.repo, args.layout, args.output, expected)
    else:
        result = verify(args.repo, args.layout, args.output, expected)
    if args.inventory_output or args.inventory_check:
        inventory = {"schema": 1, "kind": "authored-source-inventory",
                     "source_layout_sha256": digest((args.layout / "config/source-layout.json").read_bytes()),
                     "metrics": result["metrics"], "integration_credit_added": 0}
        data = (json.dumps(inventory, indent=2, sort_keys=True) + "\n").encode()
        if args.inventory_check and args.inventory_check.read_bytes() != data:
            raise ValueError("stale authored-source inventory")
        if args.inventory_output:
            args.inventory_output.parent.mkdir(parents=True, exist_ok=True)
            args.inventory_output.write_bytes(data)
    print(json.dumps(result["metrics"], indent=2))


if __name__ == "__main__":
    main()
