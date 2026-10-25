# Core response-phase and intervention assessment

## Trigger

Use for a requested review or change involving response headers, response bodies, phase mapping, engine/body settings, intervention retrieval, or related logging. This is a current-code assessment; no connector migration target is assumed.

## Source baseline

SecRules `phase:3` maps to `ResponseHeadersPhase` and `phase:4` to `ResponseBodyPhase`; their internal enum values are `4` and `5`. Sources: [mapping](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72), [enum](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L97-L170).

`processResponseHeaders()` guards phase evaluation when the engine is disabled. Response-body processing has engine, body-access, MIME, and limit conditions. `processLogging()` also returns early when the engine is disabled. Sources: [headers](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L992-L1097), [body](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1117-L1230), [logging](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1304-L1346).

## Selected review

Read [body processors](../security/components/body-processors.md), [API boundary](../security/components/api-boundary.md), [response intervention](../security/checks/response-intervention.md), and [protocol boundaries](../protocol-and-hardening.md).

Record actual settings, supplied data, phase method, reached branch, rule result, intervention, and cleanup. Choose applicable normal, disabled/detection-only, MIME/body-access, partial/limit, empty, and error cases from the concrete code and test scope.

For each statement classify the evidence as current source, executed core result, proposed behavior, or external integration gap. Do not assert client-visible blocking, post-commit abort, reset, or `off`/`safe`/`strict` connector semantics without separately scoped evidence.

## Output

A table of condition, source branch, expected observation, actual check/result, and limitation. A successful append/process call alone does not replace an intervention check.
