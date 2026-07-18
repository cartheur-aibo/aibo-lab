# CPU and Config Provenance: July 1, 2026

This note captures two adjacent provenance threads that now support the retail
config decoding effort.

## CPU-Family Reference

The repo now carries:

- [local-artifacts/cpu-docs/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/local-artifacts/cpu-docs/README.md)
- [local-artifacts/cpu-docs/R4000_Preliminary_Users_Manual_1992.pdf](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/local-artifacts/cpu-docs/R4000_Preliminary_Users_Manual_1992.pdf)

Why it matters:

- the Sony OPEN-R Programmer's Guide says AIBO uses a MIPS CPU in `32-bit`
  `little endian` mode
- a local AIBO/Webots technical report in this repo describes AIBO robots in
  ERS-7 context as targeting a MIPS `R4000` processor

That remains a CPU-family hint, not final chip identification.

## Retail Config Family Clue

The `OBJECT.CFG` work now points to a broader family of opaque Sony retail
config files:

- `MW/CONF/OBJECT.CFG`
- `SYSTEM/CONF/OBJECT.CFG`
- `APP/CONF/OBJECT.CFG`
- `SYSTEM/CONF/EXTOBJ.CFG`
- `SYSTEM/CONF/CARDDRV.CFG`
- `MW/CONF/CONNECT.CFG`
- `MW/CONF/DESIGN.CFG`
- `MW/CONF/DESIGNDB.CFG`
- `MW/CONF/SERVICE.CFG`

Current characterization:

- all tested files are aligned to `8`-byte boundaries
- retail MW/SYSTEM `OBJECT.CFG` also align cleanly to `24`-byte records
- some apparently unrelated config files share the same first `8`-byte block
- `MW/CONNECT.CFG` keeps an identical trailing region across MIND 2 and MIND 3

Observed shared leading `8`-byte blocks on MIND 2:

- `EXTOBJ.CFG` and `CARDDRV.CFG` both start with
  `9073ab72cf85c9bb`
- `CONNECT.CFG` and `DESIGNDB.CFG` both start with
  `0e207d53993b8df4`

This is important because it argues against a model of each file being pure,
independent ciphertext with random initialization. It is more consistent with:

- a shared binary container format
- a fixed header or type key
- or a deterministic block transform over structured input

## Tooling

The repo now has two complementary analysis tools:

- [scripts/analyze-retail-objectcfg.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/scripts/analyze-retail-objectcfg.sh)
- [scripts/analyze-retail-config-family.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/scripts/analyze-retail-config-family.sh)
- [scripts/compare-retail-config-pair.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/scripts/compare-retail-config-pair.sh)

The second and third tools are intended for the broader opaque retail config
family, not just `OBJECT.CFG`.

## Current Best Cross-Version Clue

`MW/CONF/CONNECT.CFG` is especially interesting:

- MIND 2 and MIND 3 versions have different first `8`-byte blocks
- but the last `51` `8`-byte blocks are identical at the same positions
- the stable suffix begins exactly at byte `512`
- the stable suffix length is `408` bytes = `17 x 24`
- this is the only tested opaque retail config pair that shows a large aligned
  same-position shared span

That is the best current evidence that some retail config files have:

- a version-specific front section
- and a stable trailing section

rather than behaving like fully scrambled unrelated blobs.

## Working Interpretation

The local Sony Programmer's Guide gives a useful conceptual frame:

- `OBJECT.CFG` enumerates executable objects
- `CONNECT.CFG` describes subject/observer service connections

The current retail evidence therefore points to a plausible model in which:

- the `MW` object roster stayed unchanged between MIND 2 and MIND 3
- the `MW/CONNECT.CFG` tail preserved middleware connection structure
- the leading `512` bytes carry version-specific front matter, keys, or section
  metadata

This is still a hypothesis, but it is now a concrete, testable one.
