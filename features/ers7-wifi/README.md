# ERS-7 Wi-Fi Setup

This folder turns the Sony OPEN-R documentation in this repo into a practical
checklist for getting an ERS-7 onto Wi-Fi.

## What You Need

- an ERS-7
- a programmable Memory Stick with an `OPEN-R` tree on it
- the `WLAN` or `WCONSOLE` OPEN-R system files
- a 2.4 GHz network that the ERS-7 can join

## Important Limitation

The Sony configuration files and manuals in this repo describe:

- open Wi-Fi
- WEP Wi-Fi
- 802.11b-era wireless modes

They do not describe WPA, WPA2, or WPA3 configuration. In practice, that means
you will usually need either:

- a separate legacy SSID with no encryption, or
- a separate legacy SSID with WEP enabled

This is an inference from the available configuration fields in the Sony files:

- `ESSID`
- `WEPENABLE`
- `WEPKEY`
- `APMODE`
- `CHANNEL`

There are no WPA-related settings in the documented configuration format.

## Source Files In This Repo

The most useful files here are:

- [docs/InstallationGuide_E.pdf](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/InstallationGuide_E.pdf)
- [docs/AIBO-quickstart.pdf](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/AIBO-quickstart.pdf)
- [opt/AIBO7M2/OPEN-R/SYSTEM/CONF/SAMPLE/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/opt/AIBO7M2/OPEN-R/SYSTEM/CONF/SAMPLE/WLANCONF.TXT:1)
- [sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R/SYSTEM/CONF/WLANDFLT.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R/SYSTEM/CONF/WLANDFLT.TXT:1)

## Step 1: Use A Wireless-Capable OPEN-R Layout

The Sony docs describe three OPEN-R base layouts:

- `BASIC`: no wireless LAN
- `WLAN`: wireless LAN, no wireless console
- `WCONSOLE`: wireless LAN plus wireless console

For setup and debugging, `WCONSOLE` is the easiest choice.

If you are preparing a fresh programmable stick from the SDK included in this
repo, start from one of these:

- `sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WLAN/memprot/OPEN-R`
- `sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WLAN/nomemprot/OPEN-R`
- `sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/memprot/OPEN-R`
- `sdk/local/OPEN_R_SDK/OPEN_R/MS_ERS7/WCONSOLE/nomemprot/OPEN-R`

Copy the `OPEN-R` directory to the root of the programmable Memory Stick.

## Step 2: Create `WLANCONF.TXT`

Sony documents two files:

- `WLANDFLT.TXT`: default wireless config, do not edit directly
- `WLANCONF.TXT`: your custom wireless config

When the ERS-7 boots, it uses `WLANCONF.TXT` first if present.

Start by copying:

- `WLANDFLT.TXT` to `WLANCONF.TXT`

or use the example file in this folder:

- [WLANCONF.example.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/docs/ers7-wifi/WLANCONF.example.TXT)

Place the finished file here on the stick:

- `/OPEN-R/SYSTEM/CONF/WLANCONF.TXT`

## Step 3: Pick Your Network Mode

The Sony docs define:

- `APMODE=1` for infrastructure mode through an access point
- `APMODE=0` for ad-hoc demo mode
- `APMODE=2` for automatic detection

For most home or lab Wi-Fi setups, use:

- `APMODE=1`

That is the clearest option when joining a router or access point.

## Step 4: Fill In The Settings

### Easiest setup: DHCP through an access point

Edit `WLANCONF.TXT` like this:

```txt
HOSTNAME=AIBO
ESSID=YOUR_WIFI_NAME
WEPENABLE=0
APMODE=1
USE_DHCP=1
```

Notes:

- `HOSTNAME` can be up to 8 alphanumeric characters and must include a letter
- if `WEPENABLE=0`, leave `WEPKEY` commented out or remove it
- `ESSID` must exactly match the Wi-Fi name

### If your Wi-Fi uses WEP

```txt
HOSTNAME=AIBO
ESSID=YOUR_WIFI_NAME
WEPENABLE=1
WEPKEY=AIBO2
APMODE=1
USE_DHCP=1
```

Sony documents these WEP key forms:

- 5 or 13 alphanumeric characters
- or hexadecimal with `0x...`

### If you want a fixed IP

```txt
HOSTNAME=AIBO
ESSID=YOUR_WIFI_NAME
WEPENABLE=0
APMODE=1
USE_DHCP=0
ETHER_IP=192.168.10.100
ETHER_NETMASK=255.255.255.0
IP_GATEWAY=192.168.10.1
DNS_SERVER_1=192.168.10.1
```

If you do not have a real gateway on that network, Sony says to set
`IP_GATEWAY` to the same value as `ETHER_IP`.

## Step 5: Boot The ERS-7

1. Power the ERS-7 off.
2. Insert the programmable Memory Stick.
3. Boot the robot.
4. Give it a little time to associate with the network and obtain DHCP if used.

## Step 6: Verify The Connection

### Best method: wireless console

If you used a `WCONSOLE` layout, Sony says the ERS-7 exposes a telnet console on
port `59000`.

From your host:

```bash
telnet AIBO_IP 59000
```

If you do not know the IP yet:

- check your router's DHCP lease table, or
- use the fixed IP you configured

### Built-in web UI

The AIBO MIND files on the stick include a browser UI under:

- [opt/AIBO7M2/OPEN-R/APP/PC/INDEX.HTM](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/opt/AIBO7M2/OPEN-R/APP/PC/INDEX.HTM:1)

That UI includes a "Connection Check" page:

- [opt/AIBO7M2/OPEN-R/APP/PC/AIBOMAIL/AMNWSTAT.FRM](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/opt/AIBO7M2/OPEN-R/APP/PC/AIBOMAIL/AMNWSTAT.FRM:1)

This is more about AIBO EYES mail connectivity than raw Wi-Fi association, but
it is another clue that the network stack is alive.

## Recommended First Try

If you just want the highest chance of success on first boot:

1. Use `WCONSOLE`.
2. Use a dedicated 2.4 GHz SSID.
3. Set that SSID to open security first, if your lab setup allows it.
4. Use `APMODE=1`.
5. Use `USE_DHCP=1`.
6. Set `HOSTNAME=AIBO`.
7. Telnet to port `59000` after boot.

## Troubleshooting

### It does not connect at all

- Make sure the network is 2.4 GHz, not 5 GHz only.
- Make sure the SSID exactly matches `ESSID`.
- If using WEP, re-check the key length and format.
- If your router only offers WPA/WPA2/WPA3, the ERS-7 will likely not join it.
- A matching SSID on a modern guest network is not enough by itself. In local
  testing, the robot joined an older-style mobile hotspot much more reliably
  than a modern router guest SSID with the same `AIBONET` name.

### `APMODE` confusion

The SDK's default `WLANDFLT.TXT` for ERS-7 uses `APMODE=2`, which Sony
documents as auto-detect. The live runtime dump in this repo uses `APMODE=1`.

For a normal access point, `APMODE=1` is the least ambiguous choice.

### DHCP never gives an address

- Try `USE_DHCP=0` with a static IP.
- Confirm your AP allows very old 802.11b clients.
- Check the router for blocked legacy security or client isolation settings.

### You want direct PC-to-AIBO wireless

Sony also documents ad-hoc setups. For that:

- match `ESSID`, `WEPENABLE`, and `WEPKEY` with the PC
- use `APMODE=0` for ad-hoc demo mode and set `CHANNEL`
- or use `APMODE=1` for IBSS peer-to-peer mode if your adapter supports it as
  Sony describes

For most people today, using a dedicated access point is much easier.

## Quick Checklist

- use `WLAN` or `WCONSOLE`, not `BASIC`
- put `WLANCONF.TXT` in `/OPEN-R/SYSTEM/CONF/`
- use `APMODE=1` for normal Wi-Fi through an AP
- use `USE_DHCP=1` unless you need a fixed IP
- expect open or WEP only
- use `telnet <ip> 59000` if you chose `WCONSOLE`
