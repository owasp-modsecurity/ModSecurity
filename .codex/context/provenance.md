# Provenance, source baseline and evidence

## 1. Recorded source baseline

| Field | Verified value |
| --- | --- |
| Official repository | `https://github.com/owasp-modsecurity/ModSecurity` |
| Development branch examined | `v3/master` |
| Pinned commit | `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` |
| Git tree | `d626de814f96ceea7fd717d6176b30a17a86f08e` |
| Commit time according to Git | `2026-10-24T18:06:05+02:00` |
| Inspection | 25 October 2026, date interpreted in Europe/Berlin |
| Main repository inventory | 850 entries: 846 blobs and 4 gitlinks |

The [commit](https://github.com/owasp-modsecurity/ModSecurity/commit/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729)
and its [source tree](https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729)
are the baseline. The values were read using `git show`, `git rev-parse` and
`git ls-tree -r`. The complete machine-readable inventory is in
[source-index.tsv](../audit/source-index.tsv), and the metadata is in
[upstream-lock.json](upstream-lock.json).

“850 entries” refers to the Git tree, not to 850 individually explained
functions or a comprehensive code review. A blob can also be an image file,
for example. A gitlink refers to a commit in a separate repository.

## 2. Historical origin in the contributor fork

At the first comparison on the inspection date, `Easton97-Jens/ModSecurity`, also on branch `v3/master`,
pointed to `2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641`. This commit is an ancestor
of the official baseline above. The comparison showed **22 additional
upstream commits and 0 commits present only on the compared fork branch**.
This describes those two branch tips at that time,
not every branch or the future state of the fork.

Evidence: [fork commit](https://github.com/Easton97-Jens/ModSecurity/commit/2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641),
[comparison of the two commits](https://github.com/owasp-modsecurity/ModSecurity/compare/2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641...effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729).
After fetching the v3 history, the following checks were also run locally:

```sh
git merge-base --is-ancestor \
  2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641 \
  effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729
git rev-list --count \
  2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641..effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729
git rev-list --count \
  effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729..2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641
```

Results in that order: exit code `0`, count `22`, count `0`.
**Historical recheck before the original fork pull request:** The fork's `v3/master` branch
had meanwhile also reached
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`. Comparing it with the first
context commit, `fbf883634454ba39e4c49aa2f87483b9c4610902`, showed one additional
documentation commit and 25 changed files. The branch tips were
also read using `git ls-remote`. No assumption is made here about who
updated the target branch.

The original fork pull request against that updated baseline therefore contained only
the context additions. The original 22-commit difference is historical
provenance information, not an additional product-code diff in the context change. Both observations are
preserved separately in the lockfile. They do not configure the current checkout
or delivery destination. The canonical project identity is
`owasp-modsecurity/ModSecurity`; discover the actual working repository and, for
PR work, the live head and base from the selected task.

## 3. Submodules are separate sources

The four direct gitlinks in the main commit are:

| Path | Pinned submodule commit |
| --- | --- |
| `bindings/python` | `bc625d5bb0bac6a64bcce8dc9902208612399348` |
| `others/libinjection` | `211782219663f889f471650150df12b623c5766e` |
| `others/mbedtls` | `0fe989b6b514192783c469039edd325fd0989806` |
| `test/test-cases/secrules-language-tests` | `f73c73027ef49ccf99c7911744929a71d15ff419` |

Source for the mappings and URLs: [.gitmodules](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules).
Source for the commit IDs: gitlinks in the
[pinned source tree](https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729),
reproducible with `git ls-tree -r` and recorded in the inventory.

The internal implementations of these submodules were not fully examined
for this documentation. Statements about their use are based on callers
and build definitions in the main repository. A change within a submodule
requires its actual commit to be read and checked separately.
This also applies to nested submodules.

## 4. Which source answers which question?

| Question | Preferred evidence |
| --- | --- |
| Public signature | Declaration in the relevant header and function definition |
| Actual behavior | Implementation, callers, state and error paths |
| Accepted rule syntax | Scanner, parser grammar and specific parser actions |
| Enabled or disabled feature | Build definitions, preprocessor switches and runtime path |
| Expected behavior | Existing tests and traceable requirements |
| Successful in this environment? | An actual recorded build or test run |
| Historical context | Git commit and diff, clearly distinguished from current code |

The README, comments and Doxygen help with orientation, but may differ from
the implementation. [fact-check.md](fact-check.md) lists concrete examples.
A registered test and a successful test run provide different evidence.

## 5. Our additions and external product documentation

`AGENTS.md`, `CODEX_START.md` and `.codex/` were added for this task.
The workflows, handoff templates and context verifier are conventions
for this working repository. They are not presented as existing upstream
features or instructions from the ModSecurity maintainers.

The `.codex` organization is adapted from the owner-selected connector repository
at the revision recorded in [template-adaptation.md](template-adaptation.md). Its
policies are structural references, not evidence for libmodsecurity behavior.
The local manifest schemas and validators are explicitly added working conventions.

The official OpenAI [AGENTS.md guide](https://learn.chatgpt.com/docs/agent-configuration/agents-md)
and [hooks reference](https://learn.chatgpt.com/docs/hooks) were read on 2026-10-25
for Codex instruction discovery and supported integration behavior. Their scope
is Codex; they do not establish OpenAI endorsement of RTK.

The optional [RTK guide](rtk.md) separately uses `rtk-ai/rtk` tag `v1.0.1`, commit
`4ef8730cf3a0dea11c8d64a9b90bc0616fe52100`. Its documentation and selected command,
tracking, and Codex initialization implementations were inspected. This is a
reference revision, not an installed dependency or a comprehensive RTK review.
The example commands were not executed with RTK in this environment.
**Neither external source is authority for ModSecurity behavior.**

This documentation approach requires no particular model version,
API key or adoption of personal settings. The files must actually be
accessible to the chat or coding assistant using them.

## 6. Limits of the consistency check

`verify-context.py` checks Git objects, the inventory, local link targets and
commit-pinned source paths, including line ranges. Current technical chapters
must match the pinned baseline commit. Historical session and decision
evidence may reference its own full commit ID and is checked against
that commit's locally available Git objects.

The verifier does not fetch anything from the network or run build programs.
A missing required Git object is reported as an error. The check requires
a Git version with `--no-lazy-fetch`. It does not check whether external
websites are reachable or whether every sentence is semantically supported
by its link. That last task remains a code and documentation review.

The starting state and all checks actually performed are recorded in
[current-state.md](current-state.md). [upstream-sync.md](upstream-sync.md)
describes maintenance when adopting a new upstream version.
