# Global unique EE code measurement

The loaded-code report counts every placement in boot and the 27 level overlays.
The unique report uses one grouping policy for both its total and its exact-C
numerator. It never divides replicated C coverage by an estimated source size.
Neither report changes the byte-exact acceptance rules for authored C.

## Partition and evidence

The catalogue covers all pinned executable EE sections. VU code is reported
separately and excluded from the EE partition. Each interval retains its program,
address, extent and raw SHA-256. Qualified C extents have current complete-symbol
proofs. Other extents are classified by structural entry, control-flow and exit
evidence; these classifications do not prove original source function boundaries.
Unsupported intervals and section gaps stay separate, uncollapsed units in the
conservative global total. The report also exposes their byte count and a
provisional partition, rather than silently treating inferred extents as verified.

Supported extents with exact reconstruction receipts start from equal-sized
relocation templates. Every masked field has a recorded address role.
Internal targets retain their relative offsets; external calls retain
alias patterns. Scalar constants, stack and structure offsets, unsupported
address uses and unproved GP values remain literal. A template hash identifies
the normalized bytes; the grouping signature additionally includes the field
role schema. These hashes are deliberately distinct.

The primary partition also retains every unowned data operand target and its
field positions, HI/LO mode and GP base. A mapped pointer consumer proves that
an operand is an address; it does not prove an original object base. For example,
`object + 12` and `object + 16` can be folded into two complete addresses with
identical normalized templates. Their raw data targets must keep them distinct.
The current catalogue has no verified original data allocations and invents no
ownership exemptions. Address-normalized data templates remain provisional.

A relocation template alone is insufficient to identify copies: otherwise a
caller of a function returning one could be grouped with a caller of a function
returning two. The conservative partition additionally refines classes through
the external static control-flow target classes until no class splits. Unknown
or unsupported targets retain distinct program/address identities. The original
template-only partition remains a separate provisional diagnostic. This policy
still describes machine-code structure under explicit address bindings, not
recovered original source or full semantic equivalence.

Cross-programme boot edges additionally require a pinned, independently replayed
combined-reference binding receipt. Named sections alone are insufficient.
This is a static code-inventory model with runtime preservation explicitly
unproved; see [the boot and remaining-duplication verification](BOOT-SHARED-CODE-VERIFICATION.md).

`verify_boot_bindings.py` writes that receipt as plain JSON, and
`build_unique_catalog.py` refuses to overwrite an existing destination, so the
receipt reaches the catalogue through three steps that no single script owns:

1. `python scripts/verify_boot_bindings.py --repo . --references <private refs>
   --catalog <fresh catalogue> --output <fresh path>.json`
2. gzip that JSON to `config/function-evidence/boot-bindings.json.gz`; its SHA-256
   and the `source_catalog_sha256` recorded inside it become the catalogue's
   `boot_binding_proof` descriptor, with `scope` = `combined-pinned-reference-images`
   and `runtime_preservation_proven` = false.
3. add the four pins the descriptor makes required to `input_pins`: the receipt,
   `config/function-evidence/pointer-arguments.json.gz`,
   `scripts/verify_boot_bindings.py` and `scripts/validate_boot_binding.py`.

Skipping step 2 or 3 does not fail loudly at build time: the refined grouping
loses every boot edge and the conservative unique numerator inflates by tens of
thousands of bytes. The guard is `combined_reference_boot_edges` in
`progress/unique-code-report.json`, which must equal the receipt's binding count
and never zero.

The generator reads private pinned references and reconstructs every complete
body byte for byte. Its public compressed chunks contain structural identifiers,
address-role metadata and hash receipts, never reference instructions or assets.
Asset-free CI checks these receipts, coverage, input freshness and paired C
proofs. It cannot independently reconstruct unavailable retail bytes.

Incoming-pointer evidence covers a conservative selected bank of callees. Its
loader replays complete incoming-value use and dereference sets against pinned
bodies before the caller normalizer can use a role. Scalars, returned or stored
pointer values, unproved calls and unsupported control flow reject that role.
This does not establish original data allocations or register preservation.

For a group to enter the primary matched-C numerator, every placement must have
a current exact C proof for the same complete extent and raw hash. Groups with
only some exact placements have a separate diagnostic measure. Masked similarity
never establishes an additional C match.

## Consumers and reproduction

`config/function-catalog/catalog.json` pins compressed per-program chunks and
the source/proof inputs. `scripts/unique_code_report.py` validates them and emits
`progress/unique-code-report.json`. `scripts/readme_unique_progress.py` combines
that summary with the physical report to generate the paired README block,
SVG and `progress/paired-code-metrics.json`. CI rejects stale outputs.

Rebuild reference-derived metadata only in a private workspace with legally
acquired pinned references and a fresh complete-image build. Supply private paths
to the boundary extractor, then generate a fresh pointer scan and catalogue:

```sh
python scripts/global_function_catalog.py --repo . --references PRIVATE_REFERENCES \
  --build PRIVATE_FRESH_BUILD --output PRIVATE_NEW_BOUNDARIES.json
python scripts/scan_pointer_roles.py --boundaries PRIVATE_NEW_BOUNDARIES.json \
  --references PRIVATE_REFERENCES --decoder-source scripts/relocation_identity.py \
  --output PRIVATE_NEW_POINTER_REPORT.json
python scripts/build_unique_catalog.py --repo . \
  --boundaries PRIVATE_NEW_BOUNDARIES.json --references PRIVATE_REFERENCES \
  --pointer-evidence PRIVATE_NEW_POINTER_REPORT.json \
  --output PRIVATE_NEW_CATALOGUE --diagnostics PRIVATE_NEW_DIAGNOSTICS.json
```

The output paths must be fresh. Retain unsuccessful or partial runs privately.
Review metadata and source pins before copying chunks into the public catalogue.

```sh
python scripts/unique_code_report.py --catalog config/function-catalog/catalog.json \
  --output progress/unique-code-report.json --check
python scripts/readme_unique_progress.py \
  --catalogue-report progress/unique-code-report.json \
  --physical-report build/decomp/report.json --check
```

The CI publishes separate physical and unique objdiff artifacts. Their histories
must remain separate: `SCUS_972.68_report` and `SCUS_972.68-unique_report` have
different denominators. Other consumers can use the committed paired summary;
this change does not modify or deploy the OpenRAC website.

There is no certified approximately 5 MB original-source total. Additional
boundary, data-object and register-preservation proofs can reduce remaining
duplication; guesses about address-looking constants cannot do so.
