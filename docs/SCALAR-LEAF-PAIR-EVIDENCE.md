# Shared scalar bit setter and signed comparison

Two authored C fragments provide a 32-byte bit setter and a 16-byte signed
halfword comparison. Each has 27 explicit native placements. The complete
current units contain 1,202 functions: 1,148 existing controls and 54 new
definitions. Fresh boot and all 27 overlay loaded-byte and metadata gates
accept 1,296 additional C bytes. Physical coverage increases from 346,192 to
347,488 bytes. Conservative primary C increases from 89,964 to 90,012 unique
bytes, with the same 44,451,612-byte denominator. Supplementary all-member C
increases from 44,808 to 44,856 template bytes, with its separate unchanged
38,345,900-byte footprint. These measurements use the current integration
receipts and complete catalogue replay, not a discovery signature.

The canonical sources are
[`entity-byte-bit2-set32.cfrag`](../src/levels/shared/entity-byte-bit2-set32.cfrag)
and [`entity-signed-tag-equals16.cfrag`](../src/levels/shared/entity-signed-tag-equals16.cfrag).
Only the function symbol changes between placements. The maintained source
writer regenerates each standalone unit without changing its previous source
prefix. Catalogues under `config/level-native/` record both complete extents
and all prior functions; reviews under `progress/level-candidates/` cover every
function in each new unit. Barlow retains its qualified G8/GP configuration;
the other units retain G0. No compiler, flag or source-order search is used.

## Measured interfaces and limitations

The bit setter consumes an ordinary pointer in A0 and a zero/nonzero scalar in
A1. It sets or clears bit 2 of the byte at pointer offset `0xBE`. Three inspected
direct callers pass scalar one and discard the result. The C `void` return is
a minimal discarded-result hypothesis, not a recovered original declaration.
The bit's game meaning, full object type and original ownership remain unknown.

The comparison consumes A0, reads the signed halfword at offset `0xAA`, and
returns integer zero or one according to equality with `0x0CDB`. Its inspected
direct caller forwards an object pointer and branches on V0. The source uses
only the measured cell width, alignment, offset and comparison constant; it
does not reconstruct an original aggregate or class declaration.

The raw bodies are identical within each family. Neither body requires GP
normalization, a data-object equivalence assumption or a guessed helper ABI.
Separate original 44-function seed trials were exact. They were not reused as
proofs of a combined translation unit: all 27 combined units were compiled and
checked anew. The independent native review path regenerated the 27 reviews
and reproduced the registered whole-object hashes. Complete STT_FUNC sizes,
section bodies, reference hashes, read-only content and all old controls must
agree. Final boot/overlay loaded-byte and metadata gates remain mandatory.

## Retained refusals from the same selection campaign

The following fixed hypotheses added no matching credit. Sources, compiler
outputs, assembly, link products and immutable outcomes remain in the private
runtime; the canonical campaign register retains their decisions and reopening
conditions.

| Candidate | Complete measured result |
| --- | --- |
| Camera callback, 112 bytes | 104 bytes emitted; 43 controls exact. Two address-construction instructions were eliminated, and identical floating stores were scheduled differently. |
| Progress collector, 456 bytes | 452 bytes emitted; 43 controls exact. Frame/register and address-lifetime lowering differ. |
| Three halfword stores, 28 bytes | Complete size agrees, but three bytes differ; 43 controls exact. A near match is not accepted. |
| Five flag/halfword assignments, 148 bytes | 120 bytes emitted; 44 controls exact. Repeated address materializations were shared by the compiler. |
| Scalar float wrapper, 72 bytes | Complete size agrees, but 14 bytes differ; 46 current controls exact. Its original opaque vector helpers receive no C credit. |

Preparation-only path/selector refusals are kept separately from compiler
measurements. Their outcomes cannot establish a compiler failure or a C match.
Equivalent-expression, register, statement-order or flag permutations do not
reopen the rejected hypotheses. Future attempts require recorded independent
ABI, storage, boundary or qualified lowering evidence.

Source sharing is an authored organization and exact compilation result, not
proof of original modules, original source identity or runtime behavior. Game
bytes and proprietary tools are not published. Physical loaded-code coverage
and the conservative unique-code metrics remain separate measurements.
