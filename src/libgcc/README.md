# libgcc sources vendored into the campaign

This directory holds runtime-library sources that are **not** authored by this
campaign and are **not** under this repository's MIT licence. They are kept
here, clearly separated, because the campaign's matching rules require the
exact source that produced a measured body to be published alongside it.

## `fp-bit-ee.c` — GCC 2.95.3 soft float, EE build

| | |
| --- | --- |
| Upstream | [`rac1-decomp`](https://github.com/OpenRAC/rac1-decomp) `src/libgcc/fp-bit.c`, commit `cb22f0b0d3a171d1fd4b6851b86fe214a22c9822` |
| Upstream SHA-256 | `3069e3a1385e9b2d316929a676a71e2af8a834666b7388a89f10c52ae0e17336` |
| Licence | GNU General Public License v2, with the libgcc linking exception |
| Transform | one pass of the campaign's own reconstructed 2.9-ee `cpp` |
| Preprocessor flags | `-P -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH -DNO_DENORMALS -DUS_SOFTWARE_GOFAST` |
| Symbol renamed | `__unpack_f` → `FUN_00123400` (one name, 16 occurrences) |
| Symbols left alone | `fptodp` and every other body keep their original names |

The file is the `cpp` output, unedited except for that one symbol name. The
rename is mechanical and reversible, and changes no emitted byte; the reason
for it is that this campaign catalogues complete symbols under their
`FUN_<address>` names, so a body has to be published under the name it was
measured with. The source inventory in `progress/source-inventory.json`
recognises catalogued definitions by their `FUN_<hex>` spelling, so the
declaration, the definition and the fourteen internal call sites all carry the
renamed symbol.

Only the measured body is renamed. `fptodp` keeps its own name because it was
**not** matched, and a `FUN_<hex>` name on an uncatalogued body would make the
inventory disagree with the catalogue. Its source stays in the file rather than
being edited away, because the campaign requires the published file to be the
exact input that produced the measured object.

### What it is used for

`FUN_00123400` in `SCUS_972.68` (144 bytes, address `0x00123400`) is byte-equal
to `__unpack_f` from this file compiled with the campaign's qualified
`-O2 -G0 -ffunction-sections` profile. It is placed into the boot unit by the
recipe for `candidates/boot.c` in `config/source-layout.json`.

`fptodp` (the body at `FUN_001234F0`) is **not** matched: its compiled body
differs from the retail body by eight bytes, all of them delay-slot filling
around the `jal` to the unpacker. See
[`docs/RAC1-FP-BIT-EVIDENCE.md`](../../docs/RAC1-FP-BIT-EVIDENCE.md).

### Why this is vendored and not merely referenced

The campaign's acceptance path compiles one standalone translation unit and
requires the published source to be the exact input that produced the measured
object. A reference alone cannot satisfy that. Copying the file here, under its
own licence and with its own provenance, is the arrangement the maintainer
chose on 5 October 2026; it follows what `rac1-decomp` does in its own
`src/libgcc/`.

### Licence boundary

The libgcc linking exception permits this file's compiled output to be linked
into a program without that program falling under the GPL. **The exception does
not relicense the file**: `fp-bit-ee.c` remains GPL v2, and anyone
redistributing it must keep the licence and provenance header. The MIT licence
in this repository's [`LICENSE`](../../LICENSE) covers the rest of the
repository and never this file.
