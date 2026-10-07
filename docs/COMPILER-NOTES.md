# Reconstructed GNU EE compiler: provenance and measured compatibility

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

## Instrument identity and provenance

Native programs may use the default `-O2 -G0 -ffunction-sections` or the narrowly
supported `-O2 -G8 -ffunction-sections` profile with the same pinned instruments.
The boot retains its default flags. Every native review binds its own catalog
flags, source, checker and object, and still requires complete byte equality.
Other optimization or small-data flag variants are rejected by the native loader.

The Barlow queue launcher provides the measured small-data case. Its reference
loads a four-byte mode word at `0x001A8F00` relative to pinned GP `0x001AEFF0`.
The reconstructed backend disables GP optimization and early known-size SDA
extern emission at `-G0`; positive `-G8` with optimization enables that path.
The maintained assembler uses its eight-byte default threshold. This changes
address generation, not the compiler binary or the source algorithm.

A separate four-byte control object at `0x0018C0B4` lies outside that GP's signed
16-bit reach and has a pinned absolute getter. An explicit `nosda` declaration
preserves this measured binding instead of allowing the positive-G size heuristic
to select an impossible relocation. All fifteen existing Barlow controls remain
exact at `-G0` with that binding. The same source at `-G8` matches all sixteen
complete functions, including the 200-byte launcher. Failed unbound qualification
and the default-profile launcher refusal remain in the campaign register.

This per-program qualification does not establish original SDK flags or make a
near-match acceptable. Fresh reviews and complete loaded-byte/metadata gates are
required before integration; GP, declarations and flags remain explicit proof inputs.

`8bed6eae` is the SHA-256 prefix of the locally rebuilt `cc1` binary, not a compiler name or version. The source lineage is GNU EE 2.9-ee-991111b. The local build recipe starts from `gnu-ee-binutils-gcc-1.1.tar.gz`, applies the RAC1/Lombyte `sce-991111b` patch stack, and makes the measured RAC2 adjustments described below. The profile was introduced for this repository in commit `b2b9101` after comparing the earlier SN ProDG 3.01 GCC 2.95.3 profile against call-bearing retail bodies.

Matching the qualified bodies establishes compatibility with those bodies. It does not establish the exact compiler binary, patch set, flags or source directives used by Insomniac for the original game. Future bodies can falsify this compatibility profile.

Authored C uses the reconstructed GNU `cpp`/`cc1`/`as` through `scripts/wsl_chain.py`. Linking still uses the SN SDK `ld.exe`. The reconstructed assembly path uses `Ps2EeAs.exe` separately. The prior SN compiler remains available locally; the SN toolchain directory passed to the current C checker supplies its linker, and does not mean that the checker invokes `ee-gcc2953`.

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

Five changes separate this reconstructed profile from the released 2.9 sources; each was
found from a witness pair in the retail image, then validated on the full corpus:

1. **Save width.** `prologue/epilogue` save GPRs with `sd` in 8-byte slots. The
   released source widens register 0 to `TImode`, which makes every slot 16
   bytes; the retail build does not (`FUN_002889B8`: `sd $s0,0($sp); sd $ra,8($sp)`,
   frame 16).
2. **Save order.** GPR saves and restores are emitted in ascending register order
   (`s0, s1, …, ra`); the released `save_restore_insns` loop descends from `$ra`.
   FPR saves retain their descending order and precede the GPR block. Restores
   retain GPR before FPR. The two emission blocks and all stack offsets are
   preserved, with their shared base initialized before the first block.
   The public [block-reordering patch](../scripts/compiler/reorder_save_blocks.py)
   applies after the ascending-GPR change. This ordering reproduces
   `FUN_002A7878` (92 bytes) and `FUN_002A78D8` (104 bytes); all 103 previously
   integrated bodies still match under the new compiler.
3. **No sibling calls, by default.** The Cygnus 2.9 sibcall pass, absent from the
   SN compiler, must stay inert (`FUN_003512B8` ends in `jal 0x11ac40` plus a
   full restore, not in a tail jump).
4. **The `mtc1` hazard nop.** The initial boot survey (`veille/mesure-regle-mtc1.py`,
   598 transfers whose next instruction reads the written FPR): a `nop` is present
   in **587** of them, at **every** distance of the source GPR (1, 2, 3, 4, 6 … 86)
   and even when that GPR is never written in the function (20 cases). The 1999
   source says the same (`gas/config/tc-mips.c`, `INSN_WRITE_FPR_T` branch: nop as
   soon as the next instruction uses the FPR). The rule is therefore the plain
   "next instruction reads the written FPR" test. In that original survey,
   the 11 cases without a `nop` sit in 5 functions
   (`0x00283ce0`, `0x00283c28`, `0x002a677c`, `0x002e0408`, `0x002e0558`), and in
   `FUN_00283CE0` the `mtc1` is the first instruction of the function. The
   earlier compatibility implementation exempted transfers after cleared
   assembler instruction history, which can also occur within a function;
   it is not a function-boundary detector. Eight retail first-transfer witnesses
   falsify that blanket exemption: four require a nop and four do not.
   The [restricted exemption transformer](../scripts/compiler/restrict_mtc1_exemption.py)
   retains only two measured producer/consumer patterns after cleared history:
   `a0` to `f0` followed by `cvt.s.w f0,f0`, and zero to `f0` followed by
   `c.lt.s f12,f0`. Other combinations retain the existing hazard logic.
   This is an empirical compatibility rule for the qualified C corpus,
   rather than recovery of the original compiler or ordering directives.
   An earlier attempt narrowed the rule to "the GPR was written two instructions
   back" (two data points) — it refused the `nop` in `FUN_002A7878`, where the
   retail has one.
5. **Short-loop padding after delay-slot scheduling.** The cumulative stack's
   patch 0054 disables the post-DBR hook because its original pipeline uses
   Ps2EeAs to pad short loops. This repository's C pipeline uses GNU `as`,
   which has no compensating loop-padding pass. Restore the existing
   `mips_r5900_pad_loops` hook after the other 0054 changes, using
   [`enable_loop_padding.py`](../scripts/compiler/enable_loop_padding.py).
   The two polling loops of `FUN_0034FB20` otherwise lack the padding needed
   to reach the measured minimum of seven instructions. With the hook and
   the correct local call view, the complete body matches at 168 bytes.
   All 114 previously accepted bodies are unchanged under this compiler.
   Count a `TRAP_IF` using its machine-description instruction length: the
   R5900 division guard expands to a branch and a break, rather than one
   encoded instruction. Counting it as one overpadded a seven-word loop in
   `FUN_0028B950`; the [counter patch](../scripts/compiler/count_trap_length.py)
   removes that extra padding while preserving all 143 earlier bodies.

The original 598-case survey covered one floating-point source field. A fresh
survey covering both source operands finds **915 immediate dependencies: 897
with a delay and 18 without**. It retains all 598 original cases. The ten cases
in `0x00283c28`, `0x002a677c`, `0x002e0408` and `0x002e0558` are reproduced by
explicit instruction-ordering control; their reordering-mode witnesses and two
positive controls retain the delay. The private diagnostic passed 52 fixtures.
This establishes a sufficient assembler mechanism without recovering the
original source directives. Synthetic alignment and branch witnesses trigger
the compatibility exemption inside a function. ISA selection supplies another
sufficient mechanism, so source provenance remains unresolved. This diagnostic
does not qualify every transfer in the game. The later restricted exemption
preserves all 165 accepted bodies and makes the two additional integer-to-float
bodies exact. With the independently authored RLE body, the combined gate is
168/168. All 52 original fixtures remain unchanged. Fourteen distance controls,
twelve boundary controls and eleven first-transfer controls bring the private
fixture suite to 89 cases. Synthetic controls have no retail oracle; the retail
first-transfer sample contains only four distinct patterns. A conflicting exact
body or a retail witness contradicting the restricted patterns would falsify
the compatibility claim. The four exception functions remain explained by a
sufficient mechanism; their original source directives remain unresolved.

6. **Architectural zero in TImode stores.** The recognition condition already
   admits zero, but the memory-store alternatives constrained the source to a
   register. The [zero-store patch](../scripts/compiler/allow_zero_ti_store.patch)
   admits constraint `J` and prints that source using `%z1`. It permits an
   actual zero constant reaching the store to select `sq` from architectural
   zero. It does not eliminate every materialised zero: five independent
   source witnesses, including a simple zero store and nonzero/copy cases,
   retain their previous output. The complete 168-byte `FUN_002E5FE0` becomes
   exact with a counted 52-entry loop, while the older compiler differs in
   six bytes on the same source and flags.
   A fresh current-profile qualification also reproduces the complete 8-byte
   `FUN_00282C88`: one 128-bit zero assignment. Its historical `dff08a34`
   baseline produced 12 bytes; the full boot source now reuses the existing
   `TI` typedef and passes 178/178 complete symbols. This additional source
   witness is recorded in the [experiment register](C-NATIVE-EXPERIMENT-REGISTER.md);
   it does not imply that every zero-store spelling selects the same form.
7. **Preserve the generic frame scheduler by default.** The cumulative P21
   option forces emission order between two frame-related instructions. The
   earlier RAC2 recipe enabled that option. The
   [default transformer](../scripts/compiler/disable_frame_order_default.py)
   restores the generic scheduler default; the explicit opt-in remains.
   `FUN_002B7170` requires `s2`, `s0`, `s1` saves rather than the forced order.
   Its source also needs the measured constant lifetimes and final store
   order. With that source, the previous profile differs only in the three
   prologue words; the new profile reproduces the complete 372-byte body.
   All 176 earlier qualified bodies remain exact, for a combined 177/177.
   This qualifies the current corpus, not every frame layout in the game.

Two complete builds reproduce release compiler `1ae7dceb` with the zero-store
change, and another two reproduce `8bed6eae` with the frame option default off.
`cpp` and GNU `as` retain their hashes. The complete release rebuilds exclude
all diagnostic buffer/ranking instrumentation. The intermediate object-rebuild
hashes remain private diagnostics rather than release identities.

Source-level lessons the witnesses also pinned down:

- A 16-byte copy must go through a 128-bit integer type
  (`__attribute__((mode(TI)))`) to reach `lq`/`sq`; the aggregate path builds
  `ld`/`sd` pairs or a `memcpy` call (`FUN_002A8C00`).
- The callee's prototype decides `$v0` vs `$v1` for a rematerialised constant
  after a call: a value-returning prototype keeps `$v0` busy (`FUN_002889B8`).
- A result variable distinct from the floating-point input parameter avoids an
  extra register copy in `FUN_002A78D8`; this source change is required in
  addition to the reordered prologue.
- A local `void`-returning function-pointer view can preserve the shared
  value-returning declaration while reproducing the register allocation at a
  particular call. The accepted call still targets the same measured address;
  this does not recover the library's original C prototype.

## Reproducing the chain

Source: `gnu-ee-binutils-gcc-1.1.tar.gz`, sha256
`1f518043e252d6eda726386971d52eda26541ab936ea73a9783d73712b595f92` (the ps2dev
archive of the Sony/Cygnus EE compiler sources, target `mips64r5900-sf-elf`).

Patch stack: the cumulative `sce-991111b` stack published with the **Lombyte**
project (github.com/mateuszklysz/Lombyte, `patches/sce-991111b/`), minus its
`saves` widening, plus the qualified adjustments above. The build host is WSL with 32-bit
support (`gcc -m32`) and bison 1.28; the compiler and assembler are hashed in the
proofs:

| Tool | sha256 |
|---|---|
| `cc1` | `8bed6eaeec23dba7b10c94e3d907416cf9931c1ddc69ce5ffd2068497a02ad5d` |
| `cpp` | `2ac3d8d3ca177e6705ac2cbdd1bd9e9a7181ac3e40f6230dea6875c3218ec155` |
| `as` | `cda1a4e43dc8eaef2670d2445d6916050137330b2051a0695fe0d2631f3d7876` |

(An earlier `as`, `87a1a012…`, carried the two-point `mtc1` rule described above
and has been superseded. The earlier `cc1`, `3e7628b7…`, emitted the GPR save
block first. `c9952c1b…` reordered the save blocks; `158e5c20…` additionally
restores the post-DBR loop hook. `dff08a34…` additionally counts division-guard
expansions by their MD length. The new compiler was completely rebuilt in
two separate source/build directories, with identical hashes. `cpp` and the
`as` at that compiler milestone was unchanged. The subsequent `cda1a4e4`
assembler narrows the cleared-history exemption; `20c5f50b` is retained as its
predecessor. Two complete builds from fresh archive extractions produce identical
`cc1`, `cpp` and `as` hashes, and each passes 168/168 bodies and 89 fixture cases.)

Apply the restricted exemption patch after the earlier transfer-hazard patch
and before building gas. An earlier incremental diagnostic produced assembler
hash `5a0c6e9e`; compiling the identical `tc-mips.c` as `./config/tc-mips.c`,
as the normal Makefile does, accounts for the release hash difference. A controlled
recompilation changing only that source argument reproduces `cda1a4e4`.

The 1999 Makefile omits a dependency from `flow.o` to `insn-flags.h`, so a
clean parallel build can race the generated headers. Generate `insn-flags.h`,
`insn-codes.h` and `insn-config.h` before the parallel `cc1`/`cpp` build. This
build-order repair does not change the resulting compiler hash.

The linker stays the SDK `ld.exe` used before. The pipeline drives the chain
through `scripts/wsl_chain.py` (the 1999 tools are 32-bit Linux binaries: they
run under WSL, and every source is compiled under its bare name inside the WSL
filesystem, so the object is reproducible from any checkout).

There are two assembly paths in the integrated build. Reviewed C goes through
`cpp`/`cc1` and GNU `as` to produce its C object. The remaining reconstructed
assembly inputs still go through `Ps2EeAs.exe` in `scripts/build.py`. That build
then links both sets of objects; Ps2EeAs does not reassemble the already produced
C object. The C path was introduced in commit `b2b9101`. A padding assumption
about Ps2EeAs therefore does not automatically apply to the GNU-assembled C.

## Complete rebuild recipe

A partial recipe silently yields a different `mips.c` and a different `cc1`: the
source adjustments below are part of the qualified identity, not optional
clean-ups. Run every step and check the three source hashes at the end.

**Host.** Ubuntu 26.04.x under WSL2 with 32-bit host support (`gcc -m32`) and a
locally built **bison 1.28** first on `PATH`. The instruments are ordinary
host-built binaries, so their hashes depend on the distribution: a build on
another host produces different `cpp`/`cc1`/`as` hashes from identical sources.
No binaries are distributed; build them here and compare with the hashes above.

**1. Sources.**

```sh
tar xzf gnu-ee-binutils-gcc-1.1.tar.gz        # sha256 1f518043e252d6eda726386971d52eda26541ab936ea73a9783d73712b595f92
mv gnu-ee-binutils-gcc src
```

**2. Lombyte `sce-991111b` stack, minus the saves widening.** Apply, in this
order, from the stack published with the Lombyte project:

```
0000-modern-host-fixes 0001-r5900-quad-saves 0015-no-sibcall 0016-no-edge-lcm-default
0019-r5900-post-dbr-loop-pad 0020-gas-absolute-unknown-symbol
0021-sched-keep-frame-related-order 0022-sibcall-default-off 0025-annul-dead-delay-slots
0026-frame-save-first 0027-gas-inline-float-literals 0028-annul-ne-zero-default
0029-call-clobber-pending 0030-pad-before-preceding 0031-annul-traced-comparison
0032-anchor-all-pads 0033-ra-not-vs-nonframe 0034-r5900-extern-buffer-optin
0036-retire-frame-save-pref 0037-game-no-strict-aliasing 0044-sda-nosda-attributes
0045-encode-section-info-sda-nosda 0046-r5900-pad-unfilled-loops
0047-pathb-reload1-localalloc-regclass-2952 0048-pathb-cse-2952
0049-sibcall-pass-needs-placeholder 0050-r5900-dli-retail-form 0051-r5900-fpr-hazard-exact
0052-r5900-dli-retail-general 0053-gas-la-absolute-unknown-symbol
0054-r5900-assembler-pads-loops 0055-r5900-no-second-hilo 0056-sda-extern-before-use
```

then reverse the saves widening, because the retail saves in `sd` (8-byte slots),
not `sq` (16):

```sh
patch -R -p1 -s < "$P/0001-r5900-quad-saves.patch"
```

**3. Source adjustments.** Each one is measured; omitting any of them changes
`mips.c` and therefore `cc1`.

| File | Adjustment | Reason |
| --- | --- | --- |
| `gcc/config/mips/mips.c` | the `if (TARGET_MIPS5900)` line preceding `mips_reg_mode[0] = TImode;` becomes `if (0 && TARGET_MIPS5900)` | the retail does not use TImode in that position |
| `gcc/config/mips/mips.h` | add `#define MACHINE_DEPENDENT_REORG_AFTER_DBR(X) mips_r5900_pad_loops (X)` before `extern void mips_r5900_pad_loops ();` | patch `0054` disables the hook for the Ps2EeAs assembler; the C path uses GNU `as` and must keep it |
| `gcc/config/mips/mips.c` | in `mips_r5900_pad_loops`, `n++;` becomes `n += pat == TRAP_IF ? get_attr_length (insn) : 1;` | a division guard expands to two MD words (`beql` + `break`) |
| `gcc/config/mips/mips.c` | emit the GPR save loop in **ascending** register order, with `gp_offset -= GET_MODE_SIZE (mips_reg_mode[0]) * (n_rac2 - 1)` pre-computed when more than one register is saved and the per-register decrement turned into `+=` | the retail saves in ascending order with the same layout and offsets |
| `gcc/config/mips/mips.c` | run the FPR save block before the GPR save block (and keep GPR restores before FPR restores) | the retail orders the two intact blocks that way; patch `0026` covers the frame-save case, this completes it |
| `gas/config/tc-mips.c` | insert the `rac2_mtc1_nop_ok()` helper and guard both `++nops` sites with it: a `nop` follows `mtc1` when the next instruction reads the written FPR, except when `mtc1` is the function's first instruction | measured 587 nop in 598 cases; the single exception is `FUN_00283CE0` |

**4. Public transformers**, applied in this order:
`scripts/compiler/allow_zero_ti_store.patch` (patch), then
`scripts/compiler/disable_frame_order_default.py`, then
`scripts/compiler/restrict_mtc1_exemption.py` (`gas/config/tc-mips.c`).

**5. Configure and build.**

```sh
export CC='gcc -m32'
export CFLAGS='-O2 -fno-strict-aliasing -fcommon -std=gnu89 -D_GNU_SOURCE'
./configure --target=mips64r5900-sf-elf --host=i686-linux-gnu --build=i686-linux-gnu     --disable-nls --enable-languages=c --without-headers
(cd libiberty && make -j16 CC="$CC" CFLAGS="$CFLAGS")
# The 1999 Makefile omits a dependency from flow.o to insn-flags.h, so a clean
# parallel build can race the generated headers. Build them first.
(cd gcc && make -j1 LANGUAGES=c CC="$CC" CFLAGS="$CFLAGS" insn-flags.h insn-codes.h insn-config.h)
(cd gcc && make -j16 LANGUAGES=c CC="$CC" CFLAGS="$CFLAGS" cc1 cpp)
(cd bfd && make -j16 CC="$CC" CFLAGS="$CFLAGS")
(cd opcodes && make -j16 CC="$CC" CFLAGS="$CFLAGS")
(cd gas && make -j16 CC="$CC" CFLAGS="$CFLAGS")     # produces gas/as-new
```

**6. Checkpoints.** The build is the qualified one only when all six identities
match:

| Artifact | sha256 |
| --- | --- |
| `gcc/config/mips/mips.c` | `c76c0bec5b56c198381ab2a4fc60c161a4287e8312d7d1fdea3d1e6a0e1af614` |
| `gcc/config/mips/mips.h` | `52f470a043ffbba535582e2f08a8353d23bbf6521b1b63e241847f464891d9e6` |
| `gas/config/tc-mips.c` | `61e51c1ebcdf860db4503b6cc6a11c40596d1f3c969daf66ee56a45f454130ca` |
| `gcc/cc1` | `8bed6eaeec23dba7b10c94e3d907416cf9931c1ddc69ce5ffd2068497a02ad5d` |
| `gcc/cpp` | `2ac3d8d3ca177e6705ac2cbdd1bd9e9a7181ac3e40f6230dea6875c3218ec155` |
| `gas/as-new` | `cda1a4e43dc8eaef2670d2445d6916050137330b2051a0695fe0d2631f3d7876` |

A fresh build from this recipe was verified on 7 October 2026 and reproduced all
six identities. A `mips.c` that hashes differently means a step above is missing
— a rebuild that skips the adjustments produces a compiler that still matches the
measured corpus on simple bodies and diverges elsewhere, which is exactly the
failure mode this section exists to prevent.

## Scope

This document claims what was measured: the named bodies and the 96 previously
integrated bodies are reproduced byte-for-byte; the profile is not offered as a
general RAC2 compiler qualification. Bodies that exercise VU/MMI instructions or
`$gp` are outside it. The four rules above are the ones the corpus could
falsify; a future body that disagrees with them is a measurement, not a surprise.

## English source transformers

The operand-exemption and frame-default adjustments are now expressed as
English Python source transformers, replacing patch files that included French
source annotations. They retain the same narrowly checked input states.

The exemption transformer reproduces the qualified `tc-mips.c` exactly
(SHA-256 `61e51c1ebcdf860db4503b6cc6a11c40596d1f3c969daf66ee56a45f454130ca`).
The frame transformer changes only the source comment relative to the qualified
frame-off source: its source hash is
`c76c0bec5b56c198381ab2a4fc60c161a4287e8312d7d1fdea3d1e6a0e1af614`,
and host preprocessing with the release flags produces identical output.
The active compiler and assembler remain the qualified binaries recorded above;
this documentation change does not claim a new binary rebuild.

## Small-data symbols under the pinned default profile (2026-10-07)

The default profile is `-O2 -G0 -ffunction-sections` and emits no `$gp` access
of its own. A body whose retail bytes address a global through `$gp` is
nevertheless reachable, because the choice is the assembler's, not the
compiler's:

* cc1 emits a bare symbol operand for a load or store it can expand as a macro;
  `gas` then decides per site — `lui`+`%lo` in ordinary flow, a one-instruction
  `$gp` form inside a `.set nomacro` region, which is where a compiler delay
  slot lands.
* `__attribute__((sda))` on the declaration restores cc1's one-instruction model
  for that symbol. It does not by itself force `$gp` anywhere; it makes cc1 emit
  the form that leaves the per-site decision to the assembler, which is what the
  retail build did. Declaring the symbol `nosda` instead makes cc1 materialise
  the address explicitly, and that two-instruction model shifts register
  allocation and scheduling away from the retail bytes.

Measured consequence: a body that a `nosda` assignment refuses can still be
exact when the same symbol is declared `sda`. The campaign driver therefore
retries a refused body with every measured `nosda` flipped to `sda` and keeps
only the variant the owner gate verifies on every placement.

The boundary that remains: an access that retail performs through `$gp` in
ordinary flow — not in a delay slot — needs the `.extern name, size` directive
that the reconstructed backend only emits under `-G8`. Such families stay on the
small-data unit route.

A related format constraint: the level catalogue requires every external address
to be word aligned, so a byte global at an odd address is bound through its
aligned base with a constant index; the assembler folds the constant into the
same immediate and the bytes are unchanged.
