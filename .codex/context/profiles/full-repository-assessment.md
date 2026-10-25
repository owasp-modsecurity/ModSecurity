# Optional full-repository assessment

## Trigger and scope

Use when the user explicitly requests a repository-wide assessment. Define whether that means source review, build/test execution, security analysis, maintainability, evidence integrity, or a stated combination. This profile does not silently add every possible scan or external integration.

The [repository map](../repository-map.md) is an inventory entry point. Establish the actual tracked tree and revisions; distinguish main-repository code, generated files, submodules, documentation, and external projects. Inventory coverage is not semantic review.

## Compose the work

- Use [all-component coverage](all-components-assessment.md) for the six core areas.
- Add [all-component security](all-components-security-assessment.md) when deep security analysis is explicitly part of the task.
- Use [build/CI/supply-chain assessment](build-ci-supply-chain-assessment.md) for selected build and delivery definitions.
- Use [evidence integrity](evidence-integrity-assessment.md) for existing and newly produced claims.
- Add [external analysis](../security/analysis-services-policy.md) or [quality scoring](../security/quality-scoring-policy.md) only when requested.
- Use [strict completeness](strict-completeness-assessment.md) only when the user explicitly selects that execution requirement.

## Coverage accounting

Record the selected directories/components, build/configuration variants, relevant tests, expected observations, actual commands, reached stage, results, and unreviewed/unexecuted areas. Include documentation, test infrastructure, and tooling when they belong to the declared scope.

A failed prerequisite is a setup result and an unexecuted target gap. Missing access or environment does not make a relevant target technically inapplicable. Continue independent authorized work while preserving the gap.

External connector behavior, external rulesets, submodule internals, and live deployments require their own selected sources and evidence. Do not import the reference repository's host-family or CRS/MRTS matrix.

## Output and completion

Produce a useful report rather than empty placeholder files: scope/inventory, coverage table, source observations, executed results, candidate dispositions, limitations, and remaining steps. Record task-owned artifacts and cleanup where execution created them.

Completion means the declared assessment scope was fully accounted for. It does not imply that all product checks passed, all vulnerabilities were excluded, or production use is approved.
