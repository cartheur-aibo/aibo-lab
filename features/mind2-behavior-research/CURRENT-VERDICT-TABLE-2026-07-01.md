# Current Simulator Verdict Table: July 1, 2026

This note compresses the current host-side simulator findings into one table.

It is not a claim about Sony's exact internal runtime logic. It is the current
best conclusion boundary from the repo's hypothesis-driven MIND 2 simulator.

## Trees Compared

- `baseline`
  - `features/aibo-mind2/build/stick`
- `0400`
  - `features/mind2-behavior-research/build/aibo-mind2-0400-specimen`
- `0400 + 1200`
  - `features/mind2-behavior-research/build/aibo-mind2-0400-1200-specimen`
- `0400 + IEG`
  - `features/mind2-behavior-research/build/aibo-mind2-0400-plus-ieg`

## Verdict Matrix

| Probe | baseline | `0400` | `0400 + 1200` | `0400 + IEG` |
| --- | --- | --- | --- | --- |
| `mind2-routine-sleep-probe.scn` | sleep | sleep | sleep | postpone sleep to preserve routine |
| `mind2-routine-disruption-probe.scn` | routine broken, sleep | routine broken, sleep | routine broken, sleep | routine preserved, stays awake |
| `mind2-social-disruption-probe.scn` | routine broken, sleep | seek interaction briefly, then sleep | seek interaction, stays awake | routine preserved, stays awake |
| `mind2-low-salience-disruption-probe.scn` | routine broken, sleep | routine broken, sleep | seek interaction briefly, then sleep | routine preserved, stays awake |
| `mind2-neutral-disruption-probe.scn` | routine broken, sleep | routine broken, sleep | registers presence, redirects briefly, then sleep | routine preserved, stays awake |

## Current Reading

### `0400`

- remains the strongest shutdown-side pivot
- by itself, it does not create durable routine preservation
- by itself, it does not sustain social persistence under weaker probes

### `1200`

- currently reads as a social-weighted bridge
- strongest after socially salient `head_touch`
- weaker after `back_touch`
- still noticeable under neutral nearby-presence cues
- but not strong enough on its own to preserve wakefulness under the
  lower-salience and neutral probes

### specimen-side sparse `IEG.CFG`

- currently reads as a routine-preserving bridge
- remains distinct under:
  - no-touch sleep pressure
  - routine disruption
  - touch-plus-disruption
  - low-salience disruption
  - neutral-cue disruption

## Practical Meaning

The current simulator now supports a three-part interpretation:

1. `0400`
   - shutdown bias
2. `1200`
   - socially weighted persistence
3. sparse specimen-side `IEG.CFG`
   - routine preservation

That is the clearest current host-side separation achieved so far.
