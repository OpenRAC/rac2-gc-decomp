# Authored source and generated compilation units

`src/` is the authoring tree. `candidates/` contains the standalone translation
units consumed by the current compiler and exact gates. Recipes in
[`config/source-layout.json`](../config/source-layout.json) bind every fragment,
placement substitution, catalog and generated unit by SHA-256 and byte length.
Check freshness before a trial, a build or a publication:

```powershell
python scripts/source_layout.py --check --inventory-check progress/source-inventory.json
```

## Progress display by authored module

The physical decomp.dev artifact groups accepted function placements by program,
proof owner and their verified authored source fragment. Each level has its own
category. Shared placements stay separate in every physical program; this display
does not deduplicate matching credit or recover original retail object boundaries.
Level categories include all scoped code and data, including unmatched remainders;
source, owner and role categories describe only the accepted subset.

The grouping runs after the ordinary integration/object proof validators. It
retains the exact function and section records, unmatched section remainders,
data coverage and all byte measures. Display unit counts describe source groups,
not the number of proved function placements. A displayed source group covers
its **accepted C subset**; an unqualified sibling in the same source file, such
as the retained libgcc `fptodp` attempt, gains no credit from that group's status.

Verified source paths point to `src/`. Ambiguous source attribution remains a
separate unresolved unit. Original roles default to `unclassified`; reviewed
assignments belong in `config/progress-modules.json` with explicit provenance.
Names and addresses alone are not evidence of a game subsystem. The optional
`source-modules.json` artifact records presentation provenance and the preserved
function-placement count; it is separate from the matching proofs.
The default descriptor has no assignments: complete source attribution does not
mean that the original roles have been classified.

The conservative unique-code artifact and its denominator remain unchanged.
Categories are overlapping filters, so summing category totals is not a global
progress calculation. The legacy ungrouped CLI remains available:

```text
python scripts/decomp_report.py --output <physical-report> --level-proof <level-proof> ...
python scripts/progress_module_report.py --output <grouped-report> --source-module-summary <presentation-summary>
```

## Boot pilot

The ordered core fragments in `src/boot/` group layouts, resident accessors,
utilities, object operations, callbacks and the packed-pixel decoder. The
generator concatenates them without introducing includes, line directives,
whitespace or additional compiler invocations. Types and declarations retain
their original order and scope. The output is still one `boot.c` translation
unit containing 241 catalogued definitions in the 8 October 2026 source snapshot. The recipe also includes the
vendored libgcc source in `src/libgcc/fp-bit-ee.c`; its one accepted definition
is upstream library code, separately licensed, rather than campaign-authored
code. Its complete notices also accompany the generated copy in `boot.c`.
The dual-prime core fragment contains the mechanically adapted MIT-licensed Lombyte
dual-prime motion-vector body, with its complete notice. Its provenance and
independent complete-body acceptance are recorded in
[`MPEG-DUAL-PRIME-EVIDENCE.md`](MPEG-DUAL-PRIME-EVIDENCE.md).
Two further attributed core fragments contain the unchanged temporary-track
update and IPU synchronization bodies. Their provenance, original mixed trial
and final whole-unit qualification are recorded in
[`SDK-TRACK-IPU-EVIDENCE.md`](SDK-TRACK-IPU-EVIDENCE.md).

These are authored organizational boundaries. They do not establish original
retail modules, object files or independently compilable units. Independent
object splitting remains a separate experiment requiring new complete proofs.

The separately qualified SDK `_sysbitFlush` body is authored as
`src/sdk/sysbit_flush.c` and rendered byte-identically to
`candidates/sdk/sysbit_flush.c`. Its source-specific catalogue and review do not
change the default GNU unit or its shared overlay placements. The boot
integration records both owners explicitly and compares each complete object
before the full loaded-image gate. See [the source and ownership evidence](SDK-SYSBIT-EVIDENCE.md).
The additional `src/sdk/cpr8_source_unit.c` source is rendered under the same
bare filename and has its own complete-object, four-call relocation and opaque
helper ownership proof. See [CPR8 evidence](SDK-CPR8-EVIDENCE.md). The three SDK
units contribute 1,144 catalogued bytes without changing default GNU or native
sources. The third unit is the unchanged attributed IPU DMA restart body; see [its evidence](SDK-RESTART-DMA-EVIDENCE.md). These authored units do not establish original retail object boundaries.

## Native family pilot

`src/levels/shared/clear-five-words.cfrag` supplies the canonical five-word clear
for 26 programs. Explicit recipe substitutions provide each program's measured
function symbol. Ship Shack retains its original `int` spelling and formatting
as a separate source variant: all 27 existing outputs remain byte-identical.
A shared type prelude is reused where its exact text agrees. The remaining
authored bodies stay in per-program fragments under `src/levels/placements/`.

The current source inventory records 132 explicit base C families with 1,516
placements. Another 2,114 contextual forms remain separate. Identity includes
the recorded helper and data addresses: sharing a fragment does not merge
unexplained contexts or establish original source ownership.

The clear family, floating-field guard and GS setup retain their explicit
per-program bindings. The [8 October native batch](NATIVE-PARENT-40K-EVIDENCE.md)
adds eight noncalling exact-source anchors grouping 220 placements, while its
297 call-bearing contextual forms and two single placements remain separate.

Two further shared fragments provide the 32-byte entity bit setter and the
16-byte signed-cell comparison, each with 27 function-name substitutions.
Their complete-unit qualification and retained refusals are described in
[the scalar leaf pair evidence](SCALAR-LEAF-PAIR-EVIDENCE.md). These fragments
do not establish original object, class or module boundaries.

## Separate small-data units

Each of the 27 overlays has a separately qualified small-data unit, authored as
one complete per-program module under `src/levels/smalldata/`. A concrete module
is copied once; the original single-function pilot template remains supported.
Capture rejects ambiguous templates, unresolved tokens and source/module
disagreement. Grelbin's old fragment remains retained, and its previous generated
unit is an exact prefix of its new complete module.

The default native profiles, including Barlow's existing G8 profile, are
unchanged. Separate small-data catalogues and reviews record their own explicit
GP, bindings and complete objects. See the [46,312-byte batch evidence](NATIVE-GP-46K-EVIDENCE.md). The latest [41,180-byte scalar batch](NATIVE-SCALAR-41K-EVIDENCE.md) also validates a bounded integer declaration context.
These are authored ownership boundaries, not recovered retail modules.

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

The checked [source inventory](../progress/source-inventory.json) records the
following snapshot after the latest 8 October 2026 scalar-state and command batch.
Its organization counters cover boot/SDK/default native sources. The separate
small-data row below comes from validated catalogues and is excluded from those
organization counters; all 58 generated sources are verified.

| Scope | Authored source | Replicated catalogued bytes |
| --- | --- | ---: |
| Default GNU boot | 241 catalogued definitions | 12,312 |
| Separate SDK boot units | 3 attributed definitions with complete-object proofs | 1,144 |
| Default native overlays | 2,246 family-or-singleton items, 2,247 contextual variants | 329,600 across 3,630 placements |
| Separate small-data overlays | 27 complete per-program modules, 1,347 definitions | 76,388 |
| Clear-family pilot, included in native scope | Two textual variants, 24 representative bytes | 648 across 27 placements |

Native representative catalogued bytes total 164,968, or an upper bound of
164,992 when both clear variants are retained separately. These organization
metrics exclude replicated common boot coverage and do not replace progress.

The prototype-labelled callback 775 has one canonical shared fragment and 27
explicit function/helper bindings. Its neutral partial field view and observed
ABI are documented in [the family evidence](UPDATE-MOBY775-76-EVIDENCE.md).
Its helper addresses remain contextual inventory operands, so sharing a source
fragment does not itself collapse those entries or prove machine equivalence.

The current integration exporter accepts **659,380 / 48,788,176 bytes**:
244 boot functions contribute 13,456 bytes; 9,373 overlay placements
contribute 645,924 bytes. The latest batch adds **1,003 complete placements /
41,180 new physical C bytes**, following 53 complete translation-unit gates over
4,846 old and new definitions. Reserved worker targets and equivalent copies remain excluded.

Fresh boot and all 27 overlay PT_LOAD bytes and metadata pass under campaign
`de67b0a1d85e444189796aef63509496`: 79,486,851 loaded bytes were compared. Existing default profiles and
the separate small-data profile retain their measured settings. Observed source
effects are recorded; original game roles and complete object boundaries remain unknown.

The conservative all-members unique measure is **161,316 / 44,408,040 bytes**.
Catalogue reconstruction and pointer-theorem replay pass over the current scoped
inventory. See the [unique report](../progress/unique-code-report.json) and
[latest batch evidence](NATIVE-SCALAR-41K-EVIDENCE.md). Static target classes and
retained unowned data operands can keep copies separate. Source identity alone
does not establish original source, data-object, module or runtime equivalence.
