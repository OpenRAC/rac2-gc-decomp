# mount_sync.sh — verify a file's content matches between host and container.
#
#   sh tools/ee/mount_sync.sh check <path> <md5>   -> 0 if verified, retry while stale
#   Runs inside the ee-build container.
#
# A host-written file that grows can be read truncated in a container that opens
# it within ~20 s of an earlier VM read (FACT #8713). This helper retries the
# read until the container's md5 matches the host's, then returns. A truncated
# read is a hard error (the caller's md5 is wrong if it doesn't match).
#
# The file is read with `cat` and piped to `md5sum`, so a caller that took the
# host md5 passes it here for verification. A mismatch is a hard error (the host
# md5 is authoritative); the caller must regenerate it.

set -eu

MODE="$1"; FILE="$2"; EXPECTED_MD5="$3"

# retry_read_md5 <path> -> md5 on stdout, retries while the VM's sshfs view is
# stale, rc 9 naming the file if it never agrees.
retry_read_md5() {
  local path="$1"
  local actual=""
  local retries=0
  while true; do
    actual=$(cat "$path" | md5sum | cut -d' ' -f1) || { echo "read failed" >&2; return 1; }
    [ "$actual" = "$EXPECTED_MD5" ] && return 0
    retries=$((retries + 1))
    [ $retries -gt 10 ] && { echo "mount_sync: stale view of $path after $retries retries" >&2; return 9; }
    sleep 1
  done
}

case "$MODE" in
  check)
    [ -f "$FILE" ] || { echo "mount_sync: $FILE missing" >&2; exit 1; }
    retry_read_md5 "$FILE" || exit 9
    ;;
  *)
    echo "mount_sync: unknown mode $MODE" >&2
    exit 2
    ;;
esac
