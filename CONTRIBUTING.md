# Contributing

Thanks for helping. This repository is a byte-matching decompilation: the only thing that
counts as progress is code that a compiler turns into **exactly** the bytes the retail disc
contains. Analysis, tooling and documentation are welcome, but they never count as a match.

Full walkthrough: **[docs/START-HERE.md](docs/START-HERE.md)**.

Before choosing a function, inspect the single register through
`scripts/campaign.py --runtime <private-runtime> queue` and the corresponding `packet`. Use the
[maintained campaign workflow](docs/CAMPAIGN-WORKFLOW.md) for new trials.
Edit authored modules under `src/`, then regenerate the standalone compilation
units using the [source layout workflow](docs/SOURCE-LAYOUT.md). Preserve the
complete compilation context and refresh affected proofs after source changes.

## The three things this repository cannot ship

1. **Your own copy of the game** — the USA v1.01 disc (`SCUS_972.68`), verified against the
   hashes pinned in `config/target.json`. Greatest Hits v2.00 and other regions are
   *different targets*; do not mix them.
2. **The local toolchains** — ProDG 2.0 (`ee/bin/Ps2EeAs.exe`, `ee/bin/ld.exe`)
   reconstructs assembly. Authored C uses the qualified GNU EE compiler and
   assembler profile, then the ProDG linker. See
   [docs/COMPILER-NOTES.md](docs/COMPILER-NOTES.md) for the measured profiles and
   their limits; a byte match does not establish the original compiler identity.
3. **Wrench** (`wrenchbuild`) to unpack the level executables.

That is deliberate: no game data and no proprietary SDK is distributed here. The tooling and
the tests run without any of it — `python -m unittest discover -s tests -v` and
`scripts/decomp_report.py` need nothing but Python 3.12.

## The loop

1. **Check your environment** — `python scripts/doctor.py` reports what you have, what is
   missing, and the next command to run.
2. **Prepare the reference locally** — `scripts/setup.py` verifies your disc and extracts the
   boot executable and the 27 level overlays into a runtime directory **outside** this
   repository.
3. **Pick a target** — a small function, or a family that is already matched. Say what you are
   working on early (a draft pull request) so two people do not match the same function.
4. **Prove it** — `scripts/check_candidates.py` compiles `candidates/boot.c` and compares each
   catalogued symbol byte for byte; `scripts/build.py` then re-links the whole boot and the
   level overlays and compares every loaded byte of both PT_LOAD segments.
5. **Open the pull request** — with the command you ran and its result.

## Hard rules

- **Write in English.** Documentation, comments, messages, catalogue
  descriptions and commit messages use English. Measured game identifiers and
  program names retain their original spelling.
- **Use detailed, scoped commit messages.** Follow the Lombyte style: a subject
  such as `overlay: qualify two native code families across 27 levels`, then
  paragraphs explaining the concrete change, its technical reason, measured
  scope and before/after progress, and the validation actually performed.
  Record relevant limitations. Substantive changes require more than a subject.
  The repository provides `.gitmessage` as a commit template.

- **Byte equality is the only acceptor.** No patched bytes, no trimming after the link, no
  "it looks right", no model verdict.
- **Never commit retail-derived material**: the disc image, `boot.elf`, level overlays,
  objects or `.bin` files produced from them, `asm/`, `build/`, runtime output. `.gitignore`
  covers the usual names — read your diff anyway.
- **Do not touch pinned identities.** `config/target.json`, `config/overlays.json`, the
  catalogue hashes and the `reference_sha256` fields are what makes a claim verifiable. If a
  catalogue legitimately changes, regenerate `progress/candidates.json` with a fresh checker
  run **on the same build snapshot** — the integration refuses a catalogue that changed since
  its review, and that refusal is a feature.
- **Say where your material comes from.** A match must be derived from the retail disc you
  own, be your own work, or come from a sister project that shares it with us — name the
  project, its author and the agreement in the pull request. Community archives, prototype
  builds and leaked material may be *cited as reference*, never as evidence. The Ratchet &
  Clank decompilations work together; what keeps that possible is that every reuse is written
  down, byte proof and permission side by side.
- **One function (or one coherent family) per pull request**, tests green:
  `python -m unittest discover -s tests -v` must pass.

## Licence

This repository is MIT ([LICENSE](LICENSE)). By opening a pull request you agree that your
contribution is distributed under the same terms. The licence covers the code in this
repository only — never the game, its assets, or the proprietary toolchains.
