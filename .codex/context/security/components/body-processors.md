# Body-processor review

## Source entry points

`appendRequestBody()` buffers bytes and applies the request limit; `processRequestBody()` handles format parsing and the later body-access/phase decision. Multipart and URL-encoded types are recognized in header handling, while JSON/XML selection uses processor actions. Sources: [append](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971), [body processing](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L674-L871), [header selection](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L576-L589), [JSON selection](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/ctl/request_body_processor_json.cc#L28-L35), [XML selection](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/ctl/request_body_processor_xml.cc#L28-L35).

JSON and XML processing depend on `WITH_YAJL` and `WITH_LIBXML2` in this path. Response-body evaluation has its own access, limit, and MIME conditions. Sources: [request body](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L674-L871), [response body](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1117-L1230).

## Review questions

- Which exact bytes and explicit lengths reach each append call, and how are empty or split inputs handled?
- Is the processor actually selected and compiled in? What variables and errors result from malformed input?
- At what point are engine state, body access, depth, total-size, or non-file-size conditions checked?
- Does partial processing change which bytes a rule sees, and does the caller separately examine intervention state?
- Are parser completion, error cleanup, temporary files, and a following legitimate transaction covered by the selected case?
- Is a claimed response result limited to inspected data, or supported by a separately scoped connector test?

An append return alone is insufficient: a reject branch may set an intervention while returning `true`. Source: [request append branches](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L929-L971).

## Select relevant checks

[Normalization](../checks/http-normalization.md), [resource bounds](../checks/resource-exhaustion.md), [memory lifecycle](../checks/memory-lifecycle.md), [rule/control bypass questions](../checks/modsecurity-bypass.md), [response intervention](../checks/response-intervention.md), [failure/recovery](../checks/runtime-failure-recovery.md), and [file/storage boundaries](../checks/filesystem-storage.md).
