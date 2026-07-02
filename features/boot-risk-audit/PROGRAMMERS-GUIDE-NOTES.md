# Programmer's Guide Notes For Stick Preflight

This note captures the pieces of Sony's
[docs/aperios/ProgrammersGuide.pdf](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios/ProgrammersGuide.pdf)
that matter most for safer pre-deployment auditing.

## Why This Matters

Our first-pass audit started as a drift checker between:

- `features/ers7m2-baseline/build/stick`
- `features/ers7m2-wconsole/build/stick`

That already caught dangerous `SYSTEM`-layer drift, but the Programmer's Guide
adds an important model:

- Sony expects runtime behavior to be driven by chosen `OPEN-R` configurations
- runtime configuration files under `OPEN-R/MW/CONF/` matter directly
- copying a chosen `OPEN-R` tree to a Programming Memory Stick is the canonical
  SDK path

This means our audit should reason in terms of configuration families, not only
raw file diffs.

## Key Guide Points

From Chapter 4:

- there are multiple runtime families:
  - `BASIC`
  - `WLAN`
  - `WCONSOLE`
- each family has `memprot` and `nomemprot` variants
- the chosen `OPEN-R` directory is copied to the Programming Memory Stick

From Chapter 4.2:

- `OBJECT.CFG` enumerates objects to execute
- `CONNECT.CFG` describes subject/observer connections
- `DESIGNDB.CFG` maps robot-dependent data files by keyword

From Chapter 4.3:

- the canonical programmable-stick layout is:
  - `OPEN-R/`
  - `OPEN-R/MW/OBJS/`
  - `OPEN-R/MW/DATA/P/`

From Chapter 6:

- Sony explicitly supports `Remote Processing OPEN-R`
- some objects can run on AIBO while others run on a remote host
- `TCPGateway` provides cross-host object communication

## Implication For Our Repo

The repo's preserved retail MIND 2 trees are not the same thing as a clean SDK
sample-style Programming Memory Stick.

That creates two distinct preflight problems:

1. SDK-style programmable trees
   These should be auditable via plain-text `OBJECT.CFG`, `CONNECT.CFG`, and
   `DESIGNDB.CFG`.

2. Retail MIND 2 trees with SDK overlays
   These can contain retail/binary config files that are not directly modeled by
   the existing sample-style simulator.

The failed `ers7m2-wconsole` boots fit the second category: a preserved retail
tree plus SDK `WCONSOLE` boot-layer drift.

## What The Audit Should Learn Next

1. Detect whether a candidate is:
   - SDK/plain-text style
   - retail/binary style
   - mixed

2. Treat mixed retail + SDK overlay cases as inherently higher risk.

3. Parse and validate plain-text `CONNECT.CFG` and `DESIGNDB.CFG` when present.

4. Keep highlighting `SYSTEM`-layer drift, because the real robot evidence says
   early boot changes are the most dangerous class so far.

## Remote Processing Opportunity

Chapter 6 suggests a second research path beyond static auditing:

- use Remote Processing OPEN-R as a safer host-assisted development model
- push more object iteration off the robot when possible
- reserve direct stick boots for smaller, better-understood deltas

That will likely need its own follow-up feature or note, but it is now clearly
part of the same safety story.
