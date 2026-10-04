# Repository language and commit messages

Use the maintained [campaign workflow](docs/CAMPAIGN-WORKFLOW.md) for task selection,
packets, trials and complete batches. `config/campaign-register.json` is the one
authority for experiment history and task decisions. Generate queue/history views
through `scripts/campaign.py views`; do not maintain a parallel queue or overwrite
an unregistered view edit. Preserve negative trials and explicit reopening conditions.

Author C under `src/` and follow [source organization](docs/SOURCE-LAYOUT.md).
`candidates/` contains generated standalone compilation units. Keep declaration
context, explicit per-program placements and source/checker hashes coherent.
Source modules do not prove original object boundaries. A source inventory counts
authored variants separately from replicated loaded-code coverage and adds no credit.
Runtime/tool paths and trial inputs remain private outside the repository.

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
