#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/analyze-retail-systemcfg.sh <mind2-retail-cfg> <mind3-retail-cfg> [<sdk-cfg> ...]

Analyze retail ERS-7 SYSTEM config files such as:

- OPEN-R/SYSTEM/CONF/CARDDRV.CFG
- OPEN-R/SYSTEM/CONF/EXTOBJ.CFG

The tool treats each retail file as a sequence of aligned 24-byte records and:

- reports record counts and hashes
- compares MIND 2 and MIND 3 record stability by position
- shows record-level hex and u32 views
- compares retail record counts against any supplied SDK plain-text /MS lists
EOF
}

if [ "$#" -lt 2 ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit $([ "$#" -ge 2 ] && [ "${1:-}" != "-h" ] && [ "${1:-}" != "--help" ] && echo 0 || echo 1)
fi

python3 - "$@" <<'PY'
from pathlib import Path
import hashlib
import struct
import sys

m2_path = Path(sys.argv[1])
m3_path = Path(sys.argv[2])
sdk_paths = [Path(arg) for arg in sys.argv[3:]]

for path in [m2_path, m3_path, *sdk_paths]:
    if not path.is_file():
        raise SystemExit(f"error: file not found: {path}")

def load_records(path: Path):
    data = path.read_bytes()
    if len(data) % 24 != 0:
        raise SystemExit(f"error: file size is not divisible by 24 bytes: {path}")
    records = [data[i:i + 24] for i in range(0, len(data), 24)]
    return data, records

def sdk_entries(path: Path):
    entries = []
    with path.open("rb") as fh:
        for raw in fh:
            try:
                line = raw.decode("utf-8")
            except UnicodeDecodeError:
                line = raw.decode("latin1", errors="ignore")
            if line.startswith("/MS/"):
                entries.append(line.split()[0])
    return entries

m2_data, m2_records = load_records(m2_path)
m3_data, m3_records = load_records(m3_path)

if len(m2_records) != len(m3_records):
    raise SystemExit("error: retail record counts differ; expected matching MIND 2/MIND 3 shapes")

stable = []
versioned = []
for index, (m2_rec, m3_rec) in enumerate(zip(m2_records, m3_records)):
    entry = {
        "index": index,
        "m2": m2_rec,
        "m3": m3_rec,
        "m2_u32le": struct.unpack("<6I", m2_rec),
        "m3_u32le": struct.unpack("<6I", m3_rec),
    }
    if m2_rec == m3_rec:
        stable.append(entry)
    else:
        versioned.append(entry)

def spans(entries):
    if not entries:
        return []
    indexes = [entry["index"] for entry in entries]
    out = []
    start = prev = indexes[0]
    for idx in indexes[1:]:
        if idx == prev + 1:
            prev = idx
            continue
        out.append((start, prev))
        start = prev = idx
    out.append((start, prev))
    return out

def print_file_summary(label: str, path: Path, data: bytes, records):
    print(f"{label}: {path}")
    print(f"  size: {len(data)}")
    print(f"  sha256: {hashlib.sha256(data).hexdigest()}")
    print(f"  first8: {data[:8].hex()}")
    print(f"  last8:  {data[-8:].hex()}")
    print(f"  records-24-byte: {len(records)}")

print("Retail SYSTEM config analysis")
print_file_summary("M2", m2_path, m2_data, m2_records)
print_file_summary("M3", m3_path, m3_data, m3_records)
print()

print(f"stable-records: {len(stable)}")
for start, end in spans(stable):
    print(f"- indexes {start}-{end} (count {end - start + 1})")
print()

print(f"versioned-records: {len(versioned)}")
for start, end in spans(versioned):
    print(f"- indexes {start}-{end} (count {end - start + 1})")
print()

shared_anywhere = len(set(m2_records) & set(m3_records))
print(f"shared-full-records-anywhere: {shared_anywhere}")
print()

print("record-table:")
for idx, (m2_rec, m3_rec) in enumerate(zip(m2_records, m3_records)):
    status = "stable" if m2_rec == m3_rec else "versioned"
    print(f"record {idx:02d}: {status}")
    print(f"  m2-hex24: {m2_rec.hex()}")
    print(f"  m3-hex24: {m3_rec.hex()}")
    print(f"  m2-u32le: {' '.join(str(v) for v in struct.unpack('<6I', m2_rec))}")
    print(f"  m3-u32le: {' '.join(str(v) for v in struct.unpack('<6I', m3_rec))}")

if sdk_paths:
    print()
    print("sdk-plain-list-comparison:")
    retail_count = len(m2_records)
    for path in sdk_paths:
      entries = sdk_entries(path)
      print(f"- {path}")
      print(f"  sdk-entries: {len(entries)}")
      print(f"  retail-records-minus-sdk-entries: {retail_count - len(entries)}")
      for entry in entries:
          print(f"  entry: {entry}")
PY
