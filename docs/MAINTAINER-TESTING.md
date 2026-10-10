# Primary maintainer test route

This route reduces repeated tool-suite runs for the primary maintainer only.
It does not change other contributors' requirements, code review, the protected
merge queue, matching credit, or byte-exact gates.

| Contribution | Local preparation | PR tool tests | Merge-group tool tests | Main tool tests |
| --- | --- | --- | --- | --- |
| Other contributors | Existing full-suite workflow | Full | Full | Full |
| Authenticated primary maintainer | Explicit focused tests and applicable matching gates | Limited smoke, changed test modules and Python syntax | **Full, always** | Reuse verified exact-SHA full queue run; otherwise full |

GitHub metadata must identify author ID `191315338`, login `llesieur99`, and
both head/base repository ID `1400228215` (`OpenRAC/rac2-gc-decomp`). Git names,
PR text, labels and a fork with the same branch name do not grant this route.
The PR selector runs from its protected base commit. A base without the selector
runs the full suite, including the rollout PR introducing this feature.

## Local focused checks

Use the repository virtual environment and authenticate `gh` to GitHub as the
primary maintainer. Select actual affected maintained test modules, for example:

```sh
python scripts/maintainer_tests.py --test test_progress_modules --test test_decomp_report_cli
```

The command verifies the immutable authenticated user ID and maintained origin,
then runs the explicit modules. This is a targeted check, not a full-suite
receipt. Review the change's dependencies when selecting modules. Changes to C,
declarations, placements, compiler or other hashed matching inputs still require
their fresh source/object/integration/reference gates. Focused tool tests do not
replace any such gate. Documentation/presentation changes with unchanged matching
inputs do not require a new game build.

For a completed matching batch, the maintainer can keep the existing finalizer
command and add a repeated test selection:

```sh
python scripts/campaign.py --runtime <private-runtime> finalize <action-id> \
  --manifest <private-manifest> --output <private-staging> \
  --maintainer-test test_decomp_report_cli
```

The finalizer authenticates the maintainer before preparation and additionally
requires `test_campaign_finalize`, `test_maintainer_tests`, and
`test_maintainer_test_policy`. Every proof, normalization, raw replay, source
inventory, privacy, drift and publication check remains active. Its private plan
and receipt record `local_test_policy.mode = targeted`, the authenticated actor,
modules and pending mandatory full merge-queue suite. Resume/apply must use the
same selection. Existing/default finalization remains a full local suite. Neither
mode adds credit or proves gameplay.

## CI and post-merge provenance

The primary maintainer PR check runs the policy/command/report smoke modules,
explicitly changed maintained test modules and an AST syntax check of changed
Python files. It does not import changed scripts, infer transitive test coverage
or fall back to full discovery for an unmapped executable. Deleted tests are
not represented as executed coverage. This is deliberately limited pre-queue
feedback; the complete merge-group suite remains mandatory for every change.
The primary-maintainer PR additionally checks current pinned source hashes,
exact rendered compilation units, source-inventory metadata, the original
physical proof/object metadata validators and physical README display once.
It does not reclassify source families or generate/group the unique and reuse
corpora on that limited route. Those full checks run in the mandatory queue.
Applicable local legal-reference/compiler/SDK raw matching gates remain required.

`Tool tests / tests` never relies on a skipped dependency: a missing or failed
policy decision fails the required job. The merge-group command is a literal
unconditional `unittest discover` route regardless of policy output.

On a protected-main push, reuse requires exactly one associated merged primary
maintainer PR whose `merge_commit_sha` equals the current commit; both repository
identities; a successful `merge_group` run of the exact `tests.yml` workflow ID,
path, repository and commit; the current attempt's successful `validation` producer, successful required
`tests` and `SCUS_972.68 Progress` consumers, completed full-suite/export steps
and the workflow byte hash reviewed in the policy. The producer exports one
physical/grouped, unique and reusable-code report set; the two required consumers
fail if the policy or shared producer fails, is cancelled or is skipped.
A matching step name alone is insufficient. The qualified workflow forces full
discovery for every merge group. There is no tree-equivalence fallback. API
errors, ambiguity, absent evidence or changed workflow bytes select full tests.
Workflow changes require reviewing and updating this qualified hash.

The policy job records the queue run, attempt, commit and workflow digest in its
log. Main reuse additionally requires a unique attempt-specific immutable
artifact, verified archive digest, current tracked-input and runtime/tool hashes,
reviewed exporter bytes and exact per-file output hashes. Only allowlisted public
report metadata is restored; unsafe paths, duplicates, symlinks, oversized archives
and missing or changing API metadata refuse reuse. The pinned objdiff consumer
is executed on the copied reports and must reproduce the captured validation
hashes. Reused queue receipts remain byte-for-byte historical receipts.
The producer and restoration guards require the checked-out commit to equal
the announced `GITHUB_SHA`, with both tracked working tree and index unchanged
before and after validation. A test that modifies tracked inputs cannot certify
the original queue commit. Python subprocesses use the same qualified interpreter.
The campaign-view check uses a cleaned temporary runtime outside the checkout,
as required by the maintained campaign guard; no runtime is placed in the repository.

The single `tests.yml` workflow now publishes the original decomp.dev artifact
names; the duplicated `progress.yml` workflow is removed. Main republishes the
verified same-SHA report bytes without regenerating the unique or reuse corpora.
If identity, queue provenance, tools, artifacts or consumer validation cannot be
verified, the job runs fresh full tests and exports. Maintainer source PRs do not
publish mass reports before the queue; other contributors still use full checks.
Actual external decomp.dev ingestion must be checked after rollout. No required
status name, review rule or local raw byte gate is weakened.

The manual `workflow_dispatch` trigger is retained on the unique workflow and
always runs full tests and exports. It cannot select the focused or reuse routes,
and a manual run cannot substitute for the required merge-group provenance.

The identity and provenance checks use GitHub's read-only
[workflow-run API](https://docs.github.com/en/rest/actions/workflow-runs) and
[merge-group event](https://docs.github.com/en/actions/reference/workflows-and-actions/events-that-trigger-workflows#merge_group).
