# Current working state

Recorded on **2026-10-25**. Recheck the actual branch, commit, and remote state
when resuming; branch names and PR status can change.

## Goal and completed content

Add English `AGENTS.md` and comprehensive reusable `.codex` context to the existing
**Easton97-Jens/ModSecurity** fork. Explain the repository, central functionality,
C++/C interfaces, configuration, builds, and tests from official v3 source. Follow
the owner-selected connector repository's context structure, tailored to the core.

The routed layout, technical guides, working policies, optional assessment
profiles, component/check guides, manifests, validators, prompts, and handover
mechanism are prepared. The [index](index.md) is the entry point. The
[adaptation record](template-adaptation.md) explains the template mapping and the
reference-specific assumptions that were omitted.

## Verified source boundaries

| Item | Recorded revision or scope |
| --- | --- |
| Official product source | `owasp-modsecurity/ModSecurity`, `v3/master`, `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` |
| Existing fork target, latest observed comparison | `Easton97-Jens/ModSecurity:v3/master`, same `effb6c0...` commit |
| Structure template | `Easton97-Jens/ModSecurity-conector:master`, `42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3` |
| Main-source inventory | 850 entries: 846 blobs and 4 gitlinks |
| Reference inventory | 72 tracked `.codex` files plus root `AGENTS.md`, `README.md`, `.gitignore`, and `Makefile` |
| Product changes | None outside `AGENTS.md`, `CODEX_START.md`, and `.codex/` |

The first fork observation was 22 commits behind upstream; the subsequent target
observation matched the baseline. These are separately retained historical facts,
not a 22-commit product delta in this context delivery. See [provenance](provenance.md)
and [upstream-lock.json](upstream-lock.json).

## Verification actually completed

All commands below were run from the repository root with Python 3.12.14 and Git
supporting `--no-lazy-fetch`. Final document counts are reported by the verifier
because later handover updates can add Markdown and links.

| Check | Observed result |
| --- | --- |
| `python3 .codex/bin/verify-context.py --strict-source` | Exit 0; pinned tree, complete source inventory, local document targets, 487 unique pinned ModSecurity source links and line ranges, and unchanged product-source boundary |
| `python3 .codex/bin/validate-structure.py` | Exit 0; 93 routed files, required layout, identity, and manifest consistency |
| `python3 .codex/bin/validate-codex-inheritance.py --check` | Exit 0; 23 local policy digests, no active child inheritance |
| Pinned template-link verification | Valid local Git objects and line ranges at the reference commit |
| Independent documentation/source review | No outstanding material finding; stale current file count corrected |
| Independent helper probes in an isolated copy | Missing/bad/outside-root links, stale digests, duplicate IDs, and invalid paths rejected; historical evidence and fenced examples handled as documented |
| Helper fixes rechecked | Dated session records need no active route; colliding derived policy IDs are rejected before writes; catalog symlinks are rejected without changing an outside sentinel file |

No semantic completeness or security guarantee follows from these checks. The
[foundation handover](../audit/sessions/2026-10-25-context-foundation.md) records
scope and verification details.

**Not executed:** C++ compilation, ModSecurity unit/regression execution, Windows
build, parser regeneration, library installation, connector integration, or a
security scan. The task changes context and local documentation helpers only.
Submodule implementations were not initialized or comprehensively reviewed.

## Delivery state

Working branch: `codex/kontext-v3` in `Easton97-Jens/ModSecurity`.
The English adaptation is published in the existing fork, with
[draft pull request #120](https://github.com/Easton97-Jens/ModSecurity/pull/120)
open against `v3/master`. Its comparison contains 98 context additions and no
product-source changes. No merge has been performed.

The published adaptation commit `bad6cd355fd78ffcfa6393b1c3ada8d734feb098` had tree
`9a884e6e02525a469bfa531e8aaa767cf915e856`, matching the reviewed local tree. It was
cloned separately from the fork; all three validators passed and the checkout
was clean. A subsequent context-only update records these delivery facts. Use
`git rev-parse HEAD` or the PR head for the latest working commit.

The PR-triggered workflow query returned no runs for the adaptation commit at
the time checked. This is not a passing CI result or evidence that no workflow
can run later. No runtime/build success is claimed.

## Next concrete step

Open PR #120 for review and use `codex/kontext-v3` in new chats until the
context change is merged. Recheck the actual remote state when resuming. For a later product task, read the relevant guides and
actual source, establish the intended behavior and environment, and select
proportionate verification. The first real C++ build remains future task evidence.
