# ERS-7 Debian Cribsheet

This is the shortest practical path to making progress on a Debian Linux host.

## Goal

Prove this chain works:

`Debian host -> OPEN-R toolchain -> sample build -> Memory Stick deploy -> ERS-7 boot`

Do not start with walking, vision, or big behavior changes. Start with one known-good sample.

## Track A: Native OPEN-R on Debian

This is the main path for this repo.

### 1. Stage the Sony SDK and toolchain

You need legally obtained Sony OPEN-R SDK files and whatever cross-toolchain your copy expects.

Current repo scripts assume:

```bash
OPENRSDK_ROOT=/usr/OPENR_SDK
```

But imported Sony sample makefiles often expect:

```bash
OPENRSDK_ROOT=/usr/local/OPEN_R_SDK
```

Before doing anything else, pick one path and standardize on it.

### 2. Install Debian host packages

```bash
sudo apt update
sudo apt install -y build-essential make gcc g++ flex bison patch perl unzip tar file
```

If the old SDK scripts fight modern Debian, use a dedicated container, chroot, or VM instead of trying to bend your main system around them.

### 3. Load the environment

```bash
source scripts/env.sh
```

Then verify:

```bash
echo "$OPENRSDK_ROOT"
which mipsel-linux-g++ || true
which mipsel-linux-strip || true
```

Expected outcome:

- `OPENRSDK_ROOT` points at your real SDK location
- the MIPS cross-tools are found on `PATH`

### 4. Build one Sony sample before touching custom code

Best first candidate:

```bash
cd samples/common/HelloWorld/HelloWorld
make
```

Why this sample:

- it is small
- it uses the classic Sony build flow
- it tells you whether the toolchain, includes, linker setup, and `mkbin` all work

Success means you get a `.bin` artifact from the original sample without rewriting the sample first.

### 5. Deploy only after a sample build succeeds

Use a separate programmable Memory Stick, not your working AIBO MIND stick.

Typical pattern:

```bash
./scripts/deploy-to-stick.sh /media/$USER/AIBO_STICK path/to/object.bin
```

### 6. Only then adapt `hello_body`

`src/hello_body` is currently a scaffold, not a complete OPEN-R object.

Treat it as a target for phase two:

- copy a known-good Sony sample structure
- keep the working build settings
- replace behavior gradually
- aim for LED or log output first
- add a small head-touch reaction next

## Track B: R-CODE on Debian

Treat this as optional until OPEN-R is working.

Questions to answer before spending time here:

1. Do you already have the R-CODE tools and documentation?
2. Are they usable on Linux directly, or do they assume older Windows-era tooling?
3. Is your real goal scripting MIND-era behavior, or low-level robotics control?

If your goal is low-level ERS-7 programming, OPEN-R is the better first investment.

If your goal is quick behavior customization while keeping more of Sony's consumer software model, R-CODE may still be interesting, but it should not block the OPEN-R path.

## Track C: Simulation

### Can you simulate Aperios itself?

Probably not in any practical full-system way.

I have not found evidence of a widely used, full ERS-7 Aperios emulator that boots the original Sony OS and runs OPEN-R exactly like the robot.

### What can you simulate instead?

Robot-level simulation is more realistic than OS-level emulation.

The most credible path is to use an AIBO-capable robotics simulator such as Webots, which has included AIBO ERS-7 and ERS-210 models in its robot set.

What that gives you:

- rough motion and sensor experimentation
- controller and behavior prototyping
- a safer place to test ideas before hardware

What it does not give you:

- a true Sony Aperios boot environment
- guaranteed drop-in OPEN-R binary compatibility
- perfect hardware fidelity for every ERS-7 detail

So the practical rule is:

- use real hardware for proving OPEN-R deployment
- use simulation for behavior ideas, control logic, and safer experimentation

## First milestone

Stop when all of these are true:

1. Debian can build an original Sony sample unchanged.
2. You know exactly where the SDK lives on this machine.
3. You can copy a built object to a dedicated Memory Stick.
4. The ERS-7 boots from that stick.
5. A tiny visible behavior works.

That is the point where this repo becomes a real development environment instead of a promising archive.

## Notes To Fill In

Write these down as you confirm them:

- Debian release:
- SDK location:
- cross-compiler path:
- first passing sample:
- output artifact name:
- Memory Stick mount path:
- ERS-7 model and firmware notes:
- patches needed for modern Debian:
