# Required pull request descriptions

Every PR targeting `RAC2`, including maintainer and AI-authored PRs, uses
[the template](../.github/pull_request_template.md). Fill it for the actual diff;
do not paste the instructions back or tick checks you did not perform. Write in
English. Keep explanations concise, but supply enough evidence for a reviewer
to assess your result without reconstructing your session.

The [merge queue](MERGE-QUEUE.md) revalidates descriptions for its temporary
combined commit. Keep the validated base you actually used; queue validation
does not fabricate a new private reconstruction or matching proof.

## Common fields

- **Summary:** the concrete problem and resulting behavior.
- **Type:** one or more of `matching`, `placement`, `nonmatching`, `tooling`,
  `documentation`, separated by commas. C source changes need a decompilation
  type even when accompanied by tooling or docs. Maintained `src/`/`candidates/`
  and matching catalogue/proof edits require `matching` or `placement`, including
  zero-credit maintenance; retained `nonmatching` attempts belong on the shelf.
- **Validated base:** the full upstream `RAC2` SHA used in your combined-source
  validation. This records provenance; it does not automatically prove your
  branch is current. Follow the [branch update guide](CONTRIBUTOR-QUICKSTART.md#5-update-your-topic-branch-before-requesting-review).
- **Reservation:** the acknowledged shared claim/issue for function work.
  Work already underway before reservations were introduced may explain its
  legacy ownership. Docs/tooling may use `N/A: no function work`. The description
  does not itself acquire or verify a reservation.
- **Targets:** exact symbols, program placements and authored source modules;
  docs/tooling identify their files or explain why game targets do not apply.
- **Validation:** alternating `- Command:` and `- Result:` lines. Repeat for
  independent checks. Give actual counts/outcomes, report paths, failures and
  skips. Put the command on its field line; optional fenced logs may follow.
  Use `<reference>`, `<toolchain>`, `<runtime>` placeholders instead of private
  paths. Never attach game bytes, proprietary binaries, credentials or runtime
  data to a PR.
- **Risks and follow-up:** limitations, negative trials, unknown semantics and
  remaining steps. `None` is valid only when accurate.
- **Checklist:** explicitly confirm the three template statements.

## Evidence appropriate to the change

For `matching` or `placement`, fill all five matching-evidence fields:

- **Proofs:** versioned proof paths and symbols; source/checker hashes and
  campaign task/trial IDs when applicable. Private full-image reports may be
  described by report hashes and measured outcomes without publishing them.
- **Physical delta / Unique delta:** begin each value with a signed integer
  byte delta, such as `+16 (700268 -> 700284)` or `0 (metadata only)`. These
  measures are distinct; replicated placements are not extra unique functions.
- **Gates:** complete-symbol sizes/byte differences and affected full-image
  results. State which images passed, failed or were not run, and whether
  changed shared providers require boot plus all affected overlays. A checker
  match alone is not an integration gate.
- **ABI review:** argument/return use, caller/global bindings examined and
  remaining uncertainty. Do not infer a field's original role solely from an
  offset. Record provenance when reusing another contributor's trial.

`nonmatching` work reports `0` for both deltas and identifies its retained
trial, mismatch and reopening condition; it earns no exact-C credit.
Docs/tooling use `N/A: <specific reason>` for fields outside their scope. A
documentation-only PR does not require reconstructing game images. Do not
claim local reconstruction merely because historical proofs or CI are green.

## Example: small tooling change

```markdown
## Summary
Reject incomplete PR descriptions before merge so reviewers can locate evidence.

## Scope
- Type: tooling, documentation
- Validated base: b847d8c8a87a2398f42498d4209988545b92a5b6
- Reservation: N/A: no function work
- Targets: scripts/pr_description.py and contributor documentation

## Validation
- Command: python -m unittest discover -s tests -p test_pr_description.py -v
- Result: targeted description and event-handler tests pass; no game build run.

## Matching evidence
- Proofs: N/A: no game source or proof inputs changed
- Physical delta: N/A: no matching credit
- Unique delta: N/A: no matching credit
- Gates: N/A: no game source or proof inputs changed
- ABI review: N/A: no game source changed

## Risks and follow-up
The format check cannot prove the truth of a contributor's claims.

## Checklist
- [x] Description reflects this head and the validated combined source.
- [x] Only authored source and public proof metadata are included; no game bytes, proprietary tools, secrets or private runtime data.
- [x] Provenance and reused work are identified; incomplete or skipped validation is disclosed.
```

## Enforcement and limits

The required **PR description** commit status fails on missing sections, empty
fields, TODOs, unconfirmed checklist statements and obvious type/changed-C
inconsistencies. Editing a PR description reruns this inexpensive check;
there is no need to push a commit to fix prose. Drafts can fail while being
prepared; fix their description before requesting review. No contributor,
trusted team member or bot is exempt from the integrity status requirement.

Check a saved body locally:

```sh
python scripts/pr_description.py --body-file <pr-body.md>
```

The workflow reads current PR metadata and filenames through GitHub's API,
executes only code from this repository's current protected `RAC2` branch and
publishes the status to the
exact PR head. It never checks out the contributor branch or executes a pasted
command. A maintainer can rerun **PR description contract** with the PR number
on the `RAC2` branch to validate an older open PR after rollout.
The checkout deliberately ignores the PR event's cached base SHA: an older
PR may still name a base commit from before the validator existed. Updating a
description must use the current trusted validator even while that PR's source
branch awaits its separate upstream refresh.

The check validates the description's structure, not the truth of tests,
ownership, hashes, ABI claims or byte equality. Existing test/progress gates,
branch freshness requirements and technical review still apply. A status
update after a body edit is asynchronous: reviewers should check the current
description and latest run before merging, especially during concurrent edits.
Commit statuses belong to a SHA, so two open PRs targeting `RAC2` must not share
the same head SHA. Close the duplicate (or use distinct commits) and rerun the
description check; one PR's description must not grant another PR a pass.
Status-writing runs are serialized across the repository, including manual
reruns. If GitHub replaces an intermediate queued event, a maintainer can
manually rerun a PR still showing a pending or missing description status.
Existing open PRs must adopt this format when the required status is enabled;
no historic test or gate should be fabricated to fill a field.
