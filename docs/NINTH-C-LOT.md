# Ninth RAC2 C lot: mixed saves and seven call-bearing bodies

The mixed floating-point save family was blocked by the order of the compiler's
save blocks. Moving the intact FPR block before the GPR block reproduces
`FUN_002A7878`. Restores keep GPR before FPR, and the register directions and
stack offsets are preserved. The shared base is initialized before either block.
`FUN_002A78D8` needs a separate result variable as well: reassigning its input
parameter keeps the result in a saved register and introduces an extra copy.
Both complete symbols now match, at 92 and 104 bytes respectively.

The compiler was rebuilt from the source archive and cumulative patch stack.
The clean and incremental builds produce identical `cc1` hashes, recorded in
[`COMPILER-NOTES.md`](COMPILER-NOTES.md). All 103 previously integrated bodies
remain exact. The assembler and preprocessor are unchanged. The public
[`reorder_save_blocks.py`](../scripts/compiler/reorder_save_blocks.py) applies
the same transformation to the already patched ascending-GPR source; applying
it produced the identical compiler source used for the clean reconstruction.

## Bodies

| Symbol | Boot bytes | Placement scope |
| --- | ---: | --- |
| `FUN_002A7878` | 92 | Boot and 27 overlays |
| `FUN_002A78D8` | 104 | Boot and 27 overlays |
| `FUN_0034F8C0` | 88 | Boot and 27 overlays |
| `FUN_0034F918` | 68 | Boot and 27 overlays |
| `FUN_00350B70` | 84 | Boot and 27 overlays |
| `FUN_00351E70` | 120 | Boot and 27 overlays |
| `FUN_003506B0` | 156 | Boot and 27 overlays |
| `FUN_00350798` | 108 | Boot only |
| `FUN_00350808` | 108 | Boot only |
| **Added** | **928** | **189 overlay placements** |

The two IPU bodies use absolute hardware addresses and are retained at the boot
under this campaign's placement policy. The other seven bodies were individually
byte-gated, then verified in the complete 112-body compilation. Their 189 overlay
placements were located uniquely, checked against each level's function entries,
and linked with measured callee addresses.

The additional source lessons are concrete: volatile stores preserve the field
order in `FUN_0034F918`; a signed 32-bit parameter preserves the sign extension
in `FUN_00350B70`; volatile counter/index accesses preserve the measured division
and update order in `FUN_00351E70`. The bodies contain no inline assembly or
embedded retail bytes.

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 103 / 2,644 | 112 / 3,572 |
| Overlay placements / bytes | 2,426 / 65,708 | 2,615 / 84,932 |
| Integrated C bytes | 68,352 | 88,504 |
| Executable-code proportion | 0.1401% | 0.1814% |

Validation: complete-symbol candidate gate **112/112**, full loaded-byte gates
for the boot and **27/27 overlays**, the repository's **157 tests**, and a manual
report export reproducing **88,504 matched code bytes**. All integration proofs
were rebuilt in one pass with the new compiler and the same source/catalogues.
These checks establish offline reconstruction; native gameplay is not verified.

## Remaining compiler and naming evidence

The candidate census still contains rejected bodies. A matching size is insufficient:
the remaining allocation, scheduling, loop padding and small-data differences stay
outside this lot. Only the exact nine bodies above count toward progression.

The wider transfer-delay census and its limits are documented in
[`COMPILER-NOTES.md`](COMPILER-NOTES.md). The new naming exports are separate
research evidence in [`ASSERT-MESSAGE-NAMES.md`](ASSERT-MESSAGE-NAMES.md) and
[`AUG6-RETAIL-ANCHORS.md`](AUG6-RETAIL-ANCHORS.md); they add no matched C bytes.
