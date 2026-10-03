# Start here

This repository reconstructs *Ratchet & Clank: Going Commando* (PS2, USA v1.01) into C that a
compiler turns into **exactly** the bytes of the retail executable. Everything below exists so
you can go from a fresh clone to a proven match without asking anyone.

The rules are in [CONTRIBUTING.md](../CONTRIBUTING.md). This page is the long version: what you
need, in what order, what each command must print, and what to do when it does not.

## What you need, and where it comes from

| You need | Where it comes from | What it unlocks |
| --- | --- | --- |
| Python 3.12 + `requirements.txt` | `pip` | tests, report export, everything |
| **Your own** disc image of the USA **v1.01** release (`SCUS_972.68`) | your legally obtained copy | the reference bytes |
| **SN ProDG 2.0** EE toolchain (`ee/bin/Ps2EeAs.exe`, `ee/bin/ld.exe`) | you supply it | assembly reconstruction, the build gate |
| **Reconstructed GNU EE 2.9-ee-991111b** `cpp`/`cc1`/`as` profile | locally rebuilt; see [compiler notes](COMPILER-NOTES.md) | current authored-C compilation and assembly |
| **SN ProDG 3.01** EE toolchain (`ee/bin/ld.exe`; earlier compiler profile `ee-gcc2953`) | you supply it | linking the current C objects; retaining the earlier SN profile |
| **Wrench** (`wrenchbuild`) | you supply it | unpacking the 27 level overlays |
| A runtime directory **outside** this repository | you create it | every generated file lands there |

Greatest Hits **v2.00** and other regions are **different targets** — the pinned hashes in
`config/target.json` reject them. The tests and `scripts/decomp_report.py` need **none** of the
proprietary entries above; a bare Python 3.12 runs them (that is what CI does).

The current C checker compiles through `scripts/wsl_chain.py` and uses the SN
toolchain directory for its linker. `8bed6eae` in a proof is the SHA-256 prefix
of the reconstructed `cc1`, not a compiler version. Compatibility with the
qualified retail bodies does not establish the original game's compiler identity.

## 0. Python

```powershell
python -m venv .venv
.venv\Scripts\python.exe -m pip install -r requirements.txt
```

`scripts/build.py` refuses to run unless `splat64`, `spimdisasm` and `rabbitizer` are exactly
the pinned versions. `scripts/doctor.py` checks that for you.

## 1. Ask your machine what it can do

```powershell
.venv\Scripts\python.exe scripts/doctor.py
```

With your inputs it prints the same list, all `ok`, and a verdict:

```text
python            3.12.10  ok
splat64           0.50.0  ok
spimdisasm        1.42.4  ok
rabbitizer        1.16.2  ok
target            Ratchet & Clank: Going Commando USA v1.01 (SCUS_972.68, 27 levels)
manifest          <runtime>\runs\<id>\manifest.json (a previous setup.py run; ...)
ProDG 2.0         <dir> has all 2 instruments
ProDG 3.01        <dir> has all 5 instruments

VERDICT
  tooling (tests + report export)  yes -- needs nothing proprietary
  build (assembly reconstruction)  yes
  C candidates (byte proofs)       yes

NEXT COMMAND
  python scripts/check_candidates.py --reference <runtime>\runs\<id>\reference\boot.elf ...
```

The last line is always the next thing to type. If you already ran `setup.py` once, the doctor
finds the manifest and will not ask you to re-verify 3.8 GB of disc.

## 2. Prepare the reference from your own disc

```powershell
.venv\Scripts\python.exe scripts/setup.py --iso <disc.iso> --runtime <runtime> --wrench <wrenchbuild.exe>
```

For an archive, use `--archive <archive.7z> --sevenzip <7z.exe>` instead of `--iso`. On success
it prints, and writes `<runtime>/latest.json`:

```json
{ "manifest": "<runtime>\\runs\\<id>\\manifest.json", "boot_sha256": "36d5814d…", "gp": "0x001AEFF0", "level_wads": 27, "overlays": 27 }
```

On failure it prints `Preparation failed: <reason>` and exits **2**. A wrong disc says
`ISO: wrong size, sha1, …`; a runtime inside the repository says so explicitly.

## 3. Rebuild, and gate every byte

```powershell
.venv\Scripts\python.exe scripts/build.py --manifest <runtime>\runs\<id>\manifest.json --toolchain <ProDG-2.0> --all-levels
```

Add `--c-toolchain <ProDG-3.01>` to link the reviewed C bodies into the boot as well. A good run
ends with:

```text
Verified report: <manifest-directory>\builds\<id>\report.json
```

and each program's `gate.json` carries `"matched": true` with the number of bytes compared.
A refusal ends with `Build failed: <reason>` — see the table below. `--all-levels` rebuilds the
boot gate inside every one of the 27 overlays; on a cold cache that is roughly 40 minutes.

## 4. Run the tests

```powershell
.venv\Scripts\python.exe -m unittest discover -s tests -v
```

The suite must end with `OK`. It needs no disc, no toolchain, no network.

## 5. The per-function loop

```powershell
.venv\Scripts\python.exe scripts/check_candidates.py --reference <runtime>\runs\<id>\reference\boot.elf --toolchain <ProDG-3.01> --runtime <runtime>
```

It compiles `candidates/boot.c`, links a standalone candidate, and compares **every catalogued
symbol** byte for byte. It prints a JSON with one entry per function, and exits:

- **0** — every catalogued symbol matches (`"matched": true`, and no different byte);
- **1** — at least one symbol differs: the difference is the diagnostic, not a failure of will;
- **2** — the run could not be set up (`Candidate preparation failed: <reason>`).

Then, for a body you want to add: put the C in `candidates/boot.c`, add its entry to
`config/candidate-catalog.json`, and **regenerate `progress/candidates.json` with a fresh
checker run on the same build snapshot**. The integration refuses a catalogue that changed since
its review — `Integration catalogue changed since review` — and that refusal is a feature, not
an obstacle to work around.

## What the gate refuses, and why

The rules are deliberate; each one exists because a wrong result once got through without it:

- **Byte equality is the only acceptor.** Not a score, not a similarity, not a model's opinion.
- **No patched bytes, no trimming after the link.** If the compiler did not produce it, it does
  not count.
- **No prefix, stale or zero-size matches.** The symbol's address, size and full body must be
  the reviewed ones.
- **Retail-derived files never enter the repository** — not the disc, not `boot.elf`, not the
  overlays, not objects or `.bin` files made from them, not `asm/` or `build/`.

## When it goes wrong

| What you see | What it means | What to do |
| --- | --- | --- |
| `Use pinned splat64 0.50.0` | your environment has another version | `pip install -r requirements.txt` |
| `Missing instrument ee-gcc2953.exe` | `--toolchain` points at the wrong root | point it at the folder **containing** `bin/` and `ee/` |
| `ISO: wrong size, sha1, …` | not the supported release | USA v1.01, `SCUS_972.68`; v2.00 is another target |
| `Runtime must be outside the source repository` | your `--runtime` is inside the clone | pick a directory elsewhere |
| `Wrong RAC2 reference identity` | `--reference` is not the pinned boot | use the `boot.elf` that `setup.py` extracted |
| `Keep private candidate builds outside sources` | same rule, for the checker | same fix |
| `Integration inputs changed since candidate review` | the reference or the C source changed after the catalogue | regenerate the catalogue and the proof together |
| `Integration catalogue changed since review` | `config/candidate-catalog.json` was edited after `progress/candidates.json` | re-run the checker on the same build snapshot |
| `Every integrated function requires a complete candidate match` | catalogue and checker disagree | the catalogue lists a symbol the checker does not fully match |
| `C candidates must not embed assembly or retail bytes` | the C uses inline asm or raw bytes | a match must come from the compiler |
| `G3 requires all 27 verified overlays` | a level build was asked for before every overlay is verified | run `--all-levels` first |
| `Candidate is older than an input` | a stale object/output was reused | delete the run directory and rebuild |

## What is not reproducible here

- **The retail compiler.** Part of the game's code was built by a patched `cc1` that is not
  redistributable. Some functions cannot be reproduced with the SN toolchains at all; those
  targets are coordinated before anyone spends a day on them.
- **Level-only bodies.** The counting path admits bodies reviewed against the **boot**; three
  measured bodies that exist only inside level overlays are documented in
  [THIRD-C-LOT.md](THIRD-C-LOT.md) and wait for a level-body lot to be wired through
  `check_candidates.py`, `integration.py`, `build.py` and `decomp_report.py`.
- **Community reference material.** [COMMUNITY-ENGINE-REFERENCE.md](COMMUNITY-ENGINE-REFERENCE.md)
  collects engine intelligence from the wider community. It is a **reference**, never evidence:
  nothing in it can make a match, and the gate never reads it.

## Asking for help

Open an issue — there is a template for a tool problem and one for "I want to help". Paste the
output of `scripts/doctor.py`; it says in one block what you have and what is missing.

The wider community — the other Ratchet & Clank titles, their repositories and their current
progress — is gathered at **[openrac.dev](https://openrac.dev/)**. If your question is about a
sister project's tooling, or about an engine pattern those projects already solved, that is
where its maintainers are.
