#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/validate-ers7m2-candidate.sh <baseline-ms-root> <candidate-ms-root>

Conservative simulator/preflight gate for the current experimental ERS-7 M2.

This validator combines:
- boot-layout simulation for plain-text MW object lists
- static drift/risk auditing against a known-good baseline
- runtime-style comparison against the preserved retail baseline

For this target, a candidate passes only if it stays baseline-like enough that
the repo can justify another live test.
EOF
}

if [ "$#" -ne 2 ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit $([ "$#" -eq 2 ] && [ "${1:-}" != "-h" ] && [ "${1:-}" != "--help" ] && echo 0 || echo 1)
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

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

is_plain_ms_list() {
  local path=$1
  [ -f "$path" ] || return 1
  grep -aq '^/MS/' "$path"
}

classify_runtime_style() {
  local root=$1
  local object_cfg="$root/OPEN-R/MW/CONF/OBJECT.CFG"
  local connect_cfg="$root/OPEN-R/MW/CONF/CONNECT.CFG"
  local designdb_cfg="$root/OPEN-R/MW/CONF/DESIGNDB.CFG"

  local plain_count=0
  local non_plain_count=0
  local file

  for file in "$object_cfg" "$connect_cfg" "$designdb_cfg"; do
    if [ ! -f "$file" ]; then
      continue
    fi

    if is_plain_ms_list "$file"; then
      plain_count=$((plain_count + 1))
    else
      non_plain_count=$((non_plain_count + 1))
    fi
  done

  if [ "$plain_count" -gt 0 ] && [ "$non_plain_count" -eq 0 ]; then
    printf 'sdk\n'
  elif [ "$non_plain_count" -gt 0 ] && [ "$plain_count" -eq 0 ]; then
    printf 'retail\n'
  elif [ "$plain_count" -gt 0 ] && [ "$non_plain_count" -gt 0 ]; then
    printf 'mixed\n'
  else
    printf 'unknown\n'
  fi
}

BASELINE_ROOT=$(resolve_ms_root "$1")
CANDIDATE_ROOT=$(resolve_ms_root "$2")

BASELINE_STYLE=$(classify_runtime_style "$BASELINE_ROOT")
CANDIDATE_STYLE=$(classify_runtime_style "$CANDIDATE_ROOT")

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

SIM_LOG="$tmpdir/simulate.log"
AUDIT_LOG="$tmpdir/audit.log"

sim_rc=0
audit_rc=0

if ! "$ROOT_DIR/scripts/simulate-openr-boot.sh" "$CANDIDATE_ROOT" >"$SIM_LOG" 2>&1; then
  sim_rc=$?
fi

if ! "$ROOT_DIR/scripts/audit-ers7-stick-risk.sh" "$BASELINE_ROOT" "$CANDIDATE_ROOT" >"$AUDIT_LOG" 2>&1; then
  audit_rc=$?
fi

echo "ERS-7 experimental M2 candidate validator"
echo "baseline:  $BASELINE_ROOT"
echo "candidate: $CANDIDATE_ROOT"
echo
echo "runtime style:"
echo "  baseline:  $BASELINE_STYLE"
echo "  candidate: $CANDIDATE_STYLE"
echo
echo "simulator exit: $sim_rc"
echo "risk-audit exit: $audit_rc"
echo

blocked=0

if [ "$BASELINE_STYLE" != "$CANDIDATE_STYLE" ]; then
  echo "BLOCK: runtime style differs from preserved baseline"
  blocked=1
fi

case "$CANDIDATE_STYLE" in
  sdk|mixed|unknown)
    echo "BLOCK: candidate is not baseline-like retail runtime for this experimental M2"
    blocked=1
    ;;
esac

if [ "$audit_rc" -ne 0 ]; then
  echo "BLOCK: static risk audit did not return LOW risk"
  blocked=1
fi

if [ "$sim_rc" -eq 2 ]; then
  echo "BLOCK: plain-text object simulation found missing referenced objects"
  blocked=1
fi

if [ "$sim_rc" -eq 3 ]; then
  echo "note: candidate uses retail/encoded OBJECT.CFG at MW layer"
fi

echo
echo "audit summary:"
sed -n '1,220p' "$AUDIT_LOG"
echo
echo "simulator summary:"
sed -n '1,200p' "$SIM_LOG"
echo

if [ "$blocked" -ne 0 ]; then
  echo "validation result: FAIL"
  echo "recommendation: do not live-test this candidate on the experimental ERS-7 M2"
  exit 2
fi

echo "validation result: PASS"
echo "recommendation: candidate is conservative enough for a carefully controlled live test"
