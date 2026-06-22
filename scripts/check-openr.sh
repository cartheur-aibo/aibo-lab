#!/usr/bin/env bash
set -euo pipefail

: "${OPENRSDK_ROOT:=/usr/OPENR_SDK}"

missing=0

check_path() {
  local p="$1"
  if [ -e "$p" ]; then
    echo "ok: $p"
  else
    echo "missing: $p"
    missing=1
  fi
}

check_path "$OPENRSDK_ROOT"
check_path "$OPENRSDK_ROOT/include"
check_path "$OPENRSDK_ROOT/lib"
check_path "$OPENRSDK_ROOT/bin"

if [ "$missing" -ne 0 ]; then
  echo "OPEN-R SDK path is incomplete. Install/configure SDK, then rerun."
  exit 1
fi

echo "OPEN-R SDK skeleton looks present."
