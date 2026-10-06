# Bounded caller GP investigation

The next step after the [local GP pilot](LOCAL-GP-PROOF.md) was to select a
complete caller that establishes GP and transmits it to a small mapped-data
leaf. The inspected boot startup path does **not** establish that theorem.
This retained negative result prevents treating the startup constant as a
universal function-entry fact.

## Pinned startup path

Target: Going Commando USA v1.01, `SCUS_972.68`, boot reference SHA256
`36d5814d8d95328d5839612ccdf7a2e7ecac0b6f868d3ad4bb2e98411f734b4a`.
The independently selected complete startup extent is `0x00131AE8 / 448`
bytes, raw SHA256
`25e77efa95336c9be8672a8b5372c77c3338c594533f18029d9c35fe73fde58e`.
These are supplied extraction extents, not recovered original source boundaries.

| Location | Observed operation | Proof consequence |
| --- | --- | --- |
| `0x00131C28`, `0x00131C3C`, `0x00131C50` | Explicit high/low construction and copy into GP | Establishes `0x001AEFF0` at that local point |
| `0x00131C58` | System call with selector `0x3C` | First opaque kernel effect; GP preservation is unproved |
| `0x00131C74` | System call with selector `0x3D` | A second unproved kernel boundary |
| `0x00131C78` | Direct call to `0x0011F828` | Target is verified, but its transmitted GP is not |
| `0x00131C7C` | Owned NOP delay slot | Does not repair the earlier missing preservation proof |

The maintained local verifier rejects the complete caller with
`Unsupported COP0/exception control in local CFG`. Removing the system calls,
starting a proof immediately after them with an asserted GP, or importing an
ABI promise would bypass the missing evidence.

The first target is a complete `0x0011F828 / 60`-byte supplied extent, raw
SHA256 `cb9dda62a1e99c1c5110d92461840b56a025a11fb61bc3f4380d3d85f55898b9`.
Its complete decoded CFG contains five direct calls and a tail transfer, with
no GP-base memory instruction. It is not the requested closed mapped-data
leaf, even if kernel preservation were independently established.

## Existing GP-writer inventory

A bounded recheck of the existing 58-body inventory covers 149,036 bytes and
731 decoded GP writes. It found no alternative explicit-known-GP direct caller
to a mapped-data leaf within that inventory:

| Existing bodies | Reason they do not supply the requested witness |
| --- | --- |
| 1 startup | Literal GP construction followed by the opaque system calls above |
| 28 scratch bodies | Locally established MMIO GP; no calls |
| 28 global-restore bodies | GP loaded from mutable storage after their two direct calls |
| 1 kernel/context body | GP loaded from mutable context, with indirect control |

This is a negative for the pinned inventory, not an exhaustive claim about
every possible caller or runtime path in the game. A mutable restore requires
value, alias and lifetime evidence before it can supply a constant entry fact.

## Requirements for a usable contextual proof

An independently pinned caller and callee must bind the same reference,
complete raw extents, mappings, decoder and verifier. Start the caller with
unknown GP, retain all CFG incoming paths and evaluate the owned call delay
slot before observing the transmitted register. Opaque preceding calls or
system effects cannot preserve facts by convention.

A proof for one selected call is conditional on entering that caller at the
supplied start. It is not a certificate for other callers, alternate entries,
callbacks, interrupts or a whole program. A closed-leaf proof must also account
for the JAL return link: canonical `JR RA` alone does not prove a return when
the call slot or callee can rewrite RA. Tail transfers, indirect control,
fallthrough exits and unproved saved-link restores require separate evidence.
Caller-side indirect reentry into the selected call must also be modeled or
refused: a rewritten return link can repeat that call with a different GP.

The private conditional-verifier experiment and its synthetic adversarial
tests are retained as research. They are not published as a qualified retail
consumer without an actual accepted packet. The initial return-link
counterexamples and corrected refusals remain retained with the experiment;
a further caller-reentry counterexample still blocks qualification of that
private prototype. Passing synthetic cases does not establish soundness for
unmodeled indirect control flow.

### Existing local-verifier qualification gap

The review also reproduced this issue in the published local verifier whose
SHA256 starts `7d604c9a`, not only in the private caller prototype. A synthetic
body establishes one GP, accesses mapped data, changes GP and rewrites RA to
the earlier access, then executes canonical `JR RA`. The producer and consumer
accept the static-CFG proof and emit an eligible field for the first target,
although the next dynamic iteration uses a different target. Exact byte
reconstruction does not repair that lifetime error.

Consequently, all local-verifier GP facts and field eligibility are conditional
on the static CFG's indirect-exit assumption. They are **not qualified dynamic
lifetime evidence** and must not authorize a production GP16 consumer. Actual
indirect-target proofs or conservative refusal are required before that use.
The prior 28-body retail pilot had zero eligible GP16 fields and zero C credit;
this finding adds neither. Both published and private counterexamples are
retained. That evidence-only investigation did not repair the verifier.

The subsequent [closed-CFG repair](LOCAL-GP-PROOF.md#current-control-admission)
refuses those escapes in both producer and consumer before emitting facts. Old
receipts remain historical evidence. This repair does not establish the missing
startup/kernel transmission theorem.

## Reopening conditions and unchanged progress

Provide either an independent machine-level preservation contract for the
two actual kernel boundaries and subsequent callees, or another complete
caller with an explicit GP definition and no opaque effect before a suitable
leaf. Kernel selector names, linker hints, compiler profiles and emulator
observations alone do not constitute that contract.

This investigation adds no accepted entry-GP theorem, GP16 field, C match or
data-object equivalence. The existing normalizer, source, compiler profiles,
catalogue and progress measurements remain unchanged. Legal reference bytes
and detailed disassembly stay private; the public record contains identifiers,
hashes, the measured refusal and explicit reopening conditions.
