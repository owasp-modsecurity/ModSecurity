# Source inventory and historical evidence

This directory holds reproducible inventories and dated handovers. These are
historical records, not automatically loaded operational policies.

| Entry | Scope |
| --- | --- |
| [source-index.tsv](source-index.tsv) | Every entry in the pinned upstream main-repository Git tree: mode, object type, object ID, path |
| [template-source-index.tsv](template-source-index.tsv) | Selected connector-template files and their Git object IDs at the recorded reference revision |
| [sessions/README.md](sessions/README.md) | Dated session records and their evidence requirements |

A gitlink records a dependency commit, not its internal contents. An inventory is
not a claim of exhaustive review. Keep large build output, private HTTP traffic,
credentials, and raw scanner data out of versioned context. Record the command,
revision, relevant result, and limits in a concise handover instead.

Current instructions are in [context/index.md](../context/index.md); source
provenance is in [context/provenance.md](../context/provenance.md).
