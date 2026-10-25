#!/usr/bin/env python3
"""Check or deliberately refresh the local policy digest catalog.

No child repositories are active. This helper does not fetch files, alter source,
or make Codex load instructions. Only --refresh writes the local catalog.
"""

import argparse
import hashlib
import json
import sys
from pathlib import Path

try:
    import tomllib
except ImportError:
    print("ERROR: Python 3.11 or later is required for TOML validation.", file=sys.stderr)
    sys.exit(2)

ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / ".codex/inheritance-manifest.toml"


def load(path):
    with path.open("rb") as handle:
        return tomllib.load(handle)


def expected_rows():
    routes = load(ROOT / ".codex/context/manifest.toml")
    policies = routes["primary_policies"]
    if not isinstance(policies, dict) or not policies:
        raise ValueError("No primary policies are routed")
    rows = []
    identifiers = set()
    sources = set()
    for key, relative in sorted(policies.items()):
        if not isinstance(key, str) or not key:
            raise ValueError(f"Invalid policy routing key: {key!r}")
        identifier = "MODSECURITY-" + key.upper().replace("_", "-")
        if identifier in identifiers:
            raise ValueError(f"Duplicate derived policy ID: {identifier}")
        identifiers.add(identifier)
        if not isinstance(relative, str) or Path(relative).is_absolute():
            raise ValueError(f"Invalid policy path: {relative!r}")
        path = (ROOT / relative).resolve()
        if not path.is_relative_to(ROOT) or not path.is_file():
            raise ValueError(f"Policy path is missing or outside the repository: {relative}")
        if path in sources:
            raise ValueError(f"Duplicate routed policy source: {relative}")
        sources.add(path)
        rows.append({
            "id": identifier,
            "source": relative,
            "sha256": hashlib.sha256(path.read_bytes()).hexdigest(),
            "inheritable": True,
        })
    return rows


def validate_catalog_location():
    if CATALOG.is_symlink():
        raise ValueError("The local policy catalog must not be a symbolic link")
    if not CATALOG.resolve().is_relative_to(ROOT):
        raise ValueError("The local policy catalog path leaves the repository")


def refresh(rows):
    validate_catalog_location()
    lines = [
        "# Local policy catalog. Generated deliberately with --refresh.",
        "# Digests record content, not correctness. No child inheritance is active.",
        "schema_version = 1", 'repository = "Easton97-Jens/ModSecurity"',
        "children = []", "",
    ]
    for row in rows:
        lines.append("[[policies]]")
        for key in ("id", "source", "sha256"):
            lines.append(f"{key} = {json.dumps(row[key])}")
        lines.extend(["inheritable = true", ""])
    CATALOG.write_text("\n".join(lines), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--check", action="store_true", help="Read-only catalog validation.")
    mode.add_argument("--refresh", action="store_true", help="Rewrite only the local catalog after policy review.")
    parser.add_argument("--json", action="store_true", help="Emit machine-readable results.")
    args = parser.parse_args()
    validate_catalog_location()
    expected = expected_rows()
    if args.refresh:
        refresh(expected)
    catalog = load(CATALOG)
    errors = []
    if catalog.get("schema_version") != 1:
        errors.append("Unsupported catalog schema_version")
    if catalog.get("repository") != "Easton97-Jens/ModSecurity":
        errors.append("Unexpected catalog repository identity")
    if catalog.get("children") != []:
        errors.append("Active child inheritance is not supported by this schema")
    actual = catalog.get("policies", [])
    if not isinstance(actual, list):
        raise ValueError("Catalog policies must be a list")
    observed = {}
    for row in actual:
        if not isinstance(row, dict) or not isinstance(row.get("id"), str):
            errors.append("Invalid policy catalog row")
            continue
        if row["id"] in observed:
            errors.append(f"Duplicate policy ID: {row['id']}")
        observed[row["id"]] = row
    wanted = {row["id"]: row for row in expected}
    for identifier in sorted(wanted.keys() - observed.keys()):
        errors.append(f"Policy missing from catalog: {identifier}")
    for identifier in sorted(observed.keys() - wanted.keys()):
        errors.append(f"Unrouted policy in catalog: {identifier}")
    for identifier in sorted(wanted.keys() & observed.keys()):
        if wanted[identifier] != observed[identifier]:
            errors.append(f"Policy path, digest, or metadata is stale: {identifier}")
    result = {
        "ok": not errors, "policies": len(expected),
        "active_child_repositories": 0, "refreshed": args.refresh, "errors": errors,
    }
    if args.json:
        print(json.dumps(result, indent=2))
    else:
        for error in errors:
            print("ERROR: " + error, file=sys.stderr)
        if not errors:
            action = "Refreshed and checked" if args.refresh else "Checked"
            print(f"OK: {action} {len(expected)} local policy digests; no active child inheritance.")
    return 1 if errors else 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (OSError, ValueError, KeyError, TypeError) as exc:
        print("ERROR: " + str(exc), file=sys.stderr)
        sys.exit(2)
