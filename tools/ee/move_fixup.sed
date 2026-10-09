# move_fixup.sed — convert cc1's `move` pseudo to `daddu` for the EE.
#
# cc1 emits `move $rd, $rs` which the SN ee-as expands to `daddu $rd, $rs, $0`.
# GNU as also knows `move` as a pseudo, but it expands to `addiu` (32-bit), not
# `daddu`. This filter rewrites `move` to `daddu` so the assembled code matches
# the original's 64-bit operations.
#
# Usage:  sed -f tools/ee/move_fixup.sed < in.s > out.s

s/\bmove\b/daddu/
