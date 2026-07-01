# Restore A Sony 32 MB Stick As MIND 2

Use this when the raw image shows:

- correct Sony 32 MB style geometry
- but no `OPEN-R/` and no `MEMSTICK.IND`

That means the media formatting is useful, but the ERS-7 boot payload is
missing.

## Source Of Truth

The known-good staged MIND 2 tree is:

- `features/aibo-mind2/build/stick`

If needed, rebuild it from the preserved repo copy:

```bash
./scripts/stage-aibo-mind2.sh
```

## Safest Restore Sequence

1. Mount the Sony stick read-write at a known mountpoint.
2. Run a dry run first:

```bash
./scripts/restore-mind2-stick.sh --dry-run /path/to/mounted-stick
```

3. If the dry run looks right, do the live restore:

```bash
./scripts/restore-mind2-stick.sh /path/to/mounted-stick
```

4. Unmount/eject cleanly.
5. Test in the ERS-7 while powered off before insertion.

## What The Restore Copies

Only these intended top-level entries:

- `MEMSTICK.IND`
- `OPEN-R/`
- `PALM/`
- `StikZap.prc` if present in the staged source

It also strips common host metadata such as:

- `.Trashes`
- `.fseventsd`
- `.DS_Store`
- AppleDouble `._*` files

It removes top-level host noise that is not part of the intended restored
stick layout, including:

- `System Volume Information`

The one top-level non-OPEN-R folder we intentionally keep is:

- `PALM/`

## Why This Is Safer Than Reformatting

The July 1, 2026 live capture showed the stick already had the correct Sony
32 MB class low-level layout:

- active FAT12 partition
- start sector `19`
- partition length `63341`
- 16 KB clusters

So the recovery goal is not to reformat the media. It is to preserve that
layout and restore the missing ERS-7 boot payload.
