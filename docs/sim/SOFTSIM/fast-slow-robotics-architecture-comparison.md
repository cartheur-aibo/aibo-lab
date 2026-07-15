# Fast/Slow Robotics Architecture Comparison

Status: `Interpretive`

Question:

- Which current embodied robotics systems publicly describe a control or "brain" architecture that resembles the fast/slow cadence being developed here?

Sources:

- `arxiv/v.2/literature/we all are robots - EP.108.pdf`
- `GR00T N1: An Open Foundation Model for Generalist Humanoid Robots`
- `Hume: Introducing System-2 Thinking in Visual-Language-Action Model`
- `AgiBot World Colosseo: A Large-scale Manipulation Platform for Scalable and Intelligent Embodied Systems`
- `Language-Conditioned Robotic Manipulation with Fast and Slow Thinking`
- `Architecture Is All You Need: Diversity-Enabled Sweet Spots for Robust Humanoid Locomotion`

Observed facts:

- `GR00T N1` explicitly presents a dual-system architecture:
  - `System 2` for vision/language interpretation,
  - `System 1` for real-time action generation.
- `Hume` explicitly presents a dual-system embodied control design:
  - low-frequency `System 2` value-guided thinking,
  - lightweight `System 1` reactive action execution.
- `RFST` (`Language-Conditioned Robotic Manipulation with Fast and Slow Thinking`) explicitly frames robotic manipulation with fast and slow thinking modes selected by task type.
- `Architecture Is All You Need` argues for layered timescales in humanoid locomotion:
  - a high-rate proprioceptive stabilizer,
  - a lower-rate perceptual policy.
- The Ziegler EP.108 PDF reports that:
  - `Holiday Robotics` describes a `Vision-Language-Skill (VLS)` stack,
  - `Holiday Sim`, `Holiday Lab`, and `OASys` are part of its deployment stack,
  - `1X` is presented primarily through dexterous hand hardware and capability growth, not through an explicit fast/slow brain description.
- `AgiBot World Colosseo` presents a large-scale embodied data platform and `GO-1` generalist policy, but not an explicit `System 1 / System 2` split in the same sense as `GR00T N1` or `Hume`.

Established:

- There are now multiple public robotics sources that explicitly separate slower semantic or reasoning layers from faster motor-control layers.
- The strongest direct matches to the architecture being developed here are `GR00T N1` and `Hume`.
- `Holiday Robotics` and `AgiBot GO-1` are architecturally adjacent but less directly aligned in their published descriptions.
- The `1X` material currently available to us is better evidence for dexterous embodied sensing/manipulation than for an explicit fast/slow control architecture.

Interpretation:

- This means the present work is not isolated conceptually: there is a recognizable emerging pattern in embodied robotics toward fast/slow decomposition.
- What remains distinctive here is the proposed substrate and execution story:
  - distributed reactive embodiment on a many-node asynchronous machine,
  - `SOFTSIM` as the functional cadence testbed,
  - `GA144` as the concrete hardware manifestation of the reactive layer.
- In other words, the novelty is likely less in the abstract existence of fast/slow layers and more in how concretely and economically they might be implemented and studied in a distributed node fabric.

Boundary:

- What can be claimed safely:
  - similar fast/slow or layered-timescale architectures are now visible in public embodied robotics literature,
  - the strongest directly comparable examples are `GR00T N1` and `Hume`,
  - the current work can be positioned as a distinctive implementation-oriented contribution within that broader trend.
- What should not yet be claimed:
  - that companies like `1X`, `Holiday`, or `AgiBot` are implementing the exact same architecture we are proposing,
  - that our current notes establish equivalence between VLA dual-system architectures and the planned `SOFTSIM` / `GA144` cadence model,
  - that all "brain" architectures in current humanoid robotics are converging on the same design.

Possible paper wording:

> Public embodied-robotics literature now includes several explicit fast/slow control decompositions, particularly in dual-system VLA architectures and layered-timescale control designs. The present work aligns with that broader trend, but differs in emphasis: rather than introducing another monolithic robot foundation model, it explores how fast reactive embodiment and slower supervisory control might be instantiated, simulated, and studied on a distributed asynchronous substrate.

Follow-up needed:

- Add local copies of the key architectural papers to `arxiv/v.2/literature`.
- If a reliable primary source for `Holiday Robotics VLS` becomes available, add it to the literature set.
- Build a minimal `SOFTSIM` example that makes the fast/slow cadence concrete.
