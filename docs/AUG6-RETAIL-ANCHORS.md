# Aug6 / retail code windows: qualified correspondences

Measured on 3 October 2026. The survey compares the August preview level code with
the pinned USA v1.01 retail overlays, **within the same level identifier**.

[`aug6-retail-anchors.tsv`](aug6-retail-anchors.tsv) lists 3,359 retail function
windows with a matching prototype window. It carries addresses, sizes, structural
labels and qualification status, without extracted code or game messages.

## Rechecked measurement

| Property | Count |
| --- | ---: |
| Retail functions in the ten source-program dumps | 34,661 |
| Eligible rows actually emitted by the original scanner | 33,252 |
| Retail windows with at least one masked match | 3,359 |
| Sum of retail window sizes | 390,712 bytes |
| First selected match identical without masking | 2,208 |
| Exactly one masked occurrence in the prototype section | 2,552 |
| More than one masked occurrence | 807 |
| Unique masked occurrence with no first-destination reuse | 2,512 |
| Rows whose first selected destination is reused by another row | 837 |

The original scanner skipped windows shorter than 16 bytes, out-of-section windows
and other ineligible functions; this explains why the TSV denominator differs from
the full function dump. The 3,359 matches are 9.69% of the full dump, approximately
336 windows per level. They are a broader correspondence survey than the existing
82-function-per-level placement catalogue, but they are **not 3,359 validated
function-to-function naming anchors**.

The mask clears only the 26 target bits of MIPS `j` and `jal`. Equality after this
mask is weaker than raw-byte equality: unrelated thin wrappers can become identical
when their call destinations are removed. Literal address construction through
`lui` and subsequent operations is not masked. The method misses other legitimate
correspondences, and its raw count also includes ambiguous windows.

All 3,359 originally selected windows were rechecked, and every selected window
still agrees under the documented mask. All aligned occurrences were enumerated;
the public table includes their virtual addresses. `exact_at_first` concerns the
original first selection, while `exact_occurrences` counts raw-equal alternatives.
`first_destination_reuse` counts reuse of that selection inside one level; it does
not by itself prove that the functions share semantics.

An independent word-by-word control found 2,433 changed words across these
selections: every change is confined to direct jump/call target bits, with zero
opcode changes and zero unexplained word differences. The mask retains the full
six-bit opcode (`0xFC000000`).

## Class identifiers resolve 20 ambiguous selections

Only 41 matched retail entries already carry a structural name in the current
Ghidra database. They use five structural role labels; every one relies on masked
equality rather than raw equality.

The privately owned prototype and retail dispatch records were independently read
by identifier and role. For **all 41 entries**, exactly one of the prototype matches
has the same class or camera identifier and role as the retail function. This
resolves **20 previously ambiguous windows** whose first byte-pattern occurrence
was not their corresponding dispatch handler. The remaining 21 named windows were
already unique under the mask.

[`aug6-retail-dispatch-qualified.tsv`](aug6-retail-dispatch-qualified.tsv) records
those 41 selected correspondences. Prototype CSV exports examined as data corroborate
all 41 addresses; no third-party script was executed. The independent evidence is
the class/role identifier in the two privately owned dispatch tables, using the
record layouts documented in [`MOBY-DISPATCH-TABLES.md`](MOBY-DISPATCH-TABLES.md).

This recovers known structural relationships, including `UpdateMoby_775`,
`UpdateMoby_2384`, `UpdateMoby_3077`, `UpdateCamera_6` and `InitCamera_24`.
It produces **zero new reliable semantic names**. No Ghidra name was changed.

## Address and transfer limits

Prototype addresses are calculated as the extracted code section's virtual base
plus the section-relative match offset. Retail addresses are ELF virtual addresses,
checked against the current program's function entries. Raw-file-offset imports
require their own translation. A level identifier is part of every key: numerical
address equality across overlays, or between an overlay and the boot, is not identity.

The prototype code was not open as a separate program in the queried Ghidra
instance. Thus a unique unqualified window can still begin inside a prototype
function, or overlap another window. The complete prototype function boundary and
the call-target relationship have not been verified for the generic candidates.

To promote a candidate, first identify the exact prototype program and function
entry, inspect its complete body and surviving call targets, and check independent
class/role evidence when available. Preserve one-to-many aliases, reject unresolved
collisions, and keep source-symbol claims separate from analyst labels. Reviewed C
still needs the project's complete-symbol byte-exact gate and normal validations.
The 3,359-window table is evidence for that review workflow, never an automatic
renaming input or a global fixed-offset mapping.
