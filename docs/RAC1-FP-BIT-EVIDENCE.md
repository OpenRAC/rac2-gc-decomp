# RAC1 evidence for the boot's fp-bit helpers

Recorded 5 October 2026 from rac1-decomp. **This is evidence for a hypothesis,
not a RAC2 match.** No RAC2 compiler trial was run for it; it adds no credit
and changes no proof input. The campaign task
`boot-fp-bit-verbatim-source-rac1-evidence-20261005` tracks it.

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

The sources are GPL v2 with the libgcc linking exception. They are referenced
here, not copied.

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
