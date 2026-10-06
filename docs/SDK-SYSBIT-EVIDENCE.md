# Source-specific SDK qualification of sysbitFlush

The isolated `_sysbitFlush` source reproduces the complete 152-byte Going
Commando boot body at `0x0012E8E8` under the fixed owned SDK pipeline. This
source-specific unit proof is separate from the unchanged default GNU unit.
Published credit requires the complete boot and overlay gates and current
owner-aware object proofs.

## Provenance and source identity

The body is attributed to the pinned MIT-licensed
[Lombyte source](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/src/sdk/debug/sysbit_flush.c),
copyright 2026 Mateusz Kłysz. The complete
[MIT notice](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/LICENSE)
accompanies the standalone source.

The original donor source hash is
`cd0db9a4436bab33ca1cb5f815926797eb734fc5eeeac4f3ef3c44686558f407`.
The isolated source hash is
`b6921af8b6d1fb1b6d65860b0f6130f5a8f6e6bc57777439e6844df8460391f4`.
The adaptation replaces the include with only the four scalar typedefs and
the unchanged `SysbitStream` layout needed by this body. The original symbol,
parameter types, statements, expressions and ordering are preserved.
This source does not establish original retail structure names or module boundaries.

## Fresh compiled-object proof

One actual SDK driver compilation, stripping step and independent SN linker
invocation were performed under the sealed source-specific plan. The actual
driver log identifies integrated preprocessing in `cc1`, followed by the SDK
assembler. A separately available preprocessor is not invoked.

The fixed donor build flags are `-DBUILD_US_VERSION -DMATCHING_DECOMP -O2
-g2 -gstabs` with its original relative include and assembler include arguments.
The diagnostic `-v` records the subprocess chain. The stripping arguments are
`-N dummy-symbol-name -R .mdebug`. The driver, compiler, assembler, strip tool
and linker identities, actual invocation logs and before/after input closure
are retained privately. No SDK or game binary is distributed.

The complete stripped object hash is
`e0a9a1a83aed6b86d94cd1b0e0f71ea6021d64f190c842ea56aa83cef9f3845c`.
It contains exactly one global function, `_sysbitFlush`, with STT extent 152
at offset zero in a complete 152-byte `.text` section aligned to eight bytes.
There are no helper functions, additional allocated data sections, relocation
sections or GP use. The linker places that whole section at the reference address.

Both the whole object text and all 152 linked reference bytes have SHA-256
`70928de6af250170c95f9516617cbc936385634206ffe953ad7dbe2007698ce0`.
The full comparison has zero differing bytes and applies no relocation mask.
This is newly compiled source evidence; the captured donor object was not
substituted for a compilation result.

## Scope and retained negative

The earlier default-compiler trial `b40912e9de5949e38cbe303c866d950f` emitted
140 bytes for this 152-byte target. That source, object and refusal remain
immutable. The SDK experiment uses independently qualified tooling and a
separate standalone unit. It does not identify one isolated cause for the
earlier difference and does not change or reclassify its outcome.

The SDK foundation remains limited to its three accepted leaf controls.
This additional proof qualifies only this exact source unit, including its
variable 64-bit shift and context loads, stores and addition. It does not
authorize arbitrary SDK targets, source permutations, a global compiler
replacement or inferred original compiler provenance. Hardware execution is
not established by byte equality.

## Separate boot object ownership

`src/sdk/sysbit_flush.c` is the authored standalone source;
`candidates/sdk/sysbit_flush.c` is its byte-identical generated compilation unit.
The unit catalogue and review live under `config/boot-units/` and
`progress/boot-units/`. This explicit ownership does not add the SDK symbol to
the default 187-function catalogue or authorize any overlay placement.

The schema-3 boot integration proof contains 188 function rows exactly once.
Its default owner retains the 187-function GNU review, source and object. Its
SDK owner carries the separate full `.text` object and source-specific review.
Each fresh campaign compiles and qualifies both objects independently before
linking them into the complete boot image. The actual complete boot gate
compares 2,521,763 loaded bytes and includes 10,704 bytes of accepted C.
All 27 overlay gates and the all-members unique export remain required before
the resulting measurements are published.
