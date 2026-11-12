# PR #3752: Sonar and CodeRabbit review corrections

This revision follows `29db0eb5bd3db8af82a100026e61d07db746674f` and addresses
specific findings from the supplied SonarCloud screenshots and the five open
CodeRabbit threads. It does not claim that a new Sonar scan has completed.

## Reporting correctness

- Empty native fixtures and reports containing only assertion controls are
  rejected; zero behavior cases cannot produce a successful suite result.
- A missing, non-executable or explicitly unavailable audit probe is BLOCKED,
  including controls that depend on its positive record. Real crashes,
  timeouts, assertion failures and invalid output remain FAIL.
- Request sensitivity-control failures are retained as FAIL entries while the
  completed behavior-case report is still written. Mutated expectations are
  separate copies, not changes to the original case.
- The companion instructions no longer compile the prepared checkout before
  `build.py` performs its own fresh build.

## C++ test-program maintenance

The request probe is split into protocol input, canary, transaction preparation,
body/parser execution and result-output functions. The former deeply nested
main function no longer owns the complete lifecycle. Resolved variable values
have scoped ownership; intervention strings are cleaned through ModSecurity's
matching API on normal and exceptional exits, rather than explicit raw
`delete`/`std::free` calls in the test program.

All three probes use a dedicated `ProbeError` for explicit harness failures.
Shared hex helpers use `std::byte` and preserve all 256 byte values and embedded
NULs. Invalid harness hex is rejected explicitly; this does not change the JSON
payloads supplied to the engine. Intentional empty logging callbacks are
explained, read-only selection arguments use string views, and escaped literal
fixtures use raw strings where appropriate.

Both new helper headers are distributed by `test/Makefile.am` and included in
each probe's build-manifest hashes. Runtime preflight rejects missing or changed
header evidence; moving shared code into headers does not bypass verification.
The reported Python exception tests prepare their input before entering
`assertRaises`, so only the intended operation is tested for an exception.

## Validation and remaining work

121 local harness self-tests passed, including 26 new tests for report/control
failures, zero-case rejection, header verification and C++ helper behavior.
The C++17 checks compile with `-Wall -Wextra -Werror`, exercise every byte value,
exceptional intervention cleanup and the no-YAJL exit-77 path. Existing
synthetic Git/Make/C++ build-integrity tests also pass. The complete repository
contains additional generated-submodule tests not rerun in this local subset.

Python syntax was checked. AST comparisons confirm that the request case
catalog, configuration and behavioral assertions, and audit field assertions
are unchanged. These are harness/helper checks, not a full ModSecurity build,
a complete WITH_YAJL probe execution, or a SonarQube/cppcheck result. Fresh CI and
external review must confirm the resulting head.

All seven suites, Ubuntu 26.04, dynamic Lua discovery and genuine requirements
failures are retained. No production engine source, native fixture, archived
result, analyzer suppression or XFAIL policy is changed. The existing 20 unmet
requirements are not reclassified as passes by these fixes. Review threads are
not manually resolved on the strength of local tests alone.
