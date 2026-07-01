#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/compare-sttlog.sh <sttlog-a> <sttlog-b>

Show a compact diff for two MIND 2 STTLOG files.

Output includes:
  - unified diff
  - rows present in both files that differ
EOF
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

if [ "$#" -ne 2 ]; then
  usage
  exit 1
fi

A=$1
B=$2

for f in "$A" "$B"; do
  if [ ! -f "$f" ]; then
    echo "error: file not found: $f" >&2
    exit 1
  fi
done

echo "STTLOG diff"
echo "A: $A"
echo "B: $B"
echo

diff -u "$A" "$B" || true

echo
echo "Changed keyed rows:"

TMP_DIR=$(mktemp -d)
trap 'rm -rf "$TMP_DIR"' EXIT

awk '/^[0-9a-f][0-9a-f][0-9a-f][0-9a-f] /{key=$1; $1=""; sub(/^ /,""); print key "\t" $0}' "$A" | sort > "$TMP_DIR/a.tsv"
awk '/^[0-9a-f][0-9a-f][0-9a-f][0-9a-f] /{key=$1; $1=""; sub(/^ /,""); print key "\t" $0}' "$B" | sort > "$TMP_DIR/b.tsv"

join -t $'\t' -j 1 "$TMP_DIR/a.tsv" "$TMP_DIR/b.tsv" |
while IFS=$'\t' read -r key left right; do
  if [ "$left" != "$right" ]; then
    echo "[$key]"
    echo "  A: $left"
    echo "  B: $right"
  fi
done
