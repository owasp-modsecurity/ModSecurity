# Project-local context validators

These helpers are additions for this fork. They are separate from ModSecurity's
build system and tests. Run them from a checkout; they derive the root from their
own location. They do not fetch, build, install, invoke scanners, or change files
unless the explicit digest refresh option below is used.

| Command | What it establishes |
| --- | --- |
| `python3 .codex/bin/verify-context.py` | Pinned Git baseline, complete source inventory, local Markdown targets, full-commit ModSecurity source paths/line ranges, and reported source drift |
| `python3 .codex/bin/verify-context.py --strict-source` | The same checks, failing if source differs outside `AGENTS.md`, `CODEX_START.md`, and `.codex/` |
| `python3 .codex/bin/validate-structure.py` | Required layout, schema, routing targets, profile/component/check inventories, and root routing marker |
| `python3 .codex/bin/validate-codex-inheritance.py --check` | The local primary-policy catalog exactly matches routed policy files and their SHA-256 digests |

The source verifier uses the Python standard library and Git with
`--no-lazy-fetch` so missing objects are reported instead of downloaded. The TOML
validators require **Python 3.11 or later** (`tomllib`); no package installation is
needed. Ordinary mismatch exits with `1`; unreadable inputs or unavailable
prerequisites exit with `2`. Successful checks exit with `0`.

## Deliberate policy maintenance

After reviewing a primary-policy edit, regenerate only the local catalog:

```sh
python3 .codex/bin/validate-codex-inheritance.py --refresh
python3 .codex/bin/validate-codex-inheritance.py --check
```

`--refresh` writes `.codex/inheritance-manifest.toml` from the current routing
manifest. Review that diff. Hash updates record changed content; they do not
establish that the change is correct. Routine session checks use `--check`.
No child/submodule policy inheritance is active or automatically discovered.

## Limits

Source links are checked against locally available Git objects. Historical
source revisions are allowed only in decision and session records. Fenced
examples are excluded from Markdown-link parsing. External websites and local
heading anchors are not fetched or fully validated. Source excerpts still need
human/code review to determine whether they support the associated claim.

A passing context check establishes neither a successful library build nor
runtime behavior, security, integration coverage, or complete C++ documentation.
