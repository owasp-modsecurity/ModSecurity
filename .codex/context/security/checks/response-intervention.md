# Response inspection and intervention questions

Keep these observations separate: response data supplied; phase method reached; rule evaluated; rule matched; intervention requested; caller action; actual external HTTP result; cleanup/log outcome.

- Which engine state, response-body access, MIME selection, and size-limit action apply?
- Was the body phase reached, or did a documented early return occur?
- Does the caller check intervention independently of append/process return values?
- Are copied intervention buffers initialized, consumed, and cleaned up according to the actual API?
- If enforcement is claimed, which separately scoped caller/host evidence establishes it?

SecRules response-body `phase:4` maps to `ResponseBodyPhase`, whose internal enum value is `5`. Sources: [phase mapping](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L72), [enum](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L97-L170). Use the [engine guide](../components/engine.md) and [rules guide](../components/rules-parser.md) to inspect disabled-engine guards as well as normal execution.

No connector `off`/`safe`/`strict` target policy, post-commit abort, reset, or transport capability is inferred here. Entry points: [body processors](../components/body-processors.md), [API boundary](../components/api-boundary.md), [response-phase profile](../../profiles/response-phase-assessment.md), and [protocol boundaries](../../protocol-and-hardening.md).
