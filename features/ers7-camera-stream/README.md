# ERS-7 Camera Stream Workflow

This workflow prepares the `W3AIBO` camera-serving sample as a real ERS-7
programmable stick payload.

It is the closest match in this repo to the old "view AIBO camera over the
network" capability.

## What This Is

This is not the stock AIBO MIND 2 photo album on port `80`.

This is a separate OPEN-R sample that:

- boots from the ERS-7 `WCONSOLE/nomemprot` programmable-stick base
- captures camera frames from the robot
- serves images over HTTP on port `60080`

Sony's sample documentation says these URLs should work after boot:

- `http://AIBO_IP:60080/`
- `http://AIBO_IP:60080/layerh`
- `http://AIBO_IP:60080/layerhr`
- `http://AIBO_IP:60080/layerm`
- `http://AIBO_IP:60080/layermr`
- `http://AIBO_IP:60080/layerl`
- `http://AIBO_IP:60080/layerlr`

## Why We Are Parking This For New Sticks

This path is a better fit for the larger original Sony sticks than for the
8 MB experiments.

It also depends on the Independent JPEG Group source archive:

- `jpegsrc.v6b.tar.gz`

without that archive, `W3AIBO` cannot be built.

## Build The Camera Stream Staging Tree

Once the JPEG archive is available, run:

```bash
./scripts/prepare-ers7-camera-stream.sh
```

That script:

- copies the ERS-7 SDK `WCONSOLE/nomemprot` base
- builds `W3AIBO.BIN` and `POWERMON.BIN`
- stages the camera-stream stick under:
  `features/ers7-camera-stream/build/stick`
- injects the known-good Wi-Fi config from:
  [src/ERS7M2/WLANCONF.TXT](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/src/ERS7M2/WLANCONF.TXT)

## Build Requirements

The script expects:

- the local SDK at `local/OPEN_R_SDK`
- the sample source at `samples/common/W3AIBO`
- `jpegsrc.v6b.tar.gz` at `samples/common/W3AIBO/jpegsrc.v6b.tar.gz`

You can override paths with environment variables:

- `OPENRSDK_ROOT`
- `SAMPLE_DIR`
- `JPEG_ARCHIVE`
- `WLANCONF_SOURCE`

## What Gets Copied To The Stick

The staged tree is meant to be copied to the root of a mounted Memory Stick:

- `MEMSTICK.IND`
- `OPEN-R/`

The sample's `OBJECT.CFG` should boot:

- `W3AIBO.BIN`
- `POWERMON.BIN`

## Copy To A Mounted Stick

After the script succeeds:

```bash
rsync -a --delete /home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-camera-stream/build/stick/ /path/to/mounted-stick/
sync
```

Then:

1. Eject the stick cleanly.
2. Insert it into the ERS-7 while powered off.
3. Boot the robot.
4. Put this machine on the same compatible Wi-Fi network.
5. Probe `http://AIBO_IP:60080/`.

## Expected Result

If the stick boots correctly and Wi-Fi is live, this machine should be able to
fetch camera images from the robot over HTTP.

That would be a stronger milestone than the current MIND 2 proof on port `80`,
because it would confirm:

- programmable-stick boot on larger media
- sample camera capture
- sample network serving on port `60080`

## Relationship To The Current Wi-Fi Milestone

We already proved:

- the ERS-7 can join a compatible mobile hotspot
- the robot is reachable from this machine over Wi-Fi
- the stock MIND 2 web server responds on port `80`

So the remaining work for this feature is mainly:

1. build `W3AIBO`
2. deploy it to larger media
3. verify port `60080`

