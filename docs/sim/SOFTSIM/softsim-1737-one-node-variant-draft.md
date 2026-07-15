# SOFTSIM 1737 One-Node Variant Draft

Status: `Mutation draft`

This note proposes the first custom experiment as a minimal mutation of the
live-recovered `1737` example.

The goal is not yet to create the two-node cadence example. The goal is to
change as little as possible while still proving that we can alter behavior in
a controlled way.

## Principle

Everything that can be copied from `1737` should stay copied.

Only one small behavior region should be deliberately changed.

That way, if the custom variant fails, the likely causes stay narrow:

- syntax in the changed loop,
- wrong assumptions about the changed entry body,
- not the entire loading structure.

## Copied directly from `1737`

These parts should stay structurally identical:

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy
...
: last   >BIN ]ASM

0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

These lines are copied because they are already live-validated.

## Intended mutation

The only intended mutation is the body of `go`.

Recovered `1737` uses:

```text
: go   clc  begin begin  +  unext unext ;
```

The first custom experiment should replace that with a visibly distinct but
similarly small body.

## Candidate variant A

The safest first candidate is:

```text
: go   clc  begin begin  dup +  unext unext ;
```

Why this candidate:

- it changes the behavior minimally,
- it preserves the same overall structural rhythm,
- it gives us a very small mutation from the known-good baseline.

## Candidate variant B

If variant A proves awkward, another small option is:

```text
: go   clc  begin begin  inv +  unext unext ;
```

This is also tiny, but it is slightly riskier because it changes the arithmetic
pattern more sharply.

## Preferred first trial

The recommended first trial is variant A:

```text
SOFTSIM LOAD

ASM[ # 0 NODE ERS # 0 org  +cy
: go   clc  begin begin  dup +  unext unext ;
: last   >BIN ]ASM

0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P

0 0 SV0 2!  0 SEE
```

## Why this is the right next experiment

This experiment tests an important boundary:

- can we mutate the tiny example's assembled behavior,
- while keeping the rest of the `1737` loading path intact?

If yes, then we have a much firmer basis for a later two-node cadence example.

## What to watch when run

When this variant is tried live, watch for:

- successful assembly without syntax complaints,
- successful load without breaking the setup sequence,
- stepping behavior that differs from baseline `1737`,
- focused execution still remaining in node `000`.

## Decision rule

If this one-node variant fails, do not jump to two-node work yet.

Instead:

1. reduce the mutation further,
2. confirm exact accepted syntax,
3. retry the one-node case.

If this one-node variant succeeds, then the next draft should add the smallest
possible second node.
