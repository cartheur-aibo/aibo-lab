# AIBO MIND 2 Memory Stick Setup

This folder explains how to stage the bundled ERS-7 AIBO MIND 2 files in this
repo so they can be copied onto a Memory Stick.

## Important Size Limit

The bundled AIBO MIND 2 tree at:

- [opt/AIBO7M2](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/opt/AIBO7M2)

is about 19.7 MB on disk in this repo. That means it does **not** fit on a
blank 8 MB Sony Memory Stick as-is.

So the safe conclusion from the files currently in this repo is:

- I can stage a full AIBO MIND 2 setup for you.
- You cannot write this full setup to an 8 MB stick without trimming it.
- I do not have evidence in this repo for a known-good trimmed 8 MB AIBO MIND 2 layout.

## What This Repo Contains

The ERS-7 AIBO MIND 2 files live here:

- [opt/AIBO7M2](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/opt/AIBO7M2)

That tree already looks like a Memory Stick dump, including:

- `MEMSTICK.IND`
- `OPEN-R/`
- `PALM/`
- `StikZap.prc`

## Build A Staging Tree

Use the included script:

```bash
./scripts/stage-aibo-mind2.sh
```

That creates a ready-to-copy staging directory with the AIBO MIND 2 layout.
By default, it stages into:

- `features/aibo-mind2/build/stick`

## Write It To A Mounted Stick

If you have a compatible stick large enough for the staged layout:

```bash
rsync -a --delete features/aibo-mind2/build/stick/ /path/to/mounted-stick/
sync
```

Then eject the stick cleanly before inserting it into the ERS-7.

## What Gets Copied

The staging script copies:

- `MEMSTICK.IND`
- `OPEN-R/`
- `PALM/`
- `StikZap.prc`

from [opt/AIBO7M2](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/opt/AIBO7M2).

## Why The 8 MB Stick Is A Problem

The largest files in the bundled image include:

- `OPEN-R/MW/DATA/P/DYNAUDIO.ODA` at about 10.6 MB
- `OPEN-R/MW/DATA/E/SYSLED.ODA` at about 0.95 MB
- `OPEN-R/APERIOS.GZ` at about 0.66 MB

That is enough by itself to push the full tree well past 8 MB.

## If You Want To Try Trimming

That would be a separate recovery/research task. The risky part is that without
verified Sony documentation for a minimal retail AIBO MIND 2 footprint, trimming
files may produce a stick that boots incorrectly or loses features.

If you want, I can do that next in one of two ways:

1. Build a conservative "boots-first" trimmed candidate from this dump.
2. Build a "full-image staging only" workflow and leave trimming out.
