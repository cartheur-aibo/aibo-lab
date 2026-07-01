# First Suspect Dossier: July 1, 2026

This note summarizes the first close look at the files that differ between:

- the repaired preserved specimen
- the older preserved MIND 2 reference

## Differing Files

- `OPEN-R/APP/DATA/P/AWAKING.CFG`
- `OPEN-R/APP/DATA/P/FVAR`
- `OPEN-R/APP/DATA/P/GVAR`
- `OPEN-R/APP/DATA/P/IEG.CFG`
- `OPEN-R/APP/DATA/P/SIDRDATA.BIN`
- `OPEN-R/APP/DATA/P/STTLOG`
- `OPEN-R/MW/DATA/P/PAT.LOG`
- `OPEN-R/SYSTEM/DATA/P/AIBO-ID`

## Immediate Signal

### `STTLOG`

- plain-text structured state/log table
- same size in both images: `2089` bytes
- differences are sparse rather than wholesale
- changed rows include entries around:
  `0046`, `0055`, `1200`, `0300`, `0400`

This is one of the highest-value next targets because:

- it is human-readable
- it appears to encode compact state transitions or counters
- the differences are small enough to inspect meaningfully

### `PAT.LOG`

- numeric text data
- same size in both images: `1089` bytes
- many byte-level differences, but still structured text rather than opaque

This may be a middleware-side activity or pattern log rather than direct
personality state, but it clearly captures different history between the two
images.

### `AIBO-ID`

- `4` bytes in both images
- differs between specimen and reference

This is a strong identity-binding clue. It raises the possibility that at
least some observed behavior is tied to robot identity or per-robot binding,
not only application-level personality state.

### `AWAKING.CFG`

- same size in both images: `605` bytes
- starts with `IEAD`
- string extraction reveals very little

This still looks promising because of its name and because it differs despite
being compact. It is a good candidate for structured binary diffing next.

### `IEG.CFG`

- same size in both images: `1697` bytes
- starts with `IEPR`
- string extraction reveals only minimal signal

Likely structured config/state. Worth diffing after `STTLOG` and `AIBO-ID`.

### `FVAR`, `GVAR`, `SIDRDATA.BIN`

- `FVAR`: `260` bytes
- `GVAR`: `517` bytes
- `SIDRDATA.BIN`: `6780` bytes
- all differ while several neighboring persistent files stay identical

These remain core suspects, but the first-pass extraction does not yet make
their semantics obvious.

## Provisional Ranking

Highest-value immediate follow-up:

1. `STTLOG`
2. `AIBO-ID`
3. `PAT.LOG`
4. `AWAKING.CFG`
5. `IEG.CFG`
6. `FVAR`
7. `GVAR`
8. `SIDRDATA.BIN`

## Next Best Technical Step

Build small file-specific summaries for:

- `STTLOG`
- `PAT.LOG`
- `AIBO-ID`

Then move to structured binary diffing for:

- `AWAKING.CFG`
- `IEG.CFG`
- `FVAR`
- `GVAR`
