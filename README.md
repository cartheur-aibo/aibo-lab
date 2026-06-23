# openr-debian for ERS-7

Debian Linux development scaffold for programming the Sony AIBO ERS-7 with OPEN-R.

This repository targets an ERS-7 / ERS-7M2 style workflow: build OPEN-R objects on Debian, copy them to a dedicated AIBO Memory Stick, and boot the robot from that stick for development.

> Legal note: this repository does **not** include Sony OPEN-R SDK files, Aperios system files, AIBO MIND software, Memory Stick images, or proprietary binaries. It expects you to install legally obtained SDK/system files separately.

## Target platform

- Robot: Sony AIBO ERS-7
- Companion software context: AIBO MIND 2, but development uses a separate OPEN-R boot Memory Stick
- SDK: OPEN-R SDK 1.1.5-era tooling
- Host OS: Debian Linux
- Language: C++ using OPEN-R object model

## Why OPEN-R?

OPEN-R exposes the low-level robotics side of AIBO: joints, sensors, LEDs, audio, camera, and wireless networking. It runs on Sony Aperios, a message-oriented cooperative multitasking OS. OPEN-R objects should process messages quickly and return control rather than blocking in long loops.

## Repository layout

```text
.
├── README.md
├── tasks/
│   ├── debian-setup.md
│   ├── memory-stick.md
│   ├── openr-model.md
│   ├── first-project.md
│   └── cribsheet.md
├── scripts/
│   ├── env.sh
│   ├── check-openr.sh
│   └── deploy-to-stick.sh
├── src/
│   └── hello_body/
│       ├── Makefile
│       ├── HelloBody.cc
│       ├── HelloBody.h
│       └── README.md
└── stick/
    └── OPEN-R/
        ├── APP/PC/
        ├── MW/OBJS/
        └── SYSTEM/conf/
```

## Quick start

```bash
git clone https://github.com/cartheur-aibo/openr-debian.git
cd openr-debian

# Edit if your SDK path differs
source scripts/env.sh

# Verify expected directories/tools
./scripts/check-openr.sh

# Build a known-good Sony sample first
cd samples/common/HelloWorld/HelloWorld
make
```

For the practical Debian bring-up order, see `tasks/cribsheet.md`.

## Development rule

Use a **separate programmable Memory Stick**. Do not modify your working AIBO MIND 2 stick.

## First milestone

The first useful program should do only three things:

1. Boot successfully from the OPEN-R Memory Stick.
2. Flash or set an ERS-7 face/body LED state.
3. React to a touch sensor with a small head movement or logged event.

That proves the full build/deploy/boot/control path before you attempt walking, vision, or autonomous behavior.
