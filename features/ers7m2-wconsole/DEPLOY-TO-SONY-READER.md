# Deploy `ers7m2-wconsole` To A Sony USB Memory Stick Reader

Use this workflow to copy the staged ERS-7 MIND 2 WCONSOLE payload onto a
larger original Sony Memory Stick mounted through a Sony USB reader.

Do not use your preserved working stick for this. Use a separate test stick.

## 1. Prepare The Staged Payload

Default preserved MIND 2 system:

```bash
./scripts/prepare-ers7m2-wconsole.sh
```

If you want the wireless console variant as well:

```bash
./scripts/prepare-ers7m2-wconsole.sh
```

This creates:

- `features/ers7m2-wconsole/build/stick`

The staged tree already contains the two items that must exist at the stick
root:

- `MEMSTICK.IND`
- `OPEN-R/`

## 2. Insert The Stick In The Sony Reader

Plug the Sony USB Memory Stick reader into the host and insert the target
Memory Stick.

Wait for it to mount, then identify the actual mount point. On this machine the
first place to check is:

```bash
ls /media/$USER
```

If the reader mounted the stick as `disk`, the stick root will usually be:

```text
/media/$USER/disk/
```

## 3. Verify You Have The Stick Root

Do a real check before copying:

```bash
test -f /media/$USER/disk/MEMSTICK.IND && test -d /media/$USER/disk/OPEN-R
```

That should succeed silently.

For visible confirmation:

```bash
ls /media/$USER/disk
```

You should see at least:

```text
MEMSTICK.IND
OPEN-R
```

If that check fails, stop. You do not have the correct mounted stick root yet.

## 4. Copy The Payload To The Mounted Stick

Use the real mount path from the Sony USB reader. If it mounted at
`/media/$USER/disk`, copy with:

```bash
rsync -a --delete \
  /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7m2-wconsole/build/stick/ \
  /media/$USER/disk/
sync
```

What this does:

- copies the staged payload to the stick root
- removes files on the target that are not present in the staged tree
- flushes writes before removal

## 5. Eject Cleanly

Unmount the Memory Stick cleanly from the host after `sync` completes:

```bash
umount /media/$USER/disk
```

Do not pull the reader or stick early.

## 6. Boot Test

1. Power the ERS-7 off.
2. Insert the programmed stick.
3. Boot the robot.
4. Put your host on the same Wi-Fi network defined by
   `src/ERS7M2/WLANCONF.TXT`.
5. Find the robot IP from your hotspot or router.
6. Test `ping AIBO_IP`.
7. Test `http://AIBO_IP/`.
8. If you built with `SYSTEM_FLAVOR=WCONSOLE`, also test:

```bash
telnet AIBO_IP 59000
```

## Expected Wi-Fi Settings

The staged test-stick workflow injects:

- `HOSTNAME=AIBOM2A`
- `ESSID=AIBONET`
- `WEPENABLE=0`
- `APMODE=1`
- `USE_DHCP=1`

Those values come from:

- [src/ERS7M2/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/ERS7M2/WLANCONF.TXT)

## One-Line Copy Command

If your Sony reader mounts the stick at `/media/$USER/disk`, this is the full
deployment command:

```bash
./scripts/prepare-ers7m2-wconsole.sh
rsync -a --delete \
  /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/ers7m2-wconsole/build/stick/ \
  /media/$USER/disk/
sync
```
