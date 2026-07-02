# ERS-7 Boot Risk Audit

This feature starts a safer preflight track for ERS-7 stick deployment work.

The goal is not to emulate Aperios fully. The goal is to catch dangerous drift
before another real-hardware boot.

## Why This Exists

The current `simulate-openr-boot.sh` tool is useful for plain-text sample
layouts, but the real MIND 2 baseline and WCONSOLE trees use retail/binary
`OBJECT.CFG` files that the simulator does not decode yet.

That means we need a second safety layer:

- compare a candidate stick against a known-good baseline
- highlight boot-critical drift
- classify a candidate as lower-risk or higher-risk before hardware testing

## First Tool

The first tool in this track is:

- [scripts/audit-ers7-stick-risk.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/scripts/audit-ers7-stick-risk.sh)

It currently audits:

- runtime style classification:
  - `sdk`
  - `retail`
  - `mixed`
- required top-level paths
- top-level layout drift
- boot-critical config drift in:
  - `OPEN-R/SYSTEM/CONF/CARDDRV.CFG`
  - `OPEN-R/SYSTEM/CONF/EXTOBJ.CFG`
- retail `OBJECT.CFG` drift as a conservative signal
- added or missing `OPEN-R/SYSTEM/OBJS/*`
- missing file references from text `/MS/...` config entries

## Second Tool

The companion explainer is:

- [scripts/explain-ers7-system-delta.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/scripts/explain-ers7-system-delta.sh)
- [features/boot-risk-audit/SYSTEM-CONFIG-LINEAGE-NOTES.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/boot-risk-audit/SYSTEM-CONFIG-LINEAGE-NOTES.md)
- [scripts/analyze-retail-systemcfg.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/scripts/analyze-retail-systemcfg.sh)
- [scripts/check-ers7-system-provenance.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/scripts/check-ers7-system-provenance.sh)

It focuses specifically on the `SYSTEM` boot layer and answers:

- are `CARDDRV.CFG` and `EXTOBJ.CFG` opaque retail configs or readable SDK
  `/MS/...` lists?
- which `SYSTEM/OBJS` files are added beyond baseline?
- which boot-object entries become visible in the candidate?

This is especially useful for the current ERS-7 MIND 2 work because the
preserved baseline uses opaque retail `SYSTEM` config while the `WLAN` and
`WCONSOLE` overlays replace that layer with SDK-style text config lists.

The record-table analyzer goes one step lower:

- it treats retail `CARDDRV.CFG` and `EXTOBJ.CFG` as aligned `24`-byte tables
- it compares retail MIND 2 and MIND 3 record stability by position
- it compares retail record counts against SDK `WLAN` and `WCONSOLE` entry
  counts

The provenance checker answers a different question:

- when a candidate uses SDK-style plain-text `CARDDRV.CFG` or `EXTOBJ.CFG`,
  do the referenced `SYSTEM/OBJS` binaries actually come from the SDK overlay,
  from the preserved retail baseline, or from a mixture of both?
- and if we force a pure-SDK `SYSTEM` lineage, which real-runtime ERS-7
  `SYSTEM/OBJS` files disappear?

## Example

Compare the preserved baseline and the WCONSOLE candidate:

```bash
./scripts/audit-ers7-stick-risk.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-wconsole/build/stick
```

Typical result:

- `LOW` means baseline-like drift only
- `MEDIUM` means meaningful boot-layer changes are present
- `HIGH` means boot-critical drift or missing references were detected

The runtime style classification is especially important:

- `sdk` means the key runtime config files look like plain-text SDK-style `/MS`
  config lists
- `retail` means they do not look like plain-text SDK config lists
- `mixed` means some runtime config files look SDK-style and others do not,
  which is treated as especially risky

For a more specific explanation of the boot-layer delta:

```bash
./scripts/explain-ers7-system-delta.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-wconsole/build/stick
```

For the retail `SYSTEM` record-table view:

```bash
./scripts/analyze-retail-systemcfg.sh \
  features/ers7m2-baseline/build/stick/OPEN-R/SYSTEM/CONF/EXTOBJ.CFG \
  opt/AIBO7M3/OPEN-R/SYSTEM/CONF/EXTOBJ.CFG \
  sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WLAN/memprot/OPEN-R/SYSTEM/CONF/EXTOBJ.CFG \
  sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R/SYSTEM/CONF/EXTOBJ.CFG
```

For the mixed-provenance check:

```bash
./scripts/check-ers7-system-provenance.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-wconsole/build/stick \
  sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R
```

## Current Limit

This is intentionally conservative.

It still does **not** prove:

- that the stick will boot safely
- that posture/motion startup will behave safely
- that retail/binary configs are semantically valid

It only improves our ability to say:

- "this looks close to baseline"
- "this changed dangerous early-boot files"
- "do not keep testing this blindly on hardware"

## Next Likely Improvements

1. Decode or partially model retail `OBJECT.CFG`.
2. Add a known-good whitelist for expected WCONSOLE drift.
3. Break risk into categories such as:
   - service-layer drift
   - motion/runtime drift
   - unknown binary drift
4. Emit machine-readable reports for later tooling.
