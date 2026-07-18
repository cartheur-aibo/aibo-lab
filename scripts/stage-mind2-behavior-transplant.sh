#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: stage-mind2-behavior-transplant.sh <neutral|sensitive> <specimen-image-or-tree> <baseline-image-or-tree> [output-dir]

Build a staged MIND 2 behavior-test tree by copying a baseline stick tree and
overlaying the current candidate persistent-state cluster from a specimen.

Modes:
  neutral    Overlay the candidate app/middleware state cluster only.
  sensitive  Overlay the same cluster and also overlay AIBO-ID.

Accepted specimen/baseline inputs:
  - a raw stick image that 7z can extract
  - an extracted tree containing OPEN-R/
  - a stick root tree containing OPEN-R/

Environment variables:
  INJECT_LAB_WLAN=1   Also replace OPEN-R/SYSTEM/CONF/WLANCONF.TXT with
                      out/ERS7M2/WLANCONF.TXT for lab testing.
EOF
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

if [ "$#" -lt 3 ] || [ "$#" -gt 4 ]; then
  usage
  exit 1
fi

MODE=$1
SPECIMEN_INPUT=$2
BASELINE_INPUT=$3
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUTPUT_DIR="${4:-$ROOT_DIR/features/mind2-behavior-research/build/$MODE-stick}"
INJECT_LAB_WLAN="${INJECT_LAB_WLAN:-0}"

case "$MODE" in
  neutral|sensitive) ;;
  *)
    echo "error: mode must be 'neutral' or 'sensitive'" >&2
    exit 1
    ;;
esac

if ! command -v 7z >/dev/null 2>&1; then
  echo "error: 7z is required but not installed" >&2
  exit 1
fi

TMP_BASE=$(mktemp -d)
trap 'rm -rf "$TMP_BASE"' EXIT

resolve_tree() {
  local input=$1
  local name=$2
  local out="$TMP_BASE/$name"

  if [ -d "$input/OPEN-R" ]; then
    printf '%s\n' "$input"
    return
  fi

  if [ -f "$input" ]; then
    mkdir -p "$out"
    7z x -y -o"$out" "$input" >/dev/null
    if [ -d "$out/OPEN-R" ]; then
      printf '%s\n' "$out"
      return
    fi
  fi

  echo "error: could not resolve OPEN-R tree from $input" >&2
  exit 1
}

SPECIMEN_TREE=$(resolve_tree "$SPECIMEN_INPUT" specimen)
BASELINE_TREE=$(resolve_tree "$BASELINE_INPUT" baseline)

require_path() {
  if [ ! -e "$1" ]; then
    echo "error: missing required path: $1" >&2
    exit 1
  fi
}

require_path "$SPECIMEN_TREE/OPEN-R/APP/DATA/P"
require_path "$SPECIMEN_TREE/OPEN-R/MW/DATA/P/PAT.LOG"
require_path "$SPECIMEN_TREE/OPEN-R/SYSTEM/DATA/P/AIBO-ID"
require_path "$BASELINE_TREE/OPEN-R"
require_path "$BASELINE_TREE/MEMSTICK.IND"

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"
cp -a "$BASELINE_TREE/." "$OUTPUT_DIR/"
chmod -R u+w "$OUTPUT_DIR"

overlay() {
  local rel=$1
  local src="$SPECIMEN_TREE/$rel"
  local dst="$OUTPUT_DIR/$rel"
  require_path "$src"
  mkdir -p "$(dirname "$dst")"
  cp -a "$src" "$dst"
}

for rel in \
  OPEN-R/APP/DATA/P/AWAKING.CFG \
  OPEN-R/APP/DATA/P/FVAR \
  OPEN-R/APP/DATA/P/GVAR \
  OPEN-R/APP/DATA/P/IEG.CFG \
  OPEN-R/APP/DATA/P/SIDRDATA.BIN \
  OPEN-R/APP/DATA/P/STTLOG \
  OPEN-R/MW/DATA/P/PAT.LOG
do
  overlay "$rel"
done

if [ "$MODE" = "sensitive" ]; then
  overlay OPEN-R/SYSTEM/DATA/P/AIBO-ID
fi

if [ "$INJECT_LAB_WLAN" = "1" ]; then
  require_path "$ROOT_DIR/out/ERS7M2/WLANCONF.TXT"
  cp -a "$ROOT_DIR/out/ERS7M2/WLANCONF.TXT" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi

MANIFEST="$OUTPUT_DIR/BEHAVIOR-TRANSPLANT.txt"
cat > "$MANIFEST" <<EOF
MIND 2 behavior transplant staging

Mode: $MODE
Specimen source: $SPECIMEN_INPUT
Baseline source: $BASELINE_INPUT
Resolved specimen tree: $SPECIMEN_TREE
Resolved baseline tree: $BASELINE_TREE
Injected lab WLAN: $INJECT_LAB_WLAN

Overlay set:
- OPEN-R/APP/DATA/P/AWAKING.CFG
- OPEN-R/APP/DATA/P/FVAR
- OPEN-R/APP/DATA/P/GVAR
- OPEN-R/APP/DATA/P/IEG.CFG
- OPEN-R/APP/DATA/P/SIDRDATA.BIN
- OPEN-R/APP/DATA/P/STTLOG
- OPEN-R/MW/DATA/P/PAT.LOG
EOF

if [ "$MODE" = "sensitive" ]; then
  cat >> "$MANIFEST" <<'EOF'
- OPEN-R/SYSTEM/DATA/P/AIBO-ID
EOF
fi

if [ "$INJECT_LAB_WLAN" = "1" ]; then
  cat >> "$MANIFEST" <<'EOF'

Lab WLAN override:
- OPEN-R/SYSTEM/CONF/WLANCONF.TXT from out/ERS7M2/WLANCONF.TXT
EOF
fi

echo "Prepared behavior transplant staging at: $OUTPUT_DIR"
echo "Mode: $MODE"
echo "Manifest: $MANIFEST"
echo "Next: copy this staged tree to sacrificial test media and log the trial."
