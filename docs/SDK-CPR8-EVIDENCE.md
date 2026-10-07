# Source-specific SDK qualification of CPR8

The attributed standalone `FUN_0012D808` source reproduces the complete
656-byte boot body at `0x0012D808`. Its four calls retain the original absolute
helper targets. This unit has its own source/object proof; full boot and overlay
integration remains a separate gate before publishing matching credit.

## Source and licence

The body comes from the pinned MIT-licensed
[Lombyte CPR8 source](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/src/sdk/library/cpr8.c).
The complete copyright and permission notice accompanies
`src/sdk/cpr8_source_unit.c`. The standalone source SHA-256 is
`526e6888e54dea5025a27a013101d88024c434cc99ce0121c06d67d84c19a66e`.

Only the required existing scalar declarations and the unchanged licensed
fragment are used. The body retains its expressions, goto and loop ordering,
field widths and hardware operations. Structure and function identifiers are
mechanically namespaced. This adaptation does not establish original game type
names or original object boundaries. The bare compiler filename remains
`cpr8_source_unit.c` so the complete object identity is reproducible.

## Complete compiled and linked ownership

One actual owned SDK driver compilation produced a complete global STT function
at offset zero in a 656-byte `.text` section aligned to eight bytes. There are
no other functions, allocated data, readonly payloads or GP use. The fixed SDK
driver invokes integrated preprocessing in `cc1`, then the SDK assembler; the
source, flags and tools were not varied.

The complete unstripped object hash is
`7a26e9567b01cead56a2a79b8f9f51f828cbb47bcd961f5d19a4f0736d2794c1`.
The stripped object hash is
`d92c545c53c0f2c0550fca7c42fa2db1dd37b6d9374e0ab435702f59622b467b`.
Stripping preserves the entire text, symbol extent and all four declared
`R_MIPS_26` rows. The unlinked text hash is
`e021478ab809bda77475adcd76f355c9ffcb5f60d13b9980951e7eb0638e8726`.

| Text offset | Opaque helper | Original complete helper extent |
| --- | --- | ---: |
| `0xF8` | `FUN_0011F5E0` | 72 bytes |
| `0x13C` | `FUN_0011F628` | 24 bytes |
| `0x174` | `FUN_0011F5E0` | 72 bytes |
| `0x1B8` | `FUN_0011F628` | 24 bytes |

The input helper symbols are exactly two global undefined `STT_NOTYPE` symbols
with zero value and size; each relocation owns a zero-addend JAL at its declared
offset. The SN linker resolves them to their original addresses using global,
zero-size `STT_OBJECT` symbols with `SHN_ABS` ownership. This is the observed
linker convention, also present in previously accepted qualification ELFs.
No helper body, stub, veneer or extra allocated section is emitted. The helpers
remain opaque reference dependencies and receive zero C credit.

The resulting ELF hash is
`b15896a43d2fa4a682d597e0ce9ae40fe16f289217a979793c6555b6ab8350b5`.
All 656 linked bytes equal the reference, without relocation masks:
`3e43b8eb74cf2d19ba065bc68d58b8bda52bfa053bb06588bbe9dc0f38c0fad8`.
The adjacent accepted 40-byte body remains outside this complete extent.

## Preserved refusals and scope

The original default-compiler trial `e4ed66e94a8f4cdebe8782484aa66260`
emitted a 736-byte function and failed its link. Its source, object, refusal and
zero measured-function result remain intact.

Two later instrumentation refusals are retained separately. After the single
successful SDK compilation, a Python marshal-based method fingerprint changed
when returned literal strings remained referenced. Stable code semantics and
code-object identity corrected that false guard. The same compiled object was
then copied, stripped and linked once; a second guard incorrectly expected
linked absolute helper symbols to remain `STT_NOTYPE`. The narrower observed SN
`STT_OBJECT` rule corrected that metadata assumption. Final readonly validation
used the existing artifacts; neither repair repeated the compiler, strip or link.

Independent audits verify source and tool closure, full object and linked STT
extents, actual relocation rows, complete raw equality and preservation of all
original artifacts. Qualification admits only this exact source unit and its
declared bindings. It does not promote arbitrary SDK call-bearing sources or
claim the original retail compiler. Hardware execution remains unverified.

## Integration ownership

The boot uses three independently compiled owners: the unchanged 187-function
GNU object, the unchanged 152-byte sysbit SDK object and this 656-byte CPR8
object. Each SDK source is admitted only through its own exact catalogue and
review. The final complete boot gate compares 2,521,763 loaded bytes with 189 C
functions and 11,360 matching C bytes. Existing overlay placements continue to
reuse the default GNU source; neither opaque helper nor any new SDK overlay
placement receives credit from this unit proof. Full overlay and all-members
unique proofs govern published progress.
