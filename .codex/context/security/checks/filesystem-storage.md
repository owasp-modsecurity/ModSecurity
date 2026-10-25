# File, path, and storage boundary questions

Use only for actual file/storage operations reached by the selected core path, such as rule loading, audit output, collection storage, or temporary-file handling. Read the concrete implementation and its configured path before selecting cases.

- Who supplies the path, which directory resolves it, and whose privileges perform the operation?
- Are creation, open, read, write, rename, deletion, and cleanup applied to the intended object?
- What containment, permissions, symlink/replacement, length, and error checks exist at the point of use?
- Is data size bounded, and what happens on short reads/writes or unavailable/full storage?
- Are temporary resources released without deleting another task's or caller's files?

A recognized configuration directive is not proof that a path setting is applied. Do not import private UDS/peer-credential assumptions from connector services into a library path that does not use that service.

Record actual file identity, relevant metadata, bounded evidence, and cleanup for any executed test. Entry points: [rules/parser](../components/rules-parser.md), [body processors](../components/body-processors.md), and [logging/storage](../components/logging-storage.md).
