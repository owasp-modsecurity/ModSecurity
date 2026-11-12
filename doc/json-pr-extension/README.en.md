# More JSON/YAJL tests for ModSecurity v3

This extension adds independently implemented tests inspired by
[jsonbench PR #4](https://github.com/digitalwave/jsonbench/pull/4),
[PR #5](https://github.com/digitalwave/jsonbench/pull/5),
[fzipi's comment](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842)
and [airween's comment](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6142615388).
The tests exercise ModSecurity itself. YAJL and production code are unchanged.

This full-source English delivery is based on the official `v3/master` snapshot
[`dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd`](https://github.com/owasp-modsecurity/ModSecurity/commit/dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd).
It includes the engine, public headers, build system and original tests, alongside
the additional tests and English documentation. Start with the
[full-source index](../../README.JSON-TESTS.md) and
[current build/results report](full-v3-master.md).

The remainder of this extension report preserves the earlier investigation's
`8271c9d3` reference, observations and counts. The new current-source results are
recorded separately; no older PASS is silently assigned to the newer source.

## Original reference and scope

The tested production reference remains official commit
[`8271c9d3bc0abcbd1189f5414c196c336c959047`](https://github.com/owasp-modsecurity/ModSecurity/commit/8271c9d3bc0abcbd1189f5414c196c336c959047).
The prior published baseline is fork commit
[`2bfb2180d8c0fa35ae7a3f2665e999600607d896`](https://github.com/Easton97-Jens/ModSecurity/commit/2bfb2180d8c0fa35ae7a3f2665e999600607d896).
The extension does not substitute a newer production base.

The newly fetched official `v3/master` tip was
[`2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641`](https://github.com/owasp-modsecurity/ModSecurity/commit/2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641).
Its direct diff from the pinned original changes
`headers/modsecurity/rules_set_properties.h`,
`src/request_body_processor/multipart.cc`,
`test/test-cases/regression/config-upload_file_mode.json` and `test/test-suite.in`.
The JSON request processor, transaction implementation and public transaction
header, and `src/modsecurity.cc` are unchanged in that comparison. This static
comparison is not a runtime test of the newer tip.

[Provenance](provenance.json) records source retrieval and task references.
The [coverage matrix](matrix.en.md), [case catalog](catalog.json) and
[final result summary](results-summary.json) give the explicit mappings,
expectations and executed results. The summary is authoritative for final
aggregate counts; the historical baseline reports retain their original counts.

## What was added

The original ten groups A–J remain covered by the prior suite. These additions
extend that coverage rather than replace it:

| Source/topic | New coverage | Test IDs or suite |
|---|---|---|
| PR #4: distinguish keys, string values and number tokens | Numeric-looking keys/strings; exact exponent, negative-zero and large-number text | `PR-N-*`, `PR-H-*` |
| PR #4/#5: path and byte handling | Dotted parent paths, array siblings, nested NUL, long decoded keys/values and complete paths | `PR-B-*`, `PR-L-*` |
| Unicode and escape cases raised in the PRs/comments | Literal backslash versus escape, lone/repeated surrogates, malformed UTF-8 and raw control bytes | `PR-G-*`, `REQ-UTF8-*`, `REQ-POLICY-surrogate-*` |
| Streaming, completion and parser lifecycle | Every two-way split and bytewise input for selected tokens, real 16 KiB parser boundaries, chunk error versus completion error | `PR-C-*` |
| Limits and state | Every scalar callback at/over an argument limit; error followed by success on the same engine with a new transaction | `PR-F-*`, `PR-S-*` |
| Malformed/trailing/incomplete input | Missing separators, extra commas, comments, trailing NUL, truncated exponent and long string | `PR-J-*`, `PR-H-invalid-*`, `PR-C-16k-incomplete-string` |
| PR #5 atomic extraction proposal | Explicit proposed rollback after syntax, completion, depth and argument-limit errors | `REQ-POLICY-rollback-*` |
| Writer/public YAJL dependency from fzipi's comment | Audit parts, controls, NUL, Unicode, duplicate headers, field types, repeated serialization and fresh transaction | `AUDX-C*`, `AUDX-P*`, `AUDX-L*` |
| Separate public JSON-generating API | `ModSecurity::processContentOffset`: bounds, current malformed output, valid-output and single-range/transformation requirements | `OFFSET-C*`, `OFFSET-R*` |

The PR heads/files used as inspiration are pinned in the provenance and individual
case `sources` entries. Their adapter behavior does not define ModSecurity's
contract. In particular, an 8192-byte observation buffer in a benchmark is not a
ModSecurity limit. Test ideas were implemented independently against actual
ModSecurity interfaces; no jsonbench adapter or test implementation was copied.
The included ModSecurity sources/documentation retain their original licensing
and attribution. Referenced projects retain their own licenses; this work does
not relicense their implementations.

## Characterization and unmet requirements

The profiles answer different questions:

- **Characterization** checks fixed, byte-exact expectations for the current
  original. Passing does not approve malformed Unicode or malformed writer output
  as desired compatibility behavior.
- **Requirements** checks useful output guarantees and explicitly labelled policy
  proposals. Unmet assertions remain ordinary **FAIL**, with a nonzero runner
  exit. No XFAIL conversion, broad alternative regex or production fix hides them.
- **Assertion controls** deliberately change expectations or supply synthetic
  alternative output to prove that the checker discriminates. They are counted
  separately from unique product requirements.

The new request characterization report currently records 192 PASS, including
four assertion controls. The separate request requirements record 6 PASS and
16 FAIL. The extended writer report records 21 PASS and 4 FAIL: nineteen current
behavior cases, two checker controls and four unmet writer requirements. These
are per-suite counts; use [the final summary](results-summary.json) for the
complete run, including preserved baseline and native cases.

The observed requirement failures concern three distinct questions:

1. Some malformed UTF-8 sequences are accepted in keys and values. Other invalid
   byte sequences are rejected, accounting for the passing UTF-8 requirements.
2. Rejection of lone/repeated surrogate escapes and atomic removal of partial
   JSON arguments are **policy proposals**, not established requirements of the
   current ModSecurity API. RFC 8259 §8.2 explains surrogate interoperability
   problems; its grammar does not make every lone surrogate escape a syntax error.
3. Invalid response bytes can yield audit output that is not valid UTF-8 JSON.
   `processContentOffset` can return success with malformed JSON and omit a lone
   range. Its three requirements retain specific structural/value assertions for
   a future fix. The invalid-byte output requirement leaves the eventual encoding
   or rejection policy open.

These are reproducible correctness observations and open product decisions, not
claims of demonstrated exploitability. No production repair is included.

## Reproduce with portable helpers

The companion repository contains tests, not a duplicate ModSecurity source tree.
Use a separate complete source checkout with the original dependencies. The
[historical build report](../en/json-yajl-baseline-v3.md#reproduction) lists the
Debian packages. Python 3, a C++17 compiler, `git`, `pkg-config`, the YAJL
development package and `ldd` are required by the portable helpers. Building
ModSecurity additionally needs its documented Autotools/parser and library
dependencies. The executed environment is recorded in provenance; no Apache or
nginx service is needed for these API tests.

From the companion or task repository root, choose new output directories:

```sh
python3 test/json-baseline/tools/prepare_source.py \
  --destination "$PWD/../modsecurity-json-original" --build
python3 test/json-baseline/tools/build.py \
  --source "$PWD/../modsecurity-json-original" \
  --output "$PWD/../json-probes"
python3 test/json-baseline/tools/run_all.py \
  --probes "$PWD/../json-probes" \
  --output "$PWD/../json-characterization-results" \
  --profile characterization
python3 test/json-baseline/tools/run_all.py \
  --probes "$PWD/../json-probes" \
  --output "$PWD/../json-requirements-results" \
  --profile requirements
```

The requirements command is expected to return nonzero while requirements fail;
its exit must be retained. To execute both profiles in one report, use
`--profile all` with another new output directory. This is a repeated execution,
not additional unique coverage. Each run writes `summary.json`, suite reports and
stdout/stderr logs into its output directory. A failed preflight produces
explicitly labelled suite-level BLOCKED records, not invented case results.

`prepare_source.py` clones the fork into an absent directory, fetches the pinned
official commit, uses detached HEAD and initializes only the recorded submodules.
It does not modify an existing checkout or push anywhere. `build.py` can instead
use an already built pinned source tree through `--source`; it records the
compiler, source/submodule identity and library/probe hashes. Normal `CXX`,
`CPPFLAGS`, `LDFLAGS` and `PKG_CONFIG_PATH` may identify relocated dependencies.

The runner verifies the original production tree, loader resolution, artifact
hashes and an active JSON canary before running suites. Headers/JSON bytes use
explicit lengths, and binary fixtures cross the harness as hex. No extra newline
is added to the request body. Arguments are lists of byte pairs, preserving
duplicates and NUL; number tokens are not parsed and reformatted by the fixture
loader. Timeout, crash, missing dependency and intentional parser rejection are
different outcomes. A mutation control must reach an assertion before it counts
as a successful rejection of a false expectation.

The request tests use the public Transaction lifecycle or the real internal
ModSecurity JSON processor with an actual transaction. Connector append calls
and parser chunk boundaries are explicitly distinguished. Audit and offset
writers are separate suites, and synthetic unusual header bytes do not prove
acceptance by an external HTTP server.

## Complete documentation in English

This package contains the complete English translations of the prior baseline,
including every original section, table, code/source reference, historical count
and caveat. Historical reports are not silently rewritten with extension results.
The separate German companion retains both language versions.

| Document | Complete English version |
|---|---|
| Original baseline overview | [Complete report](../JSON-YAJL-BASELINE.md) |
| How requests work, technical analysis and fzipi statement checks | [Complete report](../json-yajl-code-analysis.md) |
| Original executable v3 matrix, environment and reproduction | [Complete report](../json-yajl-baseline-v3.md) |
| Original ModSecurity project README | [Complete historical original](../reference/en/ModSecurity-README.md) |
| Extension and execution | [This report](README.en.md) |
| Source-to-case coverage matrix | [Complete matrix](matrix.en.md) |

The [historical provenance manifest](../json-yajl-provenance.json), new
[provenance](provenance.json), [catalog](catalog.json) and
[result summary](results-summary.json) are shared machine-readable evidence.
The historical manifest records its original source URL and SHA-256; literal
reference names, paths, SHAs and observations remain unchanged. Original Doxygen
configuration and logo remain language-neutral archival references, requiring
the full original source tree to generate API docs.

## Limits of the evidence

No JSON library migration, DOM/SAX conversion, benchmark ranking, equal-memory
claim or full external connector test is included. Serial transaction isolation
does not establish thread safety. Bounded large fixtures do not test unbounded
documents or allocation exhaustion. Any limited sanitizer run records its
instrumented scope separately; it does not establish a fully instrumented
engine/YAJL build. Complete resource-failure and offset/sanitization contracts
remain separate follow-up work. Every actual publication status must be reported
separately from validation status; a prepared repository is not a published one.

## Final functional totals

The fresh characterization profile records **399 PASS**, comprising 163 baseline
request checks, 192 expanded request/parser checks, 12 baseline audit checks,
21 expanded writer/controls and 11 native JSON regressions. The requirements
profile records **6 PASS / 20 FAIL** and exit 1. Combined: **405 PASS / 20 FAIL**,
exit 1. SKIP, BLOCKED and XFAIL are zero for these functional profiles.
The separate, limited ASan/UBSan execution repeats 124 internal-parser cases:
124 PASS, no failures. Only the helper and unchanged JSON processor were
instrumented; leak detection was disabled. These repeated checks are not new
unique functionality. Exact commands and limitations are in provenance/reports.

JSON Schema support mentioned by airween, a jsoncons SAX rewrite, vendoring a
replacement library and removing connector dependencies are **outside this
unchanged-YAJL task**. They are explicitly mapped as unimplemented capabilities,
not marked PASS by syntax tests. PR #5 includes PR #4; their cases are not counted
twice, and benchmark assertion counts do not become ModSecurity results.


## Prepared repository artifacts

Each ZIP includes every tracked file, binary logo and fixture. Each Git bundle
also contains the complete independent handover history. Restore it into a new
absent directory without any new GitHub repository being required:

```sh
git clone ModSecurity-JSON-Tests-EN.bundle ModSecurity-JSON-Tests-EN
git -C ModSecurity-JSON-Tests-EN bundle verify ../ModSecurity-JSON-Tests-EN.bundle
```

Use the DE filenames for the bilingual German repository. The export manifest
lists both local Git SHAs, every file hash, identical executable test files and
the distinct documentation language profiles. The packaging helpers belong to the [full ModSecurity fork](https://github.com/Easton97-Jens/ModSecurity), require its original history, and are excluded from this English companion. Use the standalone build and run helpers included here.
The optional `patches/native-regressions.patch` targets the fixed official base
and carries native JSON cases plus the test distribution list. Copy the entire
`test/json-baseline/` directory into a fresh full source checkout before applying
that patch. Standalone API runs do not require it.


The fresh preparation path without `--build` completed successfully: fork clone, immutable official checkout and seven recursive submodule commits were verified. A second complete engine rebuild and a GitHub Actions run were not executed; runtime suites use the existing original-matching build. Exact archival copies retain their original whitespace; the staged whitespace check passes for all other new files.
