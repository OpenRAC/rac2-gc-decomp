# Authored source and generated compilation units

`src/` is the authoring tree. `candidates/` contains the standalone translation
units consumed by the current compiler and exact gates. Recipes in
[`config/source-layout.json`](../config/source-layout.json) bind every fragment,
placement substitution, catalog and generated unit by SHA-256 and byte length.
Check freshness before a trial, a build or a publication:

```powershell
python scripts/source_layout.py --check --inventory-check progress/source-inventory.json
```

## Boot pilot

Sixteen ordered fragments in `src/boot/` group layouts, resident accessors,
utilities, object operations, callbacks and the packed-pixel decoder. The
generator concatenates them without introducing includes, line directives,
whitespace or additional compiler invocations. Types and declarations retain
their original order and scope. The output is still one `boot.c` translation
unit containing 181 catalogued definitions.

These are authored organizational boundaries. They do not establish original
retail modules, object files or independently compilable units. Independent
object splitting remains a separate experiment requiring new complete proofs.

## Native family pilot

`src/levels/shared/clear-five-words.cfrag` supplies the canonical five-word clear
for 26 programs. Explicit recipe substitutions provide each program's measured
function symbol. Ship Shack retains its original `int` spelling and formatting
as a separate source variant: all 27 existing outputs remain byte-identical.
A shared type prelude is reused where its exact text agrees. The remaining
authored bodies stay in per-program fragments under `src/levels/placements/`.

The source inventory verifies eight base families by exact authored source after
recorded symbol substitutions. Seven have 27 placements; the five-entry table
family has 26. Seven hundred nineteen contextual native bodies remain distinct in the
inventory. The clear family, floating-field guard and GS buffer setup have
canonical shared fragments. The guard and GS setup retain per-program helper
bindings; their 27 forms each
remain separate inventory items because external addresses are part of the
current normalization. Expand factorization after
the same source-identity and complete-image checks, rather than assuming every
similarly sized body belongs to one family.

## Authoring and regeneration

1. Back up the affected fragments, catalogs, generated units and layout manifest
   outside the repository; record the exact file list and verify their hashes.
2. Edit the relevant fragment and its catalog deliberately. Inspect declaration
   context and ABI before changing types or adding a body.
3. Create a private JSON object containing the current hash of
   `config/source-layout.json` and every generated destination that will change.
   Include the current hash of any catalog or other pinned layout input changed
   during this edit. Hash values describe the files currently on disk.
4. Render a private preview, or replace the reviewed generated units:

```powershell
python scripts/source_layout.py --write --output <private-preview-directory>
python scripts/source_layout.py --write --expected-output-hashes <private-current-hashes.json>
python scripts/source_layout.py --inventory-output progress/source-inventory.json
python scripts/source_layout.py --check --inventory-check progress/source-inventory.json
```

The writer preflights all changed destinations and refuses an unexpected hash.
It refreshes recipes and inventory from the authored fragments, without slicing
them again at historical bootstrap markers. It cannot manufacture compiler proof.
Changed generated C invalidates existing source/object reviews and integration
proofs until their hashes and full loaded-byte gates are regenerated. Header
includes remain unsupported by the standalone qualification path.

`capture --layout <private-directory>` is a bootstrap/research operation, not the
normal authoring path. It does not replace the public authoring tree. Preview
artifacts and current-hash files remain private.

## Separate source and coverage metrics

The checked [`source inventory`](../progress/source-inventory.json) currently records:

| Scope | Authored source | Replicated catalogued bytes |
| --- | ---: | ---: |
| Boot | 181 definitions | 9,564 |
| Native overlays | 781 family-or-singleton items, 782 contextual variants | 94,772 across 988 placements |
| Clear-family pilot | 2 textual variants, 24 representative bytes | 648 across 27 placements |

Native representative catalogued bytes total 85,588, or 85,612 when both clear
variants are retained separately. These are organization metrics, based on
explicit source membership, not a new machine-equivalence claim. This inventory
excludes replicated common boot coverage in overlays and is not a replacement
for the full game's progress denominator.

The current integration exporter accepts **311,416 / 48,788,176 bytes (0.6383%)**.
The latest lot adds two complete 52-byte float families, one per program and
2,808 bytes in total. An indexed parameter-row setter stores four row components
and one weight through ordinary typed field assignments; a nested grid setter
stores four components for a row inside a group. Both shapes are byte-identical
in all 27 programs. Their argument registers are proven from every actual
caller: five floats with the object and a signed index, or four floats with the
object and two signed selectors.

Two authored fragments retain explicit per-program function bindings. All 988
complete native functions match, including 934 controls. Generated source and
catalog bytes equal the immutable qualifications, and every original flag, GP
and external binding remains unchanged, including Barlow G8. Fresh boot and all
27 loaded-byte and metadata gates, independent exports and the full test suite
qualify integration.

The preceding lot added 27 complete 164-byte point-index append/update routines,
totaling 4,428 bytes. A byte index is appended through the observed descriptor,
its byte count increments, and original registration and geometric helpers
update the descriptor. The registration helper consumes a third ordinary
direction-pointer argument, proven before the first C trial. The final byte
count is returned explicitly; no incidental helper result is inferred. One
authored fragment retains explicit per-program bindings; the original four
helpers and their MMI/VU work receive no additional C credit.

The preceding lot added six complete 64-byte record-pointer writers, totaling
384 bytes. A zero initial marker skips the scan. Otherwise the full selector
word and signed promoted key select the first matching 80-byte record; its
payload pointer is written, with the original marker-one termination order.
All seven actual callers prove four ordinary arguments and an unused result.

One authored fragment retains explicit per-program symbols. All 204 complete
functions in the six units match, including 198 controls; generated sources
and catalogs equal immutable qualifications. Barlow keeps its qualified G8
profile and every existing external binding is unchanged. The whole-body scan
excludes the other 21 programs. Fresh boot and all 27 loaded-byte and metadata
gates, independent exports and the full test suite qualify integration.

The preceding lot added one 44-byte boot double constructor, 27 selection-grid
updates of 312 bytes, 27 slot reservation/reuse routines of 84 bytes and three
clamped local-target angle routines of 252 bytes, totaling 11,492 bytes.
The grid event helper's signed return is independently proven by an actual
consumer; correcting only its declaration restores the original allocation.
The earlier void-prototype refusal remains recorded. Slot updates preserve
separate sixteen-entry object and state arrays and return a signed index.

All 27 compound source units are newly qualified together: 901 whole native
functions match, including 844 prior controls. The boot constructor qualifies
181 complete functions including 180 controls, with original packing helpers
remaining external. The angle family excludes the other 24 programs; its
original math helpers and coefficient table receive no additional credit.
Generated source/catalog bytes equal the immutable final qualifications.
Fresh boot and all 27 loaded-byte and metadata gates, independent exports and
the full test suite qualify integration. Per-program flags and GP are unchanged.

The preceding lot added two complete 156-byte status queries in Gorn and Hrugis
Cloud, totaling 312 bytes. Each reports whether any of eleven selected
40-byte records has a nonzero first word. Actual callers use no arguments and
test the integer result. The complete body scan excludes the other 25 programs.

One authored fragment retains explicit per-program function and status-array
bindings. Both generated units and catalogs equal immutable qualifications:
65 whole functions match, including 63 previous controls. Existing profiles
and GP are unchanged; only the measured status-array externals are added.
Fresh boot and all 27 loaded-byte and metadata gates, independent exports and
the full test suite qualify integration. No data or helper credit is added.

The preceding lot added 27 complete 64-byte scalar interval predicates, totaling
1,728 bytes. The scalar must lie below a resident plane within its configured
depth. The original ordered comparisons reject unordered values. All 308
actual caller sites confirm the single F12 argument and integer result.

Two authored fragments retain the 26 array-based resident projections and
Ship Shack's existing typed-root address expression. Every generated unit and
catalog is byte-identical to its immutable qualification: all 842 whole native
functions match, including 815 controls, with unchanged flags, GP and externals.
Fresh boot and all 27 loaded-byte and metadata gates, independent exports and
the full test suite qualify integration. No helper or data credit is added.

The preceding lot added 28 complete 64-byte resident channel updates and 27
complete 396-byte record text substitutions, totaling 12,484 bytes. Channel
flag bit fifteen controls a halfword value update in the observed order.
Text substitution copies a localized string to its first percent selector,
formats a signed mapped-record value for selector `b`, then copies the
replacement and suffix. Original string and variadic formatting helpers remain
external and receive no matching credit.

All 4,334 whole shared-unit measurements and 815 complete native functions
match, including every previous control. Generated source and catalogs are
byte-identical to the immutable qualified units, with unchanged tool hashes,
per-program flags and GP. Fresh boot and all 27 loaded-byte and metadata gates,
independent coverage exports and the full test suite qualify integration.

The preceding native lot added two complete 252-byte linked-part updates in
Dobbo and Yeedil, totaling 504 bytes. A measured descriptor supplies two optional
parts and a guarded parent; original attachment/color helpers remain external.
The complete scan excludes the other 25 overlays, and both current units
qualified 58 exact complete functions including 56 controls before integration.

The preceding resident GS configuration lot added 28 complete 120-byte
placements, totaling 3,360 bytes. Its authored boot source configures volatile
64-bit privileged registers using three measured resident configuration words.
All boot and overlay image bytes and metadata match; gameplay remains unverified.

The strict integration parser now indexes original definition positions once,
selects catalog entries from one complete definition scan and renames crossing
local labels in one pass. Duplicate, missing or noncontiguous definitions and
invalid body addresses remain errors. Complete original and indexed splitter
pieces agree in a measured 180-function, 326-piece sample; regression tests
cover duplicate definitions, longer-symbol near misses and cross-object label
ownership. No input cache or acceptance relaxation is introduced. Tooling
changes add zero C matching credit.

The preceding matching lot added 144 complete placements totaling 15,416 bytes:
27 mapped-class queries of 84 bytes, 27 serialized-header relocation and row
compaction bodies of 300 bytes, 27 resident-mode predicates of 60 bytes,
19 pairs of 56-byte object flag setters and clearers, and 25 conditional
eight-slot resets of 52 bytes. Joint flag sources share their declarations
once; measured Ship Shack storage spellings retain separate exact variants.
All 759 complete native functions match, including 615 prior controls, followed
by fresh boot and all 27 loaded-byte and metadata gates. Compaction calls the
original measured PLZCW helper as an external and adds zero helper credit.
The reset family excludes Barlow and Notak because one raw caller in each lacks
a stored complete function boundary; no placement or credit is inferred.
Current per-program flags, GP and far-data attributes remain qualified.

The preceding matching lot added three complete families across all 27 overlays:
88-byte record-object searches, 160-byte object-class filters and 96-byte
kind-pointer selectors. The 81 new placements total 9,288 bytes. All 615
complete native functions match, including 534 prior controls, with fresh
combined source/object qualifications and boot plus all 27 loaded-byte and
metadata gates. Ship Shack retains its measured typed-root expression and
removes only the external binding now supplied by its new definition; Barlow
retains its qualified G8 profile and three far-pointer NOSDA declarations.
These explicit storage and call bindings do not infer original module boundaries.

The preceding matching lot added 27 selected-object row override loops of 192
bytes, totaling 5,184 bytes. Each signed sentinel list selects objects with
measured byte counts and row pointers. A row key indexes two signed halfword
overrides; nonzero values are merged with retained upper word bits. All 534
complete native functions in the current 27 units match, including 507 prior
controls, with source/catalog bytes identical to their immutable qualifications.
Fresh boot and all 27 loaded-byte and metadata gates pass. No field projection
or canonical fragment establishes an original source name or object boundary.

The preceding pair/query lot added 29 placements totaling 2,104 bytes: 27 two-key
table lookups of 68 bytes, a 188-byte Joba counter increment and an 80-byte
indexed polygon-query dispatcher. All 507 complete native functions in the
final 27 units match, including 478 controls; the combined Joba source contains
21 exact functions. The other 26 units preserve their previously qualified
source/catalog bytes exactly. Fresh boot and all 27 loaded-byte and metadata
gates pass. The two Joba bodies are absent in the other programs; the existing
204-byte crossing-parity helper receives no additional credit.

The preceding singleton lot added one 268-byte Joba class-counter consumer. It
selects one of six measured signed state counters, decrements a positive count,
and returns success. All 18 complete functions in the current Joba source match,
including 17 controls. Its raw body is absent from the other 26 programs, which
gain no inferred placement. Fresh boot and all 27 overlay loaded-byte and metadata
gates pass; the total native catalog holds 478 complete placements.

The preceding parallel lot added 78 placements totaling 7,576 bytes: 23 signed
index clamp/wrap algorithms of 100 bytes, 27 record-key updates of 116 bytes,
27 resident bitmap getters of 72 bytes and the 200-byte Barlow queue launcher.
The three independently qualified worker packets were combined with the launcher;
all 477 complete functions in the final 27 standalone units match. Fresh boot
and all 27 overlay loaded-byte and metadata gates pass. The index shape is absent
in four programs, which receive no inferred placement or credit.

Barlow uses the separately qualified small-data profile and the measured GP;
its far four-byte control object has an explicit NOSDA binding. Boot and the
other 26 native programs retain the default flags. Reviews and loaded-image
proofs bind each program's actual flags, source and checker. This does not infer
original SDK compilation flags or recovered object boundaries.

The previous lot added nine complete 36-byte scalar cubic helpers, totaling
324 bytes. Their float operation order is preserved; the shape is absent in
18 programs and no copies are inferred there.

The previous scalar lot added 101 placements totaling 4,468 bytes: 15 output
wrappers of 56 bytes, 27 status classifiers of 56 bytes, 27 classifiers of 36
bytes, 27 object-identifier checks of 32 bytes and five header getters of 56
bytes. Earlier lots added 27 136-byte GS buffer setup wrappers, 27 92-byte
floating-field guards and the 76-byte Ship Shack traversal.

Twenty-two canonical fragments supply the scalar bodies with explicit placements.
Status classifiers call each unit's existing indexed status getter definition;
the output wrappers bind measured per-program floating helper addresses.
The output wrapper is absent in 12 programs and the header getter is absent in
22 programs; no placement or credit is inferred there. The inventory's eight
base-family categories remain fixed, so the added scalar bodies remain contextual
items in its conservative counters despite their canonical fragment reuse.
This bookkeeping distinction does not change the exporter or matching numerator.

The independently requalified 80-byte signed-weight loop adds no credit: all 27
placements were already integrated through boot-shared `FUN_0027F128`. A proposed
native alias for each placement triggered the integration overlap guard. The
private trials and refusal are retained; the redundant native rows and fragments
are excluded from the current source inventory and matching total.
Refactoring sources adds zero matching credit. The earlier organization-only pilot regenerated all 28
standalone C files identically while preserving its catalogs, checker and flags.
The later small-data extension separately qualifies per-program native flags. Complete image checks remain the final acceptor.
