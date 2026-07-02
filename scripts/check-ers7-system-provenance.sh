#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/check-ers7-system-provenance.sh <baseline-ms-root> <candidate-ms-root> <sdk-openr-root>

Check the provenance of candidate ERS-7 SYSTEM objects referenced by plain-text
CARDDRV.CFG and EXTOBJ.CFG entries.

Accepted inputs for <baseline-ms-root> and <candidate-ms-root>:
  - a Memory Stick root containing OPEN-R/
  - a sample directory containing MS/OPEN-R/

Example SDK roots:
  sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WLAN/memprot/OPEN-R
  sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R
EOF
}

if [ "$#" -ne 3 ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit $([ "$#" -eq 3 ] && [ "${1:-}" != "-h" ] && [ "${1:-}" != "--help" ] && echo 0 || echo 1)
fi

resolve_ms_root() {
  local input=$1
  if [ -d "$input/OPEN-R" ]; then
    printf '%s\n' "$input"
  elif [ -d "$input/MS/OPEN-R" ]; then
    printf '%s\n' "$input/MS"
  else
    echo "error: could not locate OPEN-R tree under $input" >&2
    exit 1
  fi
}

BASELINE_ROOT=$(resolve_ms_root "$1")
CANDIDATE_ROOT=$(resolve_ms_root "$2")
SDK_OPENR_ROOT=$3

if [ ! -d "$SDK_OPENR_ROOT/SYSTEM/OBJS" ]; then
  echo "error: missing SDK SYSTEM/OBJS under $SDK_OPENR_ROOT" >&2
  exit 1
fi

python3 - "$BASELINE_ROOT" "$CANDIDATE_ROOT" "$SDK_OPENR_ROOT" <<'PY'
from pathlib import Path
import hashlib
import sys

baseline_root = Path(sys.argv[1])
candidate_root = Path(sys.argv[2])
sdk_openr_root = Path(sys.argv[3])

def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()

def read_ms_entries(path: Path):
    if not path.is_file():
        return []
    out = []
    for raw in path.read_bytes().splitlines():
        if raw.startswith(b"/MS/"):
            out.append(raw.decode("latin1").split()[0])
    return out

def classify(candidate_obj: Path, baseline_obj: Path, sdk_obj: Path):
    if not candidate_obj.is_file():
        return "missing-candidate"
    cand_hash = sha256(candidate_obj)
    baseline_hash = sha256(baseline_obj) if baseline_obj.is_file() else None
    sdk_hash = sha256(sdk_obj) if sdk_obj.is_file() else None

    if baseline_hash and cand_hash == baseline_hash and sdk_hash and cand_hash == sdk_hash:
        return "same-as-both"
    if baseline_hash and cand_hash == baseline_hash:
        return "preserved-baseline"
    if sdk_hash and cand_hash == sdk_hash:
        return "sdk-overlay"
    return "unknown-mixed"

checks = [
    ("CARDDRV.CFG", candidate_root / "OPEN-R/SYSTEM/CONF/CARDDRV.CFG"),
    ("EXTOBJ.CFG", candidate_root / "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG"),
]

print("ERS-7 SYSTEM provenance check")
print(f"baseline:  {baseline_root}")
print(f"candidate: {candidate_root}")
print(f"sdk-root:  {sdk_openr_root}")
print()

seen = set()
for label, cfg_path in checks:
    entries = read_ms_entries(cfg_path)
    print(f"{label}:")
    if not entries:
        print("  no plain-text /MS entries found")
        continue
    for entry in entries:
        rel = entry.removeprefix("/MS/")
        candidate_obj = candidate_root / rel
        baseline_obj = baseline_root / rel
        sdk_obj = sdk_openr_root / rel.removeprefix("OPEN-R/")
        status = classify(candidate_obj, baseline_obj, sdk_obj)
        print(f"  {entry}")
        print(f"    provenance: {status}")
        if candidate_obj.is_file():
            print(f"    candidate-sha256: {sha256(candidate_obj)}")
        if baseline_obj.is_file():
            print(f"    baseline-sha256:  {sha256(baseline_obj)}")
        else:
            print("    baseline-sha256:  missing")
        if sdk_obj.is_file():
            print(f"    sdk-sha256:       {sha256(sdk_obj)}")
        else:
            print("    sdk-sha256:       missing")
        seen.add(rel)
    print()

candidate_objs = sorted(p.name for p in (candidate_root / "OPEN-R/SYSTEM/OBJS").glob("*.BIN"))
extra_not_referenced = []
for name in candidate_objs:
    rel = f"OPEN-R/SYSTEM/OBJS/{name}"
    if rel not in seen:
        extra_not_referenced.append(name)

print("candidate SYSTEM objects not referenced by plain-text CARDDRV/EXTOBJ:")
if extra_not_referenced:
    for name in extra_not_referenced:
        print(f"  - {name}")
else:
    print("  none")
PY
