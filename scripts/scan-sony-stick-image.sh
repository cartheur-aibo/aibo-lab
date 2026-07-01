#!/usr/bin/env bash

set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/scan-sony-stick-image.sh <stick-image>

Inspect a raw Sony Memory Stick image with 7-Zip and classify it as one of:
  - Sony-formatted reference media with no OPEN-R runtime
  - AIBO MIND 2 style stick
  - AIBO MIND 3 style stick
  - Unknown / mixed layout

This is a read-only scanner. It does not mount or modify the image.
EOF
}

if [ "${1:-}" = "" ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

IMAGE=$1

if [ ! -f "$IMAGE" ]; then
  echo "error: image not found: $IMAGE" >&2
  exit 1
fi

if ! command -v 7z >/dev/null 2>&1; then
  echo "error: 7z is required but not installed" >&2
  exit 1
fi

LISTING=$(mktemp)
trap 'rm -f "$LISTING"' EXIT

7z l "$IMAGE" > "$LISTING"

say_marker() {
  local label=$1
  local pattern=$2
  if grep -Fq "$pattern" "$LISTING"; then
    echo "present: $label"
  else
    echo "missing: $label"
  fi
}

echo "Sony stick image scan"
echo "image: $IMAGE"
echo

grep -E '^(Path =|Type =|Physical Size =|Sector Size =|File System =|Offset =|Cluster Size =|Free Space =|Headers Size =|Label =)' \
  "$LISTING" || true

echo
say_marker "PALM/" " PALM"
say_marker "MEMSTICK.IND" " MEMSTICK.IND"
say_marker "OPEN-R/" " OPEN-R"
say_marker "OPEN-R/BOOTPARA" " OPEN-R/BOOTPARA"
say_marker "MIND 3 SA.BIN" " OPEN-R/APP/OBJS/SA.BIN"
say_marker "MIND 3 STM.BIN" " OPEN-R/APP/OBJS/STM.BIN"
say_marker "MIND 3 APP/PC/DIARY" " OPEN-R/APP/PC/DIARY"

has_memstick=0
has_openr=0
has_bootpara=0
has_sa=0
has_stm=0
has_diary=0

grep -Fq " MEMSTICK.IND" "$LISTING" && has_memstick=1 || true
grep -Fq " OPEN-R" "$LISTING" && has_openr=1 || true
grep -Fq " OPEN-R/BOOTPARA" "$LISTING" && has_bootpara=1 || true
grep -Fq " OPEN-R/APP/OBJS/SA.BIN" "$LISTING" && has_sa=1 || true
grep -Fq " OPEN-R/APP/OBJS/STM.BIN" "$LISTING" && has_stm=1 || true
grep -Fq " OPEN-R/APP/PC/DIARY" "$LISTING" && has_diary=1 || true

echo
if [ "$has_openr" -eq 0 ] && [ "$has_memstick" -eq 0 ]; then
  echo "classification: Sony-formatted reference media or non-boot stick"
  echo "reason: OPEN-R/ and MEMSTICK.IND are both absent"
elif [ "$has_openr" -eq 1 ] && [ "$has_bootpara" -eq 1 ] && \
     [ "$has_sa" -eq 0 ] && [ "$has_stm" -eq 0 ] && [ "$has_diary" -eq 0 ]; then
  echo "classification: AIBO MIND 2 style stick"
  echo "reason: OPEN-R runtime present with BOOTPARA and no MIND 3 markers"
elif [ "$has_openr" -eq 1 ] && \
     { [ "$has_sa" -eq 1 ] || [ "$has_stm" -eq 1 ] || [ "$has_diary" -eq 1 ]; }; then
  echo "classification: AIBO MIND 3 style stick"
  echo "reason: OPEN-R runtime present with one or more MIND 3 markers"
else
  echo "classification: unknown or mixed layout"
  echo "reason: image does not match the current reference rules cleanly"
fi
