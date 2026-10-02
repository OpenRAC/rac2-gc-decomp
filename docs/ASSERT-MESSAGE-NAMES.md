# Assert-message carriers — 102 level functions named, and the last 59 messages closed

**Measured 2026-10-02/03 against the pinned retail image** (`config/target.json`) and the
9 September 2003 PAL prototype disc.

After the dispatch-table pass (`docs/MOBY-DISPATCH-TABLES.md`) one family of names was still
open: **59 assert messages** localized in the retail boot had **no carrying function**. No
instruction computes their address — not in the retail boot, not in the prototype boot. This
document records where they actually live, what was named, and what is deliberately **not**
named.

## Method

1. **Our own ISO9660 reader** and a level-archive extractor. The prototype disc's level table
   sits at `RC2.HDR + 0x5000 + i*0x3000` (level LBA at `+0x0004`, scene LBA at `+0x1804`),
   verified against the volume's real LBAs; each level is a chain of `(dest_addr, copy_size,
   section_type, entry_point)` records followed by their content, in seven sections:
   `.lit`, `.bss`, `.data`, `lvl.vtbl`, `lvl.camvtbl`, `lvl.sndvtbl`, `.text`.
   **27 levels, 57 070 528 bytes** extracted.
2. **32 of the 59 messages occur inside the retail level overlays.** Each one was resolved to
   the function that references it, using the retail overlays already loaded in the analysis
   project.
3. **Strict naming rule.** A function is renamed only when **both** hold: its current name is
   still generic (`FUN_…`) **and exactly one** message targets it. An existing name is never
   overwritten; a function targeted by several messages is left alone and listed, never guessed.

## Result

**102 functions named across 25 overlays** (the level overlays each carry their own copy of
the shared library, so a name lands in every level that has it):

| Name | Functions |
| --- | ---: |
| `DirectionalLightsOverflow` | 25 |
| `CameraCollPrimTestGridOutOfBounds` | 25 |
| `LevelDoorLimitExceeded` | 25 |
| `NpcShowMessageBadId` | 10 |
| `PathSetWtoDistNullPath` | 7 |
| `ThermanatorBallsNeededReport` | 5 |
| `PathDrawInvalidIndex` | 2 |
| `MusicPitchDecreased` | 2 |
| `CutsceneFinished` | 1 |
| | **102** |

The per-function list (program, address, name) is in `docs/assert-message-names.tsv`.
Persistence was verified rather than assumed: re-running the pass after saving reports the
functions as already named, not as renames.

**Cross-confirmation worth recording:** the Thermanator's two messages are referenced by
`UpdateMoby_3212` — a function named by the dispatch-table pass from the prototype's class
identifiers alone. Two independent methods (class identifier tables, assert strings) agree
that **class 3212 is the Thermanator**.

## The other 27 messages are IOP library strings — not EE code

The remaining 27 do occur on the disc, but in the **boot image's data sections**
(`core.data` / `core.rdata` in the prototype; the retail boot keeps them in its own data
sections). They are the assert strings of **IOP-side libraries** — `989snd`, `libcdvd`,
`libdma`, `libpad2`, `libdbc`/`libmc`, `SIF` — including the version banners
`PsIIlibcdvd 2530`, `PsIIlibdma 2500`, `PsIIlibpad2 2500`, sitting in a dense cluster of
`sceDbc*` / `SifDmaAddr` strings: a module's string table carried in the EE image to be
uploaded to the IOP.

Controls for that claim, all measured:

- **0 / 27** of them is the target of a `lui`/`addiu` pair in the retail boot's `.text`,
  and **0 / 27** in the prototype boot's `.text`.
- The scanner used for that was **controlled on known values**: of the 2 982 distinct
  addresses the prototype's `.text` builds with `lui`/`addiu`, it recovers the witness
  counts exactly (119/119, 115/115, 110/110).
- The reference analysis reports no cross-reference to them either.

Naming an EE function for these would be a category error: the code that references them
runs on the IOP, a different processor with its own address space.

## Limits

- A name here describes the function **that reports the assert**; it is not a claim about
  everything the function does.
- Functions targeted by **several** messages were left untouched — for example a group of
  four debug camera controls, the pair of sound-mismatch messages, and the Thermanator pair.
  They wait for a human decision rather than a guessed name.
- The level overlays were identified by identifier transfer (dispatch tables) and assert
  strings; no byte-level correspondence between prototype and retail level code is claimed.

## Provenance

- Prototype: 9 September 2003 PAL review disc (`SCES_516.07` boot), our own readers; the
  level-archive format was reverse-engineered and is documented in the project's working
  notes, not here.
- No bytes of either game image are reproduced in this document or in the TSV — only
  identifiers, addresses and structural names.
