# Twenty-seventh C lot: indexed resident fields and identifier selection

Three authored native bodies have unique complete occurrences in every pinned
overlay: two 52-byte indexed field getters and an 88-byte identifier selector.
All 81 function boundaries were checked in the live analysis project, and
each complete body was independently compiled and compared against its pin.
Two further call sequences are integrated only in Ship Shack.

| Ship Shack witness | Bytes | Behavior | Placements |
|---|---:|---|---:|
| `002ACF68` | 52 | returns indexed field `0x1220` when field `0x1244` equals two, else zero | 27 |
| `002ACFA0` | 52 | returns indexed field `0x1248` when field `0x1244` equals two, else minus one | 27 |
| `002ADBC0` | 88 | maps inputs one, two and three to `0x72`, `0x70` and `0x6e`, else zero | 27 |
| `002A5288` | 60 | passes a measured byte and constants to two callees | 1 |
| `002AECC0` | 44 | calls three measured no-argument functions in sequence | 1 |

The indexed functions use an 80-byte row displacement relative to the measured
resident anchor. Their declaration exposes only observed field offsets. The
byte at `0x0018b2bd` is accessed at offset one from the aligned external anchor
`0x0018b2bc`; the checker's alignment guard is unchanged. There is no claim of
a complete resident layout or an inferred game-system name.

## Integrated result

The three families contribute **(52 + 52 + 88) × 27 = 5,184 bytes**. The two
Ship Shack sequences contribute **104 bytes**: **83 placements / 5,288 bytes**.

| Measure | Before | After |
|---|---:|---:|
| Boot functions / bytes | 178 / 9,336 | unchanged |
| Native placements / bytes | 151 / 5,024 | 234 / 10,312 |
| Overlay placements / bytes | 4,251 / 207,136 | 4,334 / 212,424 |
| Integrated C bytes | 216,472 | **221,760** |
| Fraction of 48,788,176 executable bytes | 0.4437% | **0.4545%** |
| Ship Shack native bodies / bytes | 21 / 760 | 26 / 1,056 |

All 27 native units were requalified before one complete reconstruction pass.
The boot and all 27 overlays passed their loaded-byte and metadata gates.
The 176 repository tests passed and the independent exporter reproduced
221,760 matched bytes. The compiler, assembler and flags are unchanged.

## Rejected sources and qualification scope

The exploratory unit was initially rejected for an unaligned byte external.
After using the aligned anchor, an 88-byte candidate for the 80-byte
`002AD080` slot overlapped its neighbor and refused the union link. Its code
was not trimmed or patched. Complete isolated comparisons identified five
exact bodies and three mismatches. A fresh source containing only the five
winners then qualified all 26 Ship Shack native bodies together before
integration. Every earlier failure remains in the
[unique experiment register](C-NATIVE-EXPERIMENT-REGISTER.md).

Six further pair-store hypotheses produced 24 bytes against 20-byte targets;
the read-only audit of 49 direct calls provides no justification for inventing
pointer returns. Those targets remain parked. A subsequent larger-body batch
has one exact private 76-byte loop and seven mismatches; neither changes this
lot's integrated count. Its measured callee-return correction preserves all
26 current native controls.

Only authored C, structural identifiers and measurement metadata are public.
Raw analysis, game bytes, objects and ELFs remain private. Native PC gameplay
is unverified; the matching campaign and the complete-game objective continue.
