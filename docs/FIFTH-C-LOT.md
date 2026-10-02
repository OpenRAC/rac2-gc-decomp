# Fifth RAC2 C lot - 2026-10-02

Six further bodies are integrated in the boot, taking the integrated C from **1752 to 2008
bytes** (86 to 92 functions), and each one is placed in all **26 non-tutorial overlays**.
`check_candidates.py` reports **92/92 matched, zero different bytes** against the pinned boot.

## Where they come from

The rule of the fourth lot, unchanged: a boot body whose reviewed C compiles to the bytes
that *open* a function in the level overlays. Nothing is ported from another game, no name
or address is promoted by analogy, and every body was located by exact byte search.

| Symbol (RAC2) | Reviewed bytes | Level placements | What it is |
| --- | ---: | ---: | --- |
| `FUN_002934B8` | 60 | 26 | four bytes copied, then two pointers computed from offsets |
| `FUN_003364C8` | 60 | 26 | masks the top byte of two words at +0 and +4 |
| `FUN_00336508` | 60 | 26 | the same mask at +8 and +0xC |
| `FUN_00336ED0` | 28 | 26 | pushes a node on a list head and decrements the count |
| `FUN_0033B030` | 28 | 26 | six floats stored at their fixed offsets |
| `FUN_00351E58` | 20 | 26 | equality of two fields, returned as 0/1 |

256 bytes in this lot: **2008 bytes integrated in the boot**, **2091 placements and 48,156
placement bytes** in the level catalogue.

The two pointer-mask bodies are worth a note, because their shape is a rule rather than a
coincidence: each one reads the same object field twice and stores through it, and the
retail build reloads that field between the two stores. The C that reproduces it is the
plain one - the field is read again because a store through the pointer may alias the
object - so the reload is the compiler's own aliasing decision, and it is what the bytes
show.

## The measurement that changes the campaign arithmetic

The working list (`gisement-prefixe-ecrivable.json`) claimed **166 writable targets, 13,888
bytes**. That filter kept every body without a call, without `$gp`, without VU/MMI and
without a data reference - and it is too generous. Filtering the 155 remaining targets
again, on 2026-10-02, gives **76 targets and 4,032 bytes**:

- a body that **reads a temporary register** (`$t0`-`$t9`, `$v0`-`$v1`, `$at`) before
  writing it is not a function at all: its inputs come from nowhere, so no C can produce
  it. This is either an internal block that the disassembler labelled, or a routine with a
  house calling convention. Six targets, including `FUN_0034F960`, `FUN_00276158`,
  `FUN_002A0468`;
- the rest of the loss is measured refusal, not doubt (below).

| Reason the remaining target is out of reach | Targets | Bytes |
| --- | ---: | ---: |
| Calls another function (cannot be placed in a level, whose catalogue is self-contained) | 34 | 4,944 |
| Cites a global (`D_...`) | 19 | 1,436 |
| VU / MMI instructions | 15 | 2,632 |
| Reads a temporary register before writing it | 6 | 544 |
| `$gp`-relative access (out of reach under the required `-G0`) | 5 | 136 |
| **Reachable ground left to write** | **76** | **4,032** |

At the counting rate of this repository - one body in the boot plus one per overlay that
carries its bytes - those 4,032 bytes are worth about **108,864 counted bytes**. The
0.5 % objective (243,941 bytes) needs roughly 7,433 bytes of C, so the reachable pool alone
does not reach it: the call, global, VU and `$gp` families have to be opened, or the
compiler profile extended.

## Shapes the qualified compiler does not produce

Four bodies of this lot's neighbourhood were written and refused by the byte gate. Each
refusal is a measurement, kept here so the next session does not pay it again.

- **A constant stored into a field and returned.** Retail: `li $v0,1` / `jr $ra` /
  `sw $v0,off($a0)` - one register, the store in the return's delay slot
  (`FUN_00339760`, `FUN_003518C8`). With `-O2 -G0`, every form tried (assignment
  expression, local variable, struct field, `volatile` field) emits *two* registers
  (`li $v1,1` / `li $v0,1` / `jr` / `sw $v1,...`): the compiler rematerialises the constant
  for the return instead of sharing the register. Six further candidates of the same shape
  remain in the reachable list and are expected to refuse the same way.
- **Float helpers that keep the conversion in the floating-point registers.** Retail:
  `cvt.w.s $f0,$f12` then `cvt.s.w $f0,$f0` then `sub.s`. The compiler routes the integer
  through a general register (`mfc1` + `mtc1`) between the two conversions, adding two
  instructions the retail does not have (`FUN_00283CB8`, `FUN_00283CC8`).
- **A hazard `nop` after `mtc1`.** Retail inserts a `nop` between the constant load and the
  first use of the register; the compiler emits none (`FUN_002A7798`). Seven reachable
  targets carry this shape.
- **Hand-written stubs.** A byte copy whose loop uses encodings the assembler marks
  `handwritten instruction` (non-zero shift fields on `add`/`addi`), and a six-`nop` delay
  loop whose body branches to its own label (`FUN_00282AB0`, `FUN_00282A38`,
  `FUN_00282A60`). These are not compiler output and are excluded from the reachable count.

## Scope

2008 boot bytes plus 48,156 level placement bytes are **50,164 counted C bytes** of
48,788,176 executable bytes, about **0.1028 %**.
