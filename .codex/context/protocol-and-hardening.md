# Library protocol boundaries and diagnostic builds

## Scope

This is an optional review guide. The core library and separately maintained connectors have different responsibilities. The public transaction API receives connection details, URI, headers, and body buffers from a caller; the upstream README places web server integration in external connectors. Sources: [API](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419), [project boundary](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L21-L46).

An API test using an HTTP-version string does not demonstrate HTTP/2, HTTP/3, ALPN, QUIC, wire framing, stream resets, or web server routing. Those claims require a separately scoped connector and transport environment with their own revisions and observed traffic. Do not multiply a core-library test plan into a fictional host or CRS/MRTS matrix.

## Response inspection and intervention

`processResponseHeaders()` checks the disabled engine state before evaluating its phase. Response-body evaluation checks engine state, body access, and MIME selection. `processLogging()` also returns early when the engine is disabled. Sources: [response headers](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L992-L1097), [response body](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1117-L1230), [logging](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1304-L1346).

`Transaction::intervention()` exposes a library decision; it does not establish whether a connector changed a status, stopped output, or reset a stream. Keep rule evaluation, match, requested intervention, caller action, and client-visible result separate. Do not import the reference connector project's `off`/`safe`/`strict` target modes as core-library configuration. Source: [intervention](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1389).

Use the [response-phase profile](profiles/response-phase-assessment.md) and [response intervention check](security/checks/response-intervention.md) when those boundaries are relevant.

## Normal and diagnostic builds

First identify the normal build contract and actual compiler. The pinned Unix build requires C++17; the Windows build also sets C++17. Sources: [configure](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63), [Windows CMake](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L104).

ASan, UBSan, TSan, fuzzing, static analysis, and hardening configurations are optional, separately recorded checks. Select flags only after reading the actual build definitions, compiler support, dependency linkage, and artifact type. Do not prescribe executable-only flags for a library or silently change a normal build into a diagnostic one.

Use separate task-owned output paths where the build supports them. Record flags, dependency variants, runtime requirements, and the actual checks performed. A diagnostic result does not substitute for a required normal build; a passing normal build does not establish sanitizer cleanliness or transport behavior.

For temporary harnesses, determine working directory, writable paths, input bounds, time/resource limits, and cleanup before execution. A core unit test does not need an invented NGINX worker identity, Envoy socket, or connector service.
