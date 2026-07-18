# Docs

This folder holds the local reference material that supports the lab work in
`aibo-lab`: Sony manuals, extracted text slices, CPU-family background, and
supporting works.

It is a document store for the research environment, not the primary home for
sample code or canonical source trees.

## Layout

- `docs/aperios/`
  Local Sony OPEN-R / Aperios manuals in PDF form, including the Programmer's
  Guide, installation material, model references, and level-2 references.

- `docs/aperios-scrapes/`
  Plain-text excerpts and working notes extracted from the Sony manuals for
  faster searching, quoting, and cross-referencing during development.

- `docs/mips/`
  CPU-family background material used to support low-level runtime and binary
  interpretation work. Right now this contains the MIPS R4000 preliminary user
  manual that helps frame ERS-7 architectural assumptions.

- `docs/works/`
  Supporting academic and technical works that inform the OPEN-R, AIBO, and
  simulator research threads.

## Boundary

- Canonical OPEN-R samples and SDK-oriented source material live in
  <https://github.com/cartheur-aibo/open-r-sdk>.
- Canonical ERS-110/111 R-CODE source material lives in
  <https://github.com/cartheur-aibo/r-code>.
- Publication-shaped outputs belong in the sibling monograph repos under
  `../monographs/`.

## Use

Reach for this folder when you need source manuals, extracted reference text,
or supporting works while working on the lab, simulator, or bench procedures.
