# ModSecurity Benchmarking

This directory contains benchmarking tools for ModSecurity v3 library performance testing.

## Available Benchmarks

### 1. Simple Benchmark (`benchmark`)

The original simple benchmark that measures transaction throughput.

**Usage:**
```bash
./benchmark [num_iterations]

# Examples
./benchmark              # Run 1M transactions (default)
./benchmark 10000        # Run 10K transactions
time ./benchmark 100000  # Measure time for 100K transactions
```

**What it measures:**
- Complete transaction lifecycle overhead
- Throughput with minimal rules (basic_rules.conf)

**Benchmarking with OWASP CRS:**
```bash
./download-owasp-v3-rules.sh    # Download OWASP Core Rule Set
# Edit basic_rules.conf to include CRS rules
time ./benchmark 1000           # Run with full rule set
```

### 2. Google Benchmark Framework (`benchmark_rules`)

Modern microbenchmarking framework for detailed performance analysis of specific rule targets and operators.

## Installing Google Benchmark

### Ubuntu/Debian
```bash
sudo apt-get install libbenchmark-dev
```

### macOS (Homebrew)
```bash
brew install google-benchmark
```

### From Source
```bash
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory "build"
cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../
cmake --build "build" --config Release
sudo cmake --build "build" --config Release --target install
```

## Building with Google Benchmark

After installing Google Benchmark, you need to configure the build system to detect it.

### Option 1: Add to configure.ac (Recommended)

Add this to `configure.ac`:
```m4
# Check for Google Benchmark
AC_ARG_WITH([benchmark],
    [AS_HELP_STRING([--with-benchmark],
        [enable Google Benchmark support @<:@default=check@:>@])],
    [],
    [with_benchmark=check])

HAVE_GOOGLE_BENCHMARK=0
AS_IF([test "x$with_benchmark" != "xno"],
    [AC_CHECK_LIB([benchmark], [main],
        [
            AC_SUBST([BENCHMARK_LDADD], ["-lbenchmark -lpthread"])
            AC_DEFINE([HAVE_GOOGLE_BENCHMARK], [1], [Define if you have Google Benchmark])
            HAVE_GOOGLE_BENCHMARK=1
        ],
        [if test "x$with_benchmark" != "xcheck"; then
            AC_MSG_FAILURE([--with-benchmark was given, but test for benchmark failed])
        fi])])

AM_CONDITIONAL([HAVE_GOOGLE_BENCHMARK], [test "$HAVE_GOOGLE_BENCHMARK" = "1"])
```

Then rebuild:
```bash
./build.sh
./configure --with-benchmark
make
```

### Option 2: Manual Build (Quick Testing)

If you don't want to modify configure.ac:

```bash
cd test/benchmark
g++ -std=c++17 -O3 -DNDEBUG \
    -I../../headers \
    benchmark_rules.cc \
    -L../../src/.libs \
    -lmodsecurity \
    -lbenchmark \
    -lpthread \
    -o benchmark_rules

# Run the benchmarks
./benchmark_rules
```

## Running Google Benchmark Tests

### Basic Usage

```bash
cd test/benchmark
./benchmark_rules
```

**Output Example:**
```
Run on (8 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 8192 KiB (x1)
Load Average: 1.23, 1.45, 1.67
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
BM_NoRules                          1234 ns         1230 ns       567890
BM_SingleHeader                     2345 ns         2340 ns       298765
BM_AllHeaders                       4567 ns         4560 ns       153456
BM_RegexSingleHeader                3456 ns         3450 ns       202789
BM_RegexAllHeaders                  8901 ns         8890 ns        78654
```

### Advanced Options

**Filter specific benchmarks:**
```bash
./benchmark_rules --benchmark_filter="SingleHeader"
./benchmark_rules --benchmark_filter="Regex.*"
```

**Control iterations:**
```bash
./benchmark_rules --benchmark_min_time=5.0     # Run each for at least 5 seconds
./benchmark_rules --benchmark_repetitions=10   # Repeat each benchmark 10 times
```

**Output formats:**
```bash
./benchmark_rules --benchmark_format=json > results.json
./benchmark_rules --benchmark_format=csv > results.csv
./benchmark_rules --benchmark_out=results.json --benchmark_out_format=json
```

**Comparing results:**
```bash
# Run baseline
./benchmark_rules --benchmark_out=baseline.json --benchmark_out_format=json

# Make changes to code and rebuild
# ...

# Run comparison
./benchmark_rules --benchmark_out=improved.json --benchmark_out_format=json

# Use compare.py from Google Benchmark tools
compare.py baseline.json improved.json
```

**CPU profiling:**
```bash
./benchmark_rules --benchmark_counters_tabular=true
```

## Available Benchmarks in benchmark_rules

### Variable Target Benchmarks

| Benchmark | Description | Measures |
|-----------|-------------|----------|
| `BM_NoRules` | Baseline with no rules | Transaction overhead |
| `BM_SingleHeader` | Target one header | `REQUEST_HEADERS:User-Agent` performance |
| `BM_AllHeaders` | Target all headers | `REQUEST_HEADERS` collection scan |
| `BM_HeadersRegexKey` | Regex key matching | `REQUEST_HEADERS:/^X-/` performance |
| `BM_HeaderNames` | Target header names | `REQUEST_HEADERS_NAMES` performance |
| `BM_Args` | Target all query params | `ARGS` collection performance |
| `BM_SingleArg` | Target one query param | `ARGS:name` performance |

### Operator Benchmarks

| Benchmark | Description | Operator |
|-----------|-------------|----------|
| `BM_RegexSingleHeader` | Regex on single header | `@rx` with one target |
| `BM_RegexAllHeaders` | Regex on all headers | `@rx` with collection |

### Transformation Benchmarks

| Benchmark | Description | Transformations |
|-----------|-------------|-----------------|
| `BM_TransformationSingleHeader` | One transformation | `t:lowercase` |
| `BM_TransformationAllHeaders` | Transformation on collection | `t:lowercase` on all |
| `BM_MultipleTransformations` | Multiple transformations | `t:lowercase,t:compressWhitespace,t:trim` |

### Multiple Rules Benchmark

| Benchmark | Description |
|-----------|-------------|
| `BM_MultipleHeaderRules` | 5 different header rules evaluated |

## Adding Custom Benchmarks

### Example: Benchmark a New Operator

```cpp
static void BM_MyNewOperator(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRequestBodyAccess Off
        SecResponseBodyAccess Off
        SecRule REQUEST_HEADERS:User-Agent "@myOperator pattern" \
            "id:2001,phase:1,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        runTransaction(trans);
        delete trans;
    }
}
BENCHMARK(BM_MyNewOperator);
```

### Example: Benchmark with Custom Headers

```cpp
static void BM_WithCustomHeaders(benchmark::State& state) {
    ModSecFixture fixture;

    // Custom headers for this test
    static const std::vector<std::pair<std::string, std::string>> custom_headers = {
        {"X-Custom-1", "value1"},
        {"X-Custom-2", "value2"},
        // ... more headers
    };

    std::string rules = R"(
        SecRuleEngine On
        SecRule REQUEST_HEADERS:X-Custom-1 "@contains value" \
            "id:2002,phase:1,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();

        // Use custom headers
        trans->processConnection(TEST_CLIENT_IP, 54321, TEST_SERVER_IP, 443);
        trans->processURI(TEST_URI, "GET", "HTTP/1.1");

        for (const auto& header : custom_headers) {
            trans->addRequestHeader(header.first, header.second);
        }
        trans->processRequestHeaders();

        delete trans;
    }
}
BENCHMARK(BM_WithCustomHeaders);
```

### Example: Parameterized Benchmark

```cpp
static void BM_HeaderCount(benchmark::State& state) {
    ModSecFixture fixture;
    int header_count = state.range(0);

    std::string rules = R"(
        SecRuleEngine On
        SecRule REQUEST_HEADERS "@contains test" \
            "id:2003,phase:1,pass,nolog"
    )";

    if (!fixture.loadRules(rules)) {
        state.SkipWithError("Failed to load rules");
        return;
    }

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        trans->processConnection(TEST_CLIENT_IP, 54321, TEST_SERVER_IP, 443);
        trans->processURI(TEST_URI, "GET", "HTTP/1.1");

        // Add N headers
        for (int i = 0; i < header_count; i++) {
            std::string name = "X-Header-" + std::to_string(i);
            trans->addRequestHeader(name, "test-value");
        }

        trans->processRequestHeaders();
        delete trans;
    }
}
BENCHMARK(BM_HeaderCount)->Range(1, 100);  // Test with 1, 8, 64, 100 headers
```

## Interpreting Results

### Time Units
- **ns** (nanoseconds): 1/1,000,000,000 second
- **us** (microseconds): 1/1,000,000 second
- **ms** (milliseconds): 1/1,000 second

### Key Metrics
- **Time**: Wall clock time per iteration
- **CPU**: CPU time per iteration
- **Iterations**: Number of times the benchmark ran

### Example Analysis

```
BM_SingleHeader      2345 ns    2340 ns    298765
BM_AllHeaders        4567 ns    4560 ns    153456
```

**Analysis:** Targeting all headers (`REQUEST_HEADERS`) is ~1.95x slower than targeting a single header (`REQUEST_HEADERS:User-Agent`). This is expected because the rule engine must iterate through all 15 headers instead of directly accessing one.

## Performance Tips

1. **Disable body processing** when testing headers:
   ```
   SecRequestBodyAccess Off
   SecResponseBodyAccess Off
   ```

2. **Use `nolog`** in benchmark rules to avoid logging overhead

3. **Disable audit logging** in test configuration

4. **Run with release build** and optimizations:
   ```bash
   ./configure CFLAGS="-O3 -march=native" CXXFLAGS="-O3 -march=native"
   make
   ```

5. **Isolate CPU** when running benchmarks:
   ```bash
   # Linux: Isolate CPUs and disable frequency scaling
   sudo cpupower frequency-set --governor performance
   ```

6. **Multiple runs** for statistical significance:
   ```bash
   ./benchmark_rules --benchmark_repetitions=10
   ```

## Troubleshooting

### Benchmark not building

**Error:** `benchmark/benchmark.h: No such file or directory`

**Solution:** Install Google Benchmark (see installation instructions above)

### Linking errors

**Error:** `undefined reference to benchmark::State::...`

**Solution:** Make sure `-lbenchmark -lpthread` is in LDADD/LDFLAGS

### Rules not loading

**Error:** `SkipWithError: Failed to load rules`

**Solution:** Check your rule syntax. Use the `modsec_rules_check` tool:
```bash
../../tools/rules-check/modsec_rules_check -r "SecRule ..."
```

## References

- [Google Benchmark Documentation](https://github.com/google/benchmark/blob/main/docs/user_guide.md)
- [ModSecurity Reference Manual](https://github.com/owasp-modsecurity/ModSecurity/wiki/Reference-Manual)
- [Performance Tuning Guide](https://github.com/owasp-modsecurity/ModSecurity/wiki/Performance-Tuning)
