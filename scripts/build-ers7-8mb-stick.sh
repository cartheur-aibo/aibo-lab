#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: build-ers7-8mb-stick.sh /path/to/output-dir

Build a scratch ERS-7 programmable Memory Stick tree sized for an 8 MB stick,
using the SDK's WCONSOLE base plus the HelloWorld + PowerMonitor test payload.

Environment variables:
  OPENRSDK_ROOT   SDK root. Defaults to ./sdk/local/OPEN_R_SDK
  STICK_FLAVOR    WCONSOLE or WLAN. Default: WCONSOLE
  MEMPROT         memprot or nomemprot. Default: memprot
  AIBO_HOSTNAME   Default: AIBO
  ESSID           Default: AIBONET
  WEPENABLE       0 or 1. Default: 0
  WEPKEY          Optional. Used when WEPENABLE=1
  APMODE          Default: 1
  CHANNEL         Default: 3
  USE_DHCP        0 or 1. Default: 1
  ETHER_IP        Required when USE_DHCP=0
  ETHER_NETMASK   Required when USE_DHCP=0
  IP_GATEWAY      Required when USE_DHCP=0
  DNS_SERVER_1    Optional
  DNS_DEFDNAME    Optional
EOF
}

if [ "$#" -ne 1 ]; then
  usage
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUTPUT_DIR="$1"

OPENRSDK_ROOT="${OPENRSDK_ROOT:-$ROOT_DIR/sdk/local/OPEN_R_SDK}"
STICK_FLAVOR="${STICK_FLAVOR:-WCONSOLE}"
MEMPROT="${MEMPROT:-memprot}"
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

BASE_DIR="$OPENRSDK_ROOT/OPEN_R/MS_ERS7/$STICK_FLAVOR/$MEMPROT/OPEN-R"
HELLO_SAMPLE_DIR="$ROOT_DIR/samples/common/HelloWorld"

require_file() {
  if [ ! -e "$1" ]; then
    echo "error: missing required path: $1" >&2
    exit 1
  fi
}

require_file "$BASE_DIR"
require_file "$HELLO_SAMPLE_DIR/Makefile"

case "$STICK_FLAVOR" in
  WCONSOLE|WLAN) ;;
  *)
    echo "error: STICK_FLAVOR must be WCONSOLE or WLAN" >&2
    exit 1
    ;;
esac

case "$MEMPROT" in
  memprot|nomemprot) ;;
  *)
    echo "error: MEMPROT must be memprot or nomemprot" >&2
    exit 1
    ;;
esac

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

rm -rf "$OUTPUT_DIR"
mkdir -p "$OUTPUT_DIR"

cp -a "$BASE_DIR" "$OUTPUT_DIR/OPEN-R"
chmod -R u+w "$OUTPUT_DIR/OPEN-R"
: > "$OUTPUT_DIR/MEMSTICK.IND"

cat > "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
HOSTNAME=$AIBO_HOSTNAME
EOF

if [ "$USE_DHCP" = "0" ]; then
  cat >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
ETHER_IP=$ETHER_IP
ETHER_NETMASK=$ETHER_NETMASK
IP_GATEWAY=$IP_GATEWAY
EOF
fi

cat >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
ESSID=$ESSID
WEPENABLE=$WEPENABLE
EOF

if [ "$WEPENABLE" = "1" ]; then
  cat >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
WEPKEY=$WEPKEY
EOF
fi

cat >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT" <<EOF
APMODE=$APMODE
CHANNEL=$CHANNEL
EOF

if [ -n "$DNS_SERVER_1" ]; then
  echo "DNS_SERVER_1=$DNS_SERVER_1" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi

if [ -n "$DNS_DEFDNAME" ]; then
  echo "DNS_DEFDNAME=$DNS_DEFDNAME" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi

if [ "$USE_DHCP" = "1" ]; then
  echo "USE_DHCP=1" >> "$OUTPUT_DIR/OPEN-R/SYSTEM/CONF/WLANCONF.TXT"
fi

make -C "$HELLO_SAMPLE_DIR" install OPENRSDK_ROOT="$OPENRSDK_ROOT"

cp -a "$HELLO_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/HELLO.BIN" "$OUTPUT_DIR/OPEN-R/MW/OBJS/"
cp -a "$HELLO_SAMPLE_DIR/MS/OPEN-R/MW/OBJS/POWERMON.BIN" "$OUTPUT_DIR/OPEN-R/MW/OBJS/"
cp -a "$HELLO_SAMPLE_DIR/MS/OPEN-R/MW/CONF/OBJECT.CFG" "$OUTPUT_DIR/OPEN-R/MW/CONF/OBJECT.CFG"

SIZE_BYTES="$(du -sb "$OUTPUT_DIR" | awk '{print $1}')"
SIZE_MIB="$(awk -v bytes="$SIZE_BYTES" 'BEGIN { printf "%.2f", bytes / (1024 * 1024) }')"

echo "Built ERS-7 test stick at: $OUTPUT_DIR"
echo "Flavor: $STICK_FLAVOR/$MEMPROT"
echo "Wi-Fi: ESSID=$ESSID APMODE=$APMODE DHCP=$USE_DHCP WEP=$WEPENABLE"
echo "Payload: HelloWorld + PowerMonitor"
echo "Size: ${SIZE_MIB} MiB (${SIZE_BYTES} bytes)"
echo "Files to copy to the stick root:"
echo "  MEMSTICK.IND"
echo "  OPEN-R/"
