# R-Code Behavior Extract: `MoveHead.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/MoveHead.R`
- states: `2`
- transitions: `1`
- commands: `MOVE=14, WAIT=14, SET=1`

## State Blocks

- `Boot`: Boot
  lines 5: `SET:Power:1`
- `Action Loop`: Act, Synchronize
  lines 9: `MOVE:HEAD:HOME`
  lines 10: `WAIT`
  lines 12: `MOVE:HEAD:ABS:30:0:0:1000`
  lines 13: `WAIT`
  lines 15: `MOVE:HEAD:ABS:-30:0:0:1000`
  ... `23` more instructions

## Transitions

- `INIT` -> `100`: entry

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot"]
    S_100["Action Loop"]
    S_INIT -->|entry| S_100
```
