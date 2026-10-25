# Optional external analysis services

Use this guide only when external analysis is requested or is already part of the authorized task. This file does not establish that any service, plugin, CLI, endpoint, account, or license is available.

## Before use

Identify the actual configured access path, supported operation, selected source revision, and authorized scope. Use current official service guidance when needed. Do not invent a TAC daemon, scanner command, run identifier, SonarQube project, or Quality Gate. Do not upload code or data beyond the task's authorized scope.

Keep credentials, private request bodies, raw customer logs, and unrelated files out of analysis inputs and retained output. Use the smallest relevant source scope that the requested analysis needs.

## Record and reconcile

For each service actually used, record its identity, invocation, revision/branch binding, analyzed files, returned run or request identifiers when available, candidate results, and limitations. A successful upload or authentication step is not a completed analysis.

Keep service findings, compiler diagnostics, source reasoning, tests, and runtime observations as separate evidence sources. Validate candidates with [finding validation](checks/finding-validation.md); deduplicate only after preserving provenance and independently reachable instances.

A clean result does not disprove an independently demonstrated issue. A disagreement requires inspection of the underlying source path and conditions. External analysis does not substitute for a required build, test, or separately scoped connector result.

If a requested service cannot run, record the exact access or environment gap and continue the independent work that remains authorized. Do not silently claim another service provided equivalent coverage.
