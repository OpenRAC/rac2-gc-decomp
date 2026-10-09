# src_alias_provides.sh — provide old address-named symbols the C sources still use
# for symbols symbol_addrs has renamed.
#
#   . tools/ee/src_alias_provides.sh <region>  >> all_addr_syms.ld
#   e.g. tools/ee/src_alias_provides.sh usa >> going-decompiled/build/usa/all_addr_syms.ld
#
# Reads going-decompiled/symbol_addrs/$REGION/alias_provides.txt, one row:
#   <old_name> <new_name>
# and emits:
#   PROVIDE(<old_name> = <new_name>);
# which allows the linker to resolve references to the old name by providing it
# as an alias to the new name. A unit that still uses the old name (e.g. a
# hand-written .s that hasn't been converted to use symbol_addrs rows yet) will
# link because ld will see PROVIDE(old = new) and resolve references to old
# against new's address.

REGION="$1"
PROVIDES="going-decompiled/symbol_addrs/$REGION/alias_provides.txt"
[ -f "$PROVIDES" ] || exit 0

# A line in alias_provides.txt is:
#   old_name new_name   (space or tab separated)
# Output a PROVIDE line for each pair:
#   PROVIDE(old_name = new_name);

awk '{ sub(/\r$/, ""); sub(/#.*/, ""); gsub(/[ \t]+/, " "); sub(/^ /, ""); sub(/ $/, ""); n = split($0, a, " "); if (n == 2) printf "PROVIDE(%s = %s);\n", a[1], a[2] }' "$PROVIDES"
