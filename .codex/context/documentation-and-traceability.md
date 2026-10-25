# Documentation and traceability

These files preserve useful context across chats. They record what is known,
what changed, and how the result was checked. They do not make an assistant
remember an inaccessible conversation or prove that every file was reviewed.

## Language and source fidelity

Maintain all added documentation, instructions, prompts, and checker messages
in English. Preserve source symbols, paths, commands, configuration keys,
Git IDs, URLs, and quoted diagnostics exactly. A German companion is not
required by this repository's working convention.

For technical claims, cite the implementation or contract actually read using
the source path, relevant symbol, and full source commit. A commit-pinned GitHub
link with a suitable line range is useful when it supports the statement.
Use [provenance.md](provenance.md) and [upstream-lock.json](upstream-lock.json)
to identify the documentation baseline.

Distinguish four kinds of statement:

| Kind | What the reader needs |
| --- | --- |
| Source fact | The implementation, declaration, build condition, or test definition at an identified revision |
| Executed result | The actual command, source revision, relevant environment/options, result, and limitations |
| Inference | The supporting observations and the reasoning; label it as an inference |
| Proposal or uncertainty | The intended change or unanswered question; do not describe it as implemented or verified |

A test definition establishes an expected case. An executed test establishes
its observed result under that run's conditions. A README, class name, scanner
result, or generated report cannot substitute for evidence of a broader claim.
Record useful documentation/code discrepancies in [fact-check.md](fact-check.md).

## Where information belongs

| Information | Primary location |
| --- | --- |
| Project purpose and the library/connector boundary | [project-context.md](project-context.md) |
| Runtime structure and code navigation | [architecture.md](architecture.md), [repository-map.md](repository-map.md), and the relevant technical chapter |
| Actual working state, verification, and next step | [current-state.md](current-state.md) |
| A durable decision and its reason | [decisions.md](decisions.md) |
| Real unfinished work or an unresolved question | [backlog.md](backlog.md) |
| Substantial session-specific history and evidence | [audit sessions](../audit/sessions/), using [handoff-template.md](handoff-template.md) |
| Source baseline and intentional updates | [provenance.md](provenance.md), [upstream-sync.md](upstream-sync.md) |

Maintain one authoritative explanation for each topic and link to it from
supporting policies. Keep current state concise. Preserve significant historical
observations with their original dates and revisions instead of rewriting them
to appear current. Remove or mark superseded plans when later instructions
change the task.

## What to record after a change

For a meaningful change, retain enough information to explain:

- the request and affected scope;
- the old and new behavior or documentation;
- the actual files changed and material decisions;
- commands actually executed, outcomes, and checks not run;
- unresolved issues and a concrete next step;
- commit, remote branch, PR, or integration facts when actually observed.

A short session entry or updated current-state section is usually sufficient.
Do not introduce a second reporting system or mandatory bilingual change
record. Never invent a commit, PR number, review result, CI status, merge, test
outcome, performance figure, or security guarantee.

Keep credentials, private request bodies, personal data, and sensitive logs out
of versioned context. Prefer a minimal sanitized reproduction and bounded
diagnostics. Do not record full environment dumps as evidence.

## Generated data and consistency checks

If a file is generated, identify its actual source and generator before
changing it. Regenerate it through that process when required; do not treat
hand-editing its output as a substitute. The source inventory update is covered
by [upstream-sync.md](upstream-sync.md).

From the repository root, the local context checks are:

```sh
python3 .codex/bin/verify-context.py
python3 .codex/bin/validate-structure.py
```

Read their reported scope and limitations. Structural consistency and valid
source links do not establish the semantic correctness of every paragraph or a
successful ModSecurity build. If a required tool or object is unavailable,
record the limitation and perform the useful checks that remain possible.
