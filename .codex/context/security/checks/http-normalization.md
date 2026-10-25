# HTTP representation and normalization questions

Begin with the bytes and lengths actually supplied to the library. Trace raw and decoded URI/query values, header names/values, body values, transformations, and reported match origins in the selected path.

- Which representation does each variable or operator receive?
- What happens to percent encoding, repeated decoding, separators, duplicate arguments, controls, embedded NULs, and malformed text in the applicable path?
- Are explicit lengths, offsets, and derived values consistent after a transformation changes the data?
- Is a parser error or partial body visible in the configuration/rules used by the case?
- Does the downstream application use the same representation, or is that an unverified external boundary?

A differential matters only when its reachable consequence is established. Record input representation, derived value, responsible function, effective rule, expected/observed result, legitimate control, and remaining uncertainty.

Entry points: [API boundary](../components/api-boundary.md), [body processors](../components/body-processors.md), [rules/parser](../components/rules-parser.md), and [integration representation](integration-representation.md).
