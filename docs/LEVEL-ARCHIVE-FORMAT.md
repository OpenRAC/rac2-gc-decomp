# Level archive format — measured, not inherited

**Measured 2026-10-02/03 on the 9 September 2003 PAL review disc**, then re-checked on the
6 August and 7 September discs. Everything below was read with our own readers and
**verified against the volume's real LBAs**; where an existing community note describes a
different layout, the measurement wins and the difference is stated.

No bytes of any game image appear here, and no download location is given: this is the
container format only.

## 1. The volume

Plain ISO9660: primary volume descriptor at sector 16 (`CD001` at offset 1), root
directory record at offset 156 of that descriptor. The disc carries 97 files, of which the
ones that matter here are:

| File | Where |
| --- | --- |
| `SYSTEM.CNF` | LBA 1000 — names the boot, and its `VMODE` |
| the boot ELF | LBA 1173 on the September discs, **LBA 1107 on the 6 August disc** |
| `RC2.HDR` | **LBA 1001** — the level table |
| `G/LEVELn.WAD`, `G/SCENEn.WAD` | one pair per level |

## 2. The level table

`RC2.HDR` is the table file. **This build's layout is not the one an existing community
note describes** (`TOC_LBA = 1001` then 64 entries of 24 bytes at `+0x4888` — that does
not hold here). Measured layout:

    entry of level i  = RC2.HDR + 0x5000 + i * 0x3000
        +0x0004 : LBA of G/LEVELi.WAD
        +0x1804 : LBA of G/SCENEi.WAD

Verified against the real ISO9660 LBAs of both files, for levels 0 to 5. 27 levels are
present; the table has room for 64 and the remaining entries are zero.

## 3. A level archive

`G/LEVELn.WAD` opens with 24 32-bit words: a header size (`0x60` on every level), a word
that is zero everywhere, the level id at `0x08` (equal to the level number on all 27),
a word at `0x0c` (the generation's `reverb`: 0 on 21 levels, 3 or 4 on the other six),
then up to ten (offset, size) couples **in sectors, relative to the start of the file**:
four at `0x10` (`data` first), then six more at `0x30` — the generation's chunk-file
ranges. Measured on all 27 levels of the 9 September disc: the four base couples chain
exactly when sorted by offset (`offset + size == next offset`, 27/27) and partition the
file after the first sector — level 0, for example: `{1, 690}`, `{691, 6467}`,
`{7158, 311}`, `{7469, 12}`, in a 15 319 728-byte file. The second couple is the level
data WAD (below); the region at `{1, N}` is raw and its structure is not yet identified;
the last couple is a small uncompressed blob. The six chunk couples are zero on 19
levels and chain in slot order on the other 8 (levels 1, 2, 4, 7, 8, 11, 19, 20 — level
20 populates all six); the community layout for this generation carries them in the
level header's `ChunkWadHeader` (three chunk files and three chunk sound banks). The
same layout labels the last three base couples gameplay (NTSC, PAL) and occlusion — the
`0x20` couple is a `"WAD"`-compressed stream on 27/27 levels, and the mapping is
otherwise kept as a lead.

**The offset of the level data is not assumed.** The extractor tries every word as an
offset — in sectors, then in bytes — and keeps the one that makes a valid section list
appear. On the 27 levels of the 9 September disc it is always the same position, and the
value differs per level.

At `level_start + offset * 0x800` sits a header of twelve 8-byte (offset, size) slots, in
bytes, relative to the header. The overlay slot is **index 0, at +0x80, on all 27 levels**
and slots `0x50`/`0x58` are empty; slots 4 to 9 begin with the `"WAD"` LZ container on
27/27 levels (five small ones, and the large compressed core-data stream — 9 767 866
bytes at `0x48` on level 0). The community layout for this generation describes an
eleven-slot (`0x58`) header whose last slot is `transition_textures`; the twelfth slot
read here is the start of the padding before the overlay. At `that header +
ofs_overlay.offset` begins the section list:

    record = dest_addr (u32), copy_size (u32), section_type (u32), entry_point (u32)
    followed by copy_size bytes of content ; the next record follows immediately.

Section types seen: `1` = PROGBITS, `8` = NOBITS, `9` = REL. **The stop criterion is
documented and then measured:** every section of one overlay repeats the same
`entry_point` (the address of `startlevel`), and the list ends **when it changes**. Each
of the 27 levels carries exactly one distinct value. Plausibility checks (destination
window, known type, non-null size) remain as guards, not as the stop rule — an earlier
size threshold was silently truncating the list at a small section, cutting ten levels
before their `.text`.

Seven sections per level, in this order (addresses from level 0 of the 9 September disc):

| # | destination | size | type | role |
| ---: | --- | ---: | ---: | --- |
| 0 | `0x001aae80` | `0xa9e8` | 1 | `.lit` — **the same address in all 27 levels** |
| 1 | `0x001b5880` | `0x80860` | 8 | `.bss` |
| 2 | `0x00236100` | `0x4fa78` | 1 | `.data` |
| 3 | `0x00285b80` | `0xb08` | 1 | `lvl.vtbl` |
| 4 | `0x00286700` | `0xb4` | 1 | `lvl.camvtbl` |
| 5 | `0x00286800` | `0x20` | 1 | `lvl.sndvtbl` |
| 6 | `0x00286880` | `0x1b3cf0` | 1 | `.text` |

**27 levels, 75 369 728 bytes** of section content in total, all 27 carrying seven
sections (a first measurement of 57 070 528 bytes came from the truncating threshold
above; the corpus grew by 18 299 200 bytes when the documented rule replaced it, and
re-extracting reproduces it byte for byte — 190 files, 0 differing).

The same reader parses the 6 August and 7 September discs without a single change: same
table formula, same section addresses, seven sections per level.

## 4. Traps, paid for once each

- **The name of a section can lie about its content.** In the disc we started from, one
  section's declared type is NOBITS while the file does carry bytes for it; the loader
  follows the segments, not the section table.
- **The destination window must extend well past `0x00280000`.** Stopping there cuts the
  list after `.data` and makes every level look like it has three sections.
- **An acceptance threshold of "at least four sections" rejects valid three-section
  lists.** The robust criterion is: first section PROGBITS, and at least one large one.
- **A file offset is not a virtual address.** Offsets returned by a raw scan of the ISO
  must be converted before being compared to anything: for the September discs,
  `vaddr = 0x00100080 + (offset_iso − 1173*0x800 − 0x1000)`. Comparing the two directly
  gives a silent "nothing found".
- **`e_phoff` of an ELF32 lives at `0x1C`, not `0x20`.** Reading it at `0x20` yields
  `e_shoff`, an empty program-header table, and empty reads with no error at all.

## 5. What this is for

The archives hold the per-level dispatch tables (`lvl.vtbl`, `lvl.camvtbl`, `lvl.sndvtbl`)
and the level code. Reading them is what allows a prototype's level content to be compared
with the retail's, and it is how the assert messages that the boot never references were
traced to the functions that carry them.
