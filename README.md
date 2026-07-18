# aibo-lab

Experimental laboratory for AIBO platform research across preserved hardware,
OPEN-R / Aperios runtime reconstruction, ERS-7 MIND 2 and MIND 3
experimentation, cross-repo ERS-111 R-CODE study, and host-side simulator
work.

The main effort in this repo is OPEN-R / Aperios runtime work under `aperios/`: documenting the original execution model from Sony's manuals, building a small host-side shim on Debian, and using original sample code to prove lifecycle behavior before attempting deeper simulation.

## Repo Charter

This repo is the experimenter's laboratory for AIBO:

- generate and study real ERS-7 physical experiments
- build host-side OPEN-R and MIND 2 simulation paths
- preserve sticks, stage test media, and compare observed outcomes
- keep the scripts, notes, artifacts, and workflows needed to reproduce bench work

The scope boundary is intentional:

- if something helps run, observe, analyze, or reproduce experiments, it belongs here
- if something is canonical sample code or SDK-packaged source material, it belongs in the dedicated component repos
- if something primarily exists as publication packaging, paper drafting, or behavior-diagram monograph work, it belongs in the sibling publication repos under `../monographs/`

The current canonical upstream repos are:

- `OPEN-R` samples and SDK-oriented source material:
  <https://github.com/cartheur-aibo/open-r-sdk>
- `ERS-110/111` `R-CODE` source material:
  <https://github.com/cartheur-aibo/r-code>

This repo may still produce a Mind 2-specific paper if the experiments uncover a strong result, but publication-first organization is no longer the main structure here.

## Current Focus

- documenting the OPEN-R / Aperios object model, lifecycle, and messaging behavior from the Sony manuals
- building a host-side shim under `aperios/` for original OPEN-R objects
- validating assumptions by compiling and running the Sony `HelloWorld` sample on Debian
- using that runtime understanding to guide later simulator and tooling work
- preserving, restoring, and behaviorally studying real ERS-7 MIND 2 stick state
- extending the lab to include ERS-111 tone-trigger bench work and
  cross-platform comparison through the dedicated `r-code` repo

## Start Here

1. Read the OPEN-R / Aperios notes and document map:

```bash
sed -n '1,220p' aperios/README.md
```

2. Run the host-side lifecycle proof:

```bash
make -C aperios run-hello-world
```

3. Check the adjacent `open-r-sdk` boot layout:

```bash
scripts/simulate-openr-boot.sh ../open-r-sdk/samples/common/HelloWorld
```

This reports the raw sample tree only. Before the sample is merged into a base
OPEN-R stick image, missing object binaries such as `HELLO.BIN` and
`POWERMON.BIN` are expected.

4. If you want a real Memory Stick payload, build and stage `HelloWorld` from
the adjacent `open-r-sdk` repo:

```bash
export OPENRSDK_ROOT="$PWD/local/OPEN_R_SDK"
export OPENRSDK_SAMPLES="$PWD/../open-r-sdk/samples/common/HelloWorld"
source scripts/env.sh
./scripts/check-openr.sh
make -C "$OPENRSDK_SAMPLES/HelloWorld" \
  OPENRSDK_ROOT="$PWD/local/OPEN_R_SDK"
mkdir -p "$OPENRSDK_SAMPLES/MS/OPEN-R/MW/OBJS"
make -C "$OPENRSDK_SAMPLES/HelloWorld" \
  OPENRSDK_ROOT="$PWD/local/OPEN_R_SDK" \
  install
```

## What This Repo Does

- studies and documents the OPEN-R / Aperios runtime model using Sony manuals and companion papers
- builds a first host-side OPEN-R lifecycle shim under `aperios/`
- builds legacy OPEN-R tools on Debian when a local SDK/toolchain install is available
- builds and stages real OPEN-R sample payloads for Memory Stick deployment
- simulates OPEN-R boot layout from `OBJECT.CFG`
- distinguishes plain-text sample `OBJECT.CFG` files from unsupported retail
  encoded configs so zero-object reports are not misread
- treats [`cartheur-aibo/open-r-sdk`](https://github.com/cartheur-aibo/open-r-sdk)
  as the canonical home for sample trees and SDK-oriented source material
- preserves real-stick images and recovery workflows for ERS-7 MIND 2 media
- pivots from stick-making into behavior forensics on persistent MIND 2 state
- treats [`cartheur-aibo/r-code`](https://github.com/cartheur-aibo/r-code)
  as the canonical home for `ERS-110/111` `R-CODE` source material
- keeps only the lab-side notes, generated findings, bench procedures, and
  cross-repo integration context needed for experiments
- treats publication repos in `../monographs/` as the home for paper-shaped outputs that grow out of this lab work

## Aperios Work

The center of gravity for the current project is [`aperios/README.md`](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/aperios/README.md). It maps the local Sony manuals and academic papers, summarizes the OPEN-R / Aperios runtime model, and documents the first Debian host shim for running original OPEN-R sample code.

Right now that work has established:

- a practical object model based on single-threaded message handling
- an explicit lifecycle model centered on `DoInit`, `DoStart`, `DoStop`, and `DoDestroy`
- a working Debian host proof for the original Sony `HelloWorld` sample
- clear next steps around subject/observer readiness, buffering, and mailbox scheduling

## Easy Commands

Read the current OPEN-R / Aperios notes:

```bash
sed -n '1,220p' aperios/README.md
```

Run the host-side lifecycle simulator for the original Sony `HelloWorld` sample:

```bash
make -C aperios run-hello-world
```

Check what the adjacent `open-r-sdk` stick tree would try to boot:

```bash
scripts/simulate-openr-boot.sh ../open-r-sdk/samples/common/HelloWorld
```

As above, the unmerged sample tree will report missing runtime object binaries
until it is staged onto a base OPEN-R stick layout.

## Build A Real Sample

```bash
export OPENRSDK_ROOT="$PWD/local/OPEN_R_SDK"
export OPENRSDK_SAMPLES="$PWD/../open-r-sdk/samples/common/HelloWorld"
source scripts/env.sh
./scripts/check-openr.sh
make -C "$OPENRSDK_SAMPLES/HelloWorld" \
  OPENRSDK_ROOT="$PWD/local/OPEN_R_SDK"
mkdir -p "$OPENRSDK_SAMPLES/MS/OPEN-R/MW/OBJS"
make -C "$OPENRSDK_SAMPLES/HelloWorld" \
  OPENRSDK_ROOT="$PWD/local/OPEN_R_SDK" \
  install
```

That produces:

- `../open-r-sdk/samples/common/HelloWorld/MS/OPEN-R/MW/OBJS/HELLO.BIN`

## MIND 2 vs MIND 3

In this repo, the preserved MIND 2 and MIND 3 stick trees share the same
top-level Sony layout:

- `MEMSTICK.IND`
- `OPEN-R/`
- `PALM/`

The practical differences are:

- MIND 2 is our smaller preserved baseline under
  [features/aibo-mind2/build/stick](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/aibo-mind2/build/stick),
  and it is the one we have already verified live on the real robot over Wi-Fi
  and HTTP.
- MIND 3 is preserved under
  [opt/AIBO7M3](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/opt/AIBO7M3)
  and is much larger in this repo, about `55M` versus about `20M` for the
  staged MIND 2 tree.
- The MIND 3 image carries extra stock content compared with the preserved
  MIND 2 tree, including more Palm-side files plus additional app-facing
  content such as `OPEN-R/APP/PC/DIARY`, `OPEN-R/APP/OBJS/SA.BIN`, and
  `OPEN-R/APP/OBJS/STM.BIN`.
- For repeatable testing, the repo now exposes both paths as separate staging
  workflows: [features/ers7m2-baseline/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m2-baseline/README.md)
  for preserved MIND 2 baseline work and [features/ers7m3-test-stick/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m3-test-stick/README.md)
  for MIND 3.

## Where To Look

- [aperios/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/aperios/README.md) for the main OPEN-R / Aperios runtime notes, document map, and host-shim status
- [simulator/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/simulator/README.md) for simulator scope and plan
- [features/ers7-wifi/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-wifi/README.md) for ERS-7 Wi-Fi setup from the bundled Sony docs
- [features/aibo-mind2/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/aibo-mind2/README.md) for staging the bundled AIBO MIND 2 ERS-7 stick layout
- [features/stick-feature-deployment/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/stick-feature-deployment/README.md) for the handoff workflow and proven conclusions for new 64 MB Sony sticks
- [features/stick-feature-deployment/TROUBLESHOOTING.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/stick-feature-deployment/TROUBLESHOOTING.md) for the operational checklist and failure-triage guide during delicate stick bring-up
- [features/ers7m2-baseline/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m2-baseline/README.md) for the preserved-only first-boot MIND 2 baseline on larger Sony sticks
- [features/ers7m2-wconsole/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m2-baseline/README.md) for the larger-stick full MIND 2 `WCONSOLE` app-testing workflow
- [features/ers7m3-test-stick/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7m3-test-stick/README.md) for the larger-stick full MIND 3 app-testing workflow
- [features/reference-sticks/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/reference-sticks/README.md) for the active baseline records of preserved genuine MIND 2 and MIND 3 sticks
- [features/mind2-behavior-research/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/mind2-behavior-research/README.md) for the new behavior-forensics research loop, candidate files, and research questions
- [features/ers7-camera-stream/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-camera-stream/README.md) for the ERS-7 `W3AIBO` camera-serving workflow on port `60080`
- [features/hello-world/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/hello-world/README.md) for a scratch-built ERS-7 Wi-Fi test stick
- [features/ers7-runtime-base/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-runtime-base/README.md) for a runtime-system hybrid ERS-7 test stick
- [features/ers7-stick-forensics/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/features/ers7-stick-forensics/README.md) for the older low-level forensics/background notes and raw-layout differences
- <https://github.com/cartheur-aibo/open-r-sdk> for the canonical OPEN-R
  sample and SDK-material repo
- <https://github.com/cartheur-aibo/r-code> for the canonical `ERS-110/111`
  `R-CODE` source repo

## Rule

Use a separate programmable Memory Stick, not your working AIBO MIND stick.
