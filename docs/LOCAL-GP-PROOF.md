# Finite intrafunction local GP proof pilot

This private verifier starts every entry GPR unknown except the hardwired zero register. It does not accept a supplied global GP, `gp_verified` assertion or ABI preservation claim. A consumer must select the pinned complete raw extent independently and recompute its proof using the same reference, executable/data mappings and verifier/CFG decoder.

The finite transfer model tracks only exact lower64 constants through LUI (sign-extended32), ORI/ANDI/XORI, ADDIU (32-bit result sign extension), DADDIU (lower64 wrap), ADDU/DADDU/OR and SLL. Other supported memory/branch/jump instructions cannot manufacture constants; GPR loads destroy their destination fact. FPR and VU memory register numbers do not become GPR writes. Unrecognized instructions and MMI destroy all tracked facts, except two independently decoded finite forms: PMFHL.SH subopcode4 and PSRAH shift7. These retain only nondestination GPR facts; any destination, including GP, becomes unknown. All 1,056 admitted operand encodings are checked against the finite GPR-write contract through Rabbitizer 1.16.2/R5900 on the actual runtime binary. Its actual SHA256 is recorded and exact consumer recomputation binds that producing artifact. The independently observed Windows artifact is identified; no unmeasured Linux binary pin or general decoder qualification is claimed; different shift/subopcode/function forms remain unsupported. No general MMI preservation is assumed. Auxiliary HI/LO/VU state is never inferred. COP0/exception control and unsupported delay/link flow are refused rather than modeled as ordinary fallthrough.

Joins retain a value only when all reachable incoming states agree. The existing pinned CFG models delay slots and likely-branch annulment. An opaque call executes its slot using the pre-call GP and then kills ALL GPR facts on its return edge, including GP, SP and nominal saved registers. No interprocedural theorem or unconditional runtime register preservation is assumed. Unreachable instructions supply no facts. A bounded worklist must converge.

Output reports locally known GP before instructions and every GP-base integer/FPR/VU memory observation. CLI mapping sections must lie wholly inside one PT_LOAD, and code ownership is limited to named `.text`/`core.text` EE sections. A target is eligible only when the GP is an exact low pointer constant and the entire simple, aligned accessed width has a unique pinned allocated nonexecutable mapping. Partial LWL/LWR/SWL/SWR/LDL/LDR/SDL/SDR and unaligned accesses remain raw; implicit alignment/partial-byte behavior is not guessed. MMIO, overflow/non-low constants, unknown lifetimes and ambiguous/unmapped targets remain raw observations. Whole-body template reconstruction is exact and unmasked after only the narrowly eligible GP16 fields are restored. Original data allocations/field identity, original function boundaries and original module ownership remain unknown; there is zero C credit.

The producer CLI reads the actual ET_EXEC reference, derives mappings itself, checks whole reference and raw body SHA256, and refuses existing output. The consumer `verify` takes its independently selected address and raw body, not the proof's address as authority; it recomputes the entire deterministic proof and refuses extra fields or changed facts/tool/reference/raw pins.

```sh
python gp_local_proof.py --repo CURRENT_REPOSITORY --reference PRIVATE_PINNED_ELF \
  --extent PRIVATE_EXTENT.json --output FRESH_PRIVATE_PROOF.json
python gp_local_proof.py --repo CURRENT_REPOSITORY --reference PRIVATE_PINNED_ELF \
  --extent PRIVATE_EXTENT.json --output FRESH_PRIVATE_PROOF.json --check
```

Extent JSON has exactly `program`, `address`, `size`, `raw_sha256`, `reference_sha256` and `boundary_evidence`. The original complete extent and its evidence remain supplied extraction scope, not a newly recovered original boundary. The script performs no compilation/link/strip or source/normalizer/profile mutation.

Tests cover unknown entry/spoofed global metadata, constant definitions and sign extension, GPR versus FPR/VU writes, normal/indirect call slots, branch-likely bypass and divergent/equal joins, unreachable writes, unsupported/MMI kills, ambiguous/cross-boundary mappings, incomplete slots and REGIMM links, system/exception control, altered constants/register/opcode/raw/tool/reference facts and exact unmasked reconstruction. Real packet selection and any exact observed-MMI extension remain independently reviewed private evidence; no production consumer is enabled by this pilot.

The first selected real scratch packet needs full-body CFG analysis: the local LUI at +0x99C dominates the first MMIO access at +0x9B0, but later +0xA20 has incoming paths bypassing that definition. A linear window must not turn that later join into a positive GP fact. The generic synthetic known-GP load-to-GP read-then-kill case does not assert the real +0xA20 value.

This revision adds SDL/SDR to the eight excluded partial/merge accesses. Earlier frozen verifier/proof receipts remain archived unchanged and are superseded only by a fresh bounded actual proof generated under this new verifier hash. Named code-owner admission is exactly `.text` or `core.text`; synthetic executable sections are refused.

## Completed bounded retail pilot

The [metadata receipt](../progress/gp-local-pilot.json) covers the complete
5,060-byte scratch body in boot and its 27 overlay copies: 141,680 raw bytes.
Every body is independently analyzed from unknown entry GP and recomputed by
the typed consumer. At offset `0x9B0`, the local definition yields GP
`0x10010000` and effective address `0x1000D400`. This is MMIO outside allocated
EE data, so the original instruction remains raw. The entry and the unsafe
join at `0xA20` remain unknown, as does the earlier use at `0x244`.

All complete bodies reconstruct exactly. This pilot exports zero eligible GP16
fields and zero C credit. It does not modify the existing normalizer, global
GP flags, compiler profiles, source code or progress denominators. The original
partial-store eligibility defect for SDL/SDR and its corrected regression are
retained privately. Linux CI runs the asset-free finite decoder contracts with
the pinned package version; its platform-specific runtime artifact is recorded
and is not described as the previously observed Windows binary.

A useful next stage would prove entry GP for a small concrete data-accessing
leaf from a complete caller witness, then independently prove any necessary
callee preservation. This report does not provide that theorem or an original
data-object identity; a matched compilation environment is not entry evidence.

The subsequent [bounded caller investigation](GP-CALLER-EVIDENCE.md) records
why the inspected startup route cannot yet supply that theorem: two opaque
kernel boundaries precede its first direct call, whose target is not a GP-data
leaf. That negative does not enable a program-wide GP annotation.

The local pilot's facts are conditional on its supplied static intrafunction
CFG. Indirect transfers are modeled as exits; their dynamic targets and possible
reentry are not established by the pilot. It is not a runtime entry, return-link
or register-preservation certificate, and must not authorize a production GP16
consumer without those additional control-flow proofs.
