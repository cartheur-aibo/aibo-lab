# R-Code Behavior Extract: `PlayHead.R`

## Summary

- category: `Behavior`
- source: `src/R-CODE/sample/PlayHead.R`
- states: `2`
- transitions: `1`
- commands: `PLAY=13, WAIT=13, SET=1`

## State Blocks

- `Boot`: Boot
  lines 5: `SET:Power:1`
- `Synchronize`: Act, Synchronize
  lines 9: `PLAY:HEAD:Akubi_sitb`
  lines 10: `WAIT`
  lines 12: `PLAY:HEAD:Dere_sitb`
  lines 13: `WAIT`
  lines 15: `PLAY:HEAD:Down_sit`
  ... `21` more instructions

## Transitions

- `INIT` -> `100`: entry

## Mermaid

```mermaid
flowchart TD
    S_INIT["Boot"]
    S_100["Synchronize"]
    S_INIT -->|entry| S_100
```
