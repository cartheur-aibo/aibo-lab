# SOFTSIM and GA144 Experiment Matrix

Status: `Research planning`

This note turns the current paper contribution into a concrete experiment matrix.

The aim is to support the paper with experiments that are:

- small enough to execute credibly,
- aligned with what `SOFTSIM` can actually validate,
- useful for eventual `GA144` manifestation,
- honest about the difference between functional evidence and timing evidence.

## Core principle

The contribution is best supported if we demonstrate:

- a reproducible fast/reactive versus slow/supervisory cadence,
- a traceable mapping from concept to node program,
- functional execution in `SOFTSIM`,
- and, where possible, movement toward physical realization on `GA144`.

The contribution is not best supported by:

- trying to outperform frontier VLA systems,
- claiming timing results from `SOFTSIM`,
- or making broad cognition claims without implementation evidence.

## Experiment matrix

### Experiment 1

Name:

- `1737` baseline replication

Purpose:

- establish the smallest confirmed executable baseline in the local toolchain

Claim supported:

- the local `SOFTSIM` installation is a usable functional execution environment

Method:

- run `SOFTSIM LOAD`
- run `1737 LOAD`
- step with `SUPER`
- inspect node `000`

Evidence type:

- `SOFTSIM-only`

Success criteria:

- successful load
- successful stepping
- stable visible node state and code view

Boundary:

- proves local functional execution only
- does not prove custom behavior yet

### Experiment 2

Name:

- one-node `1737` mutation

Purpose:

- prove that we can alter behavior while preserving the documented minimal load
  structure

Claim supported:

- the toolchain can host a custom executable behavior, not just stock examples

Method:

- clone `1737` in block-context
- mutate only the body of `go`
- keep the loader and display logic as unchanged as possible

Evidence type:

- `SOFTSIM-only`

Success criteria:

- successful custom load
- stepping enters altered code path
- visible difference from baseline `1737`

Boundary:

- still a one-node experiment
- does not yet establish reactive/supervisory cadence

### Experiment 3

Name:

- two-node supervisory override demo

Purpose:

- demonstrate the smallest reactive/supervisory cadence

Claim supported:

- a fast local loop and slower supervisory modulation can be instantiated on a
  distributed node fabric

Method:

- node `000` runs a compact reactive loop
- node `001` periodically sends a mode or control token
- node `000` changes its local behavior when the token arrives

Evidence type:

- `SOFTSIM-first`, later `GA144-candidate`

Success criteria:

- two nodes load and execute
- supervisory token is observable
- reactive behavior changes predictably after supervisory intervention

Boundary:

- proves functional cadence
- does not prove robot-level utility by itself

### Experiment 4

Name:

- local memory marker observability

Purpose:

- show that the cadence is inspectable, not just hypothesized

Claim supported:

- `SOFTSIM` is useful as an operator-facing functional debugger for distributed
  behavior

Method:

- dedicate visible RAM cells such as `MODE-MARK` and `COUNT-MARK`
- inspect them while stepping and running

Evidence type:

- `SOFTSIM-only`

Success criteria:

- memory markers change in expected relation to mode changes
- state can be observed without undocumented internals

Boundary:

- supports observability
- does not imply physical sensor/actuator semantics yet

### Experiment 5

Name:

- centralized versus distributed micro-organization

Purpose:

- compare whether a tiny behavior is easier to express as one node or as
  multiple communicating nodes

Claim supported:

- distribution is not merely decorative; it changes implementation structure

Method:

- implement a tiny behavior once in one node
- implement the same logical behavior in two or more nodes
- compare code organization, inspectability, and handoff structure

Evidence type:

- `SOFTSIM-only`

Success criteria:

- both versions run
- differences in organization are explainable and visible

Boundary:

- not a performance benchmark
- useful mainly as architectural evidence

### Experiment 6

Name:

- behavior diagram to executable mapping

Purpose:

- show that a behavior diagram can become a concrete distributed program

Claim supported:

- the paper's behavior diagrams are executable organizational models, not only
  conceptual sketches

Method:

- choose one paper-facing behavior diagram
- assign functions to specific nodes and links
- map it to per-node code and load structure
- execute the resulting ensemble in `SOFTSIM`

Evidence type:

- `SOFTSIM-first`, later `GA144-candidate`

Success criteria:

- traceable mapping from diagram element to code location
- successful functional execution of the mapped organization

Boundary:

- supports executable interpretation
- does not prove full application readiness

### Experiment 7

Name:

- `GA144` manifestation check

Purpose:

- show that at least one `SOFTSIM` organization can be carried toward hardware

Claim supported:

- the workflow reaches beyond simulation toward physical realization

Method:

- take the smallest stable `SOFTSIM` experiment
- prepare the corresponding deployment path for real hardware
- record what is preserved and what changes

Evidence type:

- `Needs GA144`

Success criteria:

- same node organization is deployable in principle or in practice
- any hardware-specific divergence is documented

Boundary:

- still not a full robot experiment
- strongest as a manifestation and workflow claim

## Claim-to-evidence mapping

### Claim

- `SOFTSIM` can function as a functional cadence testbed

Best supporting experiments:

- `1737` baseline replication
- one-node `1737` mutation
- two-node supervisory override demo
- local memory marker observability

### Claim

- a fast/reactive and slow/supervisory split can be instantiated on a
  distributed asynchronous node substrate

Best supporting experiments:

- two-node supervisory override demo
- centralized versus distributed micro-organization
- behavior diagram to executable mapping

### Claim

- the toolchain supports a path from executable organization to hardware
  realization

Best supporting experiments:

- behavior diagram to executable mapping
- `GA144` manifestation check

## Evidence classes

### `SOFTSIM-only`

Supports:

- functional execution
- inspectability
- logical handoff between nodes
- code-to-diagram traceability

Does not support:

- timing fidelity
- electrical performance
- actuator/sensor margin claims

### `Needs GA144`

Required for:

- timing-sensitive claims
- real I/O claims
- physical embodiment claims
- evidence that a simulated cadence survives actual hardware constraints

## Recommended execution order

1. `1737` baseline replication
2. one-node `1737` mutation
3. local memory marker observability
4. two-node supervisory override demo
5. behavior diagram to executable mapping
6. centralized versus distributed micro-organization
7. `GA144` manifestation check

## Best near-term paper contribution

If we complete only the first four experiments well, we already have a credible
paper core:

- a documented toolchain basis
- a custom executable mutation
- a minimal fast/slow cadence example
- and direct evidence that `SOFTSIM` is a useful functional exploration
  environment

That is enough to support an implementation-oriented contribution without
overclaiming.
