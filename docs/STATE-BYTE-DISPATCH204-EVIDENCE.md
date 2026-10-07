# Scalar entity state dispatch

One ordinary C body now reproduces two complete 204-byte entity callbacks. It dispatches the unsigned state byte at `+0x20` through cases 0/1/2, calls the original scalar state setter and status query, updates halfword flags at `+0x34`, and clears or copies the raw word at `+0x98` from the pointer at `+0x24`, dereferenced at `+0x10`. The source uses one canonical fragment with explicit per-program function/helper bindings; no source or compiler variant was tried.

| Program | Function | Complete bytes | Whole-unit result | Actual first trial |
| --- | --- | ---: | --- | --- |
| Endako | `0x003CA9F8` |204|72/72 exact, including 71 prior controls|`9788ed759f164d6ca851a0312571833d`|
| Aranos Prison | `0x003CC2E0` |204|74/74 exact, including 73 prior controls|`2df010db006441188383cf622eb29018`|

Both current qualified G0 units pass complete object-section/STT_FUNC sizes, unmasked linked/reference bytes, read-only checks and source/catalog/tool provenance. Fresh published native reviews reproduce the actual trial objects. The canonical source fragment SHA256 is `bb818ef520c2df543822d12aecc3cdf8da200a63a2e57f966bad053cc6eb7591`.

The incoming entity pointer is supported by each program's original class 2426 callback table, complete lookup/constructor chain and dispatcher: the callback stored at entity `+0x64` receives that entity in A0, and V0 is overwritten before use. The 60-byte state setter equals the existing qualified C implementation and preserves its `void (u8 *, u8, s32)` interface. The 12-byte status helper returns the observed zero or 4 from byte `+0xBE`; its word-result declaration is a compatible consumed interface, not a recovered original return-type declaration. Both original helpers receive zero additional C credit.

Widths and offsets are measured partial views. Original class, enum and field names, allocation boundaries, callback-data validity and complete original declarations remain unproven. The prototype correspondence name is only a hint. No vector helper, GP access or floating-point operation appears in the authored callback or its two scalar callees.

Public catalogues retain repository-relative candidate sources and omit loader-only cache fields. Correcting these publication fields preserves the fixed C and actual compiler objects; immutable trial catalogues remain unchanged. The two fresh reviews validate the corrected public catalogue context.

Fresh campaign action `57a62e2d99d249a592c31aa7c6b3d462` passed boot and all 27 complete overlay loaded-byte and metadata gates without input drift. The accepted final batch combines these two placements/408 bytes with 53 known-C placements/3,804 bytes: 55 placements/4,212 bytes. Physical C increases from 364,432 to 368,644 bytes out of the unchanged 48,788,176-byte loaded-code scope. No hardware/runtime behavior or original source-object boundary claim follows from these byte proofs.

The canonical source is [state-byte-dispatch204.cfrag](../src/levels/shared/state-byte-dispatch204.cfrag). Boot remains 234 integrated functions /12,048 bytes. Overlays contain 6,287 placements /356,596 bytes, including 1,891 native definitions.
