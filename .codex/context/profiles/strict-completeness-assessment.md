# Optional strict execution-completeness overlay

## Trigger

Use only when the user explicitly requires every applicable mandatory target in a declared assessment to reach an executed result. This overlay does not authorize crossing permissions, weakening controls, or silently expanding scope.

## Target accounting

Define target IDs and applicability before execution. A target is covered by an actual `passed`/`failed` result, or excluded by a technically justified `not_applicable` decision. Missing tools, access, time, or environment leave an unresolved coverage gap.

A prerequisite attempt has its own result. If it fails before the target starts, do not count the target as executed or remove it from the completeness denominator. Preserve `blocked`/`not_run` descriptions where useful while keeping the gap explicit.

## Work through gaps

Investigate the first relevant prerequisite failure, use existing supported setup paths, establish missing task-local prerequisites when authorized, rerun the original target after meaningful corrections, and continue independent targets. Retain earlier attempts and the stage reached.

Do not change assertions, delete cases, bypass isolation, or invent success to satisfy the ledger. Stop repeated attempts when they cannot resolve the concrete blocker within the available authority/environment; report that limitation.

## Completion

Remaining applicable unresolved targets make strict execution incomplete, even if the report itself is finished. An executed failing product test can still be accounted for completely. Keep execution completeness, product success, and security conclusions separate.
