# SOFTSIM Actual Execution Steps 2026-07-15

Status: `Live execution log`

This note records the steps that were actually executed on
`2026-07-15` in the local installed `arrayForth 3` environment.

Its purpose is to separate:

- the operator draft we planned,
- the steps we actually performed,
- the behaviors we directly confirmed.

## Confirmed launcher

The Plasma launcher exists locally as:

- `/home/cartheur/.local/share/applications/aF-3 DEFAULT SOFTSIM.desktop`

Its confirmed executable entry is:

- `/home/cartheur/ame/af3-04d/af3/projects/DEFAULT/afksoftsim.sh`

The launcher's working path is:

- `/home/cartheur/ame/af3-04d/af3/sF`

## Confirmed launcher script behavior

The script launches:

- `./sF6a2-af3.elf "INCLUDE ../projects/DEFAULT/custom.txt"`

This means the effective session starts from:

- the local `sF6a2-af3.elf` binary,
- in the `sF` directory,
- with the `DEFAULT` project bootstrap file included.

## Confirmed project bootstrap

The included file is:

- `/home/cartheur/ame/af3-04d/af3/projects/DEFAULT/custom.txt`

That file confirms the environment is wired to:

- `DEFAULT.src`
- `DEFAULT-back.src`
- `OBJ-DEFAULT`
- `conc`

and then drops into an `ok` prompt in the `AFORTH` environment.

## Actual execution sequence

The following sequence was actually executed:

1. launch the local `arrayForth 3` environment using the installed SOFTSIM path
2. reach the `arrayForth` help screen and `ok` prompt
3. enter `SOFTSIM LOAD`
4. confirm that the SOFTSIM help/operator interface appears
5. enter `1698 LOAD`
6. confirm that a populated SOFTSIM state loads successfully
7. enter `10 SUPER`
8. confirm that the simulator advances rather than remaining at a static load image
9. enter `1737 LOAD`
10. confirm that a smaller initialized SOFTSIM state loads successfully
11. enter `10 SUPER`
12. confirm that the smaller example also advances correctly

## Actual commands entered

These are the exact commands that were typed in the live session:

```text
SOFTSIM LOAD
1698 LOAD
10 SUPER
1737 LOAD
10 SUPER
```

## What SOFTSIM showed after `SOFTSIM LOAD`

After `SOFTSIM LOAD`, the environment displayed the documented operator help,
including the following commands:

- `OVIEW`
- `SEE`
- `OTHER`
- `MEM`
- `BREAK`
- `SS`
- `SUPER`
- `FAST`

This confirms that the simulator vocabulary described in the local notes is
present in the installed environment.

## What `1698 LOAD` confirmed

`1698 LOAD` did not fail. It produced a populated simulator state rather than a
blank or help-only screen.

That matters because it confirms:

- the documented larger SOFTSIM example is present in this installation,
- the BDL-driven initialization path is functional,
- the local environment is usable for continued SOFTSIM work.

## What `10 SUPER` confirmed

`10 SUPER` advanced the simulator.

The visible result included:

- cycle count: `10 cyc`
- approximate time: `0.0140 ~us`

This confirms that:

- simulation stepping works,
- the time and cycle counters update,
- the session was executing, not merely displaying loaded state.

## What `1737 LOAD` confirmed

`1737 LOAD` is also present in the installed environment and produces a much
smaller initialized simulator state than `1698`.

That matters because it confirms:

- the smaller documented SOFTSIM path is available locally,
- the environment supports both a larger supported setup and a small example,
- `1737` is the better template for the first custom cadence experiment.

The visible focused node after loading `1737` included:

- node `000`
- `p=200`
- cycle count initially at `0 cyc`

This matches the expectation that the small example starts from a compact,
single-focus setup.

## What stepping `1737` confirmed

After `1737 LOAD`, `10 SUPER` also advanced correctly.

The visible result again included:

- cycle count: `10 cyc`
- approximate time: `0.0140 ~us`

In the stepped state, the focused instruction area showed execution moving into
the small example's code region around:

- `loc=2D3`
- `p=2D4`

This is useful because it shows the small example is not only loadable, but
actually live and stepping through its own code path.

## Most important validated facts from today

Today we directly validated:

- the exact local SOFTSIM launcher path,
- the exact local binary path,
- the project bootstrap path,
- successful entry into `SOFTSIM`,
- successful loading of block `1698`,
- successful simulator stepping with `SUPER`,
- successful loading of block `1737`,
- successful stepping of the smaller `1737` example,
- successful recovery of the exact stored `1737` block text from disk,
- failure of hand-entered retyping of the `1737` assembly body at `clc`.

## What remains unvalidated

We have not yet validated:

- the exact smallest hand-entered two-node BDL frame,
- the first custom cadence example for nodes `000` and `001`,
- the marker-inspection workflow for `x20` and `x21` in our own draft example.

We also now know that one additional issue remains:

- the exact difference between block-context execution and hand-entered prompt
  execution for the `1737` assembly body

## Best next step

The strongest next step is:

1. compare the live `1737` interaction pattern with the notes in
   `softsim-programming-sequence.md`
2. treat the recovered `1737` block as the authoritative baseline
3. investigate why direct prompt re-entry fails at `clc`
4. use block-context mutation, rather than freehand prompt entry, for the
   first custom cadence test

That keeps the next phase grounded in an example that is both documented and
now close to the installed environment.
