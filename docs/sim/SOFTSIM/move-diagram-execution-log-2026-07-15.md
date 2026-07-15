# Move Diagram Execution Log 2026-07-15

Status: `Live execution log`

Purpose:

- record the first actual live attempt to move from the `1737` baseline toward
  the `Move`-aligned mutation
- distinguish confirmed execution facts from planning notes
- preserve the exact prompt-level failures encountered

## Session context

This session was run against the local installed `arrayForth 3` /
`SOFTSIM` environment using the same underlying `sF6a2-af3.elf` path
validated earlier.

Important environment note:

- the binary could not be driven successfully inside the ordinary sandbox
- an unsandboxed terminal session was required
- the helper script `afk` successfully launched the interactive environment in
  that unsandboxed session

## Confirmed startup path

The session reached:

- the normal `sF386/UX.6a2` startup screen
- the `arrayForth-3 Help for sF/Win32 system`
- an `ok` prompt in the interactive environment

That matters because it confirms the shell-driven route can reproduce the same
 operator environment we previously accessed through the desktop launcher.

## Baseline commands entered

The following baseline commands were entered successfully:

```text
SOFTSIM LOAD
1737 LOAD
10 SUPER
```

## What these baseline steps confirmed

### `SOFTSIM LOAD`

This again exposed the documented simulator vocabulary, including:

- `OVIEW`
- `SEE`
- `OTHER`
- `MEM`
- `BREAK`
- `SS`
- `SUPER`

### `1737 LOAD`

This again loaded the known small example into the simulator display and
focused node `000`.

The focused display again showed the compact known-good shape, including:

- `p=200`
- `loc=00`
- slot `4fetch`
- entry through `go`

### `10 SUPER`

This again advanced the simulator successfully.

The display updated to:

- `10 cyc`
- approximately `0.0140 ~us`

The focus region moved into the known code area around:

- `loc=2D3`
- `p=2D4`

This reconfirms that the baseline execution path is live and stable in the
unsandboxed shell session.

## First `Move`-aligned mutation attempt

### Intent

The first mutation attempted to stay close to the draft by entering a one-node
behavior body with:

- repeated region,
- alternate region,
- preserved `1737` assembly wrapper.

### Commands entered

```text
ASM[ # 0 NODE ERS # 0 org +cy
: go clc begin begin dup + unext begin clc unext unext ;
: last >BIN ]ASM
```

### Result

The prompt reported:

```text
clc ?
```

### Interpretation

This reproduces the same failure pattern previously seen when hand-entering
`1737`-style content directly at the prompt.

The important conclusion is:

- the `Move` mutation did not compile in prompt-entered form,
- and the failure again centered on `clc`.

This strengthens the case that block-context matters, not just the visible text
of the attempted code.

## Second reduced mutation attempt

### Intent

A second attempt removed `clc` from the body in order to test whether the
failure was caused specifically by `clc` or by the broader prompt-entered loop
shape.

### Commands entered

```text
ASM[ # 0 NODE ERS # 0 org +cy
: go begin begin dup + unext begin dup + unext unext ;
: last >BIN ]ASM
```

### Result

The prompt reported:

```text
unext Illegal
```

### Interpretation

This shows that removing `clc` is not enough to make the prompt-entered
mutation acceptable.

The failure boundary therefore appears broader:

- the prompt-level environment is not accepting this `1737`-style tiny loop in
  the form we attempted,
- even when the body is simplified.

## Strongest confirmed result from this session

The strongest confirmed result is:

- the baseline `1737` execution path works reliably in the unsandboxed shell
  session,
- but the first `Move`-aligned prompt-entered mutations fail before load,
- with failures at `clc ?` and `unext Illegal`.

This is negative evidence, but it is still valuable because it narrows the
next step sharply.

## What this does support

This session supports saying:

- the local `SOFTSIM` environment is accessible from a shell session when run
  unsandboxed
- the small documented `1737` baseline remains the trusted execution anchor
- prompt-entered mutation is currently not a reliable path for the `Move`
  experiment

## What this does not support

This session does not support saying:

- that the `Move` mutation has been executed successfully
- that the pseudo-source draft is accepted by the environment as-is
- that the first diagram-to-program mapping is complete

## Best next step

The best next step is now more specific than before:

1. stop trying to prove the `Move` mutation by prompt entry alone
2. prepare a true block-context mutation path for `1737`
3. attempt the mutation in the same stored-block style that the baseline uses
4. only after that retry the one-node `Move` test
