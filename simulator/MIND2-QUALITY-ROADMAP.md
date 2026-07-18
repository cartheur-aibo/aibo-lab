# Mind2 Simulator Quality Roadmap

This roadmap turns the current MIND 2 simulator from a useful hypothesis
engine into a better validated and easier-to-criticize research tool.

It does **not** assume we are trying to emulate Sony retail binaries. The goal
is higher-quality evidence, clearer model structure, and tighter coupling to
real ERS-7 bench work.

## Current State

The current baseline simulator is already useful for:

- loading preserved MIND 2 persistent-state files
- deriving symbolic behavior signals from those files
- replaying scenario events and comparing specimen versus reference trees
- ranking which state differences deserve real-hardware follow-up

The current quality boundary is:

- strong as a research scaffold
- promising as a comparative behavior model
- not yet validated enough to stand in for hardware

That makes the next question practical:

- how do we improve simulator quality without overclaiming what it is?

## Improvement Priorities

The priorities should be:

1. validation against live hardware
2. internal model clarity
3. regression protection
4. wider negative controls
5. richer but still disciplined scenario coverage

## Phase 1: Validation First

The first quality upgrade is not a more elaborate heuristic. It is a tighter
feedback loop between simulator predictions and live trials.

Concrete work:

- treat
  [features/mind2-behavior-research/LIVE-HARDWARE-MATRIX-2026-07-01.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/features/mind2-behavior-research/LIVE-HARDWARE-MATRIX-2026-07-01.md)
  as the primary acceptance boundary for simulator changes
- record, for each simulator distinction, which exact live observation would:
  - strengthen it
  - weaken it
  - falsify it
- stop adding new semantic rules unless they improve discrimination that the
  bench can actually test

Success criteria:

- each major simulator claim has a linked bench discriminator
- simulator changes can be explained in terms of observed or planned live
  evidence
- the model does not drift into untestable storytelling

## Phase 2: Make The Model Legible

Right now much of the MIND 2 logic is understandable, but it still lives
mostly as hand-authored semantics inside
[aperios/src/mind2_behavior_sim.cpp](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/aperios/src/mind2_behavior_sim.cpp).

The next quality step is to separate the simulator into a clearer pipeline:

1. persistent-file parsing
2. feature extraction
3. derived latent signals
4. scenario event updates
5. verdict generation

Concrete work:

- factor file-derived summaries into smaller named components
- distinguish raw observations from inferred signals such as:
  - `social_persistence_signal`
  - `routine_rigidity_signal`
  - `social_hold`
  - `routine_hold`
- add short comments or adjacent docs for thresholds that currently act as
  semantic pivots
- emit more structured trace output so a result can be read as:
  - input evidence
  - derived signals
  - update step
  - resulting verdict

Success criteria:

- a future reader can tell why a verdict happened
- file evidence and model inference are not visually collapsed together
- parameter changes become easier to review critically

## Phase 3: Add Regression Protection

The simulator now has enough meaningful named trees and named scenarios to
deserve a fixed regression matrix.

Concrete work:

- create a small automated comparison harness for the current core trees:
  - `baseline`
  - `0400`
  - `0400 + 1200`
  - `0400 + IEG`
  - `IEG transitional 61`
- pin expected verdict summaries for the current high-value scenarios:
  - `mind2-routine-endurance-probe.scn`
  - `mind2-social-endurance-probe.scn`
  - `mind2-low-salience-endurance-probe.scn`
- fail fast when a code change accidentally erases an already-established
  distinction

Success criteria:

- the simulator can be changed without losing current result boundaries
- intentional behavior changes become visible immediately
- future calibration work has a stable baseline

## Phase 4: Expand Negative Controls

A good research simulator must be able to say "this does not matter much" as
well as "this looks behaviorally important."

The repo already has a strong start here with `CONNECT.CFG` and paired
`CONNECT.CFG + DESIGNDB.CFG` swaps that did not dominate the current verdict
boundary.

Concrete work:

- preserve and extend the current control families
- continue testing files that differ structurally but may have weak behavioral
  weight
- document which changes are:
  - dominant levers
  - supporting context
  - presently inert under the current model

Success criteria:

- the simulator has a clearer sense of causal priority
- the current strong candidates look stronger because weak candidates were
  tested too
- the model becomes harder to fool with incidental file differences

## Phase 5: Broaden Scenario Discipline

More scenarios are useful only if they sharpen real distinctions.

Concrete work:

- keep scenario additions tied to an explicit research question
- prefer scenario families that discriminate between competing interpretations
- keep naming aligned with the behavior-research notes and verdict tables
- add scenarios only when they reveal:
  - a new falsifier
  - a new durability boundary
  - a new salience boundary

Success criteria:

- scenario growth remains evidence-led
- the scenario set gets more discriminating rather than merely larger

## Concrete Near-Term Engineering Plan

If we want a practical next implementation sequence inside this repo, the best
order is:

1. define the current expected verdict matrix in machine-checkable form
2. add a small regression runner around the MIND 2 simulator
3. refactor trace output so feature extraction and verdict logic are easier to
   inspect
4. document threshold justifications next to the most important rule boundaries
5. update the live-hardware notes whenever simulator changes alter predicted
   outcomes

## How This Intersects With The Simulator README

The top-level simulator plan in
[simulator/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/simulator/README.md)
describes the layered simulator strategy for the repo as a whole:

- boot-layout simulation
- lifecycle simulation
- event replay
- device-capability simulation

This roadmap fits inside that broader structure in a specific place:

- it is the quality plan for the current MIND 2 behavior-level event-replay
  simulator
- it does not replace the broader simulator plan
- it sharpens the standards for the simulator layer that is currently moving
  fastest

## Rule Of Thumb

If a simulator change makes the traces more interesting but the bench cannot
test the distinction, it is probably not yet a quality improvement.

If a simulator change makes a current distinction more explicit, more
reproducible, more testable, or easier to falsify, it probably is.
