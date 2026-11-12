# JSON/YAJL: reproducible characterization of ModSecurity v3

> Complete English translation of the historical baseline published at fork commit `2bfb2180d8c0fa35ae7a3f2665e999600607d896`. Historical counts, paths and publication statements are preserved; subsequent extensions have separate reports.

This investigation exclusively uses the official production state
[`8271c9d3bc0abcbd1189f5414c196c336c959047`](https://github.com/owasp-modsecurity/ModSecurity/commit/8271c9d3bc0abcbd1189f5414c196c336c959047).
The tests and this documentation are on the new fork branch
`test/json-yajl-baseline-v3-20261015`, locally at
`/workspace/work/json-yajl-baseline/v3`. Production code was not changed.
The original reference, working branch and later publication SHA are different
things; the SHA checked after pushing appears in the final report so this
document does not need a succession of self-referencing commits.

The [shared introduction and results index](JSON-YAJL-BASELINE.md),
[provenance manifest](../json-yajl-provenance.json) and
[code analysis including verification of statements](json-yajl-code-analysis.md)
complement the executable v3 matrix here. A passing characterization test means
"this actually happened here", not "this behavior must be retained forever".

## Test levels and reliable observation

1. The existing regression tests use the unchanged project runner.
   The file `request-body-parser-json.json` was extended with three suitable rules
   making exact assertions about number text, duplicate arguments, ARGS_NAMES,
   ARGS_POST, depth and error flags. The original six cases are preserved.
2. `test/json-baseline/probe.cc` uses the actual public `Transaction` interface:
   connection, URI/query, headers, phase 1, `appendRequestBody`,
   `processRequestBody`, phase 2 and interventions. The test helper reads existing
   variable structures; it adds no public production interface. Evaluation uses
   lists of byte pairs and a multiset comparison: duplicates and their frequency
   are preserved, while the iteration order of an `unordered_multimap` is not
   specified.
3. For real parser chunks, the existing internal ModSecurity class
   `RequestBodyProcessor::JSON` is used with a real Transaction. This tests the
   ModSecurity callbacks rather than just direct YAJL calls. It does not replace
   an HTTP connector check. An array of text lines in the project test format
   explicitly does not demonstrate multiple parser calls.
4. `audit_probe.cc` goes through the request/response lifecycle and calls
   `Transaction::toJSON`. `run_audit.py` structurally checks the generated record.
   This is a separate test of the writing side. The existing 15 audit tests
   additionally check the existing audit writer/file path.

Before every actual probe, the observer contains a real JSON canary:
`{"canary":"active"}` must arrive as `json.canary=active`, without parser errors.
Missing YAJL support causes exit 77/BLOCKED. The `whoAmI` banner alone contains no
proof of YAJL capability. The actually loaded library was checked using `ldd`
on `v3/src/.libs/libmodsecurity.so.3`. This is particularly important because an
older preinstalled fork library is present in this environment.

Inputs reach the observer as hexadecimal text and are converted there to bytes
with explicit lengths. No additional newline is appended to the body.
Content-Length is calculated from `body.size()`. The test report's JSON is only
the container for hex bytes, not a parser that rewrites payload numbers or
duplicates. Names and values, including NUL, are compared as hex bytes. ARGS,
ARGS_NAMES, ARGS_GET and ARGS_POST are checked separately. Native rules and case
K-rule-value additionally demonstrate rule access with `t:none` and, where
applicable, `t:hexEncode`.

Timeouts, crashes and protocol errors are reported as FAIL with the cause.
A missing executable or the explicit YAJL canary gate is BLOCKED. Intentional
syntax rejection is a successful negative test when both expected error flags,
the error class and the exact remaining argument list match. An empty test
filter returns exit 2. Four deliberately incorrect expectations must fail:
NUL key, original number text, duplicates and completion error.

## The ten mandatory groups

Every ID is stable. The exact bytes, rules, headers, configuration, expectations
and status of each case appear in [run.py](../../test/json-baseline/run.py) and
the [machine-readable run report](../../test/json-baseline/results/v3-request-results.json).
Expectations are fixed in the source and are not generated from output during
the test run. The report also contains the length, SHA-256 and hex bytes of each
payload. Large synthetic values are limited to 4 KiB, keys to 256 bytes and the
larger array to 128 scalar elements.

| Group | IDs / cases | Specifically checked properties |
|---|---|---|
| A | A-* / 6 | No append call, append with zero bytes, the four permitted whitespace characters, `{}`, `[]`, `""`; selection, arguments and flags. |
| B | B-* / 9 | Empty keys, empty siblings, nesting, dot/path collision, backslash, Unicode and NUL with full length. |
| C | C-* / 14 | String/empty string, integer, decimal, true, false, null; each also with whitespace. |
| D | D-* / 9 | Root array, nested arrays/objects, multiple dimensions, empty slots, siblings and an example that can be followed step by step. |
| E | E-* / 16 | Object/array/mixed form with L=3: L−1/L/L+1; root counts as 1; partial result; 0/scalar; default; three invalid configurations separately. |
| F | F-* / 16 | L−1/L/L+1 for objects and arrays, callback cancellation for all four scalar types, empty containers, duplicates, query+JSON, limit 0. |
| G | G-* / 20 | UTF-8 with 2/3/4 bytes, escapes/surrogate pair, eight real binary cases in keys/values, escape syntax/control characters, lone surrogates and NUL. |
| H | H-* / 27 | All 21 requested number tokens, including a 96-digit integer, unchanged; six actual syntax counterexamples. |
| I | I-* / 5 | Equal/different duplicate values, empty and nested names, and case sensitivity; interaction with the limit under F. |
| J | J-* / 8 | Trailing whitespace, garbage, two documents and five truncated token/structure types; partial arguments and flags. |

The additional groups K (22 cases) and L (7 cases) check processor selection,
Content-Type variants, BodyAccess/rule modes, explicit rejection, rule access,
body limits at/below/above the boundary, NoFilesLimit, longer data, connector
handoffs and transaction isolation. The isolation case first uses a syntax error
and then a successful array request with the same engine and rules. Both
transactions are checked individually for arguments, paths/indices, error
message and error flags.

L controls real parser chunks at number, UTF-8 and escape boundaries, errors in
`processChunk` versus errors in `complete`, and the initially withheld root
number token `1`: no argument before complete, then `json=1`. The empty parser
run has a successful intermediate call and a failing completion. For an empty
body, the Transaction nevertheless reports 0 because its code explicitly
suppresses the error message when body length is 0.

## Important concrete results

- A small object `{"a":1,"b":false,"c":null}` produces `json.a="1"`,
  `json.b="false"`, `json.c=""`. In v3, JSON fills ARGS and ARGS_NAMES,
  **not ARGS_POST**. Query arguments remain in ARGS_GET without a `json.` prefix.
- `{"items":[["A","B"],"C"],"tail":"D"}` produces
  `json.items.array_0.array_0=A`, `json.items.array_0.array_1=B`,
  `json.items.array_1=C`, `json.tail=D` (D-walkthrough). Empty containers consume
  an array position but do not create an argument themselves.
- `{"a.b":1}` and `{"a":{"b":1}}` produce the same name, `json.a.b`.
  If both forms are combined, both values are preserved. An empty object key
  receives `empty-key`; this can also collide.
- All number tokens remain unchanged, including `1E+02`, `1.2300`, large
  integers, `1e400` and `1e-400`. They are not reformatted through integer/double
  conversion.
- The argument limit checks `size >= limit` before insertion. Query and JSON
  share the limit; the excess value is not stored. Boolean and null callbacks
  also propagate cancellation in the current original.
- Arguments already created survive syntax or limit errors. `{"a":1,` has
  `json.a=1`, but both parser error flags are 1. A rule must decide whether to
  reject it; K-explicit-deny explicitly demonstrates status 422.
- `SecRequestBodyAccess Off` does not prevent JSON parsing here for bytes already
  supplied through the API: the guard comes after the parser block. Phase 2 and
  REQUEST_BODY preparation are skipped. This distinguishes it from the usual
  connector decision not to supply disabled bytes at all; the test makes no
  claim about that behavior in a particular web server.
- Here `SecRequestBodyNoFilesLimit` sets REQBODY_ERROR and INBOUND_DATA_ERROR to
  1, but leaves REQBODY_PROCESSOR_ERROR empty and skips the parser. This path
  explicitly differs from JSON syntax rejection.
- The Reject body limit generates the intervention during the append call.
  A connector must check for it there. ProcessPartial retains the appropriate
  prefix length; when a document is shortened by its closing brace, `json.x=1`
  may already exist even though completion fails.

### Character processing is not an already agreed target specification

YAJL 2.1.0 rejects lone continuation bytes and truncated sequences. In this
distribution, however, it accepts `c0 af` (overlong encoding) and `f4 90 80 80`
(above U+10FFFF) in names and values. These bytes are preserved. A lone `\ud800`
becomes `?`, and a lone `\udc00` becomes `ed b0 80`. These results are explicit
characterizations and **not desired UTF-8/Unicode requirements**. RFC 8259 §8.1
requires UTF-8 for open data exchange; §8.2 explains problems with lone
surrogates. A future replacement or YAJL fix must deliberately decide these
cases instead of hiding them behind a loose alternative comparison. No usable
attack or additional security impact was demonstrated here.

Well-founded compatibility requirements for a parser replacement, on the other
hand, are exact number text, preserved multiple values, clear naming, callback
cancellations that are not lost, limits effective early, defined partial
arguments/error flags and clean transaction boundaries. Dot/empty-key
collisions, Unicode nonconformance, BodyAccess ordering and the differing error
flags require a deliberate product decision. They are not repaired in these tests.

### Consider audit JSON separately

The twelve audit checks consist of ten records and two deliberate error
mutations. Request bodies and request headers are represented with `\xhh`
before YAJL when bytes are not printable ASCII. Response bodies/headers are
handled differently: Unicode characters and control characters survive JSON
decoding. This is not the same representation as ARGS. The suite checks quotes,
backslashes, NUL/control characters, Unicode, empty contents, duplicate headers,
part selection and boolean/numeric field types. It preserves duplicate JSON
object members as pair lists during structural reading. Only genuinely variable
time/server identification is checked by type rather than exact text. Synthetic
control characters in headers test the connector API, not whether an HTTP server
accepts those headers. Invalid response UTF-8 is not covered.

## Executed checks and limitations

The original baseline was run before local cases were inserted. Both original
code and original tests were unchanged at that point. The binary/audit helpers
were copied into the task branch only afterwards. Additionally executed original
body-limit/transformation tests also read their unchanged files.

| Stage | PASS | FAIL | SKIP | BLOCKED | disabled / XFAIL |
|---|---:|---:|---:|---:|---:|
| Original tests: JSON 6, issue-1576 3, ArgumentsLimit 2, Audit 15, BodyLimits 16, transformations 8 | 50 | 0 | 0 | 0 | 0 |
| Extended native JSON file: original 6 + new 3 | 9 | 0 | 0 | 0 | 0 |
| New request matrix: 159 cases + 4 expectation mutations | 163 | 0 | 0 | 0 | 0 |
| Separate audit matrix: 10 cases + 2 expectation mutations | 12 | 0 | 0 | 0 | 0 |
| Limited ASan/UBSan: only the original JSON class and test observer, 7 chunk cases | 7 | 0 | 0 | 0 | 0 |
| LeakSanitizer attempt: diagnosed as blocked by sandbox/ptrace | 0 | 0 | 0 | 6 | 0 |

The six LeakSanitizer processes actually terminated with nonzero exits; the
general test runner therefore initially reported FAIL 6. The unambiguous message
"LeakSanitizer does not work under ptrace" justifies the final classification
BLOCKED 6 for the leak check. This neither demonstrates six memory leaks nor
constitutes a passing leak test. The subsequent run with `detect_leaks=0`
successfully checked ASan/UBSan. The final ASan/UBSan run also includes the root
number token; the earlier LeakSanitizer attempt comprised six cases.

Original50 and extended native9 must not be counted as 59 different requirements:
six cases are deliberately run before/after. The sanitizer cases are repetitions
as well. These results are not a complete repository test run or a performance
benchmark. Instrumenting the original file `src/request_body_processor/json.cc`
does **not** mean that the rest of the engine, YAJL or system libraries were
instrumented.

The preliminary results were used to correct incorrect test assumptions:
among other things, `whoAmI` was not a YAJL feature banner, an object example
initially was not deep enough for the intended depth cancellation, and YAJL
handled Unicode edge cases differently from what was first assumed. These were
corrections to test design or understanding, not newly introduced production
regressions. The final expectations are explicitly fixed and freshly checked.

The [summary](../../test/json-baseline/results/v3-summary.json),
[request results](../../test/json-baseline/results/v3-request-results.json),
[audit results](../../test/json-baseline/results/v3-audit-results.json) and
[sanitizer results](../../test/json-baseline/results/v3-sanitizer-results.json)
are versioned with the tests. All payloads are synthetic. Raw build logs are
stored locally outside the sources under `/workspace/work/json-yajl-baseline/`:
`v3-bootstrap.log`, `v3-configure.log`, `v3-build.log`,
`v3-original-relevant.log`, `v3-original-{issue-1576,secargumentslimit,auditlog,config-body_limits,transformations}.log`,
`v3-native-expanded.log`, `v3-request-results.log`, `v3-audit-results.log`,
`v3-sanitizer-results.log` and `v3-sanitizer-no-leak-results.log`.
They are not required test fixtures and are not published as build artifacts.

## Reproduction

A typical Debian build environment requires `git`, `build-essential`, `autoconf`,
`automake`, `libtool`, `pkg-config`, `bison`, `flex`, `libyajl-dev`, `libpcre2-dev`,
`libcurl4-openssl-dev`, `libxml2-dev`, `liblua5.4-dev`, `liblmdb-dev`,
`libmaxminddb-dev` and `libfuzzy-dev`. The dependencies actually used here were
YAJL **2.1.0-5+b2**, g++ **14.2.0-19**, Python **3.12.14** and Linux
**6.18.44 x86_64**. Distribution package/patch details appear in the shared
manifest. Other versions constitute new comparison environments, not unchecked
proof of identical behavior. These v3 tests need no Apache/nginx service.

The shared index describes how to obtain and pin both original commits safely.
After checking out the actually published v3 task commit into a new fork clone,
the following project-appropriate commands suffice when dependencies are
installed system-wide. The working directory is the repository root;
`results_dir` is chosen as an absolute, unused path:

```sh
git submodule update --init --recursive others/libinjection others/mbedtls test/test-cases/secrules-language-tests
sh -e ./build.sh
./configure --prefix="$PWD/../v3-install" \
  --enable-assertions=yes --enable-parser-generation=no --without-geoip \
  --with-yajl --with-pcre2 --with-curl --with-libxml --with-lua --with-lmdb \
  --with-maxmind --with-ssdeep=/usr CXXFLAGS='-O2 -g'
make -j2
results_dir="$PWD/../v3-results"
mkdir -p "$results_dir"
export LD_LIBRARY_PATH="$PWD/src/.libs${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
# The project runner evaluates only the first file argument!
for suite in request-body-parser-json issue-1576 secargumentslimit auditlog config-body_limits transformations; do
  ./test/regression_tests "test/test-cases/regression/$suite.json" > "$results_dir/$suite.log" 2>&1 || exit $?
done
sh test/json-baseline/build-probes.sh "$results_dir/probes"
ldd "$results_dir/probes/probe"
python3 test/json-baseline/run.py --probe "$results_dir/probes/probe" \
  --output "$results_dir/request.json" --sensitivity
python3 test/json-baseline/run_audit.py "$results_dir/probes/audit-probe" \
  --report "$results_dir/audit.json"
```

The existing original runner can print "0 tests passed" for an incorrect file
path. Its summaries must therefore show the documented case counts. The new
matrix itself blocks an empty selection of cases. The three new native cases
are automatically read by the usual project test; the additional byte/chunk
matrix is started explicitly with the commands above. `test/Makefile.am` includes
its source files in the distribution.

In the actual session, dependencies were supplied without system installation
under `/workspace/modsecurity-env/deps/usr`. Before the build,
`/workspace/modsecurity-env/env.sh` was sourced; it sets only tool/library paths,
not credentials. The actual configure call differed from the one above only in
these relocated options:

```sh
./configure --prefix=/workspace/work/json-yajl-baseline/v3-install \
  --enable-assertions=yes --enable-parser-generation=no --without-geoip \
  --with-yajl --with-pcre2 --with-curl --with-libxml --with-lua --with-lmdb \
  --with-maxmind --with-ssdeep=/workspace/modsecurity-env/deps/usr \
  CXXFLAGS='-O2 -g' CPPFLAGS='-I/workspace/modsecurity-env/deps/usr/include' \
  LDFLAGS='-L/workspace/modsecurity-env/deps/usr/lib/x86_64-linux-gnu -Wl,-rpath,/workspace/modsecurity-env/deps/usr/lib/x86_64-linux-gnu'
make -j3
```

Reproduction does not require this local env file. For a relocated YAJL
installation, `build-probes.sh` accepts normal `CPPFLAGS`/`LDFLAGS` and
`PKG_CONFIG_PATH`; it also accounts for the parent directory of the `includedir`
reported by YAJL, because the ModSecurity headers use `<yajl/...>`. The current
source library must precede older installations in `LD_LIBRARY_PATH` during
testing.

The limited instrumentation run was built and executed as follows:

```sh
c++ -std=c++17 -DWITH_YAJL -Iheaders -I. \
  -I/workspace/modsecurity-env/deps/usr/include \
  -fsanitize=address,undefined -fno-omit-frame-pointer -g -O1 \
  test/json-baseline/probe.cc src/request_body_processor/json.cc \
  -Lsrc/.libs -L/workspace/modsecurity-env/deps/usr/lib/x86_64-linux-gnu \
  -Wl,-rpath,"$PWD/src/.libs" -lmodsecurity -lyajl \
  -o "$results_dir/probes/probe-sanitized"
ASAN_OPTIONS=detect_leaks=0:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 \
  python3 test/json-baseline/run.py --probe "$results_dir/probes/probe-sanitized" \
  --filter L-parser --output "$results_dir/sanitizer.json"
```

The final run of the final state contains seven parser cases. With system
dependencies, omit the two local `-I`/`-L` paths. A genuine original before-run
requires a separate worktree directly at the pinned original commit; the already
extended file in the task branch is not an unchanged baseline.

## Files and outstanding levels

v3 changes `test/test-cases/regression/request-body-parser-json.json` and
`test/Makefile.am`. New files are
`test/json-baseline/{probe.cc,run.py,build-probes.sh,audit_probe.cc,run_audit.py}`,
the four result files under `test/json-baseline/results/` and this documentation.
The shared documents are additionally provided by the coordinating task state.
Required binary fixtures are generated deterministically from the byte-exact
Python literals; there is no external fixture dependency. Build products,
temporary diagnostic reports and Python caches are not part of the task commit.
The gitlinks remain at their original states.

Not checked are a complete external web server connector test, cross-platform
number/UTF-8 differences, complete heap measurements, out-of-memory paths,
unbounded document sizes and an engine/YAJL build instrumented throughout.
No future replacement library is selected or incorporated. These results cannot
establish a performance or equal-memory-consumption promise between SAX and DOM.
