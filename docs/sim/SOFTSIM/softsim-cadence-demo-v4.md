# SOFTSIM Cadence Demo v4

Status: `1737-aligned draft`

This note updates the cadence demo plan using the exact live-recovered
structure of block `1737`.

The main correction is simple but important:

- the smallest confirmed custom path should follow the `1737` load style,
- not the earlier placeholder `FRAME[` style.

## Why this version exists

Earlier drafts were directionally useful, but they still guessed at the
smallest loader shape.

Live recovery of `1737` now shows that the first custom cadence experiment
should be built from:

- inline `ASM[` assembly,
- `>BIN ]ASM`,
- `!ND`,
- stack and register setup words,
- `ITS`,
- named entry word `/P`,
- explicit display setup via `SEE`.

That is a stronger starting point than our earlier generalized BDL sketch.

## Exact minimal pattern we now trust

The recovered small-example pattern is:

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy
: go   ...
: last   >BIN ]ASM

0 !ND  ...  /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

This is now the best baseline for a custom experiment.

## Revised development sequence

The safest sequence is now:

1. preserve `1737` as the known-good minimal form
2. create a custom one-node variant in the same structural style
3. confirm that the custom one-node variant still loads and steps
4. only then expand to a two-node cadence example

This reduces ambiguity because it separates:

- syntax errors,
- load/setup errors,
- multi-node communication errors.

## Revised cadence strategy

Instead of jumping directly to nodes `000` and `001`, the research path should
use two phases.

### Phase 1

Make a one-node custom variant that is visibly different from `1737` while
keeping the same load mechanics.

For example:

- keep node `000`,
- keep a single named entry word,
- change the loop behavior so the visible code path differs from `1737`,
- keep `SEE` focused on node `000`.

### Phase 2

Only after Phase 1 works, extend the same pattern toward:

- reactive node `000`,
- supervisory node `001`,
- a minimal inter-node token handoff.

## Revised minimal one-node experiment

The smallest custom experiment should now look like this conceptually:

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy
: go
  begin
    dup
    +
  unext unext ;
: last   >BIN ]ASM

0 !ND  x10000. 2DUP 2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

This is still draft-level and should not yet be called final runnable source,
but it is now shaped like the actual small example.

## Revised two-node target

Once the one-node variant is working, the two-node cadence target becomes:

- node `000` assembled in the recovered small-example style,
- node `001` added with the smallest possible supervisory role,
- load/setup kept as close as possible to the `1737` mechanics,
- display focused first on node `000`.

## What changes relative to v2 and v3

Relative to the earlier drafts, this version changes one major assumption:

- we should no longer treat `FRAME[` as the default starting point for the
  smallest custom example.

That does not mean `FRAME[` is wrong in general. It means:

- for the first custom experiment,
- the live-confirmed `1737` pattern is a safer base.

## Strongest current implementation lesson

The strongest lesson from today's execution work is:

> The first valuable custom SOFTSIM experiment should be a minimal structural
> mutation of `1737`, not an entirely fresh loader design.

That keeps the experiment traceable and lowers the risk of mixing speculative
loader assumptions with the behavior we actually want to study.

## Best next step

The next concrete task should be:

1. write a `1737`-style one-node custom variant as text,
2. mark which words are copied directly from the recovered template,
3. mark which words are our first deliberate mutation,
4. then attempt that variant live before adding node `001`.
