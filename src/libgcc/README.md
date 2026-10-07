# libgcc sources vendored into the campaign

This directory contains runtime-library code derived from GCC, rather than
source authored by this campaign. Its own copyright and licence notices apply.
The repository's root MIT licence does not relicense this code, including the
copy generated inside `candidates/boot.c`.

## `fp-bit-ee.c` — GCC 2.95.3 soft float, EE configuration

| | |
| --- | --- |
| Pinned source | [`rac1-decomp`](https://github.com/OpenRAC/rac1-decomp/blob/cb22f0b0d3a171d1fd4b6851b86fe214a22c9822/src/libgcc/fp-bit.c), commit `cb22f0b0d3a171d1fd4b6851b86fe214a22c9822` |
| Pinned source SHA-256 | `3069e3a1385e9b2d316929a676a71e2af8a834666b7388a89f10c52ae0e17336` |
| Licence notice | GNU General Public License version 2 or, at your option, any later version; the original additional permissions and linking exception are retained in the source header |
| Licence text | [`COPYING`](COPYING), GNU GPL version 2, from [GNU](https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt) |
| Transform | One pass of the campaign's reconstructed 2.9-ee `cpp` |
| Preprocessor flags | `-P -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH -DNO_DENORMALS -DUS_SOFTWARE_GOFAST` |
| Symbol renamed in the published body | `__unpack_f` → `FUN_00123400`, 16 occurrences |
| Other symbols | `fptodp` and every other body retain their original names |

The pinned RAC1 source includes the changes documented in that project's
libgcc source. The campaign preprocesses that exact pinned file and renames
one symbol mechanically. The original FSF copyright, GPL version 2-or-later
notice, additional permissions, linking exception, warranty disclaimer and
author credits are retained before the preprocessed body, with only trailing
whitespace removed from the added comment lines. The header also records the campaign's preprocessing and rename, dated 5 October 2026.
The version 2 licence text accompanies this file in `COPYING`.

The original two-symbol private trial used two preprocessor aliases outside
its body: `__unpack_f` to `FUN_00123400`, and `fptodp` to `FUN_001234F0`.
The published file instead substitutes the unpacker name in the body and
retains the name `fptodp`. It therefore has a different source hash from that
trial. The final generated boot unit and its exact object are qualified
independently; the trial is evidence for the algorithm and the retained
wrapper refusal, rather than the source-hash authority for integration.

## Measured scope

`FUN_00123400` in `SCUS_972.68` is the complete 144-byte unpacker at
`0x00123400`. The qualified `-O2 -G0 -ffunction-sections` profile reproduced
its complete body exactly. The source-layout recipe concatenates this file
into `candidates/boot.c`; the stored qualification and full-image proofs bind
the exact generated source and object used for integration.

`fptodp`, corresponding to `FUN_001234F0`, remains refused: the initial
64-byte trial differs by eight bytes around the call to the unpacker, where
two instructions exchange the delay-slot position. It remains in the source
and receives no matching credit. See
[`docs/RAC1-FP-BIT-EVIDENCE.md`](../../docs/RAC1-FP-BIT-EVIDENCE.md) and
[`docs/COMPILER-NOTES.md`](../../docs/COMPILER-NOTES.md) for the measured result
and actual compiler identity.

The declaration, definition and fourteen internal call sites use the renamed
unpacker symbol because the campaign catalogues complete definitions under
`FUN_<address>` names. The unmatched wrapper retains its original name so it
is not mistaken for a catalogued definition by the source inventory.

## Licence boundary and generated copy

Keep the retained upstream notices and accompanying `COPYING` with this
source. The original header contains both the additional linking permissions
and special exception; those texts remain the authority for their scope.
They do not replace the licence on the source itself.

The source-layout generator copies this complete file, including its notices,
into the boot translation unit. That generated file contains the same GCC
portion and notices; its surrounding campaign code and the root MIT licence
do not relicense the copied portion. Changes to notices also change source
hashes, so qualification and integration proofs must be refreshed through the
maintained campaign workflow.
