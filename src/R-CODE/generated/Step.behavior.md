# R-Code Behavior Extract: `StepDog1.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/StepDog1.R`
- states: `3`
- transitions: `4`
- commands: `MOVE=6, SET=2, GO=2, AND=1, IF=1, QUIT=1, WAIT=1`
- sensed variables: `Gsensor_status`

## State Blocks

- `Boot`: Boot
  lines 5: `SET:Power:1`
- `Sense Fall State`: Initialize State, Sense/Decide, Act, Loop/Transition
  lines 8: `SET:stat:Gsensor_status`
  lines 9: `AND:stat:1`
  lines 11: `MOVE:LEGS:STEP:SLOW:FORWARD:10`
  lines 13: `MOVE:LEGS:STEP:FAST:FORWARD:10`
  lines 15: `MOVE:LEGS:STEP:OFFLOAD:FORWARD:10`
  ... `4` more instructions
- `Recover`: Act, Synchronize, Recover, Loop/Transition
  lines 27: `QUIT:AIBO`
  lines 28: `MOVE:AIBO:ReactiveGU`
  lines 29: `WAIT`
  lines 30: `GO:100`

## Transitions

- `INIT` -> `100`: entry
- `100` -> `9000`: fallen
- `100` -> `100`: upright
- `9000` -> `100`: resume monitor

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot"]
    S_100["Sense Fall State"]
    S_9000["Recover"]
    S_100_CHECK{"Fall?"}
    S_100 --> S_100_CHECK
    S_100_CHECK -->|fallen| S_9000
    S_100_CHECK -->|upright| S_100
    S_INIT -->|entry| S_100
    S_9000 -->|resume monitor| S_100
```
