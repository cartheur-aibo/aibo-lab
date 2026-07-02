#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7m2-test-stick.sh [/path/to/output-dir]

Prepare a full ERS-7 AIBO MIND 2 test-stick staging tree for larger Sony
Memory Sticks such as 32 MB or 64 MB media.

By default this:
- copies the preserved known-good MIND 2 stick tree from
  features/aibo-mind2/build/stick
- replaces OPEN-R/SYSTEM/CONF/WLANCONF.TXT with src/ERS7M2/WLANCONF.TXT
- writes the result under features/ers7m2-test-stick/build/stick

Environment variables:
  OPENRSDK_ROOT    SDK root.
                   Default: ./sdk/local/OPEN_R_SDK
  SOURCE_STICK_DIR  Source staged stick tree.
                    Default: ./features/aibo-mind2/build/stick
  WLANCONF_SOURCE   WLAN config to inject.
                    Default: ./src/ERS7M2/WLANCONF.TXT
  SYSTEM_FLAVOR     preserved, WLAN, or WCONSOLE.
                    Default: preserved
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
OPENRSDK_ROOT="${OPENRSDK_ROOT:-$ROOT_DIR/sdk/local/OPEN_R_SDK}"
SOURCE_STICK_DIR="${SOURCE_STICK_DIR:-$ROOT_DIR/features/aibo-mind2/build/stick}"
WLANCONF_SOURCE="${WLANCONF_SOURCE:-$ROOT_DIR/src/ERS7M2/WLANCONF.TXT}"
SYSTEM_FLAVOR="${SYSTEM_FLAVOR:-preserved}"
TARGET_MOUNT="${TARGET_MOUNT:-/media/$USER/disk}"
FEATURE_DIR="$ROOT_DIR/features/ers7m2-test-stick"
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

case "$SYSTEM_FLAVOR" in
  preserved|WLAN|WCONSOLE) ;;
  *)
    echo "error: SYSTEM_FLAVOR must be preserved, WLAN, or WCONSOLE" >&2
    exit 1
    ;;
esac

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

cp -a "$SOURCE_STICK_DIR/." "$OUTPUT_DIR/"
chmod -R u+w "$OUTPUT_DIR"

if [ "$SYSTEM_FLAVOR" != "preserved" ]; then
  FLAVOR_DIR="$OPENRSDK_ROOT/OPEN_R/MS_ERS7/$SYSTEM_FLAVOR/memprot/OPEN-R"
  require_path "$FLAVOR_DIR/SYSTEM/CONF/CARDDRV.CFG"
  require_path "$FLAVOR_DIR/SYSTEM/CONF/EXTOBJ.CFG"

  mkdir -p "$OUTPUT_DIR/OPEN-R/SYSTEM/OBJS" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF"

  # Keep the known-good MIND 2 runtime as the base and add only the SDK's
  # flavor-specific system extras and config files needed to enable WLAN or
  # WCONSOLE services.
  while IFS= read -r relpath; do
    cp -a "$FLAVOR_DIR/SYSTEM/OBJS/$relpath" "$OUTPUT_DIR/OPEN-R/SYSTEM/OBJS/$relpath"
  done <<'EOF'
ANTTCPIO.BIN
EMGCYMON.BIN
HOOK.BIN
HOOKACT.BIN
WLANDRVM.BIN
EOF

  cp -a "$FLAVOR_DIR/SYSTEM/CONF/CARDDRV.CFG" \
    "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/CARDDRV.CFG"
  cp -a "$FLAVOR_DIR/SYSTEM/CONF/EXTOBJ.CFG" \
    "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/EXTOBJ.CFG"

  if [ -f "$FLAVOR_DIR/SYSTEM/CONF/WLANDFLT.TXT" ]; then
    cp -a "$FLAVOR_DIR/SYSTEM/CONF/WLANDFLT.TXT" \
      "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANDFLT.TXT"
  fi
fi

cp -a "$WLANCONF_SOURCE" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
mkdir -p "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/SAMPLE"
cp -a "$WLANCONF_SOURCE" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/SAMPLE/WLANCONF.TXT"

SIZE_BYTES="$(du -sb "$OUTPUT_DIR" | awk '{print $1}')"
SIZE_MIB="$(awk -v bytes="$SIZE_BYTES" 'BEGIN { printf "%.2f", bytes / (1024 * 1024) }')"

cat > "$COPY_NOTE_FILE" <<EOF
ERS-7 MIND 2 full-stick copy notes

Source stick dir: $SOURCE_STICK_DIR
Injected WLANCONF: $WLANCONF_SOURCE
System flavor: $SYSTEM_FLAVOR
Stick staging dir: $OUTPUT_DIR

Active WLAN settings in this staged tree:

- HOSTNAME=AIBOM2A
- ESSID=AIBONET
- WEPENABLE=0
- APMODE=1
- USE_DHCP=1

Copy the required boot payload to the root of a mounted larger Sony Memory
Stick:

- MEMSTICK.IND
- OPEN-R/

Not needed for this workflow:

- PALM/
- StikZap.prc

Example:

  rsync -a --delete "$OUTPUT_DIR"/ "$TARGET_MOUNT"/
  sync

After copying:

1. Eject/unmount the stick cleanly.
2. Insert it into the ERS-7 while powered off.
3. Boot the robot.
4. Join Aibonet from the host machine.
5. Confirm the robot joins ESSID AIBONET as HOSTNAME AIBOM2A.
6. Probe the robot over Wi-Fi.
7. If built with SYSTEM_FLAVOR=WCONSOLE, try:
   telnet AIBO_IP 59000
EOF

echo "Prepared ERS-7 MIND 2 test stick at: $OUTPUT_DIR"
echo "Source stick dir: $SOURCE_STICK_DIR"
echo "Injected WLANCONF: $WLANCONF_SOURCE"
echo "System flavor: $SYSTEM_FLAVOR"
echo "Size: ${SIZE_MIB} MiB (${SIZE_BYTES} bytes)"
echo "Copy note: $COPY_NOTE_FILE"
