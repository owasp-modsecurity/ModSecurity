# Concurrency and shared-state questions

Use for a selected path with shared mutable state, concurrent callers, callbacks, backend access, or lifecycle overlap. First establish whether that usage is supported by the actual API and caller; do not assume global thread safety or that one transaction may be used concurrently.

- Which object owns each mutable value, and which operations can overlap?
- What synchronization and lifetime boundaries apply to engine collections, rules, callbacks, logs, and transaction cleanup?
- Can error handling or callback re-entry observe partially updated state?
- Would an adversary control the timing in the real deployment, or does the case require unsupported caller behavior?
- Does the suspected effect survive a legitimate concurrent control and the strongest existing guard?

Use bounded, controlled diagnostics only when selected and available. Record scheduling assumptions, repetition count, tool/build identity, actual observation, and any inconclusive result.

Entry points: [engine](../components/engine.md), [logging/storage](../components/logging-storage.md), and [memory lifecycle](memory-lifecycle.md).
