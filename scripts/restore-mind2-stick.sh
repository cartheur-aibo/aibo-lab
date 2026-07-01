#!/usr/bin/env bash
set -euo pipefail

usage() {
  cat <<'EOF'
usage: restore-mind2-stick.sh [--dry-run] /path/to/mounted-stick [source-stick-dir]

Restore a Sony-formatted 32 MB class Memory Stick with a known-good ERS-7
AIBO MIND 2 payload while preserving the existing low-level media format.

By default, the source payload is:
  features/aibo-mind2/build/stick

This script copies only the intended AIBO top-level entries:
  MEMSTICK.IND
  OPEN-R/
  PALM/
  StikZap.prc (if present in the source)

It also removes common host-side metadata from the mounted stick tree.
EOF
}

DRY_RUN=0

if [ "${1:-}" = "--dry-run" ]; then
  DRY_RUN=1
  shift
fi

if [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
  usage
  exit 0
fi

if [ "$#" -lt 1 ] || [ "$#" -gt 2 ]; then
  usage
  exit 1
fi

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TARGET_DIR="$1"
SOURCE_DIR="${2:-$ROOT_DIR/features/aibo-mind2/build/stick}"

require_path() {
  if [ ! -e "$1" ]; then
    echo "error: missing required path: $1" >&2
    exit 1
  fi
}

require_path "$TARGET_DIR"
require_path "$SOURCE_DIR/MEMSTICK.IND"
require_path "$SOURCE_DIR/OPEN-R"

if [ ! -d "$TARGET_DIR" ]; then
  echo "error: target is not a directory: $TARGET_DIR" >&2
  exit 1
fi

if [ "$TARGET_DIR" = "/" ]; then
  echo "error: refusing to operate on /" >&2
  exit 1
fi

echo "Restoring ERS-7 MIND 2 payload"
echo "source: $SOURCE_DIR"
echo "target: $TARGET_DIR"
echo "mode:   $([ "$DRY_RUN" -eq 1 ] && echo dry-run || echo live)"
echo

RSYNC_ARGS=(-a --delete)
if [ "$DRY_RUN" -eq 1 ]; then
  RSYNC_ARGS+=(--dry-run -v)
fi

copy_if_present() {
  local rel="$1"
  if [ -e "$SOURCE_DIR/$rel" ]; then
    echo "sync: $rel"
    rsync "${RSYNC_ARGS[@]}" "$SOURCE_DIR/$rel" "$TARGET_DIR/"
  fi
}

remove_if_present() {
  local rel="$1"
  if [ -e "$TARGET_DIR/$rel" ]; then
    if [ "$DRY_RUN" -eq 1 ]; then
      echo "remove: $rel"
    else
      echo "remove: $rel"
      rm -rf "$TARGET_DIR/$rel"
    fi
  fi
}

copy_if_present "MEMSTICK.IND"
copy_if_present "OPEN-R"
copy_if_present "PALM"
copy_if_present "StikZap.prc"

echo
echo "cleanup: unexpected top-level entries"
remove_if_present "System Volume Information"

echo
echo "cleanup: host metadata"
if [ "$DRY_RUN" -eq 1 ]; then
  find "$TARGET_DIR" \
    \( -name '.DS_Store' -o -name '._*' \) \
    -print || true
  find "$TARGET_DIR" -mindepth 1 -maxdepth 1 \
    \( -name '.Spotlight-V100' -o -name '.Trashes' -o -name '.fseventsd' -o -name '.TemporaryItems' \) \
    -print || true
else
  "$ROOT_DIR/scripts/strip-host-metadata.sh" "$TARGET_DIR"
fi

echo
if [ "$DRY_RUN" -eq 1 ]; then
  echo "dry-run complete: no files were changed"
else
  sync
  echo "restore complete"
  echo "next: unmount/eject the stick cleanly before testing in the robot"
fi
