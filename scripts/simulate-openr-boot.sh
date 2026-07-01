#!/usr/bin/env bash

set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/simulate-openr-boot.sh <ms-root|sample-dir>

Simulate the OPEN-R/Aperios boot plan by reading OBJECT.CFG from a local
Memory Stick tree and checking whether each referenced object payload exists.

Accepted inputs:
  - a Memory Stick root containing OPEN-R/
  - a sample directory containing MS/OPEN-R/

This validates boot layout and object references only. It does not emulate the
Sony Aperios kernel, device drivers, or execute OPEN-R binaries.
EOF
}

if [ "${1:-}" = "" ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

INPUT=$1

if [ -d "$INPUT/OPEN-R" ]; then
  MS_ROOT=$INPUT
elif [ -d "$INPUT/MS/OPEN-R" ]; then
  MS_ROOT=$INPUT/MS
else
  echo "error: could not locate OPEN-R tree under $INPUT" >&2
  exit 1
fi

OBJECT_CFG="$MS_ROOT/OPEN-R/MW/CONF/OBJECT.CFG"
if [ ! -f "$OBJECT_CFG" ]; then
  echo "error: missing OBJECT.CFG at $OBJECT_CFG" >&2
  exit 1
fi

echo "OPEN-R boot plan simulator"
echo "MS root: $MS_ROOT"
echo "OBJECT.CFG: $OBJECT_CFG"
echo

OBJECT_SIZE=$(stat -c '%s' "$OBJECT_CFG")
if [ "$OBJECT_SIZE" -eq 0 ]; then
  echo "note: OBJECT.CFG is empty"
  echo "interpretation: this stick tree does not request any MW objects from a"
  echo "plain-text OBJECT.CFG at this layer."
  echo
  echo "Objects listed: 0"
  for aux in DESIGNDB.CFG ROBOTGW.CFG; do
    aux_path="$MS_ROOT/OPEN-R/MW/CONF/$aux"
    if [ -f "$aux_path" ]; then
      echo "aux: present $aux"
    fi
  done
  echo
  echo "result: no plain-text MW object load list to validate"
  exit 0
fi

if ! grep -aq '^/MS/' "$OBJECT_CFG"; then
  echo "note: OBJECT.CFG does not appear to be a plain-text /MS object list"
  echo "interpretation: this is likely a retail encoded/binary config file,"
  echo "which this simulator does not decode yet."
  echo
  for cfg in \
    "$MS_ROOT/OPEN-R/MW/CONF/OBJECT.CFG" \
    "$MS_ROOT/OPEN-R/SYSTEM/CONF/OBJECT.CFG"
  do
    if [ -f "$cfg" ]; then
      printf 'cfg: %s (%s bytes)\n' "$cfg" "$(stat -c '%s' "$cfg")"
    fi
  done
  echo
  echo "result: unsupported retail OBJECT.CFG format"
  echo "note: use this as a detection/sanity signal only, not as proof that the"
  echo "retail stick loads zero objects."
  exit 3
fi

missing=0
count=0

while IFS= read -r raw_line; do
  line=${raw_line%%#*}
  line=$(printf '%s' "$line" | sed 's/[[:space:]]*$//')
  [ -z "$line" ] && continue

  count=$((count + 1))

  if [[ "$line" != /MS/* ]]; then
    echo "warn: unsupported entry format: $line"
    continue
  fi

  local_path="$MS_ROOT${line#/MS}"
  if [ -f "$local_path" ]; then
    printf 'ok:      %s\n' "$line"
  else
    printf 'missing: %s\n' "$line"
    missing=$((missing + 1))
  fi
done < "$OBJECT_CFG"

echo
echo "Objects listed: $count"

for aux in DESIGNDB.CFG ROBOTGW.CFG; do
  aux_path="$MS_ROOT/OPEN-R/MW/CONF/$aux"
  if [ -f "$aux_path" ]; then
    echo "aux: present $aux"
  fi
done

echo
if [ "$missing" -eq 0 ]; then
  echo "result: boot layout is internally consistent"
  exit 0
fi

echo "result: $missing referenced object(s) are missing"
echo "note: this usually means the sample payload must be merged with a base OPEN-R stick image."
exit 2
