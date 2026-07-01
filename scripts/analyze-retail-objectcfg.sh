#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/analyze-retail-objectcfg.sh <object-cfg-file>

Summarize a retail/encoded OBJECT.CFG file by size, hash, and known matches
from the preserved ERS-7 MIND 2 and MIND 3 baselines.
EOF
}

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

if [ "$#" -ne 1 ]; then
  usage
  exit 1
fi

CFG=$1

if [ ! -f "$CFG" ]; then
  echo "error: file not found: $CFG" >&2
  exit 1
fi

size=$(stat -c '%s' "$CFG")
hash=$(sha256sum "$CFG" | awk '{print $1}')

m2_mw=420ab75f0da638a6f0c4d0e6bc70831638318fe1be9012135d30c8f3cda12d9a
m2_sys=826c0ef9112e6f44dfaf9fd0899757e61cc33ff0674503939699c4cdd6e20372
m3_mw=41aec416ae1782f7582f6313477c7333063565fb3e581bcd719146729cc124db
m3_sys=70b214e53dee01185365efed2c17671d114689b916d6a119016e3958cd7ad9c1

echo "Retail OBJECT.CFG summary"
echo "path: $CFG"
echo "size: $size"
echo "sha256: $hash"
echo

case "$hash" in
  "$m2_mw")
    echo "match: ERS-7 MIND 2 MW OBJECT.CFG baseline"
    ;;
  "$m2_sys")
    echo "match: ERS-7 MIND 2 SYSTEM OBJECT.CFG baseline"
    ;;
  "$m3_mw")
    echo "match: ERS-7 MIND 3 MW OBJECT.CFG baseline"
    ;;
  "$m3_sys")
    echo "match: ERS-7 MIND 3 SYSTEM OBJECT.CFG baseline"
    ;;
  *)
    echo "match: no known preserved retail OBJECT.CFG baseline"
    ;;
esac

zero_bytes=$(od -An -tu1 -v "$CFG" | tr ' ' '\n' | awk 'NF { if ($1 == 0) count++ } END { print count + 0 }')
string_count=$(strings -a "$CFG" | wc -l)
ms_refs=$(grep -a -c '/MS/' "$CFG" || true)
path_like=$(strings -a "$CFG" | grep -E -c 'OPEN-R|\\.BIN|/MS/' || true)

echo "zero-bytes: $zero_bytes"
echo "printable-strings: $string_count"
echo "embedded-/MS/-refs: $ms_refs"
echo "path-like-strings: $path_like"

for width in 24 12 8; do
  if [ $((size % width)) -eq 0 ]; then
    echo "candidate-record-width: $width bytes ($((size / width)) records)"
  fi
done

echo "8-byte-blocks: $((size / 8))"

echo
echo "first-bytes:"
xxd -g 1 "$CFG" | sed -n '1,4p'

echo
if [ "$string_count" -le 6 ] && [ "$path_like" -eq 0 ] && [ "$ms_refs" -eq 0 ]; then
  echo "characterization: dense binary blob with little or no embedded object-path text"
else
  echo "characterization: contains some printable material, but not a plain-text object list"
fi

if [ $((size % 8)) -eq 0 ]; then
  echo "hypothesis: consistent with a wider 8-byte-aligned retail binary-config family"
fi
