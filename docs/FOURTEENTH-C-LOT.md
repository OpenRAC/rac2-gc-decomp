# Fourteenth RAC2 C lot: eleven small complete symbols

Eleven additional bodies reproduce the pinned retail bytes under the unchanged
`dff08a34` compiler, assembler and normal flags. The source preserves the actual
return conventions, repeated reads, branch layout and field-store order.

| Symbol | Boot bytes | Qualified overlay placements |
| --- | ---: | ---: |
| `FUN_002CB4E8` | 36 | 27 |
| `FUN_002ED688` | 36 | 27 |
| `FUN_00335E38` | 36 | 27 |
| `FUN_00335F88` | 36 | 27 |
| `FUN_00342BC0` | 36 | 27 |
| `FUN_003480F0` | 36 | 27 |
| `FUN_00350878` | 36 | 27 |
| `FUN_002A7798` | 28 | 27 |
| `FUN_00350590` | 24 | 27 |
| `FUN_00339760` | 12 | 27 |
| `FUN_003518C8` | 12 | 27 |
| **Added** | **328** | **297** |

The two 12-byte setters need a `void` source declaration. The earlier authored
return-value variants caused a second immediate load and failed the comparison;
the complete `void` bodies match. The zero stores in `FUN_00350590` retain their
measured order. No body is accepted on a prefix alone, and candidates with a
missing transfer-delay nop remain rejected under the current assembler.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 151 / 7,324 | 162 / 7,652 |
| Overlay placements / bytes | 3,533 / 172,520 | 3,830 / 181,376 |
| Integrated C bytes | 179,844 | 189,028 |
| Executable-code proportion | 0.3686% | 0.3874% |

Validation: **162/162** complete-symbol matches, full loaded-byte gates for the
boot and **27/27 overlays**, **157 tests**, and a separate manual report export
reproducing **189,028 matched code bytes**. The measured source/object/catalogue
and instrument hashes are preserved in the integration proofs. The
[thirteenth lot](THIRTEENTH-C-LOT.md) remains intact; native gameplay is unverified.

The private placement tool now compares against the executable section already
loaded in memory, avoiding an ELF reread for each byte. Before its use here,
the cached-window implementation reproduced the previous catalogue byte for
byte. The acceptance rule and range checks are unchanged.
