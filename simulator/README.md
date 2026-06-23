# OPEN-R Simulator Plan

This repository can now build real OPEN-R payloads for ERS-7 on Debian, but a full Sony Aperios emulator is still out of scope.

What we can do usefully on Debian is split "simulator" into layers and prove each layer separately.

## What A Simulator Should Prove

For OPEN-R on AIBO, a useful simulator should answer these questions:

1. Will the Memory Stick boot layout make sense to Aperios?
2. Which OPEN-R objects will be loaded, and in what configuration?
3. Can our application logic survive lifecycle events like init, start, stop, and destroy?
4. Can we replay fake sensor input and inspect expected outputs before touching hardware?
5. Which parts still require a real ERS-7 to prove?

## What We Cannot Honestly Claim Yet

We do not have:

- Sony Aperios kernel sources
- Sony device-driver implementations
- a CPU-accurate ERS-7 hardware model
- a legal redistributable system image that can simply be emulated like a normal VM

So this repo should not pretend to "run Aperios" on Debian in the literal sense.

The honest goal is:

- simulate the OPEN-R deployment graph
- simulate source-level object behavior
- replay event scenarios against host-side shims
- reserve final hardware proof for a real robot and real Memory Stick boot

## Simulation Layers

### 1. Boot Layout Simulator

This is the first practical layer and is implemented now.

Script:

```bash
scripts/simulate-openr-boot.sh
```

What it does:

- reads `OPEN-R/MW/CONF/OBJECT.CFG`
- lists the object load plan in order
- checks whether each referenced `/MS/...` object exists in the local Memory Stick tree
- reports missing dependencies such as system objects expected from a base OPEN-R stick

Example:

```bash
scripts/simulate-openr-boot.sh samples/common/HelloWorld
```

Current observed result for `HelloWorld`:

- `HELLO.BIN` is present
- `POWERMON.BIN` is missing from the sample tree

That is exactly the kind of issue this layer is meant to catch: the sample payload is not a whole bootable stick by itself and must be merged with a base OPEN-R stick layout that already provides system and support objects.

Why it matters:

- it is the closest Debian-side proof of "what Aperios will try to load first"
- it catches incomplete sample payloads before copying to real media

### 2. Lifecycle Simulator

This layer now exists in a first minimal form.

Current implementation:

- `aperios/include/OPENR`
- `aperios/src/hello_world_lifecycle.cpp`
- `make -C aperios run-hello-world`

The current host shim can compile the original Sony `HelloWorld` sample source and drive:

- `DoInit()`
- `DoStart()`
- `DoStop()`
- `DoDestroy()`

For simple samples like `HelloWorld`, this is enough to prove control flow and logging behavior without executing `HELLO.BIN`.

### 3. Event Replay Simulator

After lifecycle support, we should add scenario replay:

- fake touch sensor events
- periodic timer ticks
- canned camera/audio buffers
- fake Memory Stick file reads and writes

This is where an ERS-7 "hello body" app becomes interesting, because we can drive:

- touch -> state transition
- state transition -> LED/head motion command
- command -> logged simulated actuator output

### 4. Device Capability Simulator

This layer would not attempt full physics.

Instead it would model:

- named joints and safe ranges
- LEDs and face panels as symbolic outputs
- sensor channels as scripted inputs
- camera/audio availability and timing

That is enough to catch:

- wrong object wiring
- invalid expectations about available channels
- bad state-machine assumptions

## Current Practical Value

Right now the most valuable simulation path is:

1. build a real OPEN-R payload
2. stage it into a local `MS/OPEN-R` tree
3. run `scripts/simulate-openr-boot.sh`
4. confirm which objects are present or missing
5. document what still depends on the base Sony stick image

This gives us a Debian-side proof of deployment correctness before we move to live robot testing.

## Immediate Next Build-Out

The next useful simulator work in this repo should be:

1. add a tiny host-side OPEN-R shim for lifecycle methods
2. make `HelloWorld` runnable in host-sim mode
3. add one scripted sensor-driven ERS-7 demo
4. emit a trace showing:
   - init
   - start
   - fake touch event
   - resulting behavior command
   - stop

## Relationship To Real Aperios Proof

The real robot is still required to prove:

- Sony scheduler timing behavior
- real actuator response
- camera/audio throughput
- exact wireless stack behavior
- integration with genuine system objects on the Memory Stick

So the simulator is a confidence-building layer, not a replacement for live ERS-7 validation.
