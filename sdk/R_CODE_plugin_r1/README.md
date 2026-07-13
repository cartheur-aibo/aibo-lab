# R-CODE Plug-In For OPEN-R SDK

This directory contains Sony's `R-CODE Plug-in Release 1.0 for OPEN-R SDK`
material for the `ERS-7` SDK track.

It should be treated differently from [src/R-CODE](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/README.md),
which preserves the older `ERS-110/111` standalone R-Code package.

## Scope In This Repo

- primary target: understand the `ERS-7` SDK-hosted R-Code runtime and plug-in workflow
- secondary target: compare the older standalone `ERS-111` R-Code scripts with the later `ERS-7` interpreter and plug-in model
- out of scope: treating this kit as a drop-in replacement for the `ERS-111` package

## What Is Here

- `README_E.txt` and `README_J.txt` are the preserved Sony release notes
- `OPEN_R_SDK/OPEN_R/MS_ERS7/RCODE/{Jpn,Eng}/OPEN-R` contains the base `ERS-7` Memory Stick image for the R-Code plug-in environment
- `sample/RMain` contains the disclosed `RMain` source code for the R-Code main object
- `sample/RPlugIn` contains a buildable sample plug-in, sample script, and staging files
- `VERSION.txt` identifies this package as `R-CODE Plug-in Release 1.0 for OPEN-R SDK`

## Why This Matters

This package gives us a real `ERS-7`-era R-Code path:

- R-Code is no longer just a standalone older-model script package
- the SDK base includes an `MS_ERS7/RCODE` Memory Stick tree
- `RMain` shows the interpreter structures, statements, messages, and plugin hooks
- `RPlugIn` shows how custom commands can extend R-Code on top of `OPEN-R`

That makes this directory the right place for `ERS-7` R-Code investigation,
while [src/R-CODE](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/src/R-CODE/README.md)
remains the `ERS-111` preservation track.

## Minimal Workflow

Sony's preserved English instructions in `README_E.txt` and
`sample/RPlugIn/README_E.txt` reduce to:

1. copy `OPEN_R_SDK/OPEN_R/MS_ERS7/RCODE/{Jpn,Eng}/OPEN-R` to a programming stick
2. build `sample/RPlugIn`
3. copy `sample/RPlugIn/MS/OPEN-R` onto that same stick
4. boot the robot with the staged stick

## Representation Hook

The sample script at:

- [sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/R-Code.R](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/sdk/R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/R-Code.R:1)

can be analyzed with:

```bash
python3 scripts/extract-rcode-behavior.py \
  sdk/R_CODE_plugin_r1/sample/RPlugIn/MS/OPEN-R/APP/PC/AMS/R-Code.R
```

For a checked-in example output, see:

- [sample/RPlugIn/R-Code-Behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/sdk/R_CODE_plugin_r1/sample/RPlugIn/R-Code-Behavior.md)
