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
5. All four retail files look like dense binary blobs rather than lightly
   encoded text:
   - almost no printable strings
   - no embedded `/MS/...` paths
   - no visible header or magic string
6. The retail file sizes fall exactly on `24`-byte boundaries:
   - MW: `168` bytes = `7 x 24`
   - SYSTEM: `216` bytes = `9 x 24`
7. The broader retail config family is consistently aligned to `8`-byte
   boundaries, not just `OBJECT.CFG`. For example:
   - `APP/CONF/OBJECT.CFG`: `224` bytes on MIND 2, `232` on MIND 3
   - `SYSTEM/CONF/EXTOBJ.CFG`: `96` bytes
   - `SYSTEM/CONF/CARDDRV.CFG`: `120` bytes
   - `MW/CONF/CONNECT.CFG`: `920` bytes
   - `MW/CONF/DESIGN.CFG`: `56` bytes
   - `MW/CONF/DESIGNDB.CFG`: `584` bytes
   - `MW/CONF/SERVICE.CFG`: `528` bytes
8. Some opaque retail config files share the exact same first `8`-byte block:
   - `SYSTEM/CONF/EXTOBJ.CFG`
   - `SYSTEM/CONF/CARDDRV.CFG`
   Both start with `9073ab72cf85c9bb`.
9. A second pair also shares the same first `8`-byte block:
   - `MW/CONF/CONNECT.CFG`
   - `MW/CONF/DESIGNDB.CFG`
   Both start with `0e207d53993b8df4`.
10. `MW/CONF/CONNECT.CFG` changes across MIND 2 and MIND 3 in a structured way,
    not an all-bytes-random way:
    - the first `64` blocks (`512` bytes) differ
    - the last `51` blocks (`408` bytes) are byte-identical in the same
      positions
    - that stable tail begins exactly at byte `512`
    - `408` bytes = `17 x 24`, which matches the `24`-byte unit size already
      suspected from retail MW/SYSTEM `OBJECT.CFG`
    - the final `4` blocks are identical across both versions
11. The only layer with an unchanged cross-version object roster is `MW`, and
    that same layer is where the stable `CONNECT.CFG` suffix appears:
    - `MW/OBJS`: `12` objects on MIND 2 and `12` on MIND 3, same set
    - `SYSTEM/OBJS`: MIND 3 adds `WLANDRVM.BIN`
    - `APP/OBJS`: MIND 3 adds `SA.BIN` and `STM.BIN`

This is the first strong sign that at least one opaque retail config file has a
stable trailing region and a version-specific leading region.

That does not prove the internal format, but it is our strongest current
structural clue. A fixed-length record table is now a reasonable hypothesis.
The broader `8`-byte alignment also makes a compiled and possibly encrypted
`64`-bit-block format a plausible hypothesis. The repeated leading blocks also
argue for shared headers, type keys, or deterministic structure across the
opaque retail config family.

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

For simulator/tooling work, the current best characterization is:

- retail `OBJECT.CFG` is probably not just compressed plain text
- retail `OBJECT.CFG` may be a compact fixed-record binary table
- retail `OBJECT.CFG` may also sit inside a wider Sony binary-config format
  family built around `8`-byte alignment
- some members of that family also fit cleanly on `24`-byte boundaries, while
  others only show `8`-byte alignment
- repeated leading `8`-byte blocks in sibling config files make a shared header
  or type-key scheme plausible
- at least one sibling config (`CONNECT.CFG`) has a stable tail and changed
  prefix across MIND versions, which suggests sectioned structure rather than
  uniform whole-file churn
- the `MW/CONNECT.CFG` stable tail is especially interesting because the Sony
  Programmer's Guide says `CONNECT.CFG` describes subject/observer connections
- if so, the next decoding step is to explain what one `24`-byte record means
  and whether the stable `17`-record suffix corresponds to a fixed middleware
  connection graph

## CPU Reference Provenance

To support lower-level interpretation, the repo now also carries a CPU-family
reference manual:

- [local-artifacts/cpu-docs/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/local-artifacts/cpu-docs/README.md)
- [local-artifacts/cpu-docs/R4000_Preliminary_Users_Manual_1992.pdf](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/local-artifacts/cpu-docs/R4000_Preliminary_Users_Manual_1992.pdf)

Why it is relevant:

- the Sony OPEN-R Programmer's Guide says AIBO uses a MIPS CPU in `32-bit`
  `little endian` mode
- a local AIBO/Webots technical report in this repo describes AIBO robots in
  ERS-7 context as targeting a MIPS `R4000` processor
- the same guide says:
  - `OBJECT.CFG` enumerates executable objects
  - `CONNECT.CFG` describes subject/observer service connections

That is not proof of the exact ERS-7 chip SKU, but it is strong enough to keep
the R4000 manual as a reference during binary-format and runtime analysis.

## Simulator Consequence

The boot-layout simulator should treat retail `OBJECT.CFG` as:

- a detected encoded/binary retail format
- not a plain-text `/MS/...` object list
- not evidence that the stick loads zero objects

## Open Technical Question

We still do not know the retail encoding or compiled structure of these files.

That remains a useful future simulator/tooling problem, but it is now separate
from the current higher-order behavior-difference question.
