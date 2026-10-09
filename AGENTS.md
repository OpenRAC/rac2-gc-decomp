# Repository language and commit messages

For a first-time contributor, read [docs/CONTRIBUTOR-QUICKSTART.md](docs/CONTRIBUTOR-QUICKSTART.md)
and [toolchain/README.md](toolchain/README.md). Default to a fork/topic branch and
draft PR targeting `RAC2`; do not inherit the maintainer's direct-push permissions
or private environment. Every contributor must supply a legally acquired matching
ISO and the full tool suite before contribution work. Missing prerequisites block
that work; assist with setup, not an alternative contribution route. Never fabricate
matching claims or download an SDK from an unverified source.

Start or resume through [docs/CONTINUE.md](docs/CONTINUE.md). Read the optional
ignored `.local/ENVIRONMENT.md` pointer for machine-specific tools and current
private operational state. The public repository owns methods, task decisions
and proofs; no long pasted prompt or personal workspace details belong here.

Use the maintained [campaign workflow](docs/CAMPAIGN-WORKFLOW.md) for task selection,
packets, trials and complete batches. `config/campaign-register.json` is the one
authority for experiment history and task decisions. Generate queue/history views
through `scripts/campaign.py views`; do not maintain a parallel queue or overwrite
an unregistered view edit. Preserve negative trials and explicit reopening conditions.

Before function decompilation, use the [shared reservations](docs/CONTRIBUTOR-RESERVATIONS.md).
Claim a small lot and wait for acknowledged ownership; a local Git lock or open
PR does not reserve work across forks. Check the [nonmatching shelf](nonmatching/README.md)
for retained attempts and preserve their provenance. The shelf and reservation
ledger add no matching credit and do not replace campaign decisions or proofs.

Author C under `src/` and follow [source organization](docs/SOURCE-LAYOUT.md).
`candidates/` contains generated standalone compilation units. Keep declaration
context, explicit per-program placements and source/checker hashes coherent.
Source modules do not prove original object boundaries. A source inventory counts
authored variants separately from replicated loaded-code coverage and adds no credit.
Runtime/tool paths and trial inputs remain private outside the repository.

After updating integrated progress, run `python scripts/readme_progress.py` and
include `README.md` and `progress/decompilation.svg` in the same lot. The generated
table and bar use the validated boot and all 27 overlay proofs; CI rejects either
if stale. Keep the generated progress block markers and do not hand-edit its counts.

Write all repository documentation, code comments, user-facing messages,
catalogue descriptions and commit messages in English. Preserve measured game
identifiers, symbol names, program identities and pinned reference hashes.

Use the detailed Lombyte commit style: a scoped subject such as `overlay:`,
`decomp:`, `compiler:`, `docs:` or `fix:`, followed by a substantive body.
Describe the concrete change and its technical reason, the measured scope and
before/after progress when relevant, and the validation actually performed.
State any remaining limitation needed to interpret the result. A subject alone
is insufficient for a substantive matching or tooling change.

Keep source, catalogue, object and integration proofs coherent. Regenerate
affected reviews and full loaded-byte gates after changing hashed inputs,
including translations of comments or catalogue descriptions. Publish only
authored source, structural identifiers and proof metadata; keep game bytes,
proprietary tools and private runtime artifacts outside the repository.
