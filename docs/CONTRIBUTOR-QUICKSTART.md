# Contribute with an AI: complete setup first

You do not need programming or PS2 expertise to ask a coding AI for help.
**Every contributor must first have their own legally acquired matching ISO
and the complete project tool suite.** The AI guides setup and checks the files;
it does not supply legal rights, a disc image or an unverified SDK download.

Use an AI that can edit local files and run terminal commands. A chat without
file access can explain the steps, but cannot perform this workflow itself.
You also need a GitHub account so your work can be submitted for review.

## 1. Give the AI this message

Copy this into your coding assistant, with access to a new workspace or an
existing fork checkout. Keep your ISO, BIOS and tools local.

```text
I am a beginner. Help me prepare the complete environment and make one
validated contribution to https://github.com/llesieur99/rac2-decomp,
starting from branch RAC2.

Read AGENTS.md, docs/CONTRIBUTOR-QUICKSTART.md and toolchain/README.md.
Contribution requires my own legally acquired USA v1.01 ISO (SCUS_972.68)
and the complete qualified tool suite. Help me install public dependencies,
locate my authorized local tools and verify the required versions/hashes.
Do not obtain an ISO, BIOS or SDK from an unverified source or upload them.

Finish setup before selecting or editing a contribution. Run the full
--contributor-check, verify Ghidra R5900 support and PCSX2 usability, prepare
the pinned reference and reproduce the complete baseline. If anything is
missing, name it and continue only the permitted setup steps; do not switch
to a contribution without the required ISO/tools or invent a matching result.

Explain things plainly. Only ask for actions you cannot do, such as GitHub
login, system installation permission or the path to my own permitted files.
Preserve existing work. Use/create my fork and a new topic branch from RAC2.
I authorize pushing the validated topic branch to my fork and opening a
draft PR to upstream RAC2. Do not push or merge upstream, or force-push.

Once the complete setup is verified, inspect the current task register and
prior refusals, choose one coherent contribution, implement it, run the
appropriate real gates/tests, show me the diff and use detailed English
commits. The PR must report actual tests and limitations. If GitHub login
is unavailable, prepare a local commit and PR title/body instead of claiming
a PR was submitted.
```

The AI should complete one reviewable contribution. It should not inherit the
maintainer's private paths or open-ended campaign permissions.

## 2. What you need to provide

| Requirement | What the AI does |
| --- | --- |
| Your legally acquired Going Commando USA v1.01 ISO | Verify the pinned size and hashes from `config/target.json` |
| GitHub account and coding AI with local access | Guide login; create your fork and task branch |
| Git, Python 3.12 and pinned Python packages | Reuse the configured interpreter and check required package versions |
| Wrench `wrenchbuild` | Check the executable used to extract your reference |
| Windows + WSL and qualified GNU EE `cpp`/`cc1`/`as` | Check the current actual instrument hashes |
| Authorized SN EE C linker and reconstruction tools | Check the distinct profiles and actual hashes |
| Ghidra with R5900 support and analysis access | Verify program/language/reference identity |
| PCSX2 and your permitted local BIOS | Verify configuration and use only your local game data |

All sources, acquisition status and component roles are listed in
[toolchain/README.md](../toolchain/README.md). Some historical tools have no
verified public download route here. Missing prerequisites are setup blockers,
not a reason to use an unrelated compiler or submit an unvalidated contribution.
An ISO hash confirms the edition, not how it was acquired; legal provenance is
the contributor's responsibility.

## 3. Check the complete setup

The AI can install the Python dependencies and run the software tests during
preparation. That alone does not satisfy the contribution prerequisite.
Use your existing configured Python first. Run the diagnostic before installing
anything; install only when required packages/versions are missing. A virtual
environment is optional when isolation is needed, not a mandatory setup step.

```powershell
python scripts/doctor.py
# Only if the diagnostic reports missing/incompatible packages:
python -m pip install -r requirements.txt
python scripts/doctor.py --contributor-check --iso <your-iso> --runtime <private-runtime> --wrench <wrenchbuild> --toolchain <ASM-root> --c-toolchain <C-linker-root> --ghidra <ghidra-launcher> --pcsx2 <pcsx2-executable> --bios <your-bios>
```

Use the same interpreter for `pip`, diagnostics, tests and builds. If it is shared
with another project or system-managed, resolve dependency conflicts first rather
than overwriting its packages or bypassing the system's installation protections.

Replace placeholders with the verified local paths; the AI should do that for
you after locating your files. `--contributor-check` returns nonzero when required
files/profile checks are incomplete. It hashes the ISO and current instrument
files without compiling or running the game. Separately verify legal acquisition,
Ghidra's R5900 language and actual analysis access, and usable PCSX2 configuration.
The diagnostic is a setup check, not a matching proof.

Then follow [START-HERE.md](START-HERE.md) to prepare the reference and
[CAMPAIGN-WORKFLOW.md](CAMPAIGN-WORKFLOW.md) to reproduce the boot and all
27-overlay baseline. The current matching runner uses Windows + WSL.

## 4. Select and validate one contribution

Once prerequisites and baseline are verified, the AI reads `status`, `queue`
and the selected `packet`. It respects parked targets and their reopening
conditions. New game C requires complete symbol and affected full-image gates;
metadata/diagnostic changes need their relevant checks too.

Before decompiling, inspect the [nonmatching shelf](../nonmatching/README.md)
and [reserve a small lot](CONTRIBUTOR-RESERVATIONS.md) through the shared upstream
CLI. Wait for the exact claim acknowledgement; an open draft PR or local lock
does not reserve functions across forks. Check ownership before trials. Keep
the reservation during review and release it only after stopping your work.

Before submitting, run the repository tests, source/view freshness checks and
the README bar check. Keep game images, BIOS, SDKs, objects, extracted assembly
and logs out of Git. Retain the actual private evidence rather than guessed results.

## 5. Update your topic branch before requesting review

The upstream `RAC2` branch can advance while you work. Ask your AI to check it
before starting a contribution, before requesting review and whenever GitHub
reports that the PR needs an update. Keep working on your existing topic branch.

First inspect `git status --short` and `git remote -v`. Preserve unfinished work
with a local commit or another reviewed backup before merging. Verify that
`upstream` points to `https://github.com/OpenRAC/rac2-gc-decomp.git`; if that
remote name is already used for something else, choose and use a different name.
If the upstream remote is absent, the AI can add it:

```sh
git remote add upstream https://github.com/OpenRAC/rac2-gc-decomp.git
```

Fetch the current target and compare it with your topic branch:

```sh
git fetch upstream RAC2
git log --oneline HEAD..upstream/RAC2
```

If upstream has new commits, merge them into the topic branch:

```sh
git merge upstream/RAC2
```

Resolve conflicts while preserving both the upstream changes and your authored
work. Generated source, catalogue/register views and proof/progress files must
describe the combined source snapshot. Do not take an older generated file
wholesale, hand-edit hashes or counts, or discard experiment history to finish
the merge. Use the maintained [source layout](SOURCE-LAYOUT.md),
[campaign workflow](CAMPAIGN-WORKFLOW.md) and
[guarded finalizer](CAMPAIGN-TOOLS.md) to regenerate affected outputs and validate
the complete affected symbols and full images. Keep your own verified runtime
and tool bindings private; another contributor's local paths are not portable.

Run the relevant freshness checks and tests after resolving the update. Commit
the coherent result and push normally to your fork; do not force-push. If the
target advances again and changes your validated inputs, repeat the update and
affected validation. Required CI must pass on the final PR commit before merge.
Documentation-only updates use documentation checks and do not require a new
game reconstruction.

## 6. Submit through your own fork

A **fork** is your copy of the project on GitHub. A **branch** isolates your task.
A **pull request (PR)** asks the maintainer to review your changes; it does not
merge them automatically. The AI can handle these after normal account login.
[GitHub's fork guide](https://docs.github.com/en/pull-requests/how-tos/work-with-forks/fork-a-repo)
explains the website route too. The target project branch is `RAC2`, not `main`.

You should receive a small understandable diff, a scoped English commit, actual
test/gate outcomes and a **draft PR targeting `RAC2`**. Ask the AI to explain its
description before requesting review. Never paste access tokens into chat or files.
