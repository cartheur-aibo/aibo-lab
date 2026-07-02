#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7-sdk-wconsole-helloworld.sh [/path/to/output-dir]

Prepare a pure OPEN-R SDK ERS-7 WCONSOLE stick using the local HelloWorld
sample and the SDK's WCONSOLE/memprot base.

This follows the official Aperios/Open-R document flow:
- build the sample
- copy the SDK WCONSOLE OPEN-R base
- copy the sample's MS/OPEN-R overlay
- edit WLANCONF.TXT
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
FEATURE_DIR="$ROOT_DIR/features/ers7-sdk-wconsole-helloworld"
OUTPUT_DIR="${1:-$FEATURE_DIR/build/stick}"

FEATURE_SLUG="ers7-sdk-wconsole-helloworld" \
  SAMPLE_DIR="$ROOT_DIR/samples/common/HelloWorld" \
  PAYLOAD_MODE=hello-only \
  SYSTEM_SOURCE=sdk \
  STICK_FLAVOR=WCONSOLE \
  MEMPROT=memprot \
  AIBO_HOSTNAME="${AIBO_HOSTNAME:-AIBO}" \
  ESSID="${ESSID:-AIBONET}" \
  WEPENABLE="${WEPENABLE:-0}" \
  WEPKEY="${WEPKEY:-}" \
  APMODE="${APMODE:-1}" \
  CHANNEL="${CHANNEL:-3}" \
  USE_DHCP="${USE_DHCP:-1}" \
  ETHER_IP="${ETHER_IP:-}" \
  ETHER_NETMASK="${ETHER_NETMASK:-}" \
  IP_GATEWAY="${IP_GATEWAY:-}" \
  DNS_SERVER_1="${DNS_SERVER_1:-}" \
  DNS_DEFDNAME="${DNS_DEFDNAME:-}" \
  "$ROOT_DIR/scripts/build-ers7-feature-stick.sh" "$OUTPUT_DIR"
