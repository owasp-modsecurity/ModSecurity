# SonarQube applicability and evidence

## Conditional scope

This is a working policy for tasks that actually use SonarQube. It does not
assert that this fork has a SonarQube server, project key, scanner, connected
account, wrapper, CI analysis, or mandatory quality gate configured.

Establish applicability from the current task and actual repository, PR,
workflow, or service configuration. If SonarQube is not relevant to a
documentation-only change, do not create a new analysis setup merely to satisfy
this policy.

The pinned core Makefile provides a cppcheck-based `check-static` target.
That source-backed check is separate from any SonarQube integration.
Source: [static-analysis definition][static]. For the core build and other
available checks, use [build-and-test.md](build-and-test.md).

## Access and command boundaries

Use the supported interface available in the active environment, whether a
configured connector, CLI, scanner, or authenticated readback service. Discover
its actual capabilities before constructing a command. A query interface does
not automatically provide scanning, and a scanner does not automatically
provide administrative access.

Follow [command-execution-policy.md](command-execution-policy.md). Do not
invent a fixed wrapper path, authentication variable, server address, project
identifier, or CLI flag. If the current environment requires an authentication
wrapper, inspect its documented interface and use it without bypassing that
boundary.

Use the configured authentication mechanism. Never print tokens, dump the
environment, store credentials in versioned configuration, or include secret
values in arguments, logs, or evidence. Missing access is a specific limitation;
it is not permission to search unrelated files for credentials.

Before an analysis upload, establish its intended project/destination and
that sending the selected source or analysis data there is within task scope.
A read-only findings review does not itself authorize changing server settings
or publishing a new scan.

## Bind results to the code

For a PR decision, use results associated with the current PR head SHA.
For a local or standalone analysis, record the analyzed commit, material
uncommitted changes, build configuration, and included/excluded scope.
An analysis of an older commit remains evidence for that older input.

A scanner process exiting successfully or uploading a report does not prove
that server processing completed or that a quality gate passed. When the task
depends on those outcomes, read back the terminal analysis and gate results.
Record pending, failed, cancelled, inaccessible, or incomplete readback as
such. Do not describe a partial page of findings as the complete result set.

## Findings and gate changes

Treat reported issues as evidence requiring examination of the actual code
and execution path. Distinguish reliability issues, security findings, security
hotspots, maintainability, duplication, coverage, analysis failures, and
candidate false positives.

Determine whether a finding affects this task and revision before changing
code. Do not suppress a rule, broaden an exclusion, lower a threshold, change
a quality gate, or mark an issue safe merely to obtain a green result. A
justified configuration or disposition change is a separate reviewable action
within the applicable task authority.

An applicable required gate remains part of delivery. A gate that has not
been established as applicable must not be invented. Follow
[delivery-and-ci.md](delivery-and-ci.md) for the delivered revision and final
status.

## Evidence to retain

Record the mechanism used, repository and project identity, exact analyzed
revision/scope, analysis identifier when available, scanner exit status,
server-processing status, gate result, relevant findings, and limitations.
Retain only secret-free output needed to support the claim.

If analysis cannot proceed, state the concrete cause, such as unavailable
tooling, missing configured access, unreachable service, failed analysis,
stale revision, or incomplete readback. Continue unrelated work within scope
without replacing the missing evidence with an assumed pass.

[static]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L58-L75
