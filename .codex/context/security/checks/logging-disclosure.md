# Logging and disclosure questions

Select the actual callback, debug, audit, error, temporary-artifact, or report path. Identify which fields may contain externally controlled or sensitive values before collecting evidence.

- Can bodies, credentials, cookies, identifiers, rule contents, internal paths, or prior-transaction values reach this output?
- Are escaping and serialization correct for the output format, including newlines and controls?
- Which configuration, audit relevance decision, writer, callback property, and permissions apply?
- Can a caller retain data beyond a callback's lifetime or mix transaction contexts?
- What does a failed write or unavailable destination do to the observed result?

Keep redacted, bounded evidence using lengths, categories, and minimal excerpts where possible. A callback event, an audit decision, and a persisted record are distinct observations.

Entry points: [logging/storage](../components/logging-storage.md), [file/storage boundaries](filesystem-storage.md), and [memory lifecycle](memory-lifecycle.md).
