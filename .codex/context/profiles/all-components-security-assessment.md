# Optional security assessment across core components

## Trigger

Use only when the user requests broad/deep security coverage across the core library. It does not activate automatically for a shared-code change.

Apply [component security assessment](component-security-assessment.md) to engine, rules/parser, body processors, logging/storage, API boundary, and build/dependencies from the [component index](../security/components/README.md).

## Coverage and shared causes

Keep each component's entry points, input authority, configuration, candidate IDs, validation, and gaps visible. Follow cross-component paths such as parser-to-rule execution or body-to-variable-to-action processing where the actual source requires it.

Shared code may support shared evidence only when the report explains the common path and conditions. Preserve independently reachable instances and differing build/backend/caller assumptions.

Review external dependencies or connectors only when explicitly in scope. A gitlink, public API, or mention of a connector does not establish review of that external code.

## Execution and output

Use bounded checks and independent task-owned outputs. Missing prerequisites remain coverage gaps; do not weaken assertions to reach a passing status.

Produce a component coverage table, cross-component candidate summary, command/evidence record, limitations, and proposed next steps. No fixed scan count, universal completeness claim, or production approval follows from this profile.
