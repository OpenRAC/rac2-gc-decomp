# Fixed SDK driver qualification on accepted controls

A separate owned GNU EE 2.9-ee-991111-01 SDK driver pipeline reproduces three
already accepted boot C bodies. It does not replace the reconstructed default
compiler or increase matching progress. Qualification is limited to the measured
single-function control units; general SDK, call-bearing, GP and 64-bit algorithm
compatibility have not been established by these controls.

## Actual instruments and invocation

| Role | SHA-256 |
| --- | --- |
| SDK driver | `64d0a50fef499da0b98177eb5e79e41dfb066ad44246b137c78a266ef97ee265` |
| SDK cc1, including preprocessing | `b9aef69f93efb949f15ea58189e8eef4a002b9fe4de5d3fcf89c34e1244ec026` |
| SDK assembler | `296af123052ee39d175e5b3254102aafca105d4f6e975b351a13867f59b04019` |
| Owned strip utility | `fce5d577f2ed6bb8bdb61028d44c5b6be87187b55230e1bb41eccb0053cbbf76` |
| Existing SN linker | `80f3724a63ce3c77c8b00d075f5c49448636518700ae251e5d6a6c809bbf5f48` |
| Available SDK cpp, not invoked | `f85a54d241e019993fa7a06285d3677856b3b431e10318a587ab029373c603d7` |

The actual driver log shows direct `cc1 -lang-c` invocation with integrated
preprocessing, followed by the assembler. The separately available cpp binary
is an availability pin and is not an executed producer in this pipeline.

The fixed driver options are `-c`, `-I../../src`, `-I../../include`, `-Iinclude`,
`-Wa,-I../../include`, `-Wa,-I../..`, `-DBUILD_US_VERSION`, `-DMATCHING_DECOMP`,
`-O2`, `-g2` and `-gstabs`. Diagnostic `-v` is fixed in both passes. The three
include roots are empty and the standalone control sources contain no includes,
time macros or unseen headers. The source context uses the same reviewed scalar
declarations and complete MIT fragments as the accepted boot unit.

Postprocessing uses the owned strip utility with `-N dummy-symbol-name -R
.mdebug`. Both pre-strip and post-strip objects are retained; executable bytes
remain unchanged. A whole single `.text` section is linked at the measured
control address using the existing SN linker. The pipeline adds neither `-G0`
nor `-ffunction-sections`; defaults are bound by the actual driver route and
fixed tool identities, rather than assumed equivalent to the default compiler.

## Six actual qualifications

Each of the following sources was compiled once in each of two fresh output
banks. The bare source basename and native filesystem working directory are
fixed; arbitrary-workspace portability is not claimed.

| Control | Address | Complete bytes | Post-strip object SHA-256 |
| --- | --- | ---: | --- |
| Dual-prime motion vectors | `0x0012B3E0` | 388 | `46035a2c472008a87532ed9c475a65d75279f844f2aba52789b2381e9c89361c` |
| Temporary-track update | `0x0012CFE8` | 116 | `cd51231c3d726cceae8773cb717973522dc178289f917ada87e94979dc4615b0` |
| IPU synchronization | `0x00130DB8` | 104 | `455686ca465c1b45b131a58abbbd934d223fe6f2ec026b3e1a5fb336bf689cfc` |

All six results pass complete global STT value/size/owning-section checks, whole
`.text` ownership without padding or additional functions, linked symbol checks
and full unmasked reference-byte comparisons. No generated readonly or other
ordinary allocated data is present. Source, pre-strip object, post-strip object
and linked ELF hashes are identical between passes for each control. Actual
tool closure, references, source inputs and the existing default compiler
identities were checked before and after qualification.

The first compile produced a valid exact object, but the observer initially
expected a separate cpp process. It refused before strip or link. The original
object, script, log and refusal were preserved; a metadata-only observer repair
recognized the actual integrated-preprocessing route and resumed that same
object. Five subsequent compiles complete the six-compilation total. No source,
optimization flag or failed-target permutation was attempted.

The complete private qualification receipt is identified by SHA-256
`045c484fe3d434774f0687602c04b4be6fd0876e49d9dbb9a29ef3319f4e4c0c`.
Independent inspection replays all six actual objects and links, complete
reference bytes and cross-pass determinism. Runtime paths, binaries and game
bytes remain outside the public repository.

## Provenance and next use

The owned SDK installation matches the pinned donor prebuilt archive identifier
`ed684fd98f89d36b0121caab311052089103e3b36241fcef4338cc9ea41c75b8`.
An exact source rebuild of this SDK binary has not been established. Nearby
optional patched GNU build recipes are not evidence of a byte-identical rebuild.
The original donor rule selects these SDK source units explicitly; its address
threshold must not be copied as an original RAC2 ownership rule.

Compatibility with these three control units does not identify Insomniac's
original compiler or guarantee compatibility with another body. Existing sysbit
140/152 and DMA 736/656 refusals remain preserved. A future unit requires an
explicit qualified profile, fixed source and invocation, complete object and
unmasked byte proof, disjoint ownership and all affected full-image gates before
it receives C credit. The accepted default boot and native G0/G8 profiles remain
the authority for their current objects.
