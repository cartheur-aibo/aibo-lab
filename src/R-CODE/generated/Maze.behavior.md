# R-Code Behavior Extract: `Maze1.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/Maze1.R`
- states: `12`
- transitions: `21`
- commands: `WAIT=12, MOVE=11, IF=9, SET=7, GO=6, PLAY=2, POSE=1, ADD=1`
- sensed variables: `Distance, Head_pan, Wait`

## State Blocks

- `Boot`: Boot
  lines 5: `SET:Power:1`
- `Repeat Forward Walk`: Initialize State, Assume Safe Pose, Act, Synchronize
  lines 8: `POSE:AIBO:oStanding`
  lines 9: `WAIT`
  lines 10: `MOVE:HEAD:ABS:0:0:0:1000`
  lines 11: `WAIT`
  lines 12: `MOVE:LEGS:WALK:0:FORWARD:0`
  ... `1` more instructions
- `Sense / Decide`: Sense/Decide, Synchronize, Loop/Transition
  lines 16: `IF:<:Distance:300:120`
  lines 17: `WAIT:1`
  lines 18: `ADD:t:1`
  lines 19: `IF:>:t:100:120`
  lines 20: `GO:110`
- `Synchronize`: Act, Synchronize
  lines 24: `PLAY:LEGS:WalkToWS`
  lines 26: `WAIT`
- `Head Scan Position`: Initialize State, Act
  lines 29: `MOVE:HEAD:ABS:0:0:0:1000`
  lines 30: `MOVE:HEAD:ABS:0:-90:0:1000`
  lines 31: `MOVE:HEAD:ABS:0:90:0:2000`
  lines 32: `MOVE:HEAD:ABS:0:0:0:1000`
  lines 33: `SET:dd:0`
- `Sense / Decide`: Initialize State, Sense/Decide, Loop/Transition
  lines 35: `IF:=:Wait:0:132`
  lines 36: `SET:d:Distance`
  lines 37: `SET:p:Head_pan`
  lines 38: `IF:<:d:dd:131`
  lines 39: `SET:dd:d`
  ... `2` more instructions
- `Sense / Decide`: Sense/Decide, Loop/Transition
  lines 43: `IF:<:pan:-15:200`
  lines 44: `IF:>:pan:15:300`
  lines 45: `IF:<:dd:300:133`
  lines 46: `GO:100`
- `Action Loop`: Act, Synchronize, Loop/Transition
  lines 49: `PLAY:SOUND:ang1_xxa:100`
  lines 50: `MOVE:LEGS:STEP:11:0:10`
  lines 51: `WAIT`
  lines 52: `WAIT:1000`
  lines 53: `GO:130`
- `Action Loop`: Act, Synchronize
  lines 56: `MOVE:HEAD:ABS:0:0:0:1000`
  lines 57: `WAIT`
- `Action Loop`: Sense/Decide, Act, Synchronize, Loop/Transition
  lines 59: `MOVE:LEGS:STEP:12:0:1`
  lines 60: `WAIT`
  lines 61: `WAIT:500`
  lines 62: `IF:>:Distance:500:100`
  lines 63: `GO:210`
- `Action Loop`: Act, Synchronize
  lines 66: `MOVE:HEAD:ABS:0:0:0:1000`
  lines 67: `WAIT`
- `Action Loop`: Sense/Decide, Act, Synchronize, Loop/Transition
  lines 69: `MOVE:LEGS:STEP:13:0:1`
  lines 70: `WAIT`
  lines 71: `WAIT:500`
  lines 72: `IF:>:Distance:500:100`
  lines 73: `GO:310`

## Transitions

- `INIT` -> `100`: entry
- `100` -> `110`: continue to monitor
- `110` -> `120`: if Distance < 300
- `110` -> `120`: if t > 100
- `110` -> `110`: go
- `120` -> `130`: next labeled block
- `130` -> `131`: next labeled block
- `131` -> `132`: if Wait = 0
- `131` -> `131`: if d < dd
- `131` -> `131`: go
- `132` -> `200`: if pan < -15
- `132` -> `300`: if pan > 15
- `132` -> `133`: if dd < 300
- `132` -> `100`: go
- `133` -> `130`: go
- `200` -> `210`: next labeled block
- `210` -> `100`: if Distance > 500
- `210` -> `210`: go
- `300` -> `310`: next labeled block
- `310` -> `100`: if Distance > 500
- `310` -> `310`: go

## Mermaid

```mermaid
flowchart TD
    A["Boot"]
    B["Stand And Start Advance"]
    C{"Forward Blocked?"}
    D["Stop Forward Motion"]
    E["Scan Left And Right"]
    F{"Best Direction?"}
    G["Turn Right"]
    H["Turn Left"]
    I["Signal And Retry"]
    A -->|entry| B
    B -->|enter sensing loop| C
    C -->|clear| C
    C -->|blocked| D
    D -->|begin scan| E
    E -->|compare openings| F
    F -->|right side clearer| G
    F -->|left side clearer| H
    F -->|still blocked nearby| I
    G -->|resume advance| B
    H -->|resume advance| B
    I -->|retry scan| E
```
