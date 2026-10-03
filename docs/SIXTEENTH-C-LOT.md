# Sixteenth RAC2 C lot: integer conversions and a packed RLE decoder

Three additional authored C bodies match the complete pinned retail symbols.
The compiler remains `dff08a34`; the assembler is the reproducible
`cda1a4e4` release with the restricted transfer-delay exemption documented in
[COMPILER-NOTES.md](COMPILER-NOTES.md). No assembly is embedded in the C.

| Symbol | Boot bytes | Qualified overlay placements |
| --- | ---: | ---: |
| `FUN_00336920` | 44 | 27 |
| `FUN_00336CC8` | 24 | 27 |
| `FUN_00297550` | 280 | 27 |
| **Added** | **348** | **81** |

The two small functions convert signed integer arguments to floating-point
values and store them through object fields. The previous assembler omitted
their first transfer-delay nop, producing 40 and 20 bytes respectively.
The restricted exemption preserves all 165 earlier bodies and produces both
complete new bodies, without changing compiler flags or the C backend.

The decoder reads packed colour/run records, handles half-byte alignment and
writes the requested runs through its argument pointers. Its final two-word
ordering difference was resolved by declaring the colour load before the run
load. This preserves the compiler's required lifetime and scheduling choice.
The decoder also matches under the previous assembler; its acceptance does
not depend on the transfer-delay patch. None of the three bodies references
absolute data or external functions. Each placement is separately qualified.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 165 / 7,780 | 168 / 8,128 |
| Overlay placements / bytes | 3,911 / 184,832 | 3,992 / 194,228 |
| Integrated C bytes | 192,612 | 202,356 |
| Executable-code proportion | 0.3948% | 0.4148% |

Validation: **168/168** complete-symbol matches, full loaded-byte gates for
the boot and **27/27 overlays**, **157 tests**, and a separate report export
reproducing **202,356 matched code bytes**. Two full toolchain builds from fresh
archive extractions produce identical tool hashes and independently pass the
168-body comparison and 89 assembler fixture cases. This qualifies the current
corpus, without recovering all historical ordering directives. The
[fifteenth lot](FIFTEENTH-C-LOT.md) remains intact; native gameplay is unverified.
