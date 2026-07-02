# ERS-7 MIND 2 Baseline Stick Workflow

This workflow is the preserved-only first hardware test path for larger
original Sony Memory Sticks such as 32 MB or 64 MB media.

Use this before camera-serving or any other runtime overlay work.

## Goal

Prepare a full MIND 2 stick for first boot validation with:

- the preserved known-good MIND 2 file tree
- the lab-operational Wi-Fi config from `src/ERS7M2/WLANCONF.TXT`
- no SDK system overlay beyond that WLAN injection
- a repeatable staging directory under `features/`

## Why This Exists

This baseline feature keeps the first hardware test narrow:

- preserved MIND 2 runtime and app tree
- lab WLAN config for reachability
- no extra `WCONSOLE` boot objects

## Build The Staging Tree

Run:

```bash
./scripts/prepare-ers7m2-baseline.sh
```

That creates:

- `features/ers7m2-baseline/build/stick`

and injects:

- [src/ERS7M2/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/ERS7M2/WLANCONF.TXT)

into:

- `OPEN-R/SYSTEM/CONF/WLANCONF.TXT`

## What It Uses

The script stages from:

- [features/aibo-mind2/build/stick](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/aibo-mind2/build/stick)

and injects the repo WLAN config directly without any SDK `SYSTEM` overlay.

## Copy To The Mounted Stick

Once the Sony stick is mounted at the real stick root:

```bash
rsync -a --delete /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7m2-baseline/build/stick/ /media/$USER/disk/
sync
umount /media/$USER/disk
```

Then insert the stick into the powered-off ERS-7 and boot.

For the shared Sony-reader deployment procedure, see:

- [features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md)

## Success Criteria

- robot boots normally
- no abnormal grinding or repeated posture stall during early boot
- robot joins Wi-Fi
- host can reach the robot
- `http://AIBO_IP/` opens the stock MIND 2 page

If this baseline fails, stop and debug that before trying SDK sample workflows
or app overlays.

## Next Step After A Clean Boot

After this baseline is proven on hardware, the next complete comparison paths
are:

- [features/ers7-sdk-wconsole-helloworld/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7-sdk-wconsole-helloworld/README.md)
- [features/ers7-sdk-wconsole-objectcomm/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7-sdk-wconsole-objectcomm/README.md)
