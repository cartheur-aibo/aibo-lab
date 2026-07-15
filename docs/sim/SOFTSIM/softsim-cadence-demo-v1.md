# SOFTSIM Cadence Demo v1

Status: `Interpretive`

Purpose:

- define the first concrete `SOFTSIM` demo that can illustrate a `System 1` / `System 2` cadence without requiring full robot behavior, custom peripherals, or timing claims.

## Design goal

The demo should show:

- a persistent fast reactive loop,
- a slower supervisory process,
- explicit communication between them,
- a visible state change in the reactive node when supervision changes mode.

This is enough to support the architectural claim in the paper:

- the cadence between reactive and supervisory layers can be made executable on a distributed node fabric.

## Smallest workable topology

Use two adjacent nodes on chip `0`:

- `000` as the fast reactive node
- `001` as the slower supervisory node

Reason:

- they are simple to describe,
- they can communicate directly through a single neighboring port,
- they keep the demo small enough for `SOFTSIM` inspection.

## Chosen communication style

Use direct node-to-node port communication rather than a testbed or external I/O.

Reason:

- `DB001` and `DB013` already make ports the natural communication primitive,
- it keeps the example faithful to the architecture,
- it avoids needing custom testbed code.

The supervisory node will periodically write a mode token.
The reactive node will repeatedly read or await that token and branch its loop behavior accordingly.

## Behavioral story

The initial demo can use just two mode values:

- mode `0`: reactive node stays in loop `A`
- mode `1`: reactive node stays in loop `B`

The supervisory node does this:

1. initialize mode `0`
2. delay for some loop count
3. send mode `1`
4. delay again
5. optionally send mode `0` again
6. repeat

The reactive node does this:

1. wait for incoming mode token
2. if token is `0`, run reactive loop `A`
3. if token is `1`, run reactive loop `B`
4. continue waiting for the next supervisory update

## What should differ between loop A and loop B

The difference should be visible in `SOFTSIM` without external hardware.

Good options:

- change a RAM cell repeatedly in one pattern for `A` and another for `B`
- change register `A` or `B` usage in a distinctive way
- alternate between two small code regions so the focus view clearly shows different active addresses

The safest first choice is:

- reserve two RAM words in node `000`
- in loop `A`, repeatedly store one value to a known RAM location
- in loop `B`, repeatedly store a different value to the same location

That gives a visible internal state marker without depending on I/O semantics.

## Recommended node responsibilities

### Node 000: reactive layer

Behavior:

- initialize internal state
- enter wait / receive cycle
- branch immediately based on received mode
- run compact repetitive loop until next supervisory update

What it demonstrates:

- low-latency local response
- persistent embodied-style loop
- explicit dependence on supervisory token, but not on continuous supervision

### Node 001: supervisory layer

Behavior:

- run a slower count-controlled loop
- periodically emit control tokens to node `000`
- do little else

What it demonstrates:

- lower-frequency control
- sparse intervention
- modulation rather than direct micromanagement of the fast loop

## Suggested implementation shape

This is not yet final runnable source, but the shape should follow DB013 conventions:

### Assembly sketch

- create one bin for `000`
- create one bin for `001`
- use `ASM[` / `]ASM`
- use `NODE`, `ERS`, `org`, labels, `begin`, `again`, `if`, `then`
- use a direct neighboring port literal such as `right` / `left` or the cardinal aliases

### Reactive node sketch

Conceptually:

```forth
ASM[ # 000 NODE ERS # 0 org
  : wait-mode
    ( read one token from supervisor )
  : mode-a
    ( repeatedly write marker A to RAM cell )
  : mode-b
    ( repeatedly write marker B to RAM cell )
  >BIN ]ASM
```

### Supervisory node sketch

Conceptually:

```forth
ASM[ # 001 NODE ERS # 0 org
  : send-a
    ( write mode 0 toward node 000 )
  : send-b
    ( write mode 1 toward node 000 )
  : delay
    ( count-controlled loop )
  >BIN ]ASM
```

## Suggested BDL loading shape

Use `SOFTSIM`'s documented BDL-driven loading path:

- `000 +NODE` with code from its own bin
- `001 +NODE` with code from its own bin
- set `/P` so each node starts at its loaded code rather than default warm

Conceptually:

```forth
SOFTSIM LOAD
FRAME[
  000 +NODE  !ND
  001 +NODE  !ND
  # <entry-000> /P
  # <entry-001> /P
]FRAME
```

The exact loading wrapper may need to follow the style of the documented `1737` block once or if we recover a concrete example.

## What to inspect in SOFTSIM

Focus points:

- `Node 000` should spend most of its time in one of two small loop regions.
- `Node 001` should spend most of its time in a delay loop, with occasional port writes.
- the memory word used as the reactive marker should alternate values when supervision changes mode.
- breakpoints can be placed:
  - on the branch after token receipt in node `000`
  - on the supervisory send routine in node `001`

## Why this demo is paper-useful

It demonstrates all of the following without overclaiming:

- distributed embodiment across nodes,
- separation of fast and slow roles,
- explicit inter-layer handoff,
- functional observability in `SOFTSIM`.

It does not require claims about:

- real-time correctness,
- full robot semantics,
- external sensing,
- human-level cognition.

## Immediate next move

Turn this note into a stricter pseudo-source stub:

1. pick exact entry addresses and RAM marker addresses,
2. choose the exact neighbor port names for `000 <-> 001`,
3. write a draft assembler/BDL block in note form,
4. only after that look for a place in the repo to hold runnable experimental source.
