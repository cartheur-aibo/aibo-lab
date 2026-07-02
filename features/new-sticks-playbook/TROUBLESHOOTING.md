# New Sticks Troubleshooting

This guide is the operational companion to
[features/new-sticks-playbook/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/new-sticks-playbook/README.md).

Use it when the process feels delicate, ambiguous, or easy to derail.

## Core Rule

Change one variable at a time.

For this project, the variables that matter most are:

- stick contents
- stick media and formatting
- Wi-Fi access-point compatibility
- robot boot mode and expected service

Do not change more than one of those between tests unless there is a very good
reason.

## Preflight Checklist

Before touching a stick, confirm:

- the intended workflow is clear:
  `MIND 2` or `W3AIBO`
- the correct staged tree exists
- the host knows which Wi-Fi network is being used
- the target network is compatible enough to test with the ERS-7
- the robot is powered off before stick insertion/removal

## Canonical Staging Directories

For stock MIND 2 bring-up:

- `features/ers7m2-wconsole/build/stick`

For camera-stream testing:

- `features/ers7-camera-stream/build/stick`

Preserved known-good reference tree:

- `features/aibo-mind2/build/stick`

Known-good live WLAN config:

- `src/ERS7M2/WLANCONF.TXT`

## Known-Good Network Baseline

These values were actually observed working:

- SSID: `AIBONET`
- network type: mobile hotspot
- robot IP: `192.168.43.8`
- host could reach the robot by `ping`
- host could fetch `http://192.168.43.8/`

Do not assume a home router or guest SSID is equivalent just because it uses
the same ESSID.

## Stick Preparation Checklist

Before copying to a new stick:

- prepare the staged tree first
- verify the staged tree contains `MEMSTICK.IND`
- verify the staged tree contains `OPEN-R/`
- verify the expected object binaries are present
- verify the intended `WLANCONF.TXT` is present

For MIND 2:

- expect the staged tree to be copied from `features/aibo-mind2/build/stick`

For camera stream:

- expect `OPEN-R/MW/OBJS/W3AIBO.BIN`
- expect `OPEN-R/MW/OBJS/POWERMON.BIN`

## Copy Checklist

When copying a staged tree to mounted media:

- mount the correct device
- make sure it is writable
- copy the tree to the root of the mounted stick
- run `sync`
- unmount/eject cleanly before removal

Use:

```bash
rsync -a --delete /path/to/staged-stick/ /path/to/mounted-stick/
sync
```

## Fast Sanity Checks Before Robot Test

Before inserting a newly prepared stick into the ERS-7:

- confirm you copied from the right staging directory
- confirm the mountpoint was the actual stick
- confirm no copy step failed silently
- confirm the stick was cleanly ejected
- confirm the robot is being tested against the intended network

## Expected Test Sequences

### Sequence A: Stock MIND 2

1. Prepare `features/ers7m2-wconsole/build/stick`.
2. Copy it to the new larger stick.
3. Boot the robot.
4. Join the same compatible Wi-Fi network from the host.
5. Find the robot IP.
6. Test:

```bash
ping AIBO_IP
curl -I http://AIBO_IP/
```

Expected success pattern:

- robot stays on
- robot joins Wi-Fi
- `ping` succeeds
- HTTP on port `80` responds

### Sequence B: Camera Stream

1. Prepare `features/ers7-camera-stream/build/stick`.
2. Copy it to the new larger stick.
3. Boot the robot.
4. Join the same compatible Wi-Fi network from the host.
5. Find the robot IP.
6. Test:

```bash
curl -I http://AIBO_IP:60080/
curl -I http://AIBO_IP:60080/layerhr
```

Expected success pattern:

- robot stays on
- robot joins Wi-Fi
- HTTP on port `60080` responds

## Failure Matrix

### Symptom: Sad sound, quick power-off

Treat this first as a boot-medium or boot-layout problem, not a Wi-Fi problem.

Check next:

- was the stick copied from the correct staged tree
- was the stick properly unmounted/ejected
- is this a newly prepared larger Sony stick or an older 8 MB experiment
- are we testing stock MIND 2 first before overlay/custom work

If this happens on a larger stick with a pure MIND 2 staging tree, compare the
staged tree directly against `features/aibo-mind2/build/stick`.

### Symptom: Robot stays up but never appears on Wi-Fi

Treat this first as a network compatibility or WLAN config problem.

Check next:

- is the hotspot/router actually active
- is the host on the same network
- does the staged tree contain the intended `WLANCONF.TXT`
- are we using the same open hotspot pattern that already worked

Prefer the known-working mobile hotspot baseline over a home router while
debugging.

### Symptom: Robot has IP but HTTP port `80` is closed

If testing stock MIND 2, this suggests the boot path may not actually be the
expected MIND 2 service stack.

Check next:

- did we stage the MIND 2 workflow, not the camera-stream one
- does the copied stick still contain the expected MIND 2 app tree
- can the host still `ping` the robot

### Symptom: Robot has IP, port `80` works, but `59000` is closed

This is not a contradiction.

It simply means we proved stock MIND 2 HTTP reachability, not `WCONSOLE`.

Do not block MIND 2 success on `59000`.

### Symptom: Robot has IP, but camera port `60080` is closed

Treat this first as a programmable-stick app issue.

Check next:

- did we use `features/ers7-camera-stream/build/stick`
- does the staged tree contain `W3AIBO.BIN`
- does `OBJECT.CFG` reference `W3AIBO.BIN` and `POWERMON.BIN`
- are we sure we are not still booting stock MIND 2 media

## Decision Ladder

Follow this order when something fails:

1. Confirm the correct workflow was chosen.
2. Confirm the correct staged tree was generated.
3. Confirm the staged tree was copied to the actual mounted stick.
4. Confirm the stick was cleanly synced and ejected.
5. Confirm the robot stayed powered on.
6. Confirm the robot joined Wi-Fi.
7. Confirm the host is on the same network.
8. Confirm the expected service for that workflow responds.

Do not jump straight to low-level formatting theory unless a larger known-good
Sony stick fails even with the clean stock MIND 2 workflow.

## What To Record During Each Test

For every real-hardware test, record:

- date
- stick type and capacity
- workflow used
- staging directory used
- network used
- robot IP, if any
- whether the robot stayed on
- whether `ping` worked
- which ports responded
- what changed from the previous test

This matters because subtle drift is easy to misremember.

## Commands Worth Reusing

Prepare stock MIND 2 on larger sticks:

```bash
./scripts/prepare-ers7m2-wconsole.sh
```

Prepare camera stream:

```bash
JPEG_ARCHIVE=install/jpegsrc.v6b.tar.gz ./scripts/prepare-ers7-camera-stream.sh
```

Copy staged tree:

```bash
rsync -a --delete /path/to/staged-stick/ /path/to/mounted-stick/
sync
```

Basic reachability:

```bash
ping AIBO_IP
curl -I http://AIBO_IP/
curl -I http://AIBO_IP:60080/
```

## Practical Bottom Line

The safest next-step pattern is:

1. use the larger original Sony stick
2. start with the stock MIND 2 workflow
3. prove Wi-Fi and port `80`
4. only then try overlays or `W3AIBO`

That sequence gives the cleanest signal and the least confusion.
