# Current working state

Recorded on **2026-10-25**, Europe/Berlin. This is a dated task record, not a
checkout instruction. Resolve the actual repository, branch, commit, worktree,
and live PR head/base before resuming.

## Current task and repository identity

Review and correct [upstream PR #3706](https://github.com/owasp-modsecurity/ModSecurity/pull/3706).
The canonical project is `owasp-modsecurity/ModSecurity`, development branch
`v3/master`. The PR metadata observed at the start of this correction was:

| Field | Observed value before correction |
| --- | --- |
| Base repository and branch | `owasp-modsecurity/ModSecurity:v3/master` |
| Base commit | `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` |
| Head repository and branch | `Easton97-Jens/ModSecurity:codex/kontext-v3` |
| Head commit | `a35e8c3a1116a763f286f965f718f36d5f8876cc` |
| PR state | Open, not a draft, not merged |

The fixes belong on the verified head branch of this existing PR. The separately
requested German branch is outside this correction. No new PR, merge, or direct
push to the upstream base branch is part of the task. Historical fork PR #120
is not the integration target of upstream PR #3706.

The manifests declare canonical project identity, not a push destination.
Contributor forks remain valid checkouts. The portable root `AGENTS.md` remains
repository-independent; [repository-context.md](repository-context.md) supplies
project facts. Files in this English contribution remain English. No chat
language is prescribed; each person chooses their conversation language.

## Content and source boundaries

The context covers repository layout, architecture, C++17/C APIs, ownership,
transaction processing, rules/configuration, builds/tests, optional RTK use,
working policies, assessment profiles, and session handovers. The [index](index.md)
routes to the relevant material. These are documented project conventions,
not automatic memory, inherited submodule policies, or built-in Codex manifests.

| Evidence | Recorded revision or scope |
| --- | --- |
| Official ModSecurity source | `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`; 850 entries, 846 blobs and four gitlinks |
| Structure reference | `Easton97-Jens/ModSecurity-conector`, `42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3`; selected layout files only |
| RTK reference | `rtk-ai/rtk` tag `v1.0.1`, `4ef8730cf3a0dea11c8d64a9b90bc0616fe52100`; documentation/source inspection, no installation or measured savings |
| Codex reference | Official OpenAI AGENTS.md and hooks documentation read on 2026-10-25; see [provenance](provenance.md) |
| Product changes | Strict source validation found no differences outside `AGENTS.md`, `CODEX_START.md`, and `.codex/` |

The initial and subsequent contributor-fork comparisons remain separate
historical observations in [provenance.md](provenance.md) and
[upstream-lock.json](upstream-lock.json). They do not configure the working
repository. The [foundation record](../audit/sessions/2026-10-25-context-foundation.md)
and the [pre-correction state](https://github.com/Easton97-Jens/ModSecurity/blob/a35e8c3a1116a763f286f965f718f36d5f8876cc/.codex/context/current-state.md)
preserve earlier preparation, RTK, portability, and verification history.

## Review outcome

Seven of the eight inline review findings were confirmed and addressed:

- Document Git 2.45 or later for `--no-lazy-fetch`.
- Explain that the pinned `make check` driver path can return zero despite
  failure markers; require inspection of counts and `.log`/`.trs` output.
- Replace raw remote URL output with redacted identity inspection.
- Resolve the current PR head before choosing a checkout.
- Make project identity consistent across entry points, manifests, and validators.
- Link the engine and rule guides separately in the response-intervention check.
- Protect the source inventory from overwriting existing uncommitted work.

The alleged duplicate Markdown reference is a false positive at the reviewed
head: `[rules-evaluate]` points to `RulesSet::evaluate()`, whereas `[rule-evaluate]`
points to `RuleWithOperator::evaluate()`. The existing references remain intact.
All 12 Python-helper functions now have explanatory docstrings; the local AST
count is separate from a future external CodeRabbit coverage result. The minor
historical wording correction was also applied. Details and evidence are in the
[review record](../audit/sessions/2026-10-25-upstream-pr-review.md).

## Verification and limits

The correction tree delivered in `acb380ac1ade0e111db36edfceaccac49d05a30e`
passed these checks from the repository root:

| Executed check | Result |
| --- | --- |
| `python3 .codex/bin/verify-context.py --strict-source` | Exit 0; 91 Markdown files, 508 local link targets, 488 unique pinned ModSecurity source links; unchanged product source and gitlinks |
| `python3 .codex/bin/validate-structure.py` | Exit 0; 95 routed files consistent |
| `python3 .codex/bin/validate-codex-inheritance.py --refresh` then `--check --json` | Exit 0; 23 policy digests regenerated deliberately and checked; no active child inheritance |
| `git diff --check` | Exit 0 |
| Python parsing, docstring count, and source-verifier AST comparison | All helpers parsed; 12/12 functions documented; source-verifier executable AST unchanged |
| Isolated helper error-path probes | Wrong identities rejected; denied refresh preserved catalog bytes and mtime; explicit canonical refresh migrated a stale identity |
| Independent reviews | No concrete defect found in the corrected helpers, project/checkout guidance, or remote/inventory examples |

The [review record](../audit/sessions/2026-10-25-upstream-pr-review.md) describes
additional targeted probes and their limits. The source inventory, upstream lock,
portable root agreement, and correctly distinct rule-reference labels remain
unchanged by this correction.

At the pre-correction head `a35e8c3a`, the upstream workflow API returned completed,
successful `Quality Assurance` run `40190804854` and `Quality Assurance new` run
`40190804889`. The combined status contained `CodeRabbit: success`; the observed
SonarQube Cloud comment reported its quality gate passed. These observations
apply to that old head only and are not results for a later correction commit.

No full ModSecurity C++ build, product unit/regression suite, Windows build,
connector integration, security scan, or Codex/RTK runtime integration is executed
as part of this correction. The small shell-driver reproduction uses failing
stubs and is not a product test pass. Submodule contents remain separate and
were not comprehensively reviewed. Structural checks do not prove semantic
completeness or security.

## Delivery and continuation

The correction was published as `acb380ac1ade0e111db36edfceaccac49d05a30e`
on the verified existing head branch. The GitHub API then showed that exact head
on upstream PR #3706, with base `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`;
the PR remained open and unmerged. A fresh fetch returned the same commit.
Its tree `51c3975b22895100e9731882883de5fae2444242` matched the validated local
index exactly, and the local checkout was clean after the guarded fast-forward.
The correction changed 32 context/helper files.

At the delivery observation, `Quality Assurance` run `40221460086` was queued,
`Quality Assurance new` run `40221460043` was in progress, and `CodeRabbit` was
pending for `acb380ac`. No successful result for that head is claimed here.
This receipt is a documentation-only follow-up to that observed commit. For any
later head, including a commit containing this receipt, query its own checks
again. Recorded old-head results never substitute for current-head evidence.
No new PR, merge, remote review reply, or thread-resolution action was performed.

For a later session, select the live task/PR revision, read the relevant guides
and source, and choose proportionate verification. Use [backlog.md](backlog.md)
for real remaining work. The first local full C++ build remains a separate task.
