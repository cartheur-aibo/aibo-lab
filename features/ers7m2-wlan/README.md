# ERS-7 MIND 2 WLAN Workflow

This workflow is the middle ground between:

- the preserved-only baseline
- the higher-risk `WCONSOLE` overlay

It stages a larger-stick MIND 2 tree with the SDK's `WLAN` wireless overlay,
but without the extra wireless-console hooks that `WCONSOLE` adds.

## Why This Exists

Real hardware testing showed:

- the preserved baseline boots
- the current `WCONSOLE` overlay can trigger grinding/no-full-boot behavior

The Programmer's Guide and SDK layout suggest a safer staircase:

1. baseline
2. `WLAN`
3. `WCONSOLE`

This feature provides the second rung.

## Build The Staging Tree

Run:

```bash
./scripts/prepare-ers7m2-wlan.sh
```

That creates:

- `features/ers7m2-wlan/build/stick`

Like the other ERS-7 MIND 2 staging flows, it starts from the preserved
known-good MIND 2 tree and injects:

- [src/ERS7M2/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/ERS7M2/WLANCONF.TXT)

into:

- `OPEN-R/SYSTEM/CONF/WLANCONF.TXT`

## What Changes Compared To Baseline

This workflow still changes early `SYSTEM` boot inputs, so it is not
baseline-safe.

But compared to full `WCONSOLE`, it avoids the console-specific additions:

- `OPEN-R/SYSTEM/OBJS/HOOK.BIN`
- `OPEN-R/SYSTEM/OBJS/ANTTCPIO.BIN`
- `OPEN-R/SYSTEM/OBJS/HOOKACT.BIN`

Those are the main extra boot-layer pieces that appear only in the SDK
`WCONSOLE` overlay.

## Preflight Audit

Before any hardware use, compare this candidate to the preserved baseline:

```bash
./scripts/audit-ers7-stick-risk.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-wlan/build/stick
```

If that result is still not comfortably narrow, stop there and keep the work on
the simulator or remote-processing track.

## Recommended Use

Use this feature only after:

1. the preserved baseline is stable on hardware
2. you want Wi-Fi runtime changes without jumping straight to WCONSOLE
3. the preflight audit has been reviewed

For the broader deployment process, see:

- [features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md)
