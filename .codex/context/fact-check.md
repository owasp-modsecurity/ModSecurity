# Fact check: known pitfalls and source discrepancies

## Scope and use

Inspected source revision: `owasp-modsecurity/ModSecurity`, `v3/master`, **`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`**.
This overview describes differences between comments, examples, build rules, and implementation. It makes no claims of security vulnerabilities or executed runtime tests.
The **Check** paragraphs are our own recommendations for new chats. The **Finding** paragraphs are based on the linked sources at this commit.

## 1. The README excerpt does not fully match the current C++ API

**Finding:** The README example uses `ModSecurity::Rules`, a transaction constructor with two arguments, `processConnection()` with only one address, and `intervention()` without an output parameter. The current headers declare `modsecurity::RulesSet`, constructors with `logCbData`, four connection parameters, and `intervention(ModSecurityIntervention *)`. Sources: [README example][readme-api], [RulesSet][rules-api], [transaction constructor and connection][transaction-args], [intervention][intervention-api].

**Check:** Was the signature checked directly in the header and implementation, or copied only from the README? For C++ examples, consult the actual sources under `examples/` and [cpp-guide.md](cpp-guide.md).

## 2. The test programs are under `test/`

**Finding:** The README's compilation section says `tests/`. The top-level Makefile actually includes `test` in the build; that directory's Makefile defines `unit_tests` and `regression_tests`. Sources: [README spelling][readme-tests], [build subdirectories][root-make], [unit test target][unit-target], [regression test target][regression-target].

**Check:** Are the program name, test file, and working directory correct? The direct example commands in [build-and-test.md](build-and-test.md) start inside `test/`.

## 3. The evidenced language standard is C++17

**Finding:** `configure.ac` requires `AX_CXX_COMPILE_STDCXX(17, noext, mandatory)`. The Windows build also requires C++17. The standard is established by the build definitions, rather than estimated from the project's age or isolated syntax. Sources: [Unix requirement][cpp-standard], [Windows requirement][cpp-standard-win].

**Check:** Is a proposed language construct compatible with this revision and the compiler actually in use? For a different commit, read its build definitions again.

## 4. YAJL: the README requirement differs from the Unix build logic

**Finding:** The README calls YAJL mandatory. The YAJL macro instead uses generic library detection, whose automatic search path continues when the library is missing. Only an explicitly mandatory request causes a configuration error there. `TEST_UTILITIES` is enabled only when `YAJL_FOUND=1`. Sources: [README][readme-dependencies], [YAJL macro][yajl-macro], [detection logic][find-library], [test condition][test-condition].

**Check:** Was YAJL detected, and were the test programs actually built? For an intended test build, `--with-yajl=yes` is the variant chosen in this working documentation. A successful library build alone does not establish that test programs are present.

## 5. Exit code 0 can occur without running regressions

**Finding:** With `NO_LOGS`, `regression_tests` prints a message about missing logging support and immediately returns `0`. This path occurs before argument processing and test loading. Source: [NO_LOGS branch][no-logs].

**Check:** Which cases actually ran? Record case counts, skips, disabled cases, and runner messages as well as the exit code. This finding does not imply that other build variants pass their tests.

## 6. LMDB is off by default in the Windows CMake build

**Finding:** The Windows README lists LMDB among the features enabled by default. However, `build/win32/CMakeLists.txt` sets `WITH_LMDB` to `OFF`; Lua, libxml2, MaxMind, and curl are set to `ON`. Sources: [README feature list][windows-readme], [CMake options][windows-options].

**Check:** Which CMake options were actually set? A Conan dependency or the general README list does not establish that the corresponding feature is active in the binary.

## 7. Recognized, rejected, and ineffective are different states

**Finding:** The grammar treats the following known directives differently. This table describes the respective parser branches; it is not a complete v2/v3 compatibility list.

| Directive | Evidenced behavior in the grammar branch |
| --- | --- |
| `SecRequestBodyInMemoryLimit` | Reports missing support and triggers `YYERROR`. [Source][memory-limit] |
| `SecPcreMatchLimitRecursion` | Has no executing action there; the error message is commented out. The neighboring `SecPcreMatchLimit` does set a property. [Source][recursion-limit] |
| `SecTmpDir`, `SecDataDir`, `SecStatusEngine` | The shown branches make no corresponding value assignment; error messages are commented out. [Source][compat-directives] |

**Check:** What happens after the token is recognized? Trace the grammar action, stored property, and runtime access. Neither "every unimplemented effect produces an error" nor "successful parsing means effective behavior" is a reliable conclusion here.

## 8. An operator file does not establish availability in SecRules

**Finding:** `Operator::instantiate()` recognizes `gsblookup`, `rsub`, and `validatehash`. However, the SecRules grammar explicitly rejects `@gsbLookup`, `@rsub`, and `@validateHash` with `OPERATOR_NOT_SUPPORTED`; the macro reports an error and executes `YYERROR`. The corresponding `evaluate()` implementations contain TODOs and immediately return `true`. Sources: [factory][operator-factory], [grammar branches][operator-grammar], [error macro][unsupported-macro], [GsbLookup][gsb-stub], [Rsub][rsub-stub], [ValidateHash][hash-stub].

**Check:** Was the normal SecRules loading path inspected? The existence of a class or factory entry alone does not justify saying that the operator is available for use in rules. No exploitable security effect is inferred here either.

## 9. Internal phase values are not SecRules phase numbers

**Finding:** The enum contains `ConnectionPhase`, `UriPhase`, `RequestHeadersPhase`, and other phases. `Phase::init()` translates `phase:1` to `RequestHeadersPhase`, whose internal value is `2`. It translates `phase:0` to `ConnectionPhase`. `processURI()` populates variables and does not invoke evaluation of `UriPhase` in the inspected function body. Sources: [enum][phase-enum], [mapping][phase-action], [URI function][uri].

**Check:** Does a number come from SecRules, an internal enum, an API, or a log representation? The full mapping is in [architecture.md](architecture.md).

## 10. This repository describes the library

**Finding:** The upstream README distinguishes libmodsecurity from the separately maintained web server connectors. The library processes HTTP data supplied by its caller. The examples and public transaction methods demonstrate API use, but do not establish a complete integration with a particular Nginx, Apache, or IIS version. Sources: [project scope][scope], [transaction API][transaction-public].

**Check:** Does the behavior under investigation belong to the library or the connector? For an external connector, request its own source and version, or inspect them within the authorized scope. Do not equate a library intervention with an HTTP response actually sent without further evidence.

## 11. Submodule versions come from gitlinks

**Finding:** `.gitmodules` lists paths and URLs. The following commit pins were read from the Git tree of the pinned main repository commit, rather than from branch names or README example output. Sources: [.gitmodules][gitmodules], [pinned upstream tree][source-tree].

| Submodule path | Gitlink commit |
| --- | --- |
| `bindings/python` | `bc625d5bb0bac6a64bcce8dc9902208612399348` |
| `others/libinjection` | `211782219663f889f471650150df12b623c5766e` |
| `others/mbedtls` | `0fe989b6b514192783c469039edd325fd0989806` |
| `test/test-cases/secrules-language-tests` | `f73c73027ef49ccf99c7911744929a71d15ff419` |

**Check:** Do the checkout and [upstream-lock.json](upstream-lock.json) match these gitlinks? The mapping can be reproduced with `git ls-tree -r effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`. A pin alone establishes neither that submodules are initialized nor that their file contents have been reviewed.

## 12. The scanner image does not enforce a passing test run

**Finding:** `.oss-scanner/Dockerfile` stops on errors in its build chain, but then handles `make check` with `|| echo "WARNING: make check reported failures"`. A failure of that test command therefore need not fail the image build. The comment before the test line also describes this behavior. Source: [build and test steps][scanner-docker].

**Check:** Does the evidence show "image built" or "tests passed"? Evaluate test output and the original test status separately; do not record a successful Docker build as a passing test suite.

## Maintaining this list

**Our recommendation:** When moving to a newer source revision, read the affected implementations again. Change or remove entries only with traceable sources; do not carry resolved discrepancies into new chats. For additional observations, always record the finding, source, practical significance, and remaining uncertainty.

## Sources at the pinned commit

[readme-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L233-L255
[rules-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/rules_set.h#L38-L77
[transaction-args]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L350
[intervention-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L398-L403
[readme-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L53-L54
[root-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L1-L17
[unit-target]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L29-L35
[regression-target]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L93-L99
[cpp-standard]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cpp-standard-win]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L102-L104
[readme-dependencies]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L101-L106
[yajl-macro]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/yajl.m4#L10-L11
[find-library]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/msc_find_lib.m4#L48-L97
[test-condition]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L313-L324
[no-logs]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L437-L442
[windows-readme]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/README.md#L64-L74
[windows-options]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L1-L9
[memory-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1655-L1663
[recursion-limit]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1696-L1707
[compat-directives]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L1748-L1785
[operator-factory]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/operator.cc#L148-L181
[operator-grammar]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L992-L1026
[unsupported-macro]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/seclang-parser.yy#L298-L300
[gsb-stub]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/gsblookup.cc#L26-L32
[rsub-stub]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/rsub.cc#L26-L32
[hash-stub]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/operators/validate_hash.cc#L25-L31
[phase-enum]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/modsecurity.h#L108-L170
[phase-action]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/actions/phase.cc#L25-L66
[uri]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/transaction.cc#L337-L443
[scope]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L21-L46
[transaction-public]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h#L335-L419
[gitmodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
[source-tree]: https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729
[scanner-docker]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.oss-scanner/Dockerfile#L29-L37
