# Evidence and claim-integrity assessment

## Trigger

Use when the user asks whether existing documentation, test reports, coverage, scanner results, or runtime claims are trustworthy. The task is to evaluate evidence, not to assume a new full scan is required.

## Read

[Evidence integrity check](../security/checks/evidence-integrity.md), [provenance](../provenance.md), [known source discrepancies](../fact-check.md), and the relevant [component guides](../security/components/README.md).

## Reconcile

Bind claims to working/upstream revisions, submodule pins when relevant, actual binaries, effective rules/settings, feature macros, dependency/tool versions, case IDs, command/working directory, timestamps/run identity when available, and raw observations.

Look for stale or mixed results, missing case execution, source/build-to-runtime promotion, swallowed failures, unsupported interpretation of exit codes, and generated reports with no traceable input.

An absent artifact does not prove a product defect. An existing artifact does not prove fresh execution. Distinguish unavailable evidence, contradicted claims, and evidence sufficient only for a narrower conclusion.

## Output

For each material claim, record the original wording, supporting evidence, verified boundary, discrepancy/gap, and corrected conclusion. Do not rewrite historical results as if they were new runs.
