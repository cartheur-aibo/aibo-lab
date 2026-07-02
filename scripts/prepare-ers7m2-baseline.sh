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
FEATURE_DIR="$ROOT_DIR/features/ers7m2-baseline"
OUTPUT_DIR="${1:-$FEATURE_DIR/build/stick}"

FEATURE_DIR="$FEATURE_DIR" SYSTEM_FLAVOR=preserved \
  STICK_LABEL="ERS-7 MIND 2 baseline stick" \
  "$ROOT_DIR/scripts/prepare-ers7m2-wconsole.sh" "$OUTPUT_DIR"
