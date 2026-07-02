#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/explain-ers7-system-delta.sh <baseline-ms-root> <candidate-ms-root>

Explain the visible SYSTEM-layer drift between two ERS-7 stick trees.

Accepted inputs:
  - a Memory Stick root containing OPEN-R/
  - a sample directory containing MS/OPEN-R/

This tool focuses on the boot-critical SYSTEM layer that has mattered most in
real hardware testing so far.
EOF
}

if [ "${1:-}" = "" ] || [ "${2:-}" = "" ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

resolve_ms_root() {
  local input=$1
  if [ -d "$input/OPEN-R" ]; then
    printf '%s\n' "$input"
  elif [ -d "$input/MS/OPEN-R" ]; then
    printf '%s\n' "$input/MS"
  else
    echo "error: could not locate OPEN-R tree under $input" >&2
    exit 1
  fi
}

is_plain_ms_list() {
  local path=$1
  [ -f "$path" ] || return 1
  grep -aq '^/MS/' "$path"
}

print_config_summary() {
  local label=$1
  local root=$2
  local relpath=$3
  local fullpath="$root/$relpath"

  printf '%s: ' "$label"

  if [ ! -f "$fullpath" ]; then
    echo "missing"
    return
  fi

  if is_plain_ms_list "$fullpath"; then
    local count
    count=$(grep -ac '^/MS/' "$fullpath")
    printf 'plain-text /MS list (%s entries)\n' "$count"
    grep -a '^/MS/' "$fullpath" | sed 's/^/  - /'
    return
  fi

  local size
  size=$(wc -c < "$fullpath")
  printf 'opaque/binary retail-style config (%s bytes)\n' "$size"
}

extract_ms_entries() {
  local root=$1
  local relpath=$2
  local fullpath="$root/$relpath"

  if [ -f "$fullpath" ] && is_plain_ms_list "$fullpath"; then
    grep -a '^/MS/' "$fullpath" | awk '{print $1}'
  fi
}

list_system_objs() {
  local root=$1
  find "$root/OPEN-R/SYSTEM/OBJS" -maxdepth 1 -type f -printf '%f\n' | sort
}

BASELINE_ROOT=$(resolve_ms_root "$1")
CANDIDATE_ROOT=$(resolve_ms_root "$2")

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

BASELINE_SYSTEM_OBJS="$tmpdir/baseline-system-objs.txt"
CANDIDATE_SYSTEM_OBJS="$tmpdir/candidate-system-objs.txt"
BASELINE_CARDDRV="$tmpdir/baseline-carddrv.txt"
CANDIDATE_CARDDRV="$tmpdir/candidate-carddrv.txt"
BASELINE_EXTOBJ="$tmpdir/baseline-extobj.txt"
CANDIDATE_EXTOBJ="$tmpdir/candidate-extobj.txt"

list_system_objs "$BASELINE_ROOT" > "$BASELINE_SYSTEM_OBJS"
list_system_objs "$CANDIDATE_ROOT" > "$CANDIDATE_SYSTEM_OBJS"
extract_ms_entries "$BASELINE_ROOT" "OPEN-R/SYSTEM/CONF/CARDDRV.CFG" > "$BASELINE_CARDDRV" || true
extract_ms_entries "$CANDIDATE_ROOT" "OPEN-R/SYSTEM/CONF/CARDDRV.CFG" > "$CANDIDATE_CARDDRV" || true
extract_ms_entries "$BASELINE_ROOT" "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG" > "$BASELINE_EXTOBJ" || true
extract_ms_entries "$CANDIDATE_ROOT" "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG" > "$CANDIDATE_EXTOBJ" || true

echo "ERS-7 SYSTEM delta explainer"
echo "baseline:  $BASELINE_ROOT"
echo "candidate: $CANDIDATE_ROOT"
echo

echo "config representation:"
print_config_summary "baseline CARDDRV.CFG" "$BASELINE_ROOT" "OPEN-R/SYSTEM/CONF/CARDDRV.CFG"
print_config_summary "candidate CARDDRV.CFG" "$CANDIDATE_ROOT" "OPEN-R/SYSTEM/CONF/CARDDRV.CFG"
print_config_summary "baseline EXTOBJ.CFG" "$BASELINE_ROOT" "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG"
print_config_summary "candidate EXTOBJ.CFG" "$CANDIDATE_ROOT" "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG"
echo

echo "SYSTEM object set drift:"
extra_objs=$(comm -13 "$BASELINE_SYSTEM_OBJS" "$CANDIDATE_SYSTEM_OBJS" || true)
missing_objs=$(comm -23 "$BASELINE_SYSTEM_OBJS" "$CANDIDATE_SYSTEM_OBJS" || true)

if [ -n "$extra_objs" ]; then
  echo "extra candidate objects:"
  while IFS= read -r obj; do
    [ -z "$obj" ] && continue
    echo "  + $obj"
  done <<< "$extra_objs"
else
  echo "extra candidate objects: none"
fi

if [ -n "$missing_objs" ]; then
  echo "missing baseline objects:"
  while IFS= read -r obj; do
    [ -z "$obj" ] && continue
    echo "  - $obj"
  done <<< "$missing_objs"
else
  echo "missing baseline objects: none"
fi
echo

echo "plain-text config entry drift:"

report_entry_diff() {
  local title=$1
  local baseline_entries=$2
  local candidate_entries=$3
  local baseline_count candidate_count

  baseline_count=$(wc -l < "$baseline_entries")
  candidate_count=$(wc -l < "$candidate_entries")

  echo "$title:"
  if [ "$baseline_count" -eq 0 ] && [ "$candidate_count" -eq 0 ]; then
    echo "  neither side exposed plain-text /MS entries"
    return
  fi
  if [ "$baseline_count" -eq 0 ]; then
    echo "  baseline is opaque/binary; candidate exposes these entries:"
    sed 's/^/  + /' "$candidate_entries"
    return
  fi
  if [ "$candidate_count" -eq 0 ]; then
    echo "  candidate is opaque/binary; baseline exposes these entries:"
    sed 's/^/  - /' "$baseline_entries"
    return
  fi

  local added removed
  added=$(comm -13 "$baseline_entries" "$candidate_entries" || true)
  removed=$(comm -23 "$baseline_entries" "$candidate_entries" || true)

  if [ -z "$added" ] && [ -z "$removed" ]; then
    echo "  plain-text entry set unchanged"
    return
  fi

  if [ -n "$added" ]; then
    while IFS= read -r entry; do
      [ -z "$entry" ] && continue
      echo "  + $entry"
    done <<< "$added"
  fi
  if [ -n "$removed" ]; then
    while IFS= read -r entry; do
      [ -z "$entry" ] && continue
      echo "  - $entry"
    done <<< "$removed"
  fi
}

report_entry_diff "CARDDRV.CFG" "$BASELINE_CARDDRV" "$CANDIDATE_CARDDRV"
report_entry_diff "EXTOBJ.CFG" "$BASELINE_EXTOBJ" "$CANDIDATE_EXTOBJ"
echo

echo "interpretation:"
if [ ! -s "$BASELINE_CARDDRV" ] || [ ! -s "$BASELINE_EXTOBJ" ]; then
  echo "- the baseline uses opaque retail-style SYSTEM config, so we cannot yet"
  echo "  prove semantic equivalence against the SDK overlays"
fi
if [ -n "$extra_objs" ]; then
  echo "- the candidate adds SYSTEM objects beyond the preserved baseline, which"
  echo "  means early boot behavior can change before app-layer code runs"
fi
if [ -s "$CANDIDATE_CARDDRV" ] || [ -s "$CANDIDATE_EXTOBJ" ]; then
  echo "- the candidate exposes readable SDK-style boot object references, which"
  echo "  helps analysis but also confirms a boot-layer representation change"
fi
