# Backlog and open questions

The current contribution is [upstream PR #3706](https://github.com/owasp-modsecurity/ModSecurity/pull/3706).
Resolve its live head and base before acting. This list records actual remaining
work; it does not assert a product defect or a completed optional assessment.
Historical contributor-fork PRs do not select the current integration target.

| Item | Current state | Next concrete action |
| --- | --- | --- |
| Review correction for upstream PR #3706 | Published in `acb380ac`; integrated checks passed and upstream PR head verified | Inspect the current head's own CI and any subsequent review; recorded earlier-head results are not current-head results |
| Integrate context into the selected base | Not merged by this task | Review the actual current PR and integrate only when authorized |
| First product-code task | No behavior change selected in this context task | Specify the intended change; inspect the relevant implementation and tests |
| Actual target development environment | No user deployment/build environment established | For a build task, identify OS/compiler/dependencies and initialize the pinned submodules |
| First local C++ build and executed product tests | Not run in this documentation task | Follow `build-and-test.md`; record configured features, commands, counts, logs, executed/skipped tests, and results |
| Future upstream updates | Baseline pinned and current at its inspection | Use `upstream-sync.md`, reread changed behavior, and refresh evidence deliberately |

Do not replace unknowns with plausible defaults. Add a task only when requested
or when it follows from a verified issue within the authorized scope. Remove or
update completed entries instead of carrying stale plans into future chats.
