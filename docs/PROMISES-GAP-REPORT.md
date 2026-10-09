# Gap report: Promises/RC2-Going-Decompiled

Credit: this report compares against the public work of **Promises**
([RC2-Going-Decompiled](https://github.com/Promises/RC2-Going-Decompiled)),
revision `855deaff0eed040190198f83ed5fa7b871f52efb`. See [CREDITS.md](../CREDITS.md).

This is a reference page, never evidence. Nothing here can make a gate pass and
nothing was copied from the other repository. The upstream repository has no
`LICENSE` file, so any adoption needs the author's written permission first.

## Target differs

| | This repository | Promises |
| --- | --- | --- |
| Primary target | USA v1.01, `SCUS_972.68` | USA v2.00, `SCUS_972.68` |
| Second target | none | EU v1.00, `SCES_516.07` |
| Matching toolchain | qualified GNU EE compiler profile + ProDG linker | `ee-gcc 2.9-ee-991111` under wibo in Docker |

Their addresses, symbol names and C bodies are for different binaries. Treat
every item below as a lead to re-measure against our pinned hashes.

## What their tree has that ours does not

Counts are from the reviewed revision (31 `.c`/`.cpp` units, about 128k lines
in total with configs and symbols).

1. **Symbol map for v2.00.** `symbol_addrs/usa/symbol_addrs.txt` has about 2,040
   entries, many with measured sizes and notes. Possible use: candidate names
   for v1.01 functions with matching bytes. Each name must be verified by code
   at the v1.01 address, as in [ENGINE-SYMBOL-NAMES.md](ENGINE-SYMBOL-NAMES.md).
2. **libm members built from source.** `libm/` rebuilds `isnan` and `sqrt`
   (newlib fdlibm) instead of keeping assembly. Our boot has no recorded libm
   carve. Their upstream source for it carries no licence either.
3. **libgcc built from source.** `libgcc/` vendors GCC's `libgcc2.c`,
   `fp-bit.c` and `longlong.h` with a member table (`MEMBERS`) and `PROVENANCE`.
   We carry only `src/libgcc/fp-bit-ee.c`. The `MEMBERS` table is a method to
   check against our libgcc functions.
4. **Region cross-check.** An EU build used as a validator for the USA
   decomp. We have no second region.
5. **Tooling** under `tools/ee/` (about 90 scripts), for example asm fix-up
   sed/py passes, `landing_gate.sh`, `unit_report.sh`, `objdiff` reports,
   `symaddrs_lint.py`, `overlay_bisect.sh`. Many overlap `scripts/` here;
   none has been compared line by line.
6. **Native test harness** `tools/native/` (host build of the decompiled C,
   HLE stub checks, state-seeded tests). We have `ports/` but no equivalent.
7. **objdiff configuration** (`objdiff.json`) for per-function diffs.

## Suggested order

1. Ask Promises for permission and a licence (or an explicit grant) for
   anything to be adopted.
2. Pick one item, re-measure it against v1.01, and register it through the
   campaign workflow like any other lot.
3. Add a row to the [CREDITS.md](../CREDITS.md) table in the same change.
