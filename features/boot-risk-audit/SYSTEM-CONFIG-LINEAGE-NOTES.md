# ERS-7 SYSTEM Config Lineage Notes

This note captures what we now know about the opaque retail `SYSTEM` boot
configs that sit directly under the current ERS-7 MIND 2 baseline.

Files in scope:

- `OPEN-R/SYSTEM/CONF/CARDDRV.CFG`
- `OPEN-R/SYSTEM/CONF/EXTOBJ.CFG`

## Why This Matters

The current `WLAN` and `WCONSOLE` staging paths do not merely add a few files.

They replace the preserved retail `SYSTEM` configs with SDK-style plain-text
`/MS/...` object lists.

That is a boot-layer representation change, so we wanted to answer two
questions first:

1. Is the preserved baseline using a stable known-good retail lineage?
2. Do the opaque retail configs look version-shaped rather than random?

## Baseline Lineage Result

The preserved working baseline matches bundled retail MIND 2 exactly.

Observed `sha256`:

- baseline `CARDDRV.CFG`:
  `e7af0242f92d86beb59f2baf0aac9c78f64e8b327a98bca1804d5e7d856969d5`
- bundled MIND 2 `CARDDRV.CFG`:
  `e7af0242f92d86beb59f2baf0aac9c78f64e8b327a98bca1804d5e7d856969d5`
- baseline `EXTOBJ.CFG`:
  `57b320f986ae6dfe2ebf6b32e8a036b64ba87666d04e33f178e5f2c94db1a51b`
- bundled MIND 2 `EXTOBJ.CFG`:
  `57b320f986ae6dfe2ebf6b32e8a036b64ba87666d04e33f178e5f2c94db1a51b`

So the current baseline is not an odd repaired-stick special case here. It is
aligned with the repo's known MIND 2 retail lineage.

## Structural Shape

The opaque retail files fit the same family clues we have already seen in other
Sony configs:

- MIND 2 `CARDDRV.CFG`: `120` bytes = `15 x 8` = `5 x 24`
- MIND 2 `EXTOBJ.CFG`: `96` bytes = `12 x 8` = `4 x 24`

The MIND 2 `CARDDRV.CFG` and `EXTOBJ.CFG` files share the same first `8` bytes:

- `9073ab72cf85c9bb`

That shared leading block also appears in the bundled MIND 2 copies at the same
position.

## MIND 2 vs MIND 3 Contrast

The MIND 3 retail files differ completely from the MIND 2 baseline at the
tested `8`-byte block level:

- `CARDDRV.CFG`: no same-position equal spans, no shared `8`-byte blocks
- `EXTOBJ.CFG`: no same-position equal spans, no shared `8`-byte blocks

MIND 3 also uses a different shared leading `8`-byte block:

- `08494436c1d67f88`

This is useful because it means these opaque `SYSTEM` configs behave like
version-specific structured data, not arbitrary noise.

## What The SDK Overlays Expose

The SDK `WLAN` and `WCONSOLE` overlays expose readable object-entry counts:

- `WLAN` `CARDDRV.CFG`: `2` entries
- `WLAN` `EXTOBJ.CFG`: `2` entries
- `WCONSOLE` `CARDDRV.CFG`: `3` entries
- `WCONSOLE` `EXTOBJ.CFG`: `4` entries

That count shape lines up interestingly with the retail file sizes:

- retail `EXTOBJ.CFG` = `4 x 24`
- retail `CARDDRV.CFG` = `5 x 24`

This does **not** prove one retail `24`-byte record equals one SDK object
entry.

But it is now a plausible hypothesis worth testing, especially because:

- `WCONSOLE` visibly adds exactly `1` `CARDDRV` object entry vs `WLAN`
- `WCONSOLE` visibly adds exactly `2` `EXTOBJ` object entries vs `WLAN`
- the real robot already told us these `SYSTEM` changes are behaviorally
  significant

Using
[scripts/analyze-retail-systemcfg.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/scripts/analyze-retail-systemcfg.sh),
we now have a sharper count result:

- retail `EXTOBJ.CFG` has `4` aligned `24`-byte records
- retail `CARDDRV.CFG` has `5` aligned `24`-byte records
- SDK `WCONSOLE` exposes `4` `EXTOBJ` entries, which exactly matches the retail
  `EXTOBJ` record count
- SDK `WCONSOLE` exposes only `3` `CARDDRV` entries, which is still `2` short
  of the retail `CARDDRV` record count

That asymmetry matters.

It suggests one of two things:

1. one retail `24`-byte record does not map 1:1 to one visible SDK object
   entry
2. the retail `CARDDRV` table likely carries extra boot semantics that the SDK
   text list does not expose directly

At the same time, both retail tables are fully versioned between MIND 2 and
MIND 3:

- no stable same-position records
- no shared `24`-byte records anywhere

So these files are behaving like structured version-specific boot tables, not
like containers with a large invariant core.

## Current Best Interpretation

- the preserved baseline's opaque `SYSTEM` config is a valid retail MIND 2
  lineage, not a one-off anomaly
- swapping in SDK `WLAN` or `WCONSOLE` configs is a representation-family
  change at the boot layer
- the retail `SYSTEM` config family likely encodes a structured object table
  or closely related boot mapping
- until we can relate those retail records to the SDK object lists more
  confidently, replacing these files remains a high-risk move

## Recommended Next Step

The next useful decoding task is narrow and concrete:

1. treat retail `CARDDRV.CFG` as `5` candidate `24`-byte records
2. treat retail `EXTOBJ.CFG` as `4` candidate `24`-byte records
3. compare those record counts and positions against the SDK `WLAN` and
   `WCONSOLE` object-entry sets
4. look for evidence that one or more retail records correspond to:
   - `IPSTACK.BIN`
   - `WLANENBL.BIN`
   - `EMGCYMON.BIN`
   - `NETCONFS.BIN`
   - `HOOK.BIN`
   - `ANTTCPIO.BIN`
   - `HOOKACT.BIN`
