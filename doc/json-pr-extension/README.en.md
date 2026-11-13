# JSON test coverage and expectations

See [Build and run](../../README.JSON-TESTS.md) for the single current setup
procedure. Production code and YAJL are unchanged by this test package.

## Included suites

All paths below are under `test/`.

| Suite | Entry point | Scope |
|---|---|---|
| Request baseline | `json-baseline/run.py` | Exact argument names/values, duplicates, types, malformed input, limits and rule/transaction behavior |
| Expanded request/parser | `json-baseline/run_extended.py` and `pr_cases.py` | Number text, dotted paths, NUL/Unicode, bounded long input, real parser chunk/completion calls and transaction isolation |
| Audit baseline | `json-baseline/run_audit.py` | Audit JSON structure, field types, selected parts and duplicate headers |
| Expanded writer characterization | `json-baseline/run_extended_audit.py --suite characterization` | Escaping, repeated output, lifecycle and public content-offset API observations |
| Native JSON regression | `test-cases/regression/request-body-parser-json.json` | Existing native cases plus the five investigation additions |
| Request requirements | `json-baseline/run_requirements.py` | UTF-8 rejection and explicitly proposed surrogate/atomic-extraction policies |
| Writer requirements | `json-baseline/run_extended_audit.py --suite requirements` | Valid JSON output, offset ranges and transformation values |

Case definitions and fixed assertions in the runners are authoritative for the
current revision. Binary fixtures preserve byte lengths, duplicate values and
embedded NULs. Assertion controls deliberately supply wrong expectations; they
do not count as additional product behavior. Request controls run only after
the selected behavior cases pass; a control that then crashes still fails.

## Decisions still open

Characterization and requirements are separate profiles, not alternative
success criteria. Existing partial extraction and unusual encoding behavior
are observations, not permanent compatibility promises. The requirements for
atomic rollback and surrogate rejection remain explicit policy proposals.
Invalid-output requirements retain real failures. Offset observations record
raw bytes without requiring malformed output to persist after a future fix.

These tests do not establish thread safety, allocation-exhaustion resilience,
full sanitizer coverage or acceptance of unusual bytes by an HTTP server.
Bounded inputs and sequential transaction checks must not be described as those
guarantees. Read the actual source SHA and counts in each generated report.

## Origins and archived evidence

The coverage follows the behavior-first approach in
[ModSecurity #3308](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842)
and incorporates independently implemented ideas from
[jsonbench PR #4](https://github.com/digitalwave/jsonbench/pull/4) and
[PR #5](https://github.com/digitalwave/jsonbench/pull/5). Number-token distinctions,
path/byte handling, Unicode and streaming cases were adapted to ModSecurity's
own interfaces. No jsonbench adapter implementation is copied or used as the
specification for ModSecurity.

The [original investigation snapshot](https://github.com/Easton97-Jens/ModSecurity/tree/dd63a59e3a993faf4e74b08979e39986e1df4d2c)
retains the detailed historical narrative. JSON catalogs/provenance in this
directory and [archived results](../../test/json-baseline/results/) retain their
original revisions and timestamps; they are not regenerated current evidence.
The current PR's changes are recorded in Git rather than duplicate review diaries.
