# ERS-111 Minimal Tone Demo

This is the first recommended real-hardware milestone for the `ERS-111`
track in this repo.

It is intentionally small:

- one preserved `R-CODE.R` script
- one recognized melody
- one unmistakable body motion
- one repeatable bench checklist

## Goal

Prove the full local loop:

1. prepare a valid `ERS-111` `R-CODE` programming stick
2. copy a minimal `R-CODE.R` script into `\OPEN-R\`
3. boot the robot successfully
4. present the melody `CDE` (`do re mi`)
5. observe a repeatable motion response

That gives us the first practical communication milestone for `ERS-111`:

- no Wi-Fi
- no remote shell
- but a verified input/output loop through musical-tone recognition

## Script

Use:

- [sample/ToneHello.R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/src/R-CODE/sample/ToneHello.R)

This script is a near-direct repo-native version of the Sony manual's
`Melody_id` example:

- wait for `Melody_id = 1`
- `1` corresponds to `CDE`
- when detected, perform `PLAY:AIBO:Akubi_sit`
- reset `Melody_id` to `0`
- return to the waiting loop

## Why `CDE`

The preserved tone list and programmer manual both support this mapping:

- `CDE` -> `Melody_id = 1`

Relevant references:

- [Programmers.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/src/R-CODE/Document/Programmers.md)
- [Tone_List.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/src/R-CODE/Document/Tone_List.md)

## Why `Akubi_sit`

This motion is already used in the preserved manual example and is a safer
first response than trying to chain multiple sound or walking commands on the
first bench pass.

We want:

- one tone input
- one motion output
- minimal ambiguity

## Stick Preparation

1. Start from the preserved `R-CODE` programming image tree under:
   - [MSImage/OPEN-R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/src/R-CODE/MSImage/OPEN-R)
2. Copy the script to:
   - `\OPEN-R\R-CODE.R`
3. Keep the original preserved image unchanged; prepare a separate sacrificial
   test stick that matches the documented `8MB` or `16MB` expectations.

## Bench Procedure

1. Confirm the stick contains the preserved `OPEN-R` tree plus `R-CODE.R`.
2. Insert the stick into the `ERS-111`.
3. Boot the robot with the normal `R-CODE` procedure.
4. Wait for the robot to settle.
5. Present `CDE` clearly.
6. Watch for the `Akubi_sit` response.
7. Repeat at least once to confirm the loop resets correctly.

## Expected Result

- no tone: robot waits
- `CDE`: robot performs `Akubi_sit`
- after the motion, the script returns to waiting for the next recognized tone

## What To Record

For each run, note:

- stick identity
- robot identity
- exact tone source used
- whether the robot booted successfully
- whether `CDE` triggered the motion
- whether the response repeated on a second attempt
- whether any `Error.log` file appeared under `\OPEN-R\`

## Failure Triage

If it does not work, check in this order:

1. the script is named exactly `R-CODE.R`
2. the script is copied into `\OPEN-R\`
3. the stick matches the documented media expectations
4. the robot actually booted the `R-CODE` image
5. the tone source is producing the expected `CDE` sequence cleanly
6. `\OPEN-R\Error.log` exists and contains a useful clue

## Why This Is The Right First Milestone

This demo is the smallest credible proof that:

- the preserved `ERS-111` programming image is usable
- the repo can produce a real script for it
- the robot can receive information through musical tones
- the repo's `ERS-111` track has moved from preservation into controlled bench
  execution
