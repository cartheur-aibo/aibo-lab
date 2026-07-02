#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7-sdk-wconsole-tinyftpd.sh [/path/to/output-dir]

Prepare a pure OPEN-R SDK ERS-7 WCONSOLE/nomemprot stick using the local
TinyFTPD sample for over-the-air file transfer plus wireless-console access.
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
SAMPLE_DIR="$ROOT_DIR/samples/common/TinyFTPD"
POWERMON_DIR="$ROOT_DIR/samples/common/PowerMonitor"
FEATURE_DIR="$ROOT_DIR/features/ers7-sdk-wconsole-tinyftpd"
BUILD_ROOT="$FEATURE_DIR/build"
OUTPUT_DIR="${1:-$BUILD_ROOT/stick}"
WORK_ROOT="$BUILD_ROOT/work"
WORK_SAMPLE_DIR="$WORK_ROOT/TinyFTPD"
WORK_POWERMON_DIR="$WORK_ROOT/PowerMonitor"
SDK_BASE_DIR="$OPENRSDK_ROOT/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R"
COPY_NOTE_FILE="$BUILD_ROOT/COPY_TO_MS.txt"

AIBO_HOSTNAME="${AIBO_HOSTNAME:-AIBO}"
ESSID="${ESSID:-AIBONET}"
WEPENABLE="${WEPENABLE:-0}"
WEPKEY="${WEPKEY:-}"
APMODE="${APMODE:-1}"
CHANNEL="${CHANNEL:-3}"
USE_DHCP="${USE_DHCP:-1}"
ETHER_IP="${ETHER_IP:-}"
ETHER_NETMASK="${ETHER_NETMASK:-}"
IP_GATEWAY="${IP_GATEWAY:-}"
DNS_SERVER_1="${DNS_SERVER_1:-}"
DNS_DEFDNAME="${DNS_DEFDNAME:-}"

require_path() {
  if [ ! -e "$1" ]; then
    echo "error: missing required path: $1" >&2
    exit 1
  fi
}

require_path "$SDK_BASE_DIR"
require_path "$SAMPLE_DIR/Makefile"
require_path "$SAMPLE_DIR/MS/OPEN-R/MW/CONF/OBJECT.CFG"
require_path "$SAMPLE_DIR/MS/OPEN-R/MW/CONF/PASSWD"
require_path "$POWERMON_DIR"

case "$WEPENABLE" in
  0|1) ;;
  *)
    echo "error: WEPENABLE must be 0 or 1" >&2
    exit 1
    ;;
esac

case "$USE_DHCP" in
  0|1) ;;
  *)
    echo "error: USE_DHCP must be 0 or 1" >&2
    exit 1
    ;;
esac

if [ "${#AIBO_HOSTNAME}" -gt 8 ]; then
  echo "error: AIBO_HOSTNAME must be 8 characters or fewer" >&2
  exit 1
fi

if [ "$WEPENABLE" = "1" ] && [ -z "$WEPKEY" ]; then
  echo "error: WEPKEY is required when WEPENABLE=1" >&2
  exit 1
fi

if [ "$USE_DHCP" = "0" ]; then
  if [ -z "$ETHER_IP" ] || [ -z "$ETHER_NETMASK" ] || [ -z "$IP_GATEWAY" ]; then
    echo "error: ETHER_IP, ETHER_NETMASK, and IP_GATEWAY are required when USE_DHCP=0" >&2
    exit 1
  fi
fi

rm -rf "$OUTPUT_DIR" "$WORK_ROOT"
mkdir -p "$OUTPUT_DIR" "$WORK_ROOT"

cp -a "$SAMPLE_DIR" "$WORK_SAMPLE_DIR"
cp -a "$POWERMON_DIR" "$WORK_POWERMON_DIR"
chmod -R u+w "$WORK_SAMPLE_DIR" "$WORK_POWERMON_DIR"
cp -a "$SDK_BASE_DIR" "$OUTPUT_DIR/OPEN-R"
chmod -R u+w "$OUTPUT_DIR/OPEN-R"
: > "$OUTPUT_DIR/MEMSTICK.IND"

mkdir -p "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS"

make -C "$WORK_SAMPLE_DIR" install OPENRSDK_ROOT="$OPENRSDK_ROOT"

require_path "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/TINYFTPD.BIN"
require_path "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/POWERMON.BIN"

cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/." "$OUTPUT_DIR/OPEN-R/MW/OBJS/"
cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/CONF/OBJECT.CFG" \
  "$OUTPUT_DIR/OPEN-R/MW/CONF/OBJECT.CFG"
cp -a "$WORK_SAMPLE_DIR/MS/OPEN-R/MW/CONF/PASSWD" \
  "$OUTPUT_DIR/OPEN-R/MW/CONF/PASSWD"

cat > "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
HOSTNAME=$AIBO_HOSTNAME
ESSID=$ESSID
WEPENABLE=$WEPENABLE
EOF

if [ "$WEPENABLE" = "1" ]; then
  echo "WEPKEY=$WEPKEY" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi

cat >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
APMODE=$APMODE
CHANNEL=$CHANNEL
EOF

if [ "$USE_DHCP" = "1" ]; then
  echo "USE_DHCP=1" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
else
  cat >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
USE_DHCP=0
ETHER_IP=$ETHER_IP
ETHER_NETMASK=$ETHER_NETMASK
IP_GATEWAY=$IP_GATEWAY
EOF
fi

if [ -n "$DNS_SERVER_1" ]; then
  echo "DNS_SERVER_1=$DNS_SERVER_1" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi
if [ -n "$DNS_DEFDNAME" ]; then
  echo "DNS_DEFDNAME=$DNS_DEFDNAME" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi

cat > "$COPY_NOTE_FILE" <<EOF
ERS-7 SDK TinyFTPD copy notes

SDK base: $SDK_BASE_DIR
Sample: $SAMPLE_DIR
Stick staging dir: $OUTPUT_DIR

This is the AIBO-side WCONSOLE/nomemprot stick for TinyFTPD.

Copy these items to the root of the mounted Memory Stick:

- MEMSTICK.IND
- OPEN-R/

Example:

  rsync -a --delete "$OUTPUT_DIR"/ /path/to/mounted-stick/
  sync

After copying:

1. Eject/unmount the stick cleanly.
2. Insert the wireless LAN card and the stick into the ERS-7.
3. Boot the robot.
4. Connect to the wireless console:
   telnet AIBO_IP 59000
5. Test FTP:
   ftp AIBO_IP

Default sample credentials from PASSWD:

- user: anonymous
  pass: anonymous
  root: /MS/
- user: guest
  pass: any password
  root: /MS/OPEN-R/MW/
EOF

SIZE_BYTES="$(du -sb "$OUTPUT_DIR" | awk '{print $1}')"
SIZE_MIB="$(awk -v bytes="$SIZE_BYTES" 'BEGIN { printf "%.2f", bytes / (1024 * 1024) }')"

echo "Prepared ERS-7 SDK TinyFTPD stick at: $OUTPUT_DIR"
echo "SDK base: $SDK_BASE_DIR"
echo "Sample: $SAMPLE_DIR"
echo "Flavor: WCONSOLE/nomemprot"
echo "Wi-Fi: ESSID=$ESSID APMODE=$APMODE DHCP=$USE_DHCP WEP=$WEPENABLE"
echo "FTP: port 21, passive port 3000"
echo "Size: ${SIZE_MIB} MiB (${SIZE_BYTES} bytes)"
echo "Copy note: $COPY_NOTE_FILE"
