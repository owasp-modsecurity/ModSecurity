# Core component review map

This is a thematic navigation map for optional reviews, not a declaration of isolated upstream modules or completed coverage. A data path may cross several rows; record the shared boundary rather than duplicating or silently transferring results.

| Component | Main questions |
| --- | --- |
| [Engine and rule execution](engine.md) | Phase selection, engine state, rule/action control flow, transaction state. |
| [Rules and parser](rules-parser.md) | Input acceptance, properties, rule construction, chains, and phase translation. |
| [Body processors](body-processors.md) | Bytes, format selection, buffering, limits, errors, optional processors. |
| [Logging and storage](logging-storage.md) | Callback lifetime, audit decisions, collections, paths, and disclosure. |
| [C++/C API boundary](api-boundary.md) | Ownership, explicit lengths, returns, cleanup, callbacks, and caller contracts. |
| [Build and dependencies](build-dependencies.md) | Feature identity, generated sources, gitlinks, tests, CI, and provenance. |

Begin with the [selected profile](../../profiles/README.md). The [shared assessment procedure](../core-deep-assessment.md) applies only when a deep review is requested. Each component names suggested check modules; select those relevant to the actual question.

External connectors, their host-specific routes, wire transports, and external rulesets have separate source and evidence boundaries. Include them only when the task explicitly reaches those boundaries.
