# ERS-7 8 MB Test Stick

This builds a scratch programmable ERS-7 Memory Stick from the local OPEN-R SDK,
not from the oversized AIBO MIND 2 runtime dump.

The goal is a small stick we can:

- boot on a real ERS-7
- connect over Wi-Fi
- verify through the wireless console
- keep well under the limit of an 8 MB Memory Stick

## What It Uses

The builder uses:

- the SDK's ERS-7 `WCONSOLE` base
- the Sony `HelloWorld` sample
- the Sony `PowerMonitor` sample

The `WCONSOLE` base gives us wireless networking plus a telnet console on port
`59000`, which is the easiest first validation path.

## Build It

Example for an open SSID with DHCP:

```bash
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=0 \
USE_DHCP=1 \
./scripts/build-ers7-8mb-stick.sh /tmp/ers7-8mb-stick
```

Example for WEP with DHCP:

```bash
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=1 \
WEPKEY=AIBO2 \
USE_DHCP=1 \
./scripts/build-ers7-8mb-stick.sh /tmp/ers7-8mb-stick
```

Example for a static IP:

```bash
AIBO_HOSTNAME=AIBO \
ESSID=YOUR_WIFI_NAME \
WEPENABLE=0 \
USE_DHCP=0 \
ETHER_IP=192.168.10.100 \
ETHER_NETMASK=255.255.255.0 \
IP_GATEWAY=192.168.10.1 \
DNS_SERVER_1=192.168.10.1 \
./scripts/build-ers7-8mb-stick.sh /tmp/ers7-8mb-stick
```

## What The Builder Produces

The output directory contains:

- `MEMSTICK.IND`
- `OPEN-R/`

Inside `OPEN-R/` it includes:

- the SDK's ERS-7 wireless console system
- `WLANCONF.TXT` with your Wi-Fi settings
- `HELLO.BIN`
- `POWERMON.BIN`
- the sample `MW/CONF/OBJECT.CFG`

## Why This Fits

The SDK base used here is only about `1.1 MiB` before the sample payload is
added, so it is comfortably within 8 MB.

This is very different from the `opt/AIBO7M2` dump in this repo, which is not a
verified 8 MB retail stick image.

## Write It To The Stick

After building:

```bash
rsync -a --delete /tmp/ers7-8mb-stick/ /path/to/mounted-stick/
sync
```

Then eject the stick cleanly.

## Boot Test

1. Insert the stick into the ERS-7.
2. Boot the robot.
3. Wait for Wi-Fi association or DHCP.
4. Find the ERS-7 IP from your router, or use the static IP you configured.
5. Connect with:

```bash
telnet AIBO_IP 59000
```

If the sample boots correctly, you should be able to observe the wireless
console and use that as your first proof that the stick is alive.

## What Will Happen On Boot

This stick is intentionally minimal. It does not boot AIBO MIND 2. It boots the
OPEN-R wireless console base plus two small SDK samples:

- `HELLO.BIN`
- `POWERMON.BIN`

Expected behavior:

1. The ERS-7 boots the OPEN-R `WCONSOLE` base from the stick.
2. The system loads the wireless driver and TCP/IP stack.
3. The robot applies your `WLANCONF.TXT` settings and tries to join Wi-Fi.
4. `HelloWorld` starts and prints `!!! Hello World !!!` to the wireless console.
5. `PowerMonitor` starts and registers for power-status observation.

What you should and should not expect:

- You should expect a bootable programmable OPEN-R environment.
- You should expect Wi-Fi bring-up if the AP settings are compatible.
- You should expect telnet access on port `59000` when networking succeeds.
- You should not expect AIBO MIND 2 personality, behaviors, sounds, or UI.
- You should not expect walking or expressive motions from this stick alone.

In practical terms, this is a bring-up and verification stick, not a retail
behavior stick.

## What To Look For In The Console

The most important proof point is seeing this line after connecting with telnet:

```text
!!! Hello World !!!
```

That line comes directly from the sample's `DoStart()` handler in:

- [samples/common/HelloWorld/HelloWorld/HelloWorld.cc](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/samples/common/HelloWorld/HelloWorld/HelloWorld.cc:18)

`PowerMonitor` does not print a normal startup banner. Its job is to watch power
state and help the system shut down cleanly on pause or low battery:

- [samples/common/PowerMonitor/PowerMonitor/PowerMonitor.cc](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/samples/common/PowerMonitor/PowerMonitor/PowerMonitor.cc:22)

## What Happens When You Press Pause

The `PowerMonitor` sample listens for pause or low-battery conditions. When it
sees one, it:

1. unregisters its power-status observer
2. turns motor power off
3. asks OPEN-R to shut the system down

So when you pause the robot, you should expect a clean shutdown path rather than
an AIBO MIND 2-style interactive behavior.

## Notes

- `WCONSOLE` is the default because it is best for bring-up and debugging.
- The builder defaults to `memprot` for safety.
- The Wi-Fi settings here follow the same old Sony constraints described in
  [docs/ers7-wifi/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/ers7-wifi/README.md):
  open or WEP-era networking, not WPA/WPA2/WPA3.
