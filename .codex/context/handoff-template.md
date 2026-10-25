# Session handover template

Replace placeholders with actual information before saving. Mark unknowns
explicitly. Create a session file under `.codex/audit/sessions/`; keep this file as a template.

## Identification

- Date and time zone:
- User's task:
- Repository and branch:
- Working commit before/after the session:
- Baseline from `upstream-lock.json`:
- Relevant uncommitted changes:

## Completed outcome

What concrete work is complete? Which files changed, and why?

## Evidence-backed findings

| Finding | Source path and symbol | Commit/link | Direct evidence or inference |
| --- | --- | --- | --- |
| Fill in | Fill in | Fill in | Fill in |

## Checks actually executed

| Command | Working directory | Exit code/result | Limitation |
| --- | --- | --- | --- |
| Fill in | Fill in | Fill in | Fill in |

List unperformed checks and their reasons separately. Do not record planned
commands as successfully executed.

## Decisions

State new decisions and their reasons. Record durable decisions in
`decisions.md`, or link to their entries there.

## Open or blocked

Record specific remaining work, missing information, and observed blockers.
Do not present a guessed cause as established.

## Next concrete step

Describe one actionable continuation, its entry point, required context, and
completion criterion.

## Context files updated

Which state, decision, task, and subject documents were updated?
