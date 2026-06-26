# ERS-7 8 MB Stick Evidence Statement

This note records the current evidence about whether we know enough to build a
simple bootable ERS-7 programmable 8 MB Memory Stick reliably.

## Bottom Line

At present, the evidence does not justify claiming that we have a reliable
8 MB ERS-7 stick recipe.

The last round of repeated failures is still more consistent with unresolved
media, base-image, firmware, or stick-preparation uncertainty than with a
solved OPEN-R deployment workflow.

## What The `open-r-dev-knowledge` Repo Improves

The sibling repo at `../open-r-dev-knowledge` improves our troubleshooting
discipline, but not enough to close the 8 MB question.

It strengthens several points:

- boot failure is usually an integration problem first, not an application
  logic problem
- the first checks should be firmware level, full Memory Stick layout, base
  software choice, battery state, and known-good media
- `WCONSOLE` often looks like a dead stick if used as though it were a normal
  standalone runtime image
- 8 MB versus 16 MB is usually not the main variable as long as the image fits
- image construction should start from a pristine Sony base tree and then
  overlay project files in a repeatable way

Useful source files:

- [../open-r-dev-knowledge/context/openr_bbs_context.md](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/context/openr_bbs_context.md:72>)
- [../open-r-dev-knowledge/context/openr_bbs_troubleshooting_rules.md](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/context/openr_bbs_troubleshooting_rules.md:5>)

## What It Still Does Not Prove

The same knowledge base does not give us a decisive low-level recipe for making
an arbitrary 8 MB stick behave like a genuine Sony programmable stick.

It does not resolve:

- hidden copy-protection behavior
- reader-specific corruption risks
- whether our exact 8 MB media is acceptable to the robot
- whether our current ERS-7 needs a different base image in practice
- whether another ERS-7-specific boot expectation exists outside the basic SDK
  sample workflow

The archive repeatedly warns that reader choice and formatting can destroy stick
usability or silently change the stick into something AIBO will not accept.

Relevant evidence:

- quick formatting may restore FAT structure, but full formatting is strongly
  discouraged for programmable sticks
- Sony-brand readers are repeatedly recommended because non-Sony readers may
  damage hidden copy-protection behavior

Useful source lines:

- [openr_bbs_messages.csv](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/data/openr_bbs_messages.csv:11740>)
- [openr_bbs_messages.csv](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/data/openr_bbs_messages.csv:11771>)

## What The Archive Suggests About 8 MB Versus 16 MB

The archive cuts against the idea that 8 MB itself is the key blocker.

It contains direct statements that:

- 16 MB programmable sticks work across AIBO models
- older 8 MB PMS sticks were simply older and discontinued
- 8 MB versus 16 MB should not matter if the program fits

Useful source lines:

- [openr_bbs_messages.csv](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/data/openr_bbs_messages.csv:37644>)
- [openr_bbs_messages.csv](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/data/openr_bbs_messages.csv:37650>)
- [openr_bbs_messages.csv](</home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/open-r-dev-knowledge/data/openr_bbs_messages.csv:38720>)

## Current Evidentiary Position

The strongest honest statement we can make now is:

1. We have enough evidence to troubleshoot 8 MB stick bring-up more
   systematically than before.
2. We do not yet have enough evidence to say we know how to build a simple
   bootable ERS-7 8 MB programmable stick reliably.
3. The open questions now look more like media authenticity, stick preparation,
   base-image choice, and robot-specific expectations than like missing OPEN-R
   application files alone.

## Operational Consequence

The next useful move is not to keep treating the 8 MB path as already
understood.

The better order is:

1. prove a minimal image on known-good programmable media
2. prefer `BASIC` or `WLAN` over `WCONSOLE` for first boot proof
3. build staged images by overlaying onto a pristine Sony base tree
4. only then retry the 8 MB path and treat any remaining failures as
   media-specific evidence
