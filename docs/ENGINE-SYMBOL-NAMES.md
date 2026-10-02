# Engine symbol names from the 8 August prototype — one transfers, ten do not

**Measured 2026-10-02 against the pinned retail boot** (`SCUS_972.68`, USA v1.01,
2 618 684 bytes, sha256 `36d5814d…`, `config/target.json`).

The CreepNT prototype-research repository carries a linker script,
`patches/cust_main_menu_src/frontend.ld`, that binds eleven engine symbols to fixed
addresses so that a replacement main menu can be linked against the 8 August 2003
prototype. Those addresses had never been tested against the retail image. This document
reports the per-symbol result: **one name transfers, ten do not — and the reason is
measurable, not mysterious.**

## The method: the name is never the evidence

Each symbol is judged on what the code at its address *must do*. `FlushCache` passes that
test. `sprintf` fails it, although a formatting routine of that name obviously exists
somewhere in the game — it is simply not at the address the linker script gives.

## The linker script is a map of the prototype, and it is self-consistent

The same repository carries the prototype's section table
(`extracted/sections_info/elf_secthdr.csv`), naming the two boot modules and their
sections. **Every address in `frontend.ld` falls inside the section its own comment
claims:**

| Symbol(s) | Address(es) | Prototype section | Prototype range |
| --- | --- | --- | --- |
| `sprintf`, `FlushCache` | `0x00115da0`, `0x0011aea0` | CORE `.text` | `0x00115200`–`0x00133a50` |
| `PAD`, `Settings` | `0x00138080`, `0x00139e18` | CORE `.data` | `0x00133a80`–`0x00139ec0` |
| `Level` | `0x001a81e8` | CORE `.lit` | `0x001a8000`–`0x001a82c0` |
| `mainmenu_selected_action`, `mainmenu_has_finished` | `0x001a856c`, `0x001a8954` | FRONTEND `.lit` | `0x001a8300`–`0x001abbb0` |
| `g_menus__cursor_index` | `0x001abc24` | FRONTEND `.bss` | `0x001abc00`–`0x001e6a20` |
| `guiDrawText2`, `printf`, `memcard_ResetGame`, the injection site | `0x001fbd10` … `0x001fc578` | FRONTEND `.text` | `0x001faf80`–`0x00260e10` |

So the script is a genuine map of the **prototype's own image**. Whether it is also a map
of the retail image is a separate question, and the answer is no.

## The two builds share a shape, not a layout

The retail USA boot has the same two-module structure and even the same section names —
the sections after `core.*` are the frontend module. The addresses differ:

| Section | Prototype (8 Aug) | Retail USA v1.01 |
| --- | --- | --- |
| CORE `.text` | `0x00115200`–`0x00133a50` | `0x00115200`–`0x00133b4f` |
| CORE `.data` | `0x00133a80`–`0x00139ec0` | `0x00133b80`–`0x0013a1bf` |
| CORE `.rdata` | `0x00139f00`–`0x0013c0a0` | `0x0013a200`–`0x0013c057` |
| CORE `.lit` | `0x001a8000`–`0x001a82c0` | `0x001a7480`–`0x001a7c17` |
| FRONTEND `.lit` | `0x001a8300`–`0x001abbb0` | `0x001a7c80`–`0x001b131b` |
| FRONTEND `.bss` | `0x001abc00`–`0x001e6a20` | `0x001b1380`–`0x00238c7f` |
| FRONTEND `.data` | `0x001e6a80`–`0x001fade0` | `0x00238c80`–`0x0026e82f` |
| FRONTEND `.text` | `0x001faf80`–`0x00260e10` | `0x0026ea00`–`0x00351fbf` |

Only addresses inside the **identical prefix of CORE `.text`** can transfer by identity,
because only that section starts at the same address in both builds. Exactly one of the
eleven names is such an address.

That two builds of the same engine place one object at two addresses is measurable without
any symbol at all: the 64-byte display/IPU block near the start of CORE `.data` occurs
**twice in each boot** — retail `0x00138040` and `0x001380d0`; 9 September PAL prototype
`0x0013abc0` and `0x0013ac50`. There is no single global shift between the builds: two
string anchors measured across them (a diagnostic assert and a resource path) move by
different amounts, section by section.

## Per-symbol result

| Symbol | Prototype address | What the retail boot holds there | Verdict |
| --- | --- | --- | --- |
| `FlushCache` | `0x0011aea0` | three SDK cache syscall stubs in a row | **transfers** |
| `sprintf` | `0x00115da0` | last two instructions of a function that starts at `0x00115d48` | no |
| `PAD` | `0x00138080` | a 32-byte block sent to the IPU by DMA (channel 4, `QWC=4`) from display setup | no |
| `Settings` | `0x00139e18` | zeros; no code references it, nor the two camera-flag neighbours the patch file names | unproven |
| `Level` | `0x001a81e8` | an initialised 16-bit table inside CORE `.lit`; no references | no |
| `mainmenu_has_finished` | `0x001a8954` | inside a data string in FRONTEND `.lit` | no |
| `mainmenu_selected_action` | `0x001a856c` | inside a resource-path string in FRONTEND `.lit` | no |
| `g_menus__cursor_index` | `0x001abc24` | a zero word in `.lit` next to format strings | no |
| `guiDrawText2` | `0x001fbd10` | zeros (`.bss`) — no instruction at all | no |
| `memcard_ResetGame` | `0x0021d0b8` | zeros (`.bss`) | no |
| `printf` (patch 3) | `0x001fb678` | uninitialised (`.bss`) | no |
| "Alpha/Preview Disk" (patch 2) | `0x001fc95c` | uninitialised (`.bss`); the string occurs **nowhere** in the retail boot | no |
| main menu code (injection site) | `0x001fbff8`–`0x001fc578` | uninitialised (`.bss`) — the whole window | no |

`FlushCache` is the only one that also had to be checked twice: retail `0x0011aea0` is the
first of three consecutive stubs that load a syscall number and issue `syscall`, and the
first number is 100 — the SDK's cache-flush call. The name is right, and it is right *at
that address*.

The retail frontend does exist, and it is where the section table says: its main loop —
loading data from `0x1800000`, the movie placeholder, music load, memory-card folder,
a seven-case menu-action switch — sits at `0x0026ede8`, inside retail FRONTEND `.text`.

## Consequences for this project

- **Do not apply these addresses, or any name derived from them, to the retail Ghidra
  project.** Every one indexes the prototype's image.
- The **6 356 level names** in `docs/MOBY-DISPATCH-TABLES.md` are unaffected: they come
  from the *overlay* dispatch tables and were verified against the retail overlays, not
  from this linker script.
- The prototype's section table is itself a usable result: it documents the engine's
  module split (a CORE module and a FRONTEND module, each with `.lit`/`.bss`/`.data` and,
  for the frontend, the three level dispatch tables), which is the shape the retail image
  keeps.

## Limits

- The 8 August prototype binary was **not** available to this project. The verdicts are
  what the *retail boot* holds at those addresses, plus the prototype's own section table
  (which is what makes the script self-consistent).
- Absence in the retail **boot** is not absence in the game. A name that fails here could
  still be correct for an overlay of the retail disc.
- A section-relative mapping between the two builds is a **lead, not a result**. Nothing
  in this project is named from it; a candidate would still have to be confirmed by the
  code at its own address.

## Provenance

- Symbol names and addresses: `codeberg.org/CreepNT/rc2_aug8_research` —
  `patches/cust_main_menu_src/frontend.ld`, `patches/_iso_patch.toml` (their comments are
  the source of the "Alpha/Preview Disk" and `movie()` patch sites).
- Prototype section table: same repository, `extracted/sections_info/elf_secthdr.csv`.
- Retail boot addresses: `config/target.json` pin, read by address. No bytes of either
  game image are reproduced here — only addresses, symbol names and structure.
