# ERS-7 SDK WCONSOLE HelloWorld Workflow

This is the clean repo-native version of the plain OPEN-R SDK `WCONSOLE`
sample path described in the Aperios docs.

It is intentionally **not** a MIND 2 preservation workflow.

It exists to answer the simpler question:

- can we build and stage a normal ERS-7 OPEN-R SDK `WCONSOLE` sample exactly in
  the shape the Sony docs describe?

## Source Of Truth

The local document distillation is here:

- [docs/aperios-scrapes/WCONSOLE-SDK-BUILD-NOTES.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/docs/aperios-scrapes/WCONSOLE-SDK-BUILD-NOTES.md)

The short official pattern is:

1. build the sample
2. copy the ERS-7 SDK `WCONSOLE/memprot/OPEN-R` base
3. copy the sample's `MS/OPEN-R` overlay
4. edit `WLANCONF.TXT`
5. boot AIBO and connect to `telnet AIBO_IP 59000`

## Build The Staging Tree

Run:

```bash
./scripts/prepare-ers7-sdk-wconsole-helloworld.sh
```

That creates:

- `features/ers7-sdk-wconsole-helloworld/build/stick`

This workflow uses:

- SDK base:
  `local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R`
- sample:
  [samples/common/HelloWorld](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-sdk/samples/common/HelloWorld)

## What This Produces

This is a pure SDK candidate:

- `SYSTEM` comes from the SDK `WCONSOLE/memprot` base
- `MW/OBJS/HELLO.BIN` comes from the local HelloWorld sample build
- `MW/CONF/OBJECT.CFG` contains only:
  - `/MS/OPEN-R/MW/OBJS/HELLO.BIN`

It is a much cleaner comparison point than the hybrid MIND 2 workflows.

## Wi-Fi Settings

By default the wrapper uses:

- `HOSTNAME=AIBO`
- `ESSID=AIBONET`
- `WEPENABLE=0`
- `APMODE=1`
- `USE_DHCP=1`

You can override those with the same environment variables supported by:

- [scripts/build-ers7-feature-stick.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/scripts/build-ers7-feature-stick.sh)

## Copy To The Stick

After building, copy:

- `MEMSTICK.IND`
- `OPEN-R/`

from:

- `features/ers7-sdk-wconsole-helloworld/build/stick`

to the root of a mounted programming stick.

The generated copy note in:

- `features/ers7-sdk-wconsole-helloworld/build/COPY_TO_MS.txt`

includes an example `rsync` command.

## Expected Use

If Wi-Fi comes up, the official SDK path expects the wireless console on:

- `telnet AIBO_IP 59000`

and the HelloWorld sample should print its message after the OPEN-R system
startup text.

## Why This Matters

This workflow gives us a simple baseline for:

- plain SDK `WCONSOLE` viability on ERS-7
- independent of the preserved retail MIND 2 runtime
- independent of the mixed-provenance `SYSTEM` questions

If this path fails, that is evidence about the SDK path itself.

If this path works while the MIND 2 hybrid path fails, that strongly suggests
the trouble is in the hybridization layer rather than in `WCONSOLE` alone.
