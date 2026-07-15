# Claim Ledger

Use this file to keep paper-facing claims separate from broader research exploration.

## Claim Status Legend

- `Established`: directly supported by cited documentation, code, or hardware evidence.
- `Interpretive`: a careful synthesis across sources, suitable for the paper if phrased modestly.
- `Speculative`: promising but not yet ready for paper claims.

## Template

### Claim

Status: `Established | Interpretive | Speculative`

Paper-safe wording:

> Add one or two sentences here.

Support:

- Source 1:
- Source 2:
- Hardware evidence:

Boundary:

- What this claim does support:
- What it does not support:

Promotion decision:

- `Ready for draft`
- `Needs more support`
- `Keep in notes only`

## Working Entries

### Claim

Status: `Interpretive`

Paper-safe wording:

> The behavior diagrams can be treated as executable organizational models when they are mapped onto concrete F18 node ensembles, initialized through the same deployment machinery used by the toolchain, and exercised functionally in SOFTSIM before hardware realization on GA144.

Support:

- `DB001`: F18A node, port, memory, and execution semantics
- `DB013 Chapter 4`: per-node assembly and object-bin workflow
- `DB013 Chapter 6`: BDL and boot-stream initialization
- `DB013 Chapter 7`: SOFTSIM as high-level functional execution environment

Boundary:

- What this claim does support:
- A continuous path from behavioral organization to executable distributed implementation
- What it does not support:
- Timing equivalence between SOFTSIM and physical GA144 execution

Promotion decision:

- `Needs more support`
