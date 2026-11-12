# How ModSecurity processes a JSON request

This is a complete English translation of the baseline analysis published in
commit `2bfb2180d8c0fa35ae7a3f2665e999600607d896`. The German original remains
unchanged. Counts, test results and open questions below describe that baseline;
new extension results are documented separately, rather than being inserted into
this historical report.

This analysis concerns unchanged production code from the official
`owasp-modsecurity/ModSecurity` repository at exactly these references:

| Line | Original branch | Original commit |
| --- | --- | --- |
| v3 | `v3/master` | `8271c9d3bc0abcbd1189f5414c196c336c959047` |
| v2 | `v2/master` | `151074aca78a416764b12ba1221e61a17a6c714d` |

**Types of evidence:** “static” means established by inspecting the pinned
original code. This does not prove that a test was run. The
[separate reports for each version](JSON-YAJL-BASELINE.md) and test matrices
list the cases actually run, their expectations and their results. The version
lines have different variable names and depth counting; replacing the library
is not part of this work. YAJL remains unchanged in both lines.

## 1. Introduction without C or C++ knowledge

The **body** is the sequence of bytes after the HTTP headers. A **parser**
recognizes JSON structures and values in it. YAJL reports recognized parts using
**callbacks**, functions supplied by ModSecurity that YAJL calls back into.
ModSecurity does not build a complete copy of the JSON document as an object
tree (DOM). It constructs names for scalar values, meaning strings, numbers,
booleans and null, and stores these values as arguments.

**ARGS** is the collection accessed by rules. It also contains arguments from
the URL query string. A **rule phase** determines when the engine evaluates
rules: phase 1 processes headers, and phase 2 processes the request body. A
**limit** restricts, for example, body size, nesting or the number of arguments.
**Parser completion** tells YAJL that the input has ended. Only then can it
establish, for example, that no more digits follow `1`, or that an opened
structure has remained incomplete.

A simplified processing path is:

```text
Web server / connector supplies headers and body bytes
  -> phase-1 rule selects JSON processor
  -> store body and pass it to YAJL (v2), or collect it first (v3)
  -> YAJL callbacks construct names and store scalar values in ARGS
  -> YAJL completion detects remaining / incomplete input
  -> ModSecurity sets error state
  -> phase-2 rules read ARGS and REQBODY_ERROR
  -> web server / connector implements a requested intervention
```

`Content-Type: application/json` alone does not automatically enable the JSON
processor in the engine code. The supplied example configuration contains a
phase-1 rule with `ctl:requestBodyProcessor=JSON` for this purpose. This rule is
configuration, rather than a built-in detection mechanism.
[v3: selection in `addRequestHeader`, lines 557–589][v3-header-selection],
[v3: example configuration, lines 19–37][v3-example-config],
[v2: automatic selection, lines 404–439][v2-selection].

### A small object

For `{"user":"Ada","enabled":true,"count":1.2300,"optional":null}`, the following
happens. The four entries were additionally confirmed at runtime in both version
lines by `K-intro-object`:

1. `{` opens the object context.
2. `user` becomes the current key; the string callback supplies `Ada`.
3. The boolean callback supplies the text `true`.
4. The number callback supplies exactly `1.2300`, without conversion to a
   floating-point number.
5. The null callback creates a present value with length zero.
6. `}` closes the object context; completion confirms that the input is complete.

| JSON key | v2 name in ARGS | v3 name in ARGS | Stored value |
| --- | --- | --- | --- |
| `user` | `user` | `json.user` | `Ada` |
| `enabled` | `enabled` | `json.enabled` | `true` |
| `count` | `count` | `json.count` | `1.2300` |
| `optional` | `optional` | `json.optional` | Empty string, 0 bytes |

A present null argument therefore cannot be distinguished from a present empty
JSON string by its value alone. A missing argument differs by being absent.
[v3: `addArgument` and scalar callbacks][v3-flatten],
[v2: `json_add_argument` and scalar callbacks][v2-scalars].

### A nested array

For the body `{"items":[["A","B"],"C"],"tail":"D"}`, the square brackets open array
contexts. In v3, these have their own element counters. After the inner array,
processing returns to the outer array; after that ends, the sibling field
`tail` follows. In contrast, v2 has no numeric array indices.

The exact naming of nested and empty containers is characterized in the
separate tests for each version. This analysis does not standardize the names
into JSONPath or JSON Pointer. The v3 test `D-walkthrough` checks the nested
example above byte for byte; v2 `D-walkthrough` also checks it in the Apache run.
Both runs were confirmed. The version reports contain the results and the
other D cases. For the smaller reference examples, the following holds
statically:

| Body | v2: entries as a list | v3: entries as a list |
| --- | --- | --- |
| `["A","B"]` | `[("array.array","A"),("array.array","B")]` | `[("json.array_0","A"),("json.array_1","B")]` |
| `{"items":["A","B"]}` | `[("items.items","A"),("items.items","B")]` | `[("json.items.array_0","A"),("json.items.array_1","B")]` |

For the nested example, the entries are:

| Step | v2 name | v3 name | Value |
| --- | --- | --- | --- |
| First scalar in the inner array | `items.items.items` | `json.items.array_0.array_0` | `A` |
| Second scalar in the inner array | `items.items.items` | `json.items.array_0.array_1` | `B` |
| Inner array ended; next outer scalar | `items.items` | `json.items.array_1` | `C` |
| Outer array ended; sibling in the root object | `tail` | `json.tail` | `D` |

This was confirmed at runtime in case `D-walkthrough` in both version lines
(v3 transaction API, v2 Apache integration). The static derivation is in the
array callbacks of both lines; the [version reports](JSON-YAJL-BASELINE.md)
document the runs.

The v2 names result from `prefix + "." + current_key`; the first array without a
key sets both to `array`. The lists are deliberately not dictionaries: a
dictionary could lose duplicate names.
[v2: array callbacks, lines 171–232][v2-arrays],
[v3: path construction, lines 129–162][v3-flatten].

### Error timing, limits and the HTTP decision

For `{"ok":"yes","broken":`, the first value may already have been stored before
completion reports an incomplete text. The parser does not roll back stored
arguments. The timing of the error matters: in v3, a syntax error sets the error
flags; an appropriate phase-2 rule can subsequently reject with status 400,
for example. In contrast, the Apache path in v2 translates an error first
detected at completion into HTTP 500 and does not reach normal phase 2. If an
error is detected during the parse call, v2 takes a different path, with an error
flag and subsequent rule evaluation. A parser status alone therefore does not
prove a version-independent HTTP status.
[v3: parser calls and error flags][v3-body],
[v2: reading input][v2-reqbody-json], [v2: completion][v2-reqbody-end],
[v2: translation into Apache status][v2-completion-status].

With `SecArgumentsLimit 2` and three scalar object values, the code keeps the
first two arguments and discards the excess value. The shared counter includes
query arguments already present. A callback aborted with `return 0` causes a
YAJL error; the boolean and null callbacks propagate this return value in both
lines.
[v3: argument insertion][v3-argument], [v3: boolean/null][v3-flatten],
[v2: argument insertion][v2-scalars].

## 2. Technical path in v3

### Input, selection and rule phases

The C and C++ APIs allow `appendRequestBody` with a pointer and byte length;
a connector can call it repeatedly. `requestBodyFromFile` reads a file into a
string and also passes it to `appendRequestBody`. Both routes end in the
transaction-owned `std::ostringstream m_requestBody`. This does not mean that
the parser operates on data spilled to a file.
[API: `transaction.h`, lines 385–387 and 705–709][v3-api],
[implementation: lines 899–970][v3-buffer].

The built-in header selection recognizes Multipart and URLENCODED, but not
JSON. `RequestBodyProcessorJSON::evaluate` explicitly sets
`m_requestBodyProcessor = JSONRequestBody` and `REQBODY_PROCESSOR = JSON`.
In `processRequestBody`, `SecRuleEngine Off` causes an early return before the
parser. When the JSON branch is active, it performs one `processChunk` for the
collected body, then `complete`. An array of body lines in the regression test
is not a multiple-chunk test: the runner joins them first, then calls
`appendRequestBody` once.
[Selection action][v3-ctl], [transaction path][v3-body],
[runner: lines 289–301][v3-runner-body],
[body assembly: `yajl_array_to_str`][v3-test-body].

This assembly uses C strings (`stringstream << const char *`). An embedded NUL
from an outer JSON test definition can be truncated in this process. Invalid
UTF-8 bytes and NUL values therefore require an additional byte-exact harness;
`\u0000` in the outer test format does not automatically produce an unchanged
raw body. After assembly, the original runner calculates Content-Length from
`request_body.size()`.
[`RegressionTest`, assembly at lines 52–59][v3-test-body],
[length calculation at lines 268–269][v3-test-length].

Important for API tests: the checks for `SecRequestBodyAccess` and
`ctl:requestBodyAccess` are located **after** the processor branches, at lines
827–843. Bytes already supplied through the API can therefore trigger JSON
callbacks before the guard skips phase 2. Whether a real connector supplies any
bytes at all when body access is off is a separate connector question. The
conclusion “Off means no parsing under any API call” would be statically false.
[`processRequestBody`, lines 736–843][v3-body].

### Lifetime and memory

A new `JSON` object is created for each `Transaction`; its constructor creates
the YAJL handle. `JSON::init` itself is a no-op and does not reset an existing
parser. A new HTTP transaction requires a new transaction context. The
destructor deletes any remaining containers and calls `yajl_free`; the
transaction in turn deletes its JSON processor.
[Transaction: lines 119–180][v3-lifetime],
[`JSON` constructor/destructor and `init`, lines 29–87][v3-parser].

The stored data comprises the raw body, temporary `str()` copies at the call,
YAJL lexer/token state, the stack of open containers with their names, the
current key, and separate ARGS names/values. The temporary string copy passed
through `str().c_str()` lives until the end of the function call; ModSecurity
does not store its pointer as an ARGS value. String and number callbacks copy
bytes into `std::string` using the length reported by YAJL. `VariableValue`
copies the name and value again into its own strings; the collection owns these
entries until they are released.
[Callbacks][v3-flatten], [collection: `set`/`unset`, lines 40–67][v3-collection],
[`VariableValue`, lines 42–56][v3-value].

After processing, the transaction also builds `FULL_REQUEST` and sets
`REQUEST_BODY` and `REQUEST_BODY_LENGTH`. These are additional memory paths
alongside the parser. A fully buffered raw body therefore does not establish
that an additional DOM representation would be free.
[Lines 845–870][v3-phase2].

### Names, collections and data types

`getCurrentKey` sets the root name to `json`. An empty object key becomes
`empty-key` in the relevant contexts. The container stack appends object names
with a dot, and array names with `.array_N`. A scalar increments the current
array counter; closing a child container also increments the counter of the
parent array. Empty containers therefore create no ARGS entry, but can consume
an array index.
[Header: `getCurrentKey`, lines 91–103][v3-key],
[callbacks and counters: lines 129–311][v3-flatten].

Dots are not escaped as path separators. An original key `a.b` can collide with
the path of the nested object `a`/`b`. In v3, keys and values are copied with
explicit byte lengths; an escaped NUL must be distinguished from output through
C strings or logs. The collection is an `unordered_multimap`: identical names
remain as multiple entries; this does not imply a fixed order of different
names.
[Path construction][v3-flatten], [collection and `emplace`][v3-collection],
[collection type][v3-collection-type].

`Transaction::addArgument` checks `m_variableArgs.size() >= limit` before
insertion. The excess entry is not inserted. `origin == "JSON"` inserts into
`ARGS`, but not into `ARGS_POST`; only `origin == "POST"` populates the latter
collection. `ARGS_NAMES` is the name view of ARGS; the combined size value counts
names and values. Query arguments enter the same collection earlier through
`extractArguments("GET", ...)`. JSON body values here therefore do not
automatically belong to `ARGS_POST` or `ARGS_POST_NAMES`.
[`addArgument`, lines 254–310][v3-argument],
[proxy collections: `transaction.h`, lines 213–215][v3-proxies].

### Limits, completion and errors

Root objects and root arrays both increment `m_current_depth` from zero to one
in v3. After creating the container object, the callback checks
`depth > max_depth`. A depth exactly at the limit is allowed. The internal
parser default is 10000; the example configuration sets 512. These values
belong to different layers. `setMaxDepth` accepts the configured value.
[`json.cc`, lines 29, 236–283][v3-parser],
[`setMaxDepth`][v3-key], [example configuration][v3-example-config].

The configuration scanner accepts a sequence of decimal digits for depth and
argument limits. The parser then uses `atoi` and an unsigned configuration
value. Unlike v2, it contains no explicit `> 0` check; zero can therefore occur
as a limit value. Negative or syntactically invalid configurations belong in
configuration tests, rather than JSON runtime cases. Overflow of large
configuration numbers is a separate validation question; no runtime contract
for it is claimed here.
[Scanner: lines 412, 804–805][v3-scanner],
[configuration parser: lines 1628–1636][v3-config].

`processChunk` and `complete` each return success or failure. The transaction
code instead examines the shared error string; `complete` is called even after
a failing chunk. Both methods retrieve YAJL errors and release their buffers.
Nonempty bodies with an error set both `REQBODY_ERROR` and
`REQBODY_PROCESSOR_ERROR`, as well as both message variables. In contrast, a body
with 0 bytes receives error flag zero, even if parser completion reports an
empty input as invalid. This is special handling by ModSecurity, rather than
general YAJL success for empty text.
[Parser: lines 91–125][v3-parser],
[transaction: lines 748–764][v3-body].

A syntax/callback error does not delete previously stored ARGS. HTTP rejection
for such errors is produced by rules, such as the example rule on
`REQBODY_ERROR`. Body size limits additionally have their own engine paths:
`appendRequestBody` compares `current_size + len > limit`. For `ProcessPartial`,
it writes the remaining permitted prefix and returns false; for `Reject`, in
mode On, it sets a disruptive intervention with status 403. A return value
alone is therefore not an HTTP response. `SecRequestBodyNoFilesLimit` can skip
the parser before JSON processing and set `REQBODY_ERROR`/`INBOUND_DATA_ERROR`.
[Body limit: lines 929–970][v3-buffer],
[no-files limit: lines 697–709][v3-body].

## 3. Technical path in v2

### Apache input and processor selection

`read_request_body` first checks whether a body is expected and whether body
access is permitted. Only then does it initialize the processor. Through
`ap_get_brigade`, Apache supplies bucket brigades that have already been
processed at the HTTP transport layer. After `apr_bucket_read`, nonempty pieces
of data are passed through `modsecurity_request_body_store`; at the end,
`modsecurity_request_body_end` follows. HTTP chunk boundaries, Apache buckets
and YAJL token/parser boundaries are different things.
[`apache2_io.c`, lines 181–345][v2-apache-input].

Automatic selection sets URLENCODED or MULTIPART.
`ctl:requestBodyProcessor` can explicitly select JSON. Query arguments are
already in `msr->arguments` before body values are added. As with v3, the example
configuration contains a header rule for this purpose.
[Selection and argument initialization: lines 409–449][v2-selection],
[ctl action: `re_actions.c`, lines 1179–1186][v2-ctl].

```text
Apache read_request_body
  -> modsecurity_request_body_start -> json_init -> yajl_alloc
  -> for each nonempty bucket:
       modsecurity_request_body_store -> json_process_chunk -> yajl_parse
       -> retain body (memory chunks / file)
  -> modsecurity_request_body_end -> json_complete -> yajl_complete_parse
  -> modsecurity_process_phase(PHASE_REQUEST_BODY)
  -> perform_interception for an appropriate rule decision
  -> transaction cleanup -> json_cleanup -> yajl_free
```

[Initialization][v2-reqbody-init], [chunk forwarding][v2-reqbody-json],
[completion][v2-reqbody-end],
[phase 2 in the Apache module, lines 1034–1060][v2-phase2],
[cleanup: `modsecurity.c`, lines 324–328][v2-cleanup].

### Raw body, copies and ownership

The JSON processor in v2 does actually work incrementally. The same input is
subsequently stored unless a corresponding error or limit path prevents this.
The raw body resides in malloc byte buffers with APR management structures, or
in a temporary file. When the in-memory limit is exceeded, the storage path can
switch to disk. YAJL additionally manages lexer state and incomplete tokens.
`json_add_argument` copies each value with `apr_pstrmemdup` into the transaction
pool `msr->mp`; keys and new prefixes are also allocated from the pool. A new
prefix replaces only the pointer, not bytes already allocated in the pool. The
argument values remain valid until the transaction ends, independently of the
short-lived Apache bucket.
[Storage initialization and spill: `msc_reqbody.c`, lines 44–77,
188–267][v2-storage], [JSON copies][v2-scalars].

For URLENCODED, the existing precedent is correctly described: the chunk branch
only counts bytes. At the end, `modsecurity_request_body_end_raw` constructs a
contiguous malloc buffer, copies the stored memory chunks into it, and frees
the old chunks; `end_urlencoded` subsequently calls `parse_arguments`. This
implementation is a precedent for late parsing, but does not prove that the
JSON memory or resource contract would be identical.
[Chunk path and raw buffer: lines 392–396, 550–640][v2-urlencoded].

### Path construction, values and collections

The v2 parser maintains `prefix` and `current_key` as APR strings. For object
keys, YAJL bytes are initially copied with a length, but subsequent path
operations use `%s`, `strlen` and `strrchr('.')`. Escaped NUL and dots in keys
therefore require their own characterization; v3 behavior must not be assumed
for v2. `json_add_argument` joins the prefix and current key with a dot and sets
the origin to `JSON`. A top-level scalar receives an empty name. The special
root-array path uses `array.array`. No numeric element counter exists in this
v2 implementation.
[`msc_json.c`, lines 21–114 and 171–310][v2-json-all].

Numbers are received through `yajl_number`, rather than `yajl_integer` or
`yajl_double`; their text is copied using the given length. Booleans become
`true`/`false`, and null becomes an empty value. Duplicate names are inserted
into the argument table with `apr_table_addn`, rather than `set`. The rule
variable `ARGS` and the name variable iterate over this table, including
multiple occurrences. In contrast, `ARGS_POST` filters for origin `BODY`, which
excludes JSON origin. The same applies to `ARGS_POST_NAMES`. The size counts
names and values, rather than just JSON bytes.
[Scalars and table][v2-scalars],
[`ARGS`/`ARGS_NAMES` and size calculation, lines 118–223][v2-args],
[`ARGS_POST`/`ARGS_POST_NAMES`, lines 334–430][v2-post].

### Depth, argument count and errors

v2 starts at depth zero. Without a current key, the root-object callback returns
immediately and does not count the root. In contrast, the root-array callback
increments the depth. Further object/array contexts increment the depth; the
abort comparison is `>` after the increment. The same JSON nesting under the
same numeric limit therefore cannot generally be assumed equivalent to v3.
The defaults are depth 10000 and argument count 1000. The Apache configuration
functions use `strtol` and reject values `<= 0` and `LONG_MIN`/`LONG_MAX`. They do
not check that the entire configuration text was consumed; statements about
strict configuration syntax therefore require separate tests.
[Depth: array/object callbacks][v2-json-all],
[defaults: `modsecurity.h`, lines 98–99][v2-defaults],
[configuration: lines 2098–2143][v2-config].

The argument limit counts all `msr->arguments` already present, including query
values, duplicate names and scalar JSON values. Empty containers create no
arguments. `json_add_argument` checks `nelts >= arguments_limit` before
`apr_table_addn`; the excess value has already been copied into the pool, but is
not inserted. The code sets `msc_reqbody_error` and a JSON limit message and
returns zero. All four scalar callback types propagate this abort signal.
[`json_add_argument`, lines 35–77 and callbacks 123–168][v2-scalars].

`json_process_chunk` reports errors through its return value and a message
pointer. The surrounding body path sets `msc_reqbody_error` and its message.
`json_complete` calls YAJL completion; the body-end path ignores its error when
the body length is zero. A body without Content-Length or Transfer-Encoding can
be skipped before parser initialization. An empty transport body, a body with
0 bytes, and YAJL given empty text are therefore different observations.
[Error paths: lines 373–423][v2-complete],
[body-end path: lines 724–739][v2-reqbody-end],
[Apache guard: lines 191–208][v2-apache-input].

In this v2 line, `REQBODY_ERROR` and `REQBODY_ERROR_MSG` are the registered
relevant rule variables; the v3 names `REQBODY_PROCESSOR_ERROR` and
`REQBODY_PROCESSOR_ERROR_MSG` are not assumptions that can be transferred to v2.
Parser errors do not delete arguments already created. For errors during
`yajl_parse`, the store path sets the error flag; because of this flag, the
body-end path skips further processor completion. Phase 2 can then evaluate
the remaining arguments and error flag. In contrast, if an error is first
detected by `json_complete`, `modsecurity_request_body_end` returns -1.
`read_request_body` immediately translates this into HTTP 500; the Apache
module returns before normal phase 2. Any later change must explicitly assess
this existing distinction. Independently of this, body size limits can directly
produce Apache status 413; `Reject`, `ProcessPartial` and `DetectionOnly` have
their own branches and must not be generalized from the HTTP status of a single
test.
[Variable registration: lines 4192–4212][v2-error-vars],
[completion guard: lines 685–686][v2-end-guard],
[Apache completion status: lines 345–358][v2-completion-status],
[Apache body limit: lines 997–1024][v2-phase2],
[ongoing bucket limit check: lines 261–332][v2-apache-input].

## 4. Reading and writing JSON are separate dependencies

### Audit JSON in v3

`Transaction::toJSON` uses its own `yajl_gen` handle, creates the audit record,
copies the completed buffer into a string, appends a newline and frees the
generator. The generator is not used as a request parser. Ports and the HTTP
code are written as numbers, and `is_interrupted` as a boolean. In contrast,
rule ID, severity and several other metadata fields are written as strings. A
writer test must deliberately check these types. For the request body, request
headers and certain message data, `toHexIfNeeded` is also applied first; the
audit text is therefore not generally identical to the original request bytes.
[`toJSON`, lines 1567–1762][v3-audit].

`src/modsecurity.cc` also contains generator calls, but these belong to
`ModSecurity::processContentOffset`: this public C++ function constructs a JSON
description of match and transformation positions. It is not a second audit
writer implementation. A supplied example calls it. A later replacement must
therefore also account for this separate JSON generator.
[`processContentOffset`, lines 215–385][v3-offset-writer],
[public declaration][v3-offset-api],
[example call, lines 18–30][v3-offset-example].

The `LOGFY_ADD*` macros are indeed located in the installed public
`transaction.h` and expand into YAJL generator calls. A search of the examined
source tree finds their use only in `transaction.cc`. This proves neither the
absence of external users nor that removal from a public header would be
permissible without a compatibility decision. The macros themselves also do
not declare the type `yajl_gen`; they rely on generator names and the relevant
declarations at the call site.
[Macros, lines 83–99][v3-macros],
[header installation: `src/Makefile.am`, lines 39–57][v3-build].

### Audit JSON, origin and sanitization in v2

`sec_audit_logger_json` in `msc_logging.c` uses `yajl_gen` and the macro helpers
from `msc_logging_json.h`. These helpers are direct macros, rather than an
abstract writer interface independent of YAJL. For example, `yajl_string` uses
`strlen`; embedded null bytes therefore require a separate writer observation.
The JSON body in the audit log is written as an array of stored body chunks,
rather than a JSON object parsed again.
[`msc_logging_json.h`, lines 1–13][v2-writer-helpers],
[`sec_audit_logger_json`, lines 656–678, 932–1020][v2-audit].

Here, sanitization means masking selected values before writing the audit log.
The writer sorts marked BODY/JSON arguments by `value_origin_offset`, reads the
stored raw body sequentially, and overwrites the designated regions with
asterisks. However, `json_add_argument` sets the origin length to the value
length supplied by the callback and calculates the offset as callback pointer
minus `base_offset`; `base_offset` is set for each chunk. String decoding,
tokens spanning chunk boundaries, and synthetic boolean/null values therefore
require their own end-to-end tests before correct mapping to the original body
can be claimed. An in-situ library alone would prove neither absolute origin
across multiple chunks nor the length of the original escape spelling. This
analysis does not turn these observations into a published security assessment
and does not claim proven data leakage.
[Offset/length assignment: `msc_json.c`, lines 19–55 and 378][v2-json-all],
[use in the writer: lines 879–1006][v2-audit].

### Build and connector

v3 looks for YAJL through `build/yajl.m4` with package names `yajl2 yajl`, header
`yajl/yajl_parse.h`, link library `yajl` and definition `WITH_YAJL`. Both parser
and generator depend on this build feature. v2 has `build/find_yajl.m4`, which
determines YAJL headers and link flags; the v2 test program is explicitly built
against YAJL as well.
[v3 build check][v3-yajl-build], [v2 build check][v2-yajl-build],
[v2 test link flags, lines 35–75][v2-test-build].

The separate official connector repository was examined read-only at commit
`b7cf44e57bc386bc5c15e5c9319e6b6b719357f2`. Its `config` sets
`YAJL_EXTRA="-L$YAJL_LIB -lyajl"` **only when `YAJL_LIB` is nonempty** and includes
it in the explicit ModSecurity path. The blanket statement “the connector
always links -lyajl directly” is too strong. No connector was built here, and
no claim was made about all connectors.
[Connector: `config`, lines 8–48][nginx-config].

## 5. Inventory of the original tests

These numbers are a static inventory at the pinned original commits, rather
than counts of successful runs. Here, a JSON integration test is counted as a
case whose active rules explicitly select the JSON processor and whose
body/assertions concern the request path.

| Line / original file | Cases | What the existing assertions establish |
| --- | ---: | --- |
| v3 `request-body-parser-json.json` | 6 | A flat and a nested field; empty body without an error flag; invalid text with an error flag; object depth below/above the limit |
| v3 `issue-1576.json` | 3 | Selected names/values in root/nested arrays, rather than a complete entry list |
| v3 `secargumentslimit.json` | 2 | One value below the limit; above the limit, a log message/400 from an error rule |
| v3 `config-body_limits.json` | 2 with JSON selection, 16 in total | No-files limit below/above the boundary; no complete JSON value matrix |
| v3 `transformations.json` | 3 with JSON selection | Subsequent transformations on JSON values, rather than general parser characterization |
| **v3 total under this definition** | **16** | Different depths of assertion; not equivalent to 16 fully checked JSON contracts |
| v2 `rule/15-json.t` | 7 | Object value, three array/path cases, depth below/above the limit, root number `25` |
| v2 `action/00-transformations.t` | 1 | HTML entity transformation of a JSON value |
| **v2 total under this definition** | **8** | The first four cases in `15-json.t` alternatively accept missing JSON support |

[v3 main file][v3-original-tests], [v3 arrays][v3-array-tests],
[v3 argument limit][v3-args-tests], [v3 body limits][v3-body-tests],
[v3 transformations][v3-transform-tests], [v2 main file][v2-original-tests],
[v2 transformation][v2-transform-tests].

The v3 Automake test catalog registers the named regression files; the runner
invokes the transaction path. The v2 Automake rule `test-regression` starts the
existing Perl runner; ordinary `make test` is not the same HTTP regression run.
[v3 catalog][v3-suite], [v3 runner][v3-runner-body],
[v2 test targets, lines 77–83][v2-test-build].

Further original tests were searched for ARGS, body, limits and writer
relationships: in v3, these included `variable-ARGS*.json`,
[`variable-REQBODY_PROCESSOR_ERROR.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-REQBODY_PROCESSOR_ERROR.json), [`variable-REQUEST_BODY.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-REQUEST_BODY.json),
[`action-ctl_request_body_access.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/action-ctl_request_body_access.json) and [`action-ctl_request_body_processor.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/action-ctl_request_body_processor.json);
in v2, particularly [`target/00-targets.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/target/00-targets.t), [`config/10-request-directives.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/config/10-request-directives.t),
[`config/10-audit-directives.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/config/10-audit-directives.t) and logging tests. Their existence does
not prove JSON-specific coverage. For example, the v2 argument-limit cases in
`10-request-directives.t` are URLENCODED/query cases; a JSON selection rule
inside a fuzzyHash **request body** is only test-data content and is not counted
as an active parser rule.

v3 `auditlog.json` contains 15 cases, six of which use `SecAuditLogFormat JSON`:
basic form, binary message data, body, headers, and two `is_interrupted` states.
These are writer cases. A writer regex does not yet check the complete syntactic
and structural validity of a record.
[v3 original writer cases][v3-audit-tests].

| Required group | Original v3 state | Original v2 state |
| --- | --- | --- |
| A empty body | Partial: body with 0 bytes/flag present | Absent from the JSON cases |
| B special keys | No complete coverage found | No complete coverage found |
| C root scalars | Not in the named JSON cases | Partial: root number `25` |
| D arrays/names | Partial: three targeted historical cases | Partial: three targeted historical cases |
| E depth | Partial: object below/above the limit | Partial: object below/above the limit |
| F argument limit | Partial: two JSON cases | General boundary cases present, no JSON matrix |
| G UTF-8/escapes | Writer/transformation tests do not replace a parser byte matrix | No JSON byte matrix found |
| H number text | No required boundary/lexeme matrix | Only a root number, no boundary/lexeme matrix |
| I duplicate keys | No targeted JSON duplicate matrix found | No targeted JSON duplicate matrix found |
| J completion/trailing content | Invalid `a`, no complete completion matrix | No targeted completion matrix found |

Additional original tests considered only as search context comprise:

| Line | File | Case objects in file | Classification |
| --- | --- | ---: | --- |
| v3 | [`action-ctl_request_body_access.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/action-ctl_request_body_access.json) | 3 | Body access; not automatically JSON coverage |
| v3 | [`action-ctl_request_body_processor.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/action-ctl_request_body_processor.json) | 3 | Processor selection; not automatically JSON coverage |
| v3 | [`variable-ARGS.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-ARGS.json) / [`variable-ARGS_NAMES.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-ARGS_NAMES.json) | 7 / 5 | General arguments and names |
| v3 | [`variable-ARGS_POST.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-ARGS_POST.json) / [`variable-ARGS_POST_NAMES.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-ARGS_POST_NAMES.json) | 3 / 4 | POST origin, separate from JSON |
| v3 | [`variable-REQBODY_PROCESSOR_ERROR.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-REQBODY_PROCESSOR_ERROR.json) | 2 | General processor errors |
| v3 | [`variable-REQUEST_BODY.json`](https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/variable-REQUEST_BODY.json) | 1 | Raw body variable |
| v2 | [`target/00-targets.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/target/00-targets.t) | 19 | General rule targets/collections |
| v2 | [`config/10-request-directives.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/config/10-request-directives.t) | 25 | Access, body and argument limits |
| v2 | [`config/10-audit-directives.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/config/10-audit-directives.t) | 10 | Audit configuration |
| v2 | [`action/10-logging.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/action/10-logging.t) | 24 | Logging/sanitization, no complete JSON writer suite |
| v2 | [`rule/10-xml.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/rule/10-xml.t) | 12 | XML/shared argument limits as a comparison |
| v2 | [`misc/00-phases.t`](https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/misc/00-phases.t) | 5 | Phase behavior as a comparison |

These sizes count defined case objects, rather than executed or passing cases.
The v2 count is based on the original runner's `type =>` case objects; the runner
evaluates each `.t` as a list. It was not confirmed by running all the files
listed above.
[Perl runner: file discovery and evaluation, lines 150–189][v2-runner-inventory].

The new tests and their run reports fill these gaps. A test that fixes current
behavior as its expectation does not automatically declare an unusual result
to be the desired specification.

## 6. Checking statements from fzipi's comment

The [specific comment of 14 October 2026][comment] was read directly. The table
distinguishes evidence from conclusions. In the runtime column, group IDs refer
to the explicit expectations in the [version reports](JSON-YAJL-BASELINE.md),
rather than additional blanket PASS claims. Library comparisons, package
availability, license/performance claims and an actual migration are outside
this task and were not checked.

| Statement | Result of checking | Original code evidence | Runtime evidence / limitation |
| --- | --- | --- | --- |
| v3 passes the whole body in one `processChunk` plus `complete` | Confirmed for `Transaction::processRequestBody` | [736–764][v3-body] | v3 `L-parser-*` checks real chunks; `K-connector-chunks` checks multiple API feeds. These are different levels |
| Therefore SAX has no memory advantage in v3 | Not established | [Body buffer][v3-buffer], [containers/ARGS][v3-flatten] | No comparative peak-RSS/allocator experiment; DOM nodes, copies and earlier aborts must also be considered |
| v2 parses chunks and retains the body | Confirmed on the normal path; limits can abort this path earlier | [Store][v2-storage], [JSON forwarding][v2-reqbody-json] | HTTP transport chunks do not equal parser chunks; disk spill requires separate checking |
| v2 URLENCODED parses only at the end | Confirmed | [550–640][v2-urlencoded] | A precedent, rather than equivalence of all resource/error contracts |
| Both count array elements | Disproved for numeric naming in v2 | [v2 171–232][v2-arrays], [v3 129–162][v3-flatten] | `D-root`, `D-multidim`, `D-walkthrough`: v2 repeats names, v3 creates `array_N` |
| Both flatten scalar values into ARGS | Confirmed, with different naming and collection origin | [v2][v2-scalars], [v3][v3-flatten] | Groups C/D/F: empty containers yield no arguments; null becomes an empty string |
| Both apply depth and argument limits | Confirmed, but counting/defaults/validation differ | [v2 depth][v2-json-all], [v3 depth][v3-parser], [v2 config][v2-config], [v3 config][v3-config] | Groups E/F check container/scalar aborts; later DOM traversal would have different timing |
| Numbers are received as text | Confirmed | [v2 159–168/324–335][v2-json-all], [v3 47–58/225–228][v3-parser] | Group H compares text bytes; numeric rule operators alone are insufficient |
| DOM requires only the body plus a fixed overhead per value | Not demonstrated for ModSecurity | [Stored path/value copies][v3-flatten], [v2 APR copies][v2-scalars] | No new library/allocator analysis; this approximation omits name/token lengths, depth and limit timing |
| The writer side would be the larger later change | Not yet verifiable as change scope; no migration implemented | [v2 writer][v2-audit], [v3 writer][v3-audit], [public interface][v3-offset-api] | No hypothetical diff sizes or equivalence of effort claimed |
| The writer uses YAJL in v2/v3 | Confirmed | [v2 writer][v2-audit], [v3 writer][v3-audit] | Separate `AUDIT-*` cases; writer and request parser are checked separately |
| `modsecurity.cc` is part of the audit writer | Needs qualification: a separate public offset-JSON generator | [215–385][v3-offset-writer] | The example uses the public function; external use cannot be fully determined |
| Public headers contain YAJL macros | Confirmed | [83–99][v3-macros] | Only internal use found in the examined tree; absence of external use not proved |
| ModSecurity-nginx links `-lyajl` | Conditionally confirmed | [Connector 20–23][nginx-config] | Only when `YAJL_LIB` is set; connector build not run |
| Six v3 / seven v2 parser cases | Correct for the two named files, too small as a repository inventory | [Test inventory above](#5-inventory-of-the-original-tests) | 16 v3 / 8 v2 active JSON integration cases under the disclosed definition |
| Empty body and root scalars have no tests in either line | Disproved as a blanket claim | [v3 empty body case 3][v3-original-tests], [v2 root number case 7][v2-original-tests] | The respective variants remain only partially covered |
| In-situ replaces the parser while preserving original offsets | Open; no established contract | [Offset calculation][v2-json-all], [masking][v2-audit] | Escape lengths, origin across multiple chunks and the actual audit record require end-to-end evidence |

## 7. RFC 8259, YAJL and future compatibility

[RFC 8259 §2][rfc2] allows any JSON value at the root; root scalars are therefore
not automatically invalid. Section 4 recommends unique object names with
SHOULD, but does not prohibit duplicates in the grammar. Section 6 distinguishes
number syntax from implementation limits: `1e400` or an integer beyond uint64
is not syntactically invalid solely because of its magnitude. Preserved number
text is a ModSecurity contract for propagation to rules, rather than a demand
for numeric interpretation.
[RFC §4][rfc4], [RFC §6][rfc6].

RFC §7 requires escapes for raw control characters; §8.1 requires UTF-8 for
exchange between open systems. Section 8.2 explicitly treats lone surrogates
as a problematic interoperability case that the ABNF can permit. Such Unicode
cases must be assessed separately from malformed escape syntax and invalid
UTF-8 byte sequences. Section 9 permits implementation limits, for example for
size, depth and numeric precision; §10 requires syntactically valid generator
output.
[RFC §7][rfc7], [RFC §8][rfc8], [RFC §9/10][rfc9].

The YAJL 2.1.0 API describes callback abortion by returning zero as
`yajl_status_client_canceled`. By default, the permissive options for comments,
multiple documents, trailing content and partial values are disabled. Both
ModSecurity lines explicitly set `yajl_allow_partial_values` to zero. Completion
therefore remains necessary. The YAJL documentation also describes how a number
callback can preserve number text instead of integer/double conversion. The
specific distribution version and patches used must additionally be recorded
in the provenance manifest; web documentation does not replace recording them.
[YAJL parser API][yajl-api], [YAJL callbacks][yajl-callbacks].

The following separate compatibility questions follow for a later library
decision:

* Preserve: number text, length and bytes of decoded values; multiple
  occurrences; evidenced naming conventions for each version line; ARGS/name
  view and origin; error flags; complete finalization.
* Decide deliberately: collisions caused by dots, treatment of empty/NUL keys,
  unusual array paths and different depth counting. Characterization here does
  not signify approval of current behavior.
* Preserve or assess resources: early abort at the depth/argument limit,
  additional body/token/path/value copies, body-limit actions and memory/file
  paths. Building a DOM before applying the limit is not automatically an
  equivalent implementation.
* Preserve the writer separately: structural JSON validity, field types,
  escaping/hex representation, multipart/chunk representation, metadata and the
  public offset-JSON function. Parser success does not prove writer correctness.

Resource, offset, concurrency or connector questions not evidenced by a run
remain open checks. This documentation changes no production path and selects
no replacement library.

<!-- Immutable official-source references. -->
[comment]: https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842
[v3-header-selection]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L557-L589
[v3-example-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/modsecurity.conf-recommended#L19-L71
[v3-ctl]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/actions/ctl/request_body_processor_json.cc#L28-L34
[v3-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/headers/modsecurity/transaction.h#L385-L387
[v3-buffer]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L899-L970
[v3-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L674-L843
[v3-phase2]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L845-L871
[v3-lifetime]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L119-L180
[v3-parser]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/request_body_processor/json.cc#L29-L283
[v3-flatten]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/request_body_processor/json.cc#L129-L311
[v3-key]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/request_body_processor/json.h#L91-L108
[v3-argument]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L254-L310
[v3-proxies]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/headers/modsecurity/transaction.h#L213-L215
[v3-collection-type]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/headers/modsecurity/anchored_set_variable.h#L71-L72
[v3-collection]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/anchored_set_variable.cc#L40-L109
[v3-value]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/headers/modsecurity/variable_value.h#L42-L56
[v3-scanner]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/parser/seclang-scanner.ll#L804-L805
[v3-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/parser/seclang-parser.yy#L1628-L1636
[v3-audit]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/transaction.cc#L1567-L1762
[v3-offset-writer]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/modsecurity.cc#L215-L385
[v3-offset-api]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/headers/modsecurity/modsecurity.h#L299-L300
[v3-offset-example]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/examples/reading_logs_with_offset/read.cc#L18-L30
[v3-macros]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/headers/modsecurity/transaction.h#L83-L99
[v3-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/src/Makefile.am#L39-L57
[v3-yajl-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/build/yajl.m4#L10-L31
[v3-runner-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/regression/regression.cc#L289-L301
[v3-test-length]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/regression/regression_test.cc#L268-L269
[v3-test-body]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/regression/regression_test.cc#L52-L68
[v3-suite]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-suite.in
[v3-original-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/request-body-parser-json.json
[v3-array-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/issue-1576.json
[v3-args-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/secargumentslimit.json
[v3-body-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/config-body_limits.json
[v3-transform-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/transformations.json
[v3-audit-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/8271c9d3bc0abcbd1189f5414c196c336c959047/test/test-cases/regression/auditlog.json
[v2-selection]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/modsecurity.c#L404-L449
[v2-ctl]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/re_actions.c#L1179-L1186
[v2-apache-input]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/apache2_io.c#L181-L345
[v2-phase2]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/mod_security2.c#L997-L1060
[v2-json-all]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_json.c#L19-L435
[v2-scalars]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_json.c#L21-L168
[v2-arrays]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_json.c#L171-L232
[v2-completion-status]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/apache2_io.c#L345-L358
[v2-end-guard]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_reqbody.c#L685-L686
[v2-reqbody-init]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_reqbody.c#L100-L150
[v2-reqbody-end]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_reqbody.c#L724-L739
[v2-reqbody-json]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_reqbody.c#L373-L396
[v2-storage]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_reqbody.c#L44-L267
[v2-urlencoded]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_reqbody.c#L550-L640
[v2-cleanup]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/modsecurity.c#L324-L328
[v2-complete]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_json.c#L373-L435
[v2-args]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/re_variables.c#L118-L223
[v2-post]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/re_variables.c#L334-L430
[v2-defaults]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/modsecurity.h#L98-L99
[v2-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/apache2_config.c#L2098-L2143
[v2-error-vars]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/re_variables.c#L4192-L4212
[v2-audit]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_logging.c#L656-L1020
[v2-writer-helpers]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/apache2/msc_logging_json.h#L1-L13
[v2-yajl-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/build/find_yajl.m4#L21-L153
[v2-runner-inventory]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/run-regression-tests.pl.in#L150-L189
[v2-test-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/Makefile.am#L35-L83
[v2-original-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/rule/15-json.t
[v2-transform-tests]: https://github.com/owasp-modsecurity/ModSecurity/blob/151074aca78a416764b12ba1221e61a17a6c714d/tests/regression/action/00-transformations.t
[nginx-config]: https://github.com/owasp-modsecurity/ModSecurity-nginx/blob/b7cf44e57bc386bc5c15e5c9319e6b6b719357f2/config#L8-L48
[rfc2]: https://www.rfc-editor.org/rfc/rfc8259.html#section-2
[rfc4]: https://www.rfc-editor.org/rfc/rfc8259.html#section-4
[rfc6]: https://www.rfc-editor.org/rfc/rfc8259.html#section-6
[rfc7]: https://www.rfc-editor.org/rfc/rfc8259.html#section-7
[rfc8]: https://www.rfc-editor.org/rfc/rfc8259.html#section-8
[rfc9]: https://www.rfc-editor.org/rfc/rfc8259.html#section-9
[yajl-api]: https://lloyd.github.io/yajl/yajl-2.1.0/yajl__parse_8h.html
[yajl-callbacks]: https://lloyd.github.io/yajl/yajl-2.1.0/structyajl__callbacks.html
