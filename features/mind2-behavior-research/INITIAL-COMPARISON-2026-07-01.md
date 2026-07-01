# Initial Persistent-State Comparison: July 1, 2026

This note records the first targeted comparison between:

- the repaired preserved specimen image
- the older preserved MIND 2 reference image

Compared images:

- specimen:
  `local-artifacts/sony-mind2-restored-2026-07-01.img`
- reference:
  `local-artifacts/sony-mind2-reference.img`

## First Key Result

In `OPEN-R/APP/DATA/P/`, only these files differed:

- `AWAKING.CFG`
- `FVAR`
- `GVAR`
- `IEG.CFG`
- `SIDRDATA.BIN`
- `STTLOG`

Everything else in that folder matched:

- `AIDLIST`
- `AMMSGID.TXT`
- `AMUIDL.TXT`
- `ARRMAIN.BIN`
- `ARSVDATA.BIN`
- `CUS.CFG`
- `CUSVER`
- `EPTN.BIN`
- `ESCENE.BIN`
- `FIFDATA.BIN`
- `INHFLG`
- `OWNER.TXT`
- `RVAR`
- `TR.CT`

## Why This Matters

This narrows the first behavior-forensics target significantly.

Instead of treating the entire persistent application-state tree as equally
suspect, we now have a small first-pass candidate set.

The strongest immediate human-readable or semi-readable candidates are:

- `STTLOG`
- `AWAKING.CFG`
- `IEG.CFG`

The binary state candidates worth preserving and comparing next are:

- `FVAR`
- `GVAR`
- `SIDRDATA.BIN`

## Working Hypothesis

If the repaired specimen shows distinctive shutdown or higher-order behavior,
the first transferable state cluster to test is likely:

- `AWAKING.CFG`
- `FVAR`
- `GVAR`
- `IEG.CFG`
- `SIDRDATA.BIN`
- `STTLOG`

## Next Tests

1. Compare `OPEN-R/MW/DATA/P/` and `OPEN-R/SYSTEM/DATA/P/` between the same
   two images.
2. Produce lightweight summaries or hashes for the six differing application
   state files.
3. Define the first one-variable transplant test on sacrificial cloned media.
