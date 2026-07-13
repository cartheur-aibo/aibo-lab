# Behavior Aggregate

This note summarizes the state titles currently present across the
generated `R-CODE` behavior diagrams under
`src/R-CODE/generated/*.behavior.md`.

Scope:

- includes all generated `*.behavior.md` files except `Football-Behavior.md`
- reflects the current normalized family and variant set
- reflects the most recent cleanup of placeholder state titles

## Corpus Totals

- diagrams counted: `54`
- total state instances: `292`
- unique state titles: `47`
- raw placeholder titles remaining: `0`

## Most Common State Titles

| Count | State Title |
|---|---|
| `56` | `Sense / Decide` |
| `51` | `Action Loop` |
| `36` | `Boot / Safe Pose` |
| `32` | `Synchronize` |
| `18` | `Boot` |
| `14` | `Sense Fall State` |
| `14` | `Recover` |
| `13` | `Head Scan Position` |
| `8` | `Repeat Forward Walk` |
| `4` | `Left Kick` |
| `4` | `Right Kick` |
| `3` | `It approaches a ball by the angle of the head` |

## Secondary State Titles

| Count | State Title |
|---|---|
| `2` | `Quit Behavior` |
| `2` | `Start Ball Tracking` |
| `2` | `Mode` |
| `2` | `Slow Forward Walk` |
| `1` | `Main` |
| `1` | `Increment Lost Counter` |
| `1` | `Reset Lost Counter` |
| `1` | `Set Search Mode` |
| `1` | `Turn Left` |
| `1` | `Shift Left` |
| `1` | `Advance Left` |
| `1` | `Search Turn` |
| `1` | `Shift Right` |
| `1` | `Advance Right` |
| `1` | `Forward Walk` |
| `1` | `Happy Hug Response` |
| `1` | `It sleeps until it is set up` |
| `1` | `Idle Thought` |
| `1` | `Baby Sway Forward` |
| `1` | `Baby Sway Backward` |
| `1` | `When there was an obstacle in front` |
| `1` | `Reset Sample Count` |
| `1` | `When Distance became less than 300` |
| `1` | `At the time of loop_cnt=1` |
| `1` | `At the time of loop_cnt=2` |
| `1` | `Wait loop end` |
| `1` | `Select First Average` |
| `1` | `Reset Scan Counter` |
| `1` | `Start` |
| `1` | `Load Stored Poses` |
| `1` | `Save Current Poses` |
| `1` | `Resume Listener` |
| `1` | `MainLoop` |
| `1` | `Clear Light Playback` |
| `1` | `Power Down` |

## Interpretation

The aggregate is dominated by `Sense / Decide`, `Action Loop`, and
`Synchronize`. That suggests the preserved `ERS-111` sample set is best
read as a family of small reactive loops rather than long scripted
sequences.

`Boot`, `Boot / Safe Pose`, `Sense Fall State`, and `Recover` are also
strong recurring elements. This shows that initialization and bodily
stability are not marginal concerns in the sample corpus; they are part
of its common behavioral architecture.

More specialized state titles such as `Left Kick`, `Right Kick`, and
`Start Ball Tracking` appear much less often. Those belong to narrower
families such as `Football` and related pursuit variants rather than to
the overall baseline structure of the corpus.
