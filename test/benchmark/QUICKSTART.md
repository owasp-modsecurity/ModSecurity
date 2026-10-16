# Quick Start Guide - ModSecurity Benchmarking with Google Benchmark

## TL;DR

```bash
# 1. Install Google Benchmark
brew install google-benchmark  # macOS
# OR
sudo apt-get install libbenchmark-dev  # Ubuntu/Debian

# 2. Build the benchmark
cd test/benchmark
./build_with_benchmark.sh

# 3. Run benchmarks
./benchmark_rules

# 4. Filter specific benchmarks
./benchmark_rules --benchmark_filter="SingleHeader"
```

## What's Included

### Pre-built Benchmarks (`benchmark_rules`)

Compares performance of different rule targets:

- **Single header** vs **All headers** (`REQUEST_HEADERS:User-Agent` vs `REQUEST_HEADERS`)
- **Single arg** vs **All args** (`ARGS:name` vs `ARGS`)
- **Regex key matching** (`REQUEST_HEADERS:/^X-/`)
- **Different operators** (`@contains` vs `@rx`)
- **Transformations** (none vs single vs multiple)
- **Multiple rules** (1 vs 5 rules)

### Template for Custom Benchmarks (`benchmark_template.cc`)

Copy and customize for your specific needs:
- Custom headers
- Custom URIs
- JSON body processing
- Parameterized tests (vary number of headers/rules)
- Operator comparisons

## Installation

### macOS
```bash
brew install google-benchmark
```

### Ubuntu/Debian
```bash
sudo apt-get install libbenchmark-dev
```

### CentOS/RHEL
```bash
# Install from EPEL
sudo yum install epel-release
sudo yum install google-benchmark-devel
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

## Building

### Method 1: Using the helper script (Easiest)

```bash
cd test/benchmark
./build_with_benchmark.sh
```

### Method 2: Manual compilation

```bash
cd test/benchmark

# With pkg-config
g++ -std=c++17 -O3 -DNDEBUG \
    -I../../headers \
    $(pkg-config --cflags libpcre2-8) \
    benchmark_rules.cc \
    -L../../src/.libs \
    -lmodsecurity \
    $(pkg-config --libs libpcre2-8) \
    -lyajl \
    -lbenchmark \
    -lpthread \
    -o benchmark_rules

# Run it
./benchmark_rules
```

### Method 3: Integrate into build system

1. Add the content from `configure_snippet.m4` to `configure.ac`
2. Rebuild:
   ```bash
   ./build.sh
   ./configure --with-benchmark
   make
   cd test/benchmark
   ./benchmark_rules
   ```

## Running Benchmarks

### Basic Run
```bash
./benchmark_rules
```

### Filter by Name
```bash
# Run all benchmarks with "Header" in the name
./benchmark_rules --benchmark_filter="Header"

# Run only regex benchmarks
./benchmark_rules --benchmark_filter="Regex.*"

# Run single benchmark
./benchmark_rules --benchmark_filter="^BM_SingleHeader$"
```

### Control Execution
```bash
# Run each benchmark for at least 5 seconds
./benchmark_rules --benchmark_min_time=5.0

# Run 10 repetitions for statistical confidence
./benchmark_rules --benchmark_repetitions=10

# List available benchmarks without running
./benchmark_rules --benchmark_list_tests
```

### Output Formats
```bash
# JSON output
./benchmark_rules --benchmark_out=results.json --benchmark_out_format=json

# CSV output
./benchmark_rules --benchmark_out=results.csv --benchmark_out_format=csv

# Console format (default)
./benchmark_rules --benchmark_format=console
```

## Understanding Results

### Example Output
```
Run on (8 X 2400 MHz CPUs)
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
BM_NoRules                          1234 ns         1230 ns       567890
BM_SingleHeader                     2345 ns         2340 ns       298765
BM_AllHeaders                       4567 ns         4560 ns       153456
BM_RegexSingleHeader                3456 ns         3450 ns       202789
BM_RegexAllHeaders                  8901 ns         8890 ns        78654
```

### Key Metrics
- **Time**: Wall clock time per iteration (nanoseconds)
- **CPU**: CPU time per iteration
- **Iterations**: How many times the test ran

### Analysis
In the example above:
- Baseline overhead: 1.23 microseconds (BM_NoRules)
- Single header: 2.34 microseconds (1.1 µs of rule processing)
- All headers: 4.56 microseconds (3.3 µs of rule processing)
- **Conclusion**: Scanning all headers is ~1.95x slower than single header

## Comparing Results

### Baseline and Comparison
```bash
# Baseline
./benchmark_rules --benchmark_out=baseline.json --benchmark_out_format=json

# Make changes to ModSecurity code...
# Rebuild...

# New results
./benchmark_rules --benchmark_out=improved.json --benchmark_out_format=json

# Compare (requires compare.py from Google Benchmark tools)
compare.py baseline.json improved.json
```

### Statistical Analysis
```bash
# Run with repetitions for confidence intervals
./benchmark_rules --benchmark_repetitions=10 \
                  --benchmark_report_aggregates_only=true
```

## Creating Custom Benchmarks

### Quick Customization

1. Copy the template:
   ```bash
   cp benchmark_template.cc my_benchmark.cc
   ```

2. Edit `my_benchmark.cc`:
   - Change `MY_HEADERS` array for your headers
   - Modify rule definitions in each benchmark
   - Add new benchmark functions as needed

3. Build:
   ```bash
   g++ -std=c++17 -O3 -I../../headers my_benchmark.cc \
       -L../../src/.libs -lmodsecurity -lbenchmark -lpthread \
       -o my_benchmark
   ```

4. Run:
   ```bash
   ./my_benchmark
   ```

### Example: Test Your Custom Rule

```cpp
static void BM_MyRule(benchmark::State& state) {
    ModSecFixture fixture;

    std::string rules = R"(
        SecRuleEngine On
        SecRule REQUEST_HEADERS:X-My-Header "@contains pattern" \
            "id:1,phase:1,deny,status:403"
    )";

    fixture.loadRules(rules);

    for (auto _ : state) {
        Transaction* trans = fixture.createTransaction();
        // ... setup transaction ...
        delete trans;
    }
}
BENCHMARK(BM_MyRule);
```

## Tips for Accurate Benchmarking

1. **Use release build**:
   ```bash
   ./configure CFLAGS="-O3 -march=native" CXXFLAGS="-O3 -march=native"
   make
   ```

2. **Disable CPU frequency scaling** (Linux):
   ```bash
   sudo cpupower frequency-set --governor performance
   ```

3. **Close other applications** to reduce noise

4. **Use repetitions** for statistical significance:
   ```bash
   ./benchmark_rules --benchmark_repetitions=10
   ```

5. **Disable logging** in benchmark rules:
   ```
   SecRule ... "...,nolog,..."
   ```

6. **Disable body processing** when testing headers:
   ```
   SecRequestBodyAccess Off
   SecResponseBodyAccess Off
   ```

## Troubleshooting

### "benchmark/benchmark.h: No such file"
**Solution**: Install Google Benchmark (see Installation section)

### Linking errors
**Solution**: Add `-lbenchmark -lpthread` to link command

### Rules not loading
**Solution**: Check rule syntax:
```bash
../../tools/rules-check/modsec_rules_check -r "SecRule ..."
```

### Benchmark crashes
**Solution**: Run with debug symbols:
```bash
g++ -std=c++17 -g -O0 ... -o benchmark_rules_debug
gdb ./benchmark_rules_debug
```

## Next Steps

- See `README.md` for complete documentation
- Check `benchmark_template.cc` for more examples
- Read [Google Benchmark User Guide](https://github.com/google/benchmark/blob/main/docs/user_guide.md)
- Profile with perf/valgrind for deeper analysis

## Common Use Cases

### 1. Compare two operators
Create two benchmarks with different operators, run and compare times.

### 2. Test header count scaling
Use parameterized benchmark: `BENCHMARK(BM_Test)->Range(1, 1000)`

### 3. Measure transformation overhead
Create two identical benchmarks, one with and one without transformation.

### 4. Test rule count impact
Generate N identical rules and measure total processing time.

### 5. Profile specific rule from OWASP CRS
Extract rule from CRS, create benchmark, optimize rule syntax.
