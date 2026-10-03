# Twenty-fifth C lot: two native families across 27 overlays

The newly authored selector and resident getter occur once each in every
pinned level overlay. Their complete function boundaries were checked in the
live RAC2 Ghidra project, their bytes checked against each pinned ELF, and the
authored C independently qualified in every level. They have no boot placement.
Five additional complete bodies were qualified only in Ship Shack.

| Ship Shack witness | Complete bytes | Measured behavior | Placements |
|---|---:|---|---:|
| `002AD0D0` | 20 | returns the input, or object byte `0xa9` when the input is 255 | 27 |
| `002D59A0` | 12 | returns the resident word at `0x0018c0b4` | 27 |
| `002D2878` | 28 | calls the measured float callee with the negated input | 1 |
| `002D5820` | 28 | calls the measured integer callee with zero | 1 |
| `002D5840` | 28 | calls the measured integer callee with three | 1 |
| `002D5A30` | 28 | tests the byte of the indexed 16-byte table entry | 1 |
| `002D6CF8` | 32 | sets object halfword `0x7e` and clears object byte `0x7d` | 1 |

This is **59 new placements / 1,008 C bytes**. Replication is established by
per-program evidence; equal instruction bytes alone do not establish a
function boundary. The getter's resident address belongs to the pinned boot's
`core.bss`; its declaration does not claim an overlay-owned initialized table.
The object reference view exposes only its measured pointer offset, `0x190`.
No complete object layout or game-system name is inferred.

The neighboring function boundaries include padding in several cases. The
20-, 12- and 28-byte sizes above are complete code spans, confirmed in Ghidra
and by the compiler's complete ELF symbols, rather than truncated candidates.
All unchanged padding remains in the reconstruction and in the loaded-byte gate.

## Integrated result

| Measure | Before | After |
|---|---:|---:|
| Boot functions / C bytes | 178 / 9,336 | unchanged |
| Native placements / C bytes | 58 / 2,896 | 117 / 3,904 |
| Overlay placements / C bytes | 4,158 / 205,008 | 4,217 / 206,016 |
| Total matched C bytes | 214,344 | **215,352** |
| Fraction of 48,788,176 executable bytes | 0.4393% | **0.4414%** |
| Ship Shack native bodies / bytes | 6 / 400 | 13 / 576 |

All 27 native units were requalified after their source and catalog changes.
The boot and all 27 complete overlays then passed their loaded-byte and
metadata gates. The 176 repository tests passed, and the independent exporter
reproduced 215,352 matched bytes. The compiler profile and assembler binaries
did not change.

## Preserved negative measurements and next lot

The [unique experiment register](C-NATIVE-EXPERIMENT-REGISTER.md) retains the
failed selective-store settings hypothesis, six mismatches from the first new
family, and four from the next family. No failed source is integrated.
Eight further complete native bodies have exact private proofs but remain
outside this lot's integrated count. Their first full-source trial failed on
duplicate typedefs; its source and log are retained separately from the
corrected run. Thirteen native controls remain exact in that corrected run.

All published content consists of authored C, structural identifiers and
proof metadata. The complete raw analysis, objects, linked ELFs and game
bytes remain private. These gates establish matching reconstruction; native
PC gameplay remains unverified and the 100% objective remains open.
