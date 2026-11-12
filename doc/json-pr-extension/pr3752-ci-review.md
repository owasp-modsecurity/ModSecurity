# PR #3752: CI and review corrections

This revision follows the complete-suite integration at `543730d3e015`.
All seven suites remain included; no requirement is converted to XFAIL and no
failing profile is marked `continue-on-error`. No engine or YAJL change is made.

## Current checkout versus pinned replay

The active JSON workflow builds the commit that Actions actually checked out.
It passes `--source-ref "$GITHUB_SHA"` to `tools/build.py`. For pull-request
events this may be GitHub's merge-test commit rather than the fork head; recording
that SHA is intentional. Committed engine changes can therefore be tested by
future PRs, without disabling checks for uncommitted production changes, altered
submodules, changed probes, loader resolution or the active JSON canary.

Without `--source-ref`, the helper still verifies the pinned replay reference
`1ebdc390b47ab033c8267130ce83130f235aed20`. `prepare_source.py` and the companion
workflow template retain this separate pinned-source purpose. They do not
reproduce October evidence with November helpers. Replaying the October 15 run
requires its full test/helper snapshot `dd63a59e3a993faf4e74b08979e39986e1df4d2c`
and the instructions archived with that snapshot.

For a current, normally built checkout, use new output directories outside it:

```sh
python3 test/json-baseline/tools/build.py \
  --source "$PWD" --source-ref "$(git rev-parse HEAD)" \
  --output ../json-probes-current
python3 test/json-baseline/tools/run_all.py \
  --probes ../json-probes-current --profile all \
  --output ../json-results-current
```

The verified build manifest is passed to the child suites. Expanded request and
writer reports now obtain their production SHA from that manifest and verify
the probe identity, instead of writing the October SHA into every fresh report.
A standalone invocation without that manifest records `null` for unknown source
identity. Archived report JSON files are unchanged.

## Ubuntu and Lua

Both JSON workflow files use `ubuntu-26.04`. The Lua dependency is selected with
the `apt-cache pkgnames` / `sort -V` approach from the main `ci_new.yml`, not a
hard-coded `liblua5.4-dev` or a fixed list of releases. Discovery fails explicitly
if no development package is available. The selected package is installed and
the normal ModSecurity Lua configure detection is retained.

## Probe maintenance

The three C++ probes have explicit standard includes, expanded control-flow
blocks and bounded line lengths. Mutable helper arguments are passed as pointers
rather than non-const references. The synthetic payloads, transaction order,
observed fields and expectations are retained. No files are excluded from
cpplint/cppcheck and no blanket lint suppression is introduced.

## Validation and limits

Fifteen unit tests were executed locally for current-versus-pinned source
selection, rejection of a dirty engine or wrong submodule, report attribution,
probe identity, exact canary records and preservation of real failure counts.
All fifteen passed. Python syntax, workflow YAML and shell syntax were checked.
These checks do not substitute for compiling and executing ModSecurity.

A fresh engine build and the full runtime, cpplint and cppcheck jobs were not
available in the local preparation environment. Their result must be read from
CI for the published correction commit. A requirements failure remains a real
failure and may still need a separate engine fix or a maintainer policy decision.
Neither historical counts nor an earlier green characterization job establish
that this revised head passes.
