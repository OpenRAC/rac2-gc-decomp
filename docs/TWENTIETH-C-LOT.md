# Twentieth RAC2 C lot: an overlay-owned key/flags search

`LVL_24_SHIP_SHACK_FUN_0034F218` matches its complete 80-byte retail body
under the unchanged `8bed6eae` compiler and `cda1a4e4` assembler. It searches
five eight-byte rows, sets bit 4 of the matching row's flags and returns zero;
it returns one when no key matches.

The two external names represent interleaved fields at `0x001B1B70` and
`0x001B1B74` in this overlay's `.bss`. The source does not assert two separately
allocated arrays, define their contents or assume their runtime values. The
incomplete-array/SDA declarations and the explicit nonmatching `continue`
reproduce the measured address construction and branch layout; they do not
recover the original declarations. No GP-relative instruction is emitted.

Only the ship-shack native source, catalogue, review and integration proof
change. The boot and the other 26 overlays retain their valid proofs.

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 177 / 9,328 | 177 / 9,328 |
| Overlay placements / bytes | 4,076 / 202,072 | 4,077 / 202,152 |
| Native functions / bytes | 3 / 176 | 4 / 256 |
| Integrated C bytes | 211,400 | 211,480 |
| Executable-code proportion | 0.4333% | 0.4335% |

Validation: all four native complete symbols match, the affected overlay
passes its full 2,616,168-byte loaded gate, its boot reconstruction also passes,
175 tests succeed, and an independent export reproduces 211,480 matched code
bytes with four native units totalling 256 bytes. No table data is published.
The [nineteenth lot](NINETEENTH-C-LOT.md) remains intact; native gameplay is
unverified.
