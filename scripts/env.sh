#!/usr/bin/env bash

if [ -z "${OPENRSDK_ROOT:-}" ]; then
  if [ -d /usr/OPENR_SDK ]; then
    OPENRSDK_ROOT=/usr/OPENR_SDK
  elif [ -d /usr/local/OPEN_R_SDK ]; then
    OPENRSDK_ROOT=/usr/local/OPEN_R_SDK
  else
    OPENRSDK_ROOT=/usr/OPENR_SDK
  fi
fi

export OPENRSDK_ROOT
export PATH="$OPENRSDK_ROOT/bin:$PATH"
export OPENRSDK_LIB="$OPENRSDK_ROOT/lib"
export OPENRSDK_INCLUDE="$OPENRSDK_ROOT/include"

echo "OPENRSDK_ROOT=$OPENRSDK_ROOT"
