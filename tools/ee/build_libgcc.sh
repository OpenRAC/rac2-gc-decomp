# build_libgcc.sh — build libgcc.a from GCC source for the specified region.
#
#   sh tools/ee/build_libgcc.sh <region>
#   e.g. sh tools/ee/build_libgcc.sh usa
#
# Reads going-decompiled/libgcc/members.txt, one row per member to extract from
# the built libgcc.a. Each row is:
#   <member.o> <gcc_source_file.c>
#
# Builds each source file with the pinned gcc version and extracts the object.
# Writes going-decompiled/build/<region>/lib/libgcc.a and a members.txt that
# lists the extracted objects (matching what the linker needs).
#
# A region whose .ld names no libgcc members (EU today) builds nothing and
# leaves libgcc.a empty (the .ld still links with it, so the file exists).

set -e
REGION="$1"

LIBGCC_DIR="going-decompiled/libgcc"
BUILD_DIR="going-decompiled/build/$REGION"
LIB_DIR="$BUILD_DIR/lib"

# EU has no libgcc members in its .ld yet.
case "$REGION" in
  usa) ;;
  eu)  mkdir -p "$LIB_DIR"; : > "$LIB_DIR/libgcc.a"; exit 0 ;;
  *)   echo "build_libgcc: unknown region $REGION" >&2; exit 2 ;;
esac

[ -f "$LIBGCC_DIR/members.txt" ] || { echo "build_libgcc: $LIBGCC_DIR/members.txt missing" >&2; exit 1; }

mkdir -p "$LIB_DIR"

# Build libgcc from GCC source (the verbatim source in going-decompiled/libgcc)
# using the pinned gcc version (2.9-ee-991111b/r4). Each member is compiled
# with -march=r5900 -mabi=eabi -O2 -fno-builtin.

WIBO=/usr/local/bin/wibo
GCC_LIB="$WIBO tools/ee/cc/lib/gcc-lib/ee/2.9-ee-991111"
CC="$GCC_LIB/cc1.exe"

# Build each member and extract it to libgcc.a
TMPDIR="$(mktemp -d)"
trap 'rm -rf "$TMPDIR"' EXIT

# Compile each source file
while IFS= read -r row || [ -n "$row" ]; do
  # Skip comments and empty lines
  row="${row%%#*}"
  [ -z "$row" ] && continue

  obj=$(echo "$row" | awk '{print $1}')
  src=$(echo "$row" | awk '{print $2}')

  # Compile with the same flags the ROM used
  $WIBO $CC -quiet -march=r5900 -mabi=eabi -O2 -fno-builtin "$LIBGCC_DIR/$src" -o "$TMPDIR/$obj" || {
    echo "build_libgcc: failed to compile $src" >&2
    exit 1
  }
done < "$LIBGCC_DIR/members.txt"

# Create libgcc.a with the built objects
cd "$LIB_DIR"
# Use mips-linux-gnu-ar to create the archive
if [ -n "$(ls -A $TMPDIR/*.o 2>/dev/null)" ]; then
  mips-linux-gnu-ar rcs libgcc.a "$TMPDIR"/*.o
else
  : > libgcc.a
fi

# Write members.txt for the linker
cp "$LIBGCC_DIR/members.txt" "$LIB_DIR/members.txt"

echo "build_libgcc: built libgcc.a for $REGION ($(wc -l < "$LIB_DIR/members.txt") members)"
