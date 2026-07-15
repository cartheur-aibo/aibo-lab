# SOFTSIM Cadence Demo v2

Status: `Interpretive`

This note refines `softsim-cadence-demo-v1.md` into a stricter pseudo-source
specification.

## Fixed design decisions

### Nodes

- reactive node: `000`
- supervisory node: `001`

### Communication direction

- `001 -> 000` carries the supervisory mode token

Because node `001` is immediately to the east of node `000`, the direct link is:

- from `001`, send on `west`
- from `000`, receive on `east`

This keeps the communication direction visually simple:

- supervisor writes westward
- reactive node reads eastward

### Internal marker addresses

Use two RAM cells in node `000`:

- `x20` as `MODE-MARK`
- `x21` as `COUNT-MARK`

Reason:

- they are ordinary RAM locations,
- they are easy to inspect in `SOFTSIM`,
- they stay away from address `0` where code will begin.

### Mode values

- mode `0` means reactive loop `A`
- mode `1` means reactive loop `B`

### Entry addresses

Use simple fixed entry layout:

- `000` code starts at `x00`
- `001` code starts at `x00`

Within each node:

- early words hold setup and branch logic
- loop bodies begin on aligned later words

## Behavioral intent

### Node 000

Node `000` should:

1. wait for a mode token from node `001`
2. store the received token into `MODE-MARK`
3. branch to loop `A` or `B`
4. in each loop, repeatedly update `COUNT-MARK`
5. return to wait for the next supervisory token

The key property is that node `000` spends most of its time in a compact local
loop and only changes mode when node `001` intervenes.

### Node 001

Node `001` should:

1. send mode `0`
2. delay
3. send mode `1`
4. delay
5. repeat forever

The key property is that node `001` is slower and sparse: it does not perform
the reactive loop itself, it only modulates node `000`.

## Visible state design

The easiest visible distinction in `SOFTSIM` is:

- loop `A` increments `COUNT-MARK`
- loop `B` decrements `COUNT-MARK`

This gives:

- a persistent changing value in one RAM location,
- a clear difference between the two loop bodies,
- no need for external I/O.

`MODE-MARK` should always show the last supervisory token received.

## Pseudo-source shape

This is still note-level pseudo-source, but it is now specific enough to guide
real implementation.

### Node 000 pseudo-source

```forth
ASM[ # 000 NODE ERS # 0 org

  # x20 equ MODE-MARK
  # x21 equ COUNT-MARK

  : wait-mode
    east        ( receive token from node 001 )
    dup
    MODE-MARK !         ( remember current mode )
    if
      mode-b
    then
    mode-a

  : mode-a
    # x21 a!            ( point A at COUNT-MARK )
    begin
      @
      1.
      +
      # x21 a!
      !
      ahead             ( placeholder for exit back to wait-mode )
    again

  : mode-b
    # x21 a!
    begin
      @
      1.
      inv
      +
      # x21 a!
      !
      ahead             ( placeholder for exit back to wait-mode )
    again

  >BIN ]ASM
```

This is intentionally not final F18-correct source yet. Its purpose is to pin
down structure:

- receive token,
- record mode,
- enter one of two different local loops,
- make the local loop visible in RAM.

### Node 001 pseudo-source

```forth
ASM[ # 001 NODE ERS # 0 org

  : send-a
    0. west !

  : send-b
    1. west !

  : delay
    255. for . . . . next

  : supervisor
    begin
      send-a
      delay
      send-b
      delay
    again

  >BIN ]ASM
```

Again, the exact F18 source will need cleanup, but the architecture is now
fixed.

## BDL loading sketch

The demo should be loaded by BDL in the smallest possible form:

```forth
SOFTSIM LOAD

FRAME[
  000 +NODE 000 /RAM
  0. /P

  001 +NODE 001 /RAM
  0. /P
]FRAME
```

The exact syntax may need adjustment depending on the actual environment, but
the important point is:

- only two nodes are loaded,
- each begins execution at `x00`,
- no peripheral setup is needed.

## What to inspect first

### In node 000

- `MODE-MARK` at `x20`
- `COUNT-MARK` at `x21`
- branch location after the port read
- loop body location for `A`
- loop body location for `B`

### In node 001

- send routine location
- delay loop location
- the cadence at which it leaves delay and writes a new token

## Expected qualitative behavior in SOFTSIM

- node `001` should look mostly dormant inside delay loops, with occasional
  writes on `west`
- node `000` should switch between two compact repetitive behaviors
- the switch should be attributable to a received port token, not a local timer
- `x20` should show the current mode and `x21` should change according to the
  currently active loop

## Why this version is useful

This is now specific enough to support the next step:

- either a cleaned-up draft in actual arrayForth style,
- or a companion note translating this pseudo-source into stricter F18
  assembler forms.

It is still small enough that we are not overcommitting before we know where a
real runnable source file should live.
