# DB013 Chapters 4, 6, and 7 Notes for arXiv Paper

Source documents:
- `docs/sim/DB013.pdf` - `arrayForth 3 User's Manual`
- `docs/sim/DB001.pdf` - `F18A Technology Reference`

This note captures the material from DB013 Chapters 4, 6, and 7, with the DB001 material that Chapter 4 explicitly relies on.

## Scope and relationship between the two documents

DB013 Chapter 4 opens by telling the reader to study DB001 first, because DB001 defines the F18A execution model that the arrayForth assembler targets. For the paper, the clean division is:

- `DB001` supplies the architectural substrate: F18A memory map, instruction set, instruction-word format, stack semantics, I/O space, communication ports, and port execution.
- `DB013` Chapters 4, 6, and 7 supply the software workflow: how code is assembled into persistent per-node object bins, transformed into boot streams through BDL and the Streamer, and then loaded into the high-level simulator SOFTSIM.

## DB001 details needed to understand DB013 Chapter 4

### F18A memory and addressing

- The F18A uses a 9-bit address space plus control bits in `P`.
- RAM occupies `x000-x03F` and is mirrored at `x040-x07F`.
- ROM occupies `x080-x0BF` and is mirrored at `x0C0-x0FF`.
- I/O occupies `x100-x1FF`.
- Incrementing `A` or `P` wraps within RAM or ROM, but does not auto-increment through I/O.
- `P9` enables Extended Arithmetic Mode but does not change memory decoding.

These details matter in DB013 because the assembler's `org`, inline instruction generation, jump-range handling, and boot descriptors all assume exact target addresses.

### Instruction format and execution model

- Each 18-bit instruction word contains up to four 5-bit opcodes, with jump/call instructions consuming the remaining low bits as destination.
- Jumps may appear only in slots 0-2.
- Slot 3 accepts only a restricted subset of opcodes.
- `;` and `ex` terminate execution of the current word; `unext` loops within the same word.

This is the basis for DB013's slot-management directives, forward-transfer handling, inline instruction generation, and warnings about jump range.

### Core opcodes and stacks

- DB001 defines arithmetic, memory, and control-transfer opcodes such as `@p`, `!p`, `@+`, `!+`, `if`, `-if`, `next`, `unext`, `+`, `+*`, `dup`, `drop`, `a!`, `b!`, `r>`, and `>r`.
- The machine has circular data and return stacks, with `T`, `S`, and `R` as exposed top registers.
- `@p` and `!p` are central because DB013 uses them for literals, stream pumps, and port execution patterns.

### I/O, suspension, and communication ports

- Nodes communicate through half-duplex point-to-point ports `right`, `down`, `left`, and `up`.
- Port transfers suspend until the peer performs the complementary read or write.
- Multiport addresses are legal and are used for idle/warm execution and routing.
- Port execution is a first-class feature: an F18A can execute instruction words fetched directly from a communication port, with `P` not incrementing while execution remains on that port.

This is crucial for DB013 Chapter 6 boot-stream pump structure and for DB013 Chapter 7 simulation semantics.

## DB013 Chapter 4: Programming the F18

### Overall model

DB013 Chapter 4 describes arrayForth 3 as an incremental assembler environment for many F18 nodes. Instead of monolithic recompilation, source is assembled into persistent object storage, and those object images are later used for IDE loading, simulation loading, and boot-stream construction.

### Object bins

- Assembly output is stored in persistent per-node "bins."
- Each bin is one block and contains:
  - a RAM image,
  - a ROM image,
  - a label table.
- There is one physical bin per node plus extra virtual bins for shared or reusable code.
- Incremental assembly means object code survives reboot and can be reused without recompiling everything.

For the paper, this is an important software-engineering point: the system separates source maintenance from deployment/loading, enabling reproducible node images and reuse across tooling.

### EXAMINE utility

The `EXAMINE` utility audits object bins:

- compares current RAM/ROM images to references,
- dumps RAM or ROM,
- disassembles address ranges,
- displays symbols,
- allows promoting current content to reference.

This shows that the toolchain is not just an assembler; it includes persistent inspection and regression-style comparison of generated per-node machine code.

### Assembler modes

Chapter 4 distinguishes two assembly modes:

1. Normal assembly into a node's RAM or ROM image.
2. Inline generation of an instruction word as a data literal, especially for port execution.

Inline instructions are built with `A[ ... ]]`, optionally with `=P` to state the assumed execution address. This is important because port-executed instructions can have different valid jump ranges from memory-resident code.

### Assembler state and code-bounding directives

The assembler tracks:

- the current instruction word under construction,
- the next available slot,
- the current instruction address,
- the next target memory address,
- the most recent call slot for tail optimization,
- a mode flag controlling whether numbers become literals or remain host-side numeric arguments.

Important directives include:

- `ASM[` / `]ASM` for full F18 assembly mode,
- `A[` / `]]` for inline instruction generation,
- `#` to keep a numeric token as a host-side argument instead of assembling it as an F18 literal,
- `NODE`, `BIN`, `ERS`, `-ROM`, `>ROM`, `>BIN` for selecting and writing working images,
- `org`, `here`, `,`, `+cy`, `-cy`, `<sl` for target address and slot management.

This chapter makes clear that arrayForth is not a textual macro-assembler alone; it is a structured host/target meta-environment that reasons about instruction slots, destination widths, and node identity.

### Control structure directives

High-level structured control is provided by directives such as:

- `if`, `-if`, `zif`, `ahead`, `then`,
- `for`, `next`, `unext`,
- `begin`, `while`, `until`, `again`, `repeat`, `else`.

These compile to F18 control transfers while checking slot/range constraints. That is useful for the paper because it demonstrates that the programming model stays close to Forth while targeting a very constrained asynchronous machine.

### Opcode naming and human factors

DB013 intentionally renames several colorForth opcode spellings to avoid confusion with standard Forth vocabulary:

- `inv` replaces `-`,
- `xor` replaces `or`,
- `r>` replaces `pop`,
- `>r` replaces `push`.

This is a notable design choice: the environment explicitly reduces programmer traps in a mixed host/target Forth setting.

### Labels, named literals, and directional abstractions

Chapter 4 supports:

- per-bin labels,
- named literals for ports and registers (`io`, `right`, `down`, `left`, `up`, `data`, `ldata`),
- cardinal-direction aliases (`east`, `south`, `west`, `north`),
- named multiport calls and `await`.

The directional aliases matter because node layouts may move during floorplanning, and the documentation explicitly tries to reduce bugs caused by odd/even row and column orientation changes.

### Module organization

A recommended module structure is:

1. a load block that assembles all needed source,
2. a boot descriptor block that specifies how the resulting code should be loaded,
3. optional residual paths for modules that stay resident and obstruct later loading,
4. optional IDE personalization, scripts, or other support blocks.

This is a key bridge from Chapter 4 to Chapters 6 and 7: compilation products are expected to feed both boot-stream generation and simulator initialization.

### Methods of loading code

Chapter 4 ends by naming four loading strategies:

- manual interactive loading with External or Internal IDE,
- automated loading into SoftSim using boot descriptors,
- automated boot-stream loading into a real chip,
- fully automated flash boot after reset.

This is the conceptual handoff into Chapters 6 and 7.

## DB013 Chapter 6: Preparing Boot Streams

### Why boot streams exist

Boot streams solve the problem of loading and initializing many nodes, and possibly external devices such as SRAM, in a coordinated way. The chapter explicitly contrasts this with manual IDE work: IDE is ideal for testing, while boot streams are the mechanism for practical whole-system bring-up.

### Boot-stream model

- A boot stream consists of one or more boot frames.
- Each frame includes data words, a destination start address, and a jump address.
- Boot nodes can chain frames by jumping to concatenation addresses.
- Streams may be delivered by async serial, sync serial, SPI flash, or Snorkel injection.

### Streamer utility

The `STREAMER` tool builds streams in two phases:

1. compose the stream in unpacked 18-bit word form,
2. repack it for the chosen transport medium.

The root node determines header format and packing:

- `708` for async serial,
- `300` for synchronous serial,
- `207` for Snorkel injection with no headers,
- `705` for SPI flash with packed byte-oriented encoding.

Important Streamer words include:

- `STREAM[`,
- `COURSE`,
- `FRAME[`,
- `]FRAME`,
- `/ROOT`,
- `FORWARD`,
- `]STREAM`,
- `.STREAM`,
- `?STREAM`,
- `?FLASH`.

### Boot Descriptor Language (BDL)

BDL is the declarative layer shared by automated loading tools, including SoftSim loading and real-chip stream generation.

The chapter states that BDL:

- touches all nodes on a defined path,
- defaults to minimal initialization,
- lets the user override per-node memory and register setup,
- accumulates node-specific directives in tables rather than enforcing declaration order.

Key BDL directives:

- `+NODE` to select a node entry,
- `/RAM`, `/SOME`, `/PART` for full or partial RAM loads,
- `/B`, `/A`, `/IO`, `/P` for register initialization,
- `/STACK`, `/RSTACK` for stack initialization,
- `!ND`, `+ND`, `ITS`,
- register and directional names for addressing.

There are also macros like `/RINF`, `+wire`, and `+dly`.

For the paper, BDL is probably the most important Chapter 6 contribution because it provides a single declarative representation for node initialization that can drive simulation, live booting, and flash images.

### Stream structure

The chapter breaks a frame into:

- an optional header,
- port pumps along the path,
- memory loads for each node,
- post-load initialization code,
- optional root-node programming.

Important implementation detail:

- loading proceeds along a path, and the first programmed node is the last node in the path,
- each node gets the code needed to pump the remainder of the stream further,
- initialization always includes at least the execution state needed to leave the node at its idle or specified start point.

This is strong evidence that the toolchain treats the chip fabric as an active loader network rather than a passive memory target.

### Root-node and SPI special cases

Chapter 6 gives special treatment to root-node programming and to SPI flash:

- the root node may need its own frame,
- SPI concatenation can lose alignment if the root program accesses flash directly,
- `FORWARD` pads and patches the stream so execution can resume correctly at a byte-aligned continuation point.

This is important if the paper discusses why boot generation is more than serialization of RAM images.

### Transmitting streams

From `saneFORTH`, async streams are sent through node `708`.

From `pF/144`, streams can be injected very quickly through Snorkel-rooted node `207`, or prepared for other delivery mechanisms. The examples in the chapter show streams being used not only for initial boot but also for adding capabilities, such as Ethernet support, into a live system.

### Flash writing and auditing

The chapter also documents:

- writing generated streams into boot flash,
- flash erasure behavior,
- unpacking and saving reference streams,
- comparing newly generated streams against references with `?STREAM` and `?FLASH`,
- `.LOADING` for showing what a generated stream will do.

This is useful evidence that the environment supports inspection and reproducibility of deployment artifacts, not just ad hoc loading.

## DB013 Chapter 7: Simulation Testing with SOFTSIM

### What SOFTSIM is

SOFTSIM is described as a high-level software simulator for up to two GA144 chips. Its charter is to reproduce the same value-level results as real hardware, but not the same physical timing. The manual is explicit that SOFTSIM is not a faithful asynchronous timing simulator.

For the paper, that makes SOFTSIM best characterized as a functional or behavioral simulator rather than a cycle-accurate or timing-accurate model.

### Simulation model and limits

The chapter makes several strong claims about abstraction level:

- the simulator assumes a fixed cycle time,
- all instructions begin on cycle boundaries,
- display state is shown before the next opcode begins execution,
- coarse delays are inserted for long operations such as memory access,
- inter-node timing and wall-clock timing are only rough approximations.

The documentation repeatedly warns that code respecting the communication rules should usually produce the same logical results in SOFTSIM as on real chips, but timing-sensitive behavior can diverge in either direction. That warning is especially important for any paper language about validation: SOFTSIM is appropriate for logic debugging, not for establishing timing limits.

### User interface and visualization model

After `SOFTSIM LOAD`, the display is split into:

- a full-chip view of two 8x18 arrays,
- an overview pane showing a movable 8x4 rectangle of nodes,
- a focus-node pane with detailed register, memory, and stack state,
- a normal FORTH terminal for control.

Visual encoding includes:

- `#` for nodes inside the current overview,
- `O` for the focus node,
- `=` for all other nodes,
- background colors showing suspended, active, or presumed-dead nodes.

This suggests that SOFTSIM is designed as an operator-facing inspection environment, not merely a batch simulator.

### Meaning of displayed state

The overview and focus panes show state as it exists immediately before the next opcode executes. The opcode shown is therefore the next opcode to run, unless the node is in an instruction fetch, suspension, or a delayed operation. Addresses are displayed either as RAM/ROM hex or symbolic I/O names.

The overview for each node summarizes:

- port and pin activity,
- node number and recent address-bus value,
- current slot and opcode,
- `I`, `P`, `A`, `B`, `IO`, `R`, `T`, and `S`,
- delay and suspension state.

The focus pane expands this with:

- full `cyyxx` node identity,
- symbolic and numeric address interpretations,
- instruction-register disassembly,
- a 16-cell memory dump around the current execution area,
- visible return and data stacks,
- explicit `SUSPENDED`, `DELAYING`, or `LOCKUP!` status.

### Simulator time

SOFTSIM exposes both a cycle count and an approximate elapsed time since simulated reset. The manual says this "time" is only a rough abstraction, accumulated at about `1.4 ns` per simulated cycle, and is mainly useful for perspective rather than measurement.

### How code is loaded into SOFTSIM

This chapter ties directly back to Chapter 6: SOFTSIM directly understands Boot Descriptor Language. The documented workflow is:

1. `SOFTSIM LOAD`,
2. load some BDL description,
3. let SOFTSIM initialize the simulated chip state from those descriptors.

By default, node RAM starts in a power-up-like state and nodes are initialized to reset `P` values. If the simulator is allowed to run without loading code, nodes simply suspend, mirroring the real machine's quiescent behavior after reset.

For the paper, this is the strongest Chapter 6-to-7 connection: BDL is the shared declarative artifact used for both boot/deployment and simulation setup.

### Operating vocabulary

The simulator provides direct operator commands:

- `OVIEW` to move the overview window,
- `SEE` to choose a focus node,
- `OTHER` to toggle with the previous focus node,
- `MEM` and `-MEM` to freeze or unfreeze memory display origin,
- `Z` for a single global step,
- `FAST` and `SUPER` for multi-step execution with reduced display updates,
- `SS` for operator-driven single stepping,
- `BREAK` and `-BREAK` for one breakpoint per node.

This further supports describing SOFTSIM as an interactive simulator/debugger rather than a static visualization tool.

### Breakpoints and stepping

Breakpoints are defined by node, address, and slot. SOFTSIM stops before the cycle in which the selected instruction would begin or resume. The manual notes that suspension and delayed operations require care in breakpoint choice, because a breakpoint applies to the instruction start condition rather than to every suspended or delayed substate.

### Testbeds

Chapter 7 explicitly introduces testbeds: pieces of code that simulate external circuitry well enough to exercise F18 programs. The provided examples include `TB-SRAM`, which simulates external SRAM attached to nodes `7`, `8`, and `9`.

This is a useful paper point because it shows SOFTSIM is not only a core-node simulator; it also has a pluggable environment model for selected peripherals and external interfaces.

### Scope of documented introspection

The chapter closes by noting that internal SOFTSIM vocabulary for probing state is intentionally undocumented because the simulator is still under development. That suggests the public interface is the operator vocabulary and display model, while deeper internal hooks are unstable.

## Suggested paper framing

If you want a compact interpretation for the arXiv paper, these chapters support the following claims:

- `DB001` defines the F18A as a highly minimalist asynchronous multi-node machine with first-class port execution and suspension-based communication.
- `DB013 Chapter 4` shows that arrayForth 3 builds a persistent per-node compilation model around that machine, including symbolic assembly, slot-aware instruction construction, and reusable object bins.
- `DB013 Chapter 6` shows that deployment is expressed declaratively through BDL and then materialized as transport-specific boot streams for simulation, live loading, or flash boot.
- `DB013 Chapter 7` shows that SOFTSIM reuses that same initialization model to provide high-level, interactive functional simulation with explicit visibility into node state, but without physical timing fidelity.

That combination makes the software stack look less like a conventional compiler plus simulator and more like an integrated programming environment for a communicating array of tiny asynchronous computers.
