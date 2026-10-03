# Thirteenth RAC2 C lot: instruction lengths and eight new bodies

The complete 252-byte `FUN_0028B950` body was blocked by one extra padding word.
Its division guard is one RTL node, but the R5900 machine description gives
it the length of its emitted branch/break sequence. The post-DBR loop counter
counted that node as one word and incorrectly padded a loop that was already
seven words long.

The fix uses the machine-description length for `TRAP_IF` in the existing
loop counter. It does not select a function by name or address, disable the
loop rule, or copy reference instructions. The public transformation is
[`count_trap_length.py`](../scripts/compiler/count_trap_length.py). The release
compiler was built in two directories with identical hashes; all 143 earlier
bodies remain exact. Diagnostic dump-buffer changes are absent from the
release recipe and active instruments.

| Symbol | Boot bytes | Qualified overlay placements |
| --- | ---: | ---: |
| `FUN_0028B950` | 252 | 27 |
| `FUN_0034FA30` | 180 | 27 |
| `FUN_002A8A50` | 160 | 27 |
| `FUN_003505B0` | 48 | 27 |
| `FUN_00335E88` | 40 | 27 |
| `FUN_002B3868` | 40 | 27 |
| `FUN_00335E68` | 32 | 27 |
| `FUN_00351FA0` | 32 | 27 |
| **Added** | **784** | **216** |

The other bodies preserve their measured branch layout, signed widths,
allocation, repeated field accesses and store order. `FUN_0034FA30` keeps its
budget and remaining-space calculations in the required order.
`FUN_002A8A50` expresses the two vertex accesses separately: the compiler shares
the division but retains both division guards, unlike the factored source.
The five smaller bodies were checked as complete symbols under normal flags.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 143 / 6,540 | 151 / 7,324 |
| Overlay placements / bytes | 3,317 / 151,352 | 3,533 / 172,520 |
| Integrated C bytes | 157,892 | 179,844 |
| Executable-code proportion | 0.3236% | 0.3686% |

Validation: **151/151** complete-symbol matches; full loaded-byte gates for the
boot and **27/27 overlays**; **157 tests**; a separate report export reproducing
**179,844 matched code bytes**. All proof source/object/catalogue/tool identities
were checked before finalization. The
[twelfth lot](TWELFTH-C-LOT.md) remains intact. Native gameplay is unverified;
the 0.5% campaign objective remains open.
