# Configuration, command, and path input questions

Trace the selected externally influenced value from its real source to parser, property, command, path, serialization, or build step. Identify who can set it; a rule author, CI contributor, and HTTP client do not automatically have the same authority.

- Does recognition lead to an implemented setting, an explicit error, or an empty compatibility action?
- Where are quotes, escapes, newlines, control characters, option prefixes, substitutions, or path components interpreted?
- Does validation cover the representation actually used at the sink?
- Can environment or search-path fallback select a different binary, library, rule file, or output location?
- Is shell evaluation involved at all, or does the API pass an argument vector?

Use a bounded legitimate value and a focused negative value at the same boundary. Quoting alone is not evidence of the whole path's behavior. Record the actual parse/execute/write outcome and privilege assumptions.

Entry points: [rules/parser](../components/rules-parser.md), [build/dependencies](../components/build-dependencies.md), and [known source discrepancies](../../fact-check.md).
