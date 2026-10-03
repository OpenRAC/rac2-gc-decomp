# Twenty-sixth C lot: a quadratic family and seven Ship Shack bodies

The complete 36-byte `002E2A88` body evaluates `(b-a)*(t*t)+a*t`, then converts
the float result to a signed word. Authored C preserves the measured product
association and evaluation sequence. Its instructions occur once in each of
the 27 pinned overlays; every occurrence is also a complete, independently
verified Ghidra function. The boot has no occurrence.

Seven more complete native bodies are integrated only in Ship Shack:

| Function | Bytes | Measured behavior |
|---|---:|---|
| `002EF578` | 12 | returns the signed word at `0x001be840` |
| `002EF588` | 12 | returns the signed word at `0x001be888` |
| `002EF5E8` | 12 | writes the input word at `0x001be894` |
| `002F1B80` | 20 | tests whether the measured state word equals seven |
| `002E48B0` | 28 | invokes the measured no-argument callee |
| `002E53E0` | 28 | invokes the measured callee with zero |
| `002EBB68` | 36 | invokes the measured callee with `0x47` and `0x513f1` |

The callee instruction analysis supports the declared argument widths. No
descriptive game-system name or full object layout is inferred from these
small bodies. Function padding is retained in the reconstruction; the table
lists complete compiler symbols and verified instruction spans.

## Integrated result

The quadratic contributes **27 × 36 = 972 bytes**; the seven Ship Shack
bodies contribute **148 bytes**. The total gain is **34 placements / 1,120 C bytes**.

| Measure | Before | After |
|---|---:|---:|
| Boot functions / bytes | 178 / 9,336 | unchanged |
| Native placements / bytes | 117 / 3,904 | 151 / 5,024 |
| Overlay placements / bytes | 4,217 / 206,016 | 4,251 / 207,136 |
| Total integrated C bytes | 215,352 | **216,472** |
| Fraction of 48,788,176 executable bytes | 0.4414% | **0.4437%** |
| Ship Shack native bodies / bytes | 13 / 576 | 21 / 760 |

All 27 complete native source units were requalified. The boot and all 27
overlays passed their loaded-byte and metadata gates. The 176 repository tests
passed, and the independent exporter reproduced 216,472 matched bytes. The
qualified compiler, assembler and flags are unchanged.

## Evidence retained and campaign continuation

The first full-context source redeclared existing s64/u64 typedefs and failed
before producing an object. The corrected source reuses them and reproduces
all eight new bodies plus thirteen native controls. Both runs remain indexed
in the [unique experiment register](C-NATIVE-EXPERIMENT-REGISTER.md).

The subsequent larger-body lot has five exact isolated measurements totaling
296 bytes. Its initial catalog rejected an unaligned byte external; the
aligned anchor accesses the same byte at offset one without changing the
guard. One oversized candidate then blocked the union link. Separate
complete-body comparisons preserve the five exact results and three
mismatches, but those results are not part of this lot's integrated count.
They require a fresh accepted full source and full overlay gates before credit.

Only authored C, identifiers and proof metadata are published. Raw analysis,
game bytes, objects and ELFs remain private. Matching reconstruction does not
establish native PC gameplay. The campaign continues toward the existing
milestones and the complete-game objective.
