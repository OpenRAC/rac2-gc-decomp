"""Finite intrafunction GP facts from raw instructions, never an entry ABI assumption."""
from __future__ import annotations
import argparse
from collections import deque
import hashlib
import json
from pathlib import Path
import struct
import sys

MASK64 = (1 << 64) - 1
OBSERVED_WINDOWS_RABBITIZER_SHA256 = "86b8958b1eddc22c1df07ba6801d3d2132c63442a012522dc5a220f261bca7a5"
HASH_FIELDS = ("program", "address", "size", "raw_sha256", "reference_sha256", "boundary_evidence")


def sha(data):
    return hashlib.sha256(data).hexdigest()


def require(ok, message):
    if not ok:
        raise ValueError(message)


def sx32(value):
    value &= 0xffffffff
    return (value | 0xffffffff00000000) if value & 0x80000000 else value


def entry():
    return (0,) + (None,) * 31


def join(states):
    return tuple(values[0] if all(v == values[0] for v in values) else None
                 for values in zip(*states))


def finite_mmi_kind(word):
    # Only observed PMFHL.SH subopcode4 and PSRAH shift7, with operand fields.
    if word & ~0x0000f800 == 0x70000130:
        return "pmfhl.sh"
    if word & ~0x001ff800 == 0x700001f7:
        return "psrah"
    return None


def verify_finite_mmi_decoder(words):
    selected = [(i, w, finite_mmi_kind(w)) for i, w in enumerate(words) if finite_mmi_kind(w)]
    if not selected:
        return None
    import rabbitizer
    require(rabbitizer.__version__ == "1.16.2", "Qualified finite MMI decoder version drift")
    binary_sha256 = sha(Path(rabbitizer.__file__).read_bytes())
    # Qualify precisely this finite GPR-write contract on the actual platform
    # artifact, including all admitted operand variants. No Linux artifact pin
    # is invented. The actual producing hash is bound into consumer replay.
    probes = [0x70000130 | rd << 11 for rd in range(32)]
    probes += [0x700001f7 | rt << 16 | rd << 11 for rt in range(32) for rd in range(32)]
    for word in probes:
        instruction = rabbitizer.Instruction(word, 0, rabbitizer.InstrCategory.R5900)
        kind = finite_mmi_kind(word)
        require(instruction.getOpcodeName() == kind and instruction.modifiesRd()
                and not instruction.modifiesRt() and not instruction.readsRs()
                and instruction.readsRt() == (kind == "psrah")
                and instruction.getDestinationGpr().value == word >> 11 & 31,
                "Actual platform finite MMI qualification refused")
    receipts = []
    for index, word, kind in selected:
        instruction = rabbitizer.Instruction(word, 0, rabbitizer.InstrCategory.R5900)
        destination = word >> 11 & 31
        require(instruction.getOpcodeName() == kind and instruction.modifiesRd()
                and not instruction.modifiesRt() and not instruction.readsRs()
                and instruction.readsRt() == (kind == "psrah")
                and instruction.getDestinationGpr().value == destination,
                "Finite MMI GPR-write contract differs from independent decoder")
        receipts.append({"offset": index * 4, "opcode": kind, "destination_gpr": destination,
                         "reads_rt_gpr": kind == "psrah"})
    return {"decoder": "rabbitizer-1.16.2-R5900", "decoder_sha256": binary_sha256,
            "finite_operand_contract_checks": len(probes),
            "matches_independently_observed_Windows_artifact": binary_sha256 == OBSERVED_WINDOWS_RABBITIZER_SHA256,
            "general_decoder_ISA_qualification_claimed": False,
            "scope": "only PMFHL.SH subopcode4 and PSRAH shift7; no other MMI preservation",
            "instructions": receipts}


def transfer(word, state, call_after_slot=False):
    """Known lower64 constants only; upper R5900 GPR lanes are never inferred."""
    from relocation_identity import MEMORY, signed16, _control
    regs = list(state)
    op, rs, rt, rd, fn = word >> 26, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31, word & 63
    imm, source = word & 0xffff, state[rs]
    def write(register, value=None):
        if register:
            regs[register] = value
    if word == 0:
        pass
    elif op == 15 and rs == 0:
        write(rt, sx32(imm << 16))
    elif op in (9, 25):
        result = None if source is None else source + signed16(imm)
        write(rt, None if result is None else sx32(result) if op == 9 else result & MASK64)
    elif op in (12, 13, 14):
        result = None if source is None else (source & imm if op == 12 else source | imm if op == 13 else source ^ imm)
        write(rt, result)
    elif op == 0 and fn in (0x21, 0x2d, 0x25) and word >> 6 & 31 == 0:
        left, right = state[rs], state[rt]
        value = None if left is None or right is None else left | right if fn == 0x25 else left + right
        write(rd, None if value is None else sx32(value) if fn == 0x21 else value & MASK64)
    elif op == 0 and fn == 0 and rs == 0:
        shift = word >> 6 & 31
        write(rd, None if state[rt] is None else sx32((state[rt] & 0xffffffff) << shift))
    elif finite_mmi_kind(word) is not None:
        # Qualified finite operand-write witness: no GPR other than rd changes.
        write(rd)
    elif op in MEMORY:
        action, gpr, _ = MEMORY[op]
        if action == "load" and gpr:
            write(rt)
    elif op in (2, 3):
        if op == 3:
            write(31)
    elif op in (4, 5, 6, 7, 20, 21, 22, 23):
        # Reserved BLEZ/BGTZ operand encodings cannot authorize facts.
        if op in (6, 7, 22, 23) and rt != 0:
            regs = list(entry())
    elif op == 1 and rt in (0, 1, 2, 3):
        pass
    elif op == 0 and fn in (8, 9) and rt == 0 and word >> 6 & 31 == 0:
        if fn == 9:
            write(rd)
    elif op == 17 and rs == 8 and rt in (0, 1, 2, 3):
        pass
    else:
        # Unknown/MMI/COP2 arithmetic, special register transfers, syscalls,
        # traps and unsupported encodings may not preserve any tracked fact.
        regs = list(entry())
    if call_after_slot:
        # Delay slot executes before opaque callee entry/return. No saved-GP,
        # saved-register or SP ABI preservation is imported.
        regs = list(entry())
    regs[0] = 0
    return tuple(regs)


def mapped_target(sections, target, width):
    owners = [s for s in sections if s["type"] in (1, 8) and s["flags"] & 2
              and not s["flags"] & 4 and s["address"] <= target
              and target + width <= s["address"] + s["size"]]
    return owners[0]["name"] if len(owners) == 1 else None


def prove(body, address, *, program, reference_sha256, sections, boundary_evidence):
    from relocation_identity import _cfg, MEMORY, signed16, reconstruct
    require(type(address) is int and address >= 0 and address % 4 == 0 and type(body) is bytes
            and len(body) > 0 and len(body) % 4 == 0 and address + len(body) <= 1 << 32,
            "Invalid complete aligned raw extent")
    require(isinstance(program, str) and program and isinstance(reference_sha256, str)
            and len(reference_sha256) == 64 and all(c in "0123456789abcdef" for c in reference_sha256),
            "Missing pinned program/reference")
    require(isinstance(boundary_evidence, str) and boundary_evidence, "Missing supplied extent evidence")
    require(type(sections) is list and bool(sections), "Missing pinned mapped sections")
    for section in sections:
        require(set(section) == {"name", "type", "flags", "address", "size"}
                and isinstance(section["name"], str) and section["name"]
                and all(type(section[k]) is int for k in ("type", "flags", "address", "size"))
                and section["address"] >= 0 and section["size"] > 0
                and section["address"] + section["size"] <= 1 << 32
                and section["flags"] >= 0, "Invalid mapped section")
    codeowners = [s for s in sections if s["type"] == 1 and s["flags"] & 6 == 6
                  and s["name"] in (".text", "core.text") and s["address"] <= address
                  and address + len(body) <= s["address"] + s["size"]]
    require(len(codeowners) == 1, "Complete body lacks unique EE executable owner")
    words = struct.unpack("<" + "I" * (len(body) // 4), body)
    mmi_receipt = verify_finite_mmi_decoder(words)
    # System/exception control is outside this finite intrafunction CFG.
    require(not any(w >> 26 == 16 or w >> 26 == 0 and w & 63 in (12, 13)
                    for w in words), "Unsupported COP0/exception control in local CFG")
    edges, controls, delays = _cfg(words, address)
    # A static exit is not a theorem about the next machine PC. Unknown return
    # links, indirect targets and opaque callees can reenter this body with a
    # different GP. Admit only a closed direct CFG; no ABI return assumption.
    for kind, target, _ in controls.values():
        require(kind in ("branch", "jump") and target is not None
                and address <= target < address + len(body) and target % 4 == 0,
                "Unproved control escape or reentry: closed direct CFG required")
    reachable, pending = set(), [0]
    while pending:
        index = pending.pop()
        if index in reachable:
            continue
        reachable.add(index)
        require(bool(edges[index]), "Unproved fallthrough escape: closed direct CFG required")
        if index in controls and controls[index][0] == "branch":
            require(index + 2 < len(words), "Unproved branch fallthrough escape: closed direct CFG required")
        pending.extend(edges[index])
    predecessors = {i: [] for i in range(len(words))}
    for origin, targets in edges.items():
        for target in targets:
            predecessors[target].append(origin)
    incoming, outgoing, queue, queued = {}, {}, deque([0]), {0}
    visits = 0
    while queue:
        index = queue.popleft(); queued.remove(index); visits += 1
        require(visits <= max(128, len(words) * 64), "Local CFG failed bounded convergence")
        candidates = [outgoing[p] for p in predecessors[index] if p in outgoing]
        if index == 0:
            candidates.append(entry())
        if not candidates:
            continue
        state = join(candidates); incoming[index] = state
        delay = delays.get(index)
        after_call = delay is not None and delay[1][0] in ("call", "indirect-call")
        result = transfer(words[index], state, after_call)
        if outgoing.get(index) == result:
            continue
        outgoing[index] = result
        for target in edges[index]:
            if target not in queued:
                queued.add(target); queue.append(target)
    facts, memory, relocations = [], [], []
    for index, state in sorted(incoming.items()):
        word = words[index]; op, rs = word >> 26, word >> 21 & 31
        gp = state[28]
        if gp is not None:
            facts.append({"offset": index * 4, "gp_lower64": gp})
        if op not in MEMORY or rs != 28:
            continue
        action, gpr, width = MEMORY[op]
        target = None if gp is None else (gp + signed16(word & 0xffff)) & MASK64
        owner = mapped_target(sections, target, width) if target is not None and target <= 0xffffffff else None
        pointer = gp is not None and 0 <= gp < 0x02000000
        simple_access = op not in (0x22, 0x26, 0x2a, 0x2e, 0x1a, 0x1b, 0x2c, 0x2d)
        aligned = target is not None and target % width == 0
        valid = pointer and owner is not None and simple_access and aligned
        item = {"offset": index * 4, "opcode": op, "action": action, "width": width,
                "register_bank": "GPR" if gpr else "VU" if op in (0x36, 0x3e) else "FPR", "gp_lower64": gp,
                "target": target, "mapped_section": owner, "simple_aligned_access": simple_access and aligned,
                "locally_proved_mapped_target": valid,
                "reason": "local raw CFG constant and unique mapped target" if valid else
                    "unknown lifetime, non-low32/unmapped/ambiguous address, or unaligned/partial access; immediate retained"}
        memory.append(item)
        if valid:
            relocations.append({"kind": "gp16", "offset": index * 4, "target": target,
                "gp": gp, "role": "locally-proved-gp-memory-slot:" + str(index * 4),
                "evidence": "Recompute this exact raw intrafunction proof; no original object/base identity"})
    template = bytearray(body)
    for rel in relocations:
        word = struct.unpack_from("<I", template, rel["offset"])[0]
        struct.pack_into("<I", template, rel["offset"], word & 0xffff0000)
    rebuilt = reconstruct(bytes(template), address, relocations)
    require(rebuilt == body, "Unmasked local GP reconstruction differs")
    import relocation_identity as ri
    import elf_tools as elf_mapping
    return {"schema": 1, "kind": "intrafunction-local-gp-proof", "program": program,
        "address": address, "size": len(body), "raw_sha256": sha(body),
        "reference_sha256": reference_sha256, "mapped_sections_sha256": sha(encoded(sections)),
        "boundary_evidence": boundary_evidence, "verifier_sha256": sha(Path(__file__).read_bytes()),
        "cfg_decoder_sha256": sha(Path(ri.__file__).read_bytes()),
        "elf_mapping_decoder_sha256": sha(Path(elf_mapping.__file__).read_bytes()),
        "finite_MMI_decoder_receipt": mmi_receipt,
        "entry_GP_assumed": False, "call_GP_preservation_assumed": False,
        "control_flow_policy": "closed direct CFG; no calls, indirect transfers or reachable fallthrough escape",
        "reachable_instructions": len(incoming), "locally_known_GP": facts,
        "GP_memory": memory, "eligible_scoped_relocations": relocations,
        "certificate": {"exact": True, "template_sha256": sha(bytes(template)),
            "reconstructed_sha256": sha(rebuilt)},
        "limitations": ["Lower64 constant facts only; no upper GPR lane or global GP ABI proof.",
            "Normal instruction completion only; asynchronous interrupts and exception-handler effects are not proved.",
            "Supplied complete extent is not an original-source boundary proof.",
            "Mapped target is not original object/field identity; no C credit or consumer normalization is enabled."]}


def encoded(value):
    return (json.dumps(value, sort_keys=True, separators=(",", ":")) + "\n").encode()


def verify(proof, body, address, *, program, reference_sha256, sections, boundary_evidence):
    require(type(proof) is dict and type(address) is int and proof.get("address") == address,
            "Local proof address differs from independently selected raw extent")
    expected = prove(body, address, program=program, reference_sha256=reference_sha256,
                     sections=sections, boundary_evidence=boundary_evidence)
    require(encoded(proof) == encoded(expected), "Local proof differs from typed raw recomputation")
    return expected


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, required=True)
    parser.add_argument("--reference", type=Path, required=True)
    parser.add_argument("--extent", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args(argv); sys.path.insert(0, str(args.repo.resolve() / "scripts"))
    from elf_tools import read_elf, address_bytes
    extent = json.loads(args.extent.read_bytes())
    require(type(extent) is dict and set(extent) == set(HASH_FIELDS), "Unknown/missing extent field")
    require(type(extent["size"]) is int and extent["size"] > 0, "Invalid extent size")
    reference = args.reference.read_bytes()
    require(sha(reference) == extent["reference_sha256"], "Wrong reference file pin")
    elf = read_elf(args.reference)
    require(elf["type"] == 2, "Expected pinned reference ET_EXEC")
    sections = [{k: s[k] for k in ("name", "type", "flags", "address", "size")}
                for s in elf["sections"] if s["size"] and s["flags"] & 2
                and sum(seg["type"] == 1 and seg["address"] <= s["address"]
                        and s["address"] + s["size"] <= seg["address"] + seg["memsz"]
                        for seg in elf["segments"]) == 1]
    body = address_bytes(args.reference, extent["address"], extent["size"])
    require(sha(body) == extent["raw_sha256"], "Wrong complete body raw pin")
    proof = prove(body, extent["address"], program=extent["program"],
        reference_sha256=extent["reference_sha256"], sections=sections, boundary_evidence=extent["boundary_evidence"])
    if args.check:
        verify(json.loads(args.output.read_bytes()), body, extent["address"], program=extent["program"],
            reference_sha256=extent["reference_sha256"], sections=sections, boundary_evidence=extent["boundary_evidence"])
    else:
        require(not args.output.exists(), "Private output already exists")
        args.output.parent.mkdir(parents=True, exist_ok=True); args.output.write_bytes(encoded(proof))
    print(json.dumps({"mapped_GP_memory": sum(m["locally_proved_mapped_target"] for m in proof["GP_memory"]),
                      "reachable_instructions": proof["reachable_instructions"]}))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, TypeError) as error:
        print("Local GP proof failed: " + str(error)); raise SystemExit(2)
