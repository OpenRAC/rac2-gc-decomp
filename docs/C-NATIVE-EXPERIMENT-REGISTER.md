# Historical C experiment view

Generated from the immutable legacy documents in
`config/campaign-register.json`. This file is a historical view;
current tasks and new trials live only in that structured register.

The original text below is preserved verbatim, including dated terminology.

Original document SHA-256: `e40363e07a64355915836420caf5116d940cb730cbd06d5628c3aad938f0c19b`.

# C Experiment Register

This is the canonical index for the recorded RAC2 C experiments. The original table covers `levels/24_ship_shack`; boot requalifications are appended below. Check it before proposing or running a new source variant. Add one row immediately after each trial; never overwrite a prior row. The run directory is the evidence package and retains the exact source, catalog, object, assembly, logs, and qualification result. Ghidra remains the place for function analysis and annotations; this register records compiler experiments and their outcomes. The repository tracks this index; generated game objects, binaries, and runtime artifacts remain local.

## Current scope and interpretation

- Reference: `SCUS_972.68`, `/levels/24_ship_shack.elf`, pinned ELF SHA-256 `4afbc22add84109c84ef8ca49099fdf9863905e5f497369c207ac5cb47814740`.
- This register inventories 158 target records across 53 functions: 33 exact results, 97 byte mismatches, 2 archived runs without proof, 9 compiler failures, 8 link failures and 9 source/catalog rejections. Shared-unit failure rows identify attempted targets and do not infer individual body measurements. Exact trial records are not integration credit.
- The 51 proof-bearing trials from the original candidate bank used the historical `dff08a34` C compiler profile with `-O2 -G0 -ffunction-sections`; seven later measured `FUN_002B0408` variants, five measured `FUN_002F2718` trials, two measured `FUN_00323430` variants, and the two latest exact promotions used the current `8bed6eae` profile with the same flags. Two earlier 8bed-labelled folders do not retain qualification proofs and are explicitly marked unverified; the register-binding source was separately rejected before compilation. Other pinned tool hashes for proof-bearing trials are recorded in their local evidence. Historical exact results are not current-profile qualification; requalify a candidate with the current checker before treating an old exact result as current evidence.
- The five historical exact targets are now present in the public level-native catalog at the repository revision where this register was created. The table records the trial result and its original profile, not a substitute for current proofs.
- Classification uses `qualification_passed` and `proof.functions[].matched`; `proof.state` alone is not decisive. Aggregate `*-results.json` files duplicate these per-run records. Deduplicate by run directory / `work` path.
- Archived evidence remains in the private GAMING work area. Each evidence path is relative to the private `D:\RAC2\work` root. Candidate-bank runs retain `result.json` and `catalog.json`; source and compiler artifacts are present where the run reached that stage. Proof-bearing runs also contain object qualification data. The two original no-proof runs preserve their compiler/chain errors in `result.json` and `compile.log`.

## Archived Ship Shack native runs

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---:|---|---|
| `0x00323430` | `FUN_00323430` | 112 | `run-append_pointer_array-92a84db7` | `dff08a34` | MISMATCH | 35 / 112 bytes | `a72d21fa603011a2` | `nuit-codex-level-candidates/run-append_pointer_array-92a84db7` |
| `0x00323430` | `FUN_00323430` | 112 | `run-append_wide_inputs-ae7975c5` | `dff08a34` | MISMATCH | 108 produced / 112 target bytes | `4456bafaf14b3f0a` | `nuit-codex-level-candidates/run-append_wide_inputs-ae7975c5` |
| `0x002B0408` | `FUN_002B0408` | 72 | `run-baseline_002b0408-5bc81c7f` | `dff08a34` | MISMATCH | 16 / 72 bytes | `ca61eb6acd0768a9` | `nuit-codex-level-candidates/run-baseline_002b0408-5bc81c7f` |
| `0x002D1570` | `FUN_002D1570` | 72 | `run-baseline_002d1570-619cefcf` | `dff08a34` | MISMATCH | 20 / 72 bytes | `a953f724e7eda06a` | `nuit-codex-level-candidates/run-baseline_002d1570-619cefcf` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-baseline_002d2348-e79cfd20` | `dff08a34` | MISMATCH | 47 / 68 bytes | `4bcb6462cbf857cd` | `nuit-codex-level-candidates/run-baseline_002d2348-e79cfd20` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-baseline_002e7a08-b15ae0bf` | `dff08a34` | MISMATCH | 17 / 156 bytes | `b84f46ffe3f8a5fe` | `nuit-codex-level-candidates/run-baseline_002e7a08-b15ae0bf` |
| `0x002F2510` | `FUN_002F2510` | 72 | `run-baseline_002f2510-0cadf90c` | `dff08a34` | MISMATCH | 80 produced / 72 target bytes | `316d1cf592bc13c7` | `nuit-codex-level-candidates/run-baseline_002f2510-0cadf90c` |
| `0x002F2718` | `FUN_002F2718` | 64 | `run-baseline_002f2718-0c12e506` | `dff08a34` | MISMATCH | 68 produced / 64 target bytes | `18c1213733ad34dc` | `nuit-codex-level-candidates/run-baseline_002f2718-0c12e506` |
| `0x002F2B48` | `FUN_002F2B48` | 136 | `run-baseline_002f2b48-207f527d` | `dff08a34` | MISMATCH | 124 produced / 136 target bytes | `4ffc0779ebe726f3` | `nuit-codex-level-candidates/run-baseline_002f2b48-207f527d` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-baseline_002f5de0-a374aeae` | `dff08a34` | MISMATCH | 5 / 80 bytes | `dba6a6e2986b0fa8` | `nuit-codex-level-candidates/run-baseline_002f5de0-a374aeae` |
| `0x0030E7F0` | `FUN_0030E7F0` | 188 | `run-baseline_0030e7f0-bf5a2641` | `dff08a34` | MISMATCH | 134 / 188 bytes | `fb9864634809a151` | `nuit-codex-level-candidates/run-baseline_0030e7f0-bf5a2641` |
| `0x00323430` | `FUN_00323430` | 112 | `run-baseline_00323430-dfa900cb` | `dff08a34` | MISMATCH | 108 produced / 112 target bytes | `f1a006f5e8e4d51c` | `nuit-codex-level-candidates/run-baseline_00323430-dfa900cb` |
| `0x0034F218` | `FUN_0034F218` | 80 | `run-baseline_0034f218-dc21eb5e` | `dff08a34` | MISMATCH | 47 / 80 bytes | `1047265ba1e0d602` | `nuit-codex-level-candidates/run-baseline_0034f218-dc21eb5e` |
| `0x0035E348` | `FUN_0035E348` | 72 | `run-baseline_0035e348-e1080d23` | `dff08a34` | MISMATCH | 12 / 72 bytes | `882bbd07a55436f1` | `nuit-codex-level-candidates/run-baseline_0035e348-e1080d23` |
| `0x0035E348` | `FUN_0035E348` | 72 | `run-context_complete_array-8f763eee` | `dff08a34` | MISMATCH | 12 / 72 bytes | `382fbd170a89475e` | `nuit-codex-level-candidates/run-context_complete_array-8f763eee` |
| `0x0035E348` | `FUN_0035E348` | 72 | `run-context_incomplete_sda-2405ddb5` | `dff08a34` | MISMATCH | 80 produced / 72 target bytes | `78f680be41521da4` | `nuit-codex-level-candidates/run-context_incomplete_sda-2405ddb5` |
| `0x0035E348` | `FUN_0035E348` | 72 | `run-context_struct_base-90cf4106` | `dff08a34` | EXACT; now in public catalog | 0 | `d8bdf12c50f20baf` | `nuit-codex-level-candidates/run-context_struct_base-90cf4106` |
| `0x0030E7F0` | `FUN_0030E7F0` | 188 | `run-descriptor_cached_slots-121632fd` | `dff08a34` | MISMATCH | 125 / 188 bytes | `5c85feccfe1beeea` | `nuit-codex-level-candidates/run-descriptor_cached_slots-121632fd` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-init_all_tail_volatile-6a5858c3` | `dff08a34` | MISMATCH | 13 / 80 bytes | `c94075386ebb5930` | `nuit-codex-level-candidates/run-init_all_tail_volatile-6a5858c3` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-init_both_shorts_volatile-cebbd3f5` | `dff08a34` | MISMATCH | 25 / 80 bytes | `49e9b55ec71a0161` | `nuit-codex-level-candidates/run-init_both_shorts_volatile-cebbd3f5` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-init_last_short_volatile-5b7da766` | `dff08a34` | MISMATCH | 5 / 80 bytes | `b73bc3f40959ed7d` | `nuit-codex-level-candidates/run-init_last_short_volatile-5b7da766` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-init_order_shorts_reversed-83d6ad80` | `dff08a34` | MISMATCH | 4 / 80 bytes | `31ac5621b973a2f6` | `nuit-codex-level-candidates/run-init_order_shorts_reversed-83d6ad80` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-init_order_word_after_shorts-e7a99fa4` | `dff08a34` | EXACT; now in public catalog | 0 | `2dd45e7150cd5e23` | `nuit-codex-level-candidates/run-init_order_word_after_shorts-e7a99fa4` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-refined_init_last_two_volatile-ad64fbc6` | `dff08a34` | MISMATCH | 24 / 80 bytes | `7f61295ef55e5787` | `nuit-codex-level-candidates/run-refined_init_last_two_volatile-ad64fbc6` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-refined_init_return_view-9bf15816` | `dff08a34` | MISMATCH | 9 / 80 bytes | `c46acae14a832983` | `nuit-codex-level-candidates/run-refined_init_return_view-9bf15816` |
| `0x002F5DE0` | `FUN_002F5DE0` | 80 | `run-refined_init_zero_tail_volatile-1ceb33ad` | `dff08a34` | MISMATCH | 27 / 80 bytes | `be33b5a52d041981` | `nuit-codex-level-candidates/run-refined_init_zero_tail_volatile-1ceb33ad` |
| `0x0034F218` | `FUN_0034F218` | 80 | `run-search_continue_cfg-db30286e` | `dff08a34` | EXACT; now in public catalog | 0 | `66ba195f719100dd` | `nuit-codex-level-candidates/run-search_continue_cfg-db30286e` |
| `0x0034F218` | `FUN_0034F218` | 80 | `run-search_sda_addresses-8d276c93` | `dff08a34` | MISMATCH | 36 / 80 bytes | `e5e869ba06aa0b5e` | `nuit-codex-level-candidates/run-search_sda_addresses-8d276c93` |
| `0x002D1570` | `FUN_002D1570` | 72 | `run-selector_incomplete_sda-52ea304b` | `dff08a34` | MISMATCH | 80 produced / 72 target bytes | `2ca08d2c85f880b1` | `nuit-codex-level-candidates/run-selector_incomplete_sda-52ea304b` |
| `0x002D1570` | `FUN_002D1570` | 72 | `run-selector_offset_birth-261b5338` | `dff08a34` | EXACT; now in public catalog | 0 | `7d1d28d493d31a82` | `nuit-codex-level-candidates/run-selector_offset_birth-261b5338` |
| `0x002D1570` | `FUN_002D1570` | 72 | `run-selector_struct_s32-b9338ee8` | `dff08a34` | MISMATCH | 8 / 72 bytes | `3fbfa4093350147d` | `nuit-codex-level-candidates/run-selector_struct_s32-b9338ee8` |
| `0x002D1570` | `FUN_002D1570` | 72 | `run-selector_struct_wide-d0c64ddc` | `dff08a34` | MISMATCH | 88 produced / 72 target bytes | `6039b0adb9e783e6` | `nuit-codex-level-candidates/run-selector_struct_wide-d0c64ddc` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_cast_before_flags-ad449d37` | `dff08a34` | MISMATCH | 14 / 68 bytes | `4dfd6609b99755b1` | `nuit-codex-level-candidates/run-settings_cast_before_flags-ad449d37` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_mode_early-aa74f52e` | `dff08a34` | MISMATCH | 13 / 68 bytes | `0509539fb5233ae1` | `nuit-codex-level-candidates/run-settings_mode_early-aa74f52e` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_mode_early_x_first-89bc052a` | `dff08a34` | MISMATCH | 14 / 68 bytes | `a2fa1e5d2f86d39b` | `nuit-codex-level-candidates/run-settings_mode_early_x_first-89bc052a` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_ordered_writes-4be74479` | `dff08a34` | MISMATCH | 72 produced / 68 target bytes | `14836712d25ad4ee` | `nuit-codex-level-candidates/run-settings_ordered_writes-4be74479` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_short_formal-54b5fb5f` | `dff08a34` | MISMATCH | 14 / 68 bytes | `2cd7a19ff104f9a9` | `nuit-codex-level-candidates/run-settings_short_formal-54b5fb5f` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_struct_view-d863909a` | `dff08a34` | MISMATCH | 29 / 68 bytes | `99220af885202928` | `nuit-codex-level-candidates/run-settings_struct_view-d863909a` |
| `0x002D2348` | `FUN_002D2348` | 68 | `run-settings_x_first-c850588d` | `dff08a34` | MISMATCH | 30 / 68 bytes | `6f19a51107c43aa5` | `nuit-codex-level-candidates/run-settings_x_first-c850588d` |
| `0x002F2718` | `FUN_002F2718` | 64 | `run-space_inverted_cfg-a7be93fa` | `dff08a34` | MISMATCH | 68 produced / 64 target bytes | `d91c518aa90f2bcf` | `nuit-codex-level-candidates/run-space_inverted_cfg-a7be93fa` |
| `0x002F2718` | `FUN_002F2718` | 64 | `run-space_scalar_incomplete_arrays-9937fec6` | `dff08a34` | MISMATCH | 17 / 64 bytes | `50b02302fe0d7b61` | `nuit-codex-level-candidates/run-space_scalar_incomplete_arrays-9937fec6` |
| `0x002F2510` | `FUN_002F2510` | 72 | `run-table_items_array-0bafc812` | `dff08a34` | EXACT; now in public catalog | 0 | `fb1d072ffb6e656d` | `nuit-codex-level-candidates/run-table_items_array-0bafc812` |
| `0x002F2510` | `FUN_002F2510` | 72 | `run-table_packed_align4-64e4e377` | `dff08a34` | MISMATCH | 4 / 72 bytes | `3e1f6af8284c9bfe` | `nuit-codex-level-candidates/run-table_packed_align4-64e4e377` |
| `0x002F2510` | `FUN_002F2510` | 72 | `run-table_word_array-f4204749` | `unknown` | NO PROOF | not measured | `unknown` | `nuit-codex-level-candidates/run-table_word_array-f4204749` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_arrays_named_y_first-377bdf4d` | `dff08a34` | MISMATCH | 8 / 156 bytes | `24d1bf7924fb3863` | `nuit-codex-level-candidates/run-vertices_arrays_named_y_first-377bdf4d` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_arrays_y1_first-63d74fca` | `dff08a34` | MISMATCH | 8 / 156 bytes | `d3eeee81741ad30f` | `nuit-codex-level-candidates/run-vertices_arrays_y1_first-63d74fca` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_incomplete_arrays-4e1cc476` | `dff08a34` | MISMATCH | 8 / 156 bytes | `98ec3ce19397ba9a` | `nuit-codex-level-candidates/run-vertices_incomplete_arrays-4e1cc476` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_mutated_args-6e1693b0` | `dff08a34` | MISMATCH | 17 / 156 bytes | `d438382812d9c867` | `nuit-codex-level-candidates/run-vertices_mutated_args-6e1693b0` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_mutated_args-83503df8` | `unknown` | NO PROOF | not measured | `unknown` | `nuit-codex-level-candidates/run-vertices_mutated_args-83503df8` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_named_offsets-8c67dac0` | `dff08a34` | MISMATCH | 25 / 156 bytes | `13cb0d6559b17876` | `nuit-codex-level-candidates/run-vertices_named_offsets-8c67dac0` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_volatile_load_order-35302cfa` | `dff08a34` | MISMATCH | 8 / 156 bytes | `56b07019e9c25090` | `nuit-codex-level-candidates/run-vertices_volatile_load_order-35302cfa` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_y1_first-0e39ccb7` | `dff08a34` | MISMATCH | 25 / 156 bytes | `8d07aae2c292d564` | `nuit-codex-level-candidates/run-vertices_y1_first-0e39ccb7` |
| `0x002E7A08` | `FUN_002E7A08` | 156 | `run-vertices_z_s32-f1ea1b8a` | `dff08a34` | MISMATCH | 8 / 156 bytes | `441f7f77003822a0` | `nuit-codex-level-candidates/run-vertices_z_s32-f1ea1b8a` |

| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-root8bed-work` | `8bed6eae` | MISMATCH | 9 / 72 bytes | `bc46676d9700599a` | `nuit-codex-prologue/native-2b0408-root8bed-work` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-computed-table-labelhints8bed-work` | `8bed6eae` | MISMATCH | 48 produced / 72 target bytes | `fd2c4d3b64722065` | `nuit-codex-prologue/native-2b0408-computed-table-labelhints8bed-work` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-computed-table-root8bed-work` | `8bed6eae` | MISMATCH | 48 produced / 72 target bytes | `56ea5283a9ccffed` | `nuit-codex-prologue/native-2b0408-computed-table-root8bed-work` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-ifchain-root8bed-work-v2` | `8bed6eae` | MISMATCH | 76 produced / 72 target bytes | `f7d121dae73326ea` | `nuit-codex-prologue/native-2b0408-ifchain-root8bed-work-v2` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-local-table-run8bed` | `8bed6eae` | MISMATCH | 4 / 72 bytes | `76d8a9e1d030f471` | `nuit-codex-prologue/native-2b0408-local-table-run8bed` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-values-run8bed` | `8bed6eae` | MISMATCH | 4 / 72 bytes | `6b41d6e7d33bd617` | `nuit-codex-prologue/native-2b0408-values-run8bed` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-values-reversed-run8bed` | `8bed6eae` | MISMATCH | 4 / 72 bytes | `6c230e2ba68504eb` | `nuit-codex-prologue/native-2b0408-values-reversed-run8bed` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-ifchain-root8bed-work` | `8bed path label; unverified` | COMPILE FAIL | parse error; no object | `699cd6958261562a` | `nuit-codex-prologue/native-2b0408-ifchain-root8bed-work` |
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-register-values-run8bed` | `8bed label; tool hash unavailable` | MISMATCH (manual ELF comparison) | 4 / 72 bytes | `912832837f600f8e` | `nuit-codex-prologue/native-2b0408-register-values-run8bed` |
| `0x002D1570` | `FUN_002D1570` | 72 | `native-24-002d1570-8bed/native-candidates/9a830e6b` | `8bed6eae` | EXACT; publicly integrated | 0 | `da33359d7ab9dfe7` | `native-24-002d1570-8bed/native-candidates/9a830e6b` |
| `0x0035E348` | `FUN_0035E348` | 72 | `native-24-0035e348-8bed/native-candidates/ae6987b9` | `8bed6eae` | EXACT; publicly integrated | 0 | `e1cb74637497fd03` | `native-24-0035e348-8bed/native-candidates/ae6987b9` |

| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed/trial-success-first-global-order` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `28244d9ce6f4c990` | `nuit-codex-prologue/native-2f2718-root8bed/trial-success-first-global-order` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-9a6b06b/trial-failure-first-global-order` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `fa123e6217db4b82` | `nuit-codex-prologue/native-2f2718-root8bed-9a6b06b/trial-failure-first-global-order` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-47de0aa/trial-unlikely-error-hint` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `47596ce3a2d58d5e` | `nuit-codex-prologue/native-2f2718-root8bed-47de0aa/trial-unlikely-error-hint` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-44663e4/trial-output-pointer-a1` | `unverified` | REJECTED (not genuine C) | checker rejected before compile | `c9e8bed6b418154b` | `nuit-codex-prologue/native-2f2718-root8bed-44663e4` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-1567a44/trial-output-zero-before-guard` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `91522fb7d31af33c` | `nuit-codex-prologue/native-2f2718-root8bed-1567a44/trial-output-zero-before-guard` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-d2e7070/trial-output-zero-expect-true` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `8499d0e74683785c` | `nuit-codex-prologue/native-2f2718-root8bed-d2e7070/trial-output-zero-expect-true` |
| `0x00323430` | `FUN_00323430` | 112 | `native-323430-root8bed-299e03a/trial-baseline-current-profile` | `8bed6eae` | MISMATCH | 108 produced / 112 target bytes | `9ac0e2a8242a9473` | `nuit-codex-prologue/native-323430-root8bed-299e03a/trial-baseline-current-profile` |
| `0x00323430` | `FUN_00323430` | 112 | `native-323430-root8bed-fddf057/trial-global-pointer-volatile` | `8bed6eae` | MISMATCH | 108 produced / 112 target bytes | `437fa4953b2871b5` | `nuit-codex-prologue/native-323430-root8bed-fddf057/trial-global-pointer-volatile` |
| `0x002F2B48` | `FUN_002F2B48` | 136 | `native-2f2b48-current-3ffc9ce/trial-baseline` | `8bed6eae` | MISMATCH | 124 produced / 136 target bytes | `4aa343b081032ad9` | `nuit-codex-prologue/native-2f2b48-current-3ffc9ce/trial-baseline` |

| `0x002F2B48` | `FUN_002F2B48` | 136 | `native-2f2b48-direct-fields-8bed/trial-direct-fields` | `8bed6eae` | MISMATCH | 26 / 136 bytes | `1849f21220d919e5` | `nuit-codex-prologue/native-2f2b48-direct-fields-8bed/trial-direct-fields` |

| `0x002F2B48` | `FUN_002F2B48` | 136 | `native-2f2b48-array-state-8bed/trial-array-state` | `8bed6eae` | MISMATCH | 152 produced / 136 target bytes | `8bf29f22a64fe7be` | `nuit-codex-prologue/native-2f2b48-array-state-8bed/trial-array-state` |

| `0x002F2B48` | `FUN_002F2B48` | 136 | `native-2f2b48-struct-state-8bed/trial-struct-state` | `8bed6eae` | MISMATCH | 152 produced / 136 target bytes | `289e2fdd27852c84` | `nuit-codex-prologue/native-2f2b48-struct-state-8bed/trial-struct-state` |

| `0x002F2B48` | `FUN_002F2B48` | 136 | `native-2f2b48-struct-state-normal-8bed/trial-normal-struct` | `8bed6eae` | MISMATCH | 7 / 136 bytes | `1b24b8c2695482a7` | `nuit-codex-prologue/native-2f2b48-struct-state-normal-8bed/trial-normal-struct` |

| `0x002F2B48` | `FUN_002F2B48` | 136 | `native-2f2b48-struct-state-normal-8bed/trial-table-placement` | `8bed6eae` | MISMATCH (generated-table placement) | 4 / 136 bytes | `1b24b8c2695482a7` | `nuit-codex-prologue/native-2f2b48-struct-state-normal-8bed/trial-table-placement` |


## New trial entry format

Append a row after every compiler experiment, including a failed compile or a no-match. Keep its complete run directory. Record the address, function size, immutable trial ID, compiler profile, qualification result, byte difference or size, source hash, and evidence path. If the source or compiler profile changes, it is a new trial. If evidence is unavailable, write `unknown` rather than inferring a value. Update the inventory totals and exact-current-profile status only from checker output.


## Boot requalification — current 8bed profile

These six preserved leaf sources were previously rejected under `dff08a34`. They were compiled together once under `8bed6eae`, using the unchanged flags and pinned boot reference. This is a new tool-profile measurement, not a new source guess. The batch source SHA-256 is `bf776cd816911e1d8f998a5595c6801bd39cce3111692c1eec776dc52553c3f9`. Each row points to the same complete private run, with its target as the unique record key. An exact private result requires full boot and overlay integration before it counts as progress.

| Target | Program | Trial record | C profile | Result | Produced / target bytes | Source SHA prefix | Evidence directory |
|---|---|---|---|---|---|---|---|
| `FUN_00282C88` | boot | `resume8bed-46170a12:FUN_00282C88` | `8bed6eae` | EXACT; integration pending | 8 / 8 | `bf776cd816911e1d` | `nuit-codex-prologue/boot-resume-3ffc9ce-8bed/run-resume8bed-46170a12` |
| `FUN_00283678` | boot | `resume8bed-46170a12:FUN_00283678` | `8bed6eae` | MISMATCH | 28 / 32 | `bf776cd816911e1d` | `nuit-codex-prologue/boot-resume-3ffc9ce-8bed/run-resume8bed-46170a12` |
| `FUN_0029CF58` | boot | `resume8bed-46170a12:FUN_0029CF58` | `8bed6eae` | MISMATCH | 36 / 32 | `bf776cd816911e1d` | `nuit-codex-prologue/boot-resume-3ffc9ce-8bed/run-resume8bed-46170a12` |
| `FUN_00300300` | boot | `resume8bed-46170a12:FUN_00300300` | `8bed6eae` | MISMATCH | 44 / 48 | `bf776cd816911e1d` | `nuit-codex-prologue/boot-resume-3ffc9ce-8bed/run-resume8bed-46170a12` |
| `FUN_003495A0` | boot | `resume8bed-46170a12:FUN_003495A0` | `8bed6eae` | MISMATCH (6 differing bytes) | 20 / 20 | `bf776cd816911e1d` | `nuit-codex-prologue/boot-resume-3ffc9ce-8bed/run-resume8bed-46170a12` |
| `FUN_00349610` | boot | `resume8bed-46170a12:FUN_00349610` | `8bed6eae` | MISMATCH | 20 / 24 | `bf776cd816911e1d` | `nuit-codex-prologue/boot-resume-3ffc9ce-8bed/run-resume8bed-46170a12` |
| `FUN_00282C88` | full boot source | `full-zero128-beed6e87` | `8bed6eae` | COMPILE FAIL: existing TI typedef conflicts with mode attribute token | not produced / 8 | `unknown` | `nuit-codex-prologue/boot-zero128-full/candidate-runs/beed6e87` |
| `FUN_00282C88` | full boot source | `full-zero128-reuse-type-0d1cb468` | `8bed6eae` | EXACT; 178/178 complete boot symbols | 8 / 8 | `0de6cd7c5c971515` | `nuit-codex-prologue/boot-zero128-full-reuse-type/candidate-runs/0d1cb468` |

The accepted full-source form reuses the existing `TI` typedef. It avoids redeclaring a mode attribute whose token `TI` has already become a type name in this old compiler. No assembly source or instruction bytes are embedded.

The boot zero-store result was subsequently integrated through [lot 24](TWENTY-FOURTH-C-LOT.md): 178 boot symbols, 27 complete overlay gates, and an exported total of 214,344 C bytes. Trial rows retain their original qualification states.


## PCSX2 observations

These two runtime observations are not compiler trials and do not alter the
source-trial counts above. Evidence directories are relative to the private
PCSX2 session bank. See [the validation record](../progress/pcsx2/boot-lot24.json).

| Probe | Instrument | Outcome | Private evidence |
|---|---|---|---|
| Retail ISO | PCSX2 d75a0ad, DebugServer 21512, PINE 28012 | SCUS-97268 v1.01; loaded function sample equals pinned bytes; breakpoint hit not observed | `20261003-162346-bf1dbc7d/mcp-reference-observations.json` |
| Lot 24 rebuilt ELF + same ISO | Same dedicated profile | SCUS-97268 v1.01; loaded function sample equals retail/pinned/rebuilt bytes; gameplay unverified | `20261003-163125-1a5e81ad/mcp-rebuilt-observations.json` |

## Tail-pointer state experiment — current 8bed profile

This ordinary-C hypothesis computes the next state in the switch, then forms a partial-structure pointer in the shared tail before writing state and clearing field4. It preserves the void return and the observed fields. The six integrated native controls remain exact. Both measurements produce a complete 128-byte symbol against the pinned 136-byte function; explicit placement of the generated table also changes its entries. Neither result is integrated.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002F2B48` | `FUN_002F2B48` | 136 | `tail-pointer-020b639a:normal` | `8bed6eae` | MISMATCH | 128 produced / 136 target bytes | `9f1af555c54beee60` | `nuit-codex-prologue/native-2f2b48-tail-pointer-8bed-020b639a/trial-tail-pointer` |
| `0x002F2B48` | `FUN_002F2B48` | 136 | `tail-pointer-020b639a:table-placement` | `8bed6eae` | MISMATCH (generated-table placement) | 128 produced / 136 target bytes; generated table differs | `9f1af555c54beee60` | `nuit-codex-prologue/native-2f2b48-tail-pointer-8bed-020b639a/trial-table-placement` |

Inventory after these appended measurements: 80 native records across the same 12 functions, comprising 7 exact results, 69 mismatches, 2 archived runs without proof, 1 compiler failure and 1 source rejection. The original inventory above describes its historical checkpoint.

## Boot zero-TI loop requalification

The preserved volatile zero-store loop was rejected at 44/40 bytes under dff08a34. Recompiling the identical source under the current profile still produces 44 bytes. This tests the architectural-zero store correction on a volatile loop rather than assuming the scalar zero-store result generalizes. The complete source SHA-256 is unchanged: `c94397760c68bc0484540232954e915c5058381292dc827c0b5abe67809fc0c2`.

| Target | Program | Trial record | C profile | Result | Produced / target bytes | Source SHA prefix | Evidence directory |
|---|---|---|---|---|---|---|---|
| `FUN_00282A88` | boot | `ti-loop-a2f8e58d:FUN_00282A88` | `8bed6eae` | MISMATCH | 44 / 40 | `c94397760c68bc04` | `nuit-codex-prologue/boot-ti-loop-8bed-020b639a/run-ti-loop-a2f8e58d` |

## Frame-bearing native requalification — current 8bed profile

The preserved `002E7A08` source contains a 48-byte frame and a call, so the current frame-scheduler default is relevant to its qualification. Its source SHA-256 remains `b84f46ffe3f8a5feb1cb0b7fc661f9f4a1d485a114da07a660948a1edd8f1e65`. The current-profile measurement retains the prior 17-byte difference at the complete 156-byte size; no source or compiler change was made to force a result.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002E7A08` | `FUN_002E7A08` | 156 | `frame-020b639a:preserved-source-current-profile` | `8bed6eae` | MISMATCH | 17 / 156 bytes | `b84f46ffe3f8a5fe` | `nuit-codex-prologue/native-2e7a08-frame-8bed-020b639a/trial-preserved-source-current-profile` |

Inventory after this measurement: 81 native records, including 70 mismatches; other categories and the 12-function scope are unchanged. Nine boot records and two separate PCSX2 observations are also indexed.

## Selective store-order experiment — current 8bed profile

The best archived settings source matched the first eleven retail instructions but scheduled the final five stores differently. This new hypothesis makes only y/value/mode/flags stores volatile and leaves x ordinary, testing whether the final x store can occupy the return delay slot. The complete function still produces 72 bytes against the pinned 68-byte body. No source is integrated.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002D2348` | `FUN_002D2348` | 68 | `settings-selective-stores-20261003` | `8bed6eae` | MISMATCH | 72 produced / 68 target bytes | `ac57528239a266d3` | `nuit-codex-prologue/native-2d2348-selective-stores-8bed-20261003/trial-selective-stores` |

Inventory after this measurement: 82 native records, including 71 mismatches; the other categories and the 12-function scope are unchanged.


## New small native family — live Ghidra inventory

The bridge was reconnected to the measured RAC2 project before selecting new targets. Every complete instruction span was compared against its pinned overlay bytes. Function extents, rather than the next boundary including padding, define the sizes below. Four float-pair setters, a byte fallback selector, a nullable short comparison and a pair clearer were authored independently in ordinary C. The selector is exact but does not count until integrated through the full overlay gate. Other results remain negative.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002AA1A8` | `FUN_002AA1A8` | 20 | `small-family-20261003:002AA1A8` | `8bed6eae` | MISMATCH | 4 / 20 bytes | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |
| `0x002AA1C0` | `FUN_002AA1C0` | 20 | `small-family-20261003:002AA1C0` | `8bed6eae` | MISMATCH | 4 / 20 bytes | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |
| `0x002AA1D8` | `FUN_002AA1D8` | 20 | `small-family-20261003:002AA1D8` | `8bed6eae` | MISMATCH | 4 / 20 bytes | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |
| `0x002AA1F0` | `FUN_002AA1F0` | 20 | `small-family-20261003:002AA1F0` | `8bed6eae` | MISMATCH | 4 / 20 bytes | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |
| `0x002AD0D0` | `FUN_002AD0D0` | 20 | `small-family-20261003:002AD0D0` | `8bed6eae` | EXACT; integration pending | 0 | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |
| `0x002B2348` | `FUN_002B2348` | 32 | `small-family-20261003:002B2348` | `8bed6eae` | MISMATCH | 28 produced / 32 target bytes | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |
| `0x002B2AD8` | `FUN_002B2AD8` | 20 | `small-family-20261003:002B2AD8` | `8bed6eae` | MISMATCH | 2 / 20 bytes | `d53c197aa5d5e6de` | `nuit-codex-prologue/native-small-lot-8bed-20261003/trial-small-native-family` |

Inventory after these seven measurements: 89 native records across 19 functions, comprising 8 exact results, 77 mismatches, 2 archived runs without proof, 1 compiler failure and 1 source rejection. The source SHA-256 and all four instrument hashes are retained in the complete private proof.


## Next native family — new pinned complete bodies

The live assembly spans and bytes were independently checked against the pinned overlay before writing this ten-function C lot. Six complete bodies match: three call wrappers, one scalar getter, one indexed byte test and one object-field setter. The four failed sources are retained. Exact trial rows are not integration credit.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002D1060` | `FUN_002D1060` | 20 | `next-family-20261003:002D1060` | `8bed6eae` | MISMATCH | 2 / 20 bytes | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D15B8` | `FUN_002D15B8` | 24 | `next-family-20261003:002D15B8` | `8bed6eae` | MISMATCH | 9 / 24 bytes | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D15D0` | `FUN_002D15D0` | 24 | `next-family-20261003:002D15D0` | `8bed6eae` | MISMATCH | 17 / 24 bytes | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D2878` | `FUN_002D2878` | 28 | `next-family-20261003:002D2878` | `8bed6eae` | EXACT; integration pending | 0 | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D57C8` | `FUN_002D57C8` | 20 | `next-family-20261003:002D57C8` | `8bed6eae` | MISMATCH | 2 / 20 bytes | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D5820` | `FUN_002D5820` | 28 | `next-family-20261003:002D5820` | `8bed6eae` | EXACT; integration pending | 0 | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D5840` | `FUN_002D5840` | 28 | `next-family-20261003:002D5840` | `8bed6eae` | EXACT; integration pending | 0 | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D59A0` | `FUN_002D59A0` | 12 | `next-family-20261003:002D59A0` | `8bed6eae` | EXACT; integration pending | 0 | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D5A30` | `FUN_002D5A30` | 28 | `next-family-20261003:002D5A30` | `8bed6eae` | EXACT; integration pending | 0 | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |
| `0x002D6CF8` | `FUN_002D6CF8` | 32 | `next-family-20261003:002D6CF8` | `8bed6eae` | EXACT; integration pending | 0 | `8f569df16983f20e` | `nuit-codex-prologue/native-next-lot-8bed-20261003/trial-next-native-family` |

Inventory after this lot: 99 native records across 29 functions, comprising 14 exact results, 81 mismatches, 2 archived runs without proof, 1 compiler failure and 1 source rejection.


## Eight-body source-unit compile failure

The first full-context source for these eight new targets redeclared s64 and u64 already present in the accepted Ship Shack source. This compiler rejects those duplicate typedefs before producing an object. All eight attempted targets are indexed below as failures of the shared source unit; no individual body size or byte result is inferred. The complete source and compile log are preserved, and the corrected unit must use a separate run.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002E2A88` | `FUN_002E2A88` | 36 | `eight-new-20261003:duplicate-types:002E2A88` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002E48B0` | `FUN_002E48B0` | 28 | `eight-new-20261003:duplicate-types:002E48B0` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002E53E0` | `FUN_002E53E0` | 28 | `eight-new-20261003:duplicate-types:002E53E0` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002EBB68` | `FUN_002EBB68` | 36 | `eight-new-20261003:duplicate-types:002EBB68` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002EF578` | `FUN_002EF578` | 12 | `eight-new-20261003:duplicate-types:002EF578` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002EF588` | `FUN_002EF588` | 12 | `eight-new-20261003:duplicate-types:002EF588` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002EF5E8` | `FUN_002EF5E8` | 12 | `eight-new-20261003:duplicate-types:002EF5E8` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |
| `0x002F1B80` | `FUN_002F1B80` | 20 | `eight-new-20261003:duplicate-types:002F1B80` | `8bed6eae` (compiler invoked; no object) | COMPILE FAIL (shared source unit) | duplicate existing s64/u64 typedefs; no body measurement | `697f851dc4126cf5` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003/trial-eight-new-bodies` |

Inventory: 107 target records across 37 functions, comprising 14 exact results, 81 mismatches, 2 archived runs without proof, 9 compiler failures (including eight targets in this shared failed unit), and 1 source rejection.


## Eight new native bodies — existing types reused

Removing the duplicate typedef declarations creates a separate full-context source run. All eight complete bodies and all thirteen prior native controls match exactly under the unchanged qualified tools. The float witness retains the measured product association and no fused operation. These results remain unintegrated until the affected full overlay gates pass.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002E2A88` | `FUN_002E2A88` | 36 | `eight-new-20261003:reuse-types:002E2A88` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002E48B0` | `FUN_002E48B0` | 28 | `eight-new-20261003:reuse-types:002E48B0` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002E53E0` | `FUN_002E53E0` | 28 | `eight-new-20261003:reuse-types:002E53E0` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002EBB68` | `FUN_002EBB68` | 36 | `eight-new-20261003:reuse-types:002EBB68` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002EF578` | `FUN_002EF578` | 12 | `eight-new-20261003:reuse-types:002EF578` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002EF588` | `FUN_002EF588` | 12 | `eight-new-20261003:reuse-types:002EF588` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002EF5E8` | `FUN_002EF5E8` | 12 | `eight-new-20261003:reuse-types:002EF5E8` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |
| `0x002F1B80` | `FUN_002F1B80` | 20 | `eight-new-20261003:reuse-types:002F1B80` | `8bed6eae` | EXACT; integration pending | 0 | `344c526d2936c8f0` | `nuit-codex-prologue/native-eight-lot26-8bed-20261003-v2/trial-eight-new-bodies` |

Inventory: 115 target records across 37 functions, comprising 22 exact results, 81 mismatches, 2 archived runs without proof, 9 compiler failures and 1 source rejection. Eight compiler-failure records refer to one shared source-unit failure; they do not establish individual body mismatches.


## Broader source-unit catalog rejection

The first catalog for the eight new targets included the byte address 0x0018b2bd as an external symbol. The native checker requires aligned external anchors and rejected the unit before compilation. All eight attempted target records describe this shared catalog rejection; no compiler or individual body result is inferred. The original source, catalog and preflight error remain private. A corrected source can address the measured byte as offset one from aligned anchor 0x0018b2bc without relaxing the guard.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002A40E8` | `FUN_002A40E8` | 96 | `broader-20261003:unaligned-anchor:002A40E8` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002A5288` | `FUN_002A5288` | 60 | `broader-20261003:unaligned-anchor:002A5288` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002AA4E0` | `FUN_002AA4E0` | 60 | `broader-20261003:unaligned-anchor:002AA4E0` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002ACF68` | `FUN_002ACF68` | 52 | `broader-20261003:unaligned-anchor:002ACF68` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002ACFA0` | `FUN_002ACFA0` | 52 | `broader-20261003:unaligned-anchor:002ACFA0` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002AD080` | `FUN_002AD080` | 80 | `broader-20261003:unaligned-anchor:002AD080` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002ADBC0` | `FUN_002ADBC0` | 88 | `broader-20261003:unaligned-anchor:002ADBC0` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |
| `0x002AECC0` | `FUN_002AECC0` | 44 | `broader-20261003:unaligned-anchor:002AECC0` | not invoked | REJECTED (shared catalog) | byte external is unaligned; no body measurement | `bb84263567c2b91e` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003` |

Inventory: 123 target records across 45 functions, with 22 exact results, 81 mismatches, 2 archived runs without proof, 9 compiler failures and 9 source/catalog rejections. Eight new rejection rows concern this one uncompiled catalog.


## Broader unit link refusal and isolated complete-body measurements

The aligned-anchor source compiled, but one 88-byte candidate for the 80-byte 002AD080 slot overlapped its next function and blocked the full-unit linker. The source and failed link remain unchanged. Each new complete body was then linked separately from that same compiled object, without trimming, patching or recompiling its code. Five bodies are exact and three remain mismatches. Only new full-unit qualification after excluding failed bodies can authorize integration.

### Shared link-failure records

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002A40E8` | `FUN_002A40E8` | 96 | `broader-20261003:union-link:002A40E8` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002A5288` | `FUN_002A5288` | 60 | `broader-20261003:union-link:002A5288` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002AA4E0` | `FUN_002AA4E0` | 60 | `broader-20261003:union-link:002AA4E0` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002ACF68` | `FUN_002ACF68` | 52 | `broader-20261003:union-link:002ACF68` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002ACFA0` | `FUN_002ACFA0` | 52 | `broader-20261003:union-link:002ACFA0` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002AD080` | `FUN_002AD080` | 80 | `broader-20261003:union-link:002AD080` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002ADBC0` | `FUN_002ADBC0` | 88 | `broader-20261003:union-link:002ADBC0` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |
| `0x002AECC0` | `FUN_002AECC0` | 44 | `broader-20261003:union-link:002AECC0` | `8bed6eae` | LINK FAIL (shared unit) | 002AD080 section extends into next function; no union body measurement | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/trial-broader-native-family` |

### Isolated measurements

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002A40E8` | `FUN_002A40E8` | 96 | `broader-20261003:isolated:002A40E8` | `8bed6eae` | MISMATCH | 9 / 96 bytes | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002A40E8` |
| `0x002A5288` | `FUN_002A5288` | 60 | `broader-20261003:isolated:002A5288` | `8bed6eae` | EXACT; integration pending | 0 | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002A5288` |
| `0x002AA4E0` | `FUN_002AA4E0` | 60 | `broader-20261003:isolated:002AA4E0` | `8bed6eae` | MISMATCH | 13 / 60 bytes | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002AA4E0` |
| `0x002ACF68` | `FUN_002ACF68` | 52 | `broader-20261003:isolated:002ACF68` | `8bed6eae` | EXACT; integration pending | 0 | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002ACF68` |
| `0x002ACFA0` | `FUN_002ACFA0` | 52 | `broader-20261003:isolated:002ACFA0` | `8bed6eae` | EXACT; integration pending | 0 | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002ACFA0` |
| `0x002AD080` | `FUN_002AD080` | 80 | `broader-20261003:isolated:002AD080` | `8bed6eae` | MISMATCH | 88 produced / 80 target bytes | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002AD080` |
| `0x002ADBC0` | `FUN_002ADBC0` | 88 | `broader-20261003:isolated:002ADBC0` | `8bed6eae` | EXACT; integration pending | 0 | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002ADBC0` |
| `0x002AECC0` | `FUN_002AECC0` | 44 | `broader-20261003:isolated:002AECC0` | `8bed6eae` | EXACT; integration pending | 0 | `c9bc32200b3a6e0a` | `nuit-codex-prologue/native-broader-lot27-8bed-20261003-v2/isolated-bodies-gp0/002AECC0` |

Inventory: 139 target records across 45 functions, comprising 27 exact results, 84 mismatches, 2 archived runs without proof, 9 compiler failures, 8 link failures and 9 source/catalog rejections. Shared-unit failure rows do not assert individual body mismatches.

## Five winners requalified in a complete source unit

The three mismatches are excluded from this fresh source. All five new complete
bodies and the 21 existing native controls now pass together: 26 functions and
1,056 native bytes. This qualifies the authored unit without the previous
overlap; full overlay integration remains required before credit.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002A5288` | `FUN_002A5288` | 60 | `lot27-five-full:002A5288` | `8bed6eae` | EXACT; integration pending | 0 | `85c805d4a898eb9a` | `nuit-codex-prologue/native-five-lot27-full-8bed-20261003/trial-five-winners-full-unit` |
| `0x002ACF68` | `FUN_002ACF68` | 52 | `lot27-five-full:002ACF68` | `8bed6eae` | EXACT; integration pending | 0 | `85c805d4a898eb9a` | `nuit-codex-prologue/native-five-lot27-full-8bed-20261003/trial-five-winners-full-unit` |
| `0x002ACFA0` | `FUN_002ACFA0` | 52 | `lot27-five-full:002ACFA0` | `8bed6eae` | EXACT; integration pending | 0 | `85c805d4a898eb9a` | `nuit-codex-prologue/native-five-lot27-full-8bed-20261003/trial-five-winners-full-unit` |
| `0x002ADBC0` | `FUN_002ADBC0` | 88 | `lot27-five-full:002ADBC0` | `8bed6eae` | EXACT; integration pending | 0 | `85c805d4a898eb9a` | `nuit-codex-prologue/native-five-lot27-full-8bed-20261003/trial-five-winners-full-unit` |
| `0x002AECC0` | `FUN_002AECC0` | 44 | `lot27-five-full:002AECC0` | `8bed6eae` | EXACT; integration pending | 0 | `85c805d4a898eb9a` | `nuit-codex-prologue/native-five-lot27-full-8bed-20261003/trial-five-winners-full-unit` |

Inventory after full-unit requalification: 144 target records / 45 functions,
including 32 exact results; all failure categories retain the counts above.

## Selective first-store scheduling hypothesis

Six archived pair-store bodies reverse the two stores under ordinary C. This
new hypothesis qualifies only the first observed write as volatile and leaves
the final write ordinary, testing whether the latter can fill the return delay
slot. All six produce 24 bytes against complete 20-byte functions. The 26
existing native controls stay exact. No source is integrated and no further
qualifier permutation is justified by this result alone.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002AA1A8` | `FUN_002AA1A8` | 20 | `pair-first-volatile:002AA1A8` | `8bed6eae` | MISMATCH | 24 produced / 20 target bytes | `23b9fbafed6c0588` | `nuit-codex-prologue/native-selective-pair-stores-8bed-20261003/trial-selective-first-store` |
| `0x002AA1C0` | `FUN_002AA1C0` | 20 | `pair-first-volatile:002AA1C0` | `8bed6eae` | MISMATCH | 24 produced / 20 target bytes | `23b9fbafed6c0588` | `nuit-codex-prologue/native-selective-pair-stores-8bed-20261003/trial-selective-first-store` |
| `0x002AA1D8` | `FUN_002AA1D8` | 20 | `pair-first-volatile:002AA1D8` | `8bed6eae` | MISMATCH | 24 produced / 20 target bytes | `23b9fbafed6c0588` | `nuit-codex-prologue/native-selective-pair-stores-8bed-20261003/trial-selective-first-store` |
| `0x002AA1F0` | `FUN_002AA1F0` | 20 | `pair-first-volatile:002AA1F0` | `8bed6eae` | MISMATCH | 24 produced / 20 target bytes | `23b9fbafed6c0588` | `nuit-codex-prologue/native-selective-pair-stores-8bed-20261003/trial-selective-first-store` |
| `0x002B2AD8` | `FUN_002B2AD8` | 20 | `pair-first-volatile:002B2AD8` | `8bed6eae` | MISMATCH | 24 produced / 20 target bytes | `23b9fbafed6c0588` | `nuit-codex-prologue/native-selective-pair-stores-8bed-20261003/trial-selective-first-store` |
| `0x002D1060` | `FUN_002D1060` | 20 | `pair-first-volatile:002D1060` | `8bed6eae` | MISMATCH | 24 produced / 20 target bytes | `23b9fbafed6c0588` | `nuit-codex-prologue/native-selective-pair-stores-8bed-20261003/trial-selective-first-store` |

Inventory: 150 records /45 functions; 90 mismatches. Other categories unchanged.

## Pair-store caller ABI audit

A separate read-only audit located all 49 direct calls to the six pair-store
targets in the pinned Ship Shack executable section and checked their bodies
against Ghidra memory. No caller consumes the materialized base address in v0
before a later write or an independently checked callee replaces it. A few
paths propagate the residue through an epilogue, but their caller also leaves
it unused. No address-word table reference to these targets was found in the
loaded segments.

| Target | Direct calls | Return-value evidence |
|---|---:|---|
| `002AA1A8` | 7 | no use before overwrite |
| `002AA1C0` | 14 | no use before overwrite |
| `002AA1D8` | 11 | no use before overwrite |
| `002AA1F0` | 14 | no use before overwrite |
| `002B2AD8` | 1 | no use before overwrite |
| `002D1060` | 2 | no use before overwrite |

This does not prove the original source return type absolutely, but supplies
no justification for changing it to a pointer to force store scheduling.
Keep these six targets parked pending new evidence. This ABI observation is
not a compiler trial and does not change the inventory totals above.

## Eight larger native hypotheses, with measured callee return corrected

The pinned callee 002D5860 returns a 0/1 word. This private full-context source
corrects its declaration to s32 rather than inventing a result or using a
conflicting declaration. All 26 existing controls remain exact. The new
40-record loop is exact at 76 bytes; the seven other hypotheses remain
mismatches. Their original sources, assembly and complete results are retained.

| Target | Function | Bytes | Trial | C profile | Outcome | Diff | Source SHA-256 prefix | Evidence directory |
|---|---|---:|---|---|---|---|---|---|
| `0x002A3E18` | `FUN_002A3E18` | 152 | `next960:002A3E18` | `8bed6eae` | MISMATCH | 98 / 152 bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002A3F48` | `FUN_002A3F48` | 164 | `next960:002A3F48` | `8bed6eae` | MISMATCH | 160 produced / 164 target bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002A4468` | `FUN_002A4468` | 76 | `next960:002A4468` | `8bed6eae` | EXACT; integration pending | 0 | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002A6100` | `FUN_002A6100` | 108 | `next960:002A6100` | `8bed6eae` | MISMATCH | 92 produced / 108 target bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002A6170` | `FUN_002A6170` | 124 | `next960:002A6170` | `8bed6eae` | MISMATCH | 39 / 124 bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002A67B8` | `FUN_002A67B8` | 140 | `next960:002A67B8` | `8bed6eae` | MISMATCH | 36 / 140 bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002AD170` | `FUN_002AD170` | 88 | `next960:002AD170` | `8bed6eae` | MISMATCH | 25 / 88 bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |
| `0x002B1D58` | `FUN_002B1D58` | 108 | `next960:002B1D58` | `8bed6eae` | MISMATCH | 14 / 108 bytes | `b2a5db0c634b9b61` | `nuit-codex-prologue/native-next960-8bed-20261003/trial-eight-larger-bodies` |

Inventory: 158 target records /53 functions, including 33 exact results and
97 mismatches. Other categories retain the counts above. A private source
prototype correction does not change public source or integration credit.
