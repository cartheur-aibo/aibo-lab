# HelloWorld ERS-7 Test Stick

This feature builds a minimal programmable ERS-7 Memory Stick for first bring-up
on real hardware. It is for proving the OPEN-R base boots, Wi-Fi comes up, and
the wireless console works. It is not AIBO MIND 2.

## Do This

Open Wi-Fi with DHCP:

```bash
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=0 \
USE_DHCP=1 \
./scripts/build-ers7-feature-stick.sh
```

Smallest debugging build:

```bash
PAYLOAD_MODE=hello-only \
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=0 \
USE_DHCP=1 \
./scripts/build-ers7-feature-stick.sh
```

Smallest possible boot test:

```bash
PAYLOAD_MODE=base-only \
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=0 \
USE_DHCP=1 \
./scripts/build-ers7-feature-stick.sh
```

WEP with DHCP:

```bash
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=1 \
WEPKEY=YOUR_WEP_KEY \
USE_DHCP=1 \
./scripts/build-ers7-feature-stick.sh
```

Static IP:

```bash
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=0 \
USE_DHCP=0 \
ETHER_IP=192.168.10.100 \
ETHER_NETMASK=255.255.255.0 \
IP_GATEWAY=192.168.10.1 \
DNS_SERVER_1=192.168.10.1 \
./scripts/build-ers7-feature-stick.sh
```

## Copy To Memory Stick

Copy these from:

- `features/hello-world/build/stick`

to the root of the mounted Memory Stick:

- `MEMSTICK.IND`
- `OPEN-R/`

Fastest way:

```bash
rsync -a --delete features/hello-world/build/stick/ /path/to/mounted-stick/
sync
```

Then:

1. Unmount or eject the stick cleanly.
2. Insert it into the ERS-7 while powered off.
3. Boot the robot.
4. Find its IP from DHCP, or use the static IP you configured.
5. Connect with `telnet AIBO_IP 59000`.

## Formatting Milestone

We now have a known-good Sony/AIBO reference stick to compare against.

Observed on the original ERS-210 stick:

- disk label type: `dos`
- one primary bootable partition
- partition type: `Id 1` / `FAT12`
- partition start: sector `25`
- sector size: `512`
- cluster size: `8192`
- reserved sectors: `1`
- FATs: `2`
- sectors per FAT: `3`
- root directory entries: `512`
- hidden sectors: `25`

This matters because AIBO appears to care about more than just the file tree.
`OPEN-R/` plus `MEMSTICK.IND` is necessary, but not sufficient if the stick is
formatted with modern defaults the robot does not like.

## Experimental Stick Recovery Path

Use this order:

1. Match the known-good stick as closely as possible.
2. Use `FAT12`, not `FAT16`.
3. Use one primary bootable partition.
4. Avoid modern partition alignment like sector `2048`.
5. Copy only `MEMSTICK.IND` and `OPEN-R/`.
6. Unmount cleanly before every boot test.

Read-only checks for a reference stick:

```bash
sudo /sbin/fdisk -l /dev/sdX
sudo /sbin/fsck.fat -vn /dev/sdX1
sudo /usr/bin/file -s /dev/sdX1
```

Working target geometry from the known-good ERS-210 stick:

```text
Disklabel type: dos
Partition:      /dev/sdX1
Boot flag:      set
Type:           FAT12
Start sector:   25
Sector size:    512
Cluster size:   8192
Reserved:       1
FATs:           2
FAT size:       3 sectors
Root entries:   512
Hidden sectors: 25
```

If you rebuild the experimental stick by hand, the goal is to reproduce that
layout first and only then test `base-only`, `BASIC`, `hello-only`, and the
full `hello-plus-powermon` payload.

Exact formatter command that matched the known-good ERS-210 geometry:

```bash
sudo /sbin/mkfs.fat -a -F 12 -S 512 -s 16 -R 1 -f 2 -r 512 -h 25 -g 2/16 -M 0xf8 /dev/sdX1
```

The important detail is `-a`. Without it, `mkfs.fat` aligns the filesystem
internals upward and does not reproduce the original Sony layout closely
enough.

## What We Proved

We now know all of this from hardware testing:

1. The experimental 8 MB stick can be formatted to match a known-good Sony
   AIBO-era `FAT12` layout.
2. The ERS-210 original stick was a valid guide for low-level disk geometry:
   `dos` MBR, bootable primary partition, `FAT12`, start sector `25`, `8 KB`
   clusters.
3. The real ERS-7 dog stick is not a simple SDK programming stick. It is a
   richer runtime/application image with `APP/`, many more objects, and binary
   or encoded config files such as `OPEN-R/MW/CONF/OBJECT.CFG`.
4. The staged `hello-world` stick is correctly based on Sony's minimal SDK
   `MS_ERS7` trees. It is not obviously missing files due to our build script.
5. Even after matching the known-good `FAT12` geometry, the robot still makes
   the same failure sound and shuts down.

## Conclusion So Far

The current evidence points away from "simple bad formatting" and away from
"forgot to copy one SDK file."

The strongest remaining possibilities are:

1. The specific experimental 8 MB media is still not acceptable to the robot
   even though its on-disk layout now matches the reference geometry.
2. The minimal SDK `MS_ERS7` programming base is not sufficient for this
   particular robot or firmware in practice.
3. There is another ERS-7-specific boot expectation not captured by the basic
   SDK programming-stick workflow.

For today, the honest conclusion is:

- we solved the stick-formatting question well enough to stop treating it as a
  blind guess
- we proved the real ERS-7 stick is a different kind of image from the SDK
  programming stick
- we have not yet proved that this robot will boot from this experimental 8 MB
  programming stick, even when correctly formatted

## What You Should See

If it worked, the main proof point is:

```text
!!! Hello World !!!
```

That comes from the sample's `DoStart()` handler in
[samples/common/HelloWorld/HelloWorld/HelloWorld.cc](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/samples/common/HelloWorld/HelloWorld/HelloWorld.cc:18).

## What This Stick Actually Boots

This stick boots:

- the ERS-7 `WCONSOLE` OPEN-R base
- optionally `HELLO.BIN`
- optionally `POWERMON.BIN`

That matters because the object names on the stick, not the host folder names,
control what the robot runs. The boot list lives in:

- [samples/common/HelloWorld/MS/OPEN-R/MW/CONF/OBJECT.CFG](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/samples/common/HelloWorld/MS/OPEN-R/MW/CONF/OBJECT.CFG)

`PowerMonitor` is there to handle pause and low-battery shutdown cleanly. Its
logic is in:

- [samples/common/PowerMonitor/PowerMonitor/PowerMonitor.cc](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/samples/common/PowerMonitor/PowerMonitor/PowerMonitor.cc:22)

## Useful Knowledge

- The builder is reusable. By default it derives the feature output folder from
  the sample name, so `HelloWorld` maps to `features/hello-world/`.
- Generated outputs stay under `features/hello-world/build/`, not in `samples/`.
- The SDK `WCONSOLE` base is small, about `1.1 MiB` before adding the sample
  payload, so this fits easily on an 8 MB Memory Stick.
- Wi-Fi here follows the old Sony constraints in
  [features/ers7-wifi/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-wifi/README.md):
  open or WEP-era networking, not WPA/WPA2/WPA3.

## Debugging Path

Use the stick experimentally in this order:

1. Start with `PAYLOAD_MODE=base-only`.
   This tests the OPEN-R base with no sample objects at all.
2. If `base-only` still fails, try `STICK_FLAVOR=BASIC` to remove the wireless
   stack too.
3. Move to `PAYLOAD_MODE=hello-only`.
   This adds `HELLO.BIN` but still leaves `POWERMON.BIN` out.
4. If `hello-only` boots and prints `!!! Hello World !!!`, move to the default
   `hello-plus-powermon` mode.
5. After any failed boot, remount the stick and check for `OPEN-R/EMON.LOG`.
6. Keep the stick root clean.
   Copy only `MEMSTICK.IND` and `OPEN-R/`.
7. Rule out power issues early.
   A weak battery can cause failed boots that look like software problems.
