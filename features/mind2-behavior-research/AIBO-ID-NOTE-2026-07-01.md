# AIBO-ID Note: July 1, 2026

The first preserved-specimen comparison found that `AIBO-ID` differs between:

- repaired preserved specimen image
- older preserved MIND 2 reference image

## Values

Restored specimen:

- `1c 1b 30 00`

Reference:

- `ac 22 30 00`

## Immediate Observation

- the last two bytes are identical: `30 00`
- the first two bytes differ

## Why This Matters

This suggests at least some of the preserved stick state may be tied to robot
identity, binding, or calibration context rather than only high-level app
personality state.

That does **not** prove identity coupling is the cause of the observed
behavioral difference, but it is now a serious candidate.

## Practical Consequence

Future transplant tests should be designed in two classes:

1. identity-neutral tests
   that avoid changing `AIBO-ID`
2. identity-sensitive tests
   that intentionally preserve or swap `AIBO-ID` together with app-state files

Comparing those two classes should help answer whether distinctive behavior is:

- transferable by app-state alone
- or dependent on identity-bound state
