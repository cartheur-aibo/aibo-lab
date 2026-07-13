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
    "AngryDog": "Angry",
    "BanzaiDog": "Banzai",
    "C-Tracking1": "C-Tracking",
    "CrossingDog": "Crossing",
    "FalldownDog": "Falldown",
    "HoldupDog": "Holdup",
    "HyperBabyDog": "HyperBaby",
    "HyperSoccerDog": "HyperFootball",
    "KickDog": "Kick",
    "MoveAIBO": "Move",
    "Maze1": "Maze",
    "OteDog": "Ote",
    "SoccerDog1": "Football",
    "Guru1": "Guru",
    "Quit1": "Quit",
    "QuitDog1": "BallQuit",
    "SadDog": "Sad",
    "SmileDog": "Smile",
    "SosouDog": "Sosou",
    "StepDog1": "Step",
    "WalkDog1": "Walk",
}

BEHAVIOR_CATEGORY_MAP = {
    "MotionRec": "Decider",
}

FAMILY_VARIANT_MAP = {
    "C-Tracking": "C-Tracking",
    "Guru": "Guru",
    "Maze": "Maze",
    "Quit": "Quit",
    "QuitDog": "BallQuit",
    "SoccerDog": "Football",
    "StepDog": "Step",
    "WalkDog": "Walk",
}


def display_name(path: Path) -> str:
    stem = path.stem
    if stem in GENERATED_NAME_MAP:
        return GENERATED_NAME_MAP[stem]

    family, variant = family_variant(stem)
    if family and variant:
        return f"{family}-v{variant}"

    return stem


def family_variant(stem: str) -> tuple[str, int]:
    match = re.match(r"^(.*?)(\d+)$", stem)
    if not match:
        return "", 0

    raw_family = match.group(1)
    variant = int(match.group(2))
    family = FAMILY_VARIANT_MAP.get(raw_family, "")
    return family, variant


def family_name(path: Path) -> str:
    stem = path.stem
    if stem in GENERATED_NAME_MAP:
        mapped = GENERATED_NAME_MAP[stem]
        if mapped.endswith("-v1"):
            return mapped[:-3]
        return mapped

    family, _variant = family_variant(stem)
    return family or display_name(path)


def variant_label(path: Path) -> str:
    _family, variant = family_variant(path.stem)
    if variant > 1:
        return f"v{variant}"
    return "primary"


def behavior_category(path: Path) -> str:
    return BEHAVIOR_CATEGORY_MAP.get(display_name(path), "Behavior")


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


def is_numeric_label(key: str) -> bool:
    return key.isdigit()


def clean_state_comment(comment: str) -> str:
    text = comment.strip()
    if not text:
        return ""
    text = re.sub(r"^[Ii]t is run after,\s*", "", text)
    text = re.sub(r"^[Tt]he\s+", "", text)
    text = text.strip(" .")
    if not text:
        return ""
    text = text[0].upper() + text[1:]
    return text


def infer_action_title(state: State) -> str:
    text_set = [inst.text for inst in state.instructions if inst.text]

    if any(inst.command == "RESUME" for inst in state.instructions):
        return "Resume Listener"
    if any("SET:mode:0" in text for text in text_set):
        return "Set Search Mode"
    if any("SET:mode:1" in text for text in text_set):
        return "Set Pursuit Mode"
    if any("SET:lost:0" in text for text in text_set):
        return "Reset Lost Counter"
    if any("ADD:lost:1" in text for text in text_set):
        return "Increment Lost Counter"
    if any("MOVE:LEGS:KICK:LEFT_KICK" in text for text in text_set):
        return "Left Kick"
    if any("MOVE:LEGS:KICK:RIGHT_KICK" in text for text in text_set):
        return "Right Kick"
    if any("MOVE:HEAD:C-TRACKING" in text for text in text_set):
        return "Start Ball Tracking"
    if any("PLAY:SOUND:joy2_xxt:50" in text for text in text_set):
        return "Happy Hug Response"
    if any("MOVE:HEAD:ABS:30:-90:-90:500" in text for text in text_set):
        return "Check Right"
    if any("MOVE:HEAD:ABS:30:90:90:500" in text for text in text_set):
        return "Check Left"
    if any("MOVE:HEAD:ABS" in text for text in text_set):
        return "Head Scan Position"
    if any("PLAY:APK:move:1:200" in text for text in text_set):
        return "Replay Motion Twice"
    if any("PLAY:APK:move:1:50" in text for text in text_set):
        return "Replay Motion Half-Speed"
    if any("PLAY:APK:move" in text for text in text_set):
        return "Replay Motion"
    if any(inst.command == "LOAD_POSE" for inst in state.instructions):
        return "Load Stored Poses"
    if any(inst.command == "SAVE_POSE" for inst in state.instructions):
        return "Save Current Poses"
    if any("QUIT:AIBO" in text for text in text_set) and any("SET:Power:0" in text for text in text_set):
        return "Power Down"
    if any("STOP:LIGHT" in text for text in text_set):
        return "Clear Light Playback"
    if any("STOP:SOUND" in text for text in text_set):
        return "Clear Sound Playback"
    if any("STOP:AIBO" in text for text in text_set):
        return "Stop And Signal"
    if any("MOVE:LEGS:WALK:SLOW:FORWARD:0" in text for text in text_set):
        return "Slow Forward Walk"
    if any("MOVE:LEGS:WALK:0:FORWARD:0" in text for text in text_set):
        return "Forward Walk"
    if any("PLAY:LEGS:WalkToWS" in text for text in text_set):
        return "Stop Forward Motion"
    if any("MOVE:LEGS:STEP:RIGHT_TURN" in text for text in text_set):
        return "Search Turn"
    if any("MOVE:LEGS:STEP:LEFT_TURN" in text for text in text_set):
        return "Turn Left"
    if any("MOVE:LEGS:STEP:SLOW:LEFTFORWARD" in text for text in text_set):
        return "Advance Left"
    if any("MOVE:LEGS:STEP:SLOW:RIGHTFORWARD" in text for text in text_set):
        return "Advance Right"
    if any("MOVE:LEGS:STEP:SLOW:LEFT" in text for text in text_set):
        return "Shift Left"
    if any("MOVE:LEGS:STEP:SLOW:RIGHT" in text for text in text_set):
        return "Shift Right"
    if any("MOVE:LEGS:STEP:SLOW:BACKWARD" in text for text in text_set):
        return "Back Away"
    if any("MOVE:LEGS:STEP:11:0:10" in text for text in text_set):
        return "Probe Dead End"
    if any("MOVE:LEGS:STEP:12:0:1" in text for text in text_set):
        return "Turn Left Through Gap"
    if any("MOVE:LEGS:STEP:13:0:1" in text for text in text_set):
        return "Turn Right Through Gap"
    if any("MOVE:LEGS:STEP:SLOW:FORWARD" in text for text in text_set):
        return "Advance"
    if any("MOVE:LEGS:STEP:BABY:FORWARD:10" in text for text in text_set):
        return "Baby Sway Forward"
    if any("MOVE:LEGS:STEP:BABY:BACKWARD:10" in text for text in text_set):
        return "Baby Sway Backward"
    if any("SET:dd:0" in text for text in text_set):
        return "Scan Left And Right"
    if any("SET:dd:d" in text for text in text_set):
        return "Record Best Opening"
    if any("PLAY:AIBO:Think" in text for text in text_set):
        return "Idle Thought"
    if any("PLAY:AIBO:Wake" in text for text in text_set):
        return "Help Signal"
    if any("PLAY:AIBO:Biku_sit" in text for text in text_set):
        return "Surprise Response"
    if any("PLAY:AIBO:Cry" in text for text in text_set):
        return "Cry Response"
    if any("PLAY:AIBO:Please" in text for text in text_set):
        return "Seek Attention"
    if any("PLAY:LIGHT:joy" in text for text in text_set):
        return "Happy Response"

    return ""


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

    comment_title = clean_state_comment(state.comment)
    if comment_title:
        return comment_title

    if {"IF", "ONCALL", "SWITCH"} & command_set:
        return "Sense / Decide"

    if "WAIT" in command_set and "MOVE" not in command_set:
        return "Synchronize"

    action_title = infer_action_title(state)
    if action_title:
        return action_title

    return human_title(state)


def human_title(state: State) -> str:
    if state.key == "INIT":
        return "INIT"
    if state.comment:
        if is_numeric_label(state.key):
            return state.comment
        return f"{state.key} {state.comment}"
    if not is_numeric_label(state.key):
        return state.key.replace("_", " ").title()
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
            label = "next labeled block"
            if state.key == "INIT":
                label = "entry"
            elif inferred_state_title(state) == "Repeat Forward Walk":
                label = "continue to monitor"
            transitions.append((state.key, next_state, label))

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

    consumed: set[tuple[str, str, str]] = set()
    by_source: dict[str, list[tuple[str, str, str]]] = {}
    for transition in transitions:
        by_source.setdefault(transition[0], []).append(transition)

    for state in states:
        outgoing = by_source.get(state.key, [])
        fallen = next((t for t in outgoing if t[2] == "fallen"), None)
        upright = next((t for t in outgoing if t[2] == "upright" and t[1] == state.key), None)
        if fallen and upright:
            state_id = state_node_id(state.key)
            decision_id = f"{state_id}_CHECK"
            lines.append(f'    {decision_id}{{"Fall?"}}')
            lines.append(f"    {state_id} --> {decision_id}")
            lines.append(f'    {decision_id} -->|fallen| {state_node_id(fallen[1])}')
            lines.append(f'    {decision_id} -->|upright| {state_id}')
            consumed.add(fallen)
            consumed.add(upright)

    for src, dst, label in transitions:
        if (src, dst, label) in consumed:
            continue
        src_id = state_node_id(src)
        dst_id = state_node_id(dst)
        edge_label = label.replace('"', "'")
        lines.append(f'    {src_id} -->|{edge_label}| {dst_id}')
    lines.append("")
    return "\n".join(lines)


def render_sample_mermaid(path: Path, states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    name = display_name(path)

    if name == "C-Tracking":
        return "\n".join(
            [
                "flowchart TD",
                '    A["Boot"]',
                '    B["Assume Safe Pose"]',
                '    C["Set Target Color"]',
                '    D["Start Head Tracking"]',
                "    A -->|entry| B",
                "    B -->|configure| C",
                "    C -->|activate| D",
                "",
            ]
        )

    if name == "Maze":
        return "\n".join(
            [
                "flowchart TD",
                '    A["Boot"]',
                '    B["Stand And Start Advance"]',
                '    C{"Forward Blocked?"}',
                '    D["Stop Forward Motion"]',
                '    E["Scan Left And Right"]',
                '    F{"Best Direction?"}',
                '    G["Turn Right"]',
                '    H["Turn Left"]',
                '    I["Signal And Retry"]',
                "    A -->|entry| B",
                "    B -->|enter sensing loop| C",
                '    C -->|clear| C',
                "    C -->|blocked| D",
                "    D -->|begin scan| E",
                "    E -->|compare openings| F",
                "    F -->|right side clearer| G",
                "    F -->|left side clearer| H",
                "    F -->|still blocked nearby| I",
                "    G -->|resume advance| B",
                "    H -->|resume advance| B",
                "    I -->|retry scan| E",
                "",
            ]
        )

    if name == "Football":
        return "\n".join(
            [
                "flowchart TD",
                '    A["Boot / Initialize Mode Variables"]',
                '    B["Main Monitor Loop"]',
                '    C{"Fall?"}',
                '    R["Recover"]',
                '    D{"Mode"}',
                '    E["Search Block"]',
                '    F["Scan Head Positions"]',
                '    G{"Ball Visible?"}',
                '    H["Pursuit Block"]',
                '    I{"Tracking Lost?"}',
                '    J{"Kick Range?"}',
                '    K["Kick"]',
                '    L["Approach Ball"]',
                '    M["Wait / Return To Main Loop"]',
                "    A -->|entry| B",
                "    B --> C",
                "    C -->|fallen| R",
                "    C -->|upright| D",
                "    R -->|resume main loop| B",
                "    D -->|search| E",
                "    D -->|pursuit| H",
                "    E -->|rotate and scan| F",
                "    F --> G",
                "    G -->|no| M",
                "    G -->|yes| H",
                "    H --> I",
                "    I -->|yes| E",
                "    I -->|no| J",
                "    J -->|yes| K",
                "    J -->|no| L",
                "    K --> M",
                "    L --> M",
                "    M -->|repeat| B",
                "",
            ]
        )

    return render_mermaid(states, transitions)


def render_markdown(path: Path, states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    counts = command_counts(states)
    sensors = extract_sensors(states)
    mermaid = render_sample_mermaid(path, states, transitions)
    category = behavior_category(path)
    family = family_name(path)
    variant = variant_label(path)

    lines: list[str] = []
    lines.append(f"# R-Code Behavior Extract: `{path.name}`")
    lines.append("")
    lines.append("## Summary")
    lines.append("")
    lines.append(f"- category: `{category}`")
    lines.append(f"- family: `{family}`")
    lines.append(f"- variant: `{variant}`")
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


def render_html_analysis(path: Path, states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    name = display_name(path)
    titles = [inferred_state_title(state) for state in states]
    title_set = set(titles)
    blocks = {block for state in states for block in state.blocks}

    if name == "C-Tracking":
        return (
            "This workflow is closer to capability activation than to a full autonomous behavior. "
            "It powers the robot into a usable pose, configures the target color, and then hands control "
            "to head tracking as a sustained sensorimotor routine rather than a multi-branch behavior loop."
        )

    if name == "Move":
        return (
            "This workflow is a compact locomotion loop organized around bodily safety. "
            "The robot repeats forward walking as its ordinary action, interrupts that action to monitor fall state, "
            "and routes control through a recovery routine whenever posture is lost before returning to ordinary movement."
        )

    if name == "Maze":
        return (
            "This workflow expresses obstacle avoidance as a repeated perceive-compare-reorient cycle. "
            "The robot advances while space remains clear, pauses when forward motion is blocked, scans for a better opening, "
            "and then turns back into locomotion so that movement remains coupled to ongoing spatial checking."
        )

    if name == "Football":
        return (
            "This workflow is a mode-based embodied state machine rather than a single motion script. "
            "The robot alternates between search and pursuit, uses vision and head position to decide when to approach or kick, "
            "and keeps the whole cycle grounded by returning through monitoring and fall recovery when tracking is lost or posture fails."
        )

    if name == "MotionRec":
        return (
            "This workflow behaves like a small decider that waits for symbolic input and dispatches stored handlers. "
            "Instead of roaming on its own, it listens for command events, routes control into record, playback, load, or save paths, "
            "and then resumes its listener state as a control hub for later action."
        )

    if "Recover" in title_set and "Sense Fall State" in title_set:
        return (
            "This workflow centers on maintaining ordinary action under bodily risk. "
            "The robot cycles through its main movement or monitoring routine, checks whether posture has broken down, "
            "and uses recovery as an interrupt path that restores the behavior to a usable ongoing loop."
        )

    if "Scan Left And Right" in title_set or "Record Best Opening" in title_set:
        return (
            "This workflow uses local sensing to choose how movement should continue. "
            "Rather than executing a fixed motion sequence, it pauses to compare nearby openings, selects a direction, "
            "and feeds that decision back into continued locomotion."
        )

    if "Left Kick" in title_set or "Right Kick" in title_set:
        return (
            "This workflow organizes action around target acquisition and directional commitment. "
            "The robot evaluates where a target is relative to its body, commits to a side-specific motor response, "
            "and then returns to monitoring so perception can govern the next action cycle."
        )

    if "Happy Response" in title_set or "Seek Attention" in title_set or "Happy Hug Response" in title_set:
        return (
            "This workflow treats contact or nearby social conditions as triggers for embodied response. "
            "The robot monitors for touch, proximity, or simple interaction cues, performs an expressive motor or light response, "
            "and then settles back into its ordinary loop so the interaction remains event-driven rather than continuous."
        )

    if "Start Ball Tracking" in title_set:
        return (
            "This workflow ties sensing directly to a pursuit-oriented action cycle. "
            "The robot uses target visibility and body orientation to move between tracking, approach, and short action bursts, "
            "keeping perception in control of when the behavior advances or resets."
        )

    if "Loop/Transition" in blocks and "Sense/Decide" in blocks and "Act" in blocks:
        return (
            "This workflow is best read as a sense-decide-act loop. "
            "The robot repeatedly samples a small part of its world or body state, branches into an action block based on that reading, "
            "and then returns to the loop so behavior remains continuously coupled to fresh conditions."
        )

    if "Act" in blocks and "Synchronize" in blocks:
        return (
            "This workflow is organized as an action chunk followed by a synchronization pause. "
            "The robot performs a bounded motor or expressive sequence, waits for that sequence to settle, "
            "and only then returns control to the next round of behavior."
        )

    return (
        "This workflow presents a small embodied control routine rather than a linear script. "
        "The behavior moves between setup, conditional checks, and action blocks, using repeated transitions to keep the robot's "
        "next movement or response tied to current body state and immediate context."
    )


def render_html(path: Path, mermaid: str, states: list[State], transitions: list[tuple[str, str, str]]) -> str:
    title = html.escape(f"R-Code Behavior Diagram: {display_name(path)}")
    mermaid_html = html.escape(mermaid)
    category = html.escape(behavior_category(path))
    family = html.escape(family_name(path))
    variant = html.escape(variant_label(path))
    summary = html.escape(render_html_summary(states, transitions))
    analysis = html.escape(render_html_analysis(path, states, transitions))
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
    <p><strong>Category:</strong> {category}</p>
    <p><strong>Family:</strong> {family} | <strong>Variant:</strong> {variant}</p>
    <p>{summary}</p>
    <div class="panel">
      <pre class="mermaid">{mermaid_html}</pre>
    </div>
    <p>{analysis}</p>
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
    mermaid = render_sample_mermaid(args.script, states, transitions)

    if args.write_sidecars:
        mmd_path, html_path = write_sidecars(args.script, markdown, mermaid, states, transitions)
        print(f"wrote {mmd_path}")
        print(f"wrote {html_path}")
        return 0

    sys.stdout.write(markdown)
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
