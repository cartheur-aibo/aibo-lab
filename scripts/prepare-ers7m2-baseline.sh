#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7m2-baseline.sh [/path/to/output-dir]

Prepare a preserved-only ERS-7 AIBO MIND 2 baseline stick staging tree for
first hardware bring-up on larger Sony Memory Sticks such as 32 MB or 64 MB
media.

This wrapper intentionally locks the system flavor to:
- preserved

It keeps the known-good MIND 2 runtime and app tree intact and only injects the
repo WLAN configuration for lab testing.

Environment variables:
  SOURCE_STICK_DIR  Source staged stick tree.
                    Default: ./features/aibo-mind2/build/stick
  WLANCONF_SOURCE   WLAN config to inject.
                    Default: ./out/ERS7M2/WLANCONF.TXT
EOF
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

if [ "$#" -gt 1 ]; then
  usage
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE_STICK_DIR="${SOURCE_STICK_DIR:-$ROOT_DIR/features/aibo-mind2/build/stick}"
WLANCONF_SOURCE="${WLANCONF_SOURCE:-$ROOT_DIR/out/ERS7M2/WLANCONF.TXT}"
TARGET_MOUNT="${TARGET_MOUNT:-/media/$USER/YOUR_STICK_MOUNT}"
FEATURE_DIR="$ROOT_DIR/features/ers7m2-baseline"
OUTPUT_DIR="${1:-$FEATURE_DIR/build/stick}"
COPY_NOTE_FILE="$FEATURE_DIR/build/COPY_TO_MS.txt"

require_path() {
  if [ ! -e "$1" ]; then
    echo "error: missing required path: $1" >&2
    exit 1
  fi
}

require_path "$SOURCE_STICK_DIR/OPEN-R"
require_path "$SOURCE_STICK_DIR/MEMSTICK.IND"
require_path "$WLANCONF_SOURCE"

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

cp -a "$SOURCE_STICK_DIR/." "$OUTPUT_DIR/"
chmod -R u+w "$OUTPUT_DIR"

# Keep only the ERS-7 boot payload we actively deploy.
rm -rf "$OUTPUT_DIR/PALM"

cp -a "$WLANCONF_SOURCE" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
mkdir -p "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/SAMPLE"
cp -a "$WLANCONF_SOURCE" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/SAMPLE/WLANCONF.TXT"

SIZE_BYTES="$(du -sb "$OUTPUT_DIR" | awk '{print $1}')"
SIZE_MIB="$(awk -v bytes="$SIZE_BYTES" 'BEGIN { printf "%.2f", bytes / (1024 * 1024) }')"

cat > "$COPY_NOTE_FILE" <<EOF
ERS-7 MIND 2 baseline stick copy notes

Source stick dir: $SOURCE_STICK_DIR
Injected WLANCONF: $WLANCONF_SOURCE
Stick staging dir: $OUTPUT_DIR

Active WLAN settings in this staged tree:

- HOSTNAME=AIBOM2A
- ESSID=AIBONET
- WEPENABLE=0
- APMODE=1
- USE_DHCP=1

Copy the required boot payload to the root of a mounted Sony Memory Stick:

- MEMSTICK.IND
- OPEN-R/

Example:

  rsync -a --delete "$OUTPUT_DIR"/ "$TARGET_MOUNT"/
  sync
EOF

echo "Prepared ERS-7 MIND 2 baseline stick at: $OUTPUT_DIR"
echo "Source stick dir: $SOURCE_STICK_DIR"
echo "Injected WLANCONF: $WLANCONF_SOURCE"
echo "Size: ${SIZE_MIB} MiB (${SIZE_BYTES} bytes)"
echo "Copy note: $COPY_NOTE_FILE"
