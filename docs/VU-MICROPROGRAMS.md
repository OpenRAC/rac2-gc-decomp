# VU microprograms in the retail boot — measured

**Measured 2026-10-03** on the pinned USA v1.01 boot (`SCUS_972.68`), then re-checked
on the 6 August, 7 September and 9 September 2003 prototype discs. Everything below
was read with our own ELF reader; no game bytes appear here and no download location
is given.

The overlay convention itself is documented by the community project
**re-rac/rerac** (`docs/formats/vu_microprograms.md`, ISC) for RAC1. Their renderer
attributions are a **lead to verify**, never evidence — the same rule as
COMMUNITY-ENGINE-REFERENCE.md. Section 4 separates what is measured here from what is
not.

The `.DVP.ovly*` tables are already pinned in `config/boot-sections.json`; this page
adds the payload layer they describe.

## 1. What the boot carries

The VU code area is the start of the first load (EE `0x00100080`). Four sections
describe it:

| Section | Size (bytes) | Content |
| --- | ---: | --- |
| `.DVP.overlay..*`, 50 sections | 85 720 total | **all zero** — placeholder sections emitted by the SN DVP overlay tooling |
| `.DVP.ovlystrtab` | 1 640 | the 50 section names, NUL-separated |
| `.DVP.ovlytab` | 600 | 50 records of 12 bytes |
| `.vutext` | 86 368 (0x15160) | **the payload** — 68 487 non-zero bytes |

Manually measured, the `.DVP.ovlytab` record is:

    { u32 name_offset_in_strtab; u32 ee_address; u32 vu1_address; }

Every `ee_address` lands inside `.vutext`; on the records sampled, the name's first
field equals `vu1_address` (entries 0–1 map `0x0`/`0x800`, the first two chunks of
program 104691, whose names read `0x0`/`0x800`).

Section name:

    .DVP.overlay..<vu_offset>.<program_id>.<line>.<chunk>

Grouping by `program_id` gives **50 chunks in 13 programs**. `vu_offset = chunk × 0x800`
on every chunk of every program **except** the VU0 patch `28259`, whose two chunks carry
`0xc80` and `0x0000` — consistent with VU0's smaller micro memory.

| program_id | chunks | role (community lead, RAC1 — see §4) |
| ---: | ---: | --- |
| 55907 | 8 | VU1 tfrag main (`TfragProc`) |
| 903379 | 4 | VU1 tfrag second / fallback strip list |
| 13507 | 5 | VU1 tie (`TieProc`) |
| 224979 | 8 | VU1 tie, second program |
| 56467 | 2 | VU1 shrub (`ShrubProc`, first list) |
| 912339 | 3 | VU1 shrub, second list |
| 13859 | 6 | VU1 moby renderer (resident id 6) |
| 57843 | 3 | VU1 textured sprite / billboard (resident id 7) |
| 221571 | 1 | VU1 particles (`PartProc`) |
| 104691 | 2 | VU0 program, moby-side helper |
| 436083 | 1 | VU0 program, end of frame / transitions |
| 28259 | 2 | VU0 patch, loaded once |
| **56883** | 5 | **no RAC1 counterpart** (measured in §3) |

## 2. Frozen from the first prototype to the shipped game

    sha256(.vutext)   = 14ed9dd0fac6d101c6832c48d69a337e62c687ba99e2773cfd2cd8c89af8c5c2
    sha256(.DVP.ovlytab) = 459622a6f64ed373bba93fe1561e60d92ab290383b8e2d18e63bd088a5944c56

Both hashes are **identical on the 6 August, 7 September and 9 September 2003 discs and
on USA v1.01**. The VU microcode and its upload table were frozen from the earliest
prototype we hold to the shipped game; the `.DVP.overlay..*` placeholder sections were
already known to be identical across the four (85 720 bytes, same hash).

## 3. Cross-game comparison (measured against a RAC1 boot)

The RAC1 boot we hold carries the same convention with **43 chunks in 12 programs**.
Chunk-for-chunk comparison between that boot and this game:

- all 12 RAC1 program IDs are present; **56883 is new**;
- program 13507 grows by one chunk (4 → 5) and 224979 by one (7 → 8); the others keep
  their chunk count;
- **21 of the 49 comparable chunks are byte-identical** — including **all 8 chunks of
  the tfrag program 55907**, both VU0-patch chunks (28259), all 3 of 912339, 3 of 4 of
  903379, 2 of 3 of 57843, 1 of 2 of 104691; none of 13507, 13859, 221571, 224979,
  436083 (none of the matches is a run of zeros — checked by byte density);
- the `<line>` field is identical between the two games for every program whose chunks
  did not change (e.g. 55907: 24, 306, 577, 866, 1145, 1416, 1686, 1956 on both sides)
  and shifted for the edited ones (13507, 13859, 224979) — measured evidence that the
  field is the **microcode source line**, and that exactly three programs were edited
  between the two games.

Consequence to keep: a byte-identical chunk means the community analysis of that
program applies to this game verbatim; it does **not** name anything here by itself
(repository rule: a name follows the code, never another port's name).

## 4. Measured vs lead — and open threads

Measured here: the section layout, the `ovlytab` record semantics, the name fields
(including the line-field evidence of §3), the 13 programs and their chunk counts, the
frozen hashes of §2, and the chunk-level RAC1 comparison of §3.

Lead, not measured: every role attribution in §1 is RAC1-derived community knowledge
(re-rac/rerac). Nothing in this repository reads that page; treat the roles as the
hypothesis to test the day one of these programs is traced from our own code.

Open threads:

- **No resolved cross-references** to the `.vutext` addresses appear in the boot's
  disassembly; the code that uploads these chunks is not yet located. Addresses are
  probably built at run time (or set up through DMA), the same pattern as the IOP
  library strings.
- Program **56883** (5 chunks) has no RAC1 counterpart; its role is unknown.
- The roles must be verified per program against our own code before any rename.
