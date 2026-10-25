# Definition of done

Completion is measured against the user's active task, including accepted
clarifications and earlier authorization in the conversation. It is not a claim
that every ModSecurity feature or downstream integration has been verified.

## Check results

Use precise result descriptions; these labels are available when a table helps:

| Status | Meaning |
| --- | --- |
| `passed` | The specified check ran and met its acceptance criterion |
| `failed` | The check ran and failed or disproved the claimed result |
| `blocked` | A necessary prerequisite, access, decision, or environment is missing |
| `not_run` | The check was not executed; give the material reason |
| `not_applicable` | The check does not apply to the agreed scope |
| `not_verified` | A claim lacks sufficient evidence to confirm it |

An existing test file, a proposed command, or a previous run against different
code is not a new passing check.

## Task outcome

- **Complete:** The requested deliverables and necessary acceptance criteria
  are satisfied, with evidence appropriate to the scope.
- **Partial:** Useful work is finished, but a requested deliverable or required
  verification remains incomplete.
- **Blocked:** A necessary missing prerequisite or decision prevents further
  progress on the remaining work.
- **Failed:** Validation shows that the requested outcome has not been achieved.

Documentation work can be complete without a C++ build when it does not change
product behavior and source/reference checks establish its acceptance criteria.
A required behavior check that was not run must remain an explicit gap.
Optional unrelated tests do not become requirements merely because they exist.

## Completion review

Before reporting completion, confirm the following at a level of detail useful
for the task:

1. The actual result addresses the requested scope and all material constraints.
2. Changes are present and reviewable; proposed or planned work is identified as such.
3. The changed files and final diff have been reviewed. Address relevant
   whitespace findings from `git diff --check` or an equivalent check.
4. Validation results describe what actually ran, including failures, skipped
   checks, and environmental limitations.
5. Technical explanations distinguish source facts, execution results, and
   remaining uncertainty under [provenance.md](provenance.md).
6. The relevant documentation and reusable context reflect the result; another
   session can identify the next step without reconstructing this conversation.
7. Unrelated work and source boundaries are preserved. Task-owned processes,
   agents, and temporary artifacts have a known retained or finished state.
8. Delivery claims match observed commits, branches, PRs, or merge results.
   Actions remain within the user's authorization and actual permissions.

Use [change-workflow.md](change-workflow.md) for implementation,
[findings-policy.md](findings-policy.md) for unresolved findings, and
[master-integration.md](master-integration.md) only when integration is in scope.
Do not repeat every policy as a separate checklist for a trivial edit.

## Final report and saved context

State the outcome, what changed and why, meaningful checks and results,
remaining limitations, and the actual delivery state. Explain a partial or
blocked result directly instead of marking unperformed work as complete.

Update [current-state.md](current-state.md), [decisions.md](decisions.md), or
[backlog.md](backlog.md) when their information has changed. Use the
[handoff template](handoff-template.md) and
[session directory](../audit/sessions/) for a useful durable record of
substantial work. Do not create empty records simply to satisfy a ritual.
