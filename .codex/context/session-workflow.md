# Workflow for reproducible sessions

This chapter defines the collaboration approach selected for this working
repository. It is not a feature implemented by ModSecurity.

## 1. Establish the task and starting point

Read the reusable [AGENTS.md](../../AGENTS.md) and this checkout's startup
instructions in [repository-context.md](repository-context.md). Check the branch
and status, and identify the task. Compare the checkout with
[upstream-lock.json](upstream-lock.json). Older notes apply to their stated
revisions. Check whether relevant files have changed since those notes were made.

Keep user decisions, code findings, and working hypotheses distinguishable.
Ask about an unresolved decision with material consequences. Routine choices
within the authorized task can be made, explained, and recorded.

## 2. Derive explanations from code

Find concrete symbols and paths, for example with `rg`. Read implementations,
callers, error paths, and relevant tests as well as declarations. For optional
features, also inspect build definitions and parser acceptance.

A useful explanation answers these questions in order:

1. What purpose does the function serve in this repository?
2. What data does it receive, and who owns that data?
3. What state or return values can it change?
4. Who calls it, and in what order?
5. What conditions, feature switches, and limits apply?
6. Which test defines the expected behavior, and was that test executed?

Use links containing full commit IDs. Moving branch links are useful for
navigation but do not provide a stable evidence baseline. Label conclusions
inferred from multiple functions and state the evidence behind them.

## 3. Make changes

First define the observable behavior required. Limit edits to the affected
files and inspect instructions applicable to their directories. Preserve
unrelated changes and avoid an unrequested redesign.

For a defect, use an existing or focused new regression test that can actually
detect it. Explanations and documentation corrections call for corresponding
source and document checks. A test that merely repeats an implementation detail
is not a substitute for a requirement.

## 4. Record verification results

| Field | Information to record |
| --- | --- |
| Target | The behavior or document consistency being checked |
| Revision | Commit, branch, and relevant uncommitted changes |
| Environment | Relevant platform and available dependencies |
| Command | The exact command executed, not an idealized later version |
| Working directory | A repository-relative directory or clear local path |
| Result | Exit code, material output, executed tests, and skipped tests |
| Limit | What the run did not cover or what prevented completion |

The context checker is an added project helper, not an upstream test. A passing
run does not establish correctness for every platform, configuration, or
security property.

## 5. Prepare the next chat

Keep `current-state.md` concise and current. Record the last reliable state,
outstanding questions, and the next concrete action. Store longer historical
records under `.codex/audit/sessions/`.

Use `decisions.md` for durable decisions and their reasons. Use `backlog.md` for
actual remaining work. Link to the relevant chapter instead of duplicating a
long explanation across all three files.

Use [handoff-template.md](handoff-template.md) for a handover. Save context in
the actual shared working copy or authorized repository. Saying something in
a chat does not update a project file.

## Limits

These files give new sessions a checkable starting point. They do not read or
update themselves, and they can become stale. Every session therefore starts
with an access check, revision check, and focused reading. This reduces repeated
orientation work without presenting unavailable information as remembered fact.
