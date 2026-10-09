# Reserved boot square-root leaf: FUN_00282C10

This is a one-function analysis, with zero new matching credit. It records the
evidence needed to decide whether an ordinary-C trial is supported. The canonical
decision is task `thorpencodes-boot-282c10-vu-sqrt-analysis-20261009` in
`config/campaign-register.json`; shared reservation issue
[65](https://github.com/OpenRAC/rac2-gc-decomp/issues/65) supplies ownership.

## Pinned scope and observed ABI

- Program: `boot`, Going Commando USA v1.01, `SCUS_972.68`.
- Complete extent: `FUN_00282C10`, address `0x00282C10`, size 32 bytes.
- Executable SHA-256:
  `36d5814d8d95328d5839612ccdf7a2e7ecac0b6f868d3ad4bb2e98411f734b4a`.
- Complete body SHA-256:
  `411ed1f6289d501330ba4ac6ef32fdadc8f92620c39dd65cfc9c6ba30111cbe5`.
- Catalogue boundary: `flow_supported_inferred`; this does not certify its
  original source, signature or object boundary.

Read-only Ghidra inspection under `r5900:LE:32:default` agrees with the pinned
complete body. The leaf receives a single-precision input in F12, transfers its
bits to VU0, computes a vector-unit square root, waits for Q, moves the scalar
result back and returns it in F0 through the return delay slot. It has no stack
frame, helper call or memory access. A0 and vector-unit scratch state are changed.
The observed interface can be described as `float function(float value)`; its
original identifier and source-level declaration remain unknown.

Twelve direct-call references were sampled in Ghidra. Caller `0x002AA658`
prepares F12 and consumes F0 in a floating comparison. Caller `0x002AB3F8`
prepares F12 and copies F0 into a subsequent floating calculation. Caller
`0x0028CD44` passes a sum of squares and consumes F0 as a length. These observations
support the float input/output interface, not a proof of every caller or of
standard-library behavior for all values. Exceptional inputs, signed zero,
VU rounding and status flags have not been characterized at runtime.

## Compiler support gap

The locally retained source corresponding to the qualified GNU EE chain was
inspected without extracting, editing or rebuilding the archive:

| Source | SHA-256 | Relevant result |
| --- | --- | --- |
| `gcc/config/mips/mips.md` | `0b2ab4430f78635a3d0cc97036e753be18130b86bd695650f86327222855d18c` | `sqrtsf2`, lines 3332-3340, emits CPU scalar `sqrt.s` under its target predicate |
| `gcc/config/mips/mips.c` | `c76c0bec5b56c198381ab2a4fc60c161a4287e8312d7d1fdea3d1e6a0e1af614` | No textual vector square-root/transfer lowering found |

Neither examined file contains `vsqrt`, `vwaitq`, `vaddq`, `qmtc2` or `qmfc2`.
This bounds the investigation to the current backend; it is not a theorem that
no historical tool could generate the body. An ordinary scalar square-root
expression is not evidence for this exact vector-unit implementation, and its
exceptional-value semantics must not be assumed equivalent.

No C candidate or compiler trial was created. The research task is blocked
pending independently verified compiler/source evidence that supplies a
permitted ordinary-C lowering for the complete vector-unit leaf. Inline
assembly, encoded instructions, patched outputs and changing profile hashes
to admit unrelated tools are not reopening evidence. If such a route is found,
it still needs the actual complete-symbol and affected full-image gates.

## Contributor setup and baseline verification

On 9 October 2026, before this analysis:

- The full `doctor.py --contributor-check` passed against the authorized local
  ISO, BIOS, public dependencies and qualified GNU/SN instrument hashes.
- The prepared boot and all 27 overlay hashes were independently checked
  against the preparation manifest.
- Ghidra analysis access verified the pinned program and 16 R5900-specific
  instruction samples. Its mapped-memory check covered 1,526,287 bytes; another
  995,476 bytes were unmapped or uninitialized in that project. The selected
  complete body was checked separately by its hash above.
- PCSX2 v2.8.2 launched the local game in isolated settings with memory cards
  disabled, reported `SCUS-97268` / `1.01` and returned an eight-byte code sample
  equal to the pinned reference. Only the owned process was stopped. No hit or
  execution effect for this square-root leaf was observed.
- Fresh campaign action `07ba614c98134b96bd4e994b4ffd40c3` passed the boot and
  all 27 overlay loaded-image gates: 79,486,851 bytes compared. Its report
  SHA-256 is `cddff6fcf7fd2d0313a4d2c83c545af98e3a098d5438a198a5c564551052b2a4`.
- `python -m unittest discover -s tests` ran 818 tests successfully, with two
  skipped. Source inventory, campaign views and README progress checks passed.

These checks qualify the available environment and unchanged baseline. They do
not prove new C, reproduce this function from C or add physical/unique credit.
Private tool bindings, reference bytes, disassembly, emulator data and runtime
logs remain outside Git. The other four reserved functions were not developed.
