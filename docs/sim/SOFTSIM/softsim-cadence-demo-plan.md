# SOFTSIM Cadence Demo Plan

Status: `Interpretive`

Question:

- Given that the repo has very little direct `SOFTSIM` source material, what is the smallest credible fast/reactive plus slow/supervisory demo we can design from the available DB documentation?

Sources:

- `docs/sim/README.md`
- `arxiv/v.2/notes/DB013-ch4-6-7-arxiv-notes.md`
- `DB013 Chapter 7`, especially:
  - block `1698` as a full supported initialization example,
  - block `1737` as a simple assembled-program example,
  - `SOFTSIM` operator vocabulary and BDL-driven loading model.
- `BEHAVIORS.md`

Observed repo facts:

- The repo does not currently expose an obvious local `SOFTSIM` source block or tiny example ready to run.
- `docs/sim/README.md` explicitly points to `1698 LOAD`.
- The extracted `DB013` text says:
  - block `1698` loads `SOFTSIM` plus a larger chip configuration,
  - block `1737` is the simpler example that assembles a small program, loads it into node `000`, and sets up the view.
- The repo's existing behavior writing already emphasizes:
  - `Boot`,
  - reactive embodied loops,
  - low-latency monitoring,
  - `System 1 / System 2` cadence as an interpretive framing.

Minimum viable demo concept:

- Use two nodes rather than one:
  - one `System 1` node as the fast reactive loop,
  - one `System 2` node as the slower supervisory node.
- Keep the communication primitive simple:
  - one sends a control word or mode word,
  - the other changes behavior based on that word.
- Avoid external testbeds at first:
  - no custom peripheral model is needed if we only want cadence and handoff.

Recommended first demo:

## Node roles

- `Node 000`: fast reactive loop
  - repeatedly waits for a supervisory token or checks a shared condition,
  - emits one of two simple visible internal states depending on the current mode,
  - stays in a compact loop at all times.
- `Node 001` or adjacent node: slow supervisory loop
  - runs a lower-frequency or more delayed loop,
  - periodically changes the mode word sent to `Node 000`,
  - acts as the cadence setter rather than the fast actuator layer.

## Behavior to demonstrate

- Start in mode `A`.
- `System 1` loop continuously behaves as `A`.
- After some supervisory interval, `System 2` sends mode `B`.
- `System 1` immediately changes to behavior `B`.
- The demo therefore shows:
  - continuous reactive operation,
  - intermittent supervisory intervention,
  - a visible handoff between the two layers.

## What to watch in SOFTSIM

- the fast node should appear to spend most of its time in a tight repeatable loop,
- the supervisory node should change state less often,
- the inter-node communication point should be visible in node state or memory,
- the focus view should let us inspect the exact instruction slot/address where mode changes occur.

Why this is enough for the paper:

- It does not try to prove general intelligence or full robot behavior.
- It does show an executable cadence:
  - persistent reactive loop,
  - slower supervisory modulation,
  - explicit communication across nodes.
- That is exactly the level of demonstration needed to support the architectural claim.

What not to attempt first:

- a large many-node architecture,
- complex external I/O,
- timing claims,
- anything requiring a custom `SOFTSIM` testbed before we have the minimal node-to-node cadence working.

Next implementation move:

1. search for any accessible source block or sample that resembles the documented `1737` style tiny program,
2. if none exists, write a fresh note-backed pseudo-design using the documented assembler and BDL idioms,
3. only then promote it into code or a runnable scenario.
