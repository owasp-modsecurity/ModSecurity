# Threat model

This file is read by Anthropic's OSS Scanner before it analyses libModSecurity (ModSecurity v3,
branch `v3/master`). It complements [`SECURITY.md`](../SECURITY.md) (how to report) and
[`README.md`](../README.md) (build and test).

## What this project does and where untrusted input enters

libModSecurity is a Web Application Firewall engine, a C++17 library with a C API. It parses
the SecRules language (SecLang) and evaluates rules, normally the OWASP CRS, against HTTP
transactions handed to it by a **connector**: a web server module such as ModSecurity-nginx,
which lives in its own repository. There is no web server code in this repository.

A connector creates one `modsecurity::ModSecurity`, loads one or more `RulesSet`s and shares
them across worker threads and processes. It creates one `Transaction` per HTTP transaction
and feeds it through `processConnection`, `processURI`, `addRequestHeader`,
`processRequestHeaders`, `appendRequestBody`/`requestBodyFromFile`, `processRequestBody`,
`addResponseHeader`, `processResponseHeaders`, `appendResponseBody`, `processResponseBody`
and `processLogging`. Between the steps it asks `intervention()` whether to block. The
`msc_*` functions in `headers/modsecurity/*.h` are a C façade over the same objects.

**Every byte of the request and of the response is attacker-controlled**: the method, URI,
query string, protocol version, headers, cookies, the request body in every format we
parse (urlencoded, multipart/form-data, JSON, XML, raw), and the response headers and body.
The attacker also controls how that input is split into chunks, encoded, repeated and
ordered.

**Rules and configuration are trusted.** SecLang files, directive and operator arguments,
file paths (`@pmFromFile`, `@ipMatchFromFile`, `@inspectFile`, `SecAuditLog`, ...), Lua
scripts (`SecRuleScript`, `exec`), `SecRemoteRules` sources and the `ctl:` actions in a rule
are the operator's choice. A finding that requires the attacker to write or alter rules or
configuration is not a vulnerability. A finding where a *valid* rule or directive silently
does less than documented (a limit that is accepted but not enforced, a variable or
collection key that never matches, a parse divergence) is in scope.

The connector is semi-trusted: it passes the client and server addresses and the
transaction boundaries. An API contract that makes a reasonable connector unsafe (for
example a lifetime or ownership rule that is easy to get wrong) is in scope; bugs in a
specific connector are out of scope here.

## Components that matter most / least

Most, in this order:

1. `src/transaction.cc` and the variable storage (`src/variables/`,
   `headers/modsecurity/anchored_set_variable.h`, `anchored_variable.h`, and
   `TransactionAnchoredVariables` in `transaction.h`): argument extraction, header and cookie parsing,
   the argument and body limits, interventions.
2. `src/request_body_processor/` (`multipart.cc`, `json.cc`, `xml.cc`): parser
   disagreements with real backends (PHP, Java, Python, Go, Node.js) are the classic WAF
   bypass. Every processor must honour `SecRequestBodyLimit`, `SecRequestBodyNoFilesLimit`,
   `SecRequestBodyJsonDepthLimit` and `SecArgumentsLimit`, and must set `REQBODY_ERROR`
   when it cannot fully parse a body.
3. `src/actions/transformations/` and `src/operators/`: each transformation must be a total
   function of its input; each operator must be bounded in time and memory for any input.
   `@rx` uses PCRE2 bounded by `SecPcreMatchLimit`; custom scanning loops and the
   Aho-Corasick and IP trees in `src/utils/acmp.cc` and `src/utils/msc_tree.cc` are not.
4. `src/actions/`: the actions a rule runs, each evaluated per transaction against attacker
   data:
   - disruptive actions (`src/actions/disruptive/`: `deny`, `drop`, `redirect`, `allow`,
     `pass`) and `block`: an intervention that is lost, overridden or applied to the wrong
     phase is a bypass;
   - `ctl:` (`src/actions/ctl/`): runtime changes to the engine, body processors and rule set
     (`ruleEngine`, `requestBodyAccess`, `requestBodyProcessor`, `ruleRemoveById`,
     `ruleRemoveTargetById`, ...), which must stay scoped to the current transaction;
   - `setvar`, `expirevar`, `initcol`, `setuid`/`setsid`/`setrsc`, `capture`, `exec`,
     `setenv`: they write attacker data into transaction and persistent collections, and
     their macro expansion (`%{...}`) of attacker data must not be re-interpreted;
   - `msg`, `logdata`, `tag` and the other metadata actions: macro expansion into the logs;
   - flow control (`chain`, `skip`, `skipAfter`, `multiMatch`, `phase`).
5. `src/rule_with_actions.cc`, `src/rule_with_operator.cc`, `src/rules_set.cc`,
   `src/rules_set_phases.cc`: rule evaluation, chains, `skip`/`skipAfter`, phases,
   multi-match. `RulesSet` and every `Rule` are shared, read-only state during traffic: a
   write to them from a transaction is a data race and a bug.
6. `src/collection/backend/` (in-memory and LMDB persistent collections), `src/audit_log/`
   (attacker bytes reach log files and the HTTPS writer; format and CR/LF injection are in
   scope), `src/utils/` (decoders, base64, URL and HTML entity decoding).
7. `src/parser/`: configuration is trusted, but silent misparses of valid rules are in
   scope.

Less: `examples/` (small drivers), `tools/rules-check/`, `test/` (the test harness itself),
`build/`, `doc/`.

Out of scope: third-party code in the submodules `others/libinjection` (report to
github.com/libinjection/libinjection) and `others/mbedtls`, `bindings/python`, the CRS's own
detection logic (github.com/coreruleset/coreruleset), and ModSecurity v2 (the `v2/master`
branch, enrolled separately).

## How to exercise it

- `make check` runs everything listed in `test/test-suite.in`. Two runners, both driven by
  JSON test cases:
  - `test/regression_tests` runs full transactions from `test/test-cases/regression/*.json`
    and `test/test-cases/secrules-language-tests/`: client, server, request, response,
    `rules`, and `expected` (`http_code`, a `debug_log` substring, ...). Run one file with
    `cd test && ./regression_tests test-cases/regression/<file>.json`, or one case with
    `<file>.json:<n>`.
  - `test/unit_tests` runs single operators and transformations from
    `test/test-cases/unit/*.json`.

  A new JSON file is only picked up after adding a `TESTS+=` line to `test/test-suite.in`.
- **A reproducer in the regression JSON format is what we need in a report**: it is the
  reproduction language of this project, and it becomes the regression test of the fix.
- `tools/rules-check/modsec-rules-check <file>` loads a configuration and reports parse
  errors. `examples/` contains small C and C++ connectors (`simple_example_using_c`,
  `using_bodies_in_chunks`, `multithread`, ...).
- `modsecurity.conf-recommended` is the configuration we expect in production, including
  the limits and the request body backstop rules 200002 and 200007. Score against it, not
  against a bare `SecRuleEngine On`. The OWASP CRS v4 (the latest release when the image was built: `git -C /opt/coreruleset describe --tags` shows which one; name it in any report whose result depends on the CRS) is in
  `/opt/coreruleset`: include `crs-setup.conf.example` and `rules/*.conf` after
  `modsecurity.conf-recommended` to check a finding against a default CRS deployment.
- The image is built without sanitizers. Every dependency is installed, so an ASan build
  works offline: `make distclean && ./configure --enable-assertions=yes --with-lmdb
  CFLAGS="-g -O1 -fno-omit-frame-pointer -fsanitize=address,undefined"
  CXXFLAGS="<same>" LDFLAGS="-fsanitize=address,undefined" && make -j$(nproc) && make check`.
  Use `ASAN_OPTIONS=detect_leaks=0` unless the finding is a leak. `gdb` and `valgrind` are
  installed.

## How you rate severity

We score with CVSS 3.1 and re-score every report during triage.

- **Memory corruption** (buffer overflow, use-after-free, double free, out-of-bounds read
  or write) reachable from request or response data under `modsecurity.conf-recommended`
  plus the CRS: High at a minimum, Critical when it is a controlled write. The library runs
  inside the web server process, so a crash takes down a worker for every site it serves.
- **WAF bypass** (a payload that a rule should inspect never reaches it, or reaches it
  altered so the rule misses, while the backend receives it intact): Scope Changed,
  Integrity Low. The impact lands on the protected application, which must still be
  vulnerable to the payload. That gives Medium for most bypasses. High only when a default
  `modsecurity.conf-recommended` plus CRS deployment at paranoia level 1 misses an entire
  attack class. A bypass that needs a non-default but reasonable configuration is scored as
  if that configuration were present; say what the precondition is.
- **Denial of service**: CPU or memory that grows beyond what the configured limits bound,
  from a single request, is Medium; High when a small request reliably exhausts a worker.
  Unbounded growth that the documented limits are meant to prevent is in scope even if the
  limit is not set by default; say which limit would have to be set.
- **Crash without memory corruption** (assertion, null dereference, uncaught exception)
  reachable from request data: Medium.
- **Log injection** into the audit or debug log: Low to Medium, depending on whether it can
  forge or hide entries.
- Anything that requires control of rules, configuration, Lua scripts or files on the
  server is not a vulnerability.

## How we want reports and patches

- A reproducer as a regression JSON test case (or a few lines of C against the `msc_*` API
  when the bug is in the API itself), the exact configuration, and the build options.
- Patches against `v3/master`, one fix per patch, with the regression case in the same
  patch and its `test/test-suite.in` line.
- **ABI**: 3.0.x releases keep binary compatibility with already-built connectors. A patch
  must not add, remove or reorder data members or virtual functions of the classes in
  `headers/modsecurity/`. If a fix needs that, say so explicitly in the report.
- **Parser**: `src/parser/seclang-parser.{cc,hh}` and `seclang-scanner.cc` are generated
  and committed. A change to `seclang-parser.yy` or `seclang-scanner.ll` must be
  regenerated (`./configure --enable-parser-generation`, then `make` and `make parser` at
  the top level; bison 3.8.2 and flex 2.6.4 are installed) and the generated files included.
- `ms_dbg` and `ms_dbg_a` compile to nothing when the library is configured with
  `--disable-debug-logs`: code with side effects must not be placed inside them.

## Anything to leave alone

- Findings in the test harness (`test/`), `examples/` or `tools/` that cannot be triggered
  through the library.
- Missing hardening flags, compiler warnings without an exploitable consequence, and
  findings that only apply to the deprecated legacy PCRE build (`--with-pcre`).
