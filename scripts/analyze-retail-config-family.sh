#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/analyze-retail-config-family.sh <config-file> [<config-file> ...]

Summarize opaque Sony retail config files as 8-byte block families and report
shared leading blocks across the provided files.
EOF
}

if [ "$#" -eq 0 ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

python3 - "$@" <<'PY'
from pathlib import Path
import sys

paths = [Path(arg) for arg in sys.argv[1:]]
for path in paths:
    if not path.is_file():
        raise SystemExit(f"error: file not found: {path}")

all_blocks = {}

for path in paths:
    data = path.read_bytes()
    if len(data) % 8 != 0:
        print(f"error: {path} size {len(data)} is not divisible by 8", file=sys.stderr)
        raise SystemExit(1)

    first = data[:8].hex()
    last = data[-8:].hex()
    print(f"file: {path}")
    print(f"size: {len(data)}")
    print(f"8-byte-blocks: {len(data) // 8}")
    print(f"first8: {first}")
    print(f"last8:  {last}")
    print()

    for index in range(0, len(data), 8):
        block = data[index:index + 8].hex()
        all_blocks.setdefault(block, []).append((str(path), index // 8))

repeats = {block: refs for block, refs in all_blocks.items() if len(refs) > 1}

if not repeats:
    print("shared-8-byte-blocks: none")
    raise SystemExit(0)

print("shared-8-byte-blocks:")
for block, refs in sorted(repeats.items(), key=lambda item: (-len(item[1]), item[0])):
    print(f"- {block}")
    for path, index in refs:
        print(f"  {path} block {index}")
PY
