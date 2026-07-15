# SOFTSIM 1737 Recovered Template

Status: `Live recovered block`

This note records the exact text recovered from live block `1737` in the local
installed `arrayForth 3` environment on `2026-07-15`.

Its purpose is to preserve the smallest confirmed SOFTSIM example as an
implementation template for future custom work.

## Exact recovered block text

Recovered via:

- `1737 LIST`

The visible block contents were:

```text
1737
0 ( Simple example)  SOFTSIM LOAD
1
2 ASM[ # 0 NODE ERS # 0 org  +cy
3 : go   clc  begin begin  +  unext unext ;
4 : last   >BIN ]ASM
5
6 ( Load)  0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
7    2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P
8
9 ( Display)  0 0 SV0 2!  0 SEE
10
11
12
13
14
15
```

## What this confirms

This recovered block confirms that the small SOFTSIM example is built from
three compact pieces:

1. `SOFTSIM LOAD`
2. one tiny per-node assembly block for node `0`
3. one compact load-and-display sequence

That is more precise than our earlier inference from the PDF notes alone.

## Structure of the block

### Assembly section

The assembly section is:

```text
ASM[ # 0 NODE ERS # 0 org  +cy
: go   clc  begin begin  +  unext unext ;
: last   >BIN ]ASM
```

This confirms several important details:

- the example uses `ASM[` directly in the block,
- it selects node `0`,
- it sets origin to `0`,
- it closes with `>BIN ]ASM`,
- the executable entry word is `go`.

## Load section

The load section is:

```text
0 !ND  x10000. 2DUP  2DUP 2DUP 2DUP 2DUP
   2DUP 2DUP 2DUP 2DUP  10 /STACK  /RINF  0 ITS go /P
```

This matters because it shows that the smallest runnable example in this
environment does not use the `FRAME[` shape we had been using as a placeholder
in the draft notes.

Instead, the recovered small example uses:

- `!ND`
- repeated `2DUP`
- `/STACK`
- `/RINF`
- `ITS`
- `go /P`

That is the most important practical correction produced by the live session.

## Display section

The display section is:

```text
0 0 SV0 2!  0 SEE
```

This confirms that the small example explicitly seeds the display/focus state
after load.

## Why this is important for the cadence experiment

This block gives us a real minimal template for custom work:

- one node can be assembled inline,
- one entry word can be named and loaded,
- stack and register setup can be expressed compactly,
- initial viewing can be scripted.

That means the first custom cadence experiment should probably be designed as a
minimal extension of `1737`, not as an entirely separate loader concept.

## Immediate implications for our draft notes

The live recovered block changes our confidence profile:

- our earlier assembly assumptions were broadly right,
- our placeholder custom BDL shape was too speculative for the smallest case,
- `1737` is now the most trustworthy template for custom startup structure.

It also exposes a new practical constraint:

- retyping the visible `1737` assembly text directly at the prompt does not
  behave the same as loading the stored block

In the live session, hand-entered re-entry of:

```text
: go   clc  begin begin  +  unext unext ;
```

stopped with:

```text
clc ?
```

That means the next custom experiment should assume that block-context loading
matters, even when the visible text matches exactly.

## Best next use of this template

The safest next move is:

1. preserve `1737` exactly as the baseline
2. create a `1737`-style custom variant for one-node experimentation first
3. only then expand toward a two-node cadence example

That progression reduces the chance that we confuse:

- syntax problems,
- loader problems,
- genuine behavior problems.
