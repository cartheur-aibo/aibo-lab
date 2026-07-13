# R-Code Behavior Extract: `PlaySound.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/PlaySound.R`
- states: `2`
- transitions: `1`
- commands: `PLAY=14, WAIT=7, SET=1, POSE=1`

## State Blocks

- `Boot / Safe Pose`: Boot, Assume Safe Pose
  lines 5: `SET:Power:1`
  lines 6: `POSE:AIBO:slp_slp`
- `Synchronize`: Act, Synchronize
  lines 10: `PLAY:SOUND:hppy_sit:50`
  lines 11: `PLAY:AIBO:Happy_sit`
  lines 12: `WAIT`
  lines 14: `PLAY:SOUND:ang1_xxa:50`
  lines 15: `PLAY:AIBO:Angry_sitc`
  ... `16` more instructions

## Transitions

- `INIT` -> `100`: entry

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot / Safe Pose"]
    S_100["Synchronize"]
    S_INIT -->|entry| S_100
```
