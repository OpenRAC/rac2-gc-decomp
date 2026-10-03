# Tenth RAC2 C lot: scheduling dependencies expressed in C

Two further call-bearing bodies now reproduce complete retail symbols under the
unchanged reconstructed compiler, assembler and flags. Their accepted sources
use ordinary C; no instruction bytes, inline assembly or compiler-rule override
is introduced.

| Symbol | Boot bytes | Qualified overlay placements |
| --- | ---: | ---: |
| `FUN_002FC8D0` | 188 | 27 |
| `FUN_00350A78` | 244 | 27 |
| **Added** | **432** | **54** |

## Why the source form matters

`FUN_002FC8D0` first reached the right 188-byte size with a short cast preserving
the source expression's width. Two independent updates still appeared in the
opposite order. Private RTL diagnostics identified the scheduler's dependency
tie: the decrement feeds the loop branch as well as the call barrier, while the
pointer increment has a different successor set. Swapping two statements cannot
change that relationship. Moving the decrement into the iteration clause of a
`for` changes the compiler's scheduling graph and reproduces the complete symbol.
The normal compiler and default flags remain in use.

`FUN_00350A78` needs the sum in the parenthesized subexpression preserved before
the outer subtraction. Explicit wide casts followed by a signed 32-bit cast,
together with computing the free span before the circular position, reproduce
the retail expression order and register allocation. This is a source change
under the existing compiler profile, rather than an unqualified scheduling patch.

The private diagnostic compiler used to inspect RTL fixes an allocation-size
error in the dump path only. It is not the compiler that produced these public
proofs; all accepted bodies were re-gated under the normal `cc1` recorded in
[`COMPILER-NOTES.md`](COMPILER-NOTES.md).

## Verified progression

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 112 / 3,572 | 114 / 4,004 |
| Overlay placements / bytes | 2,615 / 84,932 | 2,669 / 96,596 |
| Integrated C bytes | 88,504 | 100,600 |
| Executable-code proportion | 0.1814% | 0.2062% |

The complete candidate gate passes **114/114**. The single reconstruction pass
validates the boot and **27/27 overlays** over their complete loaded bytes.
The **157 repository tests** pass with the measured counters, and a separate
manual export reproduces **100,600 matched code bytes**. Both new bodies have
unique measured locations at the overlays' own function entries and measured
callee addresses. All proofs use the same source, object and instrument hashes.

The previous [ninth lot](NINTH-C-LOT.md) remains intact. Other candidate sizes or
diagnostic compiler variants do not count as accepted reconstruction. Native
gameplay remains unverified, and the 0.5% campaign objective remains open.
