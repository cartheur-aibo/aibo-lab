# R-CODE 110/111 Programmer's Manual

This is a cleaned-up Markdown edition of
`r-code/Document/Programmers.txt` for easier reading inside the repo.
The original text file remains the authoritative preserved source.

## Scope

R-CODE 110/111 is a simplified programming language for `ERS-110/111`.
The manual describes it as a language similar to `BASIC`.

Within this repo, we should keep that boundary explicit: this is
`ERS-111` material first, with only a possible conceptual lead-in toward
later `ERS-210` comparison work.

## Table Of Contents

- Features
- System Environment Required To Use R-CODE
- Hello AIBO
- Coding Of R-CODE
- Motion Commands
- Pose Of AIBO
- System Variables
- R-CODE Error Information Output
- How To Create Motions
- Tutorial

## Features

1. R-CODE is a programming language similar to `BASIC`.
2. You can make AIBO walk, turn, kick, touch, change head directions, and track a pink ball.
3. You can make AIBO recognize certain types of sound.
4. You can make AIBO detect obstacles and fetch touch sensor information.
5. You can use control statements such as `IF` and `FOR`.
6. You can use subroutines.
7. You can use variables with `16-bit` integer values.
8. You can use addition and subtraction arithmetic.
9. You can use `PUSH` and `POP` stack operations.
10. It is not compatible with `R-CODE 1.2` used for `AIBO Master Studio Version 1.1`.

## System Environment Required To Use R-CODE

1. A computer that can edit text files.
2. `AIBO ERS-110/111`.
3. An `AIBO Programming Memory Stick` such as `ERA-MS008` or `ERA-MS016`.
4. A system that can read from and write to `Memory Stick`.

Notes from the manual:

- the blue `Memory Stick` is also usable
- use an `8MB` or `16MB` stick
- `16MB or greater` is not usable due to hardware limits

## Hello AIBO

The manual starts with a minimal example:

```text
SET:Power:1
POSE:AIBO:slp_slp
WAIT
PLAY:ESC:4BANZ2_SIT3
WAIT
```

This program:

- turns on the motors
- moves AIBO into a known sitting pose
- waits for the pose transition to finish
- plays the `4BANZ2_SIT3` semantics
- waits for the motion to finish

To run it:

1. Copy the package `OPEN-R` folders to a programming `Memory Stick`.
2. Save the script as `R-CODE.R`.
3. Copy `R-CODE.R` into `\OPEN-R\` on the stick.
4. Insert the stick into AIBO.
5. Press the pause button to boot and execute the program.

## Coding Of R-CODE

The manual describes these basic syntax rules:

- leading spaces and tabs are ignored
- lines not beginning with an alphanumeric character or `:` are treated as comments
- text after `//` is ignored as a comment
- `:`, spaces, tabs, and newlines act as separators
- commands are colon-separated
- lines beginning with `:` are labels
- values may be decimal, hexadecimal, octal, or binary
- all-uppercase words are reserved for the system

## Motion Commands

The manual groups motion control into these command families:

- `POSE` moves AIBO into the starting pose of a motion
- `MOVE` performs built-in motions with parameters
- `PLAY` performs predefined motions, including ones built with `AIBO Performer Kit`
- `STOP` stops a motion after the current one completes
- `QUIT` stops a motion immediately

General forms:

```text
POSE:<region>:<name>[:<opt1>:<opt2>:<opt3>:<opt4>]
PLAY:<region>:<name>[:<opt1>:<opt2>:<opt3>:<opt4>]
MOVE:<region>:<name>[:<opt1>:<opt2>:<opt3>:<opt4>]
STOP:<region>
QUIT:<region>
```

Supported regions:

- `AIBO`
- `HEAD`
- `LEGS`
- `TAIL`

The manual highlights the difference between `MOVE` and `PLAY`:

- `MOVE` uses built-in interpreter motions and accepts detailed parameters
- `PLAY` uses motions defined in external data and can be customized with `AIBO Performer Kit`

## Pose Of AIBO

The manual treats these as the three basic stable poses:

- Sitting
- Standing
- Sleeping

When a motion starts from a different pose than AIBO's current pose,
R-Code first transitions AIBO into the required starting pose and then
plays the motion.

## System Variables

System variables expose joint angles and sensor state. The manual uses
`Melody_id` as an example of reacting to recognized sounds.

Example flow:

```text
:100
IF:=:Melody_id:1:200
GO:100

:200
PLAY:AIBO:Akubi_sit
SET:Melody_id:0
GO:100
```

This loops until the melody ID is `1`, performs `Akubi_sit`, resets the
variable, and resumes waiting.

## Error Information Output

The manual says that if you try to perform a motion that does not exist,
R-Code logs the error to:

```text
\OPEN-R\Error.log
```

## How To Create Motions

The manual explains that `AIBO Performer Kit` can edit motions used by
semantics. It gives this example:

```text
PLAY:ESC:P1P1
```

This calls the motion assigned to semantics `P1P1` and plays the
associated sound at the same time.

The manual also outlines a workflow for creating motions:

1. Create a motion in `AIBO Performer Kit`.
2. Write the motion to a `Memory Stick`.
3. Use the R-Code image for that stick.
4. Reference the resulting semantics from R-Code.

## Tutorial

The later part of the text manual walks through programming tasks such
as:

- operating AIBO with `Sound Commander`
- reacting to sensor input
- using loops and labels
- writing scripts to `R-CODE.R`
- copying the script into `\OPEN-R\`
- booting AIBO from the programming stick

## Practical Use In This Repo

For this project, the Markdown version is mainly here to make the
original Sony text easier to browse while we:

- compare the preserved R-Code image to the real ERS-111 media
- stage a minimal `R-CODE.R` test
- keep the lower-risk R-Code path separate from deeper `OPEN-R` work
- use any `ERS-210` crossover only as a later comparison topic, not as a compatibility claim
