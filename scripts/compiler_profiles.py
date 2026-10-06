"""Strict, non-executing metadata foundation for explicit compiler jobs.

This does not install a profile, replace the legacy compiler, or execute commands.
The first SDK scope authorizes only its three already reviewed leaf controls.
"""
from __future__ import annotations

from dataclasses import dataclass
import hashlib
import json
from pathlib import Path, PurePosixPath, PureWindowsPath
import re
import struct

from elf_tools import read_elf
from level_native import C_ONLY

SDK_PROFILE = "owned-sdk-b9-single-text-controls-v1"
LEGACY_PROFILE = "rac2-gnu8bed-default-v1"
PIPELINE = "sdk-driver-integrated-cpp-strip-v1"
FLAGS = ("-I../../src", "-I../../include", "-Iinclude", "-Wa,-I../../include",
         "-Wa,-I../..", "-DBUILD_US_VERSION", "-DMATCHING_DECOMP", "-O2", "-g2", "-gstabs")
STRIP = ("-N", "dummy-symbol-name", "-R", ".mdebug")
TOOLS = {
    "driver": "64d0a50fef499da0b98177eb5e79e41dfb066ad44246b137c78a266ef97ee265",
    "cc1": "b9aef69f93efb949f15ea58189e8eef4a002b9fe4de5d3fcf89c34e1244ec026",
    "as": "296af123052ee39d175e5b3254102aafca105d4f6e975b351a13867f59b04019",
    "cpp_available": "f85a54d241e019993fa7a06285d3677856b3b431e10318a587ab029373c603d7",
    "cc1plus_available": "da6ef339898e721fa6393f608f7c5de975b9510841d4c2967f318d6108f62512",
    "strip": "fce5d577f2ed6bb8bdb61028d44c5b6be87187b55230e1bb41eccb0053cbbf76",
    "linker": "80f3724a63ce3c77c8b00d075f5c49448636518700ae251e5d6a6c809bbf5f48",
}
CONTROLS = {
    "dual-prime388": ("dual_prime_vector.c", "FUN_0012B3E0", 0x12B3E0, 388),
    "track116": ("update_temp_track_data.c", "FUN_0012CFE8", 0x12CFE8, 116),
    "IPUsync104": ("sce_ipu_sync.c", "FUN_00130DB8", 0x130DB8, 104),
}
SOURCE_PIN = {
    "dual-prime388": "a93f12a2d464324f93702d81e9ee443f1d2558efb1e37562d4570da026231293",
    "track116": "5341a0f334ac6dda334fe3c1640c0b00d288222aaabced2eabb05ba630af68ab",
    "IPUsync104": "b9380afd55f31cac4c92fa00d81f6759f4f8d11e63823dfd1d647e583f8a931b",
}
MODULE_PIN = {
    "dual-prime388": "404fc9126ae9480c7895fd329eb66da1b7ee4bbcf5b6cb5f78f8f569bba6d3ac",
    "track116": "50040f4f5bb0739f30606a145f6da60ef5d35b90633809eadf2ed364a692a0f3",
    "IPUsync104": "65b283a69339b47088da7af35a4625372e210389cb42d52679f3081768a12d74",
}
BODY_PIN = {
    "dual-prime388": "cbd826f112e4e0f877738933fb36cb67db882ce5f8cc497b759686bc476cc949",
    "track116": "881b8eca0af3697f7830d70e73361ed7b9dd5bfd2d88169f4da511bdad7c717d",
    "IPUsync104": "e9d99b0d0566100caebf1fefb234fe159cc05faba045431055b51a218881ad5c",
}
HASH = re.compile(r"[0-9a-f]{64}\Z")
ID = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]*\Z")


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def fields(value: object, keys: set[str], label: str) -> dict:
    require(type(value) is dict and set(value) == keys, f"Unexpected {label} fields")
    return value


def integer(value: object, label: str, minimum: int = 0) -> int:
    require(type(value) is int and value >= minimum, f"Invalid {label}")
    return value


def sha(value: object) -> str:
    require(type(value) is str and HASH.fullmatch(value) is not None, "Invalid SHA256")
    return value


def encoded(value: object) -> bytes:
    return (json.dumps(value, sort_keys=True, separators=(",", ":")) + "\n").encode()


def digest(value: bytes) -> str:
    return hashlib.sha256(value).hexdigest()


def validator_sha256() -> str:
    return digest(Path(__file__).read_bytes())


def tools(value: object) -> dict:
    fields(value, set(TOOLS), "tool identity")
    require(value == TOOLS, "Driver/compiler/assembler/strip/linker closure drift")
    return value


@dataclass(frozen=True)
class Profile:
    canonical: bytes
    sha256: str

    def metadata(self) -> dict:
        return json.loads(self.canonical)


def validate_profile(value: object) -> Profile:
    p = fields(value, {"schema", "id", "pipeline", "scope", "flags", "strip_options", "tools",
                       "headers", "reference_sha256", "checker_sha256", "validator_sha256", "controls",
                       "qualification_sha256", "state", "limitations"}, "profile")
    integer(p["schema"], "schema", 1)
    require(p["schema"] == 1 and p["id"] == SDK_PROFILE and p["pipeline"] == PIPELINE, "Unknown compiler profile")
    require(p["scope"] == "leaf_controls_only", "Unqualified compiler scope")
    require(type(p["flags"]) is list and p["flags"] == list(FLAGS), "Fixed driver flags changed")
    require(type(p["strip_options"]) is list and p["strip_options"] == list(STRIP), "Fixed strip operation changed")
    tools(p["tools"])
    require(p["headers"] == {} and type(p["headers"]) is dict, "Standalone header closure changed")
    for name in ("reference_sha256", "checker_sha256", "validator_sha256"):
        sha(p[name])
    require(p["validator_sha256"] == validator_sha256(), "Profile validator drift")
    controls = fields(p["controls"], set(CONTROLS), "control set")
    for name, expected in CONTROLS.items():
        c = fields(controls[name], {"basename", "symbol", "address", "size", "source_sha256", "module_sha256",
                                   "reference_body_sha256"}, "control")
        integer(c["address"], "control address")
        integer(c["size"], "control size", 1)
        require((c["basename"], c["symbol"], c["address"], c["size"]) == expected, "Control identity/extent changed")
        require(c["source_sha256"] == SOURCE_PIN[name], "Control source drift")
        require(c["module_sha256"] == MODULE_PIN[name] and c["reference_body_sha256"] == BODY_PIN[name], "Control module/reference body changed")
    require(p["limitations"] == ["No call-bearing qualification", "No general SDK or full boot qualification",
                                  "No original compiler or module ownership claim", "No new C credit"], "Missing profile limitations")
    if p["qualification_sha256"] is None:
        require(p["state"] == "unknown_unqualified", "Missing qualification cannot authorize profile")
    else:
        sha(p["qualification_sha256"])
        require(p["state"] == "private_controls_submitted", "Profile promotion is outside this validator")
    data = encoded(p)
    return Profile(data, digest(data))


RUN_FIELDS = {"control_id", "pass_id", "source_sha256", "module_sha256", "invocation_sha256",
              "pre_strip_object_sha256", "object_sha256", "linked_elf_sha256", "link_script_sha256",
              "symbol", "address", "size", "produced_size", "text_size", "text_alignment", "text_offset",
              "symbol_count", "padding_bytes", "readonly_sections", "data_sections", "relocation_count",
              "reference_body_sha256", "candidate_body_sha256", "different_bytes", "executed_roles"}


@dataclass(frozen=True)
class QualifiedControls:
    profile: Profile
    receipt_sha256: str
    canonical_receipt: bytes
    scope: str = "leaf_controls_only"


def validate_qualification(profile: Profile, receipt: object, *, current_checker_sha256: str,
                           current_reference_sha256: str, current_modules: dict,
                           observed_tools: dict, observed_headers: dict) -> QualifiedControls:
    p = profile.metadata()
    validated = validate_profile(p)
    require(profile.canonical == validated.canonical and profile.sha256 == validated.sha256, "Profile handle drift")
    require(p["qualification_sha256"] is not None, "Unknown/unqualified profile")
    q = fields(receipt, {"schema", "profile_id", "pipeline", "scope", "state", "private_receipt_sha256",
                         "reference_sha256", "checker_sha256", "validator_sha256", "tools_before", "tools_after",
                         "headers_before", "headers_after", "runs", "credit_added"}, "qualification")
    require(digest(encoded(q)) == p["qualification_sha256"], "Qualification receipt pin changed")
    require(type(q["schema"]) is int and q["schema"] == 1 and q["profile_id"] == SDK_PROFILE
            and q["pipeline"] == PIPELINE and q["scope"] == "leaf_controls_only"
            and q["state"] == "complete_private_leaf_controls", "Incomplete/unknown qualification")
    require(type(q["credit_added"]) is int and q["credit_added"] == 0, "Controls add no credit")
    sha(q["private_receipt_sha256"])
    require(q["reference_sha256"] == p["reference_sha256"] == sha(current_reference_sha256), "Reference drift")
    require(q["checker_sha256"] == p["checker_sha256"] == sha(current_checker_sha256), "Checker drift")
    require(q["validator_sha256"] == p["validator_sha256"] == validator_sha256(), "Validator drift")
    tools(q["tools_before"]); tools(q["tools_after"]); tools(observed_tools)
    require(q["headers_before"] == q["headers_after"] == observed_headers == p["headers"] == {}, "Header drift")
    fields(current_modules, set(CONTROLS), "current control modules")
    require(type(q["runs"]) is list and len(q["runs"]) == 6, "Complete three controls/two passes required")
    seen = {}
    for item in q["runs"]:
        r = fields(item, RUN_FIELDS, "control run")
        require(r["control_id"] in CONTROLS and type(r["pass_id"]) is int and r["pass_id"] in (1, 2), "Unknown run")
        key = (r["control_id"], r["pass_id"])
        require(key not in seen, "Duplicate/missing control pass")
        c = p["controls"][r["control_id"]]
        for name in ("source_sha256", "module_sha256", "reference_body_sha256"):
            require(r[name] == c[name], "Control/source/module/reference drift")
        require(current_modules[r["control_id"]] == c["module_sha256"], "Current authored module changed")
        for name in ("invocation_sha256", "pre_strip_object_sha256", "object_sha256", "linked_elf_sha256", "link_script_sha256", "candidate_body_sha256"):
            sha(r[name])
        for name in ("address", "size", "produced_size", "text_size", "text_alignment", "text_offset", "symbol_count", "padding_bytes", "relocation_count", "different_bytes"):
            integer(r[name], name)
        require(r["symbol"] == c["symbol"] and r["address"] == c["address"]
                and r["size"] == r["produced_size"] == r["text_size"] == c["size"], "Complete symbol/text extent refused")
        require(r["symbol_count"] == 1 and r["text_offset"] == r["padding_bytes"] == r["relocation_count"] == r["different_bytes"] == 0
                and r["text_alignment"] == 8 and r["readonly_sections"] == r["data_sections"] == [], "Leaf single-text ownership refused")
        require(r["candidate_body_sha256"] == r["reference_body_sha256"], "Unmasked control mismatch")
        require(r["invocation_sha256"] == digest(encoded({"pipeline": PIPELINE, "flags": list(FLAGS),
                                                         "basename": c["basename"], "strip": list(STRIP)})), "Actual fixed invocation changed")
        require(r["executed_roles"] == ["driver", "cc1", "as", "strip", "linker"], "Unqualified preprocessing/child invocation")
        seen[key] = r
    for name in CONTROLS:
        a, b = seen.get((name, 1)), seen.get((name, 2))
        require(a is not None and b is not None, "Missing control/pass")
        require({k:v for k,v in a.items() if k != "pass_id"} == {k:v for k,v in b.items() if k != "pass_id"}, "Control determinism failed")
    return QualifiedControls(profile, digest(encoded(q)), encoded(q))


def inspect_single_text_object(path: Path, symbol: str, size: int) -> dict:
    """Use the strict ELF reader and inspect complete ET_REL ownership, no compiler."""
    integer(size, "expected complete size", 1)
    elf = read_elf(path); data = path.read_bytes()
    require(elf["type"] == 1, "Expected unmodified ET_REL object")
    sections = elf["sections"]
    text = [s for s in sections if s["type"] == 1 and s["size"] and s["flags"] & 6 == 6]
    require(len(text) == 1 and text[0]["name"] == ".text" and text[0]["size"] == size
            and text[0]["alignment"] == 8, "One whole exact .text section required")
    require(not [s for s in sections if s["size"] and s["type"] in (1, 8) and s["flags"] & 2
                 and not s["flags"] & 4 and s["name"] != ".reginfo"], "Unreviewed generated data")
    require(not [s for s in sections if s["size"] and (s["type"] in (4, 9) or s["name"] == ".mdebug")], "Leaf relocation/debug-strip contract refused")
    offset, stride, count = struct.unpack_from("<I", data, 32)[0], struct.unpack_from("<H", data, 46)[0], len(sections)
    headers = [struct.unpack_from("<10I", data, offset + i * stride) for i in range(count)]
    functions = []
    for h in headers:
        if h[1] != 2: continue
        require(h[6] < count and headers[h[6]][1] == 3 and h[9] == 16 and h[5] % 16 == 0, "Invalid object symbols")
        strings_h = headers[h[6]]; strings = data[strings_h[4]:strings_h[4]+strings_h[5]]
        for position in range(h[4], h[4]+h[5], 16):
            name, value, extent, info, _, owner = struct.unpack_from("<IIIBBH", data, position)
            if info & 15 != 2 or owner == 0: continue
            require(owner < count and name < len(strings), "Invalid function owner/name")
            end = strings.find(b"\0", name); require(end >= 0, "Unterminated symbol")
            functions.append((strings[name:end].decode("ascii"), value, extent, sections[owner]["name"], info >> 4))
    require(functions == [(symbol, 0, size, ".text", 1)], "Unexpected additional/partial/local function")
    return {"object_sha256": digest(data), "text_sha256": digest(data[text[0]["offset"]:text[0]["offset"]+size]),
            "text_alignment": 8, "text_size": size, "symbol_count": 1}


@dataclass(frozen=True)
class RuntimeBinding:
    profile_id: str
    runtime_id: str
    job_id: str
    distro: str
    cwd: str
    output_root: str
    tool_paths: tuple[tuple[str, str], ...]

    @classmethod
    def create(cls, *, profile_id: str, runtime_id: str, job_id: str, distro: str,
               cwd: str, output_root: str, tool_paths: dict) -> "RuntimeBinding":
        require(profile_id in (SDK_PROFILE, LEGACY_PROFILE), "Unknown binding profile")
        for value in (runtime_id, job_id, distro):
            require(type(value) is str and ID.fullmatch(value) is not None, "Invalid runtime/job/distro identity")
        roles = set(TOOLS) if profile_id == SDK_PROFILE else {"cc1", "cpp", "as", "ld.exe"}
        fields(tool_paths, roles, "private tool binding")
        for path in [cwd, output_root, *tool_paths.values()]:
            require(type(path) is str and not any(ord(c) < 32 for c in path)
                    and (PurePosixPath(path).is_absolute() or PureWindowsPath(path).is_absolute()), "Expected explicit absolute private path")
            require(".." not in PurePosixPath(path).parts and ".." not in PureWindowsPath(path).parts, "Private path traversal")
        require(PurePosixPath(cwd).is_absolute() and PurePosixPath(output_root).is_absolute()
                and cwd != output_root and not PurePosixPath(output_root).is_relative_to(PurePosixPath(cwd)), "Fresh output bank must be separate from immutable source cwd")
        return cls(profile_id, runtime_id, job_id, distro, cwd, output_root, tuple(sorted(tool_paths.items())))


@dataclass(frozen=True)
class ControlJob:
    profile_id: str
    qualification_sha256: str
    runtime_id: str
    job_id: str
    cwd: str
    argv: tuple[str, ...]
    strip_argv: tuple[str, ...]
    environment: tuple[tuple[str, str], ...]


def control_job(qualification: QualifiedControls, binding: RuntimeBinding, control_id: str,
                source: bytes, *, observed_tools: dict, observed_headers: dict,
                current_checker_sha256: str, current_reference_sha256: str, current_modules: dict) -> ControlJob:
    require(type(qualification) is QualifiedControls and qualification.scope == "leaf_controls_only", "Qualification required")
    require(type(binding) is RuntimeBinding and binding.profile_id == SDK_PROFILE, "Wrong explicit per-job binding")
    canonical_binding = RuntimeBinding.create(profile_id=binding.profile_id, runtime_id=binding.runtime_id, job_id=binding.job_id,
                                              distro=binding.distro, cwd=binding.cwd, output_root=binding.output_root,
                                              tool_paths=dict(binding.tool_paths))
    require(binding == canonical_binding, "Noncanonical/duplicate private tool binding")
    current = validate_qualification(qualification.profile, json.loads(qualification.canonical_receipt),
                                     current_checker_sha256=current_checker_sha256,
                                     current_reference_sha256=current_reference_sha256,
                                     current_modules=current_modules, observed_tools=observed_tools,
                                     observed_headers=observed_headers)
    require(current.receipt_sha256 == qualification.receipt_sha256, "Qualified handle drift")
    require(control_id in CONTROLS, "Target outside leaf controls scope")
    tools(observed_tools); require(observed_headers == {}, "Header closure drift")
    p = qualification.profile.metadata(); c = p["controls"][control_id]
    require(type(source) is bytes and digest(source) == c["source_sha256"], "Source changed/outside qualified controls")
    require(C_ONLY.search(source) is None and re.search(rb"(?m)^\s*#\s*include\b|\b__(?:DATE|TIME|TIMESTAMP)__\b", source) is None, "Source must remain standalone ordinary reproducible C")
    paths = dict(binding.tool_paths)
    for role in ("driver", "cc1", "as", "cpp_available", "cc1plus_available", "strip"):
        require(PurePosixPath(paths[role]).is_absolute(), "SDK tools must use explicit WSL absolute paths")
    output = str(PurePosixPath(binding.output_root) / binding.job_id / (c["basename"] + ".o"))
    env = {"PATH": ":".join([str(PurePosixPath(paths[r]).parent) for r in ("cc1", "as", "driver")] + ["/usr/bin", "/bin"]),
           "LANG": "C", "LC_ALL": "C", "TZ": "UTC", "HOME": binding.cwd, "TMPDIR": str(PurePosixPath(binding.cwd)/"tmp")}
    return ControlJob(SDK_PROFILE, qualification.receipt_sha256, binding.runtime_id, binding.job_id, binding.cwd,
                      (paths["driver"], "-v", "-c", *FLAGS, c["basename"], "-o", output),
                      (paths["strip"], output, *STRIP), tuple(sorted(env.items())))
