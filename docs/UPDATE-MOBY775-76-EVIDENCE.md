# Prototype-labelled callback 775: exact 76-byte native family

The fixed C body matched all 27 native placements with the existing GNU `8bed6eae…` compiler chain. The actual seed trial checked 43 complete functions; the other 26 trials checked 1,105. All **1,148 complete functions** matched, preserving **1,121 existing controls** and adding 27 complete 76-byte bodies: **2,052 physical bytes**. These are source-unit results; full-image gates and the refreshed unique-code report remain separate requirements. No new unique-byte total is claimed here.

## Provenance and newness

The locally measured August 6 prototype-to-retail table supplied the research label `UpdateMoby_775`. Its pinned TSV is `dc2931fd6771c1a3fa2365d4c98c5b4dbd53ac4e08a9cbdb236f7b051bd15571`; ten entries carry this name. The name does not prove the original C declaration, field meanings or module ownership.

The current 28 function chunks and 109,725 complete reference rows were checked. The family has 27 native placements whose entire 76-byte bodies differ only in the actual JAL callee-address field at offset `0x38`. The float literal, register/opcode bits, branch/delay slots, field offsets and arithmetic constants are preserved. Selection checked all 5,545 existing owned spans, maintained tasks, resolved private target catalogues and legacy address history. There were no ownership or parked-family hits and no target-catalogue resolution errors. This address-field comparison was used for exclusion and discovery, not matching credit.

## Observed algorithm and ABI

The body reads a binary32 word at receiver offset `0x2C`, multiplies it by `1.025f` (exact binary32 encoding `0x3F833333`), and writes it back. It subtracts three from the unsigned byte at `0x23`, retaining modulo-256 storage. If the updated byte is below four, it calls the original helper with the same pointer in A0. The original float store occupies the conditional branch delay slot; complete byte matching verifies the emitted ordering.

The callback association is supported by pinned original code and data: the retail class-table record contains identifier 775 and the callback address; a complete 148-byte lookup scans 12-byte records and caches the function pointer. A complete 624-byte initializer assigns that pointer to receiver offset `0x64`. The complete 260-byte invoker loads it, passes the receiver in A0 and calls indirectly; it immediately replaces V0 afterward and consumes no floating return. A second complete 212-byte invoker provides the same pointer/ignored-result observation. These support the ordinary one-pointer ABI and a **void observational model**, not recovery of an original signature. Writable callback state and runtime execution are not proved immutable by these static pins.

The C uses neutral `field23` and `field2C` names and a partial 48-byte view. LBU/SB/ANDI and LWC1/MUL.S/SWC1 establish the accesses used by this function. They do not establish the original object allocation or all fields of a generic Moby. Generic cross-game headers assign other meanings to `0x2C`; those meanings were not imported. The names “alpha” and “scale” are not asserted for this view.

## Helpers, profiles and preservation

Each original helper has a pinned complete 88-byte body. The new C emits no helper implementation and claims **zero helper C credit**. The helper's original transitive GP/MMI/VU code remains opaque and unchanged; this family does not qualify or reconstruct that code.

The Aranos seed and 25 other units retain `-O2 -G0 -ffunction-sections`. Barlow retains its existing `-O2 -G8 -ffunction-sections` and `_gp = 0x001AEFF0`; it passed its own complete 42-function trial. Its baseline already declared the original helper with a `u8 *` parameter. A separate namespace alias is bound to the same `0x003317E8` address so the old declaration remains unchanged. Only the helper placeholder changes; no cast, body, expression, operation order or compiler flag variant was introduced.

The first accepted seed is actual trial `e099126b23494e069ce96fe1184b851d`: source `785d95ed0b0d9f57bb98997d90d06496fb4f10dd8270f5b1706d90fc488d08ff`, catalogue `3cac8ebebc3fd8d56c08e4ca2ab524f8a07a80c49b5cbb8d4fcf6d316d208102`, object `af3d46b9a31b158ecb6a7594c85145a826d2417a2c4670f308b07e235d872dbd`. That object was reused rather than recompiled. All remaining actual trial IDs and artifact pins are retained in the family receipts. Independent read-only replay checked every object function's complete ET_REL extent/cardinality, every linked STT_FUNC and full raw reference body, and readonly data.

The original authored fragment is frozen at `5b4da2c8c283bf39e12f0d22fe93e4c790791a70dd93cbd67db55fc5b88c9f54`. The canonical shared fragment uses function/helper substitutions only; declaration and call tokens map to the same helper name. All 27 generated whole sources reproduce their qualified byte streams, including their unchanged current-control prefixes. Guarded installation and `source_layout.verify`, catalogue/review validation and source-inventory checks passed. Fresh full-image gates remain required before coverage publication.

No original compiler, original module/object boundary, complete original type, hardware behavior or runtime callback-lifetime theorem is inferred from this evidence.

## Completed integration

Campaign `790b5eb477624821be9ed3fcdc3c615d` passes the boot and all 27 overlay
loaded-byte and metadata gates. The independent audit verifies all 5,573
complete owned functions, readonly sections and object provenance; prior
controls and opaque helpers remain unchanged. The coherent lot adds 2,388
physical bytes across its two source families. Current physical coverage is
346,192 / 48,788,176 bytes; conservative all-members unique coverage is
89,964 / 44,451,612, measured after complete reconstruction and theorem replay.
