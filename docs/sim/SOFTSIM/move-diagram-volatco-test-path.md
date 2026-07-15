# Move Diagram to SOFTSIM and Volatco Test Path

Status: `Planning`

Question:

- Which two simple behavior-diagram targets should anchor the `v.2` paper, and
  how should the first one (`Move`) be carried through `SOFTSIM` and onto a
  `GA144`-based Volatco board?

Sources:

- `arxiv/v.1/main.tex`
- `docs/sim/SOFTSIM/softsim-ga144-experiment-matrix.md`
- `docs/sim/SOFTSIM/softsim-1737-recovered-template.md`
- `docs/sim/SOFTSIM/softsim-cadence-demo-v4.md`
- Volatco official site: `https://volatco.tech/`
- Volatco technical docs: `https://volatco.github.io/`

## Bottom line

The best first paper-facing executable target is the existing `Move` diagram.

The best second simple target is not `Football`, which is already more
specialized and branch-heavy. A better second target is a reduced
`Boot / Safe Pose -> Sense Fall State -> Recover -> Action Loop` control slice
extracted from the same embodied vocabulary.

That gives the `v.2` paper a clean two-step progression:

1. `Move` as the first direct diagram-to-execution mapping.
2. `Recovery` as the second small control slice that stresses initialization,
   fault detection, and return-to-loop behavior.

## Why `Move` is the right first diagram

`Move` is already the most paper-ready candidate because it is:

- present as an existing figure,
- behaviorally compact,
- reactive rather than semantically specialized,
- close to the confirmed `1737` single-node execution path,
- and easy to explain as a loop of motion, fall sensing, and recovery.

In the `ERS-111` paper it was already described as a compact reactive loop
organized around repeated movement, fall sensing, and recovery. That maps well
onto the current `SOFTSIM` strategy because our live-confirmed baseline is also
small, loop-oriented, and traceable through one node before expanding outward.

## Why `Football` should not be the second simple test

`Football` is useful as a comparison figure, but it is not the best second
execution target if the goal is to stay simple.

It adds:

- ball tracking states,
- pursuit branching,
- kick-specific actions,
- and more family-specific specialization.

That makes it better as a later extension than as the second minimal test.

## Better second target

The better second target is a reduced recovery-oriented diagram assembled from
the same corpus backbone:

- `Boot / Safe Pose`
- `Sense Fall State`
- `Recover`
- `Action Loop`

This is not yet a standalone published figure, but it is a stronger second
experiment because:

- it stays within the common corpus vocabulary,
- it exercises transition handling more than `Move` alone,
- it remains small enough for `SOFTSIM` inspection,
- and it is easier to map onto physical safety or watchdog-oriented board
  behavior later than `Football`.

## How `Move` should progress through the toolchain

The safest path is:

1. preserve `1737` as the known-good minimal executable baseline
2. create a one-node mutation whose loop stands in for `Move`
3. make the loop observable in `SOFTSIM`
4. then decide which parts of `Move` should remain local and which deserve a
   second node
5. finally carry the smallest stable version onto Volatco-hosted `GA144`

This keeps the work aligned with the strongest lesson from the execution notes:
the first valuable custom experiment should be a structural mutation of `1737`,
not a new loader design.

## Proposed interpretation of `Move`

The diagram should be interpreted at first in the smallest functional way:

- `Action Loop`: repeated forward or bounded locomotor step behavior
- `Sense Fall State`: local health or posture check
- `Recover`: branch taken when the loop is no longer viable
- return to `Action Loop`: resumption after stabilization

At first, this does not need real locomotion hardware semantics. In
`SOFTSIM`, it only needs:

- a visible loop,
- a visible branch condition,
- a visible alternate path,
- and a visible return to the main path.

That is enough to support the claim that the behavior diagram is acting as an
executable organizational model.

## Best initial node mapping for `Move`

### Phase 1: one-node `Move`

Use one node only.

- one visible loop stands in for repeated movement
- one counter or marker stands in for progression through that loop
- one branch condition stands in for a synthetic fall state
- one alternate code path stands in for recovery

Paper value:

- smallest direct diagram-to-code mapping
- safest use of the `1737` template
- easiest to inspect in `SOFTSIM`

### Phase 2: two-node `Move`

Only after the one-node version works, split the behavior minimally:

- node `000`: local reactive action loop
- node `001`: supervisory or recovery-trigger node

That turns `Move` into the smallest paper-facing fast/reactive versus
slow/supervisory cadence example.

## What Volatco adds

Volatco's current official materials make it a practical hardware target for
the later stage of this experiment rather than for the first stage.

From the official site and documentation:

- Volatco is presented as a Forth-native, massively parallel platform for
  real-time control, robotics, edge AI, and long-lived autonomous systems.
- The platform documentation says it includes two `GA144A12` chips, `2 MB`
  SRAM, `32 MB` SPI flash, and exposed I/O headers.
- The docs explicitly call out:
  - serial development and deployment through `J8`,
  - manual reset through `J4`,
  - development/production selection through `J5`,
  - optional no-boot through `J6`,
  - power measurement experiments through `J2` and `J3`,
  - signal access across `J9` through `J13`.

This matters because the `Move` path can be staged cleanly:

- `SOFTSIM` first for functional logic and observability,
- Volatco second for real board loading, reset control, signal exposure,
  watchdog behavior, and power-oriented follow-up.

## Minimal board-facing plan for `Move`

When the `SOFTSIM` version is stable, the first Volatco-facing steps should be:

1. keep the program in development mode using `J5`
2. disable boot with `J6` if direct development loading is needed
3. use `J8` for IDE/serial access and reset-assisted loading
4. use `J4` for manual reset during rapid iteration
5. keep the first hardware run internal, without external sensors
6. only after stable loading, expose one or two observable signals on the
   accessible headers

This is safer than trying to attach external robotics peripherals immediately.

## Paper-safe claim this supports

If the above sequence succeeds, the paper can claim:

> The `Move` behavior diagram provides a compact paper-facing example of how an
> embodied reactive loop can be carried from diagrammatic organization to a
> minimal distributed executable program, first in `SOFTSIM` and then toward
> `GA144` realization on Volatco hardware.

It should not yet claim:

- real locomotion performance,
- timing validation from `SOFTSIM`,
- or full robotic behavior equivalence on hardware.

## Recommended pair for `v.2`

The recommended two diagram targets are:

1. `Move`
   - first direct diagram-to-executable mapping
   - first `SOFTSIM` experiment
   - first Volatco candidate

2. `Recovery Slice`
   - `Boot / Safe Pose -> Sense Fall State -> Recover -> Action Loop`
   - second small executable mapping
   - stronger stress on transition handling and safety logic

## Next concrete actions

1. write a paper-safe `Move` experiment paragraph for `arxiv/v.2/paper/main.tex`
2. create a `Move`-specific `1737`-style pseudo-source draft
3. extract or sketch the second `Recovery Slice` figure for the paper
4. add a short board-deployment note that maps `SOFTSIM` success criteria to
   Volatco `J4` / `J5` / `J6` / `J8`
