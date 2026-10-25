# API call sequencing and transaction state

Use for the actual C or C++ caller/harness. Read its setup, phase calls, intervention handling, logging, and cleanup together with the implementation.

- When are connection, URI, headers, and body chunks supplied relative to phase evaluation?
- Which early returns and engine/body-access conditions apply?
- Does the caller distinguish incomplete input, append limits, processing results, and intervention state?
- Are repeated calls, missing steps, error paths, and cleanup supported by the documented/implemented contract, or merely assumptions of the harness?
- Are per-transaction IDs and user callback contexts kept separate across actual caller lifetimes?

Record the exact sequence and state observed at each relevant step. A robustness test of an unsupported call order must not be reported as ordinary supported behavior.

Do not import SPOE, gRPC, UDS frames, response companions, or host stream lifecycle into this library checklist. Those require their own implementations and scope.

Entry points: [API boundary](../components/api-boundary.md), [engine](../components/engine.md), and [architecture](../../architecture.md).
