# Supplementary global code reuse

This report measures lossless machine templates with explicit placement-specific address bindings. It leaves the physical report and conservative graph-refined unique report unchanged. Its total is derived from the current complete EE catalogue; no estimated original source size is supplied.

## Measured baseline, October 6, 2026

| Partition | Representative EE bytes | Exact C bytes, all members |
| --- | ---: | ---: |
| Physical EE placements | 48,701,808 | 346,192 |
| Conservative target/data classes | 44,451,612 | 89,964 |
| Supported binding-parameterized templates, with unknown extents retained | 38,345,900 | 44,808 |

The complete physical game-code scope additionally contains 86,368 separately
accounted VU bytes. The supplementary partition has 64,621 families including
singletons, of which 5,120 have multiple members. It retains 8,440,328 bytes
with unsupported function boundaries and 231,732 bytes of residual gaps.
Its 11,989,528 representative bytes involving address bindings without original
data ownership remain explicitly uncertain in that respect.

The C numerator uses the same partition as its denominator. Broader grouping
can place already matched functions in a family containing unmatched copies,
so the all-member numerator can decrease without losing any physical C proof.
The separate any-member diagnostic is 47,164 bytes and is not completion credit.

The maintained authored-source subset associates all 5,573 integrated placements
with 449 canonical fragment/function slots across 31 generated sources; 198
slots have multiple placements. This is evidence for reuse of the current C,
not a denominator for unknown original source. Callback775 is one 76-byte
template and one shared authored slot with 27 placements, while its 27 distinct
helper templates keep it in 27 conservative classes.

A private replay independently checked all 109,725 raw function intervals and
exactly reconstructed the 94,662 supported templates. The public exporter
validates pinned receipts and freshness; it does not access game bytes.
These measurements do not establish the suggested approximately 5 MB workload.

## Why further address proof is needed

All 28 current normalizer contexts deliberately have unproved entry GP and
unproved GP preservation across calls. The diagnostics retain 134,618 recognized
GP-memory occurrences for that reason; zero GP16 records are normalized.
A startup assignment to `0x001AEFF0` proves one program point, not every function
entry or its lifetime. Original scratch and context-restore code also writes GP.
The fixed GP used by a qualified Barlow compilation is separate evidence.

A bounded 11,348-byte Aranos/Barlow pair contains 25 GP-base memory instructions
whose signed displacements differ and five whose displacements agree. This
shows a concrete source of retained differences; it does not authorize erasing
those fields. Small raw-identical GP routines already need no normalization.

A future scoped proof must establish the actual GP reaching each entry, identify
writes and call effects that invalidate it, and prove preservation where needed.
Only then can the maintained encoder reconstruct an eligible GP16 address field.
Original data object or field identity remains a further question; proved GP
alone cannot establish that two different addressed objects are interchangeable
or that the original source workload is approximately 5 MB.

Supported, reconstruction-certified, equal-sized intervals may share a family only when their complete template hash, maintained role-bearing signature and external `(role, target)` equality pattern agree. Internal relative targets remain literal structure. Scalar constants, register operands, opcodes, field and stack offsets, float bits and trailing instructions cannot be erased by this exporter. Unsupported boundaries remain individual raw units; uncatalogued gaps are added without collapsing. VU remains excluded from EE scope.

Different static callee classes or absolute data bindings may split the conservative primary while sharing this supplementary placement-bound template. Every multi-member family retains raw pins and explicit bindings, and reports its conservative class count. Original semantic/source equivalence, original data allocations and original source boundaries are not proven. Non-J26 address families conservatively expose unproved original data ownership.

`template_all_c_bytes` uses one representative size only when every complete placement has a current exact integrated C proof for its raw hash. `template_any_c_bytes` is a separate partial-coverage diagnostic. The percentage uses this same supplementary denominator; neither the old physical nor conservative numerator is reused. This does not add C credit or authorize compilation trials.

## Authored source subset

The report separately associates current integrated placements with the 31 known generated C sources, including boot-shared placements. It verifies canonical recipe fragment hashes and lengths, exact generated source bytes, catalogued definition slots and current exact placement proof pins. Grouping uses canonical fragment SHA256 and its definition slot, with non-identifier recipe literals retained as variants. No arbitrary lexical identifier normalization is introduced. Each member retains source/context SHA256; groups report min/max machine sizes and context variants. Unequal sizes never enter a single global machine-template group.

This subset proves reuse of maintained authored fragments under recorded contexts. It does not establish original module/object/source identity or an unknown-source global denominator. Multi-placement source fragments are reported even when conservative graph classes remain distinct. The current update775 fragment associates 27 independently proved 76-byte placements.

## Generation and checking

From a current pinned repository and catalogue:

```sh
python scripts/code_reuse_report.py --repo . \
  --catalog config/function-catalog/catalog.json \
  --output progress/code-reuse-report.json \
  --families-output progress/code-reuse-families.json.gz
python scripts/code_reuse_report.py --repo . \
  --catalog config/function-catalog/catalog.json \
  --output progress/code-reuse-report.json \
  --families-output progress/code-reuse-families.json.gz --check
```

The deterministic summary contains the conservative primary reference metrics and quality, supplementary totals, unsupported/gap/data-ownership counts, authored-source subset and input snapshot. Multi-member details are kept in a separate deterministic gzip payload. The summary pins the exact details payload. A changed source, recipe, proof, normalizer, catalogue chunk or generator produces a stale result. Maintained current-credit validators check catalogue proof pins before generation.

Asset-free CI validates metadata certificates and current exact C proofs; it cannot reconstruct unavailable reference bytes. A separate optional private replay verifies all complete raw intervals and reconstructs every supported body using the maintained field encoder. It checks supplied role signatures but does **not** rerun address-role classification or incoming-pointer theorems. Those remain pinned provenance from the maintained catalogue. A malicious new mask plus freshly forged metadata can roundtrip; roundtrip alone is not address-role proof. Fresh classification requires the existing private catalogue builder and pointer-proof workflow, without normalizer changes:

```sh
python scripts/code_reuse_report.py --repo . \
  --catalog config/function-catalog/catalog.json \
  --output PRIVATE_REPLAY_SUMMARY.json \
  --families-output PRIVATE_REPLAY_FAMILIES.json.gz \
  --references PRIVATE_REFERENCE_ROOT
```

The private root holds `boot.elf` and `levels/<id>/overlay.elf`. Every reference identity, template, role signature and reconstructed full raw hash must agree. The private output marks actual raw replay distinctly. Do not publish reference bytes, objects, executable files, private paths or runtime tool bindings. Check a private replay output with the same `--references` option; ordinary asset-free output intentionally has a different proof scope.

The implementation's asset-free tests cover partial versus all-placement credit, residual gaps, unsupported extents, alias equality, malformed/stale certificates and input pins, raw credit contradictions, literal/register/opcode retention through the maintained normalizer, private replay signature refusal, source recipe freshness, unequal source-slot sizes, 27-member/27-primary-class reuse and stale summary/details detection.
