# Exact-content source span reuse

`source_layout.function_span` locates a definition in a complete source buffer.
Source rendering, inventory checks and authored-module attribution often request
the same definition from the same buffer more than once. Each request previously
repeated the regular-expression scan and brace lexer.

The generator now reuses successful span results for the exact immutable
`bytes` buffer and exact `str` symbol, with a 16,000-entry LRU limit. A changed
byte anywhere in the buffer selects a different key, including a comment that
moves offsets. Mutable buffers, views, subclasses and invalid arguments retain
the original uncached behavior. Exceptions are not retained. The limit bounds
entries, not total memory; cached buffers remain referenced until eviction or
process exit.

This reuse does not cache source freshness, catalogue validation, fragment
hashes, object proofs or matching decisions. Those checks still read current
inputs. The lexer and all other generator logic are unchanged. Updating the
generator requires regenerating its source-layout manifest, inventory and
supplementary reuse report through their maintained commands. Earlier campaign
receipts remain bound to the script versions actually used by their runs.

## Measured scope

A controlled private run of the existing 13 source-layout tests, using the same
6,999-definition corpus for both executions, took 492.986 seconds without this
cache and 209.788 seconds with it: 57.4% less elapsed time. Both executions
passed all 13 tests. This is a measurement of that test workload, not a promise
for a complete campaign, CI runner or every repository size.

The accepted PR87 corpus has 7,326 catalogued definitions in 58 standalone
source units. These counts describe source organization, not unique algorithms
or replicated physical placements. Current C bodies, catalogues, compiled
checker inputs and matching metrics are preserved by this tooling change;
physical and unique matching deltas are zero.

The cached 13-test run on that larger corpus also passed (295.295 seconds).
All 7,326 cold and warm span results and the complete serialized inventory were
equal to the uncached implementation. Approximate cache storage in that run was
33.1 MiB; this estimate is separate from process resident memory. The old and
new corpus timings are not a controlled speedup comparison.

The primary maintainer's focused PR checks and the unconditional full
merge-queue suite remain described in [MAINTAINER-TESTING.md](MAINTAINER-TESTING.md).
This cache addresses repeated pure source parsing only. Repeated campaign
closure validation and duplicate CI export work remain separate optimization
work.
