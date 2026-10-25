# Testing and evidence

## Select checks from the changed behavior

This is the validation policy of the working repository. It does not assert
that any check has already run. Choose evidence that resolves the task's concrete
acceptance criteria, using [build-and-test.md](build-and-test.md) for commands
and prerequisites.

The core suite is defined through the top-level Makefile, `test/test-suite.in`,
the test Makefile, and the actual runners. Preserve that scope when describing
a full suite. This policy does not impose the reference connector repository's
100-case inventory, historical qualification campaign, or host matrix on core.
Sources: [suite integration][suite], [test programs][programs].

## Documentation and policy changes

For a documentation-only change, read the affected source, check references,
review the diff, and run the repository's documentation checks when available:

```sh
python3 .codex/bin/verify-context.py --strict-source
python3 .codex/bin/validate-structure.py
git diff --check
```

Run these from the repository root. They are documentation-workflow commands,
not upstream C++ tests. Inspect their actual output and scope. Passing them does
not prove that every explanation is semantically correct, that external sites
were contacted, or that ModSecurity compiled.

A C++ build is not required merely because explanatory Markdown changed.
An executable helper change needs a check of its own behavior where material:
for example, verify that a reference checker rejects the specific invalid input
it is intended to reject, using task-owned temporary fixtures.

`--strict-source` is appropriate when the task promises unchanged upstream
source. If an authorized product change intentionally differs from the pinned
baseline, report that difference and review the affected context. Do not change
the pin merely to turn a source-drift failure into a passing result.

## Product and build changes

Choose focused tests for the affected behavior, including a relevant boundary
or error path when that is the reason for the change. Add a regression when it
captures an observable defect or requirement. Avoid tests that merely restate
the implementation or unrelated large test campaigns.

For build, parser, public API, or dependency changes, also check the affected
configuration and callers. Expand validation only to resolve a concrete remaining
risk or satisfy an applicable existing gate. Once the task's acceptance criteria
are sufficiently supported, finish the task and report remaining limits.

Do not disable, truncate, or rewrite tests to obtain a passing result. A broad
or platform-specific check that cannot run keeps its actual status. An automated
CI job is not exempt from truthful reporting because the local task was smaller.

## Core-specific evidence traps

| Observation | Required interpretation | Source |
| --- | --- | --- |
| Unix configuration succeeds | Confirm `TEST_UTILITIES` is enabled before claiming unit/regression binaries were built; activation depends on `YAJL_FOUND=1`. | [Test activation][activation], [conditional subdirectory][subdir]. |
| `regression_tests` exits `0` | Inspect output and counts: its `NO_LOGS` branch returns `0` without running the regressions. | [Runner branch][nologs]. |
| A case is skipped | Record it separately from executed passes; capability/resource selection matters. | [Regression resource check][resources], [suite driver][driver]. |
| A rule file loads | This demonstrates loading, not a complete HTTP transaction. `modsec-rules-check` loads through `RulesSet` and does not create a transaction. | [Rule checker][rules-check]. |
| A test JSON contains an expected result | It describes a test expectation, not a recorded execution. | Working evidence rule. |
| A core regression passes | Bind the claim to that build, case, configuration, and API path; external web-server behavior needs evidence from that separate integration. | [Core/connector boundary][connectors]. |

The actual test directory is `test/`. Direct runner examples rely on that
working directory; `test/test-suite.sh` explicitly changes into it. Read the
build guide before selecting individual files or `file.json:N` cases.
Source: [test driver][driver].

## Evidence layers and statuses

Keep source inspection, documentation validation, compilation, configuration
loading, unit tests, transaction regressions, static analysis, and external
host/runtime tests distinct. A result at one layer does not establish another.
Use [configuration-and-rules.md](configuration-and-rules.md) for rule behavior
and [cpp-guide.md](cpp-guide.md) for API and lifetime questions.

Use these working statuses consistently:

| Status | Meaning |
| --- | --- |
| `passed` | The stated check ran and satisfied its stated criterion. |
| `failed` | The check ran and did not satisfy that criterion. |
| `blocked` | A required prerequisite or authority prevented the check. |
| `not_run` | No execution occurred; state the reason. |
| `not_applicable` | The check does not apply to this scope; explain why. |

Retain native runner skip/disabled counts as separate evidence. Do not translate
an unavailable capability, missing fixture, cancelled job, or old successful
run into a fresh pass.

## Handover record

Record the exact command and directory, core commit and relevant submodule
revisions, local changes, build options, tool versions when material, selected
case/file, exit code, counts, important output, and limitations. Keep evidence
references accessible and remove secrets from retained excerpts. A subsequent
chat should be able to distinguish what was read, inferred, executed, and left
open without inventing a previous result.

[suite]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L88-L96
[programs]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L29-L99
[activation]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L313-L332
[subdir]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L2-L17
[nologs]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L437-L440
[resources]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L151-L183
[driver]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-suite.sh#L1-L40
[rules-check]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/rules-check.cc#L32-L93
[connectors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L40-L46
