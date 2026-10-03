# Eighth RAC2 C lot - the save family, and the compiler that produces it

The seventh lot opened the call family with one body that passed because it had no
callee-saved registers to spill. This lot is what happened when the rest of the family met
the wall that had been invisible: **the retail build was not compiled by the SN 2.95.3 kit
this repository had been using**, and its prologues say so.

| | Before | After |
| --- | ---: | ---: |
| Boot | 96 functions / 2 120 bytes | **103 / 2 644** |
| Levels | 2 237 placements / 51 560 bytes | **2 426 / 65 708** |
| Total | 53 680 bytes (0.1100 %) | **68 352 bytes (0.1401 %)** |

Seven bodies, all byte-exact: six of the call family (`FUN_002889B8`, `FUN_002A77E0`,
`FUN_002A7820`, `FUN_002A7940`, `FUN_003512B8`, `FUN_00351268`) and one aiguillage
(`FUN_00300540`). Each was first gated alone, then the whole repository was re-verified
under the reconstructed chain: **96/96 previously integrated bodies still match**, so the
profile move costs nothing already proven.

## What the wall was

Four of the seven bodies differed from the retail **only in the prologue and epilogue**:
the retail saves callee-saved registers with `sd` in 8-byte slots
(`sd $s0,0($sp); sd $ra,8($sp)`, frame 16), while the 2.95.3 kit emits `sq` in 16-byte
slots. `FUN_00351268` was otherwise identical to the byte. `FUN_003512B8` additionally
ended in a sibling call where the retail makes a plain `jal`, and `FUN_002A7940` was
missing a `nop` after `mtc1` that the kit never emits.

The full identification, the four compiler-side rules it needed, and the tool hashes are
in [`COMPILER-NOTES.md`](COMPILER-NOTES.md). The short version: the retail compiler is
the GNU-EE 2.9-ee-991111b lineage, and this lot moved the candidate gate and the level
integration onto it through `scripts/wsl_chain.py`.

## The two source-level lessons

- `FUN_002889B8`: the callee's prototype decides `$v0` vs `$v1` for the constant
  rematerialised after the call. Declaring the callee `void` puts it in `$v0`; the retail
  form needs the value-returning prototype (`extern s32`), the result ignored.
- `FUN_002A8C00` (an earlier body, reworked here): a 16-byte copy reaches `lq`/`sq` only
  through a 128-bit integer type (`__attribute__((mode(TI)))`); the aggregate path builds
  `ld`/`sd` pairs or calls `memcpy`.

## The trap the level chain had been hiding

`gen-level-catalog-all.py` reads `work/externes-niveaux.json`, a table of callee → address
per level that had been a manual copy since the seventh lot. The four new callees were
absent from it, so every level's qualification link failed on `undefined reference` even
though the placements were correct — the failure looked like a placement bug and was not.
The table is now generated: `work/maj-externes-niveaux.py` extracts it from the fresh
`appels-niveaux.json` (243 level/symbol couples), so it cannot go stale again.

## Files

`candidates/boot.c` and `config/candidate-catalog.json` carry the seven bodies and four
externals; `scripts/check_candidates.py` and `scripts/integration.py` compile through
`scripts/wsl_chain.py`; `docs/COMPILER-NOTES.md` documents the chain;
`config/level-catalog.json` and the proofs under `progress/` are regenerated from one
pass, as always.
