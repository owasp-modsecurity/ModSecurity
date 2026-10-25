# Shared procedure for an optional deep core review

## Trigger and scope

Use this procedure when the user requests a deep security review of the core library or selected components. The default assessment mode is analysis: source reading, scoped checks, and evidence. Product fixes, dependency changes, and external integration work follow the actual user request rather than being implied by this document.

Record the working commit, pinned upstream baseline, dirty state, selected [components](components/README.md), relevant build features, and excluded areas. Submodule gitlinks identify separate revisions; do not claim their contents were reviewed merely because the main repository records a pin.

## Recommended sequence

1. State the review question and intended invariant; identify caller-controlled input, assets, privileges, and trust boundaries.
2. Read the public entry points, concrete implementation, callers, error paths, and relevant test definitions.
3. Record applicable check modules and a bounded validation plan. Verify actual tooling and prerequisites before promising execution.
4. Investigate candidate paths, including legitimate controls and the strongest counterevidence.
5. Use the strongest suitable validation available within the task: focused existing tests, bounded temporary harnesses, or explicitly selected diagnostics.
6. Reconcile candidate status, coverage, configuration variants, and unperformed checks. Keep proposed hardening separate from implemented or validated fixes.
7. Record commands, results, limitations, task-owned artifacts, and final source changes; hand over the remaining concrete work.

## Candidate record

| Field | Required meaning for a useful candidate |
| --- | --- |
| Identity | Stable local ID, component, source revision, file, and symbol. |
| Control | Who controls the input, with what privileges and preconditions? |
| Path | Reachable entry point, relevant transformations, expected control, and sink. |
| Consequence | Observable impact and its boundary; distinguish inference from execution. |
| Counterevidence | Guards, configuration, unreachable branches, or caller contracts that may invalidate the hypothesis. |
| Validation | Actual method, legitimate control, result, tool/configuration identity, and evidence location. |
| Disposition | Candidate, validated, refuted, or unresolved, with reason and confidence. |

Use [finding validation](checks/finding-validation.md). A scanner or AI result starts as a candidate. Deduplicate common root causes while preserving independently reachable instances that need separate validation or fixes.

## Library-specific questions

- Are engine, ruleset, transaction, variable, and callback lifetimes compatible with the actual caller?
- Do the bytes and lengths tested by rules correspond to the inputs supplied by that caller?
- Which engine state, body-access setting, MIME selection, limit action, and optional processor apply?
- Is a result a method return, rule match, intervention, callback, or persisted audit record?
- Which shared backend, filesystem location, parser path, and dependency variant can the selected input reach?

These are review questions, not claims of defects. The [architecture](../architecture.md) and [C++ guide](../cpp-guide.md) provide source-linked entry points.

## Evidence and completion

A compact report may contain the scope/inventory, coverage table, candidates, command log, limitations, and proposed next steps. Split files only when useful; do not create empty report collections or assume configured external storage variables exist.

Use actual task-owned output locations and document cleanup. Keep sensitive payloads and credentials out of versioned context. Missing tooling is a coverage gap, not proof of product failure or safety.

Complete means the declared review scope was reconciled; it does not mean every check passed or every possible issue was excluded. State explicitly that the assessment is not production approval.
