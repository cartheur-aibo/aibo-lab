# Experimental M2 Validator

This validator is the current no-more-blind-plugging gate for the experimental
ERS-7 M2.

Script:

- [scripts/validate-ers7m2-candidate.sh](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/aibo-lab/scripts/validate-ers7m2-candidate.sh)

## What It Does

It combines three existing ideas into one conservative decision:

1. plain-text boot-layout simulation
2. static drift/risk audit against the preserved baseline
3. runtime-style comparison against the preserved baseline

## Current Policy

For the current experimental M2, a candidate is only treated as live-testable
if it is baseline-like enough to pass all of these checks.

That means the validator currently blocks:

- pure SDK runtime-family candidates
- mixed retail-plus-SDK candidates
- unknown runtime-style candidates
- anything the drift audit rates above `LOW`
- any plain-text sample layout with missing referenced objects

This is intentionally strict.

## Why So Strict

Real hardware already told us:

- baseline retail MIND 2 boots normally
- pure SDK `TinyFTPD` `WCONSOLE/nomemprot` caused `doo doo doo` and shutdown

So the current safe interpretation is:

- baseline-like retail candidates may be considered for live testing
- SDK-family replacements should fail validation until we have a stronger model

## Usage

Baseline against itself:

```bash
./scripts/validate-ers7m2-candidate.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7m2-baseline/build/stick
```

Baseline against the TinyFTPD SDK candidate:

```bash
./scripts/validate-ers7m2-candidate.sh \
  features/ers7m2-baseline/build/stick \
  features/ers7-sdk-wconsole-tinyftpd/build/stick
```

Expected current result:

- baseline vs baseline: `PASS`
- baseline vs TinyFTPD: `FAIL`

## Scope

This validator does not emulate Aperios or prove motion safety.

It is a repo policy gate for whether a candidate has earned another real-hardware
attempt on this experimental robot.
