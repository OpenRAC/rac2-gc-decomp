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

Thirteen ordered fragments in `src/boot/` group layouts, resident accessors,
utilities, object operations, callbacks and the packed-pixel decoder. The
generator concatenates them without introducing includes, line directives,
whitespace or additional compiler invocations. Types and declarations retain
their original order and scope. The output is still one `boot.c` translation
unit containing 178 catalogued definitions.

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
family has 26. Two hundred sixty-two contextual native bodies remain distinct in the
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
| Boot | 178 definitions | 9,336 |
| Native overlays | 270 family-or-singleton items, 271 contextual variants | 28,912 across 477 placements |
| Clear-family pilot | 2 textual variants, 24 representative bytes | 648 across 27 placements |

Native representative catalogued bytes total 19,728, or 19,752 when both clear
variants are retained separately. These are organization metrics, based on
explicit source membership, not a new machine-equivalence claim. This inventory
excludes replicated common boot coverage in overlays and is not a replacement
for the full game's progress denominator.

The current integration exporter accepts **240,360 / 48,788,176 bytes (0.4927%)**.
The latest matching lot adds 78 placements totaling 7,576 bytes: 23 signed
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

Nine canonical fragments supply the scalar bodies with explicit placements.
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
