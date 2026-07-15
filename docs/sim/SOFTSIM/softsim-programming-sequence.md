# SOFTSIM Programming Sequence

Status: `Operator draft`

This note records the exact working sequence we want to follow when moving from
the desktop environment into a first `SOFTSIM` cadence experiment.

It begins with the user-visible action and then separates:

- the concrete steps we intend to perform,
- the code or descriptors we expect to prepare,
- the points that still need live confirmation in the actual environment.

## Startup sequence

1. click `af-3 DEFAULT SOFTSIM` icon in Plasma
2. wait for the `arrayForth 3` environment to open
3. confirm that the default workspace is the `SOFTSIM`-oriented environment
4. locate the command area where `LOAD` blocks and words can be entered
5. prepare to load the documented simulator workflow from `DB013 Chapter 7`

## First environment actions

The documented path from the notes is:

1. enter `SOFTSIM LOAD`
2. load a Boot Descriptor Language description
3. let `SOFTSIM` initialize the simulated chip state from that descriptor

At this stage, the most important thing is to stay inside the documented
simulator path rather than invent a hardware-specific boot sequence.

## Minimal cadence example preparation

Before running the example, prepare two things:

1. a node image for reactive node `000`
2. a node image for supervisory node `001`

The current draft assumes both are assembled in the standard per-node style
described in `DB013 Chapter 4`.

## Draft node-source sequence

The current intended programming order is:

1. assemble node `000`
2. write its image to the working bin
3. assemble node `001`
4. write its image to the working bin
5. enter or load a tiny BDL frame that initializes both nodes
6. inspect the result in `SOFTSIM`

## Draft assembly shape

The exact source is still under refinement, but the expected sequence is:

1. enter `ASM[` for node `000`
2. select node `000`
3. set origin to `0`
4. define the reactive wait-and-loop behavior
5. close with `>BIN ]ASM`
6. enter `ASM[` for node `001`
7. select node `001`
8. set origin to `0`
9. define the supervisory send-delay-send loop
10. close with `>BIN ]ASM`

This is aligned with the documented `arrayForth` assembly model, even though
the exact final word sequence still needs live verification.

## Draft BDL loading shape

After the two node images exist, the intended loading sequence is:

1. enter `SOFTSIM LOAD`
2. enter a `FRAME[` block
3. select node `000` with `+NODE`
4. initialize node `000` RAM with `/RAM`
5. initialize node `000` program counter with `/P 0.`
6. select node `001` with `+NODE`
7. initialize node `001` RAM with `/RAM`
8. initialize node `001` program counter with `/P 0.`
9. close the frame
10. allow `SOFTSIM` to enter the initialized state

The exact closing word for the frame should be confirmed live against the real
environment and any available DB block.

## First inspection sequence

Once loaded, the first observation sequence should be:

1. use `SEE` to focus node `000`
2. inspect the RAM area around `x20` and `x21`
3. confirm that `MODE-MARK` changes when the supervisory token arrives
4. confirm that `COUNT-MARK` changes differently in the two local modes
5. switch focus to node `001`
6. confirm that `001` spends most of its time in delay behavior with occasional
   token output

## Useful operator commands

From the local `DB013` notes, the first commands most likely to help are:

- `SEE`
- `OTHER`
- `MEM`
- `-MEM`
- `Z`
- `FAST`
- `SUPER`
- `SS`
- `BREAK`
- `-BREAK`

These should be enough for a first operator pass without depending on internal,
undocumented simulator vocabulary.

## Exact working script we are aiming toward

This is the current target sequence as a compact operator script:

1. click `af-3 DEFAULT SOFTSIM` icon in Plasma
2. enter the node `000` assembly block
3. compile node `000` to its bin image
4. enter the node `001` assembly block
5. compile node `001` to its bin image
6. enter `SOFTSIM LOAD`
7. enter the tiny BDL frame for nodes `000` and `001`
8. run or step the simulator
9. inspect node `000` state
10. inspect node `001` state
11. verify that the supervisory node changes the reactive node's visible mode

## What still needs live confirmation

The following pieces still need to be checked in the actual environment:

- the exact desktop label and launch behavior of `af-3 DEFAULT SOFTSIM`
- the exact accepted syntax for the smallest `FRAME[` block
- the exact load order expected by the local `arrayForth 3` workspace
- the exact visual path for inspecting node RAM markers
- whether the current pseudo-source needs small syntax adjustments to assemble

## Why this note matters

This file is intentionally procedural. It gives us a bridge from:

- document interpretation,
- to operator sequence,
- to the first reproducible `SOFTSIM` experiment.

That helps keep the implementation work concrete while preserving traceability.
