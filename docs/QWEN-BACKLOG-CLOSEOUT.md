# Retained Qwen backlog: reviewed closeout

The local closeout has passed the complete-unit, all-image, finalizer, and tool-test gates. Three retained sources have fresh mismatch trials and research-only source/metadata pairs. Both the portable shelf check and the retained-evidence check passed for all three entries, with zero integration credit.

The retained 82-job run has an owner disposition for every job. No new model inference was used for this closeout, and Qwen remains paused. Historical results were preserved rather than treated as current integration evidence.

| Outcome | Measured result |
| --- | --- |
| Historical jobs | 70 partial, 11 privately exact, 1 runtime error |
| Imported provenance | 163 tasks and 430 trials: 360 mismatches, 59 compilation failures, 11 privately exact |
| Current source qualification | 10 complete boot functions, 248 bytes exact; one corrected 20-byte function mismatched |
| Complete boot qualification | 297/297 functions, 14,040 bytes exact |
| Full-image integration | Boot and all 27 overlays passed; 79,486,851 loaded bytes compared |
| Measured physical coverage | 700,284 -> 700,532 bytes (+248) |
| Measured unique coverage | 178,740 -> 178,864 bytes (+124); current denominator 44,400,168 |

All 430 imported historical trial records remain equal to their retained originals. Of the 81 imported candidate tasks, ten are now integrated and 71 are stopped. Importing the old tasks and trials adds no matching credit. The ten accepted sources were freshly qualified against the current boot context, then checked together in the complete boot unit and all 28 images. The source lives in [the reviewed boot fragment](../src/boot/100-qwen-backlog-reviewed.cfrag); [the catalogue](../config/candidate-catalog.json), [boot proof](../progress/candidates.json), [integration proof](../progress/integration.json), and [paired metrics](../progress/paired-code-metrics.json) supply the public verification trail.

Five sources could be retained unchanged. Five required one ordinary C correction before their fresh match: consistent word signedness, an external declaration instead of tentative storage, or the already-qualified helper prototype and pointer argument type. The old boot C prefix was preserved, and compiler profiles were retained. No ASM substitution, register trick, or compiler-option cycling was used to obtain credit.

The observed operations are word reads, a word setter, a constant comparison, a sentinel store, helper-derived field reads or writes, and a cleanup call followed by a word clear. The original game roles remain unknown. The review checked current declarations and helper contracts, complete reference bodies, direct callers, argument setup, return use, and relevant data bindings. It does not prove the absence of indirect callers or establish original object layouts, allocation extents, or pointer lifetimes.

`FUN_002C9A60` demonstrates why a privately exact result was insufficient: its original C indexed beyond a declared scalar object. A single correction expressed the two observed stores through separate external word declarations. That defined source differed by nine bytes on fresh qualification and was parked; the unsafe original was not admitted. Other original-source refusals and failed trials remain available as negative history.

The 37 linked partial results remain private retained evidence, not automatically accepted shelf entries. Existing `FUN_001163A0` coverage was recorded without inventing a missing candidate trial or adding credit. Fresh qualification of `FUN_0012FB48`, `FUN_0028B288`, and the corrected `FUN_002C9A60` returned mismatches in all three cases. Their current trials are `b0c8328b644c49a4879bf12948f4d848`, `4c420cb545b448a1ba2f60ef4b86de30`, and `eb241e4ccd8740d78750da20f9e05924`, respectively. The three source/metadata pairs are present under [the nonmatching shelf](../nonmatching/README.md), with a verified index and freshly rechecked retained evidence. All three compare equal-sized complete bodies and add zero matching credit.

| Retained source | Complete body size | Different bytes | Public metadata |
| --- | --- | --- | --- |
| `FUN_0012FB48` | 20 bytes | 3 | [Entry](../nonmatching/boot/0012fb48-14.json) |
| `FUN_0028B288` | 44 bytes | 14 | [Entry](../nonmatching/boot/0028b288-2c.json) |
| Corrected `FUN_002C9A60` | 20 bytes | 9 | [Entry](../nonmatching/boot/002c9a60-14.json) |

Reopening requires new compiler or structural evidence for the first two entries, or new declaration or structural evidence for the corrected store. No automatic retry is scheduled.

The first finalizer attempt failed its tests because its snapshot omitted `.github` files needed by the reservation and PR-description tests. The snapshot inventory was corrected, with 51 targeted tests passing, and the full-image gate was rerun under the updated input pins. The final all-image action is `3291fd09b8a84c74ba7d1c38407beb85`, report SHA-256 `351e3757a71f78b2414074d51254d732f21a9c94d08b5496680caf2657212195`. The finalizer applied successfully with 41 backups and closed all 11 selected candidate tasks. Its tool suite ran 820 tests in 434.914 seconds: OK, with two skipped. Source-layout, source-inventory, register-view consistency, and whitespace checks also passed. Native gameplay has not been verified.

This closes retained work that began before the reservation workflow. It does not authorize another run or claim a next batch. Any future function work must follow [the contributor reservation workflow](CONTRIBUTOR-RESERVATIONS.md).
