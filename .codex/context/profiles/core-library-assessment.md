# Bounded core-library behavior assessment

## Trigger

Use when the user asks to verify a library behavior, build, or API path without requesting a broad security scan. Select the component, source revision, rules/configuration, feature variants, and observable expectation.

## Read

[Architecture](../architecture.md), [C++/C guide](../cpp-guide.md), [build and tests](../build-and-test.md), the relevant [component guide](../security/components/README.md), and [evidence integrity](../security/checks/evidence-integrity.md). Add other checks only when the selected question requires them.

## Proposed sequence

1. Establish source and actual tool/dependency identity; inspect the existing test/harness and its assertions.
2. Prepare/build only the targets and feature variants needed for the requested behavior.
3. Confirm that the intended cases were discovered and executed.
4. Trace the relevant configuration/API inputs, phase conditions, outputs, and cleanup; include a legitimate control and a relevant error case when needed.
5. Record the reached stage, actual result, command/working directory, evidence, and limitations.

Use task-owned outputs and preserve unrelated changes. Do not infer a library result from build success or connector enforcement from a library intervention. A failure before a target test begins leaves that test unexecuted.

## Output

A concise table of case ID, component/configuration, expected observation, command, actual result, and remaining gap. Keep builds, tests, diagnostics, and external integrations separate.
