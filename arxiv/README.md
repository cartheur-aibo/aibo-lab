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

## Additional references

https://ziegler.substack.com/p/ep108-1x-equips-neo-with-human-level?publication_id=1071177&post_id=207037341&isFreemail=true&r=20hk3e&triedRedirect=true&utm_source=substack&utm_medium=email