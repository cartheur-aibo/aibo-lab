# R-CODE 110/111 Reference Manual

This is a cleaned-up Markdown reading edition of `Reference.txt`.

## Scope

The reference manual is the command and data reference for
`R-CODE 110/111`.

## Main Sections

- list of R-CODE commands
- command reference
- list of R-CODE system variables
- quick reference of operators
- list of semantics of `AIBO Performer Kit`
- `ESC` file notes
- motion definition file notes
- musical scale recognition file notes

## Command Groups

### Controls

- `HALT`
- `DEF`
- `SET`
- `SET_MOVE`
- `SET_POSE`
- `REC`
- `LOAD_POSE`
- `SAVE_POSE`
- `LIMITTER`

### Motions

- `POSE`
- `PLAY`
- `MOVE`
- `STOP`
- `QUIT`
- `WAIT`
- `PLAY:APK`
- `POSE:APK`

### Control Structures

- labels with `:`
- `GO`
- `IF`
- `SWITCH`
- `CSET`
- `CASE`
- `FOR` / `NEXT`
- `WHILE` / `WEND`
- `REPEAT` / `UNTIL`
- `DO` / `LOOP`
- `BREAK`
- `CALL`
- `ARG`
- `RETURN`
- `RET`
- `ONCALL`
- `RESUME`

### Variables And Operations

- `GLOBAL`
- `LOCAL`
- `LET`
- `SET`
- arithmetic and logic operators such as `ADD`, `SUB`, `MUL`, `DIV`, `AND`, `IOR`, `XOR`
- comparison operators such as `EQ`, `NE`, `LT`, `LE`, `GT`, `GE`
- stack operations such as `PUSH`, `POP`, `DUP`, `JT`, `JF`

## Practical Highlights

The preserved reference includes:

- command syntax and argument descriptions
- region-specific motion naming for `AIBO`, `HEAD`, `LEGS`, and `TAIL`
- references to `.cfg` files under `\OPEN-R\SONY\CONF\`
- semantics names used by `AIBO Performer Kit`
- notes on `Melody_id` and other system variables
- file locations for pose, motion, and tone data on the `Memory Stick`

## Use In This Repo

Use `Reference.txt` as the preserved source and this Markdown version as
the fast-reading index into the command set.

