# Three existing boot C bodies shared across the overlays

The existing `candidates/boot.c` definitions `FUN_002E5F60` (120 bytes),
`FUN_00350798` (108 bytes) and `FUN_00350808` (108 bytes) each have one explicit
placement in all 27 overlays. This lot changes the shared placement map;
it adds no authored C definition and does not clone or rewrite the source.
The qualified GNU G0 boot context is retained, along with the existing native
units and Barlow's separate G8 configuration.

The callback function reads the existing resident state view at `0x00188660`:
the count is at offset `0x1730`, the table pointer at `0x1734`, entries have
stride `0x90`, and the callback pointer is at entry offset four. It passes the
entry pointer in A0 and discards the result. The observed resident extent is
owned by boot zero-filled storage and is disjoint from all overlay allocations,
including BSS. This is a reference storage association, not a recovered original
allocation or class identity.

The two wrappers retain their existing architectural volatile MMIO accesses
and original resident helper bindings. The complete helpers at `0x0011F5E0`
(72 bytes) and `0x0011F628` (24 bytes) have pinned executable boot ownership
disjoint from every overlay. They consume no incoming GPR/FPR argument and
produce scalar results discarded by the wrappers. Their original COP0/status
implementations remain opaque and receive no additional C credit.

All 81 complete raw reference bodies equal their qualified boot counterparts.
The placement review checks complete boundaries, closed local control flow,
delay slots, entry callers and their complete owners, and disjointness from
current shared/native C and the other proposed extents. All 4,235 existing
shared placements, external bindings, exclusions and reference identities are
preserved. The proposed map contains 4,316 shared placements; the 1,202 native
definitions remain unchanged. No function alias, schema extension, profile
search or entry-GP assumption is introduced.

Acceptance requires fresh actual shared compilation/linking and all boot plus
27 overlay loaded-byte and metadata gates. The maintained shared linker keeps
only explicitly placed sections from the unchanged qualified boot object.
Raw identity alone adds no progress. The 81 placements represent 9,072 physical
bytes; unique-code changes must come from the fresh common catalogue and paired
reports, not by counting copies as new algorithms. Source sharing does not
establish original source/object identity or runtime behavior.

## Accepted full-image result

Fresh campaign action `4c333e68e6634eb4b197d98c10e2309b` passed all 28
loaded-byte and metadata gates. The published proofs contain all 81 new
`boot-shared` placements with `candidate_source=candidates/boot.c`, and retain
all 1,202 native controls. Boot remains 190 functions /11,696 bytes; overlays
now contain 5,518 placements /344,864 bytes. Physical C increases from 347,488
to 356,560 bytes out of the unchanged 48,788,176-byte loaded-code scope.
No native gameplay execution is established by these reconstruction gates.
The same fresh catalogue replay measures conservative primary C at 90,348 of
44,451,612 bytes, an increase of 336 unique bytes. Supplementary all-member C
increases from 44,856 to 45,192 template bytes; any-member C increases from
47,212 to 47,428. Its separate 38,345,900-byte footprint is unchanged. The
primary increase equals one representative of each 120/108/108-byte body;
any-member coverage differs because some families already had an exact member.
All 109,725 catalogue functions reconstruct under the maintained policy, and
the whole pointer-theorem replay is current. The existing pointer-argument
artifact is retained only after its measured tier/eligibility checks pass.