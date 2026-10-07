# Small-data level units: measured evidence

## The problem

Some retail bodies address a global through `$gp` — a single `sw $v0,16792($gp)`
where the address base is the small-data pointer register. The pinned default
profile is `-O2 -G0 -ffunction-sections`, which never emits that form: it always
materialises the address absolutely. A C body in that shape therefore cannot be
reproduced at all under the default profile, whatever the source looks like —
about thirty-five measured families are parked for exactly this reason.

Changing a level's existing unit to `-G8` is not an option: one unit compiles
with one flag set, and every body already integrated in it would be recompiled
into different bytes.

## The mechanism

An overlay may carry a **second C unit** with its own profile, gp, catalog,
review and object. Both units are compiled, linked into the same overlay, and
every function of both is compared complete against the pinned image. The
pattern is the one the boot already uses for its separately qualified SDK units.

| Element | Path |
| --- | --- |
| authored body | `src/levels/smalldata/<name>.cfrag` |
| generated unit | `candidates/levels/<level>-g8.c` |
| catalogue | `config/level-g8/<level>.json` |
| review | `progress/level-g8/<level>.json` |

## The guards

* The loader accepts only the measured profile (`-O2 -G8 -ffunction-sections`),
  only the measured overlay gp (`0x001AEFF0`), and only a unit that names an
  authored fragment under `src/levels/smalldata/`. Any other value is refused
  before a compile.
* The unit is qualified exactly like a default native unit: complete symbols,
  own object proof, own review, instruments identical to the shared ones.
* The integration links both objects, tags each function with its owner
  (`boot-shared`, `level-native`, `level-smalldata`), and refuses overlap.
* The proof exporter validates the small-data section with the same rules as the
  native one and rejects a section without a reviewed unit, or a reviewed unit
  without its section.
* The program's dependency receipt covers the unit's three files, so a change to
  any of them invalidates that program's proof.

## Pilot result

`19_grelbin` gained one 108-byte body (`LVL_19_GRELBIN_FUN_002EDDF0`), which
writes four ring-header words and then advances the resident pointer **through
the global itself** — the store the default profile cannot express. The level
proof now records three owner classes (163 boot-shared, 96 level-native, 1
level-smalldata), its full gate compares 2,822,184 loaded bytes, and all
twenty-eight programs still match byte for byte.

## Limits

* One measured gp, one measured flag set, one overlay at a time; anything else
  is refused rather than approximated.
* The unit adds C integration bytes only. It claims no original source, object
  boundary or data ownership, and the body is ordinary C — no byte is patched.
* A body belongs in this unit only when its retail form actually addresses
  through `$gp`; a body that can be expressed under the default profile stays
  there.
