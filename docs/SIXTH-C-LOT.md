# Sixth RAC2 C lot - 2026-10-02

One body, twenty bytes, and the first one this project has integrated that **counts in the
boot only**. `FUN_002AB650` reads `D_0018C0B0`, copies a 64-bit field out of the object it
points to and stores it into the argument. The boot gate reports **93/93 matched, zero
different bytes** against the pinned boot, and the measured counters move by exactly its
size:

| | Before | After |
| --- | ---: | ---: |
| Boot | 92 functions / 2 008 bytes | **93 / 2 028** |
| Levels | 2 091 placements / 48 156 bytes | 2 091 / 48 156 (unchanged) |
| Total | 50 164 bytes (0.1028 %) | **50 184 bytes (0.1029 %)** |

## Why it counts once

The level catalogue is built with an empty external map (`scripts/integration.py`), so the
link of a level overlay refuses a body that names a global: the symbol has nowhere to come
from. The reviewed level bodies are self-contained by construction, and this one is not. It
is therefore placed in the boot only, and its twenty bytes are counted once rather than
twenty-seven times.

That is the trade of this family: a **global-citing body** is the cheapest remaining family
to open — the boot catalogue already carries an `externals` map — but it pays once. The
measurement recorded in the previous lot stands: of 155 remaining targets, 19 cite a global
(1 436 bytes), 34 call another function (4 944 bytes), 15 are VU/MMI, 5 touch `$gp`, 6 read a
temporary register before writing it, and 76 are plain bodies (4 032 bytes, counted ×27).

## What the lot paid to learn

The level map is extended by byte search (`work/etend-catalogues-niveaux.py`), and that
search **does** find this body's bytes in the level overlays: the bytes match, the
relocation does not. Without a filter the couple is written, `config/level-catalog.json`
gains the symbol, and every level build would then fail its link on an undefined reference.

The filter is an explicit exclusion list, so a **new** global-citing body is not covered by
it until it is added — and the couples already written must be purged from the map, because
the exclusion only applies to the scan:

1. add the symbol to the exclusion list;
2. purge it from the level map (`work/catalogues-niveaux.json`);
3. regenerate `config/level-catalog.json`.

The first attempt at this lot's rebuild failed for a second, unrelated reason worth
recording: the level build refuses to start when the reviewed candidate record no longer
matches the boot source — `Integration inputs changed since candidate review`. The fresh
`check_candidates.py` report must be copied to `progress/candidates.json` **before** the
level rebuild, not after. Both traps cost one full rebuild each, and both are guards doing
their job rather than defects.

## Provenance

RAC2's own bytes, as in the fourth and fifth lots: no body here is ported from another game.
The five expectations in `tests/test_decomp_report.py` move with the lot (2 008 to 2 028,
92 to 93, 268 to 269), which is the repository's ritual: those tests read the real progress
files.
