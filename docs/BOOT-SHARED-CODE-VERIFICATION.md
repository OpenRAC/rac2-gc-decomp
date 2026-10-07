# Boot shared-code verification and remaining duplication

The initial structural partition retained 44,879,164 EE bytes. Its unresolved
cross-programme dependencies included 22,522 targets at known `core.text` boot
entries. The catalogue had omitted section names, disabling the previous
name-based fallback. Names are now preserved and checked against the pinned
physical scope. A name alone no longer authorizes cross-programme resolution.

## Combined reference code space

The boot core occupies `[0x00115200, 0x00133B50)`, 125,264 bytes. The producer
independently checks all 28 pinned ELF images, complete caller and target body
hashes, executable section and file-backed segment ownership, and direct
control targets decoded from each caller's original words. Every overlay
allocated section and PT_LOAD memory span, including zero-fill, is disjoint
from the complete core interval. All 22,522 candidate edges bind to 103 current
supported boot bodies; none is rejected by those static checks.

`verify_boot_bindings.py` writes structural metadata only. Its source, ELF
reader, decoder dependency, reference identities and complete function-chunk
descriptors are pinned. The original input catalogue hash is a source-context
pin; the chunk digest avoids a circular hash when a manifest attaches its proof.
`validate_boot_binding.py` verifies freshness and current membership before
creating the immutable per-edge map used by `call_graph_refinement.py`.
Explicit `target_program` labels cannot bypass the proof. Uncovered edges stay
unresolved under the caller's programme identity.

This proves ownership in **combined pinned reference images**. It does not prove
that boot code stays unchanged at runtime: its PT_LOAD is writable. A bounded
write audit found no direct constant-destination core writes, but generic fill,
disc/IOP transfers, aliasing, DMA and dynamic patching remain outside that claim.
Two complete boot routines were inspected and pinned without identifying a
complete overlay loader or proving a universal nonwrite theorem. The report
therefore retains `runtime_preservation_proven: false`, and makes no original
source or semantic-equivalence certification.

```sh
python scripts/verify_boot_bindings.py --repo . --references PRIVATE_REFERENCES \
  --catalog config/function-catalog/catalog.json --output PRIVATE_NEW_PROOF.json
```

References remain private. The output must be fresh. Attach only reviewed
metadata and its current source/chunk pins, then regenerate both unique totals
and unique C credit with the same grouping policy. Physical matching is unchanged.

## Measured effect

The conservative partition decreases from **44,879,164 to 44,451,612 bytes**:
427,552 bytes of additional grouping, and 84,841 to 83,093 structural classes.
The 6,666,844 loaded bytes touched by prospective boot edges were not an estimate
of recoverable duplication. After binding, 3,293 external targets remain unresolved.
The template-only provisional total remains 37,873,548 bytes; neither figure
establishes approximately 5 MB of original source code.

Primary unique exact-C credit changes from 106,348 to 67,844 bytes because newly
merged groups require every member to have current C integration. Previously
separate fully qualified groups can join an unqualified copy. Existing C proofs
are retained and physical matching stays **311,644 / 48,788,176 bytes**. The
separate any-qualified-member diagnostic is 69,032 bytes and does not replace
the primary numerator. No new C or matching credit is claimed.

## Largest uncertain boundaries

The 20 largest weaker spans cover 672,792 placement bytes. Under their pinned
table snapshots they are large guarded switch bodies, rather than demonstrated
containers that should be split. Selector guards bound the first 18 to 147
table slots and the last two to 141. Definition and passing-edge dominance,
unclobbered predicates/indexes, exact table ownership, aligned internal case
targets and complete delay slots were checked. Finite CFG expansion reaches
every nonzero word and closes all selected paths.

All tables lie in writable `.data`. Original allocations, original source
boundaries and runtime table preservation remain unproved. These results are
retained as conditional boundary proposals: this lot changes no extent or
boundary tier. The existing normalizer has exact **raw identity** reconstruction
receipts for these bodies, but does not follow their indirect table flow or
qualify address-normalized copy identity from that CFG evidence alone.
`guarded_switch_bounds.py` and its negative guard/clobber/bypass controls retain
the narrow method. Full structural receipts are in
`progress/verification/guarded-switch-audit.json.gz`.

## Data, GP and compiler differences

The [bounded data/GP audit](DUPLICATION-DATA-GP-AUDIT.md) ranks 20 discovery
families: 681 placements and 3,786,240 loaded bytes. Every selected entire body
reconstructs exactly; discovery masks are excluded from accepted identity.
All original reference ELFs lack symbol tables and sized original objects.
No new original allocation, entry GP value or preservation theorem was found.

Observed register selectors are identical within these selected families.
Register-allocation differences are therefore not the demonstrated obstacle
in this sample; unselected functions and historical near matches remain separate.
No compiler trial is reopened merely by renaming variables or cycling equivalent
expressions. Data-base/addend ownership, scoped GP and qualified callee bindings
are the concrete next proof obligations. Constants and unowned data operands
remain literal in the primary partition; no arbitrary approximately 5 MB total
is selected.
