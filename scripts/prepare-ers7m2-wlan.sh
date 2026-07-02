#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7m2-wlan.sh [/path/to/output-dir]

Prepare an ERS-7 AIBO MIND 2 WLAN stick staging tree for a narrower wireless
overlay than full WCONSOLE.

This wrapper intentionally locks the system flavor to:
- WLAN

It keeps the known-good MIND 2 runtime and app tree intact, injects the repo
WLAN configuration for lab testing, and adds only the SDK WLAN system overlay.
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
FEATURE_DIR="$ROOT_DIR/features/ers7m2-wlan"
OUTPUT_DIR="${1:-$FEATURE_DIR/build/stick}"

FEATURE_DIR="$FEATURE_DIR" SYSTEM_FLAVOR=WLAN \
  STICK_LABEL="ERS-7 MIND 2 WLAN stick" \
  "$ROOT_DIR/scripts/prepare-ers7m2-wconsole.sh" "$OUTPUT_DIR"
