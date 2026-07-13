# Everyday Embodied Behaviors

This note proposes a single composite diagram for the subset of `R-CODE`
behaviors that read as everyday embodied activity rather than games,
stunts, or explicit demonstrations.

The point is not to claim that Sony shipped one literal script with this
exact topology. The point is to create a higher-level research
representation that gathers the ordinary body-level concerns already
visible across the preserved `ERS-111` samples.

This composite should now also be read as an umbrella over multiple
generated behavior families and their linked variants, not only as a
summary of one exemplar script per topic.

Included behavior motifs:

- locomotion and roaming from `Move`, `Walk`, and `WalkAround`
- obstacle checking and reorientation from `Maze`
- touch or nearby-person response from `Ote` and `HyperBaby`
- fall or turnover recovery from `Move` and `Recover`

Main contributing families:

- `Move`, `Walk`, `Walk-v2`, `Walk-v3`, and `WalkAround`
- `Maze`, `Maze-v2`, `Maze-v3`, `Maze-v4`, and `Maze-v5`
- `Ote` and `HyperBaby`
- `Recover`

Excluded from this composite:

- game-specific pursuit and kick logic such as `Football` and `HyperFootball`
- explicit show or playback routines such as `Play*`, `Pose*`, and `MotionRec`
- one-off expressive demos whose main purpose is display rather than ordinary autonomy

Files:

- composite diagram: [../generated/EmbodiedBehaviors.mmd](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/EmbodiedBehaviors.mmd:1)
- viewer: [../generated/EmbodiedBehaviors.html](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/EmbodiedBehaviors.html:1)
- svg export: [../generated/EmbodiedBehaviors.svg](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/generated/EmbodiedBehaviors.svg:1)

Generated-viewer convention:

- the HTML viewers retain a compact structural summary above each diagram
- a fuller analytical paragraph now appears below each diagram discussing
  the workflow exhibited by that specific behavior
- related scripts beyond the first exemplar are named as linked family
  variants such as `Maze-v2` or `Football-v2`

Interpretation:

`ordinary embodied loop = maintain posture -> watch surroundings -> move if clear -> reorient if blocked -> respond if contacted -> recover if fallen`

```mermaid
flowchart TD
    A[Boot And Safe Pose] --> B[Embodied Monitoring Loop]
    B --> C{Body Or World Event?}
    C -->|fallen or overturned| D[Recover Posture]
    C -->|obstacle ahead| E[Pause Forward Motion]
    C -->|touch or nearby person| F[Social Contact Response]
    C -->|no immediate issue| G[Roam Or Advance]
    D --> H[Resume Ordinary Monitoring]
    E --> I[Scan Left And Right]
    I --> J{Open Direction?}
    J -->|left| K[Turn Left]
    J -->|right| L[Turn Right]
    J -->|still blocked| M[Retry Or Back Away]
    K --> H
    L --> H
    M --> H
    F --> H
    G --> N[Check Posture And Space]
    N --> C
    H --> C
```

Why this is useful:

- it gives us one analyzable umbrella representation for non-game bodily behavior
- it separates everyday autonomy from specialized samples like ball play or playback
- it creates a shared reference frame for comparing future diagrams
- it gives us a stable umbrella above individual family variants, so new
  related scripts can be added without losing the conceptual grouping
