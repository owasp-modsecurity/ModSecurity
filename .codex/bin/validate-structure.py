#!/usr/bin/env python3
"""Validate this fork's routed context layout without changing files."""

import json
import sys
from pathlib import Path

try:
    import tomllib
except ImportError:
    print("ERROR: Python 3.11 or later is required for TOML validation.", file=sys.stderr)
    sys.exit(2)

ROOT = Path(__file__).resolve().parents[2]
GROUPS = (
    "technical_guides", "primary_policies", "profiles", "components",
    "security_checks", "session_context", "supporting_context", "tools",
)


def load(relative):
    with (ROOT / relative).open("rb") as handle:
        return tomllib.load(handle)


def local_path(relative):
    if not isinstance(relative, str) or not relative or Path(relative).is_absolute():
        raise ValueError(f"Expected a nonempty repository-relative path: {relative!r}")
    path = (ROOT / relative).resolve()
    if not path.is_relative_to(ROOT):
        raise ValueError(f"Path leaves the repository: {relative}")
    return path


def main():
    manifest = load(".codex/context/manifest.toml")
    structure = load(".codex/structure-manifest.toml")
    errors = []
    for name, data in (("context", manifest), ("structure", structure)):
        if data.get("schema_version") != 1:
            errors.append(f"{name}: unsupported schema_version")
    if structure.get("repository") != "Easton97-Jens/ModSecurity":
        errors.append("Structure repository identity is unexpected")
    if structure.get("children") != []:
        errors.append("Active child inheritance is not supported by this schema")
    for relative in structure.get("required_files", []):
        if not local_path(relative).is_file():
            errors.append(f"Missing required file: {relative}")
    for relative in structure.get("required_directories", []):
        if not local_path(relative).is_dir():
            errors.append(f"Missing required directory: {relative}")
    routed = set()
    for group in GROUPS:
        rows = manifest.get(group)
        if not isinstance(rows, dict) or not rows:
            errors.append(f"Missing or empty routing group: {group}")
            continue
        for key, relative in rows.items():
            path = local_path(relative)
            if not path.is_file():
                errors.append(f"Missing route {group}.{key}: {relative}")
            if relative in routed:
                errors.append(f"Duplicate route target: {relative}")
            routed.add(relative)
    for relative in structure.get("inventory_roots", []):
        actual = {
            p.relative_to(ROOT).as_posix()
            for p in local_path(relative).rglob("*.md") if p.is_file()
        }
        for missing in sorted(actual - routed):
            errors.append(f"Markdown file has no manifest route: {missing}")
    agents_path = local_path(manifest["root_instruction"])
    agents = agents_path.read_text("utf-8")
    marker = structure["routing_marker"]
    if marker not in agents:
        errors.append(f"AGENTS.md lacks routing marker: {marker}")
    if len(agents.splitlines()) > 150:
        errors.append("AGENTS.md exceeds the 150-line concise entry-point limit")
    if manifest["index"] != ".codex/context/index.md":
        errors.append("Unexpected context index path")
    lock = json.loads((ROOT / ".codex/context/upstream-lock.json").read_text("utf-8"))
    template = lock.get("structure_template", {})
    if template.get("repository") != structure.get("template_repository"):
        errors.append("Template repository differs between manifests and lock")
    if template.get("commit") != structure.get("template_commit"):
        errors.append("Template revision differs between manifests and lock")
    for error in errors:
        print("ERROR: " + error, file=sys.stderr)
    if errors:
        print(f"RESULT: {len(errors)} structure errors", file=sys.stderr)
        return 1
    print(f"OK: Context structure and {len(routed)} routed files are consistent.")
    print("LIMIT: File routing is checked, not semantic correctness or runtime behavior.")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print("ERROR: " + str(exc), file=sys.stderr)
        sys.exit(2)
