#!/usr/bin/env python3
"""Maintain reviewed runner data without allowing the bot to edit workflow code."""

import argparse
from copy import deepcopy
from datetime import date, datetime, timezone
import json
import os
from pathlib import Path
import re
import sys
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen

from runner_catalog import announcement_review_notes, parse_announcements, parse_catalog

CATALOG_URL = "https://raw.githubusercontent.com/actions/runner-images/main/README.md"
ISSUES_URL = "https://api.github.com/repos/actions/runner-images/issues"
LABEL = re.compile(r"(?:ubuntu|macos|windows|xcode)-[a-z0-9]+(?:[.-][a-z0-9]+)*\Z")
SOURCE = re.compile(r"https://github\.com/actions/runner-images/issues/[0-9]+\Z")
GROUPS = {"linux": "ubuntu", "macos": "macos", "windows": "windows"}
REPOSITORY_ROOT = Path(__file__).resolve().parents[2]


def _repository_path(path):
    """Confine data files to the checkout, including existing symlink parents."""
    resolved = Path(path).resolve()
    if not resolved.is_relative_to(REPOSITORY_ROOT):
        raise ValueError("Runner data paths must stay inside the repository")
    return resolved


def _command_file(path, variable):
    """Permit an external command file only when GitHub supplied its exact path."""
    resolved = Path(path).resolve()
    expected = os.environ.get(variable)
    if expected and resolved == Path(expected).resolve():
        return resolved
    return _repository_path(resolved)


def _read_text(path):
    """Read a UTF-8 data file only after checking checkout containment."""
    return _repository_path(path).read_text(encoding="utf-8")


def _append_command_file(path, content, variable):
    """Append workflow data to a checked repository or GitHub command file."""
    with _command_file(path, variable).open("a", encoding="utf-8", newline="\n") as stream:
        stream.write(content)


def _label(value):
    """Accept only literal runner labels suitable for matrix data."""
    if not isinstance(value, str) or not LABEL.fullmatch(value):
        raise ValueError("Invalid runner label")
    return value


def validate_manifest(data):
    """Reject invalid policy before emitting a matrix or modifying files."""
    if not isinstance(data, dict) or data.get("version") != 1:
        raise ValueError("Expected runner manifest version 1")
    if not isinstance(data.get("coverage"), dict) or set(data["coverage"]) != set(GROUPS):
        raise ValueError("Expected linux, macos and windows coverage")
    for group, family in GROUPS.items():
        policy = data["coverage"][group]
        if not isinstance(policy, dict):
            raise ValueError("Invalid runner policy")
        if policy.get("family") != family or policy.get("architecture") not in {"x64", "arm64"}:
            raise ValueError("Invalid runner family or architecture")
        labels = policy.get("labels")
        if not isinstance(labels, list) or not labels or not all(isinstance(label, str) for label in labels) or len(labels) != len(set(labels)):
            raise ValueError("Runner labels must be a nonempty, unique list")
        for label in labels:
            _label(label)
            if not label.startswith(family + "-"):
                raise ValueError("Runner label does not match its family")
        ignored = policy.get("ignored", {})
        if not isinstance(ignored, dict):
            raise ValueError("Ignored runners must include reasons")
        for label, reason in ignored.items():
            _label(label)
            if not isinstance(reason, str) or not reason.strip():
                raise ValueError("Ignored runners need a reason")
        pcre1 = policy.get("pcre1_labels", [])
        if not isinstance(pcre1, list) or not set(pcre1).issubset(labels):
            raise ValueError("PCRE1 labels must be a subset of the configured runners")
    if not isinstance(data.get("pinned", {}), dict) or not isinstance(data.get("lifecycle", {}), dict):
        raise ValueError("Invalid pinned or lifecycle data")
    for label, reason in data.get("pinned", {}).items():
        _label(label)
        if not isinstance(reason, str) or not reason.strip():
            raise ValueError("Pinned runners need a reason")
    authors = data.get("trusted_announcement_authors", [])
    if not isinstance(authors, list) or any(not isinstance(name, str) or not re.fullmatch(r"[A-Za-z0-9-]{1,39}", name) for name in authors):
        raise ValueError("Invalid announcement publisher allowlist")
    notes = data.get("review_notes", [])
    if not isinstance(notes, list) or any(not isinstance(url, str) or not SOURCE.fullmatch(url) for url in notes):
        raise ValueError("Invalid announcement review links")
    return data


def load_manifest(path):
    """Load and validate runner policy from a file confined to the checkout."""
    return validate_manifest(json.loads(_read_text(path)))


def matrix_outputs(manifest):
    """Keep workflow feature/compiler axes intact; only OS choices are data driven."""
    validate_manifest(manifest)
    output = {group: policy["labels"] for group, policy in manifest["coverage"].items()}
    pcre1 = manifest["coverage"]["linux"].get("pcre1_labels", [])
    output["pcre1"] = pcre1
    output["linux_exclude"] = [
        {"os": label, "configure": {"label": "with pcre", "opt": "--with-pcre"}}
        for label in output["linux"] if label not in pcre1
    ]
    return output


def _version(label):
    """Compare numeric runner generations without lexical version ordering."""
    match = re.fullmatch(r"(?:ubuntu|macos|windows)-(\d+)(?:\.(\d+))?(?:-(?:intel|arm))?", label)
    return tuple(int(value or 0) for value in match.groups()) if match else (0, 0)


def _standard(label, family):
    """Identify standard versioned labels, excluding aliases and paid variants."""
    patterns = {"ubuntu": r"ubuntu-\d{2}\.\d{2}(?:-arm)?", "macos": r"macos-\d+(?:-intel)?", "windows": r"windows-\d{4}"}
    return re.fullmatch(patterns[family], label) is not None


def _source(url):
    """Restrict report links to canonical runner announcements or the catalog."""
    return url if isinstance(url, str) and (SOURCE.fullmatch(url) or url == CATALOG_URL) else CATALOG_URL


def announcement_data(issues, trusted_authors, watched_labels):
    """A public issue template can apply Announcement even for an unknown opener."""
    allowed = {name.casefold() for name in trusted_authors}
    trusted = []
    notes = set()
    for issue in issues:
        if not isinstance(issue, dict):
            continue
        user = issue.get("user")
        author = user.get("login", "") if isinstance(user, dict) else ""
        author = author if isinstance(author, str) else ""
        association = issue.get("author_association", "")
        if association in {"OWNER", "MEMBER", "COLLABORATOR"} or author.casefold() in allowed:
            trusted.append(issue)
            continue
        records = parse_announcements([issue])
        if set(records) & set(watched_labels) or announcement_review_notes([issue]):
            url = issue.get("html_url", "")
            if isinstance(url, str) and SOURCE.fullmatch(url):
                notes.add(url)
    notes.update(announcement_review_notes(trusted))
    return parse_announcements(trusted), sorted(notes)


def reconcile(manifest, catalog, announcements, today):
    """Add eligible generations and annotate lifecycle; never delete existing coverage."""
    validate_manifest(manifest)
    if not catalog:
        raise ValueError("Runner catalog is empty; refusing to change runner data")
    updated = deepcopy(manifest)
    for policy in updated["coverage"].values():
        original = list(policy["labels"])
        newest = max(_version(label) for label in original)
        for label, image in sorted(catalog.items(), key=lambda item: (_version(item[0]), item[0])):
            if not LABEL.fullmatch(label) or not _standard(label, policy["family"]):
                continue
            if label in policy.get("ignored", {}) or label in policy["labels"]:
                continue
            if image.get("family") != policy["family"] or image.get("architecture") != policy["architecture"]:
                continue
            if image.get("preview") or image.get("deprecated") or label in announcements:
                continue
            if _version(label) <= newest or set(image.get("aliases", [])) & set(policy["labels"]):
                continue
            policy["labels"].append(label)

    labels = set(updated.get("pinned", {}))
    for policy in updated["coverage"].values():
        labels.update(policy["labels"])
    lifecycle = {}
    for label in sorted(labels):
        image = catalog.get(label)
        notice = announcements.get(label)
        retirement = None
        needs_review = False
        if notice:
            source = _source(notice.get("source"))
            needs_review = bool(notice.get("needs_review"))
            if notice.get("retirement") and not needs_review:
                try:
                    retirement = date.fromisoformat(notice["retirement"]).isoformat()
                except (ValueError, TypeError):
                    needs_review = True
            status = "retired" if retirement and date.fromisoformat(retirement) <= today else "deprecated"
            needs_review = needs_review or retirement is None
        elif image:
            source = _source(image.get("source"))
            status = "deprecated" if image.get("deprecated") else "active"
            needs_review = bool(image.get("deprecated"))
        else:
            source = CATALOG_URL
            status = "unknown"
            needs_review = True
        lifecycle[label] = {"status": status, "retirement": retirement, "source": source, "needs_review": needs_review}
    updated["lifecycle"] = lifecycle
    return validate_manifest(updated)


def _cell(value):
    """Escape report values so they cannot inject Markdown table rows."""
    return str(value).replace("|", "\\|").replace("\n", " ").replace("\r", " ")


def render_report(manifest):
    """No run timestamp: unchanged sources should not create weekly PR noise."""
    lines = ["# Runner lifecycle", "", "Generated from GitHub's runner catalog and retirement announcements. Review runner changes before merging; existing runners are retained.", "", "| Runner | Status | Retirement date | Source |", "| --- | --- | --- | --- |"]
    for label, item in sorted(manifest.get("lifecycle", {}).items()):
        status = item["status"] + (" (review required)" if item.get("needs_review") else "")
        lines.append(f"| `{_cell(label)}` | {_cell(status)} | {_cell(item.get('retirement') or 'not announced')} | [GitHub]({_source(item.get('source'))}) |")
    lines.extend(["", "## Pinned coverage", ""])
    for label, reason in sorted(manifest.get("pinned", {}).items()):
        lines.append(f"- `{_cell(label)}`: {_cell(reason)}")
    ignored = [(label, reason) for policy in manifest["coverage"].values() for label, reason in policy.get("ignored", {}).items()]
    if ignored:
        lines.extend(["", "## Excluded additions", ""])
        for label, reason in sorted(ignored):
            lines.append(f"- `{_cell(label)}`: {_cell(reason)}")
    if manifest.get("review_notes"):
        lines.extend(["", "## Announcements requiring review", "", "These announcements have an unknown publisher or could not be matched to an exact runner and retirement date. They do not change confirmed retirement dates.", ""])
        for url in sorted(set(manifest["review_notes"])):
            lines.append(f"- [GitHub announcement]({url})")
    return "\n".join(lines) + "\n"


def _fetch(url, token=None):
    """Fetch a bounded official response, sending credentials only to GitHub API."""
    headers = {"User-Agent": "ModSecurity-runner-maintenance", "Accept": "application/vnd.github+json"}
    if token and url.startswith("https://api.github.com/"):
        headers["Authorization"] = "Bearer " + token
    try:
        with urlopen(Request(url, headers=headers), timeout=30) as response:
            data = response.read(10_000_001)
    except (HTTPError, URLError, TimeoutError) as error:
        raise RuntimeError("Could not read an official runner source; runner files were not changed") from error
    if len(data) > 10_000_000:
        raise ValueError("Runner source response exceeds the size limit")
    return data.decode("utf-8")


def fetch_sources():
    """Read the catalog and all bounded announcement pages before any file changes."""
    readme = _fetch(CATALOG_URL)
    issues = []
    for page in range(1, 21):
        result = json.loads(_fetch(f"{ISSUES_URL}?labels=Announcement&state=all&per_page=100&page={page}", os.environ.get("GH_TOKEN")))
        if not isinstance(result, list):
            raise ValueError("Unexpected announcement response")
        issues.extend(item for item in result if isinstance(item, dict) and "pull_request" not in item)
        if len(result) < 100:
            return readme, issues
    raise ValueError("Announcement pagination limit reached; runner files were not changed")


def _write_changed(path, content):
    """Write reviewed data inside the checkout only when its content changes."""
    path = _repository_path(path)
    if path.exists() and path.read_text(encoding="utf-8") == content:
        return False
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8", newline="\n")
    return True


def main(argv=None):
    """Validate all requested paths before reading sources or changing data."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=["outputs", "update"])
    parser.add_argument("--manifest", type=Path, default=Path(".github/runner-matrix.json"))
    parser.add_argument("--report", type=Path, default=Path(".github/runner-lifecycle.md"))
    parser.add_argument("--github-output", type=Path)
    parser.add_argument("--summary", type=Path)
    parser.add_argument("--catalog", type=Path, help="Offline catalog fixture")
    parser.add_argument("--announcements", type=Path, help="Offline announcement fixture")
    parser.add_argument("--today", type=date.fromisoformat)
    args = parser.parse_args(argv)
    args.manifest = _repository_path(args.manifest)
    if args.command == "outputs":
        if args.github_output:
            args.github_output = _command_file(args.github_output, "GITHUB_OUTPUT")
    else:
        args.report = _repository_path(args.report)
        if bool(args.catalog) != bool(args.announcements):
            parser.error("Pass both --catalog and --announcements for an offline run")
        if args.catalog:
            args.catalog = _repository_path(args.catalog)
            args.announcements = _repository_path(args.announcements)
        if args.summary:
            args.summary = _command_file(args.summary, "GITHUB_STEP_SUMMARY")
    manifest = load_manifest(args.manifest)
    if args.command == "outputs":
        output = "".join(f"{key}={json.dumps(value, separators=(',', ':'))}\n" for key, value in matrix_outputs(manifest).items())
        if args.github_output:
            _append_command_file(args.github_output, output, "GITHUB_OUTPUT")
        else:
            print(output, end="")
        return 0
    if args.catalog:
        readme = _read_text(args.catalog)
        issues = json.loads(_read_text(args.announcements))
    else:
        readme, issues = fetch_sources()
    catalog = parse_catalog(readme)
    notices, notes = announcement_data(issues, manifest.get("trusted_announcement_authors", []), set(catalog))
    updated = reconcile(manifest, catalog, notices, args.today or datetime.now(timezone.utc).date())
    updated["review_notes"] = notes
    validate_manifest(updated)
    report = render_report(updated)
    changed_manifest = _write_changed(args.manifest, json.dumps(updated, indent=2) + "\n")
    changed_report = _write_changed(args.report, report)
    if args.summary:
        _append_command_file(args.summary, report, "GITHUB_STEP_SUMMARY")
    print("Runner data changed" if changed_manifest or changed_report else "Runner data unchanged")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError, RuntimeError) as error:
        print(f"Runner maintenance failed: {error}", file=sys.stderr)
        sys.exit(1)
