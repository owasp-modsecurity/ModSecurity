# Build, dependency, and CI review

## Source entry points

Read `configure.ac`, the relevant Makefiles, platform build files, `.gitmodules`, and selected workflow definitions. The pinned Unix and Windows builds set C++17. Test utilities are enabled by the configure condition only when `YAJL_FOUND=1`. Sources: [Unix standard](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63), [Windows standard](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L104), [test utility condition](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L313-L324).

## Review questions

- Which compiler, dependency versions, feature macros, linkage mode, and actual binary belong to the claimed result?
- Are generated parser outputs consistent with their `.ll`/`.yy` inputs and build rules?
- Do submodule checkouts match the main repository's gitlink commits rather than a moving remote branch?
- Which workflow inputs can reach shell commands, paths, downloads, caches, artifacts, or credentials?
- Were the intended tests built, discovered, and executed with meaningful assertions?
- Do local and remote results belong to the same source, configuration, and artifact?

A regression runner built with `NO_LOGS` can return `0` before loading tests. The scanner Dockerfile catches `make check` failure with a warning. These are reasons to inspect execution evidence, not claims that a particular run passed or failed. Sources: [runner early return](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L437-L442), [Dockerfile test handling](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.oss-scanner/Dockerfile#L29-L37).

## Select relevant checks

[Supply chain](../checks/supply-chain.md), [configuration/input injection](../checks/config-injection.md), [advisory research](../checks/advisory-research.md), [disclosure](../checks/logging-disclosure.md), and [evidence integrity](../checks/evidence-integrity.md).

Use [build and test guidance](../../build-and-test.md) for actual repository commands. Inventory submodule contents and external packages only when in scope; their existence or pin does not establish a completed dependency audit.
