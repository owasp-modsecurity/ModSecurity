# ModSecurity v3: complete JSON/YAJL test package

This branch contains the complete existing JSON investigation test package from
[`test/json-yajl-v3-master-english-20261015`](https://github.com/Easton97-Jens/ModSecurity/tree/dd63a59e3a993faf4e74b08979e39986e1df4d2c),
ported onto upstream `v3/master` at
`1ebdc390b47ab033c8267130ce83130f235aed20` for
[PR #3752](https://github.com/owasp-modsecurity/ModSecurity/pull/3752).
It supersedes that PR's initial scope of only five additional native cases.

YAJL and production code are unchanged. This is not a parser replacement,
a selection of a JSON library, or a claim that every proposed policy is agreed.
The official project [README](README.md) remains the starting point for
ModSecurity, dependencies, connectors and contribution guidance.

## Current revision and validation status

- **Original test snapshot:** `dd63a59e3a993faf4e74b08979e39986e1df4d2c`.
- **Pinned production reference:** `1ebdc390b47ab033c8267130ce83130f235aed20`.
- **Existing PR branch:** `test/json-characterization-upstream-20261112`.
- **Review corrections:** all seven suites remain included. Harness corrections,
  including the offset and repeat-transaction checks, are described in the
  [integration report](doc/json-pr-extension/pr3752-upstream-20261112.md).
- **Clean-build provenance:** the helper now builds the selected commit in a new
  checkout and records both the library and native test runner. See the
  [build-integrity revision](doc/json-pr-extension/pr3752-build-integrity.md).
- **Validation:** 32 new local build-integrity/manifest tests pass, including a
  real compiler test using a small synthetic repository with stale binaries.
  This is not a new full ModSecurity or sanitizer result. Read CI evidence for
  the actual PR head; archived reports do not establish that a new revision passes.

## All included suites

| Suite | What it exercises |
|---|---|
| Request baseline | JSON body handling, byte-exact argument names/values, types, limits, errors and assertion controls |
| Expanded request/parser tests | jsonbench-derived cases, number text, numeric-looking keys/strings, dotted paths, embedded NUL, Unicode, bounded long inputs, direct chunk/completion calls and lifecycle behaviour |
| Audit baseline | Audit JSON fields, types, structure and log selection |
| Expanded writer characterization | Additional audit cases, escaping, repeated serialization and the public offset JSON writer |
| Native JSON regression | All six existing upstream cases plus all five additions from the investigation |
| Request requirements/policy | Additional encoding and atomic-extraction expectations; unmet expectations remain FAIL |
| Audit/writer requirements | Additional output guarantees; unmet expectations remain FAIL |

The complete [`test/json-baseline/`](test/json-baseline/) directory is included:
three C++ probes, every Python runner and case definition, both shell build
helpers, build/source-preparation/verification tools, the workflow template and
archived result files. The native cases remain in the existing
[`request-body-parser-json.json`](test/test-cases/regression/request-body-parser-json.json).
`test/Makefile.am` distributes the helper assets. The added
[JSON workflow](.github/workflows/json-tests.yml) runs both profiles separately,
using Ubuntu 26.04 and dynamic Lua development-package selection.

## Build and run everything

Install the dependencies listed in the [JSON workflow](.github/workflows/json-tests.yml).
From the repository root, select the actual commit to test:

```sh
git submodule update --init --recursive
python3 test/json-baseline/tools/build.py \
  --source "$PWD" --source-ref "$(git rev-parse HEAD)" \
  --output ../json-probes-pr3752
python3 test/json-baseline/tools/run_all.py \
  --probes ../json-probes-pr3752 --profile all \
  --output ../json-results-pr3752
```

`build.py` performs bootstrap, configure, the complete engine build and probe
compilation itself. It uses a new independent checkout inside the output
directory; it neither cleans nor reuses the input checkout's build products.
The selected commit must already be available in `--source`, whose required
submodules must be initialized. The fresh checkout initializes its own pinned
submodules, which may require network access. `--jobs` defaults to 2.

Without `--source-ref`, the helper selects the pinned production reference above.
For a separate pinned-source run, use `tools/prepare_source.py --destination ...`
without `--build`, then pass that checkout to `build.py`. There is no need for a
separate prebuild. Older manifests without clean-build provenance are rejected;
rebuild into a new directory instead of relabelling an old report.

Choose new output directories outside the source checkout, and keep results
outside the probe/build directory too. The runner executes **all seven suites**,
writes `summary.json` plus individual reports/logs, and returns nonzero for
failures or blocked execution. Use `--profile characterization` or
`--profile requirements` with separate new results directories to inspect each
profile. Source/submodule identity, library and native-runner hashes, probe
hashes, loader resolution and active JSON support are verified before execution.

**Do not expect `--profile all` to be green while proposed requirements remain
unmet.** No case is omitted or converted to XFAIL to force success.
A characterization PASS describes existing behaviour; it does not endorse it.
Ordinary `make check` includes the native fixture, not the standalone suites;
use the command above or the added workflow for the complete package.

## Provenance and historical evidence

The test ideas include my contributions to
[jsonbench PR #4](https://github.com/digitalwave/jsonbench/pull/4) and
[PR #5](https://github.com/digitalwave/jsonbench/pull/5), together with
[fzipi's proposal](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842)
and [airween's discussion](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6142615388).
They were independently adapted to ModSecurity's actual interfaces; benchmark
adapter implementations are neither copied nor treated as the specification.

| Documentation | Scope |
|---|---|
| [Build-integrity revision](doc/json-pr-extension/pr3752-build-integrity.md) | Fresh-build provenance, manifest validation and new tests |
| [November integration report](doc/json-pr-extension/pr3752-upstream-20261112.md) | Package integration and earlier review corrections |
| [PR/comment test extension](doc/json-pr-extension/README.en.md) | Mapping from jsonbench contributions and the issue discussion to tests |
| [Coverage matrix](doc/json-pr-extension/matrix.en.md) and [case catalog](doc/json-pr-extension/catalog.json) | Individual expectations and open decisions |
| [October full-source report](doc/json-pr-extension/full-v3-master.md) | Historical execution on `dd1d0f82672e`; not the November base |
| [Original baseline](doc/JSON-YAJL-BASELINE.md) and [v3 characterization](doc/json-yajl-baseline-v3.md) | Earlier historical investigation |
| [Code analysis](doc/json-yajl-code-analysis.md) | Historical v2/v3 processing-path analysis and discussion checks |
| [Archived results](test/json-baseline/results/) | Original per-case reports, including limited sanitizer evidence |

All archived provenance and result files retain their original references and
counts. Their use of "current" refers to their own dated source.
The October report records 399 passing characterization checks and separately
6 passing / 20 failing requirement checks. These include assertion controls and
repeated checks, **not 399 new native test cases**, and are **not fresh results
for this PR revision**. Source-pin adaptation does not transfer a PASS.

This package remains v3-focused. It does not implement the full v2 test plan,
choose a replacement library, prove equal SAX/DOM memory consumption, validate
all external connectors or establish thread safety. Bounded fixtures and the
archived limited sanitizer instrumentation do not prove allocation-exhaustion
resilience or full-engine sanitizer coverage.
