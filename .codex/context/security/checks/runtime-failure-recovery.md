# Failure, cleanup, and follow-up questions

Select failures reachable in the actual library/harness: invalid rules, unavailable optional processing, malformed bodies, limit handling, failed file/backend operations, callback problems, or caller error handling. Verify the concrete failure mechanism rather than assuming it exists.

- What return value, error text, variable, intervention, or exception carries the failure?
- Does the caller preserve that signal or convert it into apparent success?
- Which transaction/shared resources are left after the failure?
- Does cleanup occur once with the correct owner, and is a legitimate follow-up case unaffected?
- Did a later success follow a setup correction, or is there unexplained intermittent behavior?

Record the original failure and every meaningful correction. A successful retry does not erase the earlier observation. Host/service restarts or network timeouts belong here only when a separately scoped integration actually supplies them.

Entry points: [API boundary](../components/api-boundary.md), [body processors](../components/body-processors.md), [logging/storage](../components/logging-storage.md), and [evidence integrity](evidence-integrity.md).
