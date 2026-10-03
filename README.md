<p align="center">
  <img src="assets/rac2-logo.png" alt="RAC2 — Going Commando" width="560">
</p>
<p align="center"><strong>Ratchet &amp; Clank: Going Commando</strong></p>

<p align="center">
  <a href="progress/report.json"><img src="https://img.shields.io/badge/Progress-Measured-e89b35?style=flat-square&amp;labelColor=0d1117" alt="Measured progress"></a>
  <a href="#prepare-locally"><img src="https://img.shields.io/badge/Build-Guide-c3cbd8?style=flat-square&amp;logo=gnubash&amp;logoColor=c3cbd8&amp;labelColor=0d1117" alt="Build guide"></a>
  <a href="#supported-version"><img src="https://img.shields.io/badge/PS2-USA_v1.01-c3cbd8?style=flat-square&amp;labelColor=0d1117" alt="PlayStation 2 USA version 1.01"></a>
  <a href="https://openrac.dev/"><img src="https://img.shields.io/badge/OpenRAC-tracked-e89b35?style=flat-square&amp;labelColor=0d1117" alt="Tracked on OpenRAC"></a>
</p>

<p align="center">
  A work-in-progress, byte-matching decompilation of Going Commando for PlayStation 2.<br>
  Recovering readable C/C++ from the original game, with a native PC port as the long-term goal.
</p>

> [!NOTE]
> This project uses AI-assisted research, coding, and tooling under human direction.
> Architecture and priorities remain human decisions. Matching claims require compiler-produced
> code to pass byte-for-byte comparisons against the targeted game executable;
> an AI-generated answer alone is not evidence of correctness.

> [!WARNING]
> This is an early decompilation and reconstruction project, not a playable PC port.
> No game assets, disc images, rebuilt executables, or proprietary toolchains are distributed.
> You must supply your own legally obtained copy of the supported release.

## Supported version

| Game | Platform | Region | Version | Boot executable |
| --- | --- | --- | --- | --- |
| Ratchet & Clank: Going Commando (2003) | PlayStation 2 | USA / NTSC-U | 1.01 | `SCUS_972.68` |

Greatest Hits v2.00 and other regions are different targets.
This repository is part of project 45.

## Current status

Assembly reconstruction and matching C/C++ are tracked separately.
See [the measured progress report](progress/report.json) and
[the C integration proof](progress/integration.json) for their respective results.
The native runtime remains a separate development milestone.

Verified on **2026-10-02**: the complete boot image (**2,521,763 loaded bytes,
two PT_LOAD segments**) and **all 27 level overlays** rebuild identically in loaded
memory. **157 tool tests pass**. **Ninety-two C functions (2,008 bytes) are integrated in the boot, and the same reviewed C is integrated in all 27 level overlays (2,091 placements, 48,156 bytes)**
using genuine compiler-produced objects, with the complete boot matching both
original loaded segments. The general compiler profile and native runtime
remain to be established.

## Requirements

- Python 3.12 and the pinned dependencies in `requirements.txt`.
- A local image of your own matching game disc (or a local archive of that image).
- Wrench `wrenchbuild` for unpacking level executables.
- Windows and a locally supplied **SN ProDG 2.0** EE toolchain containing `ee/bin/Ps2EeAs.exe`
  and `ee/bin/ld.exe`, plus the **SN ProDG 3.01** C toolchain (`bin/ee-gcc2953.exe`,
  `bin/ee-as.exe`, `lib/gcc-lib/ee/2.95.3/cc1.exe`) to prove matching C.
  No SDK is supplied or downloaded by these scripts.

The target's size and disc hashes are pinned in `config/target.json` against
[Redump disc 13103](https://redump.info/disc/13103). Boot identity is measured
locally. Greatest Hits v2.00 and other regions are different targets.

## Prepare locally

Use a short runtime directory **outside** this repository. Each preparation and
build creates a new directory, preserving prior evidence and avoiding stale output.

```powershell
python -m venv .venv
.venv\Scripts\python.exe -m pip install -r requirements.txt
.venv\Scripts\python.exe scripts/setup.py --iso <disc.iso> --runtime D:\RAC2\runtime --wrench <wrenchbuild.exe>
```

For an archive, replace `--iso` with `--archive <archive.7z> --sevenzip <7z.exe>`.
The entire extracted ISO is verified before any analysis or unpacking. The setup
reads ISO9660 directly to extract and verify `SYSTEM.CNF` and the boot executable;
it derives the GP register from `.reginfo`, not from a RAC1 address.
`config/boot-sections.json` records the measured section/segment inventory;
`config/overlays.json` pins the 27 extracted level executables. Neither is a
reviewed function-boundary catalogue.

`<runtime>/latest.json` points to the successful preparation manifest. Use that
manifest explicitly to reconstruct and compare the boot and all 27 overlays:

```powershell
.venv\Scripts\python.exe scripts/build.py --manifest <manifest.json> --toolchain <EE-gcc-directory> --all-levels
.venv\Scripts\python.exe -m unittest discover -s tests -v
```

To build the boot with the reviewed C functions, supply the independently
qualified compiler toolchain in addition to the assembly toolchain:

```powershell
.venv\Scripts\python.exe scripts/build.py --manifest <manifest.json> --toolchain <SN-ProDG-2.0-EE-gcc-directory> --c-toolchain <SN-ProDG-3.01-EE-gcc-directory>
```

The integration snapshots the reviewed C, qualifies the exact new object in a
standalone link, then links that same object into the complete boot. It removes
only the reviewed assembly bodies, retains padding and remaining assembly
fragments, and preserves original call names as linker aliases to C symbols.
Both PT_LOAD segments and each C STT_FUNC body must still match before a proof
is emitted. No bytes are patched or trimmed after the link.

## Community

This project is tracked on **[openrac.dev](https://openrac.dev/)**, the community hub for
Ratchet & Clank decompilation, as its *Going Commando* project — next to
[RAC1](https://github.com/Lynder063/rac1-decomp) (Ratchet & Clank, 2002) and
[UYA](https://github.com/vetusmagnus/ratchet-uya-decomp) (Up Your Arsenal, 2004). The hub
aggregates the verified progress of each title from its repository; this repository remains
the source of truth for its own numbers.

The three games share engine code, so what one project measures often serves the others.
Reuse travels with credit: the C bodies this repository took from RAC1's reconstruction tree
name their origin per function in [docs/SECOND-C-LOT.md](docs/SECOND-C-LOT.md), and engine
intelligence contributed here is credited the same way in
[docs/COMMUNITY-ENGINE-REFERENCE.md](docs/COMMUNITY-ENGINE-REFERENCE.md).

## Contributing

Contributions are welcome. The rules are in [CONTRIBUTING.md](CONTRIBUTING.md); the full
walkthrough — what you need, the order to run things in, what each command must print, and what
to do when it does not — is [docs/START-HERE.md](docs/START-HERE.md).

Start with the environment check. It says what your machine can already do, what is missing,
and the next command to run:

```powershell
.venv\Scripts\python.exe scripts/doctor.py
```

## Lessons carried forward from RAC1

- Verify the exact edition, SHA-256, program, virtual address and size together.
  Identical addresses in different overlays do not identify the same function.
- Keep generated assembly, assets, runtime outputs, SDKs and private manifests
  local. Only source tools and sanitized measurements belong in this repository.
- Keep ELF section order and explicit virtual addresses. Derive splat's duplicate
  object names from each ELF; do not reuse RAC1 offsets. GC has two PT_LOAD
  segments and repeated section names, so compare **both** segments and give
  repeated names distinct generated identifiers.
- Require successful subprocess exit codes, newly generated output, exact lengths,
  entry point, segment flags and memory sizes. Never count a matching prefix,
  zero-sized alias, copied oracle, ELF magic, or stale build as a reconstruction.
- VU operand ordering, negative zero and linker padding fixes inherited from RAC1
  remain subject to the RAC2 byte gate; they are not assumptions of correctness.
- Wrench produces 27 level executables. Internal level identifiers are not
  contiguous. The boot's `map level` strings cover fewer levels than the disc.
- Compiler choice, optimization, GP-relative access, stack frames, relocations,
  function boundaries and delay slots must be measured on RAC2 independently.
  The RAC1 `-O2 -G2` profile is deliberately **not** declared valid for RAC2.
- Native runtime work must execute original code; HLE replacements and movie
  playback are not evidence of a functioning native game.

## C candidate lots

`candidates/boot.c` holds every reviewed body. It began with seven project-authored
leaf functions. Their initial neutral address-based names are retained; engine-purpose
names are not inferred. Ghidra's saved GC programme was reopened read-only with its
SHA-256 and R5900 language verified. All seven contiguous bodies, callers and return
delay slots were reviewed. `config/candidate-catalog.json` records their individual
boundaries. Two later lots added nineteen bodies measured in RAC1's reconstruction
corpus whose bytes are identical in RAC2 - see `docs/SECOND-C-LOT.md` and
`docs/THIRD-C-LOT.md`. Sixty-six more come from RAC2's own bytes instead, where one boot
body opens a function in the level overlays - see `docs/FOURTH-C-LOT.md` and
`docs/FIFTH-C-LOT.md`. One further body is the first that **cannot** be placed in a level:
it reads a global, and the level catalogues are built with an empty external map, so it
counts in the boot only - see `docs/SIXTH-C-LOT.md`. One further body is the first that
**calls** another function; it is placed in all twenty-seven overlays through a per-level
external map measured by masked search - see `docs/SEVENTH-C-LOT.md`.

```powershell
.venv\Scripts\python.exe scripts/check_candidates.py --reference <boot.elf> --toolchain <SN-ProDG-3.01-EE-gcc-directory> --runtime D:\RAC2\runtime
```

This lot uses `ee-gcc2953.exe`, its GNU `ee-as.exe`, and `ld.exe`, with
`-O2 -G0 -ffunction-sections`. Those settings are measured for these seven small
functions only. They are not a qualified profile for the remaining SDK or game
functions. In particular, a 64-bit zero-return declaration emitted `por`, while
the 32-bit declaration emitted the required `daddu`; the failed attempt is not
counted. The original API widths beyond the observed accesses remain to be studied.
Later lots extended the demonstrated scope of the same profile - arithmetic loops,
float code, pointer-length parameters, counted loops over byte structures and one
quadword copy - with every body proved individually through this same gate. The
profile is still a per-function qualification, never a general one.

The gate requires a fresh compile/link, a defined global STT_FUNC symbol, its
exact address and **full symbol size**, and all its bytes. Absolute aliases,
zero-size symbols, unlinked objects and identical prefixes with extra code are
refused. A deliberately wrong pointer-return candidate was compiled and rejected.
`progress/candidates.json` records reproducible source, tool and byte hashes.

These ninety-four functions are now **integrated** into the whole-boot reconstruction.
`progress/integration.json` records the complete boot gate, exact C object hash,
post-link function hashes and removed assembly inputs. `progress/candidates.json`
is the independent qualification of that same object before the complete link.
The remaining SDK and game functions still require separate compiler qualification.

## Level dispatch tables

Every level overlay carries three dispatch tables (`lvl.vtbl`, `lvl.camvtbl`,
`lvl.sndvtbl`) keyed by class identifier, and those identifiers are stable between the
prototype builds and the retail. **6 356 functions in the 27 retail overlays therefore
carry an identifier already named by the community's prototype extraction** —
`UpdateMoby_<oClass>`, `InitCamera_<id>`, `UpdateSound_<id>`. The transfer rests on a
falsifiable control (282 pairs of merged prototype functions, 282 identical retail
handlers), and it names level-local code: none of the 5 788 handlers is one of the boot
bodies placed in the levels. Method, limits and provenance in
`docs/MOBY-DISPATCH-TABLES.md`; the mapping in `docs/moby-dispatch.tsv`.

## decomp.dev reporting

The CI uploads `SCUS_972.68_report` in objdiff report v2 format. It records
**211,400 integrated C bytes out of 48,788,176 executable bytes (about 0.433%)**,
independently of assembly reconstruction. The measured scope includes the boot and all 27
overlays, with executable and initialized-data section sizes, including VU code.
Function counts are omitted until boundaries have been reviewed. Generated section
units are placeholders for that future catalogue, not completed translation units.

`python scripts/decomp_report.py --output build/decomp/report.json` regenerates
the report using measured metadata and integration proofs; it requires no game
assets or proprietary SDK in GitHub Actions. It rejects absent or inconsistent
integration evidence, changed source/catalogue hashes, object mismatches and
double-counted ranges. C units are split out of the remaining assembly units,
so the full code/data totals remain unchanged. The workflow hands every level
proof to the export, so the report carries the measured total.

## Next milestones

1. Keep the established boot and 27-overlay reconstruction gates passing.
2. Build a RAC2-specific function catalogue with reviewed boundaries and stable
   `(ELF SHA-256, level, address, size)` identities; labels alone are not proof.
3. Qualify the compiler/assembler combinations against real SDK and game functions.
4. Decompile and validate C/C++ units independently before any integration.
5. Develop and validate the native runtime against the matching PS2 edition.

No game assets, proprietary SDK, reconstructed executable, or copied retail
assembly is distributed. Third-party dependencies are installed separately; this
repository currently includes only project-45-authored tools and measurements.

## Licence

MIT — see [LICENSE](LICENSE). It covers the code in this repository only, never the game, its
assets, or the proprietary toolchains. By opening a pull request you agree that your
contribution is distributed under the same terms.
