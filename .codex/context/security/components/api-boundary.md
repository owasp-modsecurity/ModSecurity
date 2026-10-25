# C++/C API-boundary review

## Source entry points

Read the public headers together with the wrapper and C++ method implementations. `Transaction` stores engine/ruleset pointers without deleting those two objects in its destructor; their lifetime must cover their use. Sources: [public transaction API](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419), [C declarations](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L677-L767), [constructor/destructor](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L113-L182).

Header overloads without lengths use `strlen()`, while explicit-length overloads pass the supplied lengths to string operations. Callback text is backed by a local string during the call. Sources: [header overloads](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L618-L654), [callback](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L193-L212).

## Review questions

- Which pointers are borrowed, transferred, newly allocated, or references into another object's storage?
- Do lengths describe valid accessible buffers, and is null termination required by this particular overload?
- Does a public-header change affect callers, class layout, ABI, cleanup ownership, or C/C++ behavior?
- Does the caller distinguish loading counts, method success, parser errors, and intervention state?
- Are callback properties, user-context type, lifetime, and any retained data compatible?
- Which errors or exceptions cross the actual wrapper, and where does the caller recover?

The C constructor wrappers use `new`; do not assume a universal exception-to-error-code translation. Interventions copy buffers and reset internal state; use the corresponding cleanup path. Sources: [engine wrapper](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L411-L482), [transaction wrapper](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1804-L1829), [intervention](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L1350-L1389), [cleanup](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L2153-L2195).

## Select relevant checks

[Memory lifecycle](../checks/memory-lifecycle.md), [API sequencing](../checks/api-sequencing.md), [normalization](../checks/http-normalization.md), [integration representation](../checks/integration-representation.md), [failure/recovery](../checks/runtime-failure-recovery.md), and [finding validation](../checks/finding-validation.md).

A caller-contract violation and a reachable library defect are different hypotheses. Establish which one the actual case supports.
