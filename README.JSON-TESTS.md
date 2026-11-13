# JSON/YAJL tests for ModSecurity v3

This package tests ModSecurity's JSON request parser, audit output and public
content-offset writer before a possible YAJL replacement. It changes neither
the production engine nor the JSON library. All seven suites are described in
[Coverage and expectations](doc/json-pr-extension/README.en.md).

## Build and run

Use a clean checkout with the dependencies in the
[JSON workflow](.github/workflows/json-tests.yml): Python 3.9 or newer, a C++17
compiler, Git, Autotools, pkg-config, YAJL and the configured engine libraries.
The managed build uses Linux tools including `ldd`. CI uses Ubuntu 26.04 and
selects the available Lua development package dynamically.

From the repository root, choose new output directories outside the checkout:

```sh
git submodule update --init --recursive
python3 test/json-baseline/tools/build.py \
  --source "$PWD" --source-ref "$(git rev-parse HEAD)" \
  --output ../json-probes
python3 test/json-baseline/tools/run_all.py \
  --probes ../json-probes --profile all --output ../json-results
```

Do not run `build.sh`, `configure` or `make` in the input checkout first.
`build.py` creates an independent checkout of the selected commit, initializes
its submodules, builds the engine and then compiles the probes. Submodule setup
may require network access. `--jobs` controls build parallelism (default: 2).
A dirty input checkout is rejected; the helper does not clean it for you.

Keep results outside the probe/build directory as well. Use
`--profile characterization` or `--profile requirements` with a separate new
results directory to run either group alone. Omitting `--source-ref` selects the
fixed baseline `1ebdc390b47ab033c8267130ce83130f235aed20`, not the current checkout.
For that separate replay, `tools/prepare_source.py --destination PATH` prepares
the source; omit its optional `--build` because `build.py` handles compilation.

## Read the results

`summary.json`, per-suite JSON reports and logs identify the selected source and
results. Build metadata verifies source/submodule identities, generated build
files, the library, native runner, probes and helper headers before execution.
Old manifests without the required build evidence must be rebuilt.

`PASS` means that the stated expectation matched. `FAIL` means a failed
assertion or execution error; `BLOCKED` means a prerequisite prevented testing.
A profile exits nonzero for either FAIL or BLOCKED. Characterization describes
existing behavior; it is not approval of every behavior. The requirements
profile includes unmet guarantees and open policy proposals, so a nonzero exit
must not be hidden with XFAIL or `continue-on-error`.

CI uploads the evidence even after failures. Judge each commit by its own run,
not by archived counts. The original machine-readable reports in
[`test/json-baseline/results/`](test/json-baseline/results/) and
[`doc/json-pr-extension/`](doc/json-pr-extension/) are historical evidence only.

## Harness checks and scope

```sh
python3 -m unittest discover -s test/json-baseline/tools -p 'test_*.py' -v
```

These self-tests exercise the harness, not the full engine. Ordinary
`make check` runs the native JSON fixture but not all standalone suites.
Use the profile runner above for the complete package. This remains v3-focused:
no v2 migration, replacement-library choice or external connector validation is
claimed. Test origins and remaining policy questions are in the coverage guide.
