# Move Diagram Live Operator Checklist

Status: `Operator draft`

Purpose:

- translate the `Move`-specific `1737`-style mutation into a live trial
  checklist
- reduce ambiguity during the next `arrayForth 3` / `SOFTSIM` session
- keep the experiment traceable as:
  - copied baseline mechanics,
  - deliberate mutation,
  - expected visible outcomes

## Scope of this checklist

This checklist is only for the first `Move`-aligned one-node trial.

It is not yet:

- the two-node cadence test,
- the recovery-slice test,
- or the Volatco board deployment sequence.

## Preconditions

Before attempting this checklist, assume the following are already true:

- the local `af-3 DEFAULT SOFTSIM` launcher works
- `SOFTSIM LOAD` works
- `1737 LOAD` works
- `10 SUPER` works after `1737 LOAD`
- the exact recovered `1737` block text is preserved in
  `softsim-1737-recovered-template.md`

If any of those fail in the live environment, stop and return to the validated
baseline before attempting the `Move` mutation.

## Trial objective

The objective of this trial is modest:

- preserve the exact `1737` load and display structure
- mutate only the body of `go`
- produce a visible code-path difference from baseline `1737`
- create a first symbolic stand-in for `Move`

This trial does not need to prove:

- real locomotion,
- real fall sensing,
- or final accepted branching syntax.

## Copied from baseline

These parts should remain structurally identical to the recovered `1737`
baseline:

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy
...
: last   >BIN ]ASM

0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

## Deliberate mutation

The first deliberate mutation should be only this:

```text
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
```

This is the smallest current candidate for separating:

- a repeated main region,
- and a second distinct region that can stand in for recovery.

## Recommended live sequence

1. launch `af-3 DEFAULT SOFTSIM`
2. confirm arrival at the normal `arrayForth` environment
3. enter `SOFTSIM LOAD`
4. confirm the simulator operator vocabulary appears
5. enter `1737 LOAD`
6. confirm the known-good small example loads normally
7. if possible, inspect or list the working block context before mutation
8. prepare a block-context copy of `1737`
9. replace only the body of `go` with the `Move` mutation candidate
10. keep `: last   >BIN ]ASM` unchanged
11. keep the `!ND`, `/STACK`, `/RINF`, `ITS`, `/P`, and `SEE` lines unchanged
12. load the mutated block in the same block-context style as `1737`
13. step with `10 SUPER`
14. repeat stepping as needed while observing the visible node state

## Exact mutation target

This is the full intended mutated block shape:

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

This should still be treated as a mutation target, not as already-validated
source.

## What to observe

During the live run, observe:

- whether the mutated block assembles without immediate syntax complaints
- whether loading still preserves a stable node `000` focus
- whether stepping advances into a visibly different code region than baseline
- whether the repeated region and alternate region appear distinct in the code
  view
- whether the simulator remains stable rather than collapsing back to prompt
  errors

## Success criteria

Count the trial as a success if all of the following hold:

- the mutated block loads in block-context
- stepping succeeds
- the visible execution path differs from baseline `1737`
- the difference is explainable as a `Move`-like structural mutation

## Failure handling

If the trial fails:

1. do not add node `001`
2. do not add Volatco hardware steps yet
3. reduce the mutation further
4. confirm exact accepted syntax around `begin`, `unext`, and `clc`
5. retry with the smallest possible change from recovered `1737`

## Evidence to record

After the live attempt, record:

- the exact block used
- whether it loaded successfully
- the first syntax or runtime error, if any
- whether `10 SUPER` advanced
- the focused node number
- any visible code location or `p=` changes
- whether the result is strong enough to justify a second-node trial

## Best follow-up after a successful run

If this trial succeeds, the next best step is:

1. preserve the exact working `Move` mutation text
2. document what visible behavior changed relative to `1737`
3. draft the smallest node `001` supervisory addition
4. only then move toward the two-node `Move` cadence test
