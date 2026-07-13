#!/usr/bin/env python3
"""Extract a higher-level behavior view from a Sony R-Code sample."""

from __future__ import annotations

import argparse
import html
import re
import sys
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path


LABEL_RE = re.compile(r"^:(\S+)(?:\s*//\s*(.*))?$")
COMMAND_RE = re.compile(r"^([A-Z_]+)(?::(.*))?$")
GENERATED_NAME_MAP = {
    "C-Tracking1": "C-Tracking",
    "MoveAIBO": "Move",
    "Maze1": "Maze",
    "SoccerDog1": "Football",
}


def display_name(path: Path) -> str:
    return GENERATED_NAME_MAP.get(path.stem, path.stem)


@dataclass
class Instruction:
    raw: str
    text: str
    line_no: int
    comment: str = ""
    command: str = ""
    args: list[str] = field(default_factory=list)


@dataclass
class State:
    key: str
    title: str
    comment: str = ""
    instructions: list[Instruction] = field(default_factory=list)
    blocks: list[str] = field(default_factory=list)


def split_comment(line: str) -> tuple[str, str]:
    if "//" not in line:
        return line.rstrip(), ""
    code, comment = line.split("//", 1)
    return code.rstrip(), comment.strip()


def parse_instruction(line: str, line_no: int) -> Instruction | None:
    stripped = line.strip()
    if not stripped:
        return None

    code, comment = split_comment(stripped)
    if not code:
        return Instruction(raw=line.rstrip("\n"), text="", line_no=line_no, comment=comment)

    match = COMMAND_RE.match(code)
    if not match:
        return Instruction(raw=line.rstrip("\n"), text=code, line_no=line_no, comment=comment)

    command = match.group(1)
    arg_text = match.group(2) or ""
    args = arg_text.split(":") if arg_text else []
    return Instruction(
        raw=line.rstrip("\n"),
        text=code,
        line_no=line_no,
        comment=comment,
        command=command,
        args=args,
    )


def load_states(path: Path) -> list[State]:
    states: list[State] = [State(key="INIT", title="INIT", comment="preamble before first label")]
    current = states[0]

    for line_no, line in enumerate(path.read_text(encoding="utf-8", errors="replace").splitlines(), start=1):
        stripped = line.strip()
        if not stripped:
            continue
        if stripped.startswith("//"):
            continue

        label_match = LABEL_RE.match(stripped)
        if label_match:
            label = label_match.group(1)
            comment = (label_match.group(2) or "").strip()
            current = State(key=label, title=f"State {label}", comment=comment)
            states.append(current)
            continue

        instruction = parse_instruction(line, line_no)
        if instruction is not None:
            current.instructions.append(instruction)

    return [state for state in states if state.instructions or state.key != "INIT"]


def classify_blocks(state: State) -> list[str]:
    commands = {inst.command for inst in state.instructions if inst.command}
    blocks: list[str] = []

    if any(inst.text.startswith("SET:Power:1") for inst in state.instructions):
        blocks.append("Boot")
    if "SET" in commands and not any(inst.text.startswith("SET:Power:1") for inst in state.instructions):
        blocks.append("Initialize State")
    if "POSE" in commands:
        blocks.append("Assume Safe Pose")
    if {"IF", "ONCALL", "SWITCH", "CSET"} & commands:
        blocks.append("Sense/Decide")
    if {"MOVE", "PLAY", "STOP", "QUIT", "SAVE", "SEND", "LOAD"} & commands:
        blocks.append("Act")
    if "WAIT" in commands:
        blocks.append("Synchronize")
    if any(
        inst.command == "QUIT" or "ReactiveGU" in inst.text or inst.command == "RESUME"
        for inst in state.instructions
    ):
        blocks.append("Recover")
    if any(inst.command in {"GO", "ONCALL", "RESUME"} for inst in state.instructions):
        blocks.append("Loop/Transition")

    return blocks or ["Action"]


def summarize_state(state: State) -> None:
    state.blocks = classify_blocks(state)


def fmt_condition(inst: Instruction) -> str:
    if not inst.args:
        return inst.command
    if inst.command == "IF" and len(inst.args) >= 4:
        op, left, right = inst.args[0], inst.args[1], inst.args[2]
        return f"{left} {op} {right}"
    if inst.command == "ONCALL" and len(inst.args) >= 4:
        op, left, right = inst.args[0], inst.args[1], inst.args[2]
        return f"on {left} {op} {right}"
    return ":".join([inst.command, *inst.args[:3]])


def state_node_id(key: str) -> str:
    safe = re.sub(r"[^A-Za-z0-9_]", "_", key)
    return f"S_{safe}"


def inferred_state_title(state: State) -> str:
    commands = [inst.command for inst in state.instructions if inst.command]
    command_set = set(commands)
    text_set = {inst.text for inst in state.instructions if inst.text}

    if state.key == "INIT":
        if any(inst.text.startswith("SET:Power:1") for inst in state.instructions) and "POSE" in command_set:
            return "Boot / Safe Pose"
        return "Boot"

    if any("ReactiveGU" in text for text in text_set):
        return "Recover"

    if "MOVE" in command_set and "WAIT" in command_set and "Gsensor_status" not in " ".join(text_set):
        if any("MOVE:LEGS:WALK" in text for text in text_set):
            return "Repeat Forward Walk"
        return "Action Loop"

    if any("Gsensor_status" in text for text in text_set):
        return "Sense Fall State"

    if {"IF", "ONCALL", "SWITCH"} & command_set:
        return "Sense / Decide"

    if "WAIT" in command_set and "MOVE" not in command_set:
        return "Synchronize"

    return human_title(state)


def human_title(state: State) -> str:
    if state.key == "INIT":
        return "INIT"
    if state.comment:
        return f"{state.key} {state.comment}"
    return state.title


def extract_transitions(states: list[State]) -> list[tuple[str, str, str]]:
    transitions: list[tuple[str, str, str]] = []

    for index, state in enumerate(states):
        next_state = states[index + 1].key if index + 1 < len(states) else ""
        seen_explicit = False
        state_text = " ".join(inst.text for inst in state.instructions if inst.text)
        monitors_fall = "Gsensor_status" in state_text
        recovery_state = any("ReactiveGU" in inst.text for inst in state.instructions if inst.text)

        for inst in state.instructions:
            if inst.command == "GO" and inst.args:
                label = "go"
                if monitors_fall and inst.args[0] == state.key:
                    label = "upright"
                elif recovery_state:
                    label = "resume monitor"
                transitions.append((state.key, inst.args[0], label))
                seen_explicit = True
                continue

            if inst.command == "IF":
                if len(inst.args) >= 4:
                    condition = fmt_condition(inst)
                    label = f"if {condition}"
                    if monitors_fall and inst.args[:3] == ["=", "stat", "1"]:
                        label = "fallen"
                    transitions.append((state.key, inst.args[3], label))
                    seen_explicit = True
                if len(inst.args) >= 5:
                    condition = fmt_condition(inst)
                    transitions.append((state.key, inst.args[4], f"else {condition}"))
                    seen_explicit = True
                continue

            if inst.command == "ONCALL" and len(inst.args) >= 4:
                target = inst.args[3]
                transitions.append((state.key, target, fmt_condition(inst)))
                seen_explicit = True
                continue

            if inst.command == "RESUME":
                transitions.append((state.key, state.key, "resume"))
                seen_explicit = True

        if not seen_explicit and next_state:
            transitions.append((state.key, next_state, "fallthrough"))

    return dedupe_transitions(transitions)


def dedupe_transitions(transitions: list[tuple[str, str, str]]) -> list[tuple[str, str, str]]:
    seen: set[tuple[str, str, str]] = set()
    ordered: list[tuple[str, str, str]] = []
    for transition in transitions:
        if transition in seen:
            continue
        seen.add(transition)
        ordered.append(transition)
    return ordered


def command_counts(states: list[State]) -> Counter[str]:
    counts: Counter[str] = Counter()
    for state in states:
        for inst in state.instructions:
            if inst.command:
                counts[inst.command] += 1
    return counts


def extract_sensors(states: list[State]) -> list[str]:
    sensors: set[str] = set()
    interesting_prefixes = (
        "Gsensor_",
        "Touch_",
        "Distance",
        "Cdt_",
        "Head_",
        "Psd_",
        "Sound_",
        "Light_",
        "Wait",
    )
    for state in states:
        for inst in state.instructions:
            for token in inst.args:
                if token.startswith(interesting_prefixes):
                    sensors.add(token)
    return sorted(sensors)


def render_mermaid(states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    lines: list[str] = []
    lines.append("flowchart TD")
    for state in states:
        node_id = state_node_id(state.key)
        label = inferred_state_title(state).replace('"', "'")
        lines.append(f'    {node_id}["{label}"]')
    for src, dst, label in transitions:
        src_id = state_node_id(src)
        dst_id = state_node_id(dst)
        edge_label = label.replace('"', "'")
        lines.append(f'    {src_id} -->|{edge_label}| {dst_id}')
    lines.append("")
    return "\n".join(lines)


def render_markdown(path: Path, states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    counts = command_counts(states)
    sensors = extract_sensors(states)
    mermaid = render_mermaid(states, transitions)

    lines: list[str] = []
    lines.append(f"# R-Code Behavior Extract: `{path.name}`")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- source: `{path}`")
    lines.append(f"- states: `{len(states)}`")
    lines.append(f"- transitions: `{len(transitions)}`")
    lines.append(f"- commands: `{', '.join(f'{name}={count}' for name, count in counts.most_common(10))}`")
    if sensors:
        lines.append(f"- sensed variables: `{', '.join(sensors[:12])}`")
    lines.append("")
    lines.append("## State Blocks")
    lines.append("")

    for state in states:
        block_text = ", ".join(state.blocks)
        title = inferred_state_title(state)
        lines.append(f"- `{title}`: {block_text}")
        for inst in state.instructions[:5]:
            snippet = inst.text if inst.text else f"comment: {inst.comment}"
            lines.append(f"  lines {inst.line_no}: `{snippet}`")
        if len(state.instructions) > 5:
            lines.append(f"  ... `{len(state.instructions) - 5}` more instructions")
    lines.append("")
    lines.append("## Transitions")
    lines.append("")
    for src, dst, label in transitions:
        lines.append(f"- `{src}` -> `{dst}`: {label}")
    lines.append("")
    lines.append("## Mermaid")
    lines.append("")
    lines.append("```mermaid")
    lines.append(mermaid.rstrip())
    lines.append("```")
    lines.append("")
    return "\n".join(lines)


def render_html_summary(states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    titles = [inferred_state_title(state) for state in states]
    block_order = [
        "Boot",
        "Initialize State",
        "Assume Safe Pose",
        "Sense/Decide",
        "Act",
        "Synchronize",
        "Recover",
        "Loop/Transition",
    ]
    blocks_present: list[str] = []
    for block in block_order:
        if any(block in state.blocks for state in states):
            blocks_present.append(block)

    parts = [
        f"This diagram shows {len(states)} extracted state{'s' if len(states) != 1 else ''}",
        f"and {len(transitions)} transition{'s' if len(transitions) != 1 else ''}.",
    ]

    if "Repeat Forward Walk" in titles and "Sense Fall State" in titles:
        parts.append("It captures a repeated forward-walk loop with fall monitoring.")

    if blocks_present:
        shown = ", ".join(blocks_present[:5])
        parts.append(f"It includes {shown} block{'s' if len(blocks_present[:5]) != 1 else ''}.")

    if any("Recover" in state.blocks for state in states):
        parts.append("A recovery path is present in the extracted control flow.")

    return " ".join(parts)


def render_html(path: Path, mermaid: str, states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    title = html.escape(f"R-Code Behavior Diagram: {display_name(path)}")
    mermaid_html = html.escape(mermaid)
    summary = html.escape(render_html_summary(states, transitions))
    return f"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>{title}</title>
  <style>
    :root {{
      color-scheme: dark;
      --bg: #101418;
      --fg: #e6edf3;
      --muted: #94a3b8;
      --panel: #161b22;
      --border: #30363d;
    }}
    body {{
      margin: 0;
      padding: 24px;
      font-family: system-ui, sans-serif;
      background: var(--bg);
      color: var(--fg);
    }}
    main {{
      max-width: 1200px;
      margin: 0 auto;
    }}
    .panel {{
      background: var(--panel);
      border: 1px solid var(--border);
      border-radius: 12px;
      padding: 16px;
    }}
    .mermaid {{
      overflow: auto;
    }}
    p {{
      color: var(--muted);
    }}
    code {{
      color: var(--fg);
    }}
  </style>
  <script type="module">
    import mermaid from "https://cdn.jsdelivr.net/npm/mermaid@11/dist/mermaid.esm.min.mjs";
    mermaid.initialize({{ startOnLoad: true, theme: "dark" }});
  </script>
</head>
<body>
  <main>
    <h1>{title}</h1>
    <p>{summary}</p>
    <div class="panel">
      <pre class="mermaid">{mermaid_html}</pre>
    </div>
  </main>
</body>
</html>
"""


def sidecar_paths(path: Path) -> tuple[Path, Path, Path]:
    if "src/R-CODE/sample" in path.as_posix():
        generated_dir = path.parents[1] / "generated"
        generated_dir.mkdir(parents=True, exist_ok=True)
        base_name = display_name(path)
        base = generated_dir / base_name
        return (
            base.with_suffix(".behavior.md"),
            base.with_suffix(".mmd"),
            base.with_suffix(".html"),
        )

    base = path.with_suffix("")
    return (
        base.with_suffix(".behavior.md"),
        base.with_suffix(".mmd"),
        base.with_suffix(".html"),
    )


def write_sidecars(path: Path, markdown: str, mermaid: str, states: list[State], transitions: list[tuple[str, str, str]]) -> tuple[Path, Path]:
    md_sidecar, mmd_sidecar, html_sidecar = sidecar_paths(path)

    md_sidecar.write_text(markdown, encoding="utf-8")
    mmd_sidecar.write_text(mermaid, encoding="utf-8")
    html_sidecar.write_text(render_html(path, mermaid, states, transitions), encoding="utf-8")
    return mmd_sidecar, html_sidecar


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("script", type=Path, help="Path to a Sony R-Code .R script")
    parser.add_argument(
        "--write-sidecars",
        action="store_true",
        help="Write .behavior.md, .mmd, and .html files next to the source script",
    )
    args = parser.parse_args(argv)

    if not args.script.exists():
        print(f"error: script not found: {args.script}", file=sys.stderr)
        return 1

    states = load_states(args.script)
    for state in states:
        summarize_state(state)
    transitions = extract_transitions(states)
    markdown = render_markdown(args.script, states, transitions)
    mermaid = render_mermaid(states, transitions)

    if args.write_sidecars:
        mmd_path, html_path = write_sidecars(args.script, markdown, mermaid, states, transitions)
        print(f"wrote {mmd_path}")
        print(f"wrote {html_path}")
        return 0

    sys.stdout.write(markdown)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
