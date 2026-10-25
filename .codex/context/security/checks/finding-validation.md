# Candidate validation and impact questions

A suspicious pattern, advisory hit, scanner message, or AI suggestion begins as a candidate.

Before calling it a validated finding, establish the controlled input and privileges, reachable source path, intended invariant, missing/broken control, affected sink, observable consequence, relevant guards, and strongest counterevidence. Cite the exact revision, file, and symbol.

Select a legitimate same-boundary control and a relevant alternative input or path when useful. Prefer existing focused cases or a bounded harness appropriate to the actual API. Do not change production code simply to make a hypothesis reproducible.

Distinguish source-supported reasoning from executed reproduction. If runtime validation was not performed or is inconclusive, state that limitation explicitly. A test definition is not a test result.

Deduplicate common root causes while preserving independently reachable instances whose reachability or correction differs. Identify external ownership boundaries without assuming permission to modify them.

Use [evidence integrity](evidence-integrity.md) for result binding. These questions describe a method; they assert no existing vulnerability.
