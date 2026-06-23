#!/usr/bin/env bash
set -euo pipefail

if [ "$#" -ne 2 ]; then
  echo "usage: $0 /path/to/mounted/aibo-stick /path/to/object-binary"
  exit 1
fi

STICK="$1"
OBJ="$2"

if [ ! -d "$STICK/OPEN-R" ]; then
  echo "error: $STICK does not look like an OPEN-R Memory Stick mount"
  exit 1
fi

if [ ! -f "$OBJ" ]; then
  echo "error: object binary not found: $OBJ"
  exit 1
fi

mkdir -p "$STICK/OPEN-R/MW/OBJS"
cp -v "$OBJ" "$STICK/OPEN-R/MW/OBJS/"
sync

echo "Deployed $(basename "$OBJ") to $STICK/OPEN-R/MW/OBJS/"
echo "Now unmount/eject the stick cleanly before inserting it into AIBO."
