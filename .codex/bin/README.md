# Project-local context validators

These helpers maintain this project's context. They are separate from ModSecurity's
build system and tests. Run them from a checkout; they derive the root from their
own location. They do not fetch, build, install, invoke scanners, or change files
unless the explicit digest refresh option below is used.

| Command | What it establishes |
| --- | --- |
| `python3 .codex/bin/verify-context.py` | Pinned Git baseline, complete source inventory, local Markdown targets, full-commit ModSecurity source paths/line ranges, and reported source drift |
| `python3 .codex/bin/verify-context.py --strict-source` | The same checks, failing if source differs outside `AGENTS.md`, `CODEX_START.md`, and `.codex/` |
| `python3 .codex/bin/validate-structure.py` | Canonical project identity and consistency across context/structure manifests, required layout, schema, routing targets, profile/component/check inventories, and root routing marker |
| `python3 .codex/bin/validate-codex-inheritance.py --check` | Canonical project identity and consistency across context/catalog manifests; the local primary-policy catalog exactly matches routed policy files and their SHA-256 digests |

The source verifier uses the Python standard library and requires **Git 2.45 or
later** on `PATH`. The [Git 2.45 release notes](https://github.com/git/git/blob/master/Documentation/RelNotes/2.45.0.adoc)
record the introduction of `--no-lazy-fetch`; the [Git 2.45 manual](https://git-scm.com/docs/git/2.45.0)
describes how it prevents downloading missing objects from a promisor remote.
Use a trusted Git executable on `PATH`; the verifier passes arguments directly,
without invoking a shell. The TOML validators require **Python 3.11 or later**
(`tomllib`); no package installation is needed. Ordinary mismatch exits with `1`;
unreadable inputs or unavailable prerequisites exit with `2`. Successful checks
exit with `0`.

The `repository` fields identify the canonical project,
`owasp-modsecurity/ModSecurity`. This identity is independent of the checkout's
owner, branch, or remotes, so a fork can use the same manifests. The validators
do not select a push destination; discover and verify that destination separately.

## Deliberate policy maintenance

After reviewing a primary-policy edit, regenerate only the local catalog:

```sh
python3 .codex/bin/validate-codex-inheritance.py --refresh
python3 .codex/bin/validate-codex-inheritance.py --check
```

`--refresh` writes `.codex/inheritance-manifest.toml` from the current routing
manifest with the canonical project identity. It refuses to write when the
routing manifest identifies another project. Review that diff. Hash updates
record changed content; they do not establish that the change is correct.
Routine session checks use `--check`.
No child/submodule policy inheritance is active or automatically discovered.

## Limits

Source links are checked against locally available Git objects. Historical
source revisions are allowed only in decision and session records. Fenced
examples are excluded from Markdown-link parsing. External websites and local
heading anchors are not fetched or fully validated. Source excerpts still need
human/code review to determine whether they support the associated claim.

A passing context check establishes neither a successful library build nor
runtime behavior, security, integration coverage, or complete C++ documentation.
