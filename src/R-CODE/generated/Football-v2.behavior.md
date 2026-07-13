# R-Code Behavior Extract: `SoccerDog2.R`

## Summary

- category: `Behavior`
- family: `Football`
- variant: `v2`
- source: `src/R-CODE/sample/SoccerDog2.R`
- states: `9`
- transitions: `15`
- commands: `CASE=17, MOVE=9, IF=7, CSET=7, SET=6, GO=5, WAIT=3, SWITCH=1, ADD=1, MOD=1`
- sensed variables: `Cdt_npixel, Gsensor_status, Head_pan, Head_tilt`

## State Blocks

- `Boot`: Boot
  lines 5: `SET:Power:1`
  lines 7: `SET:mode:0`
  lines 8: `SET:head:0`
  lines 9: `SET:lost:0`
- `Sense Fall State`: Sense/Decide, Loop/Transition
  lines 12: `IF:&:Gsensor_status:1:9000`
  lines 13: `IF:=:mode:0:1000`
  lines 14: `IF:=:mode:1:2000`
  lines 15: `GO:100`
- `Action Loop`: Initialize State, Sense/Decide, Act, Synchronize
  lines 18: `SET:mode:0`
  lines 19: `MOVE:LEGS:STEP:RIGHT_TURN:0:4`
  lines 20: `SWITCH:head`
  lines 21: `CASE:0:MOVE:HEAD:ABS:-15:0:0:500`
  lines 22: `CASE:1:MOVE:HEAD:ABS:-15:-40:0:500`
  ... `11` more instructions
- `Mode`: Initialize State, Sense/Decide, Act
  lines 36: `SET:mode:1`
  lines 37: `IF:<:Cdt_npixel:32:1000`
  lines 39: `MOVE:HEAD:C-TRACKING:100`
  lines 40: `IF:>:Head_tilt:-58:2300`
  lines 42: `IF:>:Head_pan:0:2210:2220`
- `Left Kick`: Act, Loop/Transition
  lines 44: `MOVE:HEAD:HOME`
  lines 45: `MOVE:LEGS:KICK:LEFT_KICK:0`
  lines 46: `MOVE:LEGS:STEP:SLOW:FORWARD:1`
  lines 47: `GO:2900`
- `Right Kick`: Act, Loop/Transition
  lines 49: `MOVE:HEAD:HOME`
  lines 50: `MOVE:LEGS:KICK:RIGHT_KICK:0`
  lines 51: `MOVE:LEGS:STEP:SLOW:FORWARD:1`
  lines 52: `GO:2900`
- `It approaches a ball by the angle of the head`: Sense/Decide
  lines 55: `CSET:>:Head_pan:60:1`
  lines 56: `CSET:>:Head_pan:45:2`
  lines 57: `CSET:>:Head_pan:15:3`
  lines 58: `CSET:<:Head_pan:-60:4`
  lines 59: `CSET:<:Head_pan:-45:5`
  ... `10` more instructions
- `Synchronize`: Synchronize, Loop/Transition
  lines 72: `WAIT`
  lines 73: `GO:100`
- `Recover`: Act, Synchronize, Recover, Loop/Transition
  lines 76: `QUIT:AIBO`
  lines 77: `MOVE:AIBO:ReactiveGU`
  lines 78: `WAIT`
  lines 79: `GO:100`

## Transitions

- `INIT` -> `100`: entry
- `100` -> `9000`: if Gsensor_status & 1
- `100` -> `1000`: if mode = 0
- `100` -> `2000`: if mode = 1
- `100` -> `100`: upright
- `1000` -> `100`: if Cdt_npixel < 32
- `2000` -> `1000`: if Cdt_npixel < 32
- `2000` -> `2300`: if Head_tilt > -58
- `2000` -> `2210`: if Head_pan > 0
- `2000` -> `2220`: else Head_pan > 0
- `2210` -> `2900`: go
- `2220` -> `2900`: go
- `2300` -> `2900`: next labeled block
- `2900` -> `100`: go
- `9000` -> `100`: resume monitor

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot"]
    S_100["Sense Fall State"]
    S_1000["Action Loop"]
    S_2000["Mode"]
    S_2210["Left Kick"]
    S_2220["Right Kick"]
    S_2300["It approaches a ball by the angle of the head"]
    S_2900["Synchronize"]
    S_9000["Recover"]
    S_INIT -->|entry| S_100
    S_100 -->|if Gsensor_status & 1| S_9000
    S_100 -->|if mode = 0| S_1000
    S_100 -->|if mode = 1| S_2000
    S_100 -->|upright| S_100
    S_1000 -->|if Cdt_npixel < 32| S_100
    S_2000 -->|if Cdt_npixel < 32| S_1000
    S_2000 -->|if Head_tilt > -58| S_2300
    S_2000 -->|if Head_pan > 0| S_2210
    S_2000 -->|else Head_pan > 0| S_2220
    S_2210 -->|go| S_2900
    S_2220 -->|go| S_2900
    S_2300 -->|next labeled block| S_2900
    S_2900 -->|go| S_100
    S_9000 -->|resume monitor| S_100
```
