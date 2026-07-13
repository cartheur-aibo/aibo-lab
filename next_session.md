# Next Session

Continue the `R-CODE` behavior-representation work from the current
`ERS-111` generated diagram set.

Primary requirement for next session:

- update each generated diagram viewer so that below the diagram there is a
  short analytical paragraph discussing the workflow exhibited in that
  specific diagram

What that paragraph should do:

- describe the control logic in plain research language
- explain what the behavior is accomplishing bodily or functionally
- point out the important loop, decision, and recovery structure for that
  diagram

What to avoid:

- do not use generic structural filler such as "this diagram shows N states
  and M transitions"
- do not write advice to an audience; write direct analytical commentary on
  the represented behavior

Current relevant artifacts:

- generated diagrams live under `src/R-CODE/generated/`
- the composite everyday umbrella diagram is:
  `src/R-CODE/generated/EmbodiedBehaviors.{mmd,html,svg}`
- the composite note is:
  `src/R-CODE/Document/Embodied_Behaviors.md`
- `MotionRec` is categorized as `Decider`

Recent intent:

- keep naming normalized around `Football` rather than `SoccerDog`
- avoid `Dog` in generated artifact names because AIBO is already a dog
- continue treating the generated diagrams as research representations that
  can be compared analytically across behaviors
