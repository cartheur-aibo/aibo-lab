# ERS-7 MIND 2 WCONSOLE SDK-System Comparison Workflow

This workflow exists for simulator and preflight comparison only.

It answers a narrower question than the normal `ers7m2-wconsole` path:

- what changes if we use a pure SDK `WCONSOLE` `SYSTEM` lineage
- instead of the current mixed preserved+SDK `SYSTEM` staging tree?

## Why This Exists

The current `ers7m2-wconsole` workflow keeps the preserved MIND 2 tree as the
base and overlays SDK `WCONSOLE` config plus selected SDK system objects.

Preflight analysis showed that this creates a mixed-provenance `SYSTEM` layer:

- SDK plain-text `CARDDRV.CFG` and `EXTOBJ.CFG`
- some referenced `SYSTEM/OBJS` from preserved retail MIND 2
- some referenced `SYSTEM/OBJS` from the SDK

That is useful for compatibility experiments, but it makes it harder to answer
whether failures come from:

- the SDK config representation
- mixed object provenance
- or both together

This comparison workflow isolates that variable by using the SDK's full
`WCONSOLE` `SYSTEM` object lineage.

## Build The Staging Tree

Run:

```bash
./scripts/prepare-ers7m2-wconsole-sdk-system.sh
```

That creates:

- `features/ers7m2-wconsole-sdk-system/build/stick`

Like the other ERS-7 MIND 2 staging flows, it still:

- starts from the preserved MIND 2 stick tree for the broader runtime/app
  layout
- injects [src/ERS7M2/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/ERS7M2/WLANCONF.TXT)

But for `OPEN-R/SYSTEM/`, it uses the SDK `WCONSOLE` lineage directly.

## Comparison Use

This candidate is meant for analysis first:

```bash
./scripts/audit-ers7-stick-risk.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-wconsole-sdk-system/build/stick
```

And for provenance confirmation:

```bash
./scripts/check-ers7-system-provenance.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-wconsole-sdk-system/build/stick \
  sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R
```

The expected difference from the normal `ers7m2-wconsole` path is that the
referenced `SYSTEM/OBJS` entries should classify as SDK-origin rather than a
baseline/SDK mix.

## Current Comparison Result

The pure-SDK `SYSTEM` candidate does remove the mixed-provenance issue:

- referenced `CARDDRV.CFG` and `EXTOBJ.CFG` objects classify as SDK-origin

But it also introduces a new gap relative to the preserved ERS-7 runtime:

- `RRWH.BIN` is present in the preserved baseline
- `RRWH.BIN` is present in the mixed `ers7m2-wconsole` tree
- `RRWH.BIN` is absent from the SDK `WCONSOLE` `SYSTEM/OBJS` lineage

That means the pure-SDK candidate is cleaner as a lineage comparison, but it is
not automatically closer to the real working robot runtime.

In current audit output this shows up as:

- `HIGH: baseline SYSTEM object missing: RRWH.BIN`

## Current Position

Treat this as a comparison candidate, not a recommended hardware deployment
path.

The goal is to reduce uncertainty in the simulator/preflight track before more
real robot boot attempts.
