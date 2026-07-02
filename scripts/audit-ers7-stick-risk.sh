#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
Usage: scripts/audit-ers7-stick-risk.sh <baseline-ms-root> <candidate-ms-root>

Compare a candidate ERS-7 Memory Stick tree against a known-good baseline and
emit a conservative boot-risk report.

Accepted inputs:
  - a Memory Stick root containing OPEN-R/
  - a sample directory containing MS/OPEN-R/

This is a static preflight audit. It does not emulate Aperios or prove a safe
hardware boot.
EOF
}

if [ "${1:-}" = "" ] || [ "${2:-}" = "" ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
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

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir"' EXIT

BASELINE_FILES="$tmpdir/baseline-files.txt"
CANDIDATE_FILES="$tmpdir/candidate-files.txt"
TOPLEVEL_BASELINE="$tmpdir/toplevel-baseline.txt"
TOPLEVEL_CANDIDATE="$tmpdir/toplevel-candidate.txt"

find "$BASELINE_ROOT" -type f | sed "s#^$BASELINE_ROOT/##" | sort > "$BASELINE_FILES"
find "$CANDIDATE_ROOT" -type f | sed "s#^$CANDIDATE_ROOT/##" | sort > "$CANDIDATE_FILES"
find "$BASELINE_ROOT" -maxdepth 1 -mindepth 1 -printf '%f\n' | sort > "$TOPLEVEL_BASELINE"
find "$CANDIDATE_ROOT" -maxdepth 1 -mindepth 1 -printf '%f\n' | sort > "$TOPLEVEL_CANDIDATE"

risk_score=0
finding_count=0

is_plain_ms_list() {
  local path=$1
  [ -f "$path" ] || return 1
  grep -aq '^/MS/' "$path"
}

is_mostly_text() {
  local path=$1
  [ -f "$path" ] || return 1
  LC_ALL=C grep -aq '[[:print:][:space:]]' "$path"
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

raise_risk() {
  local weight=$1
  local severity=$2
  shift 2
  finding_count=$((finding_count + 1))
  risk_score=$((risk_score + weight))
  printf '%s: %s\n' "$severity" "$*"
}

check_required_path() {
  local path=$1
  if [ ! -e "$CANDIDATE_ROOT/$path" ]; then
    raise_risk 10 "HIGH" "missing required path: $path"
  fi
}

check_required_path "MEMSTICK.IND"
check_required_path "OPEN-R"

echo "ERS-7 stick boot-risk audit"
echo "baseline:  $BASELINE_ROOT"
echo "candidate: $CANDIDATE_ROOT"
echo

BASELINE_STYLE=$(classify_runtime_style "$BASELINE_ROOT")
CANDIDATE_STYLE=$(classify_runtime_style "$CANDIDATE_ROOT")

echo "runtime style classification:"
echo "baseline style:  $BASELINE_STYLE"
echo "candidate style: $CANDIDATE_STYLE"
echo

case "$CANDIDATE_STYLE" in
  mixed)
    raise_risk 8 "HIGH" "candidate mixes SDK-style and retail-style runtime configuration"
    ;;
  retail)
    raise_risk 2 "LOW" "candidate uses retail/non-plain runtime configuration"
    ;;
  sdk)
    :
    ;;
  unknown)
    raise_risk 4 "MEDIUM" "candidate runtime configuration style is unknown"
    ;;
esac

if [ "$BASELINE_STYLE" != "$CANDIDATE_STYLE" ]; then
  raise_risk 6 "MEDIUM" "runtime style differs from baseline ($BASELINE_STYLE -> $CANDIDATE_STYLE)"
fi

echo
echo "top-level layout checks:"

if ! diff -q "$TOPLEVEL_BASELINE" "$TOPLEVEL_CANDIDATE" >/dev/null 2>&1; then
  raise_risk 2 "MEDIUM" "top-level layout differs from baseline"
  echo "top-level diff:"
  diff -u "$TOPLEVEL_BASELINE" "$TOPLEVEL_CANDIDATE" || true
  echo
fi

echo "critical config checks:"

critical_diff() {
  local relpath=$1
  if [ ! -e "$BASELINE_ROOT/$relpath" ] || [ ! -e "$CANDIDATE_ROOT/$relpath" ]; then
    raise_risk 8 "HIGH" "critical config missing from baseline or candidate: $relpath"
    return
  fi

  if ! cmp -s "$BASELINE_ROOT/$relpath" "$CANDIDATE_ROOT/$relpath"; then
    raise_risk 8 "HIGH" "critical config changed: $relpath"
  fi
}

critical_diff "OPEN-R/SYSTEM/CONF/CARDDRV.CFG"
critical_diff "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG"

if [ -e "$BASELINE_ROOT/OPEN-R/MW/CONF/OBJECT.CFG" ] && [ -e "$CANDIDATE_ROOT/OPEN-R/MW/CONF/OBJECT.CFG" ]; then
  if ! cmp -s "$BASELINE_ROOT/OPEN-R/MW/CONF/OBJECT.CFG" "$CANDIDATE_ROOT/OPEN-R/MW/CONF/OBJECT.CFG"; then
    raise_risk 5 "MEDIUM" "retail MW OBJECT.CFG differs from baseline"
  fi
fi

if [ -e "$BASELINE_ROOT/OPEN-R/SYSTEM/CONF/OBJECT.CFG" ] && [ -e "$CANDIDATE_ROOT/OPEN-R/SYSTEM/CONF/OBJECT.CFG" ]; then
  if ! cmp -s "$BASELINE_ROOT/OPEN-R/SYSTEM/CONF/OBJECT.CFG" "$CANDIDATE_ROOT/OPEN-R/SYSTEM/CONF/OBJECT.CFG"; then
    raise_risk 5 "MEDIUM" "retail SYSTEM OBJECT.CFG differs from baseline"
  fi
fi

echo
echo "runtime config model checks:"

for relpath in \
  "OPEN-R/MW/CONF/OBJECT.CFG" \
  "OPEN-R/MW/CONF/CONNECT.CFG" \
  "OPEN-R/MW/CONF/DESIGNDB.CFG"
do
  baseline_cfg="$BASELINE_ROOT/$relpath"
  candidate_cfg="$CANDIDATE_ROOT/$relpath"

  if [ ! -f "$baseline_cfg" ] || [ ! -f "$candidate_cfg" ]; then
    raise_risk 6 "MEDIUM" "runtime config missing from baseline or candidate: $relpath"
    continue
  fi

  baseline_plain=0
  candidate_plain=0
  is_plain_ms_list "$baseline_cfg" && baseline_plain=1 || true
  is_plain_ms_list "$candidate_cfg" && candidate_plain=1 || true

  if [ "$baseline_plain" -eq 1 ] && [ "$candidate_plain" -eq 1 ]; then
    if ! cmp -s "$baseline_cfg" "$candidate_cfg"; then
      raise_risk 4 "MEDIUM" "plain-text runtime config changed: $relpath"
    fi
    continue
  fi

  if ! cmp -s "$baseline_cfg" "$candidate_cfg"; then
    raise_risk 5 "MEDIUM" "retail or non-plain runtime config differs: $relpath"
  fi

  if [ "$candidate_plain" -eq 0 ]; then
    raise_risk 2 "LOW" "runtime config is not a plain-text /MS list: $relpath"
  fi
done

echo
echo "system object drift:"

SYSTEM_OBJS_BASELINE="$tmpdir/system-objs-baseline.txt"
SYSTEM_OBJS_CANDIDATE="$tmpdir/system-objs-candidate.txt"
find "$BASELINE_ROOT/OPEN-R/SYSTEM/OBJS" -maxdepth 1 -type f -printf '%f\n' | sort > "$SYSTEM_OBJS_BASELINE"
find "$CANDIDATE_ROOT/OPEN-R/SYSTEM/OBJS" -maxdepth 1 -type f -printf '%f\n' | sort > "$SYSTEM_OBJS_CANDIDATE"

extra_system_objs=$(comm -13 "$SYSTEM_OBJS_BASELINE" "$SYSTEM_OBJS_CANDIDATE" || true)
missing_system_objs=$(comm -23 "$SYSTEM_OBJS_BASELINE" "$SYSTEM_OBJS_CANDIDATE" || true)

if [ -n "$extra_system_objs" ]; then
  while IFS= read -r obj; do
    [ -z "$obj" ] && continue
    raise_risk 4 "MEDIUM" "extra SYSTEM object added: $obj"
  done <<< "$extra_system_objs"
fi

if [ -n "$missing_system_objs" ]; then
  while IFS= read -r obj; do
    [ -z "$obj" ] && continue
    raise_risk 7 "HIGH" "baseline SYSTEM object missing: $obj"
  done <<< "$missing_system_objs"
fi

echo
echo "text config reference checks:"

check_ms_refs() {
  local relpath=$1
  local fullpath="$CANDIDATE_ROOT/$relpath"
  [ -f "$fullpath" ] || return 0

  if ! grep -q '^/MS/' "$fullpath"; then
    return 0
  fi

  while IFS= read -r ms_path; do
    [ -z "$ms_path" ] && continue
    local_path="$CANDIDATE_ROOT${ms_path#/MS}"
    if [ ! -f "$local_path" ]; then
      raise_risk 10 "HIGH" "missing referenced object in $relpath: $ms_path"
    fi
  done < <(grep '^/MS/' "$fullpath" | awk '{print $1}')
}

check_ms_refs "OPEN-R/SYSTEM/CONF/CARDDRV.CFG"
check_ms_refs "OPEN-R/SYSTEM/CONF/EXTOBJ.CFG"
check_ms_refs "OPEN-R/MW/CONF/OBJECT.CFG"
check_ms_refs "OPEN-R/MW/CONF/DESIGNDB.CFG"

echo
echo "broad drift summary:"
extra_files_count=$(comm -13 "$BASELINE_FILES" "$CANDIDATE_FILES" | wc -l)
missing_files_count=$(comm -23 "$BASELINE_FILES" "$CANDIDATE_FILES" | wc -l)
printf 'extra files vs baseline:   %s\n' "$extra_files_count"
printf 'missing files vs baseline: %s\n' "$missing_files_count"

if [ "$missing_files_count" -gt 0 ]; then
  raise_risk 6 "MEDIUM" "candidate is missing baseline files"
fi

if [ "$extra_files_count" -gt 0 ]; then
  raise_risk 2 "LOW" "candidate adds extra files beyond baseline"
fi

echo
if [ "$risk_score" -ge 12 ]; then
  final_risk="HIGH"
elif [ "$risk_score" -ge 5 ]; then
  final_risk="MEDIUM"
else
  final_risk="LOW"
fi

echo "findings: $finding_count"
echo "risk score: $risk_score"
echo "risk level: $final_risk"
echo "note: this audit is grounded in static file/config drift. It does not emulate Aperios object execution or posture safety."

case "$final_risk" in
  HIGH)
    echo "recommendation: do not use this candidate on hardware without reducing boot-critical drift first"
    exit 2
    ;;
  MEDIUM)
    echo "recommendation: use only with caution on sacrificial media after understanding the listed drift"
    exit 1
    ;;
  LOW)
    echo "recommendation: candidate looks baseline-like, but this is not a hardware safety guarantee"
    exit 0
    ;;
esac
