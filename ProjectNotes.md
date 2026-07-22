# Project Notes

## July 2026

An Aibo laboratory that replaces iCybie.

### 2026-07-20

#### 19:10

Nominate Chuck Moore for the Turing Prize: `DONE` 22.07.2026-1140.

#### 19:45

Repo state snapshot.

The repo is currently centered on two connected tracks: OPEN-R / Aperios
host-side runtime reconstruction under `aperios/` and ERS-7 MIND 2
behavior-forensics work under `features/mind2-behavior-research/`, with the
second track presently furthest along operationally.

The main lab infrastructure looks established:

- preserved MIND 2 and MIND 3 reference material
- staging and deployment workflows for Sony-formatted sticks
- Debian-side OPEN-R lifecycle proof using the Sony `HelloWorld` sample
- a behavior-level simulator with a quality roadmap
- prepared live-hardware trial trees under `trials/01-baseline` through
  `trials/04-0400-plus-ieg`

Repo guidance in `README.md`, `NEXT.md`, and `PLAN.md` is aligned around using
the simulator as a hypothesis engine rather than a substitute for bench
evidence.

Immediate next-step suggestions:

- prioritize live ERS-7 validation using the prepared trial order and
  observation template
- turn the current simulator verdict expectations into a small regression
  harness so distinctions stop drifting silently
- refactor simulator trace output to better separate file evidence from
  inferred signals
- only expand simulator semantics when a planned bench discriminator exists
  for the new claim
