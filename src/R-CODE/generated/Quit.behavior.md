# R-Code Behavior Extract: `Quit1.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/Quit1.R`
- states: `1`
- transitions: `0`
- commands: `WAIT=3, MOVE=2, SET=1, POSE=1, QUIT=1`

## State Blocks

- `Boot / Safe Pose`: Boot, Assume Safe Pose, Act, Synchronize, Recover
  lines 5: `SET:Power:1`
  lines 6: `POSE:AIBO:slp_slp`
  lines 7: `WAIT`
  lines 8: `MOVE:HEAD:HOME`
  lines 9: `WAIT`
  ... `3` more instructions

## Transitions


## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot / Safe Pose"]
```
