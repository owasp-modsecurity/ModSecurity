# ModSecurity: JSON tests and complete documentation

This standalone test repository exercises the actual YAJL processing in official ModSecurity v3. It contains the complete previous baseline, additional tests inspired by the jsonbench PRs and both ModSecurity comments, and **the complete explanatory documentation in English**. This English repository contains only English explanatory documents. The separate German repository contains both language versions; the executable tests are identical.

Start with the [results and reproduction guide](README.en.md), the [coverage matrix](matrix.en.md), or [How ModSecurity processes a JSON request](../json-yajl-code-analysis.md). The complete historical reports and original project README are included in English.

Historical tested production base: [`8271c9d3bc0abcbd1189f5414c196c336c959047`](https://github.com/owasp-modsecurity/ModSecurity/commit/8271c9d3bc0abcbd1189f5414c196c336c959047). The October 15, 2026 runs recorded **399 PASS** for characterization and **6 PASS / 20 FAIL** for requirements. The [archived machine-readable results](results-summary.json) identify that source revision, scope and repeated executions; these are not fresh results for the current PR head. Earlier archived baseline reports identify `dd1d0f82672ecf7d0f014a0c3fc4fd0ab40b3bdd` and must be read against their own recorded revisions.

The current preparation helper instead selects `1ebdc390b47ab033c8267130ce83130f235aed20`. The commands below start a new run on that selected source; they do not reproduce the archived counts. Ordinary PR CI uses its actual checkout SHA through `--source-ref`. A complete ModSecurity source build is required; the standalone package can use a separate checkout, while PR #3752 also integrates it into the full upstream-based tree. Production code and YAJL are not modified by the test package.

Unmet requirements remain ordinary failures, including explicitly undecided policy proposals. No XFAIL hides them. Use newly generated reports and their verified source metadata for claims about a new execution.

```sh
python3 test/json-baseline/tools/prepare_source.py --destination ../modsecurity-original
python3 test/json-baseline/tools/build.py --source ../modsecurity-original --output ../json-probes
python3 test/json-baseline/tools/run_all.py --probes ../json-probes --output ../json-results --profile characterization
python3 test/json-baseline/tools/run_all.py --probes ../json-probes --output ../json-requirements --profile requirements
```

`prepare_source.py` only prepares the source here. `build.py` performs the required engine and probe build in a fresh checkout; a separate `--build` preparation would compile an unused engine twice.

The historical requirements run returned exit 1 on its recorded source. The current command also retains a nonzero exit for unmet requirements or blocked execution; its outcome must be read from the new run. See the linked guide for dependencies, build details, status classes and unchanged historical reports. The CI template runs the profiles separately without suppressing requirements failures.

Test ideas were implemented independently; GPL adapter code from jsonbench was not copied. Original documentation and license notices are retained. License: [Apache-2.0](../../LICENSE).
