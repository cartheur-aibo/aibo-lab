#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/compare-mind2-persistent-state.sh <image-a> <image-b>

Extract two raw stick images with 7-Zip and compare the most important
persistent-state areas for MIND 2 behavior work.

Current focus:
  OPEN-R/APP/DATA/P/
  OPEN-R/MW/DATA/P/
  OPEN-R/SYSTEM/DATA/P/
EOF
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

if [ "$#" -ne 2 ]; then
  usage
  exit 1
fi

IMG_A=$1
IMG_B=$2

for img in "$IMG_A" "$IMG_B"; do
  if [ ! -f "$img" ]; then
    echo "error: image not found: $img" >&2
    exit 1
  fi
done

if ! command -v 7z >/dev/null 2>&1; then
  echo "error: 7z is required but not installed" >&2
  exit 1
fi

TMP_BASE=$(mktemp -d)
trap 'rm -rf "$TMP_BASE"' EXIT
DIR_A="$TMP_BASE/a"
DIR_B="$TMP_BASE/b"

7z x -y -o"$DIR_A" "$IMG_A" >/dev/null
7z x -y -o"$DIR_B" "$IMG_B" >/dev/null

compare_tree() {
  local rel=$1
  local label=$2
  local a="$DIR_A/$rel"
  local b="$DIR_B/$rel"

  echo "== $label =="

  if [ ! -d "$a" ] || [ ! -d "$b" ]; then
    echo "missing tree in one image: $rel"
    echo
    return
  fi

  local files
  files=$( (find "$a" -maxdepth 1 -type f -printf '%f\n'; find "$b" -maxdepth 1 -type f -printf '%f\n') | sort -u )

  while IFS= read -r f; do
    [ -z "$f" ] && continue
    if [ ! -f "$a/$f" ]; then
      echo "ONLY_B $f"
    elif [ ! -f "$b/$f" ]; then
      echo "ONLY_A $f"
    else
      local ha hb
      ha=$(sha256sum "$a/$f" | awk '{print $1}')
      hb=$(sha256sum "$b/$f" | awk '{print $1}')
      if [ "$ha" = "$hb" ]; then
        echo "SAME $f"
      else
        echo "DIFF $f"
      fi
    fi
  done <<< "$files"

  echo
}

echo "Persistent-state comparison"
echo "A: $IMG_A"
echo "B: $IMG_B"
echo

compare_tree "OPEN-R/APP/DATA/P" "OPEN-R/APP/DATA/P"
compare_tree "OPEN-R/MW/DATA/P" "OPEN-R/MW/DATA/P"
compare_tree "OPEN-R/SYSTEM/DATA/P" "OPEN-R/SYSTEM/DATA/P"
