# arXiv Bundle

This folder contains a self-contained submission bundle for arXiv.

Contents:

- `main.tex`: primary LaTeX source for the paper
- `main.pdf`: compiled PDF generated from `main.tex`
- `EmbodiedBehaviors.pdf`
- `Move.pdf`
- `Football.pdf`

Local build:

```bash
cd arxiv
pdflatex -interaction=nonstopmode -halt-on-error main.tex
pdflatex -interaction=nonstopmode -halt-on-error main.tex
```

Suggested upload set:

- `main.tex`
- `EmbodiedBehaviors.pdf`
- `Move.pdf`
- `Football.pdf`

Published at: http://arxiv.org/abs/2607.12115