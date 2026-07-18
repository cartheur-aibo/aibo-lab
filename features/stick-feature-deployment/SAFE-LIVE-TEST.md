# Safe Live Test Plan

Use this plan when you want the shortest safe path from repo to real robot
test, without mixing preserved retail MIND 2 runtime and SDK `WCONSOLE`.

## Goal

Prove these in order:

1. the test stick and reader workflow are healthy
2. the robot boots calmly on the target stick
3. the robot joins Wi-Fi on the chosen network
4. the pure SDK `TinyFTPD` `WCONSOLE` payload is reachable for console and FTP

## Rule Zero

Do not start with `WCONSOLE`.

Start with the preserved baseline on the same physical stick first.

That tells you whether a later problem is:

- stick/media/formatting
- network compatibility
- or the SDK `WCONSOLE` runtime itself

## Stage 1: Baseline Safety Check

Build the preserved baseline:

```bash
./scripts/prepare-ers7m2-baseline.sh
```

Copy it to the mounted dev stick:

```bash
rsync -a --delete \
  /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m2-baseline/build/stick/ \
  /media/$USER/AIBOM2DEV/
sync
umount /media/$USER/AIBOM2DEV
```

Robot-side test:

1. Power off the ERS-7.
2. Insert the stick and WLAN card.
3. Boot the robot.
4. Listen for normal, non-alarming startup.
5. Confirm Wi-Fi association on `AIBONET`.
6. Test:
   - `ping AIBO_IP`
   - `curl -I http://AIBO_IP/`

Only continue if:

- the robot stays up
- there is no grinding or mechanical distress
- Wi-Fi works
- port `80` responds

## Stage 2: TinyFTPD WCONSOLE Test

Build the pure SDK TinyFTPD stick:

```bash
./scripts/prepare-ers7-sdk-wconsole-tinyftpd.sh
```

Copy it to the same mounted dev stick:

```bash
rsync -a --delete \
  /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-sdk-wconsole-tinyftpd/build/stick/ \
  /media/$USER/AIBOM2DEV/
sync
umount /media/$USER/AIBOM2DEV
```

Robot-side test:

1. Power off the ERS-7.
2. Insert the same dev stick and WLAN card.
3. Boot the robot.
4. Abort the test immediately if the robot sounds mechanically wrong.
5. If the robot stays calm, find its IP on the same network.
6. Test:
   - `ping AIBO_IP`
   - `telnet AIBO_IP 59000`
   - `ftp AIBO_IP`

Suggested FTP login:

- user: `anonymous`
- password: `anonymous`

## Stop Conditions

Stop immediately and roll back to baseline if any of these happen:

- grinding, straining, or repeated failed motion at boot
- failure to remain powered on
- no Wi-Fi association on a stick that just worked as baseline

That pattern points to runtime-lineage risk in the SDK `WCONSOLE` path, not a
small application bug.

## Interpretation

If baseline works and TinyFTPD fails dangerously:

- the stick is probably fine
- the network is probably fine
- the problem is probably in the pure SDK `WCONSOLE` runtime path on this robot

If both baseline and TinyFTPD work:

- you have a valid dev-stick path for OTA transfer and console debugging

## Current Observed Result

Current real-hardware result on the experimental M2:

- baseline worked normally
- TinyFTPD `WCONSOLE` produced `doo doo doo` and shutdown

So the current safe conclusion is:

- baseline remains the live-safe stick
- pure SDK `TinyFTPD` `WCONSOLE` is not currently safe on this robot
- future work should treat this as a runtime-family problem, not a copy-step
  problem

## Related Docs

- [features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/stick-feature-deployment/DEPLOY-TO-SONY-READER.md)
- [features/stick-feature-deployment/TROUBLESHOOTING.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/stick-feature-deployment/TROUBLESHOOTING.md)
- [features/boot-risk-audit/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/boot-risk-audit/README.md)
- [features/ers7m2-baseline/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m2-baseline/README.md)
- [features/ers7-sdk-wconsole-tinyftpd/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-sdk-wconsole-tinyftpd/README.md)
