# RAC2 merge queue

Maintainers activate the queue only after its workflows and trusted validator
are integrated into protected `RAC2`.

The merge queue tests an approved contribution against the current `RAC2`
branch before integrating it. When another PR merges first, a queued PR does
not need a topic-branch update merely because its base is older. GitHub prepares
a temporary combined commit and reruns the required checks on that commit.

## Submit and integrate

1. Author and validate your contribution against a current upstream base. Keep
   the actual validated base and proof provenance in the required PR description.
2. Obtain the required review and pass the PR checks. Maintainers use the merge
   queue action on GitHub, or `gh pr merge <number> --match-head-commit <sha>`
   when the PR is eligible. Do not use an administrator override.
3. The queue runs `tests`, `SCUS_972.68 Progress`, and `PR description` on its
   temporary combined commit. A successful group merges automatically in order.
4. If the queue removes a failed contribution, inspect the failing run. Correct
   the contribution and its evidence, then submit it to the queue again.

The configured policy uses merge commits, one build at a time, one PR per group,
and `ALLGREEN` validation. Every group must pass; a green later contribution
cannot excuse an earlier failure. Required checks have a 60-minute response
timeout. The queue does not waive review, reservations, discussion resolution,
or the existing integrity checks.

## Proofs and conflicts

The queue does not resolve conflicting source edits or regenerate byte-matching
proofs. Function reservations prevent duplicate ownership but do not prevent
conflicts in shared compilation units, catalogues, registers or generated reports.
If combined C or proof inputs change, follow the branch-update procedure and
refresh the affected complete-symbol and full-image evidence with your verified
private environment. Do not hand-edit hashes, counts or gate results to make a
queue check pass. Documentation-only changes do not require a game rebuild.

The description's validated base remains the base actually used by the author.
Queue checks are an additional combined-commit validation, not a claim that CI
reconstructed the game or independently established an ABI.

## Trusted description checks

Normal PR metadata is checked by the protected `pull_request_target` workflow,
which reports a commit status on the exact contribution head. Queue descriptions
use a separate read-only job named `PR description`. It loads the validator from
protected `RAC2`, resolves live queue membership and references through GitHub,
and validates current PR bodies and changed-file lists as data. Missing or changed
membership, malformed events and unavailable API evidence fail the group.

The queue job has no status-writing permission and never executes contributor
Python with its API token. GitHub Actions reports its job result on the temporary
commit. Workflow changes still require code review; the format check is not a
technical approval of a contribution.

See GitHub's [merge queue guide](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/configuring-pull-request-merges/managing-a-merge-queue).
