# RAC1 evidence for the boot's fp-bit helpers

Recorded 5 October 2026 from rac1-decomp, and **superseded later the same day by
the trial it called for**: `FUN_00123400` matched exactly and
`FUN_001234F0` did not. Read [What the RAC2 trial measured](#what-the-rac2-trial-measured-5-october-2026)
for the result; the sections below are the evidence as it was first recorded,
and remain the reason the trial was worth running.

## The RAC2 targets

Three boot functions sit together and have the sizes of fp-bit's single-float
helpers in RAC1, in the same order:

| RAC2 boot | Size | Register state | RAC1 `SCES_509.16` | Size | RAC1 result |
| --- | ---: | --- | --- | ---: | --- |
| `FUN_00123268` | 44 | integrated (`root-boot-make-double44-*`) | `__make_dp` at 0x120670 | 44 | exact |
| `FUN_00123400` | 144 | stopped (`root-boot-unpack-single144-*`) | `__unpack_f` at 0x1206B0 | 144 | exact |
| `FUN_001234F0` | 64 | stopped (`root-boot-fptodp64-*`) | `fptodp` (`__extendsfdf2`) at 0x120778 | 64 | exact |

RAC1 sizes were measured on the RAC1 executable as the extent through the
function's `jr ra` and delay slot. Equal size and order are a correspondence
hypothesis; they do not show equal bytes.

## What matched in RAC1

rac1-decomp builds these from GCC's own `fp-bit.c`, compiled **whole** by Sony's
gcc 2.9-ee-991111 through its driver (not `cc1`), exactly as Sony's `libgcc.a`
builds `fp-bit.o`:

```
-O2 -G2 -S -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH -DNO_DENORMALS -DUS_SOFTWARE_GOFAST
```

- `-DFLOAT` selects the single-float variant, which contains `__unpack_f` and
  `fptodp`. `__make_dp` comes from the double variant (`dp-bit.o`, same
  defines without `-DFLOAT`).
- `FLOAT_BIT_ORDER_MISMATCH` is the little-endian bitfield layout that GCC's
  MIPS makefile fragments select.
- `NO_DENORMALS` requires the one-hunk implementation in `unpack_d`/`unpack_f`
  that GCC trunk received on 2000-03-16 (Cygnus); rac1-decomp backports it,
  marked in the file. Without it `__unpack_f` does not match.
- `US_SOFTWARE_GOFAST` produces the GOFAST names (`fptodp`, `dpadd`, ...).
- These are fp-bit's own configuration macros, the ones Sony's library build
  defines. They select which source is compiled; they are not optimization or
  code-generation flags.

With those settings `__unpack_f` and `fptodp` matched RAC1 byte for byte with
relocations masked, and the whole fp-bit and libgcc2 region
(0x11DFE8-0x1206A0) has no differing word. Retail's linker dead-stripped the
other single-float functions; rac1-decomp removes their bodies from its
assembly afterwards to reproduce the layout. A per-symbol RAC2 trial does not
need that step, because the checker compares catalogued symbols only.

## Pinned sources

rac1-decomp `main` at `cb22f0b0d3a171d1fd4b6851b86fe214a22c9822`:

| File | SHA-256 |
| --- | --- |
| `src/libgcc/fp-bit.c` (GCC 2.95.3, two marked changes) | `3069e3a1385e9b2d316929a676a71e2af8a834666b7388a89f10c52ae0e17336` |
| `src/libgcc/README.md` | provenance and per-module results |
| `Makefile.sn`, lines 128-158 | the exact build lines |

The sources are GPL version 2 or later with their original libgcc linking
permissions and exception. The complete notices and [COPYING](../src/libgcc/COPYING)
accompany the vendored and generated copies. Until the trial ran,
they were referenced here and not copied; the trial's exact input is now
published under its own licence in [`src/libgcc/`](../src/libgcc/README.md).

## What the RAC2 trial measured, 5 October 2026

The trial this document called for was run. Its standalone unit is the `cpp`
output of the pinned `fp-bit.c`, with the two campaign symbol mappings
(`__unpack_f` → `FUN_00123400`, `fptodp` → `FUN_001234F0`) and the text between
the markers unedited. It was compiled once with the qualified
`-O2 -G0 -ffunction-sections` profile and both complete symbols were compared
against the retail boot image.

| Symbol | Retail size | Result |
| --- | ---: | --- |
| `FUN_00123400` | 144 | **exact**, every byte equal |
| `FUN_001234F0` | 64 | refused, 8 bytes differ |

The refusal is confined to delay-slot filling around the `jal` to
`__unpack_f`: retail stores `s.s $f12,16($sp)` before the call and keeps
`daddu a1,sp,zero` in the delay slot, while this chain emits the two in the
opposite order. Size and instruction set agree; only the ordering does not.
The qualified compiler in this trial is already the reconstructed GNU EE
`2.9-ee-991111b` (`cc1` SHA-256 prefix `8bed6eae`), as recorded in
[COMPILER-NOTES.md](COMPILER-NOTES.md). The SN directory supplies the linker;
it does not mean that this trial used the earlier SN GCC 2.95.3 compiler.
The remaining difference is evidence for a scheduling or compilation-context
question, not proof of a different retail compiler. A reopening requires a
specific independent compiler or context witness, with the unchanged source
and current exact controls retained; no source or flag permutations were run.

The refused body stays in the published source instead of being edited down to
the part that matched, because the campaign requires the published file to be
the exact input that produced the measured object.

## What a RAC2 trial would have to show

This repository's acceptance rules are unchanged:

- The campaign requires a standalone unit, so the trial source would be
  `fp-bit.c` preprocessed with the defines above and the EE target predefines
  (`__mips__`, `__R5900__`, ...) that the 2.9-ee driver passes. Record the
  preprocessor used, together with the unit's SHA-256.
- Compile it with the qualified GNU EE profile from `progress/candidates.json`
  (default `-O2 -G0`). rac1-decomp used `-G2`; fp-bit's single-float helpers
  may not reference small data, but that is a measured question.
- A fresh catalogue must cover each complete symbol (`FUN_00123400`, 144 bytes;
  `FUN_001234F0`, 64 bytes, with its callees bound as externals), and the
  complete loaded-byte gates still decide.
- A refusal is recorded like any other; it does not invalidate the RAC1 result.
