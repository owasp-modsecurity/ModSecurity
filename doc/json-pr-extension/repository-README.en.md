# ModSecurity: JSON tests and complete documentation

This standalone test repository exercises the actual YAJL processing in official ModSecurity v3. It contains the complete previous baseline, additional tests inspired by the jsonbench PRs and both ModSecurity comments, and **the complete explanatory documentation in English**. This English repository contains only English explanatory documents. The separate German repository contains both language versions; the executable tests are identical.

Start with the [new results and reproduction guide](README.en.md), the [coverage matrix](matrix.en.md), or [How ModSecurity processes a JSON request](../json-yajl-code-analysis.md). The complete historical reports and original project README are included in English.

Tested production base: [`8271c9d3bc0abcbd1189f5414c196c336c959047`](https://github.com/owasp-modsecurity/ModSecurity/commit/8271c9d3bc0abcbd1189f5414c196c336c959047). The engine and YAJL remain unchanged. This distribution contains tests and documentation; building requires a separate complete ModSecurity checkout.

Fresh characterization runs give **399 PASS**. Separate requirements give **6 PASS / 20 FAIL**, retaining unmet assertions as ordinary failures. Some are explicitly undecided policy proposals. No XFAIL hides them. The [machine-readable results](results-summary.json) explain scope and repeated executions.

```sh
python3 test/json-baseline/tools/prepare_source.py --destination ../modsecurity-original --build
python3 test/json-baseline/tools/build.py --source ../modsecurity-original --output ../json-probes
python3 test/json-baseline/tools/run_all.py --probes ../json-probes --output ../json-results --profile characterization
python3 test/json-baseline/tools/run_all.py --probes ../json-probes --output ../json-requirements --profile requirements
```

The last command returns exit 1 on the examined original. See the linked guide for dependencies, build details, status classes and unchanged historical reports. The CI template runs the profiles separately; its requirements job accordingly remains red.

Test ideas were implemented independently; GPL adapter code from jsonbench was not copied. Original documentation and license notices are retained. License: [Apache-2.0](../../LICENSE).
