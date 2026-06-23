#!/usr/bin/env bash
set -euo pipefail

if [ -z "${OPENRSDK_ROOT:-}" ]; then
  if [ -d /usr/OPENR_SDK ]; then
    OPENRSDK_ROOT=/usr/OPENR_SDK
  elif [ -d /usr/local/OPEN_R_SDK ]; then
    OPENRSDK_ROOT=/usr/local/OPEN_R_SDK
  else
    OPENRSDK_ROOT=/usr/OPENR_SDK
  fi
fi

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
check_path "$OPENRSDK_ROOT/bin"

if [ -d "$OPENRSDK_ROOT/OPEN_R" ]; then
  check_path "$OPENRSDK_ROOT/OPEN_R/include"
  check_path "$OPENRSDK_ROOT/OPEN_R/bin"
else
  check_path "$OPENRSDK_ROOT/include"
  check_path "$OPENRSDK_ROOT/lib"
fi

check_path "$OPENRSDK_ROOT/bin/mipsel-linux-g++"
check_path "$OPENRSDK_ROOT/bin/mipsel-linux-strip"

if [ -d "$OPENRSDK_ROOT/OPEN_R/bin" ]; then
  check_path "$OPENRSDK_ROOT/OPEN_R/bin/mkbin"
fi

if [ "$missing" -ne 0 ]; then
  echo "OPEN-R SDK path is incomplete. Install/configure SDK, then rerun."
  exit 1
fi

echo "OPEN-R SDK skeleton looks present."
