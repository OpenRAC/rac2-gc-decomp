# Twenty-fourth RAC2 C lot: aligned 128-bit zero store

`FUN_00282C88` clears one aligned 128-bit object with one C assignment through
the repository's existing `TI` type. The unchanged qualified `8bed6eae` profile
reproduces its entire 8-byte body. The historical `dff08a34` baseline produced
12 bytes; this lot makes no compiler changes.

An isolated batch requalified six preserved leaf sources. Only this body
matched; the other five refusals remain in the [experiment register](C-NATIVE-EXPERIMENT-REGISTER.md).
The first full-source attempt redeclared `mode(TI)` after `TI` became a typedef,
causing an old-compiler parser error. Reusing the existing type fixed the error,
and all 178 boot symbols then matched.

The placement search resolves the body to one reviewed function entry in each
of the 27 pinned overlays. Every overlay and the complete boot were rebuilt and
gated. The new review was normalized to LF before the final gate pass so every
native integration records the published review hash.

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 177 / 9,328 | 178 / 9,336 |
| Overlay placements / bytes | 4,131 / 204,792 | 4,158 / 205,008 |
| Native placements / bytes | 58 / 2,896 | 58 / 2,896 |
| Integrated C bytes | 214,120 | 214,344 |
| Executable-code proportion | 0.4389% | 0.4393% |

Validation: 178/178 complete boot symbols, all 2,521,763 loaded boot bytes, all
27 complete overlay gates, 176 repository tests, and a fresh export with
`matched_code=214344` and `total_code=48788176`. Existing exporter fixtures were
updated to the new committed corpus counts. The measured gain is 224 bytes:
8 in the boot plus 216 across the overlays.

The 0.5%, 10%, and complete-game objectives remain open. Native gameplay is
unverified. Game images, objects and source snapshots stay private; the
repository contains authored C, structural identifiers and proof metadata.
