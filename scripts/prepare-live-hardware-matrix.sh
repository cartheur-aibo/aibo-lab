#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-live-hardware-matrix.sh [output-root]

Prepare the first four live-hardware matrix stick trees:

1. baseline
2. 0400
3. 0400 + 1200
4. 0400 + IEG

The script copies the current selected source trees into one bench-facing
directory with stable numbered names and writes a manifest.

Environment variables:
  INJECT_LAB_WLAN=1   Replace WLANCONF.TXT in each prepared tree with
                      out/ERS7M2/WLANCONF.TXT
EOF
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUTPUT_ROOT="${1:-$ROOT_DIR/features/mind2-behavior-research/build/live-hardware-matrix-2026-07-01}"
INJECT_LAB_WLAN="${INJECT_LAB_WLAN:-0}"

if [ "$INJECT_LAB_WLAN" = "1" ] && [ ! -f "$ROOT_DIR/out/ERS7M2/WLANCONF.TXT" ]; then
  echo "error: out/ERS7M2/WLANCONF.TXT not found for lab WLAN injection" >&2
  exit 1
fi

prepare_one() {
  local label=$1
  local source_tree=$2
  local target_dir="$OUTPUT_ROOT/$label"

  if [ ! -d "$source_tree/OPEN-R" ]; then
    echo "error: missing OPEN-R in source tree: $source_tree" >&2
    exit 1
  fi

  rm -rf "$target_dir"
  mkdir -p "$target_dir"
  cp -a "$source_tree/." "$target_dir/"
  chmod -R u+w "$target_dir"

  if [ "$INJECT_LAB_WLAN" = "1" ]; then
    cp -a "$ROOT_DIR/out/ERS7M2/WLANCONF.TXT" \
      "$target_dir/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
  fi
}

rm -rf "$OUTPUT_ROOT"
mkdir -p "$OUTPUT_ROOT"

prepare_one 01-baseline "$ROOT_DIR/features/aibo-mind2/build/stick"
prepare_one 02-0400 "$ROOT_DIR/features/mind2-behavior-research/build/aibo-mind2-0400-specimen"
prepare_one 03-0400-plus-1200 "$ROOT_DIR/features/mind2-behavior-research/build/aibo-mind2-0400-1200-specimen"
prepare_one 04-0400-plus-ieg "$ROOT_DIR/features/mind2-behavior-research/build/aibo-mind2-0400-plus-ieg"

cat > "$OUTPUT_ROOT/MANIFEST.txt" <<EOF
Live hardware matrix preparation
Date: 2026-07-01
Injected lab WLAN: $INJECT_LAB_WLAN

Prepared trees:
- 01-baseline -> features/aibo-mind2/build/stick
- 02-0400 -> features/mind2-behavior-research/build/aibo-mind2-0400-specimen
- 03-0400-plus-1200 -> features/mind2-behavior-research/build/aibo-mind2-0400-1200-specimen
- 04-0400-plus-ieg -> features/mind2-behavior-research/build/aibo-mind2-0400-plus-ieg

Copy each tree to separate sacrificial media before testing.
EOF

echo "Prepared live hardware matrix trees at: $OUTPUT_ROOT"
echo "Manifest: $OUTPUT_ROOT/MANIFEST.txt"
