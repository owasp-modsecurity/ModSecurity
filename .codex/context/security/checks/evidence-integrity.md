# Evidence integrity and claim binding

For each claim, record component, source revision, caller/harness, effective rules and settings, optional-feature/dependency identity, tool versions, command/working directory, observed result, and evidence location.

- Is the artifact from the reported revision and configuration, or from an earlier/mixed run?
- Were tests actually built, discovered, and executed, with meaningful cases and assertions?
- Does an exit code hide a skip, empty run, caught failure, or failed prerequisite?
- Does a generated report preserve the original observations, or introduce a stronger claim?
- Is a source statement being promoted into a runtime result, or a core result into connector enforcement?
- Does shared evidence actually cover each reported component and configuration?

Keep `passed`, `failed`, `blocked`, `not_run`, and justified `not_applicable` distinct. A prerequisite failure leaves the target unexecuted; it is not a target test failure.

Entry points: [build/dependencies](../components/build-dependencies.md), [evidence profile](../../profiles/evidence-integrity-assessment.md), and [known source discrepancies](../../fact-check.md).
