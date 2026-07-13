# R-CODE Behavior Ladder

This note turns four sample scripts into a simple-to-complex research
ladder for studying how behavior is represented in preserved Sony
`R-CODE`.

The point is not just to rank scripts by size. The point is to create a
stable analytical vocabulary we can carry from simple cases into more
complex ones without losing clarity.

## Ladder

Use the samples in this order:

1. `C-Tracking.R`
2. `Move`
3. `Maze`
4. `Football`

## Why This Order

Each step adds one meaningful behavior feature:

1. `C-Tracking`
   Adds a minimal `Boot -> Pose -> Act` pattern.
2. `Move`
   Adds explicit monitoring and recovery.
3. `Maze`
   Adds environment-dependent branching and repeated local decision loops.
4. `Football`
   Adds explicit behavior modes and a richer finite-state structure.

That means each sample can be read as an incremental extension of the
previous one, instead of as a totally new complexity jump.

The preserved source scripts stay under `src/R-CODE/sample/`.
Generated analysis outputs for this ladder now live under
`src/R-CODE/generated/`.

## Core Features

For this repo, these five features are a good starting comparison frame.

| Feature | Meaning | Why it matters |
|---|---|---|
| `Initialization Pattern` | how the script powers on and enters a usable pose or mode | shows whether behavior assumes a stable robot baseline |
| `Action Granularity` | whether the script issues one action, a sequence, or repeated motor primitives | shows the level at which behavior is composed |
| `Sensing Presence` | whether sensor state is read and used | separates demos from reactive behaviors |
| `Recovery Behavior` | whether the script has a protective or restorative path | shows robustness and bodily awareness |
| `Loop Complexity` | how much control structure the script uses | gives a rough proxy for behavioral sophistication |

## Comparison Matrix

| Sample | Initialization Pattern | Action Granularity | Sensing Presence | Recovery Behavior | Loop Complexity |
|---|---|---|---|---|---|
| `C-Tracking` | `SET:Power:1` then `POSE` | one tracking action | none visible in control flow | none | none |
| `Move` | power on and sitting pose | repeated walk + sound chunk | `Gsensor_status` | explicit `ReactiveGU` | one monitoring loop |
| `Maze` | power on then standing locomotion start | locomotion plus scan-turn blocks | `Distance`, `Head_pan`, `Wait` | indirect retry/escape, no fall recovery | nested local loops |
| `Football` | power on and explicit mode variables | search, track, approach, kick blocks | vision, head pose, obstacle/fall data | explicit fall recovery | multi-mode state machine |

## Sample 1: `C-Tracking`

Files:

- preserved source: [../sample/C-Tracking1.R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/sample/C-Tracking1.R:1)
- generated sidecar: [../generated/C-Tracking.behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/C-Tracking.behavior.md:1)
- viewer: [../generated/C-Tracking.html](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/C-Tracking.html:1)

Minimal reading:

```text
Boot -> Safe Pose -> Set Target Color -> Start Tracking
```

Research use:

- best first example of command-level intent
- establishes that not every script is a loop
- useful baseline for distinguishing setup from behavior

Interpretation:

This script is better understood as `activation of a capability` than as
a complete behavioral program.

## Sample 2: `Move`

Files:

- preserved source: [../sample/MoveAIBO.R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/sample/MoveAIBO.R:1)
- generated sidecar: [../generated/Move.behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Move.behavior.md:1)
- viewer: [../generated/Move.html](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Move.html:1)

Minimal reading:

```text
Boot -> Safe Pose -> Repeat Forward Motion -> Monitor Fall -> Recover
```

Research use:

- first good example of `body-level safety`
- first clear split between `action block` and `monitoring block`
- small enough to annotate by hand without overload

Interpretation:

This is a compact `action + monitor + recover` architecture.

## Sample 3: `Maze`

Files:

- preserved source: [../sample/Maze1.R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/sample/Maze1.R:1)
- generated sidecar: [../generated/Maze.behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Maze.behavior.md:1)
- viewer: [../generated/Maze.html](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Maze.html:1)

Minimal reading:

```text
Advance -> Detect Obstacle -> Scan -> Compare -> Turn -> Resume Advance
```

Research use:

- first clean example of environment-coupled branching
- useful for studying how the script builds a temporary local world model
- introduces repeated decision loops without yet becoming multi-mode

Interpretation:

This is a compact `perceive -> compare -> reorient` behavior.

## Sample 4: `Football`

Files:

- preserved source: [../sample/SoccerDog1.R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/sample/SoccerDog1.R:1)
- working analysis: [../generated/Football-Behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Football-Behavior.md:1)
- generated sidecar: [../generated/Football.behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Football.behavior.md:1)
- viewer: [../generated/Football.html](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/Football.html:1)

Minimal reading:

```text
Search -> See Ball -> Pursue -> Kick -> Repeat
```

Research use:

- first example where mode variables are central to interpretation
- best current example of a finite-state behavior in the sample set
- good destination case once the earlier feature vocabulary is stable

Interpretation:

This is a `mode-based embodied state machine`, not just a motion script.

## Working Vocabulary

When discussing these scripts in notes, prefer these compact labels:

- `Capability Activation`
- `Action Chunk`
- `Monitoring Loop`
- `Recovery Path`
- `Decision Loop`
- `Mode Switch`
- `Embodied State Machine`

Applied to the ladder:

- `C-Tracking` -> `Capability Activation`
- `Move` -> `Action Chunk + Monitoring Loop + Recovery Path`
- `Maze` -> `Decision Loop`
- `Football` -> `Mode Switch + Embodied State Machine`

## Suggested Research Sequence

1. Start with `C-Tracking` and mark only setup versus act.
2. Move to `Move` and mark act versus monitor versus recover.
3. Move to `Maze` and mark perception, comparison, and reorientation.
4. Move to `Football` only after the earlier labels feel stable.

This keeps the analytical frame coherent as complexity grows.

## Next Analytical Questions

- where does a script stop being a demonstration and become a behavior?
- which samples maintain an internal mode versus reacting directly to sensors?
- what kinds of bodily failure does Sony treat as worthy of explicit recovery?
- when does the code begin to represent the environment rather than just the robot?
