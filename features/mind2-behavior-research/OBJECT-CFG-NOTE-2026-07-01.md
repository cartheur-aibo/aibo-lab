# OBJECT.CFG Note: July 1, 2026

`OBJECT.CFG` became a live question because the boot-layout simulator originally
reported `Objects listed: 0` for retail MIND 2 trees, which could be mistaken
for meaning "the retail stick loads no objects."

That interpretation is false.

## What We Established

1. Retail ERS-7 MIND 2 `OPEN-R/MW/CONF/OBJECT.CFG` is not plain text.
2. Retail ERS-7 MIND 2 `OPEN-R/SYSTEM/CONF/OBJECT.CFG` is also not plain text.
3. The repaired preserved specimen, the older MIND 2 reference, and the
   bundled MIND 2 tree all carry byte-identical retail `OBJECT.CFG` files.
4. MIND 2 and MIND 3 retail `OBJECT.CFG` files differ from each other, but
   keep the same file sizes within their respective MW and SYSTEM roles.

## Sizes

MIND 2:

- MW `OBJECT.CFG`: `168` bytes
- SYSTEM `OBJECT.CFG`: `216` bytes

MIND 3:

- MW `OBJECT.CFG`: `168` bytes
- SYSTEM `OBJECT.CFG`: `216` bytes

## Meaning

For the current behavior investigation, this is the key conclusion:

- `OBJECT.CFG` is not the source of the distinctive repaired-specimen behavior
  we observed

Why:

- it is invariant across all MIND 2 images compared in this session
- while several persistent-state files do differ

## Simulator Consequence

The boot-layout simulator should treat retail `OBJECT.CFG` as:

- a detected encoded/binary retail format
- not a plain-text `/MS/...` object list
- not evidence that the stick loads zero objects

## Open Technical Question

We still do not know the retail encoding or compiled structure of these files.

That remains a useful future simulator/tooling problem, but it is now separate
from the current higher-order behavior-difference question.
