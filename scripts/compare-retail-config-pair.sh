#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/compare-retail-config-pair.sh <file-a> <file-b>

Compare two opaque Sony retail config files as sequences of 8-byte blocks and
report same-position equal spans, stable prefix/suffix runs, and any shared
blocks.
EOF
}

if [ "$#" -ne 2 ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit $([ "$#" -eq 2 ] && echo 0 || echo 1)
fi

python3 - "$1" "$2" <<'PY'
from pathlib import Path
import sys

a_path = Path(sys.argv[1])
b_path = Path(sys.argv[2])

for path in (a_path, b_path):
    if not path.is_file():
        raise SystemExit(f"error: file not found: {path}")

a = a_path.read_bytes()
b = b_path.read_bytes()

if len(a) % 8 != 0 or len(b) % 8 != 0:
    raise SystemExit("error: both files must be divisible by 8 bytes")

a_blocks = [a[i:i + 8] for i in range(0, len(a), 8)]
b_blocks = [b[i:i + 8] for i in range(0, len(b), 8)]

print(f"A: {a_path}")
print(f"B: {b_path}")
print(f"A blocks: {len(a_blocks)}")
print(f"B blocks: {len(b_blocks)}")
print(f"A first8: {a_blocks[0].hex()}")
print(f"B first8: {b_blocks[0].hex()}")
print(f"A last8:  {a_blocks[-1].hex()}")
print(f"B last8:  {b_blocks[-1].hex()}")
print()

same_positions = []
for index, (ab, bb) in enumerate(zip(a_blocks, b_blocks)):
    same_positions.append(ab == bb)

spans = []
start = None
for index, same in enumerate(same_positions):
    if same and start is None:
        start = index
    if not same and start is not None:
        spans.append((start, index - 1))
        start = None
if start is not None:
    spans.append((start, len(same_positions) - 1))

shared_any = sorted(set(a_blocks) & set(b_blocks))

prefix_blocks = 0
for ab, bb in zip(a_blocks, b_blocks):
    if ab != bb:
        break
    prefix_blocks += 1

suffix_blocks = 0
for ab, bb in zip(reversed(a_blocks), reversed(b_blocks)):
    if ab != bb:
        break
    suffix_blocks += 1

print(f"same-position-equal-spans: {len(spans)}")
for start, end in spans:
    print(
        f"- blocks {start}-{end} "
        f"(bytes {start * 8}-{((end + 1) * 8) - 1}, count {end - start + 1})"
    )

print()
print(f"shared-leading-blocks: {prefix_blocks}")
print(f"shared-trailing-blocks: {suffix_blocks}")
if suffix_blocks:
    suffix_bytes = suffix_blocks * 8
    suffix_start = len(a) - suffix_bytes
    print(
        f"stable-suffix: bytes {suffix_start}-{len(a) - 1} "
        f"({suffix_bytes} bytes)"
    )
    if suffix_bytes % 24 == 0:
        print(f"stable-suffix-24-byte-records: {suffix_bytes // 24}")

print()
print(f"shared-8-byte-blocks-anywhere: {len(shared_any)}")
for block in shared_any[:20]:
    print(f"- {block.hex()}")
if len(shared_any) > 20:
    print(f"- ... {len(shared_any) - 20} more")
PY
