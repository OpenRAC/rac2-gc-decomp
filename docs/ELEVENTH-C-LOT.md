# Eleventh RAC2 C lot: scalar bodies, data views and polling loops

Eighteen complete C symbols were qualified together with the previously accepted
114 bodies. Fourteen are self-contained scalar routines, three reference boot
data, and one submits and waits for SIF DMA. All use ordinary C with measured
target-specific type annotations where needed, without embedded instructions or
retail byte sequences.

| Symbol | Boot bytes | Placement scope |
| --- | ---: | --- |
| `FUN_0027F128` | 80 | Boot + 27 overlays |
| `FUN_00288A00` | 156 | Boot + 27 overlays |
| `FUN_00288AA0` | 116 | Boot + 27 overlays |
| `FUN_00295BE8` | 100 | Boot + 27 overlays |
| `FUN_0029F978` | 120 | Boot + 27 overlays |
| `FUN_002C9A98` | 60 | Boot + 27 overlays |
| `FUN_002C9AD8` | 60 | Boot + 27 overlays |
| `FUN_002DE810` | 64 | Boot + 27 overlays |
| `FUN_00336D50` | 80 | Boot + 27 overlays |
| `FUN_00348068` | 108 | Boot + 27 overlays |
| `FUN_00349150` | 64 | Boot + 27 overlays |
| `FUN_00349630` | 60 | Boot + 27 overlays |
| `FUN_00349918` | 80 | Boot + 27 overlays |
| `FUN_00350750` | 68 | Boot + 27 overlays |
| `FUN_0034FB20` | 168 | Boot + 27 overlays |
| `FUN_00285708` | 128 | Boot only |
| `FUN_002B7C10` | 112 | Boot only |
| `FUN_002B7D88` | 112 | Boot only |
| **Added** | **1,736** | **405 overlay placements** |

## What opened the cases

The scalar routines need their actual 32-bit parameter widths, address/index
evaluation order, return values and store order preserved. A `long` suggested by
the decompiler can introduce wide shifts absent from the reference; a pointer
return can decide the allocation even when callers ignore it. Each correction
was accepted by its compiled complete symbol, not by a pseudocode resemblance.

The three data-dependent functions remain at the boot under the campaign's
placement policy. Incomplete arrays with the `sda` annotation suppress compiler
address splitting while the assembler emits the measured absolute accesses;
one scalar declaration emits the measured GP store. This is a qualification of
these authored source forms, not evidence of the original data-section layout.
The boot GP value was checked independently in the ELF metadata, its startup
initialization and the instruction offsets; the catalogue value is consistent.

The polling loops in `FUN_0034FB20` exposed a tool mismatch in the cumulative
compiler patch stack: a post-delay-slot padding hook had been disabled assuming
that Ps2EeAs would pad the loops, but this C pipeline uses GNU `as`. Restoring
the existing hook produces the required short-loop padding. A local `void`
call view preserves the shared value-returning declaration and the correct
allocation. The new release compiler was rebuilt in two directories with the
same hash, and all 114 previous bodies still match. The new rule and its public
patch are in [`COMPILER-NOTES.md`](COMPILER-NOTES.md).

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 114 / 4,004 | 132 / 5,740 |
| Overlay placements / bytes | 2,669 / 96,596 | 3,074 / 133,964 |
| Integrated C bytes | 100,600 | 139,704 |
| Executable-code proportion | 0.2062% | 0.2863% |

Validation: **132/132** complete-symbol matches, full loaded-byte gates for the
boot and **27/27 overlays**, **157 tests**, and a separate manual export of
**139,704 matched code bytes**. All 28 integration proofs share the reviewed
source/object hashes and the recorded instruments. Source, catalogue and proof
text use LF. Native gameplay and the 0.5% objective remain unverified/open.

## Candidate filters are diagnostics

The private survey had also labelled 15 candidates as `VU/MMI` because its
mnemonic expression included scalar `mult`, `div`, `mfhi` and `mflo` operations.
A raw-instruction review found no COP2 or packed-SIMD arithmetic in those 15
windows; one uses quadword moves. This reclassification opens candidates for
ordinary C review, but contributes no progress by itself. Future bodies enter
only after the same full-symbol and integration gates used here.
