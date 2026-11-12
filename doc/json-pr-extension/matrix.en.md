# Test matrix: additional ModSecurity v3 checks from PR #4/#5 and the comments
[Overview and reproduction](README.en.md)

This matrix concerns the **unchanged official production commit
`8271c9d3bc0abcbd1189f5414c196c336c959047`**, with YAJL 2.1.0 enabled. New tests
exercise ModSecurity itself: the public transaction API, the real internal JSON
request processor, and separate JSON writers. The v2 analysis remains in the
historical English documentation; this extension was implemented and run only for v3.

The [complete catalog](catalog.json) is the authoritative case index. It and the
machine-readable reports linked below contain the complete bytes, lengths/hashes,
headers, rules, configuration, explicit expectations and observed results. The
tables here provide a readable mapping, rather than replacing those exact data.
In particular, arguments are lists of byte pairs, rather than dictionaries;
duplicate names, NUL and number spellings are preserved. Expectations were fixed
in test code and are not regenerated from the parser's output during each test.

## 1. Sources, test types and counting

- [jsonbench PR #4](https://github.com/digitalwave/jsonbench/pull/4) and
  [PR #5](https://github.com/digitalwave/jsonbench/pull/5) supply ideas for token
  kinds, naming, byte handling, limits, completion and proposed atomic argument
  extraction. The tests were written independently for ModSecurity; benchmark
  expectations are not adopted as its contract without review. The inspected PR
  revisions are recorded in [provenance.json](provenance.json).
- [Fzipi's comment](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842)
  supplies the ten required groups A–J and further questions about streaming,
  resources, limits and the writing side. All A–J remain present as named groups,
  even where a group needs no new cases. This does not claim exhaustive testing
  of every possible variant.
- [Airween's comment](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6142615388)
  is linked in particular to empty input, large integers and invalid UTF-8.
  These three topics are mapped explicitly below. A library choice, SAX redesign
  or JSON Schema capabilities are not implemented features here: YAJL remains
  unchanged; schema validation is neither added nor reported as a tested
  ModSecurity feature.

**Characterization** checks observed current behavior. A PASS does not endorse
problematic Unicode handling or malformed writer output as the desired
specification. **Requirement tests** separately check guarantees and explicitly
labelled policy proposals. Unmet expectations remain ordinary FAIL with a
nonzero exit, rather than XFAIL. **Controls** check that deliberately incorrect
expectations actually fail, or that a permitted alternative output is accepted.

| Report / level, all v3 | Product cases | Additional controls | PASS | FAIL | SKIP / BLOCKED / XFAIL |
| --- | ---: | ---: | ---: | ---: | --- |
| [Previous request baseline](../../test/json-baseline/results/v3-request-results.json), transaction / internal parser / configuration | 159 | 4 | 163 | 0 | 0 / 0 / 0 |
| [New request characterization](../../test/json-baseline/results/extended-request-results.json), 60 transaction cases, 4 transaction sequences, 124 internal parser cases | 188 | 4 | 192 | 0 | 0 / 0 / 0 |
| [Request requirements](../../test/json-baseline/results/requirements-results.json), transaction API | 22 | 0 | 6 | 16 | 0 / 0 / 0 |
| [Extended writers](../../test/json-baseline/results/extended-audit-results.json), audit / public offset API | 23 | 2 | 21 | 4 | 0 / 0 / 0 |

These four reports together contain **382 PASS and 20 FAIL**, including ten
controls. The final combined state additionally contains the retained audit
checks (12 PASS) and the native JSON regression file (11 PASS). Thus, according
to [results-summary.json](results-summary.json), there are **399 PASS in the
characterization profile**, **6 PASS / 20 FAIL in the requirements profile**,
and **405 PASS / 20 FAIL** combined; SKIP, BLOCKED and XFAIL are all 0. The
limited [sanitizer rerun](../../test/json-baseline/results/extended-sanitizer-results.json)
with 124 PASS is reported separately and is not additional unique case coverage.
Historical baseline statistics are not rewritten by these results.

## 2. Required groups A–J: inspectable mapping

“Baseline” here counts the **primary A–J ID prefixes**. The report additionally
contains 29 baseline cases K/L and four controls. “New” counts the actual subject
checked, rather than automatically including every broad `area` tag from the
generator. For example, UTF-8 chunk cases are not counted as number cases. A new
case can check multiple groups, so the column **overlaps and must not be summed**.
There are 233 group assignments for 184 distinct new cases; four further cases
check only ASCII value lengths as a resource topic. All new IDs and their
specific assignments appear in section 5.

| Group / subject | Baseline cases | New characterization cases, overlapping | Related additional request requirements | Checked expectation and limitation |
| --- | ---: | ---: | --- | --- |
| A: empty body | 6 | 0 | None | Missing append call and explicit 0 bytes: no ARGS, error flag 0; JSON whitespace only: error flag 1; `{}`/`[]`: no ARGS; `""`: one empty root value. `L-parser-empty` distinguishes actual parser completion with empty input |
| B: empty/special keys | 9 | 17 | UTF-8 keys separately under G | Exact `json.*` names, `empty-key`, dots/path collisions, backslash, Unicode and length-aware NUL; new cases check return to siblings, nested NUL and long complete paths |
| C: top-level values | 14 | 7 | None | String, empty string, integer, decimal, true/false/null, each with/without whitespace, named `json`; new real chunk cases for `1E+02` check empty pre-completion ARGS and insertion only at completion |
| D: arrays and naming | 9 | 13 | None | `json.array_N`, nested paths, empty containers consume an index without ARGS; new cases combine token kinds, dotted names, siblings and array argument limits |
| E: depth | 16 | 1 | `REQ-POLICY-rollback-depth` | Limit 3 with depths 2/3/4 for objects/arrays/mixed forms; root counts as 1, exact limit allowed; limit 0, default sanity check and three rejected configurations separately. New sequence: depth error, then success without residual state |
| F: argument limit | 16 | 11 | `REQ-POLICY-rollback-arguments` | Shared counting including query/duplicates; excess value absent; empty containers do not count; every scalar callback including true/false/null cancels effectively. New arrays at limit 2: two values succeed, third discarded and error flags 1 |
| G: UTF-8, Unicode, escapes | 20 | 63 | 12 `REQ-UTF8-*`, 6 `REQ-POLICY-surrogate-*` | Decoded bytes and lengths in names/values, actual invalid bytes, escape syntax, surrogates, NUL and boundaries inside UTF-8/escape sequences; acceptance of malformed Unicode remains characterization only |
| H: numbers and number text | 27 | 71 | None | Exact lexemes rather than numeric equality; 64-bit/IEEE-754 boundaries, long integers, exponents and syntax counterexamples; new token-kind comparisons, 512 digits, large exponents and all selected chunk splits |
| I: duplicate keys | 5 | 0 | None | Equal/different values retained as multiple occurrences; empty/nested keys, casing and rule variables; `F-duplicates` additionally checks the shared limit. Existing tests are retained rather than duplicated |
| J: trailing content and incomplete input | 8 | 50 | `REQ-POLICY-rollback-syntax`, `REQ-POLICY-rollback-eof` | Whitespace accepted, trailing characters/two documents rejected; truncated containers/strings/numbers/literals; new separator/comment errors, NUL, long incomplete tokens, error timing in chunk or only at complete, and retained partial arguments |

Baseline IDs by group (all entries in the [baseline report](../../test/json-baseline/results/v3-request-results.json)):

- **A**: `A-absent`, `A-zero`, `A-whitespace`, `A-object`, `A-array`, `A-string`.
- **B**: `B-empty`, `B-nested-empty`, `B-siblings`, `B-dot-flat`, `B-dot-nested`, `B-dot-collision`, `B-backslash`, `B-unicode`, `B-nul`.
- **C**: `C-string`, `C-string-ws`, `C-empty`, `C-empty-ws`, `C-integer`, `C-integer-ws`, `C-decimal`, `C-decimal-ws`, `C-true`, `C-true-ws`, `C-false`, `C-false-ws`, `C-null`, `C-null-ws`.
- **D**: `D-object-walkthrough`, `D-root`, `D-object-array`, `D-array-object`, `D-multidim`, `D-empty-slots`, `D-mixed`, `D-walkthrough`, `D-tail`.
- **E**: `E-object-2`, `E-object-3`, `E-object-4`, `E-array-2`, `E-array-3`, `E-array-4`, `E-mixed-2`, `E-mixed-3`, `E-mixed-4`, `E-partial`, `E-zero-object`, `E-zero-scalar`, `E-default-depth`, `E-config--1`, `E-config-nope`, `E-config-1.5`.
- **F**: `F-object-2`, `F-object-3`, `F-object-4`, `F-array-2`, `F-array-3`, `F-array-4`, `F-callback-string`, `F-callback-number`, `F-callback-boolean`, `F-callback-boolean-true`, `F-callback-null`, `F-empty-containers`, `F-duplicates`, `F-query`, `F-query-full`, `F-zero`.
- **G**: `G-utf2`, `G-utf3`, `G-utf4`, `G-escapes`, `G-common-escapes`, `G-nul-value`, `G-invalid-value-continuation`, `G-invalid-key-continuation`, `G-invalid-value-overlong`, `G-invalid-key-overlong`, `G-invalid-value-truncated`, `G-invalid-key-truncated`, `G-invalid-value-outside`, `G-invalid-key-outside`, `G-bad-escape`, `G-short-unicode`, `G-nonhex`, `G-raw-control`, `G-high-surrogate`, `G-low-surrogate`.
- **H**: `H-number-00`, `H-number-01`, `H-number-02`, `H-number-03`, `H-number-04`, `H-number-05`, `H-number-06`, `H-number-07`, `H-number-08`, `H-number-09`, `H-number-10`, `H-number-11`, `H-number-12`, `H-number-13`, `H-number-14`, `H-number-15`, `H-number-16`, `H-number-17`, `H-number-18`, `H-number-19`, `H-number-20`, `H-invalid-01`, `H-invalid-+1`, `H-invalid-1.`, `H-invalid-1e`, `H-invalid-NaN`, `H-invalid-Infinity`.
- **I**: `I-same`, `I-different`, `I-empty`, `I-nested`, `I-case`.
- **J**: `J-trailing-ws`, `J-trailing-data`, `J-two-docs`, `J-incomplete-object`, `J-incomplete-array`, `J-incomplete-string`, `J-incomplete-number`, `J-incomplete-literal`.

## 3. Additional integration, resources and real chunks

The existing `K-*` cases check processor selection (including the absence of
automatic JSON selection based on Content-Type), explicit vendor/case variants,
body access/rule mode, ARGS rule access, body limits immediately around their
boundary with Reject/ProcessPartial/DetectionOnly, the no-files limit and
transaction state. The seven `L-parser-*` cases call the actual internal
ModSecurity processor. An array of body lines or multiple connector
`appendRequestBody` calls, in contrast, do not prove multiple
`JSON::processChunk` calls: v3 collects the body on the transaction path.

| New case family | Distinct cases | Level / fixed expectation |
| --- | ---: | --- |
| `PR-L-raw-value-*`, `PR-L-raw-key-*`, `PR-L-escaped-value`, `PR-L-escaped-key`, `PR-L-complete-path`, `PR-L-incomplete-string` | 11 | Transaction: raw/decoded lengths around 8192 and up to 16384, complete long path and sibling, incomplete string retaining `json.a=1`. 8192 is not a ModSecurity boundary; a benchmark observer buffer is not adopted as a production limit |
| `PR-S-sequence-*` | 4 | Two fresh transactions on the same engine and RulesSet: syntax/depth/argument/completion error, then `{"ok":1}` with exactly `json.ok=1`, error flags 0 and an empty message |
| `PR-F-array-*` | 10 | Transaction: string, number, true, false and null, each with two/three elements at limit 2; exactly two entries, error flags 0 or 1 respectively, callback cancellation at the third element |
| `PR-C-utf8-*`, `PR-C-escape-*`, `PR-C-number-*`, `PR-C-root-number-*`, `PR-C-incomplete-number-*`, `PR-C-incomplete-string-*` | 111 | Internal ModSecurity JSON processor: every two-way split, including empty beginning/end, plus bytewise feeds; all parse calls succeed, complete tokens have complete=1, incomplete tokens have complete=0 |
| `PR-C-trailing-split-*` | 9 | Internal processor: trailing character rejected in the chunk, complete=0; `[0]` or `[1,0]` depending on the split, previously extracted `json.a=1` retained |
| `PR-C-16k-*` | 4 | Real parser boundary at byte 16384 in UTF-8, a surrogate escape, number or incomplete long string; the latter is identified only by complete=0 and its error class, not by the two successful chunk calls |

The three chunk families together are the 124 new internal parser cases. They
are **not direct YAJL-only tests** and do not change production interfaces. Byte
values are transported as hex before the call and reconstructed with explicit
length in the probe. Long fixtures are bounded for CI: new request cases reach
at most 49166 body bytes; this does not test allocation exhaustion or unbounded
documents. A successful bounded run proves neither SAX/DOM memory equivalence
nor thread safety. No schema validation or SAX/DOM migration was implemented
or reported as passing.

## 4. Requirements and policy proposals remain visibly failing

All 22 request cases below expect **no JSON arguments**, `REQBODY_ERROR=1`,
`REQBODY_PROCESSOR_ERROR=1`, `INBOUND_DATA_ERROR=0`, phase-2 marker `1`, and no
disruptive intervention (observer status 200). The message class is `lexical`,
except for the four rollback cases (`parse`, `parse`, `depth`, `cancel`). An error
flag alone does not force an HTTP error; the minimal rules here do not deny.
Exact bytes and configurations are in the
[requirements report](../../test/json-baseline/results/requirements-results.json)
and catalog. The JSON fixture loader does not repair invalid bytes.

| Category | Count | PASS / FAIL | Interpretation |
| --- | ---: | --- | --- |
| `REQ-UTF8-*` | 12 | 6 / 6 | Rejection of actually malformed UTF-8 bytes in key and value. RFC 8259 §8.1 requires UTF-8 for exchange between open systems; acceptance of overlong, out-of-Unicode-range or UTF-8-encoded surrogate sequences is not adopted as desired behavior |
| `REQ-POLICY-surrogate-*` | 6 | 0 / 6 | Separate proposal to reject unpaired/repeated surrogate escapes. RFC §8.2 describes interoperability problems; not every such escape spelling is prohibited by the JSON ABNF |
| `REQ-POLICY-rollback-*` | 4 | 0 / 4 | Open atomic-extraction policy from PR #5: discard all previously extracted JSON arguments on error. The current original retains `json.a=1`; this proposal is not an adopted compatibility contract |

| ID | Body bytes (hex) | Message class | Classification | Result |
| --- | --- | --- | --- | --- |
| `REQ-UTF8-key-continuation` | `7b2280223a2276227d` | `lexical` | UTF-8 requirement | **PASS** |
| `REQ-UTF8-value-continuation` | `7b2278223a2280227d` | `lexical` | UTF-8 requirement | **PASS** |
| `REQ-UTF8-key-overlong` | `7b22c0af223a2276227d` | `lexical` | UTF-8 requirement | **FAIL** |
| `REQ-UTF8-value-overlong` | `7b2278223a22c0af227d` | `lexical` | UTF-8 requirement | **FAIL** |
| `REQ-UTF8-key-truncated` | `7b22e282223a2276227d` | `lexical` | UTF-8 requirement | **PASS** |
| `REQ-UTF8-value-truncated` | `7b2278223a22e282227d` | `lexical` | UTF-8 requirement | **PASS** |
| `REQ-UTF8-key-above-unicode` | `7b22f4908080223a2276227d` | `lexical` | UTF-8 requirement | **FAIL** |
| `REQ-UTF8-value-above-unicode` | `7b2278223a22f4908080227d` | `lexical` | UTF-8 requirement | **FAIL** |
| `REQ-UTF8-key-surrogate` | `7b22eda080223a2276227d` | `lexical` | UTF-8 requirement | **FAIL** |
| `REQ-UTF8-value-surrogate` | `7b2278223a22eda080227d` | `lexical` | UTF-8 requirement | **FAIL** |
| `REQ-UTF8-key-ff` | `7b22ff223a2276227d` | `lexical` | UTF-8 requirement | **PASS** |
| `REQ-UTF8-value-ff` | `7b2278223a22ff227d` | `lexical` | UTF-8 requirement | **PASS** |
| `REQ-POLICY-surrogate-key-high` | `7b225c7564383030223a2276227d` | `lexical` | open Unicode policy proposal | **FAIL** |
| `REQ-POLICY-surrogate-value-high` | `7b2278223a225c7564383030227d` | `lexical` | open Unicode policy proposal | **FAIL** |
| `REQ-POLICY-surrogate-key-low` | `7b225c7564633030223a2276227d` | `lexical` | open Unicode policy proposal | **FAIL** |
| `REQ-POLICY-surrogate-value-low` | `7b2278223a225c7564633030227d` | `lexical` | open Unicode policy proposal | **FAIL** |
| `REQ-POLICY-surrogate-key-high-high` | `7b225c75643830305c7564383031223a2276227d` | `lexical` | open Unicode policy proposal | **FAIL** |
| `REQ-POLICY-surrogate-value-high-high` | `7b2278223a225c75643830305c7564383031227d` | `lexical` | open Unicode policy proposal | **FAIL** |
| `REQ-POLICY-rollback-syntax` | `7b2261223a31202262223a327d` | `parse` | open rollback policy proposal | **FAIL** |
| `REQ-POLICY-rollback-eof` | `7b2261223a312c` | `parse` | open rollback policy proposal | **FAIL** |
| `REQ-POLICY-rollback-depth` | `7b2261223a312c2278223a7b2279223a7b227a223a327d7d7d` | `depth` | open rollback policy proposal | **FAIL** |
| `REQ-POLICY-rollback-arguments` | `7b2261223a312c2262223a6e756c6c7d` | `cancel` | open rollback policy proposal | **FAIL** |

### Separate writer requirements and audit checks

Writer cases do not replace request parser cases. The
[writer report](../../test/json-baseline/results/extended-audit-results.json) and
[`run_extended_audit.py`](../../test/json-baseline/run_extended_audit.py) define
exact field sets and types, contents and absences. Audit records are decoded
strictly as UTF-8 and checked structurally as JSON; duplicate headers remain
member lists. Variable time/server metadata are specifically checked for type
and nonempty content.

- `AUDX-C01`–`AUDX-C07` check all 32 control bytes, JSON-looking text, embedded
  NUL, Unicode, long values, emptiness and the existing ASCII hex rendering of
  binary request data.
- `AUDX-P01`–`AUDX-P07` check individual audit parts, including expected absence;
  `AUDX-L01` checks three identical serializations, and `AUDX-L02` a fresh
  transaction without retained headers/body data.
- `AUDX-R01` requires valid UTF-8 JSON for invalid response bytes. It fails during
  strict UTF-8 decoding. The future representation (for example escape/hex or
  rejection) remains open; the test is not weakened into an arbitrary string
  comparison.
- `OFFSET-C01` records the current malformed result from the public
  `ModSecurity::processContentOffset` API; this is not a desired output format.
  `OFFSET-C02/C03` check range errors. `OFFSET-R01/R02/R03` require valid JSON and,
  for the single range and transformation, specific structure and values. All
  three already fail on JSON syntax. This does not demonstrate successful
  checking of subsequent value assertions; they remain in the tests for a later
  fix.
- `AUDX-M01` requires rejection of an incorrect expectation. `AUDX-M02` requires
  acceptance of a synthetic permitted alternative encoding. Both are harness
  controls, rather than additional product capabilities.

| ID | Level | Fixed expectation / purpose | Result |
| --- | --- | --- | --- |
| `AUDX-C01` | audit writer | all control bytes, exact request/response distinction | **PASS** |
| `AUDX-C02` | audit writer | JSON-looking text remains a string | **PASS** |
| `AUDX-C03` | audit writer | embedded NUL is length-preserved | **PASS** |
| `AUDX-C04` | audit writer | Unicode header names and line-separator values | **PASS** |
| `AUDX-C05` | audit writer | bounded long escaped values | **PASS** |
| `AUDX-C06` | audit writer | empty request versus prefixed response | **PASS** |
| `AUDX-P01` | audit writer | A alone has mandatory metadata | **PASS** |
| `AUDX-P02` | audit writer | B includes request headers only | **PASS** |
| `AUDX-P03` | audit writer | C includes request body only | **PASS** |
| `AUDX-P04` | audit writer | E includes response body only | **PASS** |
| `AUDX-P05` | audit writer | F includes response headers only | **PASS** |
| `AUDX-P06` | audit writer | H includes producer and empty messages | **PASS** |
| `AUDX-P07` | audit writer | other parts do not add JSON fields | **PASS** |
| `AUDX-L01` | audit writer | three serializations preserve transaction state | **PASS** |
| `AUDX-L02` | audit writer | new transaction has no previous body or headers | **PASS** |
| `AUDX-C07` | audit writer | request binary data is ASCII hex-rendered | **PASS** |
| `AUDX-R01` | audit writer | response writer output is valid UTF-8 JSON | **FAIL** |
| `OFFSET-C01` | public offset API | single offsets currently omitted | **PASS** |
| `OFFSET-C02` | public offset API | start at byte length is rejected | **PASS** |
| `OFFSET-C03` | public offset API | start beyond byte length is rejected | **PASS** |
| `OFFSET-R01` | public offset API | empty match produces valid JSON (representation otherwise open) | **FAIL** |
| `OFFSET-R02` | public offset API | single variable range and operator range round-trip | **FAIL** |
| `OFFSET-R03` | public offset API | transformation output is valid structural JSON | **FAIL** |
| `AUDX-M01` | harness control | wrong expectation rejected | **PASS** |
| `AUDX-M02` | harness control | valid alternative invalid-byte encoding accepted | **PASS** |

## 5. All 188 new request characterization cases

Each row below denotes exactly one case from the
[extension report](../../test/json-baseline/results/extended-request-results.json).
`ARGS` is the expected entry count in the first/only transaction; exact names,
values and multiplicities are under `expected.args_hex` in the report and
[catalog](catalog.json). For sequences, `expected.second_transaction`
additionally contains the complete second expectation. `T` denotes the
transaction API, `S` two transactions, and `P` the real internal ModSecurity JSON
processor. For `T/S`, state `E/P/I/Phase/Status` denotes expectations for
REQBODY_ERROR, REQBODY_PROCESSOR_ERROR, INBOUND_DATA_ERROR, phase-2 marker and
intervention status. For `P`, it instead gives `complete` and the location of an
intentional error; `expected.chunk_results`, `expected.pre_args_hex` and `chunks`
contain exact intermediate states. Group `K` here denotes additional
resource/lifecycle checks. All states are fixed expectations; PASS requires the
complete comparison, rather than only the entry count.

| ID | Groups | Level | Body bytes | ARGS | Expected state | Result |
| --- | --- | --- | ---: | ---: | --- | --- |
| `PR-N-token-kinds` | H | T | 92 | 6 | `0/0/0/1/200` | PASS |
| `PR-N-array-token-kinds` | D,H | T | 43 | 8 | `0/0/0/1/200` | PASS |
| `PR-B-dotted-parent-sibling` | B,D | T | 35 | 3 | `0/0/0/1/200` | PASS |
| `PR-B-dotted-array-sibling` | B,D | T | 33 | 3 | `0/0/0/1/200` | PASS |
| `PR-B-nested-nul` | B,G | T | 45 | 2 | `0/0/0/1/200` | PASS |
| `PR-G-literal-backslash-surrogate` | B,G | T | 35 | 2 | `0/0/0/1/200` | PASS |
| `PR-G-high-ordinary` | G | T | 15 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-key-high-ordinary` | B,G | T | 15 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-high-high` | G | T | 20 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-key-high-high` | B,G | T | 20 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-low-high` | G | T | 20 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-key-low-high` | B,G | T | 20 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-high-end` | G | T | 14 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-key-high-end` | B,G | T | 14 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-low-end` | G | T | 14 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-key-low-end` | B,G | T | 14 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-raw-surrogate-value` | G | T | 11 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-raw-surrogate-key` | B,G | T | 11 | 1 | `0/0/0/1/200` | PASS |
| `PR-G-ff-key` | B,G | T | 9 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-G-raw-nul-key` | B,G | T | 11 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-G-raw-nul-value` | G | T | 11 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-J-trailing-raw-nul` | J | T | 8 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-J-syntax-colon` | J | T | 7 | 0 | `1/1/0/1/200 (parse)` | PASS |
| `PR-J-syntax-comma` | J | T | 13 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-J-syntax-equals` | J | T | 7 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-J-syntax-object-comma` | J | T | 8 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-J-syntax-array-comma` | J | T | 4 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-J-syntax-block-comment` | J | T | 20 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-J-syntax-line-comment` | J | T | 18 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-H-invalid-dot-one` | H,J | T | 8 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-H-invalid-exponent-plus` | H,J | T | 9 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-H-invalid-double-minus` | H,J | T | 9 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-H-decimal30` | H | T | 38 | 1 | `0/0/0/1/200` | PASS |
| `PR-H-negative-zero` | H | T | 10 | 1 | `0/0/0/1/200` | PASS |
| `PR-H-huge-exponent` | H | T | 14 | 1 | `0/0/0/1/200` | PASS |
| `PR-H-tiny-exponent` | H | T | 14 | 1 | `0/0/0/1/200` | PASS |
| `PR-H-exponent-zeroes` | H | T | 13 | 1 | `0/0/0/1/200` | PASS |
| `PR-H-long-integer` | H | T | 518 | 1 | `0/0/0/1/200` | PASS |
| `PR-H-long-incomplete-exponent` | H,J | T | 519 | 0 | `1/1/0/1/200 (lexical)` | PASS |
| `PR-F-array-string-2` | D,F | T | 9 | 2 | `0/0/0/1/200` | PASS |
| `PR-F-array-string-3` | D,F | T | 13 | 2 | `1/1/0/1/200 (cancel)` | PASS |
| `PR-F-array-number-2` | D,F | T | 13 | 2 | `0/0/0/1/200` | PASS |
| `PR-F-array-number-3` | D,F | T | 19 | 2 | `1/1/0/1/200 (cancel)` | PASS |
| `PR-F-array-true-2` | D,F | T | 11 | 2 | `0/0/0/1/200` | PASS |
| `PR-F-array-true-3` | D,F | T | 16 | 2 | `1/1/0/1/200 (cancel)` | PASS |
| `PR-F-array-false-2` | D,F | T | 13 | 2 | `0/0/0/1/200` | PASS |
| `PR-F-array-false-3` | D,F | T | 19 | 2 | `1/1/0/1/200 (cancel)` | PASS |
| `PR-F-array-null-2` | D,F | T | 11 | 2 | `0/0/0/1/200` | PASS |
| `PR-F-array-null-3` | D,F | T | 16 | 2 | `1/1/0/1/200 (cancel)` | PASS |
| `PR-S-sequence-syntax` | J,K | S | 13 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-S-sequence-depth` | E,J,K | S | 25 | 1 | `1/1/0/1/200 (depth)` | PASS |
| `PR-S-sequence-arguments` | F,J,K | S | 16 | 1 | `1/1/0/1/200 (cancel)` | PASS |
| `PR-S-sequence-eof` | J,K | S | 7 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-L-raw-value-8191` | K | T | 8199 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-raw-value-8192` | K | T | 8200 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-raw-value-8193` | K | T | 8201 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-raw-value-16384` | K | T | 16392 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-raw-key-8191` | B,K | T | 8202 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-raw-key-8192` | B,K | T | 8203 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-raw-key-8193` | B,K | T | 8204 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-escaped-value` | G,K | T | 49166 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-escaped-key` | B,G,K | T | 49166 | 1 | `0/0/0/1/200` | PASS |
| `PR-L-complete-path` | B,K | T | 8726 | 2 | `0/0/0/1/200` | PASS |
| `PR-L-incomplete-string` | J,K | T | 16397 | 1 | `1/1/0/1/200 (parse)` | PASS |
| `PR-C-16k-incomplete-string` | J,K | P | 16397 | 1 | `complete=0; error at complete` | PASS |
| `PR-C-utf8-split-000` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-001` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-002` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-003` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-004` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-005` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-006` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-007` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-008` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-009` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-010` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-011` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-012` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-013` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-014` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-015` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-split-016` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-utf8-bytewise` | G,K | P | 16 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-000` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-001` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-002` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-003` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-004` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-005` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-006` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-007` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-008` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-009` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-010` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-011` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-012` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-013` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-014` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-015` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-016` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-017` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-018` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-019` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-020` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-021` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-split-022` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-escape-bytewise` | G,K | P | 22 | 1 | `complete=1` | PASS |
| `PR-C-number-split-000` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-001` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-002` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-003` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-004` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-005` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-006` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-007` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-008` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-009` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-010` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-011` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-012` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-013` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-014` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-015` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-016` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-017` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-018` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-019` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-020` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-021` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-022` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-023` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-024` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-025` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-026` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-027` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-028` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-029` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-030` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-031` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-032` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-033` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-034` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-035` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-036` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-037` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-038` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-039` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-040` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-041` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-042` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-043` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-split-044` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-number-bytewise` | H,K | P | 44 | 1 | `complete=1` | PASS |
| `PR-C-root-number-split-000` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-root-number-split-001` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-root-number-split-002` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-root-number-split-003` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-root-number-split-004` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-root-number-split-005` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-root-number-bytewise` | C,H,J,K | P | 5 | 1 | `complete=1` | PASS |
| `PR-C-incomplete-number-split-000` | H,J,K | P | 3 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-number-split-001` | H,J,K | P | 3 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-number-split-002` | H,J,K | P | 3 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-number-split-003` | H,J,K | P | 3 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-number-bytewise` | H,J,K | P | 3 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-000` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-001` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-002` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-003` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-004` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-005` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-006` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-007` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-008` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-split-009` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-incomplete-string-bytewise` | J,K | P | 9 | 0 | `complete=0; error at complete` | PASS |
| `PR-C-trailing-split-0` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-1` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-2` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-3` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-4` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-5` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-6` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-7` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-trailing-split-8` | J,K | P | 8 | 1 | `complete=0; error at chunk` | PASS |
| `PR-C-16k-utf8` | G,K | P | 16389 | 1 | `complete=1` | PASS |
| `PR-C-16k-escape` | G,K | P | 16397 | 1 | `complete=1` | PASS |
| `PR-C-16k-number` | H,K | P | 16389 | 2 | `complete=1` | PASS |

The four additional request controls are `PR-N-token-kinds`, `PR-B-nested-nul`,
`PR-C-root-number-split-000` and `PR-J-syntax-colon` under `sensitivity[]` in the
report. Only the expectation is deliberately changed; PASS means that the checker
rejects the incorrect expectation. These are not counted again in the 188
product cases.

For traceability, the 21 valid baseline numeric literals (`H-number-*`) are
mapped explicitly again:

| Baseline ID | Expected unchanged number text |
| --- | --- |
| `H-number-00` | `0` |
| `H-number-01` | `-0` |
| `H-number-02` | `1` |
| `H-number-03` | `-1` |
| `H-number-04` | `-9223372036854775808` |
| `H-number-05` | `9223372036854775807` |
| `H-number-06` | `-9223372036854775809` |
| `H-number-07` | `9223372036854775808` |
| `H-number-08` | `18446744073709551615` |
| `H-number-09` | `18446744073709551616` |
| `H-number-10` | `9007199254740991` |
| `H-number-11` | `9007199254740992` |
| `H-number-12` | `9007199254740993` |
| `H-number-13` | `1.0` |
| `H-number-14` | `1.2300` |
| `H-number-15` | `1e2` |
| `H-number-16` | `1E+02` |
| `H-number-17` | `1e-2` |
| `H-number-18` | `1e400` |
| `H-number-19` | `1e-400` |
| `H-number-20` | `999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999` |

## 6. What this matrix explicitly does not prove

The ten required groups are linked to named cases and fixed expectations; a
large number of chunk splits does not prove completeness across all data forms.
New cases extend existing coverage; they replace neither the official
regressions nor an actual external connector run.

Not implemented/tested are a library migration, JSON Schema validation, a new
SAX/DOM design, a performance comparison, complete allocator/fault injection,
unbounded load, or complete offset/sanitization end-to-end contracts. Serial
transaction sequences do not prove concurrency safety. An internal parser case
does not itself set transaction error flags; its empty fields must not be read
as “transaction error-free.” Normal transactions additionally check processor
selection, ARGS/ARGS_NAMES/ARGS_GET/ARGS_POST, error states, rule phase and
intervention.

The [complete English analysis](../json-yajl-code-analysis.md) provides
commit-pinned code evidence for these distinctions. The
[reproduction instructions](README.en.md) describe the runners actually
available. Publication and test status are separate evidence: a prepared
repository is not a newly published GitHub repository; conversely, a successful
push would not make a failing requirement test pass.
