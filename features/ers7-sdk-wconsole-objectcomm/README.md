# ERS-7 SDK WCONSOLE ObjectComm Workflow

This is the repo-native ERS-7 `ObjectComm` sibling to the SDK HelloWorld
workflow.

It follows the official Remote Processing OPEN-R AIBO-side pattern:

- use the SDK `WCONSOLE/nomemprot` base
- build the local `ObjectComm` sample
- copy the sample `MS/OPEN-R` overlay
- edit `WLANCONF.TXT`

## Source Of Truth

The distilled local doc note is:

- [docs/aperios-scrapes/WCONSOLE-SDK-BUILD-NOTES.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/docs/aperios-scrapes/WCONSOLE-SDK-BUILD-NOTES.md)

The specific Installation Guide excerpt is:

- [docs/aperios-scrapes/InstallationGuide-WCONSOLE-RP-section.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/docs/aperios-scrapes/InstallationGuide-WCONSOLE-RP-section.txt)

## Build The AIBO-Side Stick

Run:

```bash
./scripts/prepare-ers7-sdk-wconsole-objectcomm.sh
```

That creates:

- `features/ers7-sdk-wconsole-objectcomm/build/stick`

This workflow uses:

- SDK base:
  `local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R`
- sample:
  [samples/common/ObjectComm](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-sdk/samples/common/ObjectComm)

## What This Produces

This is a pure SDK candidate:

- SDK `SYSTEM` from `WCONSOLE/nomemprot`
- `MW/OBJS/POWERMON.BIN`
- `MW/OBJS/SUBJECT.BIN`
- `MW/OBJS/OBSERVER.BIN`
- `MW/CONF/OBJECT.CFG`
- `MW/CONF/CONNECT.CFG`

The sample `CONNECT.CFG` links:

- `SampleSubject.SendString.char.S`
- to `SampleObserver.ReceiveString.char.O`

## Host-Side Follow-Up

This workflow prepares the robot-side stick only.

For the host side, the local doc recipe says to:

1. build the host program:
   ```bash
   cd samples/common/ObjectComm/RP/host
   make install
   ```
2. run:
   ```bash
   /usr/local/OPEN_R_SDK/RP_OPEN_R/bin/start-rp-openr
   ```

The repo does not yet wrap the host-side RP launch into a dedicated helper, but
this README preserves the path and local reference for it.

## Wi-Fi Settings

By default this wrapper writes:

- `HOSTNAME=AIBO`
- `ESSID=AIBONET`
- `WEPENABLE=0`
- `APMODE=1`
- `CHANNEL=3`
- `USE_DHCP=1`

You can override these by setting environment variables before running the
script.

## Why This Matters

Together with:

- [features/ers7-sdk-wconsole-helloworld/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-sdk-wconsole-helloworld/README.md)

this gives us both:

- a plain SDK `WCONSOLE/memprot` console sample
- and a plain SDK `WCONSOLE/nomemprot` RP-style sample

without involving the hybrid MIND 2 runtime path.
