# Benchmark-Tests (ModSecurity)

## 1. Überblick

Dieses Repository enthält zwei Benchmark-Programme in `test/benchmark/`:

1. `benchmark` (`test/benchmark/benchmark.cc`)
   - Zweck: wiederholte Ausführung einer ModSecurity-Transaktion in einer Schleife.
2. `json_benchmark` (`test/benchmark/json_benchmark.cc`)
   - Zweck: Benchmarking der JSON-Request-Body-Verarbeitung mit festen Szenarien.

Relevante Dateien:

- `test/benchmark/benchmark.cc`
- `test/benchmark/json_benchmark.cc`
- `test/benchmark/run-json-benchmarks.sh`
- `test/benchmark/json_benchmark_rules.conf`
- `test/benchmark/basic_rules.conf`
- `test/benchmark/download-owasp-v3-rules.sh`
- `test/benchmark/download-owasp-v4-rules.sh`
- `test/benchmark/Makefile.am`
- `README.md` (Benchmark-Abschnitt)

---

## 2. Voraussetzungen

### Benötigte Tools

- POSIX-Shell (`bash`)
- `make`
- C/C++-Toolchain (`gcc`, `g++`)
- Autotools-Bootstrap-Unterstützung aus diesem Repo (`./build.sh`)
- `git` (für Submodule)

### Benötigte Repository-Vorbereitung

Der Benchmark-Build benötigt Submodule.

Verwendete Befehle:

```bash
./build.sh
git submodule update --init --recursive
./configure
```

### Optionale Voraussetzungen

- `time` für externe Laufzeitmessung von `benchmark`
- Zwei getrennte Build-Verzeichnisse für Backend-Vergleiche mit `run-json-benchmarks.sh`

### Beobachtete Einschränkungen in dieser Umgebung

- `./configure` schlug zunächst fehl, bevor Submodule initialisiert wurden.
- Beobachtete Fehlermeldung:

```text
configure: error:

  libInjection was not found within ModSecurity source directory.
  ...
  $ git submodule update --init --recursive
```

Nach dem Submodule-Update lief `./configure` in dieser Umgebung erfolgreich durch.

---

## 3. Build-Anleitung

## Standardablauf (so für die Verifikation verwendet)

```bash
./build.sh
git submodule update --init --recursive
./configure
make -j2 -C others
make -j2 -C src libmodsecurity.la
make -j2 -C test/benchmark benchmark json_benchmark
```

## Hinweise

- `test/benchmark/Makefile.am` definiert beide Binaries (`benchmark`, `json_benchmark`).
- `json_benchmark` benötigt `MSC_JSON_BENCHMARK_RULES_DIR` aus dem Build-System.

## Typische Prüf-Befehle bei Fehlern

```bash
git submodule status
ls -l test/benchmark/benchmark test/benchmark/json_benchmark
```

---

## 4. Ausführung

## `benchmark`

```bash
cd test/benchmark
./benchmark
./benchmark 1000
```

Aktuelle Usage aus dem Quelltext:

```text
Usage: benchmark [num_iterations|-h|-?|--help]
```

## `json_benchmark`

```bash
cd test/benchmark
./json_benchmark --scenario numbers --iterations 100 --target-bytes 1048576
./json_benchmark --scenario utf8 --iterations 50 --output json
```

Aktuelle Usage aus dem Quelltext:

```text
Usage: json_benchmark --scenario NAME [--iterations N] [--target-bytes N] [--depth N] [--include-invalid] [--output json]
```

## Batch-Skript: `run-json-benchmarks.sh`

```bash
test/benchmark/run-json-benchmarks.sh \
  --simdjson-build /pfad/zum/simdjson-build \
  --jsoncons-build /pfad/zum/jsoncons-build
```

Optional:

```bash
test/benchmark/run-json-benchmarks.sh ... --include-invalid
```

Das Skript schreibt JSON-Ausgaben von `json_benchmark` in `json-benchmark-results.jsonl`.

---

## 5. Beispiele

## Beispiel A: verifizierter Lauf (`benchmark`)

Ausgeführter Befehl:

```bash
cd test/benchmark && ./benchmark 10
```

Beobachtete Ausgabe:

```text
Doing 10 transactions...
Summary:
  elapsed_seconds: 0.01
  avg_transaction_ns: 917598.70
  throughput_tx_per_sec: 1089.80
```

## Beispiel B: verifizierter Lauf (`json_benchmark`)

Ausgeführter Befehl:

```bash
cd test/benchmark && ./json_benchmark --scenario numbers --iterations 5 --target-bytes 256
```

Beobachtete Ausgabe:

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

Auf anderen Systemen unterscheiden sich die absoluten Werte.

---

## 6. Gemessene Metriken

## `benchmark`

Aktuelle Summary-Felder:

- `elapsed_seconds`
- `avg_transaction_ns`
- `throughput_tx_per_sec`

Diese Werte werden um die gesamte Loop gemessen und am Ende ausgegeben.

## `json_benchmark`

Vom Code ausgegebene Metriken:

- `append_request_body_ns`
- `process_request_body_ns`
- `total_transaction_ns`
- `parse_success_count`
- `parse_error_count`
- `ru_maxrss_kb`

Optionale Instrumentierungsmetriken sind verfügbar, wenn mit `--enable-json-audit-instrumentation` gebaut wird.

---

## 7. Testdaten und Szenarien

## `benchmark`

- Verwendet feste synthetische Request-/Response-Daten aus dem Quelltext.
- Rule-Datei: `test/benchmark/basic_rules.conf`.
- Optionale CRS-Erweiterungen über:
  - `test/benchmark/download-owasp-v3-rules.sh`
  - `test/benchmark/download-owasp-v4-rules.sh`

## `json_benchmark`-Szenarien

Im Quelltext unterstützte Szenarien:

- `large-object`
- `deep-nesting`
- `numbers`
- `utf8`
- `truncated` (benötigt `--include-invalid`)
- `malformed` (benötigt `--include-invalid`)

Rule-Konfigurationsdatei:

- `test/benchmark/json_benchmark_rules.conf`

---

## 8. Troubleshooting

## Problem: `./configure` scheitert wegen fehlender eingebetteter Abhängigkeit

Prüfen:

```bash
git submodule status
```

Beheben:

```bash
git submodule update --init --recursive
./configure
```

## Problem: Benchmark-Binaries fehlen

Prüfen:

```bash
ls -l test/benchmark/benchmark test/benchmark/json_benchmark
```

Bauen:

```bash
make -j2 -C src libmodsecurity.la
make -j2 -C test/benchmark benchmark json_benchmark
```

## Problem: Skript meldet „missing benchmark binary“

Das Skript erwartet:

- `<build_dir>/test/benchmark/json_benchmark`

Pfad und Executable-Bit prüfen.

---

## 9. Grenzen der Benchmarks

Was gemessen wird:

- Kosten der wiederholten Transaction-Ausführung (`benchmark`).
- JSON-Body-Verarbeitung unter festen synthetischen Szenarien (`json_benchmark`).

Was nicht direkt gemessen wird:

- Verteiltes Multi-Node-Load-Verhalten.
- Volle Produktions-Traffic-Diversität.
- Latenz-Perzentile (p95/p99) aus externem Lastgenerator.

Interpretationshinweis:

- Ergebnisse hängen von Umgebung ab (CPU, Compiler-Flags, Rule-Umfang, Backend-Auswahl, Systemlast).

---

## 10. Dateien und Referenzen

- `README.md` — Benchmark-Abschnitt und Schnellbefehle.
- `test/benchmark/benchmark.cc` — allgemeiner Transaction-Benchmark.
- `test/benchmark/json_benchmark.cc` — JSON-Szenario-Benchmark.
- `test/benchmark/run-json-benchmarks.sh` — Hilfsskript für Backend-Vergleiche.
- `test/benchmark/json_benchmark_rules.conf` — Rules für JSON-Benchmark.
- `test/benchmark/basic_rules.conf` — Baseline-Rules für allgemeinen Benchmark.
- `test/benchmark/download-owasp-v3-rules.sh` — lädt CRS v3 und ergänzt Includes.
- `test/benchmark/download-owasp-v4-rules.sh` — lädt CRS v4 und ergänzt Includes.
- `test/benchmark/Makefile.am` — Benchmark-Targets in Automake.
