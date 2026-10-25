# Session completion prompt

```text
Prepare a handover for a new chat.

Review the actual diff and identify changed files.
Update .codex/context/current-state.md with the current working state,
checks actually executed and their results, and outstanding questions.
Record durable new decisions in .codex/context/decisions.md and keep
.codex/context/backlog.md limited to actual remaining work.

If a detailed handover is useful, use .codex/context/handoff-template.md
for a record under .codex/audit/sessions/. Cite source paths, symbols,
and the verified commit. Write maintained context in English.

Do not claim a check, publication, or save operation that you did not
actually perform and verify. Keep unverified points visible.
Identify the next concrete action and the first files to read for it.
Run the appropriate final context consistency check.
```
