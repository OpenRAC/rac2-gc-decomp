# Nineteenth RAC2 C lot: an overlay-owned table accessor

`LVL_24_SHIP_SHACK_FUN_002F2510` matches its complete 72-byte retail body
under the unchanged `8bed6eae` compiler and `cda1a4e4` assembler. It copies five
32-bit entries from the measured table address into a local aggregate and
returns the indexed entry. The authored C contains no table values or assembly.

The source aggregate has four-byte alignment, which reproduces the measured
unaligned 64-bit copy operations and the final 32-bit copy. This is a qualified
source form, not recovery of the original type declaration.

The address `0x001A8EB0` belongs to the pinned overlay's `.lit` section. Its
20 bytes agree with the explicit overlay program in the analysis project and
differ from the boot image at the same address. Address equality alone therefore
does not establish data ownership. The linker alias names the overlay; the
data stays in the privately reconstructed original sections.

Only this overlay's native source, catalogue, review and integration proof
change. The boot and the other 26 overlays retain their valid proofs.

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 177 / 9,328 | 177 / 9,328 |
| Overlay placements / bytes | 4,075 / 202,000 | 4,076 / 202,072 |
| Native functions / bytes | 2 / 104 | 3 / 176 |
| Integrated C bytes | 211,328 | 211,400 |
| Executable-code proportion | 0.4332% | 0.4333% |

Validation: all three native complete symbols match, the affected overlay
passes its full 2,616,168-byte loaded gate, its boot reconstruction also passes,
175 tests succeed, and an independent report export reproduces 211,400 matched
code bytes. No table bytes are published. The
[eighteenth lot](EIGHTEENTH-C-LOT.md) remains intact; native gameplay is unverified.
