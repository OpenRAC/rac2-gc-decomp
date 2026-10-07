# Interactive trial review and batch finalization

These commands use the existing campaign register and exact validators. Complete
the normal contributor setup first. Keep reference images and SDK/toolchains
private. Store objects, logs, HTML reviews and staging directories outside every
Git checkout.

## Review a recorded trial

```text
python scripts/campaign.py --runtime <private-runtime> diff <task-id> --serve --open
```

The command reads the task's last immutable trial. It verifies the registered
manifest/outcome hashes and the frozen source, object, catalogue, reference and
linked-image pins before creating a private standalone HTML review. It does not
compile, edit source, change the register, accept an experiment or add credit.

The review provides:

- Target and complete-function selectors, initially preferring the task's
  selected symbols over its existing controls.
- Reference and candidate instruction bytes and R5900 disassembly side by side.
- Every differing, missing or extra byte, without address masking, fuzzy
  alignment, prefix comparison or trimming of trailing instructions.
- Previous/next difference navigation and an only-differences filter.
- The immutable whole-unit C snapshot, hashes and recorded trial state.

Select a historical event or a particular function explicitly:

```text
python scripts/campaign.py --runtime <private-runtime> diff <task-id> --trial <trial-id> --target <target-id> --symbol <function-symbol> --output <new-private-review.html>
```

Omitting `--serve` creates an offline HTML file with no external assets. The
optional server binds only `127.0.0.1`, serves one randomly addressed read-only
document and refuses directory access and writable requests. `--port 0` chooses
an unused local port; `Ctrl+C` stops the server. `--open` requires `--serve` and
opens the local browser only when explicitly requested.

All views are historical diagnostics. A selected complete body may be raw exact
while another function or generated readonly section prevents its whole unit
from matching. A current task may already be integrated without making the
historical source snapshot the current public compilation unit. The viewer does
not decide either form of acceptance.

If linking failed, the viewer may show the pinned relocatable object as an
explicitly unplaced diagnostic. Unresolved relocations or missing/invalid
symbols cannot produce a raw-exact label. Compile failures have no measured
candidate body. Preparation-rejected events with no candidate snapshots are
refused with an explanation; their recorded failure remains in the register.

## Finalize a completed full-image batch

First qualify the current source units and execute the normal full integration:

```text
python scripts/campaign.py --runtime <private-runtime> integrate -- --manifest <preparation-manifest.json> --toolchain <ASM-root> --c-toolchain <C-linker-root> --sdk-binding <private-sdk-binding.json> --program-jobs 4 --jobs 2
```

Use the actual returned action UUID. `finalize` accepts only a registered passed
build/integration with boot and all 27 overlays, current source/profile/tool
inputs, and the immutable action manifest/outcome hashes recorded by the current
campaign CLI. Old actions lacking these pins remain historical evidence: run a
fresh integration instead of modifying their records.

One command prepares, validates and publishes the lot:

```text
python scripts/campaign.py --runtime <private-runtime> finalize <action-id> --manifest <preparation-manifest.json> --output <new-private-finalization-directory> --task <candidate-task-id> --apply
```

Repeat `--task` for each candidate that should close. Omit it when refreshing a
tooling-only batch with no new matching target. Research tasks retain their
explicit decisions and are not silently marked done. When the preparation
manifest does not use the conventional `boot.elf` and
`levels/<level>/overlay.elf` reference layout, supply `--references` with that
complete pinned reference tree.

The command stages a private source/metadata snapshot, then:

1. Checks the completed action, all 28 gate/reference/image artifacts and the
   current actual GNU/SDK instrument identities.
2. Validates and stages the complete boot/overlay proofs and physical export.
3. Regenerates function boundaries, pointer evidence, the global catalogue and
   its full reconstruction/theorem checks through maintained project commands.
4. Refreshes boot bindings, primary/paired/supplementary reports, README displays,
   the source inventory and register views.
5. Closes explicitly selected candidates through the existing proof validator,
   preserving research decisions and every negative trial.
6. Runs the tool tests and source/view/display freshness checks in the private
   snapshot, and scans proposed public outputs for private paths and credentials.
7. Checks the unchanged public/private inputs again, creates counted SHA-256
   before-backups and publishes only the allowlisted verified metadata.

Reference binaries, SDKs, extracted assembly, compiler objects, local HTML and
derived objdiff exports are never publication candidates. Counts derive from
the validated gates and exporters; the finalizer itself adds zero credit.

## Inspect or resume a prepared publication

Omit `--apply` to run preparation and checks without changing the public checkout.
The output contains numbered private logs, `review.diff`, `plan.json`,
`journal.json` and `receipt.json`. Run the same command with the same arguments
and `--apply` to publish that prepared plan. Expensive completed preparation
checks are retained; actual compiler instruments are observed again before
publication.

Publication refuses source/profile/tool drift, changed staged outputs and
concurrent register or generated-file edits. Before-images are hash-verified.
Publication requires Windows or Linux and a private output directory on the
same filesystem volume as the checkout. Windows uses native replacement with a
backup; Linux uses native rename exchange. Unsupported primitives fail safely.
Each replacement captures the actual displaced file and checks its expected
SHA-256 after the atomic operation. New paths are created without replacing an
existing file. Rollback uses the same capture checks.

A racing writer prevents successful publication. Displaced versions and
operation records remain in the private `publication-operations/` bank;
bounded restoration attempts preserve further racing versions too. Inspect a
conflict rather than automatically retrying it. This is guarded publication of
individual files, not an atomic transaction across the whole checkout or a
lock shared by arbitrary editors. Keep the batch inputs frozen throughout.
Repeating an applied invocation verifies its after-images without closing tasks
twice.

If preparation fails before a plan exists, inspect `failed-checks.json` and its
numbered log, correct the cause and use a new private output directory. Do not
overwrite the failed bank. An abruptly killed publisher may leave a register
lock; inspect its recorded owner and retained journal before recovery. Never
remove another process's live lock or bypass a conflict guard.

The finalizer does not stage Git files, commit, push or merge. Review its diff
and receipt, publish the scoped English commit/PR through the user's authorized
workflow, and require the current CI before merging.
