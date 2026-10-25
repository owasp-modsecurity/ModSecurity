# Upstream PR #3706 review correction

Recorded on **2026-10-25**, Europe/Berlin. This is a dated evidence record,
not a checkout or publication instruction for a later task.

## Scope and inspected revision

The user requested review and correction of
[upstream PR #3706](https://github.com/owasp-modsecurity/ModSecurity/pull/3706).
The GitHub API identified the open, non-draft PR as follows before correction:

- Base: `owasp-modsecurity/ModSecurity:v3/master`,
  `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`.
- Head: `Easton97-Jens/ModSecurity:codex/kontext-v3`,
  `a35e8c3a1116a763f286f965f718f36d5f8876cc`.
- Scope: correct the English context and its helpers on that existing head.
  The separately requested German branch is outside this change. No new PR,
  merge, review reply, or thread-resolution action is part of the correction.

Review statements were checked against the actual files and their cited source.
They were not treated as instructions to install or invoke suggested tools.

## Inline findings and disposition

| Review comment | Verified finding | Correction or disposition |
| --- | --- | --- |
| [4370775743](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775743) | `--no-lazy-fetch` requires Git 2.45 or later. | State the minimum version in the entry points and helper documentation. The source verifier's executable logic is unchanged. |
| [4370775746](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775746) | The pinned suite script and custom driver can report failures while returning zero. | Explain the control flow and require inspection of failure-marker counts and `.log`/`.trs` output in the build and testing guides. |
| [4370775749](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775749) | Raw remote URL output can contain credentials. | Replace `git remote -v` examples with one read-only example that captures effective URLs, redacts user-info/query/fragment, and hides unsupported forms; link to it from the update guide. |
| [4370775753](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775753) | Alleged duplicate rule-evaluation reference. | False positive at the reviewed head: `[rules-evaluate]` and `[rule-evaluate]` are distinct, correctly used labels. Leave `project-context.md` and reference parsing unchanged. |
| [4370775754](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775754) | The startup prompt selected a saved contributor branch. | Resolve the selected PR's live head repository, branch, and full commit before choosing a checkout. |
| [4370775762](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775762) | Upstream-facing guidance and manifests treated a contributor fork/PR as the project and integration target. | Use canonical project identity consistently; discover the actual checkout, PR head/base, and authorized destination separately. Update both identity validators and the policy catalog; preserve dated fork history as history. |
| [4370775764](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775764) | The combined engine/rules link pointed only to the rules guide. | Link the engine and rules guides separately. |
| [4370775766](https://github.com/owasp-modsecurity/ModSecurity/pull/3706#discussion_r4370775766) | Inventory regeneration could overwrite existing work. | Require a regular tracked target matching HEAD and the index, check status/content before generation and immediately before replacement, and require exclusive task ownership. |

Seven inline findings were confirmed; one was rejected using the actual source.
The associated docstring warning was addressed by adding meaningful function
docstrings. A local AST count found 12 documented functions out of 12; this is
not a claim that a later external coverage check has already run. The small
historical wording issue (`main source tree`) was also corrected.

The review's subprocess diagnostics did not establish request injection: the
verifier uses argument arrays without a shell, fixed Git operations, validated
object IDs, and locally obtained Git values. The helper guide explicitly requires
a trusted Git executable on `PATH`. No external scanner was rerun, and this
inspection does not claim that arbitrary executable search paths are safe.

## Supporting source

- [Git 2.45 release notes](https://github.com/git/git/blob/master/Documentation/RelNotes/2.45.0.adoc)
  introduce `--no-lazy-fetch`; the [versioned Git manual](https://git-scm.com/docs/git/2.45.0)
  explains that it prevents lazy fetching from a promisor remote.
- At the pinned ModSecurity baseline, `Makefile.am` lines 91–93 select
  `test/test-suite.sh` and `test/custom-test-driver`. The suite script's final
  `cd -` normally determines its status. The custom driver's `estatus` assignment
  and failure-marker counts do not cause a failing exit on its ordinary path.
  See the immutable references in [build-and-test.md](../../context/build-and-test.md#what-a-test-result-establishes).
- In [project-context.md](../../context/project-context.md), `[rules-evaluate]`
  refers to `RulesSet::evaluate()` in `src/rules_set.cc`, while `[rule-evaluate]`
  refers to `RuleWithOperator::evaluate()` in `src/rule_with_operator.cc`.

## Focused verification performed during correction

The task used temporary fixtures outside the working repository and independent
read-only review. These checks were scoped to the changed documentation helpers
and examples, not to ModSecurity runtime correctness.

| Check | Observed result |
| --- | --- |
| Helper behavior probes | Seventeen CLI probes covered accepted canonical identity, conflicting/equally wrong identities, stale policy digests, deliberate refresh, and refused refresh. Expected outcomes observed. |
| Independent helper review | No concrete defect found. Additional isolated probes observed no writes or changed mtimes for four rejected routing identities; a permitted refresh wrote only the catalog. Fixtures had no Git checkout or `origin`. |
| Source-verifier preservation | AST comparison, excluding docstrings, matched the reviewed source verifier. All three helpers parsed; all 12 functions had docstrings. |
| Remote-display examples | Both embedded Python examples compiled. Eighteen synthetic URL cases and effective fetch/multiple push destinations, including `insteadOf` and `pushInsteadOf`, passed. Synthetic credentials and failed-command raw output were not displayed; configuration was unchanged. |
| Inventory example | A clean isolated local checkout regenerated a byte-identical inventory. Existing edits, missing target, symlink, wrong working directory, invalid baseline ID, and an edit injected during generation were rejected without losing existing data. |
| Test-driver reproduction | Unchanged pinned scripts invoked failing unit/regression stubs. The suite script and custom driver returned zero despite two failure markers in each case. This confirms the documented status trap; it is not execution of the real product suites. |

The remote display is not a general detector for confidential material in a
host or repository path; the guide states that boundary. The inventory example
does not lock out concurrent writers and therefore still requires exclusive
ownership. A staged-dirty inventory fixture was not executed; status and
HEAD/index comparisons implement that guard. No real credentials or remote URL
configuration were used in the synthetic probes.

Independent read-only review of the remote/inventory snippets found no concrete
defect; it also parsed both examples and checked eight synthetic redaction cases.
A separate document review found the canonical identity and historical/current
checkout distinction consistent.

## Integrated validation and delivery

From the repository root, after all policy edits and the deliberate catalog
refresh, the correction tree delivered in `acb380ac1ade0e111db36edfceaccac49d05a30e`
passed:

| Command or check | Observed result |
| --- | --- |
| `python3 .codex/bin/verify-context.py --strict-source` | Exit 0; baseline available and ancestor of HEAD; inventory 850 entries (846 blobs, four gitlinks); 91 Markdown files, 508 local link targets, 488 unique pinned ModSecurity source links; no product-source differences |
| `python3 .codex/bin/validate-structure.py` | Exit 0; 95 routed files consistent |
| `python3 .codex/bin/validate-codex-inheritance.py --refresh` | Exit 0; deliberately regenerated the 23 local policy digests |
| `python3 .codex/bin/validate-codex-inheritance.py --check --json` | Exit 0; `ok: true`, 23 policies, `refreshed: false`, no active child repositories, no errors |
| `git diff --check` | Exit 0 |
| Final integrated helper inspection | All helper files parsed; 5/5, 3/3, and 4/4 functions documented; source-verifier executable AST matched the pre-correction head |
| Additional isolated integration probes | Canonical fixture accepted without Git metadata; equally wrong identities rejected; denied refresh returned 1 with no catalog byte/mtime change; explicit canonical refresh repaired an old catalog identity and passed recheck |

The upstream PR metadata was rechecked before delivery: head `a35e8c3a` and
base `effb6c0f` were unchanged. A fresh `git ls-remote` confirmed the English head
and the separate German branch at `4a483fe21be6c30f1e0880fcbee7c358528db2f2`.
The local German checkout was clean at that revision.

### Observed delivery receipt

The following publication facts were verified before this follow-up receipt was
written on 2026-10-25 (18:29 UTC):

| Field | Observed result |
| --- | --- |
| Published correction | `acb380ac1ade0e111db36edfceaccac49d05a30e` |
| Parent | `a35e8c3a1116a763f286f965f718f36d5f8876cc` |
| Published tree | `51c3975b22895100e9731882883de5fae2444242`, identical to the validated local index |
| Destination | Existing `Easton97-Jens/ModSecurity:codex/kontext-v3` head, updated with an expected-head guard and without force |
| Upstream PR | #3706 reported the published commit as its head; still open and unmerged; base unchanged |
| Read-back | A fresh fetch returned the published commit and tree; guarded local fast-forward left a clean worktree |
| Changed files | 32 context/helper files; none outside `.codex/` and `CODEX_START.md` |
| Checks for `acb380ac` at observation | `Quality Assurance` run `40221460086` queued; `Quality Assurance new` run `40221460043` in progress; `CodeRabbit` pending |

This receipt adds documentation after the observed correction commit. A later
head, including the receipt's own commit, needs its own remote check query.
Old-head CI is recorded separately in [current-state.md](../../context/current-state.md);
it is not evidence for a later correction head. No new PR, merge, review reply,
or thread-resolution action was performed.

No full local C++ build, product test suite, connector/host test, security scan,
or Codex/RTK runtime integration was executed in this correction. Source inventory
and product-code preservation are checked separately from documentation meaning.
