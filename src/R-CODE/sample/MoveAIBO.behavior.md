# R-Code Behavior Extract: `MoveAIBO.R`

## Summary

- source: `src/R-CODE/sample/MoveAIBO.R`
- states: `4`
- transitions: `5`
- commands: `PLAY=6, MOVE=6, WAIT=6, SET=2, GO=2, POSE=1, AND=1, IF=1`
- sensed variables: `Gsensor_status`

## State Blocks

- `INIT`: Boot, Assume Safe Pose
  lines 5: `SET:Power:1`
  lines 6: `POSE:AIBO:slp_slp`
- `State 100`: Act, Synchronize
  lines 9: `PLAY:SOUND:trk4_xxx:50`
  lines 10: `MOVE:LEGS:WALK:SLOW:FORWARD:10`
  lines 11: `WAIT`
  lines 13: `PLAY:SOUND:trk4_xxx:50`
  lines 14: `MOVE:LEGS:WALK:SLOW:FORWARD:10`
  ... `11` more instructions
- `State 200`: Initialize State, Sense/Decide, Loop/Transition
  lines 32: `SET:stat:Gsensor_status`
  lines 33: `AND:stat:1`
  lines 34: `IF:=:stat:1:9000`
  lines 35: `GO:200`
- `State 9000`: Act, Synchronize, Recover, Loop/Transition
  lines 38: `MOVE:AIBO:ReactiveGU`
  lines 39: `WAIT`
  lines 40: `GO:200`

## Transitions

- `INIT` -> `100`: fallthrough
- `100` -> `200`: fallthrough
- `200` -> `9000`: if stat = 1
- `200` -> `200`: go
- `9000` -> `200`: go

## Mermaid

```mermaid
flowchart TD
    S_INIT["INIT"]
    S_100["State 100"]
    S_200["State 200"]
    S_9000["State 9000"]
    S_INIT -->|fallthrough| S_100
    S_100 -->|fallthrough| S_200
    S_200 -->|if stat = 1| S_9000
    S_200 -->|go| S_200
    S_9000 -->|go| S_200
```
