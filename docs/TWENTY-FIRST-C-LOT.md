# Twenty-first RAC2 C lot: two native code families across all overlays

Two bodies first qualified in `24_ship_shack` have one exact, aligned occurrence
in each of the 27 pinned retail overlays: the 24-byte five-field clear and the
72-byte indexed copy of an overlay-owned five-entry table.

Before creating new placements, each occurrence was checked against the named
analysis program: entry address, complete inclusive function boundary and every
body byte agree with the pinned ELF. The two older generic analysis program
paths were resolved explicitly, rather than relying on the active program.
All 54 function checks pass. For the table accessor, each of the 27 images has
one file-backed data owner for its referenced 20 bytes, matching that program's
analysis memory. The contents are not inferred from another image.

The 26 additional source files use each overlay's namespace and measured
function addresses. Each source/catalogue/object pair independently qualifies
under the unchanged `8bed6eae` compiler and `cda1a4e4` assembler. A byte search
alone was never an integration proof. No extracted body or table data is stored
in the repository; the sources are the two authored C algorithms.

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 177 / 9,328 | 177 / 9,328 |
| Overlay placements / bytes | 4,077 / 202,152 | 4,129 / 204,648 |
| Native functions / bytes | 4 / 256 | 56 / 2,752 |
| Integrated C bytes | 211,480 | 213,976 |
| Executable-code proportion | 0.4335% | 0.4386% |

Validation: 52 new complete-symbol matches, full loaded-byte gates for all 26
affected overlays and their boot reconstructions, the retained ship-shack gate,
176 tests and an independent export reproducing 213,976 matched code bytes.
The export attributes 2,752 bytes to native C and 201,896 overlay bytes to the
shared boot source. A new test checks the actual committed proof set and both
source categories without replacing the synthetic legacy compatibility tests.
The [twentieth lot](TWENTIETH-C-LOT.md) remains intact; native gameplay is
unverified.
