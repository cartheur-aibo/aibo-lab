# R-CODE 110/111

This folder appears to contain a Sony `R-CODE110/111` package for
`ERS-110/111`.

R-Code is a simplified scripting language for programming AIBO, and the
programmer manual describes it as a language similar to `BASIC`. For
this project, that makes it the most practical low-risk software path to
try for `ERS-111` before deeper `OPEN-R` work.

## Scope In This Repo

Treat this material as `ERS-111`-first.

- primary target: understand and preserve the original `ERS-110/111` R-Code workflow
- acceptable secondary use: use it as background when comparing how Sony's scripting and stick conventions may have evolved into `ERS-210` era tooling
- out of scope: treating this package as a direct `ERS-7` programming path

For the later `ERS-7` SDK-hosted R-Code track, use
[sdk/R_CODE_plugin_r1/README.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/sdk/R_CODE_plugin_r1/README.md).

## What Is Here

- `Document/` contains the readme, programmer manual, reference manual, and support lists
- `Document/readme.md`, `Document/Reference.md`, `Document/MTN-FFORM-E.md`, `Document/ODA-FFORM-E.md`, `Document/LED_Light_List.md`, `Document/Sound_List.md`, and `Document/Tone_List.md` are cleaned-up Markdown reading editions of the preserved text docs
- `Document/Programmers.md` is a cleaned-up Markdown reading edition of the programmer manual
- `Document/Behavior_Abstraction.md` lifts the sample scripts into higher-level behavior blocks and flow diagrams
- `Document/SoccerDog1-Behavior.md` is the first checked-in generated Mermaid state-diagram example from the extractor
- `Document/Behavior_Abstraction.md` pairs well with the `ERS-7` plug-in sample extract under [sdk/R_CODE_plugin_r1/sample/RPlugIn/R-Code-Behavior.md](/home/cartheur/ame/aiventure/aiventure-github/cartheur-aibo/openr-debian/sdk/R_CODE_plugin_r1/sample/RPlugIn/R-Code-Behavior.md)
- `MSImage/OPEN-R/` contains the bootable image tree for a programming stick
- `sample/` contains example `.R` scripts such as `AngryDog.R`, `MoveAIBO.R`, and `PlaySound.R`
- `env.png` appears to be an environment or packaging image for the kit

## Operational Notes

- the package says it is only for `AIBO ERS-110/111`
- it says to use an `AIBO Programming Memory Stick`
- it says `8MB` or `16MB` sticks are usable
- it says `16MB or greater` is not usable due to hardware limits
- it expects a script named `R-CODE.R` to be placed in `\OPEN-R\`

## `aperios.gz`

The file `MSImage/OPEN-R/aperios.gz` is not a source archive or a normal
desktop executable.

What we found:

- it is a real `gzip` file
- compressed size is `1,068,532` bytes
- decompressed size is `3,560,288` bytes
- the decompressed payload does not look like `ZIP`, `tar`, `ELF`, or a common filesystem image
- the raw bytes look like a packed `MIPS`/`Aperios` runtime image

The decompressed image contains many internal runtime strings such as:

- `Scheduler::Initialize`
- `MetaCore::InvokeBoundHandler`
- `ThreadManager`
- `ContextManager`
- `OPENRBusManager`
- `BatteryManager`
- `Timer::Initialize`
- `!!!! Panic !!!!`

That strongly suggests `aperios.gz` is the compressed `Aperios`
system/runtime image used during boot, not just an R-Code script
container.

## Why This Helps

- it gives us a real `ERS-110/111` programming path
- it gives us a staged `OPEN-R` tree we can study without touching original media
- it gives us sample scripts that are easier to reason about than full native `OPEN-R` objects
- it helps separate low-risk `R-Code` experiments from deeper system-level investigation
- it may give us a careful lead-in for later `ERS-210` comparison work without claiming compatibility

## Suggested Next Steps

- preserve and inspect the original working media before any write tests
- compare the original stick layout to `MSImage/OPEN-R`
- confirm the exact media and adapter arrangement used on the real robot
- prepare a separate test stick that matches the documented size limits
- start with a minimal `R-CODE.R` script using simple sound or pose playback
- only move to deeper `OPEN-R` analysis after the boot path is confirmed

## Deeper Analysis Options

- extract a `strings` report from `aperios.gz` into the docs for future reference
- identify likely load addresses and CPU endianness for the decompressed runtime image
- determine whether the R-Code interpreter lives inside `aperios.gz` or elsewhere in the stick tree
