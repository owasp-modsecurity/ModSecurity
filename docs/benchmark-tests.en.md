# Benchmark Tests (ModSecurity)

## 1. Overview

This repository contains two benchmark executables in `test/benchmark/`:

1. `benchmark` (`test/benchmark/benchmark.cc`)
   - Purpose: run repeated end-to-end-style ModSecurity transaction processing in a loop.
2. `json_benchmark` (`test/benchmark/json_benchmark.cc`)
   - Purpose: benchmark JSON request-body processing with fixed scenarios.

Relevant files:

- `test/benchmark/benchmark.cc`
- `test/benchmark/json_benchmark.cc`
- `test/benchmark/run-json-benchmarks.sh`
- `test/benchmark/json_benchmark_rules.conf`
- `test/benchmark/basic_rules.conf`
- `test/benchmark/download-owasp-v3-rules.sh`
- `test/benchmark/download-owasp-v4-rules.sh`
- `test/benchmark/Makefile.am`
- `README.md` (benchmark section)

---

## 2. Prerequisites

### Required tools

- POSIX shell (`bash`)
- `make`
- C/C++ toolchain (`gcc`, `g++`)
- Autotools bootstrap support used by this repo (`./build.sh`)
- `git` (required for submodule initialization)

### Required repository preparation

The benchmark build requires repository submodules.

Commands used:

```bash
./build.sh
git submodule update --init --recursive
./configure
```

### Optional prerequisites

- `time` command for external wall-clock timing of `benchmark`
- Two separate build directories if you want to compare JSON backends with `run-json-benchmarks.sh`

### Known environment constraints observed during verification

- Initial `./configure` failed before submodules were initialized.
- Error observed:

```text
configure: error:

  libInjection was not found within ModSecurity source directory.
  ...
  $ git submodule update --init --recursive
```

After submodule initialization, `./configure` completed successfully in this environment.

---

## 3. Build Instructions

## Standard build flow used for benchmark verification

```bash
./build.sh
git submodule update --init --recursive
./configure
make -j2 -C others
make -j2 -C src libmodsecurity.la
make -j2 -C test/benchmark benchmark json_benchmark
```

## Notes

- `test/benchmark/Makefile.am` declares both binaries (`benchmark`, `json_benchmark`).
- `json_benchmark` depends on `MSC_JSON_BENCHMARK_RULES_DIR` provided by the build system.

## Typical error checks

If build fails, verify:

```bash
git submodule status
ls -l test/benchmark/benchmark test/benchmark/json_benchmark
```

---

## 4. Execution

## `benchmark`

```bash
cd test/benchmark
./benchmark
./benchmark 1000
```

Current usage string from source:

```text
Usage: benchmark [num_iterations|-h|-?|--help]
```

## `json_benchmark`

```bash
cd test/benchmark
./json_benchmark --scenario numbers --iterations 100 --target-bytes 1048576
./json_benchmark --scenario utf8 --iterations 50 --output json
```

Current usage string from source:

```text
Usage: json_benchmark --scenario NAME [--iterations N] [--target-bytes N] [--depth N] [--include-invalid] [--output json]
```

## Batch script: `run-json-benchmarks.sh`

```bash
test/benchmark/run-json-benchmarks.sh \
  --simdjson-build /path/to/simdjson-build \
  --jsoncons-build /path/to/jsoncons-build
```

Optional:

```bash
test/benchmark/run-json-benchmarks.sh ... --include-invalid
```

This script appends JSON output lines from `json_benchmark` into `json-benchmark-results.jsonl`.

---

## 5. Examples

## Example A: verified local run (`benchmark`)

Command executed:

```bash
cd test/benchmark && ./benchmark 10
```

Observed output:

```text
Doing 10 transactions...
Summary:
  elapsed_seconds: 0.01
  avg_transaction_ns: 917598.70
  throughput_tx_per_sec: 1089.80
```

## Example B: verified local run (`json_benchmark`)

Command executed:

```bash
cd test/benchmark && ./json_benchmark --scenario numbers --iterations 5 --target-bytes 256
```

Observed output:

```text
backend: simdjson
scenario: numbers
iterations: 5
body_bytes: 223
append_request_body_ns: 4021
process_request_body_ns: 288499
total_transaction_ns: 840357
parse_success_count: 5
parse_error_count: 0
ru_maxrss_kb: 33720
```

If you run on another machine, absolute numeric values will differ.

---

## 6. Measured Metrics

## `benchmark`

Current human-readable summary fields:

- `elapsed_seconds`
- `avg_transaction_ns`
- `throughput_tx_per_sec`

These are measured around the whole loop and printed at the end.

## `json_benchmark`

Metrics printed by code:

- `append_request_body_ns`
- `process_request_body_ns`
- `total_transaction_ns`
- `parse_success_count`
- `parse_error_count`
- `ru_maxrss_kb`

Optional instrumentation metrics are available when compiled with `--enable-json-audit-instrumentation`.

---

## 7. Test Data and Scenarios

## `benchmark`

- Uses fixed synthetic request/response data from source.
- Rule file: `test/benchmark/basic_rules.conf`.
- Optional CRS additions via:
  - `test/benchmark/download-owasp-v3-rules.sh`
  - `test/benchmark/download-owasp-v4-rules.sh`

## `json_benchmark` scenarios

Supported scenario names in source:

- `large-object`
- `deep-nesting`
- `numbers`
- `utf8`
- `truncated` (requires `--include-invalid`)
- `malformed` (requires `--include-invalid`)

Rule configuration file:

- `test/benchmark/json_benchmark_rules.conf`

---

## 8. Troubleshooting

## Problem: `./configure` fails with missing embedded dependency

Check:

```bash
git submodule status
```

Fix:

```bash
git submodule update --init --recursive
./configure
```

## Problem: benchmark binaries missing

Check:

```bash
ls -l test/benchmark/benchmark test/benchmark/json_benchmark
```

Build:

```bash
make -j2 -C src libmodsecurity.la
make -j2 -C test/benchmark benchmark json_benchmark
```

## Problem: JSON benchmark script reports missing binary

The script expects binaries at:

- `<build_dir>/test/benchmark/json_benchmark`

Validate path and executable bit.

---

## 9. Benchmark Limits

What these benchmarks do measure:

- Repeated transaction execution path cost (`benchmark`).
- JSON body handling under fixed synthetic scenarios (`json_benchmark`).

What they do not directly measure:

- Distributed multi-node load behavior.
- Full production traffic diversity.
- Latency percentiles (p95/p99) across an external load generator.

Interpretation caution:

- Results are environment-dependent (CPU, compiler flags, ruleset size, selected backend, system load).

---

## 10. Files and References

- `README.md` — benchmark section and quick commands.
- `test/benchmark/benchmark.cc` — generic transaction benchmark.
- `test/benchmark/json_benchmark.cc` — JSON scenario benchmark.
- `test/benchmark/run-json-benchmarks.sh` — backend comparison helper script.
- `test/benchmark/json_benchmark_rules.conf` — rules for JSON benchmark.
- `test/benchmark/basic_rules.conf` — baseline rules include for generic benchmark.
- `test/benchmark/download-owasp-v3-rules.sh` — fetch and append CRS v3 includes.
- `test/benchmark/download-owasp-v4-rules.sh` — fetch and append CRS v4 includes.
- `test/benchmark/Makefile.am` — benchmark targets in automake.
