# Block-Context Resolution Write-Up

Status: `Action note`

Purpose:

- explain what you need to do next to address the current blocker
- connect the earlier note in `docs/sim/working-notes.txt` to the later live
  `Move` experiment results
- reduce the problem to a short sequence of practical investigations

## The actual problem

The current blocker is no longer whether `SOFTSIM` works.

That has already been confirmed repeatedly:

- `SOFTSIM LOAD` works
- `1737 LOAD` works
- `10 SUPER` works
- the `1737` example executes reliably in the live environment

The real problem is narrower:

- the stored `1737` block executes,
- but re-entering or mutating the same kind of code at the prompt does not yet
  work reliably.

The evidence now points to a block-context difference rather than a general
`SOFTSIM` failure.

## What the earlier working note already told us

The earlier note in `docs/sim/working-notes.txt` identified exactly the right
next investigation:

1. determine whether `1737 LOAD` succeeds because it is executed as a stored
   block
2. investigate what `clc` depends on in that context
3. clone `1737` into a fresh working block and make only one tiny edit
4. identify the safest editor/block commands for copying and editing a block in
   place

That advice remains correct after the later `Move` trial.

## What the later live `Move` trial added

The later live trial sharpened the diagnosis:

- a prompt-entered `Move`-aligned mutation failed at `clc ?`
- a reduced prompt-entered mutation without `clc` failed at `unext Illegal`

That means the issue is not just one bad word.

It more likely means one of these:

- the prompt is in the wrong compile state for this kind of tiny assembly entry
- block execution performs setup that prompt entry does not
- the editor/block loader preserves metadata or context that freehand entry
  loses
- the accepted tiny loop form depends on being compiled through stored-block
  mechanics

## What you need to do next

### 1. Stop using prompt entry as the main proof path

Do not spend the next round trying to prove the custom experiment by typing
more variants at the prompt.

Why:

- we already have enough negative evidence from prompt entry
- more freehand attempts will mostly blur the diagnosis
- the stored-block path is the trusted baseline

### 2. Treat `1737` as the authoritative execution unit

The next useful unit of work is not “write a new tiny program.”

It is:

- duplicate `1737`
- preserve its block form
- change one tiny thing only

Why:

- that tests mutation while holding the execution context steady
- it isolates the real question: can the environment accept a changed block
  when everything except the smallest behavior detail is preserved?

### 3. Find the safest block-copy workflow in the live environment

This is now one of the most important tasks.

You need to identify:

- how to duplicate a stored block cleanly
- how to edit that duplicate in place
- how to reload it without leaving the original block execution path

What to determine in the live session:

- which editor/view command is safest for block inspection
- which copy command or workflow duplicates block `1737`
- how to confirm that the duplicate really contains the copied text
- how to run the duplicate without altering the original

This is the most likely route to the first successful custom experiment.

### 4. Make only one tiny mutation in `: go`

Once a safe duplicate exists, change only one very small region.

Do not:

- add a second node yet
- add new loader structure
- add board-facing behavior
- add a complex branch tree

Do:

- keep `SOFTSIM LOAD`
- keep `ASM[ ... >BIN ]ASM`
- keep `!ND`
- keep `/STACK`
- keep `/RINF`
- keep `ITS`
- keep `go /P`
- keep `SEE`
- mutate only the body of `go`

The best first test is still the smallest visible structural change from the
stored baseline.

### 5. Answer the exact key question explicitly

The question from the earlier note remains the right one:

> What does block execution provide that prompt re-entry does not?

You should try to answer this with direct observations, not guesses.

Possible answers to test:

- a different compile mode is active
- a different vocabulary is active
- the block loader wraps the source in setup not visible in the listed text
- inline assembly is accepted only in a stored-block path

### 6. Record the result as a yes/no boundary

When you try the duplicated-block mutation, record the result in a crisp way:

- `yes`: duplicated block with one tiny edit loads and runs
- `no`: duplicated block still fails, and where it fails

That result matters more than trying many variants at once.

## Practical live-session checklist

In the next live session, the practical order should be:

1. launch the known-good `arrayForth` / `SOFTSIM` environment
2. confirm baseline with `SOFTSIM LOAD`, `1737 LOAD`, and `10 SUPER`
3. inspect block `1737`
4. duplicate `1737` into a fresh working block
5. verify the duplicate contents
6. change only one tiny part of `: go`
7. load the duplicated block through the same block-context route
8. step with `SUPER`
9. record exactly what changed or failed

## What not to do yet

Do not move on yet to:

- the two-node cadence experiment
- Volatco board deployment
- real sensor or actuator integration
- stronger paper claims about executable mapping success

Those all depend on first resolving the stored-block mutation path.

## Best-case outcome

The best-case next result is simple:

- a duplicated `1737` block with one tiny mutation loads successfully
- steps successfully
- and differs visibly from baseline

If that happens, you have cleared the main blocker.

At that point:

- the one-node `Move` experiment becomes credible
- the first diagram-to-executable claim becomes much stronger
- and the path to node `001` and later Volatco work becomes much clearer

## Bottom line

What you need to do is not “debug SOFTSIM” in the broad sense.

What you need to do is:

- preserve the `1737` block execution context,
- duplicate it safely,
- make one tiny behavior mutation inside that preserved context,
- and observe whether the mutated block still loads and steps.

That is the shortest path to solving the current blocker.
