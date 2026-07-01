# First Experimental Design: Identity-Sensitive vs Identity-Neutral

This note defines the first clean experimental split after the initial
persistent-state comparison.

## Goal

Determine whether the distinctive repaired-specimen behavior depends on:

- application-level persistent state alone
- robot identity/binding state
- or a combination of both

## Baseline Specimen

Use as the specimen baseline:

- `local-artifacts/sony-mind2-restored-2026-07-01.img`

Use as the comparison reference:

- `local-artifacts/sony-mind2-reference.img`

## First Candidate State Cluster

The smallest high-signal differing cluster currently is:

- `OPEN-R/APP/DATA/P/AWAKING.CFG`
- `OPEN-R/APP/DATA/P/FVAR`
- `OPEN-R/APP/DATA/P/GVAR`
- `OPEN-R/APP/DATA/P/IEG.CFG`
- `OPEN-R/APP/DATA/P/SIDRDATA.BIN`
- `OPEN-R/APP/DATA/P/STTLOG`
- `OPEN-R/MW/DATA/P/PAT.LOG`

Identity-sensitive file:

- `OPEN-R/SYSTEM/DATA/P/AIBO-ID`

## Test Class A: Identity-Neutral

Change only the application and middleware persistent-state cluster:

- transplant the seven non-identity files above
- leave `AIBO-ID` unchanged on the target stick

Question:

- does the distinctive behavior transfer without the specimen's `AIBO-ID`?

## Test Class B: Identity-Sensitive

Change the same cluster and preserve the specimen identity marker:

- transplant the seven non-identity files above
- also transplant `AIBO-ID`

Question:

- does behavior transfer more strongly, or only, when `AIBO-ID` comes with it?

## Interpretation Rule

If Class A transfers behavior:

- app-state alone may be sufficient

If only Class B transfers behavior:

- identity binding is likely important

If neither transfers behavior:

- the state cluster is incomplete, or the observed behavior is more contextual
  than currently assumed

## Operational Rules

- preserve the original specimen image unchanged
- use cloned sacrificial media for all tests
- change one class of variables at a time
- record exact file provenance for every transplant
- log the observed shutdown and wake behavior with the shared observation
  template
