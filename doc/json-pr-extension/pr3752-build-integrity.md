# PR #3752: bind test results to a clean engine build

This revision addresses the build-provenance and malformed-manifest review
findings on `a6ca10754666c1ac106b213e5a6c2a5008a527e0`.
All seven JSON suites remain included. No production source, archived result,
expected parser/writer behaviour or static-analysis suppression is changed.

## Why source identity and loader hashes were not enough

A checkout can move to a new commit while ignored `src/.libs` artifacts and the
native regression executable still come from an older build. Checking Git
contents, the library's hash and an active JSON canary does not establish that
this library was compiled from the recorded revision.

`tools/build.py` now performs the build itself in an independent local clone
under its new output directory. It checks out the full selected SHA, initializes
its pinned submodules, runs bootstrap/configure/make and only then compiles the
three test probes. Existing libraries, native runners and ignored/untracked
headers from the input checkout are not copied. The input checkout is neither
cleaned nor rebuilt. No destructive `git clean` or force update is used.

Build-manifest schema 2 contains the clean-build method, selected source SHA,
configure arguments and hashes/paths for the shared library, native executable
and libtool launcher. The source is checked before and after compilation.
`run_all.py` checks all three engine artifacts again before executing any suite,
in addition to the existing submodule, probe-hash, loader and JSON-canary checks.
A changed native runner cannot silently be substituted for the recorded one.
This is local build evidence, not a signed remote-build attestation.

The main workflow selects the Actions checkout SHA explicitly. The companion
workflow retains its pinned-source purpose. Both now rely on the managed clean
build rather than building twice. Evidence uploads include manifests, probe
binaries, build logs and test reports, not the complete cloned source tree.
Ubuntu 26.04, dynamic Lua detection and normal requirements failure status remain.

## Follow-up: generated submodule files are not source changes

The run of `7d043d5` stopped after compilation with
`Submodule has untracked sources: others/libinjection`. The post-build guard
incorrectly treated every non-ignored build output as an unexpected input.
Autotools/Libtool can create such files inside submodules during a normal build.

The input checkout and fresh clone are still checked strictly before building.
After successful bootstrap/configure/make, the helper records generated files
from the main tree and all verified recursive submodules, with exact paths and
SHA-256 hashes. It saves `engine-logs/generated-files.json` and includes that
inventory in the build manifest. Post-build and runtime source checks accept
only those recorded files and reject changed, missing or additional files.
Tracked source changes, wrong submodule commits, traversal and symlink aliases
remain errors. This is not a blanket ignore rule or a change to any submodule.

Both JSON workflows follow the main `ci_new.yml` checkout convention:
`actions/checkout@v7`, full history and recursive submodules. They retain its
dynamic Lua package discovery, include Bison/Flex and PCRE2 utilities, and pass
`nproc` to the managed build. Each build phase prints its command and log path.
The existing main QA workflow is unchanged.

## Malformed manifests and result reports

Both manifest-read paths reject non-object JSON before accessing fields.
Invalid values such as `null`, arrays, scalars, malformed source paths or probe
maps produce a `BLOCKED` summary for each selected suite instead of an
`AttributeError` without a report. No engine or probe is executed in this case.
Missing/old provenance requires rebuilding, not acceptance of a legacy PASS.
Case reports also validate their object/array shapes and individual statuses.
Results may not be written inside the probe/build directory.

NUL-delimited Git filename output is preserved without stripping leading
whitespace, so the untracked-file guard inspects the actual filenames.

## Reproduction

From a checkout with the normal dependencies and required submodules installed:

```sh
python3 test/json-baseline/tools/build.py \
  --source "$PWD" --source-ref "$(git rev-parse HEAD)" \
  --output ../json-probes-clean
python3 test/json-baseline/tools/run_all.py \
  --probes ../json-probes-clean --profile all --output ../json-results-clean
```

Both destinations must be new; results must also be outside the probe/build
folder. Do not run a separate engine prebuild for these commands. Omitting
`--source-ref` still selects `1ebdc390b47ab033c8267130ce83130f235aed20`.
Existing shell helpers remain available for standalone, unverified probe use;
they do not generate clean-build provenance accepted by the profile runner.

## Validation performed for this revision

For the original build-integrity revision, all 32 new tests in
`tools/test_build_integrity.py` passed locally. They include an actual
Git/Make/C++ fixture: an input repository retains version-1 binaries while its
source is committed as version 2. The clean build produces version 2 in both
the shared library and native executable without changing the stale input
binaries. A real shallow detached checkout is also tested. Other controls cover
modified artifacts, symlink escapes, failed builds, untracked sources,
malformed manifests and retention of genuine failures.

The generated-file follow-up adds 12 passing local tests with real Git
submodules (including a nested submodule), Libtool and compiled C++ fixtures.
They reproduce the old rejection without the generated-file record, accept the
normal build with the record, exercise profile preflight, and reject file
modification, deletion, unexpected additions and path/symlink escapes.

These tests validate the harness using small synthetic sources; they are not a
fresh ModSecurity execution, sanitizer run or SonarQube scan. Full-engine and
static-analysis results must be read from CI for the resulting PR head. Existing
historical counts are not reassigned to this revision. No review thread or
analysis issue is marked resolved by these tests alone.
