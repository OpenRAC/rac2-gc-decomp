# Eighteenth RAC2 C lot: four boot bodies and the first native overlay bodies

Four additional boot functions match the complete pinned retail symbols under
the reproducible `8bed6eae` compiler and unchanged `cda1a4e4` assembler.
Two independently reviewed functions belong to the ship-shack overlay and
are compiled from its own source, with names identifying that program.

| Symbol | Boot bytes | Overlay placements |
| --- | ---: | ---: |
| `FUN_00300280` | 124 | 27 |
| `FUN_002E5FE0` | 168 | 0: boot globals |
| `FUN_002E5F60` | 120 | 0: boot globals |
| `FUN_002B7170` | 372 | 0: boot globals |
| `LVL_24_SHIP_SHACK_FUN_002D6960` | 0 | 1 / 24 bytes |
| `LVL_24_SHIP_SHACK_FUN_002F5DE0` | 0 | 1 / 80 bytes |

The counted slot function covers 124 bytes, including the internal nop omitted
by the earlier 120-byte inventory. The clear loop uses architectural zero for
its 128-bit stores. The callback's local pointer views reproduce its temporary
address and register allocation. The wait function needs the generic frame
scheduler and the measured constant lifetimes and final store order. The
[compiler notes](COMPILER-NOTES.md) distinguish these source requirements from
the two general backend changes. No assembly is embedded in these C bodies.

The native clear function writes five 32-bit object fields. The native wrapper
calls its own overlay helper and sets object fields. That helper overwrites
the incoming argument registers before reading them: the void call is supported
by its code, rather than inferred from its empty call delay slot.

The new [native C workflow](LEVEL-NATIVE-C.md) preserves the existing shared
boot gate and adds a separate source, catalogue, object and review per overlay.
Its proof validates their union without overlap or duplicate credit, retains
both compiler and reconstruction instrument identities, and requires the full
loaded-byte gate. A boot proof cannot substitute for an overlay review.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 173 / 8,544 | 177 / 9,328 |
| Overlay placements / bytes | 4,046 / 198,548 | 4,075 / 202,000 |
| Integrated C bytes | 207,092 | 211,328 |
| Executable-code proportion | 0.4245% | 0.4332% |

Validation: **177/177** boot symbols, both complete native symbols, full
loaded-byte gates for the boot and **27/27 overlays**, **175 tests**, and an
independent export reproducing **211,328 matched code bytes**. The proof
attributes only 104 bytes to native C and retains the other 201,896 overlay
bytes as shared boot C. Two complete builds reproduce the compiler hash.
The [seventeenth lot](SEVENTEENTH-C-LOT.md) remains intact; four original call
targets remain open. Native gameplay is unverified.
