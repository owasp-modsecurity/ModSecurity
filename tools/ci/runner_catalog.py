"""Parse public runner metadata without executing or fetching upstream content.

The catalog is the Available Images table in actions/runner-images/README.md.
Announcement bodies are prose, so unknown or conflicting retirement dates stay
unset and require review. Replacement labels and unchecked template items are
never considered affected images.
"""

import re
from datetime import date

CATALOG_SOURCE = "https://github.com/actions/runner-images#available-images"
_ISSUE_URL = re.compile(r"https://github\.com/actions/runner-images/issues/([1-9]\d*)\Z")
_LABEL = re.compile(
    r"(?:ubuntu-(?:latest|\d{2}\.\d{2}|slim)(?:-arm)?(?:-\d+-cores)?"
    r"|macos-(?:latest|\d+(?:\.\d+)?)(?:-(?:intel|large|xlarge))?"
    r"|xcode-\d+(?:\.\d+)?(?:-xlarge)?"
    r"|windows-(?:latest|\d{4}|11)(?:-vs\d{4})?(?:-arm)?(?:-\d+-cores)?)\Z"
)
_MONTHS = {
    name.casefold(): index
    for index, name in enumerate(
        ("January", "February", "March", "April", "May", "June",
         "July", "August", "September", "October", "November", "December"), 1
    )
}


def _family(label):
    """Map Xcode image labels to the macOS family used by coverage policy."""
    return "macos" if label.startswith("xcode-") else label.split("-", 1)[0]


def _cells(line):
    """Split a Markdown table row without treating escaped pipes as columns."""
    return [cell.strip() for cell in re.split(r"(?<!\\)\|", line.strip().strip("|"))]


def parse_catalog(readme):
    """Return one record per official table label, retaining each alias group.

    Raise ValueError on an absent, empty, or structurally changed catalog rather
    than treating a failed upstream parse as an empty set of available runners.
    """
    if not isinstance(readme, str):
        raise ValueError("The runner catalog must be Markdown text")
    section = re.search(r"(?im)^##\s+Available Images\s*$", readme)
    if not section:
        raise ValueError("Available Images section is missing")
    lines = readme[section.end():].splitlines()
    while lines and not lines[0].strip():
        lines.pop(0)
    if len(lines) < 2:
        raise ValueError("Available Images table is missing")
    header = [re.sub(r"\s+", " ", cell).casefold() for cell in _cells(lines[0])]
    required = ("image", "architecture", "yaml label", "included software")
    if any(header.count(name) != 1 for name in required):
        raise ValueError("Available Images table columns changed")
    if not lines[0].lstrip().startswith("|"):
        raise ValueError("Available Images table header is malformed")
    if len(_cells(lines[1])) != len(header) or not all(
        re.fullmatch(r":?-{3,}:?", cell.replace(" ", "")) for cell in _cells(lines[1])
    ):
        raise ValueError("Available Images table separator is malformed")
    columns = {name: header.index(name) for name in required}
    records = {}
    for line in lines[2:]:
        if not line.lstrip().startswith("|"):
            if line.strip() and not re.match(r"^#{1,6}\s", line):
                raise ValueError("Available Images table ended unexpectedly")
            break
        cells = _cells(line)
        if len(cells) != len(header):
            raise ValueError("Available Images table row is malformed")
        raw_arch = cells[columns["architecture"]].casefold().replace("_", "-")
        architecture = {
            "x64": "x64", "amd64": "x64", "x86-64": "x64",
            "arm64": "arm64", "aarch64": "arm64",
        }.get(raw_arch)
        if architecture is None:
            raise ValueError("Unknown runner architecture")
        aliases = list(dict.fromkeys(re.findall(r"`([^`\r\n]+)`", cells[columns["yaml label"]])))
        if not aliases or any(_LABEL.fullmatch(label) is None for label in aliases):
            raise ValueError("Unknown or missing runner label")
        families = {_family(label) for label in aliases}
        if len(families) != 1:
            raise ValueError("Runner alias group crosses OS families")
        image = cells[columns["image"]]
        entry = {
            "family": families.pop(),
            "architecture": architecture,
            "preview": bool(re.search(r"\b(?:preview|beta)\b", image, re.I)),
            "deprecated": bool(re.search(r"\bdeprecated\b", image, re.I)),
            "source": CATALOG_SOURCE,
            "aliases": aliases,
        }
        for label in aliases:
            if label in records and records[label] != entry:
                raise ValueError("Runner label appears in conflicting image rows")
            records[label] = dict(entry, aliases=list(aliases))
    if not records:
        raise ValueError("Available Images table has no runner records")
    return records


def _sections(body):
    """Group announcement prose by normalized headings, omitting template comments."""
    sections = {}
    heading = ""
    for line in re.sub(r"<!--.*?-->", "", body, flags=re.S).splitlines():
        match = re.match(r"^#{1,6}\s+(.+?)\s*#*\s*$", line)
        if match:
            heading = match.group(1).strip().casefold()
        else:
            sections.setdefault(heading, []).append(line)
    return {name: "\n".join(lines) for name, lines in sections.items()}


def _literal_labels(text):
    """Extract recognized backtick labels without inventing YAML runner names."""
    return {
        value for value in re.findall(r"`([^`\r\n]+)`", text)
        if _LABEL.fullmatch(value)
    }


def _source(issue):
    """Accept canonical runner-images Announcement issues with matching identities."""
    if not isinstance(issue, dict) or "pull_request" in issue:
        return None
    source = issue.get("html_url")
    match = _ISSUE_URL.fullmatch(source) if isinstance(source, str) else None
    if not match:
        return None
    number = issue.get("number")
    if number is not None and (isinstance(number, bool) or str(number) != match.group(1)):
        return None
    labels = issue.get("labels")
    if labels is not None:
        if not isinstance(labels, (list, tuple)) or not any(
            (value.get("name") if isinstance(value, dict) else value) == "Announcement"
            for value in labels
        ):
            return None
    return source


def _lifecycle_issue(issue, sections):
    """Distinguish whole runner-image retirements from bundled tool removals."""
    title = issue.get("title", "")
    breaking = sections.get("breaking changes", "") + "\n" + sections.get("", "")
    lifecycle = r"\b(?:deprecat\w*|retir\w*|unsupported)\b"
    if not isinstance(title, str) or not re.search(lifecycle, title, re.I):
        return False
    if re.search(r"\b(?:tools?|runtimes?|simulators?|parameters?|packages?)\b", title, re.I):
        return False
    return bool(
        re.search(r"\brunner\s+images?\b", title, re.I)
        or (
            re.search(lifecycle, breaking, re.I)
            and re.search(r"\brunner\s+images?\b", breaking, re.I)
        )
    )


def _checked_images(text):
    """Return only checked image entries from the upstream issue template."""
    return [
        match.group(1).strip()
        for line in text.splitlines()
        if (match := re.match(r"^\s*[-*]\s+\[[xX]\]\s+(.+?)\s*$", line))
    ]


def _checked_allows(label, checked):
    """Match literal impact labels to checked display names, never invent labels."""
    plain = [re.sub(r"[`*_]", "", item).strip().casefold() for item in checked]
    match = re.fullmatch(r"ubuntu-(\d{2}\.\d{2})(-arm)?", label)
    if match:
        name = "ubuntu " + match.group(1) + (" arm64" if match.group(2) else "")
        return name in plain
    match = re.fullmatch(r"windows-(\d{4})(?:-vs(\d{4}))?", label)
    if match:
        name = "windows server " + match.group(1)
        if match.group(2):
            name += " with visual studio " + match.group(2)
        return name in plain
    match = re.fullmatch(r"macos-(\d+(?:\.\d+)?)(?:-(?:intel|large|xlarge))?", label)
    if match:
        # The display name alone does not identify a historical macOS alias.
        # Accept the literal impact list only when both architectures are checked.
        name = "macos " + match.group(1)
        return name in plain and name + " arm64" in plain
    return False


def _leading_labels(text):
    """Separate retirement statements from recommendations in the same section."""
    confirmed = set()
    unconfirmed = set()
    recommendation = re.compile(
        r"\b(?:please\s+)?(?:migrat\w*|switch\w*|updat\w*|upgrad\w*|mov(?:e|ing)"
        r"|recommend\w*|consider\w*|replac\w*)\b"
        r"|\b(?:in favou?r of|instead|replacement)\b"
        r"|(?:^|,\s*|\band\s+|\bthen\s+)(?:please\s+)?use\b"
        r"|\b(?:should|must|can)\s+use\b",
        re.I,
    )
    for sentence in re.split(r"(?<=[.!?])\s+|;\s*|[\r\n]+", text):
        # Keep the retirement statement before an inline migration recommendation.
        affected_text = recommendation.split(sentence, maxsplit=1)[0]
        labels = _literal_labels(affected_text)
        if re.search(r"\b(?:deprecat\w*|retir\w*|unsupported|unavailable|remov\w*)\b", affected_text, re.I):
            confirmed.update(labels)
        else:
            unconfirmed.update(labels)
    if not confirmed:
        return unconfirmed, set(unconfirmed)

    # Prose mentioning several generations of one OS can describe replacements
    # rather than multiple retirements. Require review unless clearly separated.
    generations = {}
    for label in confirmed:
        match = re.match(r"((?:ubuntu|macos|windows|xcode)-\d+(?:\.\d+)?)", label)
        generations.setdefault(_family(label), set()).add(match.group(1) if match else label)
    ambiguous = {
        label for label in confirmed
        if len(generations[_family(label)]) > 1
    }
    return confirmed, ambiguous


def _affected_label_details(sections):
    """Combine explicit impact labels and checked entries, tracking ambiguity."""
    preamble = re.split(
        r"(?im)^\s*(?:mitigation ways?|possible impact|runner images affected)\s*:?\s*$",
        sections.get("", ""), maxsplit=1,
    )[0]
    leading = preamble + "\n" + sections.get("breaking changes", "")
    affected, ambiguous = _leading_labels(leading)
    checked = _checked_images(sections.get("runner images affected", ""))
    checked_labels = set()
    for item in checked:
        checked_labels.update(_literal_labels(item))
    for label in _literal_labels(sections.get("possible impact", "")):
        if _checked_allows(label, checked):
            checked_labels.add(label)
    return affected | checked_labels, ambiguous - checked_labels


def _affected_labels(sections):
    """Return impacted labels without exposing internal ambiguity bookkeeping."""
    return _affected_label_details(sections)[0]


def _date_value(value):
    """Normalize a complete ISO or English date, rejecting incomplete dates."""
    value = value.strip()
    try:
        if re.fullmatch(r"\d{4}-\d{2}-\d{2}", value):
            return date.fromisoformat(value).isoformat()
        match = re.fullmatch(r"([A-Za-z]+)\s+(\d{1,2})(?:st|nd|rd|th)?,\s*(\d{4})", value, re.I)
        if match and match.group(1).casefold() in _MONTHS:
            return date(int(match.group(3)), _MONTHS[match.group(1).casefold()], int(match.group(2))).isoformat()
    except ValueError:
        pass
    return None


def _retirement(sections):
    """Accept one explicit retirement date; flag missing or conflicting values."""
    lines = [
        re.sub(r"[*_]", "", line).strip()
        for line in sections.get("target date", "").splitlines()
    ]
    dates = []
    invalid = False
    for index, line in enumerate(lines):
        match = re.fullmatch(r"(?:final\s+)?retirement(?:\s+date)?\s*:\s*(.*)", line, re.I)
        if not match:
            continue
        value = match.group(1).strip()
        if not value:
            value = next((item for item in lines[index + 1:] if item), "")
        parsed = _date_value(value)
        if parsed:
            dates.append(parsed)
        else:
            invalid = True
    unique = set(dates)
    if invalid or len(unique) != 1:
        return None, True
    return unique.pop(), False


def _parsed_issue(issue):
    """Validate the source and lifecycle scope before parsing announcement data."""
    source = _source(issue)
    body = issue.get("body") if isinstance(issue, dict) else None
    if source is None or not isinstance(body, str):
        return None
    sections = _sections(body)
    if not _lifecycle_issue(issue, sections):
        return None
    return source, sections


def parse_announcements(issues):
    """Return affected labels and explicit retirement dates from announcements.

    Invalid URLs, PRs, unrelated tool changes, mitigation labels and unchecked
    template entries are ignored. Conflicting or incomplete dates require review.
    """
    records = {}
    parsed = [value for issue in issues if (value := _parsed_issue(issue))]
    for source, sections in sorted(parsed, key=lambda item: int(item[0].rsplit("/", 1)[1])):
        retirement, needs_review = _retirement(sections)
        affected, ambiguous = _affected_label_details(sections)
        for label in sorted(affected):
            label_retirement = None if label in ambiguous else retirement
            entry = {
                "status": "deprecated",
                "retirement": label_retirement,
                "source": source,
                "needs_review": needs_review or label in ambiguous,
            }
            if label in records:
                previous = records[label]
                conflict = previous["retirement"] != label_retirement
                entry["source"] = previous["source"]
                entry["retirement"] = None if conflict else label_retirement
                entry["needs_review"] = previous["needs_review"] or entry["needs_review"] or conflict
            records[label] = entry
    return records


def announcement_review_notes(issues):
    """Return source links for lifecycle announcements without resolvable labels."""
    return sorted({
        source for issue in issues
        if (value := _parsed_issue(issue))
        for source, sections in [value]
        if not _affected_labels(sections)
    })
