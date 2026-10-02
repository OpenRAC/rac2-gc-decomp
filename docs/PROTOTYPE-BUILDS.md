# Prototype builds — which one to use, and for what

**Measured 2026-10-03 against the pinned retail boot** (`SCUS_972.68`, USA v1.01,
2 618 684 bytes, sha256 `36d5814d…`, `config/target.json`).

Three of the six known 2003 builds were obtained and measured. This document records what
each one is good for, and one result that inverts a natural assumption: **the earliest
build is the closest to the retail**, not the latest.

## What was measured

| Build | Boot | Boot size | Symbols | Level archives |
| --- | --- | ---: | --- | --- |
| **6 Aug 2003** (NTSC preview) | `SCUS_972.68` | 1 541 852 @ LBA 1107 | none | 51 files on disc |
| **7 Sep 2003** (NTSC review) | `SCUS_972.68` | 3 497 308 @ LBA 1173 | none | 97 files on disc |
| **9 Sep 2003** (PAL review) | `SCES_516.07` | 3 505 500 @ LBA 1173 | none | 97 files on disc |

The 8 August, 18 August and 4 September builds could not be obtained: the public file
links for them return 404, so nothing is claimed about them here.

**No build carries symbols.** All three boots have zero `.symtab` / `.mdebug` sections —
the shortcut that gave the community an authentic `MobyInstance` on a Deadlocked
prototype does not exist for Going Commando, on any disc we hold.

**The 7 and 9 September boots are the same code.** Their section tables are identical
(same entry point `0x1344b8`, same `core.*` sizes, same 52 `.DVP.overlay.*` sections), and
compared to the retail boot they produce *exactly* the same numbers (same count of
identical bytes, same first divergence, same longest identical run). The two discs differ
by 8 192 bytes of data, not by code.

## Distance to the retail

Identical bytes over the common window of each section, prototype vs retail:

| Section | 6 Aug | 7 Sep | 9 Sep |
| --- | ---: | ---: | ---: |
| `core.text` | **26.5 %** (longest run 2 687) | 12.1 % (786) | 12.1 % (786) |
| `core.data` | **93.8 %** | 91.4 % | — |
| `core.rdata` | **88.7 %** | 22.4 % | — |
| `core.lit` | 20.7 % | 15.9 % | 15.8 % |

The September builds add roughly 11 400 bytes to `core.text` over the 6 August size
(125 008 → 136 440, against 125 264 in the retail), and that insertion shifts everything
after it, which is why twice as much code stops matching. **For byte-crossing toward the
retail, the 6 August build is the better anchor by a factor of two.**

## A working data bridge

Using the 178 assert messages localized in the retail boot as anchors, and finding the
same strings in the 7 September boot, the deltas cluster instead of scattering:

| Sections (retail ← prototype) | Anchors | Dominant delta |
| --- | ---: | --- |
| `core.data` ← `core.data` | 4 | **−0x2B80** (4/4) |
| `core.rdata` ← `core.rdata` | 85 | **−0x32F0** (83/85) |
| other section pairs | — | scattered — no global correspondence |

An independent anchor family agrees on the first one: the 64-byte display block at retail
`0x00138040` sits at prototype `0x0013abc0`, which is exactly `+0x2B80`. So within
`core.data` and within the string block of `core.rdata`, a prototype address maps to the
retail by a constant that can be *computed*, not guessed. Outside those regions it does
not, and nothing is named from a projection.

## Which build to reach for

| Need | Build |
| --- | --- |
| Anchors for byte-crossing toward the retail | **6 Aug** (closest code and data) |
| Level archives and level content | 6 Aug or 7 Sep — both use the same level table and section format |
| Anything about the engine's module split | any — the two-module shape (CORE + FRONTEND) is the same in all three |
| What the community's `rc2_aug8_research` tooling targets | **6 Aug** — the boot offset and size declared in its patch file, and four patch sites it documents, all match this disc exactly |

## Limits

- Availability is not a claim about existence: the three unmeasured builds are published
  as pages, but their files are unreachable, so they are simply absent from this table.
- "Closest" here means identical bytes over the common window of a section. It is a
  proxy for usefulness as an anchor, not a statement about which build is more similar in
  behaviour.
- No bytes of any game image are reproduced here, and no download location is given —
  the builds are identified by date, serial and boot size only.
