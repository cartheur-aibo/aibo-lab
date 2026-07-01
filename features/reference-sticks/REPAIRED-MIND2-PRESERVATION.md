# Repaired MIND 2 Preservation

Use this when a previously non-boot Sony 32 MB stick has been restored and now
boots successfully in an ERS-7.

## Goal

Preserve the repaired, working stick as a separate artifact from:

- the pre-repair forensic capture
- the older known-good MIND 2 reference image

## Recommended Names

Local raw image:

- `local-artifacts/sony-mind2-restored-2026-07-01.img`

Repo checksum record:

- `features/reference-sticks/sony-mind2-restored-2026-07-01.img.sha256`

Optional operator note:

- `features/reference-sticks/sony-mind2-restored-2026-07-01.md`

## Why Preserve Separately

This repaired image is a milestone artifact:

- it preserves the exact post-recovery contents that booted on real hardware
- it documents a successful restoration path from Sony-formatted non-boot
  media to a working ERS-7 MIND 2 stick
- it should remain distinguishable from the pre-repair capture

## One-Variable Rule

Capture the repaired stick before making any further edits, Wi-Fi changes, or
content experiments so the artifact corresponds to the exact working state.
