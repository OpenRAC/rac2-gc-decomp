# Fifteenth RAC2 C lot: three complete field-update functions

Three additional authored C bodies reproduce the pinned retail instructions
with the unchanged `dff08a34` compiler and `20c5f50b` assembler. The source
preserves the flag-dependent floating-point stores, signed counter stores,
and the observed return value of the minimum update.

| Symbol | Boot bytes | Qualified overlay placements |
| --- | ---: | ---: |
| `FUN_00349AB8` | 40 | 27 |
| `FUN_00349AE0` | 52 | 27 |
| `FUN_00350670` | 36 | 27 |
| **Added** | **128** | **81** |

The first two functions initialise related fields with opposite flag-dependent
floating-point values. The third stores the signed minimum and returns that
updated value. These bodies contain no absolute data references or external
calls. Placement is established independently in each overlay.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 162 / 7,652 | 165 / 7,780 |
| Overlay placements / bytes | 3,830 / 181,376 | 3,911 / 184,832 |
| Integrated C bytes | 189,028 | 192,612 |
| Executable-code proportion | 0.3874% | 0.3948% |

Validation: **165/165** complete-symbol matches, full loaded-byte gates for
the boot and **27/27 overlays**, **157 tests**, and an independent report export
reproducing **192,612 matched code bytes**. The proofs record the actual source,
object, catalogue and tool hashes. The [fourteenth lot](FOURTEENTH-C-LOT.md)
remains intact. Native gameplay is unverified.
