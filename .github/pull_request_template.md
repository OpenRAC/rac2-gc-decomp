<!-- Required format: docs/PR-DESCRIPTIONS.md. Replace TODOs; keep headings and
field names. Use English and report only checks actually run. Commands use
portable placeholders, never private paths. This check does not prove claims. -->

## Summary

TODO: Explain the concrete problem and resulting behavior.

## Scope

- Type: TODO (matching, placement, nonmatching, tooling, documentation; comma-separated)
- Validated base: TODO (full upstream RAC2 commit SHA used for validation)
- Reservation: TODO (acknowledged issue/claim; or N/A: reason for non-decomp work)
- Targets: TODO (symbols, programs and source modules; or N/A: reason)

## Validation

<!-- Repeat Command/Result pairs as needed. Include failures and skipped checks.
Use repository paths or <reference>, <toolchain>, <runtime> placeholders. -->
- Command: TODO
- Result: TODO (actual outcome, counts and relevant report location)

## Matching evidence

<!-- Matching/placement changes fill every field. Otherwise use N/A: reason.
Nonmatching changes report zero deltas. Distinguish physical loaded coverage
from unique code. No matching credit for ASM or approximate C. -->
- Proofs: TODO (versioned proof paths, symbols and source/checker hashes)
- Physical delta: TODO (signed byte count; before/after when available)
- Unique delta: TODO (signed byte count; before/after when available)
- Gates: TODO (complete symbols and affected full images; disclose failures/skips)
- ABI review: TODO (arguments, returns, globals/callers checked and unknowns)

## Risks and follow-up

TODO: State limitations, negative trials and remaining work. Use None only when justified.

## Checklist

- [ ] Description reflects this head and the validated combined source.
- [ ] Only authored source and public proof metadata are included; no game bytes, proprietary tools, secrets or private runtime data.
- [ ] Provenance and reused work are identified; incomplete or skipped validation is disclosed.
