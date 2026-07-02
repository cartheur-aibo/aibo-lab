# ERS-7 SDK WCONSOLE TinyFTPD Workflow

This is the clean repo-native OPEN-R SDK path for a wireless-console stick that
also exposes FTP for over-the-air file transfer.

It follows the same pure-SDK pattern as the other WCONSOLE features:

- use the SDK `WCONSOLE/nomemprot` base
- build the local `TinyFTPD` sample
- copy the sample `MS/OPEN-R` overlay
- write `WLANCONF.TXT`

## Why This Matters

For your experimental AIBO M2 goal, this is the closest clean SDK-native match
to:

- wireless console on port `59000`
- OTA file transfer over FTP
- behavior-debug sample work without touching the preserved retail MIND 2 tree

It is still a programmable-stick SDK workflow, not a proven retail MIND 2
overlay.

## Source Of Truth

The local distilled doc note is:

- [docs/aperios-scrapes/WCONSOLE-SDK-BUILD-NOTES.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/aperios-scrapes/WCONSOLE-SDK-BUILD-NOTES.md)

The strongest local sample hint is:

- [samples/common/ImageCapture/README_E.txt](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/samples/common/ImageCapture/README_E.txt)

That sample says image data can be retrieved over wireless LAN if `TinyFTPd` is
running.

## Build The AIBO-Side Stick

Run:

```bash
./scripts/prepare-ers7-sdk-wconsole-tinyftpd.sh
```

That creates:

- `features/ers7-sdk-wconsole-tinyftpd/build/stick`

This workflow uses:

- SDK base:
  `sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R`
- sample:
  [samples/common/TinyFTPD](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/samples/common/TinyFTPD)

## What This Produces

This is a pure SDK candidate:

- SDK `SYSTEM` from `WCONSOLE/nomemprot`
- `MW/OBJS/POWERMON.BIN`
- `MW/OBJS/TINYFTPD.BIN`
- `MW/CONF/OBJECT.CFG`
- `MW/CONF/PASSWD`

The staged tree keeps the SDK `CONNECT.CFG` and other base config files unless
the sample overrides them.

## Real Hardware Result On Experimental ERS-7 M2

Observed result on the current experimental M2:

- preserved `ers7m2-baseline` booted normally on the same stick
- this pure SDK `TinyFTPD` `WCONSOLE/nomemprot` payload caused
  `doo doo doo` and shutdown

So this workflow is currently:

- reproducible as a host-side build
- useful as a pure SDK reference
- not a safe live deployment path for this experimental M2 at this time

That strongly suggests the failure is in runtime lineage and boot behavior, not
in basic stick copying or a trivial sample-build mistake.

## FTP Behavior

The sample source config defines:

- control port `21`
- data port `20`
- passive data port `3000`
- password file path `/MS/OPEN-R/MW/CONF/PASSWD`

The default sample `PASSWD` file allows:

- `anonymous` / `anonymous` rooted at `/MS/`
- `guest` / any password rooted at `/MS/OPEN-R/MW/`

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

## Expected Robot-Side Tests

After deployment and boot:

```bash
ping AIBO_IP
telnet AIBO_IP 59000
ftp AIBO_IP
```

If `ftp` connects, try logging in as `anonymous` and listing `/MS/`.

## Important Scope Note

What this feature tells us:

- whether a complete pure-SDK `WCONSOLE` + `TinyFTPD` stick can be built and
  staged reproducibly
- whether the robot accepts that SDK lineage on the target media
- whether OTA transfer and console debugging are reachable in one clean sample

What it does not yet prove:

- that mixing `WCONSOLE` into the preserved retail MIND 2 runtime is safe
- that a grinding or shutdown real-robot boot can be solved by this feature
  alone

## Related Features

- [features/ers7m2-baseline/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7m2-baseline/README.md)
- [features/ers7-sdk-wconsole-helloworld/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7-sdk-wconsole-helloworld/README.md)
- [features/ers7-sdk-wconsole-objectcomm/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7-sdk-wconsole-objectcomm/README.md)
