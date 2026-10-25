# ModSecurity v3 working agreement

<!-- codex-control-plane-routing:modsecurity-core -->

## Scope and language

This agreement applies to work in **Easton97-Jens/ModSecurity**. The upstream
product is `owasp-modsecurity/ModSecurity`, branch `v3/master`. All new context,
explanations, prompts, and handovers are **English**. Preserve actual identifiers,
commands, filenames, and quoted diagnostics.

`AGENTS.md`, `CODEX_START.md`, and `.codex/` are additions for this fork. They are
working conventions, not upstream product functionality. Their routed layout is
adapted from the owner's connector repository; see the
[adaptation record](.codex/context/template-adaptation.md).

## Start a session

1. Read [context/index.md](.codex/context/index.md),
   [current-state.md](.codex/context/current-state.md),
   [decisions.md](.codex/context/decisions.md), and
   [upstream-lock.json](.codex/context/upstream-lock.json).
2. Establish the actual repository, branch, `git rev-parse HEAD`, and
   `git status --short`. Preserve unrelated and uncommitted work.
3. Identify the current request and existing authorization. Load only the
   relevant policies, technical guides, and assessment profile from the index.
4. Read affected source, declarations, callers, build switches, and tests.
   State the verified revision, unresolved questions, and next concrete action.
5. When available, run `python3 .codex/bin/verify-context.py` to detect context
   drift. This read-only helper requires Python 3 and Git with `--no-lazy-fetch`.

With chat-only repository access, read the same files through that access. If a
file cannot be read, identify the exact gap. A URL does not transfer a checkout
or previous conversations. Saved context helps only when it is read and kept current.

## Evidence and product authority

The owner requires **no invented facts**. Use this order for ModSecurity claims:

1. Current implementations, public headers, parser grammar, and build definitions.
2. Actual tests and CI definitions, distinguished from their execution results.
3. Versioned upstream documentation and comments, checked against implementation.
4. These derived guides and historical notes, at their explicitly stated revisions.

Cite `path + symbol` and, where useful, a full-commit GitHub source link. Label
statements **verified**, **inferred**, **proposed**, or **unverified** as appropriate.
Record discrepancies in [fact-check.md](.codex/context/fact-check.md). File
existence, parser recognition, compilation, and runtime support are separate claims.

Never invent API signatures, CVEs, benchmarks, compatibility, thread safety,
security guarantees, or successful tests. Keep core library, external connectors,
Core Rule Set, `v2/master`, and `v3/master` distinct. Submodule gitlinks identify
separate sources; their internals require separate inspection.

## Commands, changes, and ownership

Use the [task workflow](.codex/context/task-workflow.md) and
[command policy](.codex/context/command-execution-policy.md). Continue routine
work within the user's authorized scope. Ask only about a consequential unresolved
choice, missing required access, or an action outside that scope.

Discover commands from the current checkout. The [build guide](.codex/context/build-and-test.md)
explains its Autotools and Windows paths and conditional tests. Do not assume
connector-specific targets, a global skill, a command wrapper, a scan service,
or a local configuration file exists.

Follow `.editorconfig` and neighboring style. Read declarations and definitions
together; preserve C++17 compatibility. Consider object lifetimes, explicit
lengths, C/C++ interface differences, phase order, intervention handling, error
paths, and feature switches. Distinguish parser inputs from generated code.
Do not change dependency pins, generated files, or unrelated formatting incidentally.

## Git, validation, and delivery

Preserve user work. Stage only the task's files. Do not force-push, rewrite shared
history, remove unknown work, or change repository visibility. Target this existing
fork. A feature branch and PR are the normal review route; commit/push permission
does not by itself authorize merging into `v3/master`.

Use [testing-and-evidence.md](.codex/context/testing-and-evidence.md) for checks
proportionate to the change. Documentation consistency is not a C++ test result.
Record command, working directory, revision, outcome, skips, and limits. A
security profile is a review workflow, not evidence that an assessment occurred.

Before handover, apply [definition-of-done.md](.codex/context/definition-of-done.md):
update current state, justified decisions, and the real backlog; record a useful
session under `.codex/audit/sessions/` using the
[handoff template](.codex/context/handoff-template.md). Keep credentials and private
traffic out of versioned notes. Report what changed, what was checked, and the
next concrete step. Do not claim automatic memory or automatic policy inheritance.
