# Level dispatch tables — the prototype's class identifiers reach the retail

**Measured 2026-10-02 against the pinned retail overlays** (`config/overlays.json`,
27 levels). This document reports one transfer that had not been tested before: the
class identifier axis. The positional axis — same index in the prototype's table and in
the retail's — stays refuted, as measured on 2026-10-01.

## What the retail carries

Each level overlay ships three dispatch tables in its own sections, and none of them is a
flat pointer array:

| Section | Record | Words |
| --- | --- | ---: |
| `lvl.vtbl` | `oClass`, `UpdateMoby_<oClass>` handler, auxiliary pointer | 3 |
| `lvl.camvtbl` | camera id, `InitCamera`, `ActivateCamera`, `UpdateCamera`, `ExitCamera` | 5 |
| `lvl.sndvtbl` | sound id, `UpdateSound_<id>` | 2 |

The moby table ends on padding, the camera and sound tables on an `0xffffffff`
terminator. In `lvl.vtbl` the third word is the same value for every record of a level in
26 of the 27 levels; level 1 has exactly one record pointing elsewhere (`0x2a5d94`), so the
field is a usable pointer to a shared descriptor rather than padding — so far unused here.

Handlers live in the level's own `.text`. **5,598 of 5,788 handlers (96.7 %) are preceded
by a `jr ra` within the three words before them** — they are function entries, not
arbitrary addresses. The remaining 3.3 % show no such return before them in that window.

## Why an identifier transfers

Class identifiers are not positions. `UpdateMoby_<oClass>` numbering is the community's
convention for these tables, and the same numbering survives from the 8 August 2003
prototype to the retail USA v1.01.

The control that makes this evidence rather than an assumption is falsifiable and free:
in the prototype's extraction, several oClass values **share one address**, because the
linker merged identical functions (for instance `UpdateMoby_13` and `UpdateMoby_14`).
If the identifier really transfers, those oClass values must also share one retail
handler.

| Control, 27 levels | Result |
| --- | ---: |
| Pairs of oClass sharing one prototype address | **282** |
| … whose two retail handlers are the same function | **282 / 282** |
| Retail handlers shared by oClass with *different* prototype addresses | **1** (`0x03cfb00`, level 0) |

One exception in 27 levels is a retail linker merge, not a broken correspondence.

## Coverage

| Table | Records | Functions named |
| --- | ---: | ---: |
| Moby (`lvl.vtbl`) | **5 788** | **5 460** (94.3 %) — 4 843 distinct handlers |
| Camera (`lvl.camvtbl`) | 217 | **864** |
| Sound (`lvl.sndvtbl`) | 32 | **32** |
| | | **6 356 names proposed** |

The 328 unnamed moby records are class identifiers **absent from the 8 August build** —
added later. Their identifiers are read from the table; no name can be given to them from
this source. The retail camera identifier set (`0,3,4,5,6,7,17,19,20,23,24,25,26,27`)
contains the prototype's set exactly, plus `20`; camera transfer rests on that set equality
and on the record layout, not on the pair control above, which is moby-specific.

The data is in **`docs/moby-dispatch.tsv`** —
`table · level · id · role · address · aux · proposed_name · source` — with virtual addresses
as they appear in the overlay ELF. A tool that loads an overlay at file offset rather than at
its virtual address must translate first.

## What this is not

- **Not designer names.** `UpdateMoby_3032` states a moby class, not a creature. The only
  route to real names found so far is the assert-message pass on the prototype that named
  one hundred and six boot functions in this project.
- **Not boot code.** Crossing all 5,788 handlers with the boot bodies this project has
  mapped into the levels (82 per level in `config/level-catalog.json`, 288 in the wider
  prefix survey) gives **zero overlap**. The dispatch tables point at level-local code —
  which is also why the earlier "0 of 698" measurement looked like a dead end.
- **Not a placement.** Naming a level function does not put reviewed C into a level, and
  this table is not part of `config/level-catalog.json`.

## Provenance

The class identifiers come from **CreepNT's `rc2_aug8_research`**
(https://codeberg.org/CreepNT/rc2_aug8_research) — extracted tables of the 8 August 2003
prototype build, `extracted/sections_info/moby_symtab.csv`, `cam_symtab.csv`,
`snd_symtab.csv`. No game bytes are reproduced here: the table published alongside this
document holds identifiers, addresses and structural role names only. The retail side is
read from the same pinned overlays this repository already records by SHA-256, so every
row can be re-derived.
