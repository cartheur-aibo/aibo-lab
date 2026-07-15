# SOFTSIM Cadence Demo v3

Status: `Traceable draft`

This note turns the current cadence demo into a source-traceable working draft.
Its purpose is to keep three things separate:

- what `DB013` explicitly documents,
- what the current `SOFTSIM` cadence example proposes,
- what still needs confirmation before we can call the example runnable.

## Why this version exists

The current research value depends on not blending:

- documented `arrayForth` and `SOFTSIM` capabilities,
- our interpretation of a `System 1` / `System 2` cadence,
- a future claim about physical `GA144` manifestation.

This note therefore treats the demo as a disciplined implementation sketch
rather than as an already-validated source listing.

## Directly supported by DB013

The following points are grounded in the local chapter notes from `DB013`:

- Chapter 4 documents a per-node assembly workflow using `ASM[` / `]ASM`.
- Chapter 4 documents labels, structured control words, and cardinal aliases
  such as `east` and `west`.
- Chapter 6 documents BDL as the declarative mechanism for node loading and
  initialization.
- Chapter 6 documents directives including `+NODE`, `/RAM`, and `/P`.
- Chapter 7 documents the `SOFTSIM LOAD` followed by BDL-loading workflow.
- Chapter 7 documents `SOFTSIM` as a high-level functional simulator rather
  than a timing-faithful model.

These are the stable foundations for the demo.

## Proposed cadence interpretation

The minimal cadence interpretation remains:

- node `001` acts as the slower supervisory source,
- node `000` acts as the faster local reactive loop,
- `001` occasionally sends a mode token to `000`,
- `000` updates its local behavior based on the most recent token.

This is not claimed to be a full cognitive architecture. It is only the
smallest distributed pattern that demonstrates:

- persistent local activity,
- sparse supervisory intervention,
- visible state change attributable to inter-node communication.

## What is still interpretive

The following items are still design choices rather than confirmed recovered
source:

- the exact node pair `000` and `001`,
- the exact RAM marker addresses `x20` and `x21`,
- the exact send and receive words used in the pseudo-source,
- the exact delay-loop form for node `001`,
- the exact BDL wrapper syntax for the smallest two-node load block.

These choices are reasonable because they fit the documented model, but they
should remain labeled as proposed until tested or matched to a known block.

## Most defensible minimal claim

At this stage, the strongest claim we can make is:

> `SOFTSIM` and the documented `arrayForth` workflow appear sufficient to host
> a tiny two-node fast/slow cadence example in which one node performs a local
> reactive loop and a neighboring node occasionally alters its mode through
> direct communication.

That claim is narrower and safer than saying the demo is already runnable.

## Draft implementation contract

To keep the effort disciplined, the demo should satisfy this contract:

- exactly two nodes are involved,
- no custom testbed is required,
- no timing claim depends on `SOFTSIM`,
- the supervisory effect is visible in node-local memory,
- the communication path is direct and easy to inspect.

If a future version violates those constraints, it is no longer the minimal
cadence demonstration.

## Working source outline

The current outline from `v2` remains the best working structure:

### Node 000

- wait on a neighbor port,
- store the received token in `MODE-MARK`,
- branch into one of two compact local loops,
- update `COUNT-MARK` in a way that is visually distinguishable.

### Node 001

- emit mode `0`,
- spend time in a delay loop,
- emit mode `1`,
- repeat.

### Loader

- enter `SOFTSIM`,
- load a tiny BDL description,
- initialize both nodes with RAM images and start addresses.

## Verification targets

Before the example can be described as more than a traceable draft, we should
verify at least one item in each category:

- syntax:
  confirm exact assembly and BDL forms against a real environment or recovered
  source block
- behavior:
  confirm that the supervisor token actually changes node `000` behavior
- observability:
  confirm that `MODE-MARK` and `COUNT-MARK` can be inspected cleanly in
  `SOFTSIM`
- scope:
  confirm that the example works without introducing undocumented simulator
  internals

## Relationship to the paper

This note belongs in `docs/sim/SOFTSIM` because it is implementation-facing.
For the paper in `arxiv/`, the safer distilled message is:

- prior robotics literature already motivates fast/slow architectures,
- `SOFTSIM` gives us a functional environment for exploring one distributed
  implementation pattern,
- real timing, electrical behavior, and final manifestation still belong to
  physical `GA144` evaluation.

## Best next technical step

The next practical move is not to enlarge the example. It is to tighten one of
these:

1. recover a closer documented tiny `SOFTSIM` source shape from the DB
   material,
2. translate `v2` into stricter arrayForth-style notation with explicit
   uncertainty markers,
3. create a paper-safe one-paragraph summary of the cadence demo that cites
   only validated points.

That sequence keeps the research contribution valuable while reducing the risk
of mixing hypothesis with documentation.
