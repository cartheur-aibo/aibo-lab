# Candidate Files For Higher-Order MIND 2 Behavior

These are the most promising first places to investigate when a preserved MIND
2 image shows distinctive live behavior.

## High-Priority Persistent State

Under `OPEN-R/APP/DATA/P/`:

- `AWAKING.CFG`
  likely relevant to wake/startup state
- `RVAR`
  likely runtime or relationship-like variable storage
- `GVAR`
  likely global state storage
- `FVAR`
  likely feature, flags, or learned-state storage
- `STTLOG`
  plain-text state/log output worth parsing first
- `OWNER.TXT`
  owner-facing persistent data
- `IEG.CFG`
  persistent config that may affect behavior
- `ARRMAIN.BIN`
  possible higher-level arrangement/state container
- `ARSVDATA.BIN`
  possible reserved state container
- `SIDRDATA.BIN`
  possible side or session state container
- `FIFDATA.BIN`
  candidate learned/persistent binary data
- `EPTN.BIN`
  candidate pattern/state data
- `ESCENE.BIN`
  candidate scene or behavior-state data
- `TR.CT`
  candidate training or transition-related file
- `INHFLG`
  likely inhibition/flag state

## Secondary Persistent State

Under `OPEN-R/MW/DATA/P/`:

- `REBOOT.DAT`
  reboot-state marker
- `PAT.LOG`
  middleware-side logging that may correlate with behavior
- `AUDIO.ODA`
- `MOTION.ODA`
- `LED.ODA`
- `HDDYNAUD.ODA`
- `DYNAUDIO.ODA`

These may be more about assets than higher-order state, but they can matter if
behavior differences involve motion, sound, or expressiveness rather than only
decision state.

## Identity / Binding Clues

Under `OPEN-R/SYSTEM/DATA/P/`:

- `AIBO-ID`
- `IPE.IIB`
- `SRTDSP01.ESP`

These may help answer whether some behavior is tied to robot identity,
calibration, or model-specific state rather than app personality alone.

## Working Hypothesis

The most likely behaviorally meaningful cluster is:

- `OPEN-R/APP/DATA/P/`

That folder should be the first comparison target between:

- preserved known-good MIND 2 references
- repaired working images with distinctive behavior
- future clones after one-variable changes

## Method

For now:

- prefer diffing or hashing whole persistent-state groups before editing
- change one file or one closely-related file group at a time
- always preserve the original specimen image unchanged
