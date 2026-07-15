# Move Diagram 1737-Style Draft

Status: `Pseudo-source draft`

Purpose:

- turn the paper-facing `Move` diagram into the smallest credible
  `1737`-style executable sketch
- preserve the live-confirmed `1737` loader mechanics
- change only the behavioral core needed to represent:
  - repeated movement,
  - fall sensing,
  - recovery,
  - and return to loop

## Design rule

Everything outside the behavior body should stay as close as possible to the
live-recovered `1737` structure.

That means:

- keep `SOFTSIM LOAD`
- keep `ASM[ ... >BIN ]ASM`
- keep `!ND`
- keep `/STACK`
- keep `/RINF`
- keep `ITS`
- keep `/P`
- keep `SEE`

The only major intentional change should be the internal organization of the
entry behavior.

## What this draft is trying to represent

The `Move` diagram in paper terms is interpreted here as:

- `Action Loop`: forward or repeated locomotor action
- `Sense Fall State`: check whether the motion loop is still viable
- `Recover`: alternate path taken if the loop is no longer viable
- return to `Action Loop`: continue after stabilization

In this first draft, these are represented symbolically, not physically.

## Mapping from diagram to pseudo-source roles

- `Action Loop`
  - a repeated arithmetic or token-writing loop
- `Sense Fall State`
  - a synthetic condition derived from a counter or local marker
- `Recover`
  - a visibly separate alternate code path
- return to main loop
  - branch back into the repeated action section

## `1737`-style skeleton with `Move` interpretation

This is intentionally still pseudo-source and should not yet be called known
good runnable code.

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy

: move-step
    dup +
;

: sense-fall
    dup
;

: recover-step
    clc
;

: go
    clc
    begin
        move-step
        sense-fall
        if
            recover-step
        then
    unext
    begin
        move-step
    unext
;

: last   >BIN ]ASM

0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

## Why this should be read cautiously

This draft is structurally useful even if some exact words are not yet the
final accepted ones for the live environment.

Its value right now is that it makes the organizational intention explicit:

- a named action fragment,
- a named sensing fragment,
- a named recovery fragment,
- and a controlling entry word that cycles among them.

That is closer to the paper claim than the earlier generic cadence notes,
because it directly mirrors one existing behavior diagram.

## Safer mutation strategy

The safest practical development path is probably not to enter the full draft
above first.

Instead:

1. keep the exact recovered `1737` loader and display shape
2. mutate the body of `go` only
3. prove one visible alternate path
4. only then split named helper words if the environment accepts them cleanly

## Minimal mutation candidate

A smaller first trial that stays closer to the recovered rhythm is:

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy
: go
  clc
  begin
    begin
      dup +
    unext
    begin
      clc
    unext
  unext ;
: last   >BIN ]ASM

0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

This candidate still does not encode a real conditional branch, but it does
separate a main repeated action region from a second distinct region that can
stand in for recovery while we are still learning the accepted tiny-program
syntax.

## Development phases for `Move`

### Phase 1

Goal:

- preserve exact `1737` mechanics
- make the loop visibly different from baseline

Evidence supported:

- custom executable behavior exists

### Phase 2

Goal:

- represent main-loop versus recovery-path distinction in one node

Evidence supported:

- a `Move`-like diagram can be approximated as an executable organization

### Phase 3

Goal:

- split local action loop from supervisory or recovery trigger across two nodes

Evidence supported:

- the `Move` diagram can support a minimal reactive/supervisory cadence

### Phase 4

Goal:

- carry the smallest stable organization onto Volatco-hosted `GA144`

Evidence supported:

- at least one paper-facing diagram survives translation from `SOFTSIM` to
  board-hosted execution

## What to watch in SOFTSIM

When this draft or its reduced variant is tried, watch for:

- successful assembly in block-context
- successful loading through the preserved `1737` path
- a visible difference from the baseline code path
- a stable focused node display
- observable distinction between the repeated region and the alternate region

## Best next action after this note

The best next step is to convert the `Minimal mutation candidate` into a
live-operator checklist, so the experiment can be attempted with less ambiguity
inside the running `arrayForth 3` environment.
