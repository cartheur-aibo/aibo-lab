# Live Stick Scan: July 1, 2026

Scanned image:

- `local-artifacts/live-mind2-sdb.img`

SHA-256:

- `362bab6a0e7f36843ce884ee404adc275621035f0572f9966641770d71f2066f`

## Raw-layout finding

The captured stick has the same Sony 32 MB class geometry as the preserved
reference media:

- MBR partitioned image
- active FAT12 partition
- partition start sector `19`
- partition length `63341`
- partition payload offset `9728`
- cluster size `16384`

This means the startup problem is not explained by the low-level Sony media
formatting alone.

## Filesystem finding

The captured live image does **not** contain the AIBO boot payload:

- no `MEMSTICK.IND`
- no `OPEN-R/`
- no `OPEN-R/BOOTPARA`

It contains only a small Sony/Palm-style tree plus host metadata:

- `PALM/`
- `System Volume Information/`
- `.fseventsd/`
- `.Trashes/`

This is consistent with a Sony-formatted stick that is not currently an ERS-7
boot stick.

## Practical interpretation

If this exact stick is inserted into an ERS-7, the robot would not find the
expected `OPEN-R` runtime and application tree. That matches the observed
symptom where the robot emits the unhappy startup tones and powers off.

## Recovery direction

The most direct recovery path is to write a known-good ERS-7 boot tree onto
this Sony-formatted 32 MB media, preserving the Sony formatting:

1. stage a known-good MIND 2 tree
2. copy `MEMSTICK.IND` and `OPEN-R/` to the stick root
3. eject cleanly
4. retest on the robot

The preserved known-good content baselines in this repo are:

- `opt/AIBO7M2/`
- `local-artifacts/sony-mind2-reference.img`

The important lesson from this case is:

- valid Sony 32 MB formatting is not sufficient
- the boot payload itself must also be present
