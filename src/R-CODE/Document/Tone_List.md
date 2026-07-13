# Musical Scale Recognition List

This is a cleaned-up Markdown edition of `Tone_List.txt`.

## Purpose

R-Code can recognize predefined musical note sequences. These are stored
in:

```text
\OPEN-R\SONY\CONF\TD.CFG
```

## Note Symbols

- `C` = do
- `D` = re
- `E` = mi
- `F` = fa
- `G` = so
- `A` = la
- `B` = ti
- `#` = sharp
- `~` = flat

## Example

If AIBO recognizes `do re mi`, represented as `CDE`, the corresponding
line number in `TD.CFG` is assigned to `Melody_id`.

## Example Melody IDs

- `1` = `CDE`
- `2` = `C#D#F`
- `3` = `DEF#`
- `4` = `D#FG`
- `5` = `EF#G#`
- `6` = `FGA`
- `7` = `F#G#A#`
- `8` = `GAB`
- `9` = `CED`
- `10` = `C#FD#`

Use the preserved `Tone_List.txt` for the full mapping.

