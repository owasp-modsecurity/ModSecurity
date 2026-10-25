# All-component coverage assessment

## Trigger

Use when the user asks for coverage across the core library's main areas. This profile organizes the scope; it does not automatically request a deep security scan or every possible platform/feature combination.

Apply [bounded core-library assessment](core-library-assessment.md) to each selected row. Identify cross-component paths and justify any reuse of evidence.

| Component | Coverage question |
| --- | --- |
| [Engine](../security/components/engine.md) | Which phase/action and transaction-state behavior was examined? |
| [Rules/parser](../security/components/rules-parser.md) | Which accepted/rejected settings and construction paths were examined? |
| [Body processors](../security/components/body-processors.md) | Which format, limit, access, and error variants were examined? |
| [Logging/storage](../security/components/logging-storage.md) | Which callback, audit, collection, and file paths were examined? |
| [API boundary](../security/components/api-boundary.md) | Which C/C++ signatures, ownership, length, and cleanup contracts were examined? |
| [Build/dependencies](../security/components/build-dependencies.md) | Which toolchain, optional features, provenance, and tests were examined? |

For each row record source review, available tests, actual execution, configuration, findings/questions, and gaps separately. Listing six rows does not prove every file, function, input, or backend was covered.

Parallelize only independent work whose output paths and resources do not conflict. Do not create a connector/transport matrix for a core-only task.

Report `not_run` and `blocked` with reasons; use `not_applicable` only for a technically excluded condition, not an inconvenient prerequisite.
