# Temporary-track update and IPU synchronization

Two complete boot bodies use mechanically adapted, attributed Lombyte C:

| RAC2 symbol | Complete extent | Pinned source |
| --- | ---: | --- |
| `FUN_0012CFE8` | 116 bytes | [UpdateTempTrackData](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/src/sdk/library/update_temp_track_data.c) |
| `FUN_00130DB8` | 104 bytes | [sceIpuSync](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/src/sdk/video/ipu/sce_ipu_sync.c) |

Both use the [pinned MIT licence](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/LICENSE),
copyright 2026 Mateusz Kłysz. Complete notices accompany each canonical fragment
and the generated boot unit. The fragments' capture-receipt reference denotes
private provenance evidence; the source and body identifiers are published here.

## Source and body identity

The temporary-track source SHA-256 is
`0afc1a517eeb21e4db3fd38ecec3e0f343cb13531a296eff78a3d89d3802eafc`.
Its complete reference-body hash is
`881b8eca0af3697f7830d70e73361ed7b9dd5bfd2d88169f4da511bdad7c717d`.
The IPU source SHA-256 is
`7a2d2f2611c3db1c84d4e1757a1fa5a39cd0eef31be225ac8d57912cde5819a2`.
Its complete reference-body hash is
`e9d99b0d0566100caebf1fefb234fe159cc05faba045431055b51a218881ad5c`.

Each original RAC1 body, actual compiled donor object body and RAC2 reference
body agree across the whole extent. The selected donor build used its SDK
compiler, which differs from the qualified RAC2 compiler. That identity supplied
source evidence; separate current RAC2 compilation establishes acceptance.

Original donor structures, parameter types, signedness, statement order and
expressions are retained. Duplicate EE declaration/include context is removed
and function symbols are renamed. The 185 accepted boot-control source prefix
is preserved byte for byte. No failed sysbit body or unique sysbit declaration
is included in the canonical source.

Complete 196-byte and 200-byte original callers and their tail-transfer delay
slots support the pointer/signed-delta interface and integer IPU mode argument.
The temporary-track layout preserves the measured offsets `0x150`, `0x1AC`,
`0x84C`, `0x850` and `0x854`. IPU synchronization preserves the volatile access
to `0x10002010`, the blocking busy-bit loop, polling mode and default return.
These are static ABI and code-generation observations; hardware execution is
not established by the matching proof.

## Mixed trial and final qualification

The first combined trial, `b40912e9de5949e38cbe303c866d950f`, measured all 188
complete functions: 185 controls and these two bodies matched exactly, while
the third target, `FUN_0012E8E8`, emitted 140 bytes against a 152-byte reference.
That is a complete-symbol refusal with zero credit. The mixed event, source,
object and negative result remain immutable. Different tools, flags and unit
contexts prevent attributing its different epilogue arrangement to one cause.
No source or profile permutation was attempted to rescue it.

Removing the failed body changes the translation unit. The reduced 187-function
source therefore received its own actual qualification in trial
`967acd7ce3f443708f3b3f25802f15cf`. Two adapter-only comments then received a
timeless provenance sentence, with all definitions and notices unchanged. Final
source qualification `42030a917af1479e9502eef515e656a2` reproduced all 187 complete
functions under the qualified G0 profile. Both qualified source forms produce
the identical actual object:
`b71624c21c7eb1161c91670dd8b3cae6afb5d1a6f40e678905086cdb8f1a76c6`.
The final source hash is
`fec0fb61d30ac322b5989e7fcf9611b1017d7c64775c507b3f30d48329b66b52`.

Independent replay checks complete object STT extents and owning sections,
linked symbol sizes, full unmasked reference bytes, unchanged controls and
readonly data. The 13 preexisting unaccepted fp-bit helper sections remain
unchanged and uncredited.

The verified global catalogue contains only each boot occurrence with these
complete raw hashes; no exact overlay copy is credited. Original object/module
boundaries remain inferred. Current source reviews, all 28 full-image proofs and
the freshly replayed all-members unique export govern integrated progress.
