#!/usr/bin/env python3
"""Export simple Mermaid flowcharts to SVG using Graphviz."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

NODE_RE = re.compile(r'^\s*([A-Za-z0-9_]+)(\[\s*"([^"]*)"\s*\]|\{\s*"([^"]*)"\s*\})\s*$')
EDGE_RE = re.compile(r'^\s*([A-Za-z0-9_]+)\s*-->\|([^|]*)\|\s*([A-Za-z0-9_]+)\s*$')
EDGE_PLAIN_RE = re.compile(r'^\s*([A-Za-z0-9_]+)\s*-->\s*([A-Za-z0-9_]+)\s*$')


def escape_dot(text: str) -> str:
    return text.replace("\\", "\\\\").replace('"', '\\"')


def parse_mermaid(text: str) -> tuple[list[tuple[str, str, str]], list[tuple[str, str, str]]]:
    nodes: list[tuple[str, str, str]] = []
    edges: list[tuple[str, str, str]] = []

    for raw_line in text.splitlines():
        line = raw_line.strip()
        if not line or line.startswith("flowchart "):
            continue

        node_match = NODE_RE.match(line)
        if node_match:
            node_id = node_match.group(1)
            shape = "diamond" if node_match.group(4) is not None else "box"
            label = node_match.group(3) if node_match.group(3) is not None else node_match.group(4)
            nodes.append((node_id, shape, label))
            continue

        edge_match = EDGE_RE.match(line)
        if edge_match:
            edges.append((edge_match.group(1), edge_match.group(3), edge_match.group(2).strip()))
            continue

        edge_plain_match = EDGE_PLAIN_RE.match(line)
        if edge_plain_match:
            edges.append((edge_plain_match.group(1), edge_plain_match.group(2), ""))

    return nodes, edges


def to_dot(nodes: list[tuple[str, str, str]], edges: list[tuple[str, str, str]]) -> str:
    lines = [
        "digraph G {",
        '  rankdir=TB;',
        '  graph [bgcolor="#101418", pad="0.25", nodesep="0.5", ranksep="0.8"];',
        '  node [fontname="Helvetica", fontsize="16", fontcolor="#e6edf3", color="#9aa4b2", penwidth="1.2", style="rounded"];',
        '  edge [fontname="Helvetica", fontsize="13", fontcolor="#94a3b8", color="#9aa4b2", arrowsize="0.8"];',
    ]

    for node_id, shape, label in nodes:
        dot_shape = "diamond" if shape == "diamond" else "box"
        lines.append(
            f'  {node_id} [shape={dot_shape}, label="{escape_dot(label)}", fillcolor="#1a2028", style="rounded,filled"];'
        )

    for src, dst, label in edges:
        if label:
            lines.append(f'  {src} -> {dst} [label="{escape_dot(label)}"];')
        else:
            lines.append(f"  {src} -> {dst};")

    lines.append("}")
    lines.append("")
    return "\n".join(lines)


def export_svg(path: Path) -> Path:
    nodes, edges = parse_mermaid(path.read_text(encoding="utf-8"))
    if not nodes:
        raise ValueError(f"no supported nodes found in {path}")

    dot_text = to_dot(nodes, edges)
    svg_path = path.with_suffix(".svg")
    proc = subprocess.run(
        ["dot", "-Tsvg", "-o", str(svg_path)],
        input=dot_text.encode("utf-8"),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    if proc.returncode != 0:
        raise RuntimeError(proc.stderr.decode("utf-8", errors="replace"))
    return svg_path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("files", nargs="+", type=Path, help="Mermaid .mmd files to export")
    args = parser.parse_args(argv)

    for path in args.files:
        svg_path = export_svg(path)
        print(f"wrote {svg_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
