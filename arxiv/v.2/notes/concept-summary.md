# System 1 and System 2 Summary

This note summarizes the author's description of `System 1` and `System 2`
from the opening section of Kahneman's book.

## Core distinction

Kahneman presents two broad modes of thought:

- `System 1`: fast, automatic, frequent, emotional, stereotyped, and largely
  unconscious
- `System 2`: slow, effortful, infrequent, logical, calculating, and conscious

The distinction is not simply about speed. It is also about:

- automatic response versus deliberate control
- low-effort pattern completion versus active attention
- intuitive judgment versus explicit reasoning

## Typical examples of System 1

Examples given for `System 1`, in roughly increasing complexity, include:

- determining that one object is farther away than another
- localizing the source of a sound
- completing a common phrase such as `war and ...`
- displaying disgust when seeing a gruesome image
- solving a very basic arithmetic problem such as `2 + 2`
- reading text on a billboard
- driving on an empty road
- thinking of a good chess move, for an expert player
- understanding simple sentences

## Typical examples of System 2

Examples given for `System 2` include:

- preparing for the start of a sprint
- directing attention toward particular people in a crowded environment
- searching for a person with a specific feature
- trying to recognize a difficult sound
- sustaining a faster-than-normal walking rate
- judging whether an action is socially appropriate
- counting the number of occurrences of a letter in a text
- giving one's own telephone number to someone else
- parking in a tight space
- evaluating the price-to-quality ratio of products
- checking the validity of complex logical reasoning
- multiplying two-digit numbers such as `17 x 24`

## Related concepts named by the author

The book also associates this distinction with themes such as:

- coherence
- attention
- cognitive laziness
- association
- jumping to conclusions
- `WYSIATI` (`What You See Is All There Is`)
- judgment formation under limited evidence

## High-level interpretation

The summary supports a familiar reading:

- `System 1` handles fast, patterned, low-deliberation responses
- `System 2` handles explicit attention, control, checking, and reasoning

This is useful for our paper because it gives a psychologically recognizable
motivation for a fast/slow architecture. But by itself, it does not specify an
implementation substrate, a robotics control stack, or a distributed machine
organization.

## Paper-facing analysis

For our work, this note is most valuable as conceptual motivation, not as
direct implementation evidence.

What it helps support:

- why a fast/reactive versus slow/supervisory split is a meaningful framing
- why a cadence between low-effort response and higher-effort control is worth
  studying
- why embodied systems might benefit from separating immediate action from more
  deliberative modulation

What it does not by itself support:

- that Kahneman's framework is a robotics architecture
- that `System 1` and `System 2` map directly onto `GA144` nodes
- that a `SOFTSIM` implementation is implied by the psychological literature
- that the paper is novel merely because it uses the labels `System 1` and
  `System 2`

## Best use in the paper

The safest use of this material is:

- cite it as part of the conceptual lineage for fast/slow cognition
- pair it with direct robotics literature for actual architecture claims
- reserve the paper's contribution for the implementation-oriented question:
  how such a cadence might be instantiated on a distributed asynchronous
  substrate and explored in `SOFTSIM`

## Recommended framing sentence

One paper-safe interpretation would be:

> Kahneman's `System 1` / `System 2` distinction provides a useful conceptual
> vocabulary for separating rapid reactive processing from slower deliberative
> control, but the present work is concerned not with that psychological theory
> alone, rather with how an analogous cadence might be instantiated and studied
> on a distributed asynchronous computational substrate.
