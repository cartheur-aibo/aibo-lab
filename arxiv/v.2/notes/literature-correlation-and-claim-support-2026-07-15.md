# Literature Correlation and Claim Support 2026-07-15

This note combines:

- the literature already staged in `arxiv/v.2/literature/`
- the current paper notes
- a broader external search across primary robotics sources

Its purpose is to clarify which claims are already well supported, which are
only partly supported, and what `SOFTSIM` / `GA144` work can credibly be said
to perform.

## Bottom line

The literature now supports the paper best when it is framed as:

- not inventing the general idea of `System 1` / `System 2`, fast/slow, or
  layered-timescale embodied control
- contributing an implementation-oriented perspective on how such a cadence
  might be instantiated on a distributed asynchronous substrate
- using `SOFTSIM` as a functional execution and inspection environment
- relating that functional implementation path to eventual realization on
  `GA144`

The literature does not yet support stronger claims such as:

- superiority over current VLA or humanoid-control architectures
- direct equivalence between psychological dual-process theory and a concrete
  robot controller
- timing validation of a robot architecture through `SOFTSIM` alone

## What the local literature already supports

The local staged literature gives strong support for the claim that fast/slow
or dual-system architectures are already active in robotics research.

### Explicit dual-system or fast/slow examples

- `GR00T N1: An Open Foundation Model for Generalist Humanoid Robots`
  - dated `March 18, 2025`
  - explicitly describes a dual-system humanoid architecture
  - `System 2` handles vision/language interpretation
  - `System 1` generates fluid motor actions in real time

- `Hume: Introducing System-2 Thinking in Visual-Language-Action Model`
  - dated `May 27, 2025`
  - explicitly uses `System 2` and `System 1` language
  - `System 2` performs low-frequency value-guided thinking
  - `System 1` is a lightweight reactive visuomotor policy operating
    asynchronously in real time

- `Language-Conditioned Robotic Manipulation with Fast and Slow Thinking`
  - dated `January 8, 2024`
  - explicitly frames robotic manipulation in fast/slow cognitive terms
  - separates an instruction discriminator and a slower reasoning-capable
    subsystem from faster policy execution

### Related layered-timescale examples

- `Architecture Is All You Need: Diversity-Enabled Sweet Spots for Robust
  Humanoid Locomotion`
  - supports the broader pattern of layered or decoupled control timescales
  - useful as correlation, but weaker than the explicit dual-system examples

- `AgiBot World Colosseo`
  - supports scale, data, and generalist embodied-policy development
  - does not by itself support an explicit `System 1` / `System 2` claim

### Industry-context material

- `we all are robots - EP.108.pdf`
  - useful for industry context, dexterity, sensing, and hardware momentum
  - not strong evidence for explicit fast/slow control architecture on its own

## What the broader search adds

The broader search reinforces that the field direction is real and current, not
just local to a few staged PDFs.

### Additional external correlation

- `Gemini Robotics: Bringing AI into the Physical World`
  - dated `March 25, 2025`
  - supports a split between embodied reasoning and direct robot control
  - useful for showing that reasoning-plus-action decomposition is broadening
    across major robotics foundation-model efforts, even when not labeled
    explicitly as `System 1` / `System 2`

- `LeVERB: Humanoid Whole-Body Control with Latent Vision-Language Instruction`
  - dated `June 16, 2025`
  - supports hierarchical separation between high-level semantic guidance and
    low-level whole-body control
  - useful correlation for layered control, though it is not a direct
    `SOFTSIM` or asynchronous-node analogue

### What this means for the paper

This broader search strengthens one important positioning claim:

- the paper is entering an already-active conversation about separating slower
  semantic or deliberative layers from faster motor or reactive layers

It also sharpens the novelty boundary:

- the paper should not claim novelty in the mere existence of a fast/slow split
- the paper can still claim novelty in implementation emphasis if it stays
  centered on distributed asynchronous realization and functional executable
  workflow

## What DB001 and DB013 support about SOFTSIM and GA144

The strongest support for `SOFTSIM` and `GA144` still comes from the local
primary documentation rather than the broader robotics web search.

### What can be claimed safely

From `DB001` and `DB013`, plus the live local execution work:

- `GA144` is a distributed many-node asynchronous substrate built around F18A
  nodes and direct inter-node communication
- `arrayForth 3` supports per-node assembly and persistent object-bin workflow
- boot and deployment are expressed declaratively through BDL-like mechanisms
- `SOFTSIM` can functionally load and execute distributed node programs
- `SOFTSIM` provides operator-visible inspection, stepping, and breakpoint
  control
- the local installation successfully ran:
  - `SOFTSIM LOAD`
  - `1698 LOAD`
  - `1737 LOAD`
  - stepping with `SUPER`

### What this implies about what SOFTSIM/GA144 work can perform

The current evidence supports saying that `SOFTSIM` / `GA144` work can perform:

- functional execution of distributed asynchronous node programs
- operator-visible exploration of node-level state and communication behavior
- implementation experiments involving small reactive and supervisory node
  organizations
- a concrete path from behavior diagram to node implementation to simulation to
  hardware realization

The current evidence does not support saying that `SOFTSIM` / `GA144` work can
already perform:

- full contemporary humanoid VLA capability
- large-scale embodied reasoning comparable to current cloud-scale robot
  foundation models
- timing-faithful validation of embodied robotic control
- proven superiority for real-world dexterous humanoid deployment

## Best-supported paper claims

The following claims are now the strongest.

### Claim A

Recent robotics literature already contains explicit fast/slow or dual-system
control architectures.

Support:

- `GR00T N1`
- `Hume`
- `Language-Conditioned Robotic Manipulation with Fast and Slow Thinking`
- `Gemini Robotics`
- `LeVERB`

### Claim B

The paper's contribution is better framed as implementation-oriented rather
than concept-priority-oriented.

Support:

- the external robotics literature already occupies the general fast/slow
  architecture space
- `DB001` and `DB013` provide a concrete distributed asynchronous implementation
  path that is different in emphasis from monolithic foundation-model work

### Claim C

`SOFTSIM` is a valid functional testbed for studying a small distributed
reactive/supervisory cadence.

Support:

- `DB013 Chapter 7`
- live local execution of `SOFTSIM LOAD`, `1698 LOAD`, and `1737 LOAD`
- live stepping with `SUPER`

Boundary:

- this is a claim about functional execution and observability
- it is not yet a claim about physical timing or robot-level deployment success

## Claims that still need careful wording

These claims remain plausible but should still be treated carefully:

- that `GA144` is especially well suited to `System 1` embodied control
- that slower supervisory cognition can remain logically centralized while fast
  reactive behavior is physically distributed
- that a small `SOFTSIM` cadence demo is representative of larger embodied
  robot architectures

These are good research directions, but they still depend on implementation
evidence more than on literature alone.

## Recommended wording direction

A safe synthesis would be:

> Current robotics literature already includes multiple explicit fast/slow or
> dual-system control architectures. The present work does not claim priority
> over that general idea. Its distinct contribution is to explore how a similar
> reactive/supervisory cadence may be instantiated on a distributed
> asynchronous substrate, functionally exercised in SOFTSIM, and related to
> deployment on GA144 hardware.

## Practical recommendation

For the next paper draft, the best evidence mix is:

- use Kahneman only as conceptual vocabulary
- use `GR00T N1`, `Hume`, and `RFST`-style work for direct fast/slow robotics
  support
- use `Gemini Robotics` and `LeVERB` as broader correlation for hierarchical or
  reasoning-plus-control decomposition
- use `DB001`, `DB013`, and the live local `SOFTSIM` execution evidence for the
  implementation and toolchain claims
