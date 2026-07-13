# R-Code Behavior Extract: `StepDog3.R`

## Summary

- category: `Behavior`
- family: `Step`
- variant: `v3`
- source: `src/R-CODE/sample/StepDog3.R`
- states: `3`
- transitions: `4`
- commands: `MOVE=7, SET=2, GO=2, AND=1, IF=1, QUIT=1, WAIT=1`
- sensed variables: `Gsensor_status`

## State Blocks

- `Boot`: Boot
  lines 5: `SET:Power:1`
- `Sense Fall State`: Initialize State, Sense/Decide, Act, Loop/Transition
  lines 8: `SET:stat:Gsensor_status`
  lines 9: `AND:stat:1`
  lines 11: `MOVE:LEGS:STEP:ADAPTIVE:FORWARD:10`
  lines 13: `MOVE:LEGS:STEP:CUSTOM:FORWARD:10`
  lines 15: `MOVE:LEGS:STEP:SKIP_BACK:FORWARD:10`
  ... `5` more instructions
- `Recover`: Act, Synchronize, Recover, Loop/Transition
  lines 28: `QUIT:AIBO`
  lines 29: `MOVE:AIBO:ReactiveGU`
  lines 30: `WAIT`
  lines 31: `GO:100`

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
