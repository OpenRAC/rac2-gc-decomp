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

## Detailed comparison

Based on reading the upstream tree at the reviewed revision. No address below
has been re-measured against `config/target.json`; "overlap" means a similar
purpose, not identical behavior.

### libgcc and libm

- Upstream `libgcc/MEMBERS` builds 20 archive members from GCC's own source:
  the `libgcc2.c` DI modules (`_divdi3`, `_muldi3`, `_fixunsdfdi`,
  `_floatdidf`, ...) and `fp-bit.c` modules (for example `_fpcmp_parts_df`),
  each selected by `FINE_GRAINED_LIBRARIES` and its `L_<module>` define.
  `LINK_ALIASES` binds names a member imports to game symbols without renaming
  the game function.
- This repository has the `fp-bit-ee.c` soft-float body (from rac1-decomp) and
  RAC1 findings that the same DI modules match there
  ([RAC1-DECOMP-FINDINGS.md](RAC1-DECOMP-FINDINGS.md)). Our register has the
  `gnu-libgcc-udivdi3-pure-c-first-20261006` task stopped after a 1504-byte
  result against 1488 required bytes. A per-module build as in their `MEMBERS`
  is a different approach to that open question, so it is worth checking
  against the stopped task's reopening condition, not restarting it.
- Upstream also carves two libm members (`s_isnan`, `w_sqrt`) from newlib and
  records their `.rodata`. We have no libm entry in the register or catalogue.
  Check whether v1.01 contains the same members before using this.

### Symbols

- Upstream `symbol_addrs/usa/symbol_addrs.txt` pins names, sizes and splat
  boundary fixes. It also records measured negatives, such as a name that was
  a wrong libc match. Those notes are the most reusable part: they show which
  libc/libm names fail on the same code shape.
- Addresses are v2.00. We found no evidence in this review that v1.01 shares
  them, so a name can move only when v1.01's code at the address does the same
  job.

### Tooling overlap

| Upstream tool | Purpose | Closest here |
| --- | --- | --- |
| `tools/ee/symaddrs_lint.py` | Runs splat's own parser on each symbol file in a fresh process | none; we do not use splat symbol files |
| `tools/ee/landing_gate.sh` | Runs flag-table, split and shadow checks before a landing | `scripts/campaign_finalize.py`, CI workflows |
| `tools/ee/flagdiff.py` | Checks that per-unit compiler flag tables agree across scripts | `scripts/compiler_profiles.py` |
| `tools/ee/overlay_bisect.sh` | Relinks excluding unit sets to find which body breaks boot | none |
| `tools/ee/unit_report.sh`, `objdiff.json` | Per-function match reports | `scripts/decomp_report.py`, `campaign.py diff` |
| `tools/native/` | Host build of the C bodies with HLE stubs and state-seeded tests | `ports/` (no unit-test harness) |

The upstream approach depends on splat and a Docker/wibo build of the EE
compiler, which this repository does not use, so only the ideas carry over. The
lint's rule of one process per file because splat's symbol table is
module-global is the kind of finding worth recording in
[COMPILER-NOTES.md](COMPILER-NOTES.md) if we adopt splat inputs.

### Not comparable

Upstream's 31 C/C++ units target v2.00 and EU, and it keeps unmatched functions
as `INCLUDE_ASM` with a `TARGET_NATIVE` body. Our bar counts only
compiler-proven bytes on v1.01, so none of their source can add progress here
without a v1.01 match.

## Function import candidates (boot)

Method: the function definitions with a C body in upstream's boot units
(`src/usa/cod/*`, 320 names) were compared by address with the functions
defined in `candidates/boot.c` (296 `FUN_`/`func_` definitions). Several
addresses coincide between the two projects (for example `0x122630` and
`0x1339F0`), so the boot layouts look close, but that is an observation and
not a proof for each address.

- 255 upstream `func_XXXXXXXX` definitions have no definition here.
- 158 of those are neither in `config/campaign-register.json` nor on the
  nonmatching shelf. These are the unclaimed leads.
- For 28 of them our `config/function-evidence/boot-bindings.json.gz` records
  a size, so a size check is possible. Smallest first (address, upstream unit,
  our size in bytes):

| Address | Unit | Size | Upstream shape |
| --- | --- | --- | --- |
| 0x00133220 | 0321A0 | 12 | store 1 to a flag, return it |
| 0x0011AA20, AA40, AAB0, AAD0, AB10, AC20, AC30, AC60, AEA0 | 015180 | 16 | EE kernel syscall stubs (`addiu $3, N; syscall`) |
| 0x0011F628 | 015180 | 24 | restore interrupts |
| 0x00133230 | 0321A0 | 32 | |
| 0x00133988 | 0321A0 | 36 | |
| 0x00126F38 | 022FA8 | 40 | libdma channel lookup, table of 10 entries |
| 0x00127630 | 022FA8 | 52 | `McOpen` wrapper that records error `0xB` |
| 0x0011F5E0 | 015180 | 72 | disable interrupts |
| 0x00131A98 | 0314C0 | 72 | |
| 0x0012F9C8 | 022FA8 | 80 | |

The remaining 18 sized candidates are 148 to 680 bytes (for example
`0x00123130`, `0x00124B88`, `0x0012F690`, `0x00122B00`).

Caveats before importing any of them:

- A size equal to ours is necessary, not sufficient. Upstream's own sizes
  (`symbol_addrs` `size:`) are given for only a few entries, so the size
  comparison is mostly one-sided.
- The syscall stubs are inline `__asm__` bodies. Check them against our
  rule that asm-only bodies earn no matching credit before listing them as
  candidates.
- Upstream compiles with `ee-gcc 2.9-ee-991111`; our matches use the
  qualified profile in [COMPILER-NOTES.md](COMPILER-NOTES.md). A body that
  matches there must be rebuilt and gated here.
- Several upstream bodies carry `TARGET_NATIVE` alternates and notes such as
  a "MATCHING WALL" on `func_0011B…` printf. Those are not matches.
- Reserve each small lot through [CONTRIBUTOR-RESERVATIONS.md](CONTRIBUTOR-RESERVATIONS.md)
  before any trial, and add a `CREDITS.md` row naming the upstream file and
  commit for every body used.
