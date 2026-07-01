# MIND 2 Recovery Milestone: July 1, 2026

This session established an end-to-end recovery and preservation workflow for a
Sony 32 MB class stick associated with an ERS-7 AIBO MIND 2 startup failure.

## What We Proved

- the original live capture had correct Sony 32 MB low-level formatting
- the original live capture was not bootable because it lacked `MEMSTICK.IND`
  and the full `OPEN-R/` tree
- restoring the MIND 2 payload onto that Sony-formatted media produced a stick
  that booted successfully on real hardware
- the repaired working stick was then preserved as a separate raw image

## Preserved Artifacts

Pre-repair forensic image:

- `local-artifacts/live-mind2-sdb.img`

Post-repair preserved image:

- `local-artifacts/sony-mind2-restored-2026-07-01.img`
- SHA-256:
  `81fd3c76eaf89d146f2e2d95738ab15cd43192e80acc48609c626bf542bcbdc1`

## Repo Outcomes

This milestone added:

- a read-only Sony stick image classifier
- a restore workflow that preserves Sony formatting while restoring MIND 2
  payload contents
- a case note for the broken stick capture
- a preservation note for repaired-image naming and handling

## Why It Matters

This validates one of the repo's original goals:

- preserve real stick knowledge
- separate media-format questions from payload-layout questions
- recover bootable ERS-7 media from forensic evidence instead of guesswork
