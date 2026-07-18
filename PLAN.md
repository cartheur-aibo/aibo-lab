# Programming the Sony AIBO ERS-111 and ERS-7

_A work plan_

## Summary

```
Two active programming tracks

ERS-111 `R-CODE` track

1. Boot from `R-CODE` programming stick.
2. Run a minimal `R-CODE.R` script.
3. Trigger behavior through touch, pose, or musical-tone conditions.
4. Observe motion, light, sound, and tone-response behavior.
5. Use musical tones as the practical communication channel.

ERS-7 `OPEN-R` / MIND 2 track

1. Boot from OPEN-R stick.
2. Set Illume-Face LEDs.
3. Read head/back touch sensor.
4. On touch: nod head once.
5. Log event over wireless console.
```

## Overview

This repo now spans two different AIBO programming lineages:

1. `ERS-110/111`

   * primary practical path in this repo: `R-CODE`
   * platform lineage: early `OPEN-R` generation, associated with `OPEN-R v1`
   * lower-risk scripting and behavior study
   * communication boundary: no Wi-Fi path; practical signaling can occur
     through motion, sound, light, and recognizable musical tones

2. `ERS-7`

   * primary practical paths in this repo: preserved MIND 2 study and
     `OPEN-R`
   * deeper native robotics access
   * communication boundary: can expose WLAN services and `WCONSOLE` on
     compatible sticks

`ERS-210` is currently a reference-only adjacent platform in this planning
context, not an active substantive work track yet.

The Sony AIBO ERS-7 supports several levels of programmability, ranging from simple behavior customization to complete low-level robot control.

For an ERS-7 running **AIBO MIND 2**, there are three primary development approaches:

1. **R-CODE**

   * Easiest entry point.
   * Create scripted behaviors while preserving the MIND personality system.
   * Ideal for custom tricks, interactions, and entertainment behaviors.

2. **AIBO Remote Framework / Entertainment Player**

   * Control the robot remotely from a PC over Wi-Fi.
   * Useful for experimentation and automation.
   * Requires less knowledge of the internal architecture.

3. **OPEN-R SDK**

   * Sony's native robotics development platform.
   * Provides full access to sensors, actuators, vision, audio, networking, and LEDs.
   * Intended for robotics research and advanced development.

OPEN-R is the most powerful option and the focus of this guide.

For `ERS-111`, the equivalent first-class path in this repo is the preserved
`R-CODE` environment under
[src/R-CODE/](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/src/R-CODE/README.md),
where behavior can be scripted and observed with the robot's built-in media and
tone-recognition facilities. It should also be understood as part of the
earlier `OPEN-R v1` lineage rather than as an `ERS-7`-style Wi-Fi-native
development target.

The dedicated `ERS-111` repo now lives at:

- <https://github.com/cartheur-aibo/ERS-111>

The dedicated `ERS-7` repo now lives at:

- <https://github.com/cartheur-aibo/ERS-7>

The dedicated `ERS-210` repo currently exists as a reference-only home at:

- <https://github.com/cartheur-aibo/ERS-210>

## Platform Communication Boundary

The communication story differs sharply across the two platforms:

| Platform | Main programmable path here | Practical communication channel in this repo |
| --- | --- | --- |
| `ERS-111` | `R-CODE` | motion, lights, sounds, and musical-tone recognition/output |
| `ERS-7` | MIND 2 research + `OPEN-R` | Wi-Fi, `WCONSOLE`, HTTP, and normal sensor/actuator paths |

This difference matters for planning:

- `ERS-7` work can treat wireless logging and interactive network observation as
  normal milestones
- `ERS-111` work should instead treat tone patterns, audible responses, and
  local scripted behavior as the practical interaction loop
- shared research questions may still cross both platforms, but the operational
  interfaces are different
- `ERS-210` should currently be treated as a documented comparison target and
  future expansion point, not as an active implementation line in this repo

## Current Repo Position

This plan describes the longer-range destination for the repo: deliberate ERS-7
programming through OPEN-R, beginning with simple controlled applications such
as the `hello body` program and eventually reaching richer robotics work.

The repo's current active center of gravity is slightly earlier in that path:

- preservation and analysis of the `ERS-110/111` `R-CODE` package and samples
- preservation and recovery of real ERS-7 MIND 2 media
- behavior forensics over preserved persistent state
- a host-side MIND 2 simulator used as a hypothesis engine for bench work
- Debian-side OPEN-R lifecycle and boot-layout simulation

That current work does not replace the OPEN-R goal. It de-risks it.

In practical terms, the MIND 2 behavior and simulator track helps this plan by:

- improving our understanding of how the retail system persists and expresses
  behavior
- sharpening which questions need real-hardware validation before deeper
  programming claims are trusted
- giving the repo a disciplined event-replay and scenario-comparison layer
  before full OPEN-R applications are mature
- building confidence with sticks, deployment paths, WLAN boundaries, and
  host-side analysis workflows

So the relationship should be read like this:

- `PLAN.md` is now the combined planning surface for `ERS-111` `R-CODE` work
  and `ERS-7` `OPEN-R` / MIND 2 work
- `ERS-111` gives the repo a lower-risk scripting and behavior path with
  tone-based interaction
- `ERS-7` remains the long-range native robotics destination
- `ERS-210` is present for reference and comparison only, pending later
  substantive work
- the Mind 2 research and simulator work is the current operational precursor
- both tracks belong in the same lab because they converge on controlled AIBO
  experimentation across different generations and interfaces

For the current quality plan of the MIND 2 simulator layer, see:

- [simulator/MIND2-QUALITY-ROADMAP.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/simulator/MIND2-QUALITY-ROADMAP.md)

## Status Snapshot

As of July 18, 2026, this plan is best read as a mix of completed groundwork,
partially completed platform work, and still-open application goals.

### Completed

- preserved `ERS-110/111` `R-CODE` package, sample set, and behavior-analysis
  notes under `src/R-CODE/`
- documented `ERS-210` as a reference-only adjacent platform with local Sony
  manuals and a dedicated external repo home
- OPEN-R SDK and preserved toolchain recovery sufficient for local build work
- sample build and staging workflows for ERS-7 OPEN-R payloads
- dedicated-stick discipline and preserved-baseline handling for experimental
  work
- Debian-side boot-layout validation with
  [scripts/simulate-openr-boot.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/scripts/simulate-openr-boot.sh)
- first host-side OPEN-R lifecycle proof with the Sony `HelloWorld` sample
- WLAN and `WCONSOLE` operational knowledge for ERS-7 staging and observation
- preserved MIND 2 and MIND 3 reference-stick baselines
- first host-side MIND 2 behavior simulator for higher-level research triage

### Partially Completed

- practical `ERS-111` bench workflow that closes the loop from preserved
  `R-CODE.R` script to repeatable real-robot tone/behavior trials
- real-hardware confirmation for the broader OPEN-R programming path
- standardization of a single canonical OPEN-R development-stick workflow
- practical mastery of the OPEN-R object model beyond first lifecycle proof
- integration of simulator findings with repeated live-hardware validation

### Not Yet Completed

- a crisp end-to-end `ERS-111` minimal communication demo using musical tones
  as the signaling channel
- the full end-to-end `hello body` application described in this plan
- verified live completion of the sequence:
  - boot from OPEN-R stick
  - set Illume-Face LEDs
  - read touch input
  - nod head
  - log over wireless console
- the later progression levels:
  - Level 1 LED/touch/sound applications
  - Level 2 head tracking and vision work
  - Level 3 wireless control and multi-state behaviors
  - Level 4 distributed and research-grade robotics behaviors

### Immediate Efficiency Rule

For current repo work, the most efficient interpretation is:

- treat `ERS-111` `R-CODE` preservation and `ERS-7` infrastructure, staging,
  preservation, and first simulation layers as already established
- treat a minimal `ERS-111` tone-communication loop as one of the next
  unfinished low-risk milestones
- treat the `hello body` application and repeated live validation as the next
  highest-value unfinished milestones
- use the MIND 2 simulator and behavior-forensics track to de-risk those later
  `ERS-7` `OPEN-R` milestones rather than treating them as separate projects

---

# What is OPEN-R?

OPEN-R is Sony's robotics development environment built on top of the **Aperios** operating system.

Applications are written in C++ and execute as independent objects that communicate through asynchronous message passing.

This architecture is notable because it encourages event-driven design rather than traditional polling loops.

An OPEN-R application can access:

* Joint servos
* Camera frames
* Audio input/output
* Touch sensors
* Accelerometers
* Distance sensors
* LEDs and Illume-Face
* Wireless networking
* Memory Stick storage

Unlike MIND 2, OPEN-R does not provide the companion-pet personality layer.

When booted into OPEN-R, the robot behaves as a programmable robotics platform rather than an autonomous pet.

---

# Recommended Learning Path

## Step 1: Obtain the OPEN-R SDK

Locate:

* OPEN-R SDK documentation
* Sample applications
* ERS-7 hardware references
* OPEN-R API manuals

The SDK includes examples demonstrating:

* Motion control
* Sensor reading
* LED control
* Camera access
* Audio playback

Study the samples before attempting custom development.

---

## Step 2: Build the Development Environment

Historically, OPEN-R development used:

* Linux
* Sony's MIPS cross-compiler
* OPEN-R SDK build tools

Modern hobbyist setups typically use:

```bash
git
gcc
make
flex
bison
```

along with preserved Sony toolchain archives.

The target processor is a MIPS-based embedded system.

---

## Step 3: Compile a Sample Program

Before writing new code:

1. Build a sample project.
2. Generate the OPEN-R binaries.
3. Copy them to a test Memory Stick.
4. Confirm successful booting on the robot.

Good first examples include:

* Head movement
* LED flashing
* Touch sensor reading
* Audio playback

---

## Step 4: Prepare a Dedicated Memory Stick

Do **not** modify your working MIND 2 Memory Stick.

Instead:

1. Obtain a second compatible Memory Stick.
2. Install OPEN-R software on it.
3. Configure networking separately.
4. Use it exclusively for development.

This allows easy switching between:

* MIND 2 mode
* OPEN-R development mode

by swapping Memory Sticks.

---

## Step 5: Learn the OPEN-R Object Model

OPEN-R applications are composed of independent objects.

Typical architecture:

```text
+-------------------+
| Perception Object |
+---------+---------+
          |
          v
+-------------------+
| State Machine     |
+---------+---------+
          |
          v
+-------------------+
| Motion Controller |
+---------+---------+
          |
          v
+-------------------+
| Joint Outputs     |
+-------------------+
```

Each object:

* Receives messages
* Processes events
* Sends messages to other objects
* Returns control quickly

Avoid long-running loops.

The system is designed around asynchronous event handling.

---

# First Project: "Hello Body"

A useful introductory project:

## Objectives

* Boot from an OPEN-R Memory Stick
* Control LEDs
* Read touch sensors
* Move the head
* Log events over Wi-Fi

## Behavior

```text
Robot boots

↓

Set Illume-Face expression

↓

Wait for head touch

↓

Touch detected

↓

Nod head once

↓

Send event log message

↓

Return to waiting state
```

This exercise teaches:

* Deployment
* Boot process
* Sensor input
* Servo control
* State machines
* Wireless communication

---

# Suggested Progression

After completing "Hello Body":

## Level 1

* LED animations
* Touch interactions
* Sound playback

## Level 2

* Head tracking
* Vision processing
* Object detection

## Level 3

* Wireless control
* Multi-state behaviors
* Autonomous navigation

## Level 4

* Distributed robot behaviors
* Vision-guided interaction
* Research-grade robotics experiments

---

# MIND 2 vs OPEN-R

| Feature              | MIND 2  | OPEN-R |
| -------------------- | ------- | ------ |
| Pet personality      | Yes     | No     |
| Autonomous companion | Yes     | No     |
| Behavior scripting   | Limited | Full   |
| Sensor access        | Limited | Full   |
| Servo control        | Limited | Full   |
| Camera access        | Limited | Full   |
| Research use         | No      | Yes    |
| Custom robotics      | No      | Yes    |

---

# Recommendation

For users interested primarily in custom tricks and pet behaviors:

* Start with **R-CODE**.

For users interested in robotics, embedded systems, computer vision, or autonomous behavior:

* Move directly to **OPEN-R**.

The ERS-7 remains one of the most sophisticated consumer robotics platforms of its era, and OPEN-R provides unusually deep access to the hardware compared to modern consumer robots.
