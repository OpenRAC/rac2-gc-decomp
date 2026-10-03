# Twelfth RAC2 C lot: parameter conventions and ordinary scalar candidates

Eleven more complete symbols reproduce the pinned retail bytes under the same
`158e5c20` compiler profile and normal flags. No new backend rule is introduced.

| Symbol | Boot bytes | Placement scope |
| --- | ---: | --- |
| `FUN_0026FEB8` | 56 | Boot + 27 overlays |
| `FUN_002802E0` | 56 | Boot + 27 overlays |
| `FUN_0029AE78` | 56 | Boot + 27 overlays |
| `FUN_002AB1F0` | 48 | Boot + 27 overlays |
| `FUN_002AB220` | 48 | Boot + 27 overlays |
| `FUN_002AD0B0` | 48 | Boot + 27 overlays |
| `FUN_00349498` | 52 | Boot + 27 overlays |
| `FUN_0034F960` | 208 | Boot + 27 overlays |
| `FUN_00350628` | 72 | Boot + 27 overlays |
| `FUN_002E59B0` | 72 | Boot only |
| `FUN_002E59F8` | 84 | Boot only |
| **Added** | **800** | **243 overlay placements** |

## Temporary register names do not establish a calling convention

The old candidate filter rejected functions that read `t0` through `t3` before
writing them. This alone does not make them internal blocks: GNU-EE EABI passes
its fifth through eighth integer arguments in those registers, and later
arguments can arrive on the stack. The compiler ABI definitions and 64 direct
call sites were checked against the private ELF for the reviewed family.

`FUN_0034F960` has a measured five-argument call; `FUN_002802E0` has measured
nine-argument calls. `FUN_0026FEB8` is a variadic stub: an empty authored C body
with its variadic declaration reproduces all the argument spills and return,
without embedding instructions or reading the variadic values. This is a
complete-symbol match, not a substituted logging implementation.

The remaining five-argument candidate is still rejected for code-generation
differences. Evidence for arguments neither guarantees a match nor justifies
inventing a parameter for every temporary-register read.

## Scalar forms that pass

The other new bodies preserve the necessary signed additions, condition grouping,
field access order and short-loop return paths in C. Three came from a private
survey incorrectly labelled `VU/MMI`: its mnemonic expression also matched scalar
arithmetic. Their raw instructions and resulting C were reviewed separately.
The two functions referencing `D_00188660` remain boot-only and contribute 156
bytes once. The self-contained 72-byte body is placed in all 27 overlays.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 132 / 5,740 | 143 / 6,540 |
| Overlay placements / bytes | 3,074 / 133,964 | 3,317 / 151,352 |
| Integrated C bytes | 139,704 | 157,892 |
| Executable-code proportion | 0.2863% | 0.3236% |

Validation: **143/143** complete-symbol candidate matches, full loaded-byte gates
for the boot and **27/27 overlays**, **157 tests**, and a separate manual export
reproducing **157,892 matched code bytes**. Source, object, catalogue and tool
identities are retained in the integration proofs. The
[eleventh lot](ELEVENTH-C-LOT.md) remains intact. Native gameplay is unverified;
the 0.5% campaign objective remains open.
