#!/usr/bin/env python3
"""Check the added documentation against its pinned upstream Git objects.

This project-local helper reads files and invokes read-only Git commands. It
does not fetch, build, install, modify source, or establish semantic correctness.
Python standard library only; Git 2.45 or later must be available on PATH
for the --no-lazy-fetch option.
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path
from urllib.parse import unquote, urlsplit


ROOT = Path(__file__).resolve().parents[2]
SOURCE_URL = re.compile(
    r"https://github\.com/owasp-modsecurity/ModSecurity/"
    r"(?:blob|tree)/[^\s<>)`\"]+"
)
INLINE_LINK = re.compile(r"(?<!!)\[[^\]\n]+\]\(([^)\n]+)\)")
REFERENCE_DEF = re.compile(r"^\[([^\]]+)\]:\s+(\S+)", re.MULTILINE)
REFERENCE_USE = re.compile(r"\[[^\]\n]+\]\[([^\]\n]+)\]")
LINE_FRAGMENT = re.compile(r"L([1-9][0-9]*)(?:-L([1-9][0-9]*))?\Z")
HEX40 = re.compile(r"[0-9a-f]{40}\Z")


def git(*args):
    """Return stdout bytes for Git arguments run in ROOT, or raise on failure.

    Git is resolved through PATH and invoked without a shell. Lazy fetching,
    optional lock writes, and the filesystem monitor are disabled.
    """
    completed = subprocess.run(
        ["git", "--no-lazy-fetch", "--no-optional-locks", "-c", "core.fsmonitor=false", *args],
        cwd=ROOT,
        capture_output=True,
        check=False,
    )
    if completed.returncode:
        error = completed.stderr.decode("utf-8", errors="replace").strip()
        raise RuntimeError("git " + " ".join(args) + ": " + error)
    return completed.stdout


def context_path(path):
    """Return whether a repository-relative path belongs to the context addition."""
    return path in {"AGENTS.md", "CODEX_START.md"} or path.startswith(".codex/")


def markdown_prose(content):
    """Return text outside fenced examples and whether a fence remains unclosed."""
    lines = []
    fence = None
    for line in content.splitlines(keepends=True):
        stripped = line.lstrip()
        match = re.match(r"(`{3,}|~{3,})", stripped)
        if fence is not None:
            if (
                match and match.group(1)[0] == fence[0]
                and len(match.group(1)) >= len(fence)
                and not stripped[match.end():].strip()
            ):
                fence = None
            continue
        if match:
            fence = match.group(1)
            continue
        lines.append(line)
    return "".join(lines), fence is not None


def main():
    """Check the pinned source and document links, returning 0 or 1 for the result.

    Report product-source drift, treating it as an error with --strict-source.
    Input or Git failures propagate to the command-line handler for exit 2.
    """
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--strict-source",
        action="store_true",
        help="Fail when the worktree differs from upstream outside context files.",
    )
    args = parser.parse_args()
    errors = []
    lock = json.loads((ROOT / ".codex/context/upstream-lock.json").read_text("utf-8"))
    if lock.get("schema_version") != 1:
        raise ValueError("Unknown schema_version in upstream-lock.json")
    base = lock["upstream"]["commit"]
    expected_tree = lock["upstream"]["tree"]
    if not HEX40.fullmatch(base) or not HEX40.fullmatch(expected_tree):
        raise ValueError("Commit and tree must be full Git object IDs")
    root_from_git = Path(git("rev-parse", "--show-toplevel").decode().strip()).resolve()
    if root_from_git != ROOT:
        raise ValueError("Documentation is not in the expected Git repository root")
    actual_tree = git("rev-parse", base + "^{tree}").decode().strip()
    if actual_tree != expected_tree:
        errors.append("The baseline commit tree differs from upstream-lock.json")
    git("merge-base", "--is-ancestor", base, "HEAD")
    print("OK: Baseline commit is available and an ancestor of HEAD: " + base)

    entries = {}
    directories = {""}
    index_lines = ["mode\ttype\tobject\tpath\n"]
    for item in git("ls-tree", "-r", "-z", base).split(b"\0"):
        if not item:
            continue
        metadata, path = item.decode("utf-8").split("\t", 1)
        mode, kind, oid = metadata.split()
        entries[path] = (mode, kind, oid)
        index_lines.append(f"{mode}\t{kind}\t{oid}\t{path}\n")
        directories.update(str(p) for p in Path(path).parents if str(p) != ".")
    actual_index = (ROOT / ".codex/audit/source-index.tsv").read_text("utf-8")
    if actual_index != "".join(index_lines):
        errors.append("source-index.tsv does not match the pinned Git tree")
    if lock["tracked_entry_count"] != len(entries):
        errors.append("tracked_entry_count does not match the Git tree")
    blobs = sum(kind == "blob" for _, kind, _ in entries.values())
    if lock["tracked_blob_count"] != blobs:
        errors.append("tracked_blob_count does not match the Git tree")
    gitlinks = {p: oid for p, (mode, _, oid) in entries.items() if mode == "160000"}
    locked_gitlinks = {item["path"]: item["commit"] for item in lock["submodules"]}
    if gitlinks != locked_gitlinks:
        errors.append("Submodule commits do not match the Git tree")
    print(f"INFO: Source inventory: {len(entries)} entries, {blobs} blobs, {len(gitlinks)} gitlinks")

    docs = [ROOT / "AGENTS.md", ROOT / "CODEX_START.md"]
    docs.extend(sorted((ROOT / ".codex").rglob("*.md")))
    unique_source_urls = set()
    local_count = 0
    content_cache = {}
    historical_trees = {}
    for document in docs:
        name = str(document.relative_to(ROOT))
        historical = (
            document == ROOT / ".codex/context/decisions.md"
            or ROOT / ".codex/audit/sessions" in document.parents
        )
        content = document.read_text("utf-8")
        prose, unclosed_fence = markdown_prose(content)
        if unclosed_fence:
            errors.append(f"{name}: unclosed Markdown code fence")
        definitions = dict(REFERENCE_DEF.findall(prose))
        normalized_definitions = {key.casefold() for key in definitions}
        for ref in REFERENCE_USE.findall(prose):
            if ref.casefold() not in normalized_definitions:
                errors.append(f"{name}: Markdown reference [{ref}] is not defined")
        links = INLINE_LINK.findall(prose) + list(definitions.values())
        for target in links:
            target = target.strip("<>")
            parsed = urlsplit(target)
            if parsed.scheme or parsed.netloc or not parsed.path:
                continue
            local_count += 1
            resolved = (document.parent / unquote(parsed.path)).resolve()
            try:
                resolved.relative_to(ROOT)
            except ValueError:
                errors.append(f"{name}: local link leaves the repository: {target}")
                continue
            if not resolved.exists():
                errors.append(f"{name}: local link target is missing: {target}")

        for url in SOURCE_URL.findall(content):
            parsed = urlsplit(url)
            parts = parsed.path.strip("/").split("/", 4)
            if len(parts) < 4:
                errors.append(f"{name}: incomplete source link: {url}")
                continue
            kind, commit = parts[2:4]
            source_path = unquote(parts[4]) if len(parts) == 5 else ""
            if not HEX40.fullmatch(commit):
                errors.append(f"{name}: source link does not use a full commit ID: {url}")
                continue
            if commit != base and not historical:
                errors.append(f"{name}: source link does not use the pinned commit: {url}")
                continue
            if url in unique_source_urls:
                continue
            unique_source_urls.add(url)
            source_entries, source_directories, source_gitlinks = entries, directories, gitlinks
            if commit != base:
                if commit not in historical_trees:
                    history_entries = {}
                    history_directories = {""}
                    for item in git("ls-tree", "-r", "-z", commit).split(b"\0"):
                        if not item:
                            continue
                        metadata, path = item.decode("utf-8").split("\t", 1)
                        mode, object_type, oid = metadata.split()
                        history_entries[path] = (mode, object_type, oid)
                        history_directories.update(
                            str(p) for p in Path(path).parents if str(p) != "."
                        )
                    history_gitlinks = {
                        path: oid for path, (mode, _, oid) in history_entries.items()
                        if mode == "160000"
                    }
                    historical_trees[commit] = (
                        history_entries, history_directories, history_gitlinks
                    )
                source_entries, source_directories, source_gitlinks = historical_trees[commit]
            entry = source_entries.get(source_path)
            if kind == "tree":
                if source_path not in source_directories and source_path not in source_gitlinks:
                    errors.append(f"{name}: source directory is missing at {commit}: {source_path}")
                if parsed.fragment:
                    errors.append(f"{name}: directory link has an unchecked fragment: {url}")
                continue
            if entry is None or entry[1] != "blob":
                errors.append(f"{name}: source file is missing at {commit}: {source_path}")
                continue
            if parsed.fragment:
                match = LINE_FRAGMENT.fullmatch(parsed.fragment)
                if not match:
                    errors.append(f"{name}: source fragment is not a line range: {url}")
                    continue
                start = int(match.group(1))
                end = int(match.group(2) or match.group(1))
                if entry[2] not in content_cache:
                    content_cache[entry[2]] = git("cat-file", "blob", entry[2])
                line_count = len(content_cache[entry[2]].splitlines())
                if start > end or end > line_count:
                    errors.append(
                        f"{name}: range {start}..{end} exceeds "
                        f"{source_path} ({line_count} lines)"
                    )

    print(f"INFO: {len(docs)} Markdown files, {local_count} local link targets")
    print(f"INFO: {len(unique_source_urls)} unique commit-pinned ModSecurity source links")
    changed = git("diff", "--no-ext-diff", "--no-textconv", "--name-only", "-z", base, "--").decode().split("\0")
    untracked = git("ls-files", "--others", "--exclude-standard", "-z").decode().split("\0")
    deviations = sorted({path for path in changed + untracked if path and not context_path(path)})
    if deviations:
        print("DIFFERENCES: Files outside the context additions:")
        for path in deviations:
            print("  " + path)
        print("The technical guides describe the baseline; recheck affected claims.")
        if args.strict_source:
            errors.append("--strict-source: source differences found")
    else:
        print("OK: No source differences outside AGENTS.md, CODEX_START.md and .codex/")

    for error in errors:
        print("ERROR: " + error, file=sys.stderr)
    if errors:
        print(f"RESULT: {len(errors)} consistency errors", file=sys.stderr)
        return 1
    print("RESULT: Documentation consistency check passed.")
    print("LIMIT: No semantic guarantee, external website check, or C++ build/test execution.")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, RuntimeError, ValueError, KeyError, TypeError) as exc:
        print("ERROR: " + str(exc), file=sys.stderr)
        print("Check the checkout and upstream-lock.json; no objects are fetched automatically.", file=sys.stderr)
        sys.exit(2)
