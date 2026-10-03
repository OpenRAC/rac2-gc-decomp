# The retail compiler, identified and reproduced

**Measured 2026-10-03 against the pinned retail boot** (`SCUS_972.68`, USA v1.01,
sha256 `36d5814d…`, `config/target.json`).

The integrated C bodies are reproduced byte-for-byte by **GNU-EE 2.9-ee-991111b**
— the same compiler lineage as the first Ratchet game — plus a cumulative
retail-behaviour patch stack. The previous profile (the SN ProDG 3.01 kit,
`ee-gcc2953`/2.95.3) reproduced the simple leaf bodies but could not reproduce
three families, which is what drove the move:

| Family | Retail | SN 2.95.3 |
|---|---|---|
| callee-saved register saves | `sd` in 8-byte slots | `sq` in 16-byte slots |
| calls at the end of a function | plain `jal` + full epilogue | sibling call (`j target`) |
| GP→FP transfers (`mtc1`) | a `nop` in *some* cases only | n/a (different assembler) |

## The evidence

Seven campaign bodies were compiled, assembled and linked with the reconstructed
chain, then compared to the retail bytes: `FUN_002889B8`, `FUN_002A77E0`,
`FUN_002A7820`, `FUN_002A7940`, `FUN_003512B8`, `FUN_00351268` (the six bodies of
the call-bearing family) and `FUN_00300540` (an aiguillage) — **all byte-exact**.

The stronger check: the **96 bodies already integrated under the previous
profile were re-verified under the new chain — 96/96 still byte-exact**. The
profile switch therefore costs nothing that was already proven, and the same
reconstruction reproduces the save-bearing family.

## Why the two profiles agree on the simple bodies

The 96 leaf bodies contain no saves, no calls and no `lq`/`sq`; on that subset the
2.9 and 2.95 code generators emit the same instructions. The divergence appears
precisely in the prologue/epilogue and in block moves — which is why the
campaign's first 96 matches never exercised it.

## The compiler-side rules that had to be measured

Four changes separate the released 2.9 sources from the retail compiler; each was
found from a witness pair in the retail image, then validated on the full corpus:

1. **Save width.** `prologue/epilogue` save GPRs with `sd` in 8-byte slots. The
   released source widens register 0 to `TImode`, which makes every slot 16
   bytes; the retail build does not (`FUN_002889B8`: `sd $s0,0($sp); sd $ra,8($sp)`,
   frame 16).
2. **Save order.** Saves and restores are emitted in ascending register order
   (`s0, s1, …, ra`); the released `save_restore_insns` loop descends from `$ra`.
   The frame layout is identical either way.
3. **No sibling calls, by default.** The Cygnus 2.9 sibcall pass, absent from the
   SN compiler, must stay inert (`FUN_003512B8` ends in `jal 0x11ac40` plus a
   full restore, not in a tail jump).
4. **The `mtc1` hazard nop.** Measured on the whole boot (`veille/mesure-regle-mtc1.py`,
   598 transfers whose next instruction reads the written FPR): a `nop` is present
   in **587** of them, at **every** distance of the source GPR (1, 2, 3, 4, 6 … 86)
   and even when that GPR is never written in the function (20 cases). The 1999
   source says the same (`gas/config/tc-mips.c`, `INSN_WRITE_FPR_T` branch: nop as
   soon as the next instruction uses the FPR). The rule is therefore the plain
   "next instruction reads the written FPR" test, **with one measured exception**:
   the 11 cases without a `nop` sit in 5 functions
   (`0x00283ce0`, `0x00283c28`, `0x002a677c`, `0x002e0408`, `0x002e0558`), and in
   `FUN_00283CE0` the `mtc1` is the **first instruction of the function**.
   An earlier attempt narrowed the rule to "the GPR was written two instructions
   back" (two data points) — it refused the `nop` in `FUN_002A7878`, where the
   retail has one.

Two source-level lessons the witnesses also pinned down:

- A 16-byte copy must go through a 128-bit integer type
  (`__attribute__((mode(TI)))`) to reach `lq`/`sq`; the aggregate path builds
  `ld`/`sd` pairs or a `memcpy` call (`FUN_002A8C00`).
- The callee's prototype decides `$v0` vs `$v1` for a rematerialised constant
  after a call: a value-returning prototype keeps `$v0` busy (`FUN_002889B8`).

## Reproducing the chain

Source: `gnu-ee-binutils-gcc-1.1.tar.gz`, sha256
`1f518043e252d6eda726386971d52eda26541ab936ea73a9783d73712b595f92` (the ps2dev
archive of the Sony/Cygnus EE compiler sources, target `mips64r5900-sf-elf`).

Patch stack: the cumulative `sce-991111b` stack published with the **Lombyte**
project (github.com/mateuszklysz/Lombyte, `patches/sce-991111b/`), minus its
`saves` widening, plus the four rules above. The build host is WSL with 32-bit
support (`gcc -m32`) and bison 1.28; the compiler and assembler are hashed in the
proofs:

| Tool | sha256 |
|---|---|
| `cc1` | `3e7628b7eb97e4f20d5f1e336c0b9ce51b939dea0a0d362815b6b5586214d86a` |
| `cpp` | `2ac3d8d3ca177e6705ac2cbdd1bd9e9a7181ac3e40f6230dea6875c3218ec155` |
| `as` | `20c5f50b02abbd86bf55213249995b23476d61ceec1d5eacce886bed43109dc7` |

(An earlier `as`, `87a1a012…`, carried the two-point `mtc1` rule described above
and has been superseded. `cc1` and `cpp` are unchanged: only the assembler moved.)

The linker stays the SDK `ld.exe` used before. The pipeline drives the chain
through `scripts/wsl_chain.py` (the 1999 tools are 32-bit Linux binaries: they
run under WSL, and every source is compiled under its bare name inside the WSL
filesystem, so the object is reproducible from any checkout).

## Scope

This document claims what was measured: the named bodies and the 96 previously
integrated bodies are reproduced byte-for-byte; the profile is not offered as a
general RAC2 compiler qualification. Bodies that exercise VU/MMI instructions or
`$gp` are outside it. The four rules above are the ones the corpus could
falsify; a future body that disagrees with them is a measurement, not a surprise.
