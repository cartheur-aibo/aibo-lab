#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: prepare-ers7m2-wconsole-sdk-system.sh [/path/to/output-dir]

Prepare an ERS-7 AIBO MIND 2 WCONSOLE comparison tree that uses the SDK
WCONSOLE SYSTEM lineage rather than the mixed preserved+SDK SYSTEM overlay.

This wrapper intentionally locks:
- SYSTEM_FLAVOR=WCONSOLE
- SYSTEM_SOURCE=sdk
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
FEATURE_DIR="$ROOT_DIR/features/ers7m2-wconsole-sdk-system"
OUTPUT_DIR="${1:-$FEATURE_DIR/build/stick}"

FEATURE_DIR="$FEATURE_DIR" SYSTEM_FLAVOR=WCONSOLE SYSTEM_SOURCE=sdk \
  STICK_LABEL="ERS-7 MIND 2 WCONSOLE SDK-system stick" \
  "$ROOT_DIR/scripts/prepare-ers7m2-wconsole.sh" "$OUTPUT_DIR"
