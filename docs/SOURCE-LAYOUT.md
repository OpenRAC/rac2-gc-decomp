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

Nineteen ordered core fragments in `src/boot/` group layouts, resident accessors,
utilities, object operations, callbacks and the packed-pixel decoder. The
generator concatenates them without introducing includes, line directives,
whitespace or additional compiler invocations. Types and declarations retain
their original order and scope. The output is still one `boot.c` translation
unit containing 187 catalogued definitions. The recipe also includes the
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
change the default 187-function unit or its shared overlay placements. The boot
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

The source inventory verifies eight base families by exact authored source after
recorded symbol substitutions. Seven have 27 placements; the five-entry table
family has 26. Nine hundred six contextual native bodies remain distinct in the
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

The checked [source inventory](../progress/source-inventory.json) records:

| Scope | Authored source | Replicated catalogued bytes |
| --- | ---: | ---: |
| Default GNU boot | 187 catalogued definitions (183 campaign-authored, four attributed definitions) | 10,552 |
| Separate SDK boot units | Three attributed definitions with complete-object proofs | 1,144 |
| Native overlays | 941 family-or-singleton items, 942 contextual variants | 109,380 across 1,148 placements |
| Clear-family pilot | Two textual variants, 24 representative bytes | 648 across 27 placements |

Native representative catalogued bytes total 100,196, or 100,220 when both clear
variants are retained separately. These organization metrics exclude replicated
common boot coverage and do not replace the game's progress denominator.

The prototype-labelled callback 775 has one canonical shared fragment and 27
explicit function/helper bindings. Its neutral partial field view and observed
ABI are documented in [the family evidence](UPDATE-MOBY775-76-EVIDENCE.md).
Its helper addresses remain contextual inventory operands, so sharing a source
fragment does not itself collapse those entries or prove machine equivalence.

The current integration exporter accepts **346,192 / 48,788,176 bytes**:
190 boot functions contribute 11,696 bytes; 5,383 overlay placements contribute
334,496 bytes, including 1,148 native functions. The latest lot adds 27 complete
76-byte callbacks and one independently qualified 336-byte SDK IPU restart body,
for 2,388 additional loaded C bytes. All existing controls remain exact.
The default GNU object and both previous SDK objects retain their prior hashes.
Barlow retains its G8 profile and fixed GP; other native units retain G0.

Fresh boot and all 27 overlay PT_LOAD bytes and metadata pass under campaign
`790b5eb477624821be9ed3fcdc3c615d`. The prior batch's boot symbol-dispatch refusal
and all earlier source/instrumentation refusals remain retained evidence. No
helper implementation, source permutation or generic SDK profile is credited.

The conservative all-members unique measure is **89,964 / 44,451,612 bytes**.
All 109,725 function reconstructions and the complete pointer-theorem replay
pass. Static target classes and retained unowned data operands can keep copies
separate; source identity alone does not establish original source, data-object,
module or runtime equivalence.
