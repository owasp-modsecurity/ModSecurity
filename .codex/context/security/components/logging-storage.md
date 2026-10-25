# Logging and storage review

## Source entry points

`ModSecurity::serverLog()` supplies either callback text or a `RuleMessage`. The text path uses a local string's `c_str()` during the callback. `processLogging()` returns early with the engine disabled; otherwise it evaluates the logging phase and considers audit storage through `saveIfRelevant()`. Sources: [callback](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L193-L212), [logging](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1304-L1346), [audit decision](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/audit_log/audit_log.cc#L278-L325).

The engine creates shared collection backends with LMDB under `WITH_LMDB`, otherwise `InMemoryPerProcess`; transaction collections create a separate in-memory `TX` backend. Sources: [engine backends](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L109), [transaction collections](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/collection/collections.cc#L37-L56).

## Review questions

- Which fields can contain sensitive or attacker-controlled text, and which escaping/serialization path writes them?
- What are the effective logging/audit settings, callback properties, and backend feature macros?
- Does the callback copy data needed after it returns, and does its user context outlive every call?
- Who owns the collection, writer, path, file, and synchronization involved in the selected operation?
- What happens on a missing directory, failed write, full storage, malformed value, or cleanup path?
- Does the evidence establish a callback, an audit-selection decision, or an actually persisted record?

Do not infer universal persistence, thread safety, or audit delivery from the presence of a backend or callback. Inspect the concrete writer/backend and caller for those questions.

## Select relevant checks

[Disclosure](../checks/logging-disclosure.md), [file/storage boundaries](../checks/filesystem-storage.md), [concurrency](../checks/concurrency-races.md), [memory lifecycle](../checks/memory-lifecycle.md), [resource bounds](../checks/resource-exhaustion.md), and [evidence integrity](../checks/evidence-integrity.md).
