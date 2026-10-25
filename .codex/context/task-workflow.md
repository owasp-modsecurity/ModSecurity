# Task workflow and cross-chat continuity

Use saved context to resume efficiently, then verify the code and working state
relevant to the new task. The files are a durable starting point, not a promise
that every new chat has automatically read the repository or prior conversation.

## Start with the actual task

1. Read the applicable instructions, [index.md](index.md),
   [current-state.md](current-state.md), and relevant entries in
   [decisions.md](decisions.md).
2. Identify the user's intended result, constraints, and earlier authorizations
   that still apply. Treat new clarifications as updates to the active task
   unless the user replaces or cancels it.
3. Check the working repository, branch, commit, and existing changes. Separate
   the working commit from the source baseline in
   [upstream-lock.json](upstream-lock.json).
4. Select the relevant technical chapters and policies. Read the affected source,
   callers, build conditions, and tests rather than relying only on summaries.
5. Establish a practical acceptance criterion and the next useful action.

Use [project-context.md](project-context.md) to learn the library,
[repository-map.md](repository-map.md) to find code, and
[architecture-and-boundaries.md](architecture-and-boundaries.md) to establish
ownership. Use [feasibility-policy.md](feasibility-policy.md) when prerequisites
or external actions materially affect execution.

If a chat has no checkout, use the repository access actually available. Ask for
a specific inaccessible file only when it is needed; do not claim to have read
files or remembered conversations that are unavailable.

## Plan in proportion to the work

For a small, reversible edit, a short task summary and focused check are enough.
For substantial work, keep a compact plan containing the useful details:

- goal and deliverables;
- repository and file ownership;
- material constraints, assumptions, and unresolved decisions;
- acceptance criteria and validation;
- implementation steps and delivery state.

Track each material requirement until it is satisfied, explicitly superseded,
or identified as blocked. A table is optional. Do not silently omit a requested
part or expand a task to unrelated code, dependencies, scans, or integration.

## Questions, decisions, and authorization

Investigate available evidence before asking. Make reasonable implementation
choices when they are within scope and reversible. Ask when an unresolved
choice materially changes behavior, compatibility, ownership, cost, access,
or a destructive action and the existing conversation does not resolve it.

User authorization persists throughout the active task. A new phase or tool
does not by itself require renewed permission. Prepare concrete, reviewable
work before asking for a genuinely missing final authorization. If one part is
blocked, complete useful independent parts and explain the exact remaining gap.

## Work and verification

Apply [change-workflow.md](change-workflow.md) to implementation and
[documentation-and-traceability.md](documentation-and-traceability.md) to
explanations. Preserve unrelated changes and assign disjoint files when work is
delegated. Keep delegated findings tied to evidence and review the integrated
result; delegation does not itself verify a claim.

Use checks that establish the requested result. The context check entry points,
run from the repository root, are:

```sh
python3 .codex/bin/verify-context.py
python3 .codex/bin/validate-structure.py
```

These checks concern the maintained context. Use
[build-and-test.md](build-and-test.md) and actual build definitions for product
verification. Do not describe a structural check as a C++ test run, or repeat
broad tests after the necessary evidence is sufficient.

## End with a resumable state

Apply [definition-of-done.md](definition-of-done.md) and reconcile the material
requirements with the actual result. Update [current-state.md](current-state.md)
with the working state, real verification, and next concrete step. Update
[decisions.md](decisions.md) and [backlog.md](backlog.md) when decisions or open
work changed.

For substantial work, save a concise record in
[audit sessions](../audit/sessions/) using
[handoff-template.md](handoff-template.md). Include the source revision, files,
meaningful checks, unresolved questions, and delivery facts. Keep historical
results attached to their actual revision and avoid duplicating whole technical
chapters or private conversation transcripts.

Finish with a self-contained account of what was achieved, how it was checked,
what remains uncertain, and where the work stands. Follow
[git-policy.md](git-policy.md) for authorized delivery and
[upstream-sync.md](upstream-sync.md) when the task intentionally changes the
official source baseline.
