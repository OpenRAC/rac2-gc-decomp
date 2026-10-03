# Seventeenth RAC2 C lot: three original call targets and two shared leaves

Five additional authored C bodies reproduce complete pinned retail symbols
under the unchanged `dff08a34` compiler and `cda1a4e4` assembler.

| Symbol | Boot bytes | Qualified overlay placements |
| --- | ---: | ---: |
| `FUN_00284000` | 108 | 0: boot only |
| `FUN_002B6D28` | 88 | 0: boot only |
| `FUN_00284E40` | 60 | 0: boot only |
| `FUN_002A7750` | 60 | 27 |
| `FUN_002B8888` | 100 | 27 |
| **Added** | **416** | **54** |

The three call targets access boot globals or measured absolute pointer cells.
They are deliberately excluded from overlay placement. The state caller uses
a local typed view of the existing byte-array declaration, preserving all
earlier bodies and declarations. The display wrapper loads its pointer through
the measured absolute cell. The GS wrapper requires a small-data store and
an absolute load from the same measured cell; its source preserves both forms.

An empty call delay slot does **not** prove that the call has no argument.
`FUN_002B6D28` forwards the already loaded state value to `FUN_00133400`.
The callee's argument use and the complete caller bytes independently constrain
this declaration. A source declaration that omits the argument does not match.

The two shared leaves update byte state and initialise object fields. Keeping
the two masks as separate memory updates avoids an incorrect merged mask.
The initialiser uses the measured store ordering, with its first write ordinary
and subsequent selected writes volatile. These are qualified authored C forms;
they do not recover the original source declarations.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 168 / 8,128 | 173 / 8,544 |
| Overlay placements / bytes | 3,992 / 194,228 | 4,046 / 198,548 |
| Integrated C bytes | 202,356 | 207,092 |
| Executable-code proportion | 0.4148% | 0.4245% |

Validation: **173/173** complete-symbol matches, full loaded-byte gates for
the boot and **27/27 overlays**, **157 tests**, and an independent report export
reproducing **207,092 matched code bytes**. The proofs record source, object,
catalogue and tool hashes. The [sixteenth lot](SIXTEENTH-C-LOT.md) remains intact;
native gameplay is unverified. Seven original call targets remain open.
