# Original JSON processing with YAJL: results overview

> Complete English translation of the historical baseline published at fork commit `2bfb2180d8c0fa35ae7a3f2665e999600607d896`. Counts, paths and publication statements below describe that baseline; the new extension reports document subsequent work separately.

This investigation records the behavior of official ModSecurity before a replacement library is chosen. YAJL and all production files remain unchanged. The new tests and this documentation belong exclusively to two new branches in the [Easton97-Jens/ModSecurity](https://github.com/Easton97-Jens/ModSecurity) fork.

## Reading and reproducing the results

1. [How ModSecurity processes a JSON request; technical analysis and verification of fzipi's statements](json-yajl-code-analysis.md).
2. Version report with executed commands, test IDs, expectations, results and limitations: [v3](json-yajl-baseline-v3.md). The v2 report is at `doc/json-yajl-baseline-v2.md` on `test/json-yajl-baseline-v2-20261015`. Each branch contains its own tests; the final handover report links both published states.
3. [Provenance manifest](../json-yajl-provenance.json): full original SHAs, existing fork references, comparisons, submodules, working directories and environment.

| Line | Original branch and pinned commit | New task branch | Local working directory |
|---|---|---|---|
| v2 | `v2/master`, `151074aca78a416764b12ba1221e61a17a6c714d` | `test/json-yajl-baseline-v2-20261015` | `/workspace/work/json-yajl-baseline/v2` |
| v3 | `v3/master`, `8271c9d3bc0abcbd1189f5414c196c336c959047` | `test/json-yajl-baseline-v3-20261015` | `/workspace/work/json-yajl-baseline/v3` |

The original commits were fetched on October 15, 2026; the explicit fetch completed at `2026-10-15T12:22:19+00:00` (14:22:19 CEST). The base was not updated afterwards. The [initial comment by fzipi](https://github.com/owasp-modsecurity/ModSecurity/issues/3308#issuecomment-6166461842) was read directly. Library selection, parser replacement, production fixes, pull requests and tags are outside this work.

## What the results mean

A request body is the sequence of bytes after the HTTP headers. ModSecurity passes it to a parser when the configuration selects the JSON processor. For keys, values and container boundaries, YAJL calls functions in ModSecurity. These **callbacks** do not build a complete JSON tree; they create arguments from scalar values. A rule can read them through collections such as `ARGS`. The technical analysis explains exactly which collections are affected, when rules run and how errors become visible.

Three levels remain separate: a parser can report a syntax error; ModSecurity then sets an error state; a rule, an engine limit path or the web server decides the HTTP response. For example, v2 can return HTTP 500 before phase 2 for an error detected only during parser completion; an error during input can instead remain visible to a phase 2 rule. Arguments already created may also survive a later error. The new tests therefore check arguments and error state together.

The names used by v2 and v3 differ. Numbers are passed through the text callback: a test must not use numeric conversion to equate `1.2300` and `1E+02` with `1.23` and `100`. Duplicate keys require a list of argument entries; a dictionary would lose multiple occurrences. The separate expectation lists for each version specify the details and edge cases.

A passing characterization test means: **This is how this original version behaves in this environment.** It does not automatically decide that every observed detail is desirable. The technical analysis and version reports distinguish demonstrated behavior, possible bugs, compatibility requirements and open decisions. A future fix must deliberately change the affected expectations.

## Existing work and provenance

The unchanged initial clone `/workspace/ModSecurity` remained on its existing branch `work` at `48e561b2ba44d35eec799176608720129c678777`. It was clean at the start; existing ignored build artifacts and the `/workspace/modsecurity-env` environment were not overwritten. No existing local branches were checked out, reset, stashed, merged or rebased. The task branches were created with `git worktree add --no-track -b ... <FULL-ORIGINAL-SHA>` (the placeholder means the full original SHA).

`origin` is the fork and `upstream` is the official original. The latter was only read. The manifest contains the complete previous fork reference inventory. Direct comparisons show:

- Fork `v3/master` is four original commits behind; differences concern the README, security policy and timestamp type in the public transaction header. The JSON processor is identical.
- Compared directly, `v3/master-json` contains identical JSON processor files, but different build and transaction states; none of that was adopted.
- `v3/master-json2` contains additional backends and substantial parser changes. These explicitly are not the original reference for this investigation.
- There is no fork branch `v2/master`. Despite its name, the existing branch `Modsecurity/v2` contains a v3 source tree (commit message v3.0.16); it was not used as a v2 source.

The first fetch of older fork references encountered errors when automatically recursing into historical Python/language-test submodules that were no longer available. The parent references were present; the explicit fetch was successfully repeated with `fetch.recurseSubmodules=false`. The manifest and version report list the pinned original submodules and the states actually used. No unrelated submodules were updated to their current branch tips.

## Original tests and additional tests

The original tests are run before local test changes; raw logs are stored outside the sources under `/workspace/work/json-yajl-baseline/`. Afterwards, the same production sources run with extended tests. An old runner failure, missing dependencies, parser rejection and a crash are distinct outcomes.

The two files named in the comment actually contain six v3 and seven v2 cases. This is not the complete JSON coverage: further tests cover argument limits, body limits, transformations and audit logs, among other things. The named v3 file already contains an empty-body case, and the v2 file contains a top-level number. Some v2 expectations explicitly permit missing JSON support; passing those tests alone therefore does not demonstrate functioning JSON processing. The new suites verify active YAJL support and use unambiguous expectations.

The ten mandatory groups are each tracked through stable IDs: A empty body, B keys, C top-level values, D arrays/names, E depth, F argument limit, G UTF-8/Unicode/escapes, H number text, I duplicates, J trailing content/completion. The version-specific tests specify the concrete variants, byte sequences, expected argument lists and result states. Controlled chunk splits are separate parser API tests; splits into HTTP or test-format lines do not demonstrate this property.

## Environment and limitations of the conclusions

Both builds use Debian 13 on x86-64, GCC/G++ 14.2.0 and YAJL from Debian `2.1.0-5+b2` (source package `2.1.0-5`). The distribution changelog lists patches for CVE-2017-16516, CVE-2022-24795 and the revised fix for CVE-2023-33460. This is not unpatched YAJL 2.1.0. Package and library hashes and exact options belong in the version reports or manifest.

This investigation does not promise equal memory consumption for SAX and DOM. A buffered raw body, parser state, decoded tokens, path copies and persistently stored ARGS are different memory components. Parsing first and traversing a tree afterwards can apply a limit at a different time. Measured performance and selection of a replacement library are outside these tests.

## Safe restoration

Publication SHAs are recorded in the final handover report outside each commit. This avoids an endless succession of commits that only add their own SHA. To restore the state, check out precisely that verified SHA from the fork, then initialize the submodules named in the version report at the states recorded in the tree and run the documented build/test commands.

```sh
git clone --no-checkout https://github.com/Easton97-Jens/ModSecurity.git modsecurity-json-baseline
cd modsecurity-json-baseline
git remote add upstream https://github.com/owasp-modsecurity/ModSecurity.git
git -c fetch.recurseSubmodules=false fetch --no-tags upstream \
  refs/heads/v2/master:refs/remotes/upstream/v2/master \
  refs/heads/v3/master:refs/remotes/upstream/v3/master
# Then use the task SHA verified in the handover report:
git switch --detach <VERIFIED-TASK-SHA>
git diff --stat <ORIGINAL-SHA> HEAD
```

The fetch above obtains the branch references currently offered; their tips do **not** replace the pinned SHAs of this investigation. Use `git cat-file -e <ORIGINAL-SHA>^{commit}` to check whether the specific original commit is present, and fetch it explicitly if necessary. No `git pull` is needed. The tested working state is reconstructed from the task commit, including all new files; an older fork branch is insufficient.

## Published tests and final validation

Both new task branches were pushed normally to `origin`, then compared with the locally expected SHA using `git ls-remote`. The following test commits are actually published and verified; the later documentation tips appear with full SHAs in the final handover report. No PRs, releases or tags were created.

| Line | Verified test commit | Main executable tests |
|---|---|---|
| v2 | [a37f9b531d9ba8dd2d60f17eb4f3984668b34c50](https://github.com/Easton97-Jens/ModSecurity/commit/a37f9b531d9ba8dd2d60f17eb4f3984668b34c50) | [tests/json-yajl-baseline/cases.py](https://github.com/Easton97-Jens/ModSecurity/blob/a37f9b531d9ba8dd2d60f17eb4f3984668b34c50/tests/json-yajl-baseline/cases.py) |
| v3 | [57a42a31b4be6f6754dbb0fde662fbee9114cf9e](https://github.com/Easton97-Jens/ModSecurity/commit/57a42a31b4be6f6754dbb0fde662fbee9114cf9e) | [test/json-baseline/run.py](https://github.com/Easton97-Jens/ModSecurity/blob/57a42a31b4be6f6754dbb0fde662fbee9114cf9e/test/json-baseline/run.py) |

| Executed level | v2 PASS / FAIL | v3 PASS / FAIL |
|---|---:|---:|
| Relevant original regressions | 7 / 0 | 50 / 0 |
| Extended native JSON file | 11 / 0 | 9 / 0 |
| New request/rule matrix | 145 / 0 | 163 / 0 |
| Additional real parser chunks | 611 / 0 | included in the matrix above (7) |
| Additional configuration tests | 14 / 0 | included in the matrix above |
| Separate audit matrix | 5 / 0 | 12 / 0 |
| Limited ASan/UBSan | 611 / 0 | 7 / 0 |

The new v2 matrix contains 144 fixtures plus one YAJL control case; v3 contains 159 cases plus four deliberately incorrect expectations as detection controls. The twelve v3 audit checks are ten records plus two such controls. Repeated sanitizer runs and original cases run before/after are not additional unique requirements. SKIP, BLOCKED and XFAIL are each zero for these final functional runs.

Separately: the unchanged general v2 unit suite had **4245 PASS / 1 FAIL** (`ipMatch`, invalid CIDR prefix, existing assertion). LeakSanitizer was blocked by the ptrace environment: **v2 1 BLOCKED, v3 6 BLOCKED**; the other status counts for these finally classified leak checks are zero. In the v3 attempt, the general runner originally reported six FAIL exits; the version report retains both the raw count and the classification of causes. Only the parser/test helper were instrumented for ASan/UBSan, not all other libraries.

The direct final check reran the affected tests after the last fixture change. Empty objects and arrays now explicitly also appear **after** the last scalar array element. Groups A–J have concrete byte-exact expectations on both lines; the tables in the version reports name the variants and limitations. Memory exhaustion, complete thread/offset sanitization contracts and some raw-body-file/audit boundaries in particular remain unproven. The respective version reports list these gaps individually.

The manifest contains the complete separate lists of new and changed files for each version under `deliverable_files` (v2: 17 files; v3: 15 files). Locally generated files are listed under `local_only_untracked_artifacts`; ignored build output and raw logs also remain local. All required synthetic fixtures, generators and test helpers are versioned. Separate fallback patches are unnecessary because both lines have their own published branches.
