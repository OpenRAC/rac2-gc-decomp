# Native C Experiment Register — Ship Shack

This is the canonical index for native-C reconstruction experiments against `levels/24_ship_shack`. Check it before proposing or running a new source variant. Add one row immediately after each trial; never overwrite a prior row. The run directory is the evidence package and retains the exact source, catalog, object, assembly, logs, and qualification result. Ghidra remains the place for function analysis and annotations; this register records compiler experiments and their outcomes. The repository tracks this index; generated game objects, binaries, and runtime artifacts remain local.

## Current scope and interpretation

- Reference: `SCUS_972.68`, `/levels/24_ship_shack.elf`, pinned ELF SHA-256 `4afbc22add84109c84ef8ca49099fdf9863905e5f497369c207ac5cb47814740`.
- This register inventories 70 distinct archived trials across 12 functions: 7 exact results, 58 byte mismatches, 2 archived runs without proof, 1 compiler failure, 1 compiled candidate not byte-compared, and 1 source rejected before compilation.
- The 51 proof-bearing trials from the original candidate bank used the historical `dff08a34` C compiler profile with `-O2 -G0 -ffunction-sections`; seven later measured `FUN_002B0408` variants, five measured `FUN_002F2718` trials, and the two latest exact promotions used the current `8bed6eae` profile with the same flags. Two earlier 8bed-labelled folders do not retain qualification proofs and are explicitly marked unverified; the register-binding source was separately rejected before compilation. Other pinned tool hashes for proof-bearing trials are recorded in their local evidence. Historical exact results are not current-profile qualification; requalify a candidate with the current checker before treating an old exact result as current evidence.
- The five historical exact targets are now present in the public level-native catalog at the repository revision where this register was created. The table records the trial result and its original profile, not a substitute for current proofs.
- Classification uses `qualification_passed` and `proof.functions[].matched`; `proof.state` alone is not decisive. Aggregate `*-results.json` files duplicate these per-run records. Deduplicate by run directory / `work` path.
- Archived evidence remains in the private GAMING work area. Each evidence path is relative to the private `D:\RAC2\work` root. Candidate-bank runs retain `result.json` and `catalog.json`; source and compiler artifacts are present where the run reached that stage. Proof-bearing runs also contain object qualification data. The two original no-proof runs preserve their compiler/chain errors in `result.json` and `compile.log`.

## Archived runs

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
| `0x002B0408` | `FUN_002B0408` | 72 | `native-2b0408-register-values-run8bed` | `8bed path label; unverified` | COMPILED; NOT COMPARED | not byte-compared | `912832837f600f8e` | `nuit-codex-prologue/native-2b0408-register-values-run8bed` |
| `0x002D1570` | `FUN_002D1570` | 72 | `native-24-002d1570-8bed/native-candidates/9a830e6b` | `8bed6eae` | EXACT; publicly integrated | 0 | `da33359d7ab9dfe7` | `native-24-002d1570-8bed/native-candidates/9a830e6b` |
| `0x0035E348` | `FUN_0035E348` | 72 | `native-24-0035e348-8bed/native-candidates/ae6987b9` | `8bed6eae` | EXACT; publicly integrated | 0 | `e1cb74637497fd03` | `native-24-0035e348-8bed/native-candidates/ae6987b9` |

| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-5163ea2/trial-success-first-global-order` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `28244d9ce6f4c990` | `nuit-codex-prologue/native-2f2718-root8bed-5163ea2/trial-success-first-global-order` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-9a6b06b/trial-failure-first-global-order` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `fa123e6217db4b82` | `nuit-codex-prologue/native-2f2718-root8bed-9a6b06b/trial-failure-first-global-order` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-47de0aa/trial-unlikely-error-hint` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `47596ce3a2d58d5e` | `nuit-codex-prologue/native-2f2718-root8bed-47de0aa/trial-unlikely-error-hint` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-44663e4/trial-output-pointer-a1` | `unverified` | REJECTED (not genuine C) | checker rejected before compile | `c9e8bed6b418154b` | `nuit-codex-prologue/native-2f2718-root8bed-44663e4` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-1567a44/trial-output-zero-before-guard` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `91522fb7d31af33c` | `nuit-codex-prologue/native-2f2718-root8bed-1567a44/trial-output-zero-before-guard` |
| `0x002F2718` | `FUN_002F2718` | 64 | `native-2f2718-root8bed-d2e7070/trial-output-zero-expect-true` | `8bed6eae` | MISMATCH | 68 produced / 64 target bytes | `8499d0e74683785c` | `nuit-codex-prologue/native-2f2718-root8bed-d2e7070/trial-output-zero-expect-true` |

## New trial entry format

Append a row after every compiler experiment, including a failed compile or a no-match. Keep its complete run directory. Record the address, function size, immutable trial ID, compiler profile, qualification result, byte difference or size, source hash, and evidence path. If the source or compiler profile changes, it is a new trial. If evidence is unavailable, write `unknown` rather than inferring a value. Update the inventory totals and exact-current-profile status only from checker output.
