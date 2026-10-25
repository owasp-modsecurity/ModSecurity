# Repository-specific context: ModSecurity core

The root [AGENTS.md](../../AGENTS.md) is a reusable agreement for any repository.
This file supplies the concrete identity, source rules, startup path, and technical
constraints for this checkout. Keep these project-specific details here or in the
linked guides so the root agreement can be reused without modification.

## Identity, language, and scope

| Role | This repository's value |
| --- | --- |
| Canonical project and product source | `owasp-modsecurity/ModSecurity`, branch `v3/master` |
| Documented source commit | `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` |
| Working repository and branch | Discover the actual checkout selected by the current task; contributor forks are valid |
| Pull-request head | Resolve the current head repository, branch, and full commit from the selected PR before checkout |
| Delivery and integration target | The destination authorized for this task; verify the PR base when a PR is involved |
| Maintained files | English; preserve actual identifiers and diagnostics |

Recheck the current branch, commit, worktree, and PR state before acting. The
`repository` fields in the routing, structure, and policy-catalog manifests name
the canonical ModSecurity project. They are not a remote URL or a push target;
the validators do not require a contributor's `origin` to point to upstream.
Source metadata and the two separately recorded historical fork observations are
in [upstream-lock.json](upstream-lock.json) and [provenance.md](provenance.md).
Historical contributor branches and PRs do not select the current checkout.

The `.codex` layout is adapted from the owner's connector repository as described
in [template-adaptation.md](template-adaptation.md). This is a working convention;
it is not an upstream library feature. Only the root agreement is intended for
unchanged reuse across repositories. These concrete guides, manifests, source
pins, and helper scripts remain tailored to this core checkout.

## Startup for this checkout

1. Read the reusable root agreement and [index.md](index.md).
2. Read [current-state.md](current-state.md), the relevant [decisions](decisions.md),
   and [upstream-lock.json](upstream-lock.json). Keep the working commit distinct
   from the pinned product baseline.
3. Check actual repository state where checkout access is available. Preserve
   unrelated work. With chat-only access, read the same files through the access
   actually available and identify a required inaccessible file precisely.
4. Load the technical guides, policies, and optional profile relevant to the task.
   Read affected implementation, declarations, callers, build conditions, and tests.
5. Run suitable context checks only when their files and prerequisites are available.
   Use [bin/README.md](../bin/README.md) for the complete check contract and limits.

From this repository's root, the available context checks are:

```sh
python3 .codex/bin/verify-context.py
python3 .codex/bin/validate-structure.py
python3 .codex/bin/validate-codex-inheritance.py --check
```

Use `--strict-source` on the source verifier when upstream product files are
expected to be unchanged. The source verifier needs Python 3 and Git 2.45 or later supporting
`--no-lazy-fetch`; the TOML validators need Python 3.11 or later. These requirements
belong to these helpers, not to the reusable root agreement. Do not infer a
successful library build or product test run from their results.

## ModSecurity evidence and implementation constraints

Derive ModSecurity facts from official code actually read at the pinned revision
or an explicitly verified newer official revision. The connector template supplies
structure only. Record code/documentation conflicts in [fact-check.md](fact-check.md).
Keep the core library, external connectors, Core Rule Set, and v2/v3 branches
separate. The four gitlinks identify separate sources; their internals require
separate inspection and their changes must be within the user's task.

For this baseline, preserve C++17 compatibility and inspect C/C++ declarations
with their implementations. Follow `.editorconfig` and surrounding style. Review
object ownership and lifetimes, callbacks, explicit data lengths, phase ordering,
intervention handling, error paths, and optional feature switches when relevant.
Do not assume global thread safety or full C/C++ interface parity. The
[C++ guide](cpp-guide.md) and [architecture](architecture.md) give source-linked
details; the build sets the standard in
[configure.ac](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L63).

Distinguish `.ll`/`.yy` parser inputs from generated files. Use recorded dependency
pins for ordinary setup. Discover real Autotools/Windows build commands and
conditional test behavior in [build-and-test.md](build-and-test.md); a connector's
Make targets do not establish commands for this core checkout.

## Delivery and continuation

Follow the user's authorized repository and branch rather than a destination
copied from a historical handover. Updating an upstream PR normally changes its
verified head branch, which can be in a contributor fork; it does not authorize
a direct upstream push or merge. Follow [git-policy.md](git-policy.md),
[delivery-and-ci.md](delivery-and-ci.md), and
[master-integration.md](master-integration.md) for their respective operations.
Preparing or updating a PR does not itself merge it into its base branch.

Update current state, justified decisions, and the real backlog when they change.
For a useful longer handover, use [handoff-template.md](handoff-template.md) and
`.codex/audit/sessions/`. Follow [definition-of-done.md](definition-of-done.md) and
report actual verification. The new root portability and language decisions are
recorded in [decisions.md](decisions.md).
