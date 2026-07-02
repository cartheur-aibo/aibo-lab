# ERS-7 MIND 2 WCONSOLE Workflow

This workflow is for larger original Sony Memory Sticks such as the 64 MB
sticks you ordered.

It avoids the 8 MB bring-up path entirely and instead starts from the preserved
known-good AIBO MIND 2 stick tree we captured from the real robot.

If you want the lowest-risk first hardware boot path, start with:

- [features/ers7m2-baseline/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7m2-baseline/README.md)

and only return here once the preserved-only baseline is stable.

## Current Status

This workflow is still experimental on real hardware.

The current `SYSTEM_FLAVOR=WCONSOLE` overlay has produced abnormal boot
behavior on the ERS-7, including grinding/no-full-boot symptoms that do not
appear with the preserved-only baseline.

Treat this feature as an analysis and staging path, not a currently safe
hardware deployment path.

## Goal

Prepare a full MIND 2 WCONSOLE stick for app testing with:

- the known-good MIND 2 file tree
- the lab-operational Wi-Fi config from `src/ERS7M2/WLANCONF.TXT`
- the SDK's `WCONSOLE` system overlay for wireless-console testing
- a repeatable staging directory under `features/`

## Build The Staging Tree

Run:

```bash
./scripts/prepare-ers7m2-wconsole.sh
```

That creates:

- `features/ers7m2-wconsole/build/stick`

and injects:

- [src/ERS7M2/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/ERS7M2/WLANCONF.TXT)

into:

- `OPEN-R/SYSTEM/CONF/WLANCONF.TXT`

The default `ers7m2-wconsole` build still includes the SDK's `WCONSOLE` extras
for wireless-console testing, but current evidence says that overlay is not yet
safe enough for routine robot boots.

If you need a narrower preserved-only first boot instead, use:

```bash
./scripts/prepare-ers7m2-baseline.sh
```

If you want to override the test-stick build back to a non-`WCONSOLE` flavor,
run:

```bash
SYSTEM_FLAVOR=preserved ./scripts/prepare-ers7m2-wconsole.sh
```

The standard test-stick path keeps the preserved MIND 2 runtime and app tree,
but overlays the `WCONSOLE`-specific system files needed for the telnet
console path.

## What It Uses

The script copies from:

- [features/aibo-mind2/build/stick](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/aibo-mind2/build/stick)

which is the preserved staged copy of the known-good 32 MB MIND 2 stick.

This means the 64 MB workflow starts from the best source of truth we have in
the repo.

## WLAN Scope

The repo currently has two different valid WLAN contexts and they should not be
mixed up:

- `features/aibo-mind2/build/stick/OPEN-R/SYSTEM/CONF/WLANCONF.TXT`
  belongs to the preserved specimen-style MIND 2 baseline and may reflect the
  state that came from the preserved stick itself
- `src/ERS7M2/WLANCONF.TXT`
  is the lab-operational config used to prepare test sticks for the current
  shared environment

This workflow intentionally injects `src/ERS7M2/WLANCONF.TXT` because it is for
testing and development in the lab, not for preserving a historical specimen
unchanged.

## Hostname Naming Convention

`src/ERS7M2/WLANCONF.TXT` currently uses:

- `HOSTNAME=AIBOM2A`

That is not descriptive enough for the current lab setup. With multiple ERS-7
units, especially more than one MIND 2 robot, the repo should treat hostnames
as per-robot identity rather than a shared default.

The current intended naming convention is:

- `AIBO-M2A`
- `AIBO-M2B`
- `AIBO-M3A`

Future work should likely define per-robot WLAN variants built from that
convention rather than assuming one shared hostname forever.

## Copy To The 64 MB Stick

Once the 64 MB Sony stick is mounted:

```bash
rsync -a --delete /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7m2-wconsole/build/stick/ /path/to/mounted-stick/
sync
```

Then eject the stick cleanly before inserting it into the ERS-7.

For the shared Sony-reader deployment procedure, see:

- [features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md)

## Why This Path Is Still Useful

- the full MIND 2 layout already works on the robot
- `Aibonet` is already proven live on the working 32 MB stick
- the new 64 MB sticks remove the 8 MB capacity pressure
- it gives us a repeatable candidate tree to audit against the preserved
  baseline

What it is **not** yet:

- a proven safe WCONSOLE hardware boot path
- a justified reason to keep trying blind robot boots with the current overlay

## Verified Milestone

We now have a verified live network result with the real robot:

- the ERS-7 joined a compatible mobile-hotspot `AIBONET`
- the robot obtained IP `192.168.43.8`
- this machine reached the robot over Wi-Fi
- `ping 192.168.43.8` succeeded
- `http://192.168.43.8/` responded with:
  `Server: AIBO HTTPD v1.14 (Aperios)`
- the top page identified itself as `AIBO MIND 2 | Top Page`

This is the strongest networking proof we have so far. It confirms:

- the robot can join a compatible access point as a client
- the lab `WLANCONF.TXT` in `src/ERS7M2/WLANCONF.TXT` is a credible operational
  baseline
- earlier failures were strongly influenced by access-point compatibility, not
  just stick contents

One important caveat:

- port `59000` was still closed during this test, so this proved MIND 2 HTTP
  reachability, not the OPEN-R wireless console

Later hardware testing also showed that the current WCONSOLE overlay can push
the robot into a bad boot state even though the baseline still works.

For that reason, this feature should now be paired with:

- [features/boot-risk-audit/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/boot-risk-audit/README.md)

before any further hardware deployment attempts.

## Next App-Test Step

Before more real-hardware WCONSOLE attempts, the next work should be:

1. reduce or explain the `SYSTEM/CONF/CARDDRV.CFG` drift
2. reduce or explain the `SYSTEM/CONF/EXTOBJ.CFG` drift
3. classify which added `SYSTEM/OBJS` files are actually required
4. prefer simulator or remote-processing work where possible
