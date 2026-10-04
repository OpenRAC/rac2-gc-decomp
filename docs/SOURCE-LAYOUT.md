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
family has 26. Forty-seven contextual native bodies remain distinct in the
inventory. The clear family and the floating-field guard have canonical shared
fragments. The guard retains explicit per-program helper bindings; its 27 forms
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
| Native overlays | 55 family-or-singleton items, 56 contextual variants | 12,872 across 262 placements |
| Clear-family pilot | 2 textual variants, 24 representative bytes | 648 across 27 placements |

Native representative catalogued bytes total 3,688, or 3,712 when both clear
variants are retained separately. These are organization metrics, based on
explicit source membership, not a new machine-equivalence claim. This inventory
excludes replicated common boot coverage in overlays and is not a replacement
for the full game's progress denominator.

The current integration exporter accepts **224,320 / 48,788,176 bytes (0.4598%)**.
The latest matching lot adds 27 complete 92-byte floating-field guards, with all
prior controls exact and fresh boot plus 27 overlay gates. The previous lot added
the complete 76-byte Ship Shack array traversal.

The independently requalified 80-byte signed-weight loop adds no credit: all 27
placements were already integrated through boot-shared `FUN_0027F128`. A proposed
native alias for each placement triggered the integration overlap guard. The
private trials and refusal are retained; the redundant native rows and fragments
are excluded from the current source inventory and matching total.
Refactoring sources adds zero matching credit. The earlier pilot regenerated all 28
standalone C files identically; boot/native catalogs, checker and compiler profile
retain their existing bytes. Complete image checks remain the final acceptor.
