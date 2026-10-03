# Assert-message carriers: 597 recorded annotations

Measured on 3 October 2026 against the USA v1.01 retail programs identified by
[`config/target.json`](../config/target.json) and
[`config/overlays.json`](../config/overlays.json).

The second naming pass adds **495 function annotations, using 33 distinct labels,
across 25 level overlays**. Combined with the previous 102 entries, the public table
contains **597 unique `(program, address)` entries**. Every one of the 495 new entries
and every one of the 102 previous entries was compared with the current Ghidra function
name at the exact entry address; all 597 agreed. The two passes have no overlapping keys.

[`assert-message-names.tsv`](assert-message-names.tsv) contains only program identifiers,
virtual entry addresses and analyst-assigned structural labels. It contains no game
messages, extracted instructions or assets.

## What the labels mean

These are annotations of functions that reference a diagnostic message. They are
**not original source symbols**, inferred function signatures, or proof that the
entire function implements the condition named in its label. A reference can occur in
one error branch of a much larger routine. The `Report` suffix describes the observed
diagnostic relationship; it does not establish that reporting is the routine's only job.

For example, level 0 entry `0x00369008`, currently labelled `TfragTextureOverflow`,
builds a DMA chain and checks a threshold within that work. Level 0 entry `0x00321878`,
labelled `BoltsSpawnNegativeCount`, contains a larger spawning routine. These labels
are useful search anchors and should be refined only after reviewing the whole body.
They must not be promoted directly to decompilation API names.

## Provenance and checks

The private second-pass journal records 495 successful renames and zero failures.
Its 495 rename records have 495 distinct program/address keys. The input selection
has 33 diagnostic patterns, each associated with one analyst label. The pass visited
the boot and 25 overlays; the boot contributed **zero new entries**, because its
relevant functions already had names. Thus 26 programs visited is compatible with
25 programs contributing to this export.

The naming script preserved existing non-generic names and rejected a function when
the selected patterns supplied more than one different label. Its search used the
first matching occurrence of each selected pattern in a program and followed existing
Ghidra references. This is a narrower property than proving that the function contains
exactly one diagnostic string in the entire binary. The export verification establishes
name fidelity; it does not repair missing references or certify every semantic decision.

The previous pass contributed 102 annotations across 25 overlays. Its independent
class-identifier cross-check remains documented in
[`MOBY-DISPATCH-TABLES.md`](MOBY-DISPATCH-TABLES.md). Functions with conflicting
diagnostic labels and names already assigned by another method were preserved.

No game text is necessary to reproduce the application step. The privately owned
retail image and analysis database remain the source for semantic review.

## Applying or verifying the table

[`ApplyAssertMessageNames.java`](../scripts/ghidra/ApplyAssertMessageNames.java)
accepts a program identifier and the TSV path. It previews by default; the explicit
third argument `--apply` enables renaming. It checks the complete level `.text`
SHA-256 against its recorded retail fingerprint before processing names, resolves a
virtual-address or raw-file-offset import only when exactly one mapping matches,
requires a function at the exact entry address, and never overwrites an existing
non-generic name. Conflicting rows fail before any renames.

Example arguments for the level 0 overlay:

```text
0_aranos_tutorial docs/assert-message-names.tsv
```

With `--apply`, review the summary and save the program explicitly after inspecting
the changes. The script does not create functions, infer signatures, touch the boot,
transfer prototype addresses, or apply the cross-build anchor survey.

## Separate cross-build evidence

The Aug6/retail survey is qualified separately in
[`AUG6-RETAIL-ANCHORS.md`](AUG6-RETAIL-ANCHORS.md). Matching code windows, diagnostic
annotations and class-table identifiers are different forms of evidence. None of them
alone establishes a byte-exact C reconstruction or a playable runtime.
