# 2026-10-25 — English context foundation and template adaptation

## Request and scope

Create `AGENTS.md` and detailed reusable context in the existing
`Easton97-Jens/ModSecurity` fork. Use English throughout. Ground ModSecurity
explanations in official `v3/master` code. Adapt the `.codex` organization from
`Easton97-Jens/ModSecurity-conector:master` so later chats can resume from saved
facts, decisions, and actual verification.

Source baseline: `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`.
Template baseline: `42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3`.
The [provenance](../../context/provenance.md) and
[adaptation record](../../context/template-adaptation.md) distinguish their roles.

## Delivered content and review

The additions cover the repository map and languages, architecture, C++17/C API,
ownership and callbacks, phases and interventions, configuration/rules/features,
builds/tests, factual caveats, and upstream maintenance. Routed policies and
profiles follow the template's organization with actual core components.
Current state, decisions, backlog, prompts, and this session mechanism provide
explicit continuity. No automatic memory or inheritance is asserted.

The technical chapters were independently cross-reviewed with targeted source
inspection. Corrections preserved in the final guides include disabled-engine
guards, conditionally built YAJL test utilities, outdated examples, and the
fact that method success is not the same as an intervention or connector result.
A stale current 25-file count was removed after the layout expanded.

All 72 tracked reference `.codex` files were read across the parallel review,
along with root instructions/README/ignore rules and the relevant Makefile
inventory. That is not an audit of all 1,714 reference repository entries or
its external modules. Main-library submodule contents were not comprehensively
reviewed; their exact gitlink pins are inventoried.

## Checks and evidence limits

The following checks completed successfully in the final local adaptation:

```sh
python3 .codex/bin/verify-context.py --strict-source
python3 .codex/bin/validate-structure.py
python3 .codex/bin/validate-codex-inheritance.py --check
```

Each returned exit 0. The source verifier checked 850 main-tree entries and 487
unique commit-pinned ModSecurity links. The structure validator checked 93 routes;
the catalog validator checked 23 primary-policy digests. Python was 3.12.14.
Pinned connector-template source links and line ranges were separately checked
against the local reference Git objects.

Independent temporary-fixture probes established that:

- missing local links, invalid source ranges, and links outside the root fail;
- stale catalog digests and duplicate actual IDs fail;
- derived-ID collisions fail before `--refresh` writes a catalog;
- catalog symlinks fail, preserving an outside sentinel file;
- historical source links are accepted in the new decisions/session locations,
  and rejected in active guides and the obsolete session location;
- fenced examples do not create false Markdown-reference errors;
- a new dated session record passes without entering normal policy routing.

The temporary probes stayed outside the delivery tree. The three helper defects
identified during review were corrected and the corresponding cases rerun.
No C++ build, product test suite, security scan, or connector runtime was run.
A source-linked explanation and structural checks are not those execution results.

## Repository boundary and continuation

The original main source tree and all gitlinks remain unchanged relative to the
pinned baseline. Only `AGENTS.md`, `CODEX_START.md`, and `.codex/` are additions.
The delivery branch is `codex/kontext-v3` in the existing fork.
At this recorded foundation stage,
[draft PR #120](https://github.com/Easton97-Jens/ModSecurity/pull/120) was open against
`v3/master` and had not been merged. This historical delivery record does not select
a repository or branch for a later task. The published adaptation at
`bad6cd355fd78ffcfa6393b1c3ada8d734feb098` was cloned from the fork and all three
validators passed there. Its tree `9a884e6e02525a469bfa531e8aaa767cf915e856`
matched the reviewed local tree. The observed comparison contained 98 added
context files and no product-source changes. No PR-triggered workflow runs were
returned for that commit when checked; this was not reported as a CI pass.
Consult [current state](../../context/current-state.md) and the actual PR head
for subsequent context-only delivery updates.

Start a later chat with [the startup prompt](../../context/prompts/start-new-chat.md).
Read actual source and selected policies for that task. Update the saved state
with results actually obtained; do not infer a completed build or merge from this
handover.
