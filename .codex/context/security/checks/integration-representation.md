# Optional caller/library representation comparison

Use when the selected task includes a real caller or connector and asks whether the library inspects the relevant application data. Core-only source review does not establish wire parsing or request-smuggling behavior.

- Which caller revision parses, decodes, combines, truncates, or transforms data before API calls?
- Which exact bytes, lengths, headers, and transaction boundaries reach libmodsecurity?
- Does the actual downstream application use the same representation and request identity?
- For a suspected mismatch, what intended control changes and what legitimate control remains valid?
- Which observation is library-only, and which has correlated caller/downstream evidence?

Transfer framing, duplicate length headers, trailers, HTTP/2 or HTTP/3 translation, connection reuse, and stream resets are relevant only when a real scoped integration provides those paths. Do not claim coverage from an HTTP-version string or a core API test.

Entry points: [API boundary](../components/api-boundary.md), [normalization](http-normalization.md), and [protocol boundaries](../../protocol-and-hardening.md).
