# PR54: resident word clear

[liolu's contribution](https://github.com/OpenRAC/rac2-gc-decomp/pull/54),
original commit `0634723e4a72597252b5e4385ca0e62cdbe0d72a`, adds the
12-byte boot function `FUN_0011B0A0`. Its source clears the aligned 32-bit
cell at `0x00134688` in mutable resident data. The original source declaration,
data-object boundaries and game purpose remain unknown beyond the observed store.

The independently checked static caller at `0x0011EEA0` calls it at
`0x0011EEC0` and overwrites `V0` immediately on return at `0x0011EEC8`.
The `void(void)` source has no observed return-use contradiction. Static scans
do not establish the absence of every possible indirect or dynamic caller.
The catalogue's reference count was corrected from zero to one observed call.

The original authored fragments are preserved. Generated outputs were reconciled
against the current main branch rather than replacing its preceding contribution
lot. Fresh qualification matches all 286 complete boot functions and preserves
the previous 285 bodies. The four C tool identities and both reconstruction tool
identities agree with the qualified chain; compiler flags are unchanged.

The original contributor's three action records, including the failed private
binding attempt, remain retained as external historical metadata with zero credit.
Current acceptance uses independently generated local proofs.

Campaign `1c29b1a1ee654c02a5154350b275ea36` passes all 28 loaded-image and
metadata gates, comparing **79,486,851 loaded bytes**. Immutable report SHA-256:
`54aec7c8d9d1b15377e4638c0b8e79a93e4c78dd1b4d684f40ffe24ebbd23783`.

Guarded finalization accepts **700,268 physical C bytes**, up 12 from
700,256. The separately regenerated conservative unique result is
**178,724 / 44,400,168 bytes**. Tool tests: **701 passed,
2 skipped**. Required CI must pass on the final publication commit
before merge. Private images, object files, tools and runtime data are excluded.
