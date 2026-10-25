# Decisions

These decisions describe the working foundation created on 2026-10-25.
No history from earlier conversations is assumed. Future entries should state
the date, decision, reason, affected revision, and consequences.

## D001 — Use the official v3/master source as the technical baseline

**Decision:** The initial documentation describes official commit
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`.

**Reason:** The owner requires facts grounded in the official ModSecurity v3
source. The fork was 22 commits behind at the first comparison. A later check,
before opening the pull request, found that its target branch already matched
the documented upstream baseline. Both observations are recorded in
[provenance.md](provenance.md).

**Consequence:** A later source update requires explanations and references to be
reviewed again. Source comparison and documentation changes must be distinguished
from an actual update of the fork's target branch.

## D002 — Keep startup instructions concise and subject knowledge separate

**Decision:** `AGENTS.md` defines the working rules and reading order. Detailed
explanations live in `.codex/` and are read according to the task.

**Reason:** Reusable context needs clear navigation. One long startup instruction
would mix operating rules with technical detail. The [index](index.md) routes
readers to the appropriate chapters.

**Consequence:** New sessions read the current state and reopen code for detailed
questions. Automatic loading of every document or remembering every previous
conversation is not promised.

## D003 — Maintain all added material in English

**Decision:** Documentation, assistant instructions, prompts, handovers, and
checker messages are maintained in English, following the owner's explicit
clarification. Actual API names, paths, commands, branch identifiers, and quoted
source diagnostics retain their original spelling.

**Reason:** The owner requested that the complete addition be in English.
Preserving source identifiers keeps explanations directly searchable in code.

## D004 — Distinguish source evidence from recommendations

**Decision:** ModSecurity explanations cite concrete source locations at the
pinned commit. Suggested workflows and learning paths are identified as project
conventions. Known conflicts are recorded in [fact-check.md](fact-check.md).

**Reason:** The owner explicitly requires that information not be invented or
asserted without verification. A plausible README summary alone is insufficient.

## D005 — Preserve the original source for this documentation task

**Decision:** This task adds `AGENTS.md`, `CODEX_START.md`, and `.codex/`. Existing
implementation, build definitions, tests, and original documentation remain
unchanged relative to the pinned baseline.

**Reason:** The task is a reusable, understandable working foundation. Product
behavior changes would require their own requirements and verification.

**Consequence:** Strict context verification checks that boundary. Documenting
an upstream discrepancy does not automatically fix it in the product.

## D006 — Do not invent verification success

**Decision:** Source/document checks and ModSecurity builds/tests are reported
separately. Only commands actually run are recorded as executed. Unperformed
checks remain visible.

**Reason:** Reading source establishes specific code facts, not successful
execution on a later target platform.

## D007 — Use the existing fork

**Decision:** Following the owner's explicit clarification, the files belong in
`Easton97-Jens/ModSecurity`. The working branch is `codex/kontext-v3`, with a pull
request targeting that fork's `v3/master`.

**Reason:** The owner wants the existing fork as the working location. A fresh
comparison before the pull request found that its `v3/master` already matched
the documented official baseline. The pull request therefore contains only the
context additions; the earlier lag is retained as a historical observation.

**Consequence:** Read and work on the context branch before integration into
`v3/master`. Merging is a separate action. Actual branch and pull-request state
is recorded in [current-state.md](current-state.md).

## D008 — Adapt the selected connector context layout

**Date:** 2026-10-25.

**Decision:** Follow the structure inspected at connector-template commit
`42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3`, with `context/`, routed policies and
profiles, security checks, `bin/`, `audit/`, and explicit manifests. Place detailed
core guides and durable session context in that layout.

**Reason:** The owner explicitly selected the existing connector setup as the
structural model. Core component areas replace connector host deltas. No
Framework/MRTS topology, machine paths, mandatory unavailable wrappers, historical
connector outcomes, or bilingual requirement is imported.

**Consequence:** The [adaptation record](template-adaptation.md) identifies the
scope and changes. The local catalog checks primary-policy digests; it does not
activate policy inheritance in submodules. ModSecurity product facts continue to
come exclusively from the pinned official core source.

## D009 — Make the root agreement reusable across repositories

**Date:** 2026-10-25.

**Decision:** Keep `AGENTS.md` independent of a particular project, account,
branch, implementation language, build system, or installed helper. Discover
available project context conditionally. Place this fork's concrete constraints
and startup procedure in [repository-context.md](repository-context.md), routed
from the index. The `.codex` guides and validators remain specific to this core.

**Reason:** The owner clarified that the agreement must be valid for all
repositories. A reusable root must also be usable without this `.codex` layout,
Python helpers, or a Git checkout.

**Consequence:** The same root file can be reused unchanged elsewhere. This change
does not distribute it to other repositories or install global instructions.
It preserves project-specific source authority and verification through the local
context instead of embedding those assumptions in the portable agreement.

## D010 — Leave chat language to each person

**Date:** 2026-10-25.

**Decision:** Maintained repository files remain English. The repository agreement,
local context, and startup prompts prescribe no chat language. Each person chooses
the language of their conversation.

**Reason:** The owner explicitly clarified that chat language is an individual
choice. A language requested in one conversation must not become a repository-wide
default. D003 continues to govern maintained files.

## D011 — Use optional RTK summaries with official Codex integration guidance

**Date:** 2026-10-25.

**Decision:** Prefer supported RTK commands for routine summaries when the tool is
available, with native-tool fallback. Keep the root rule portable and route the
source-checked examples and setup boundaries through [rtk.md](rtk.md). Codex
mechanisms follow official OpenAI documentation; RTK commands follow the inspected
RTK `v1.0.1` source at `4ef8730cf3a0dea11c8d64a9b90bc0616fe52100`.

**Reason:** The owner requested RTK to reduce token usage and explicitly required
compliance with official Codex documentation. Reading fewer routine output bytes
is useful, but a filtered summary cannot replace exact evidence.

**Consequence:** Source inspection, complete diffs, diagnostics, machine-readable
data, and final validation use suitable unfiltered output. ModSecurity build/test
commands and context-validator implementations stay unchanged. This task adds
guidance without installing RTK, running its initializer, enabling hooks/proxies,
or changing personal Codex settings. No actual token or billing saving is claimed.
