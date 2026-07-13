# R-Code Behavior Extract: `WalkDog2.R`

## Summary

- category: `Behavior`
- family: `Walk`
- variant: `v2`
- source: `src/R-CODE/sample/WalkDog2.R`
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
  lines 11: `IF:=:stat:1:200`
  lines 12: `MOVE:LEGS:WALK:STABLE:FORWARD:10`
  lines 14: `MOVE:LEGS:WALK:LIZARD:FORWARD:10`
  ... `4` more instructions
- `Recover`: Act, Synchronize, Recover, Loop/Transition
  lines 24: `QUIT:AIBO`
  lines 25: `MOVE:AIBO:ReactiveGU`
  lines 26: `WAIT`
  lines 27: `GO:100`

## Transitions

- `INIT` -> `100`: entry
- `100` -> `200`: fallen
- `100` -> `100`: upright
- `200` -> `100`: resume monitor

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot"]
    S_100["Sense Fall State"]
    S_200["Recover"]
    S_100_CHECK{"Fall?"}
    S_100 --> S_100_CHECK
    S_100_CHECK -->|fallen| S_200
    S_100_CHECK -->|upright| S_100
    S_INIT -->|entry| S_100
    S_200 -->|resume monitor| S_100
```
