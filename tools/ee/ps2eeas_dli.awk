# ps2eeas_dli.awk — ps2eeas_dli_sites.txt allowlist pass for dli expansion.
#
# Usage: awk -f tools/ee/ps2eeas_dli.awk < input.s > output.s
#
# Reads ps2eeas_dli_sites.txt and replaces `dli` instructions with the ROM's
# expansion at sites listed there. Every other `dli` stays GNU as's default.
#
# The pass prints one line on stderr (task #1205):
#   asm_unit.sh: dli: N transforms (M allowlist rows for <region>)
# N = number of dli SITES substituted, M = valid allowlist rows for <region>.
# This is an INTERFACE: landing_gate.sh's ASMUNIT row knows it.

BEGIN {
  count = 0
}

# Count mode (empty input): print <valid> <usa> <eu> <bad>
FNR == NR && count_mode {
  if ($0 ~ /^#/ || $0 ~ /^[[:space:]]*$/) next
  n = split($0, a, " ")
  if (n != 4) { bad++; next }
  if (a[1] == "usa") usa++
  else if (a[1] == "eu") eu++
  else bad++
  valid++
  next
}

# Data mode: load the allowlist
FNR != NR {
  if ($0 ~ /^#/ || $0 ~ /^[[:space:]]*$/) next
  n = split($0, a, " ")
  if (n != 4) next
  region = a[1]
  fn = a[2]
  operands = a[3]
  words = a[4]
  
  key = region SUBSEP fn SUBSEP operands
  sites[key] = words
}

# Process input .s file
FNR != NR {
  # Check if this is a dli instruction with operands we know
  if (/^[[:space:]]*dli[[:space:]]/) {
    # Extract operands
    line = $0
    sub(/^[[:space:]]*dli[[:space:]]+/, "", line)
    sub(/[[:space:]]*$/, "", line)
    
    # Get current function
    for (i = 1; i <= NR; i++) {
      if (lines[i] ~ /^[[:space:]]*\.ent[[:space:]]/) {
        match(lines[i], /\.ent[[:space:]]+([A-Za-z_][A-Za-z0-9_]*)/, arr)
        curfn = arr[1]
        break
      }
    }
    
    key = current_region SUBSEP curfn SUBSEP line
    if (key in sites) {
      # Replace with expansion
      words = sites[key]
      split(words, w, " ")
      print "\t.word\t0x" w[1]
      print "\t.word\t0x" w[2]
      print "\t.word\t0x" w[3]
      print "\t.word\t0x" w[4]
      # Add marker for counting
      print "\t# ps2eeas_dli_sites.txt " current_region ": dli " curfn " " line " -> " words
      transforms++
      next
    }
  }
  
  # Track region and function
  if (/^[[:space:]]*\.text/) {
    current_region = (REGION == "") ? "usa" : REGION
  }
  if (/^[[:space:]]*\.ent[[:space:]]/) {
    match($0, /\.ent[[:space:]]+([A-Za-z_][A-Za-z0-9_]*)/, arr)
    curfn = arr[1]
  }
  
  print
}

END {
  if (count_mode) {
    print valid " " usa " " eu " " bad
  } else {
    print "asm_unit.sh: dli: " transforms " transforms (" valid " allowlist rows for " current_region ")" > "/dev/stderr"
  }
}
