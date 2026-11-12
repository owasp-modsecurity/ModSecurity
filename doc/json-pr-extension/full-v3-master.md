# Complete official v3/master source with English JSON documentation

This delivery contains the complete official ModSecurity v3 source tree, including
`src/`, public `headers/`, build files, examples, original tests, security policy
and pinned submodules. The English JSON investigation, additional tests and
current execution reports are integrated alongside them. Start with
[README.JSON-TESTS.md](../../README.JSON-TESTS.md); the official
[README.md](../../README.md) is preserved unchanged.

## Exact source and fork branch

| Reference | Value |
|---|---|
| Read-only original | `https://github.com/owasp-modsecurity/ModSecurity.git` |
| Original branch | `v3/master` |
| Retrieved | October 15, 2026, 15:24:31 UTC / 17:24:31 CEST |
| Pinned official commit | [`dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd`](https://github.com/owasp-modsecurity/ModSecurity/commit/dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd) |
| Writable fork | `https://github.com/Easton97-Jens/ModSecurity.git` |
| New delivery branch | `test/json-yajl-v3-master-english-20261015` |
| Local worktree | `/workspace/work/json-yajl-pr-expansion/full-v3-english` |

The branch was created directly from the pinned official commit. Existing fork
branches were neither synchronized nor replaced. The source reference is fixed;
a later change to official `v3/master` does not silently change this result.
The final handover records the published fork commit after reading the remote
reference back. That result commit is distinct from the official source SHA.

[Full provenance and file inventory](full-v3-provenance.json) record the exact
comparison, submodule commits, environment, build, results and delivery files.
The delivery location is the [new fork branch](https://github.com/Easton97-Jens/ModSecurity/tree/test/json-yajl-v3-master-english-20261015).

## Original code and historical analysis

All 846 tracked original entries are retained, including their original gitlinks.
Production sources, public headers, root build files, official CI files and the
official README match the pinned original. The existing-file task changes are
confined to the native JSON regression fixture and the test distribution list.
The added JSON workflow builds this complete checkout and checks it against the
pinned reference. YAJL remains unchanged; there is no parser migration or
production fix to make an assertion pass.

The earlier English analysis and archived reports describe official commit
`8271c9d3bc0abcbd1189f5414c196c336c959047`. Their counts and source references remain
historical. Comparing that source with the current reference changes five files:
root `Makefile.am`, `headers/modsecurity/rules_set_properties.h`,
`src/request_body_processor/multipart.cc`, the upload-mode regression fixture,
and `test/test-suite.in`. JSON request processing, transactions, JSON writers and
the relevant public transaction header are byte-identical in this comparison.
The current results below are nevertheless fresh executions on the new source.

A new engine build was required: the intervening header change makes
`ConfigValue::parse` virtual and changes the upload-mode value type. Reusing the
old compiled library with new headers would not establish a valid test. The new
shared library has SHA-256
`d9ccbbc719645a630969680d99a4c07d36e0dc8a4b9c129095948790e29e8f21`;
loader and probe manifests establish that this library was actually used.

## Executed results

| Execution level | PASS | FAIL | SKIP | BLOCKED | XFAIL |
|---|---:|---:|---:|---:|---:|
| Unmodified official source and existing relevant native tests, before overlay | 53 | 0 | 0 | 0 | 0 |
| Current source with characterization tests, including native JSON and controls | 399 | 0 | 0 | 0 | 0 |
| Current source with additional requirements and explicit policy proposals | 6 | 20 | 0 | 0 | 0 |
| Combined extended execution | 405 | 20 | 0 | 0 | 0 |
| Limited ASan/UBSan repeat of internal-parser cases | 124 | 0 | 0 | 0 | 0 |

The first row includes the original six JSON cases, 44 other relevant original
cases and three current upload-mode cases. It is not an execution of every test
in the repository. The 399 characterization checks comprise 163 baseline request
checks, 192 expanded request/parser checks, 12 baseline audit checks, 21 expanded
writer/control checks and 11 native JSON cases. Controls and repeated checks
are identified by the underlying reports; these rows are not additive unique
requirements. The combined command returns **exit 1** because requirements fail.

The 20 failures remain ordinary FAIL: six malformed-UTF-8 rejection requirements,
six open surrogate-policy proposals, four proposed atomic-extraction/rollback
requirements, one audit-output encoding requirement and three public offset-writer
requirements. The six passing requirements reject other malformed UTF-8 inputs.
No broad XFAIL, relaxed regex or production change masks the unmet assertions.
A characterization PASS does not endorse questionable current behavior.

Machine-readable evidence:

- [Original build and 53-test baseline](full-v3-original-baseline.json).
- [Toolchain and dependency environment](full-v3-environment.json).
- [Current combined execution, commands, counts, library/probe hashes and loaders](full-v3-validation.json).
- Current individual reports under [`test/json-baseline/results/`](../../test/json-baseline/results/), named `full-v3-*.json`.
- [All ten A–J groups and explicit expectations](matrix.en.md), supplemented by the current reports. The older reports retain their original filenames and references.

The current limited ASan/UBSan run repeats 124 internal-parser cases. Only the
helper and unchanged `json.cc` are instrumented; the fresh current engine library
and YAJL are not fully instrumented. Leak checking is disabled (`detect_leaks=0`).
[Exact compile/run commands and scope](../../test/json-baseline/results/full-v3-sanitizer-results.json)
are retained. This repeat adds no unique functionality; earlier sanitizer evidence
under `8271c9d3` is not counted as a run of this source.
A remote GitHub Actions run is not asserted to have passed. Its requirements job
keeps a nonzero exit when these assertions remain unmet.

## Reproduction

Clone into a new directory and initialize the exact stored module commits:

```sh
git clone --single-branch --no-tags --branch test/json-yajl-v3-master-english-20261015 https://github.com/Easton97-Jens/ModSecurity.git modsecurity-v3-json-en
cd modsecurity-v3-json-en
git submodule update --init --recursive
```

Use the official README for dependencies. The added
[JSON workflow](../../.github/workflows/json-tests.yml) lists Debian/Ubuntu
development packages and runs the full-source build. With those dependencies
installed in normal system locations:

```sh
sh -e ./build.sh
./configure --enable-assertions=yes --enable-parser-generation=no \
  --without-geoip --with-yajl --with-pcre2 --with-curl --with-libxml \
  --with-lua --with-lmdb --with-maxmind --with-ssdeep=/usr
make -j3
python3 test/json-baseline/tools/build.py --source "$PWD" --output ../json-probes
python3 test/json-baseline/tools/run_all.py --probes ../json-probes --output ../json-results --profile all
```

The final command is expected to return exit 1 on the examined source. Its output
directory contains `summary.json`, per-suite JSON reports and separate stdout and
stderr logs. Choose new output directories; the helpers refuse existing paths
and writes inside the source checkout. Use `--profile characterization` or
`--profile requirements` with separate new output directories to run each group.
No Apache or nginx service is needed for these v3 API tests.

The actual session used relocated Debian dependencies from
`/workspace/modsecurity-env/env.sh`, GCC/G++ 14.2.0 and Debian YAJL `2.1.0-5+b2`.
The environment and original-build reports preserve the exact actual configure
arguments, including relocated include/library paths and the new install prefix.
They also identify the raw log directory outside versioned sources. The system
package recipe above is a reproducible adaptation; it is not represented as a
second execution with system-installed packages in this session.

`tools/build.py` pins this branch's current original commit and refuses tracked
production differences or mismatched modules. It records the fresh library and
probe hashes; `run_all.py` checks those hashes, actual loader resolution and active
JSON support. Its current-source pin and report labels were updated explicitly;
historical report contents and fixed fixture expectations were not rewritten.

## Remaining limits

This task does not choose a replacement library, provide JSON Schema validation,
test every external connector, establish thread safety or prove equal SAX/DOM
memory use. Bounded fixtures are not an allocation-failure campaign. The full
source is provided, but validation is limited to the executed levels listed above.
Unrelated tests, release generation and publication of tags or pull requests are
not claimed as completed.
