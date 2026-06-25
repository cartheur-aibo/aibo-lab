#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7-camera-stream.sh [/path/to/output-dir]

Build and stage the ERS-7 W3AIBO camera-stream sample as a programmable
Memory Stick tree under features/ers7-camera-stream/.

By default this:
- uses the ERS-7 SDK WCONSOLE/nomemprot base
- builds W3AIBO and PowerMonitor from samples/common/W3AIBO
- injects src/ERS7M2/WLANCONF.TXT when present
- writes the staged stick tree under features/ers7-camera-stream/build/stick

Environment variables:
  OPENRSDK_ROOT    SDK root. Default: ./sdk/local/OPEN_R_SDK
  SAMPLE_DIR       Sample root. Default: ./samples/common/W3AIBO
  JPEG_ARCHIVE     Path to jpegsrc.v6b.tar.gz
                   Default: ./samples/common/W3AIBO/jpegsrc.v6b.tar.gz
  WLANCONF_SOURCE  WLAN config to inject.
                   Default: ./src/ERS7M2/WLANCONF.TXT
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
SAMPLE_DIR="${SAMPLE_DIR:-$ROOT_DIR/samples/common/W3AIBO}"
JPEG_ARCHIVE="${JPEG_ARCHIVE:-$SAMPLE_DIR/jpegsrc.v6b.tar.gz}"
WLANCONF_SOURCE="${WLANCONF_SOURCE:-$ROOT_DIR/src/ERS7M2/WLANCONF.TXT}"
POWERMON_SOURCE_DIR="$ROOT_DIR/samples/common/PowerMonitor"

FEATURE_DIR="$ROOT_DIR/features/ers7-camera-stream"
BUILD_ROOT="$FEATURE_DIR/build"
OUTPUT_DIR="${1:-$BUILD_ROOT/stick}"
WORK_COMMON_DIR="$BUILD_ROOT/work/samples/common"
WORK_SAMPLE_DIR="$WORK_COMMON_DIR/W3AIBO"
WORK_POWERMON_DIR="$WORK_COMMON_DIR/PowerMonitor"
COPY_NOTE_FILE="$BUILD_ROOT/COPY_TO_MS.txt"
BASE_DIR="$OPENRSDK_ROOT/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R"

require_path() {
  if [ ! -e "$1" ]; then
    echo "error: missing required path: $1" >&2
    exit 1
  fi
}

require_path "$BASE_DIR"
require_path "$SAMPLE_DIR/Makefile"
require_path "$POWERMON_SOURCE_DIR/PowerMonitor/Makefile"
require_path "$SAMPLE_DIR/MS/OPEN-R/MW/CONF/OBJECT.CFG"
require_path "$SAMPLE_DIR/MS/OPEN-R/MW/CONF/CONNECT.CFG"
require_path "$JPEG_ARCHIVE"

rm -rf "$OUTPUT_DIR" "$WORK_COMMON_DIR"
mkdir -p "$OUTPUT_DIR" "$WORK_COMMON_DIR"

cp -a "$SAMPLE_DIR" "$WORK_SAMPLE_DIR"
cp -a "$POWERMON_SOURCE_DIR" "$WORK_POWERMON_DIR"
chmod -R u+w "$WORK_SAMPLE_DIR"
chmod -R u+w "$WORK_POWERMON_DIR"
mkdir -p "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS"
cp -f "$JPEG_ARCHIVE" "$WORK_SAMPLE_DIR/jpegsrc.v6b.tar.gz"

make -C "$WORK_SAMPLE_DIR" install OPENRSDK_ROOT="$OPENRSDK_ROOT"

require_path "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/W3AIBO.BIN"
require_path "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/POWERMON.BIN"

cp -a "$BASE_DIR" "$OUTPUT_DIR/OPEN-R"
chmod -R u+w "$OUTPUT_DIR/OPEN-R"
: > "$OUTPUT_DIR/MEMSTICK.IND"

cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/CONF/OBJECT.CFG" \
  "$OUTPUT_DIR/OPEN-R/MW/CONF/OBJECT.CFG"
cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/CONF/CONNECT.CFG" \
  "$OUTPUT_DIR/OPEN-R/MW/CONF/CONNECT.CFG"
cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/W3AIBO.BIN" \
  "$OUTPUT_DIR/OPEN-R/MW/OBJS/W3AIBO.BIN"
cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/POWERMON.BIN" \
  "$OUTPUT_DIR/OPEN-R/MW/OBJS/POWERMON.BIN"

if [ -e "$WLANCONF_SOURCE" ]; then
  cp -a "$WLANCONF_SOURCE" "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
  WLANCONF_STATUS="Injected WLANCONF: $WLANCONF_SOURCE"
else
  WLANCONF_STATUS="Injected WLANCONF: none (kept SDK default)"
fi

SIZE_BYTES="$(du -sb "$OUTPUT_DIR" | awk '{print $1}')"
SIZE_MIB="$(awk -v bytes="$SIZE_BYTES" 'BEGIN { printf "%.2f", bytes / (1024 * 1024) }')"

cat > "$COPY_NOTE_FILE" <<EOF
ERS-7 camera-stream copy notes

Feature slug: ers7-camera-stream
SDK base: $BASE_DIR
Sample source: $SAMPLE_DIR
JPEG archive: $JPEG_ARCHIVE
$WLANCONF_STATUS
Stick staging dir: $OUTPUT_DIR

Copy this staged tree to the root of a mounted programmable Sony Memory Stick:

- MEMSTICK.IND
- OPEN-R/

Example:

  rsync -a --delete "$OUTPUT_DIR"/ /path/to/mounted-stick/
  sync

After copying:

1. Eject/unmount the stick cleanly.
2. Insert it into the ERS-7 while powered off.
3. Boot the robot.
4. Join the same compatible Wi-Fi network from the host machine.
5. Probe these URLs:
   - http://AIBO_IP:60080/
   - http://AIBO_IP:60080/layerh
   - http://AIBO_IP:60080/layerhr
   - http://AIBO_IP:60080/layerm
   - http://AIBO_IP:60080/layermr
   - http://AIBO_IP:60080/layerl
   - http://AIBO_IP:60080/layerlr
EOF

echo "Prepared ERS-7 camera-stream stick at: $OUTPUT_DIR"
echo "Sample source: $SAMPLE_DIR"
echo "JPEG archive: $JPEG_ARCHIVE"
echo "$WLANCONF_STATUS"
echo "Size: ${SIZE_MIB} MiB (${SIZE_BYTES} bytes)"
echo "Copy note: $COPY_NOTE_FILE"
