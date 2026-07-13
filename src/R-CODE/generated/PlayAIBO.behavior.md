# R-Code Behavior Extract: `PlayAIBO.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/PlayAIBO.R`
- states: `2`
- transitions: `1`
- commands: `PLAY=16, WAIT=16, SET=1, POSE=1`

## State Blocks

- `Boot / Safe Pose`: Boot, Assume Safe Pose
  lines 5: `SET:Power:1`
  lines 6: `POSE:AIBO:slp_slp`
- `Synchronize`: Act, Synchronize
  lines 10: `PLAY:AIBO:Akubi_sit`
  lines 11: `WAIT`
  lines 13: `PLAY:AIBO:BadB_sit`
  lines 14: `WAIT`
  lines 16: `PLAY:AIBO:Banzai_sit_C`
  ... `27` more instructions

## Transitions

- `INIT` -> `100`: entry

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot / Safe Pose"]
    S_100["Synchronize"]
    S_INIT -->|entry| S_100
```
