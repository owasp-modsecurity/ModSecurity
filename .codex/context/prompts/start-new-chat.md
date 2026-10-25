# New chat startup prompt

This is a template. First ensure that the chat has access to the correct
repository and current branch or files. Replace the last line with your task.

```text
Work on the selected ModSecurity checkout and the task's actual branch.
The canonical project is owasp-modsecurity/ModSecurity; a contributor fork is valid.
For a pull request, resolve its current head repository, branch, and full commit
before selecting the checkout. Verify the local HEAD against that revision.
Do not choose a branch or delivery destination from historical session notes.

First read AGENTS.md, .codex/context/index.md, .codex/context/repository-context.md,
.codex/context/current-state.md, .codex/context/decisions.md, and
.codex/context/upstream-lock.json.
Check the actual branch, commit, and worktree where your tools allow it.
For a checkout, run the available local context checks described in .codex/bin/README.md.
Load only the relevant technical guides, policies, and task profile from the index.

Ground ModSecurity claims in the actual code at the stated official v3/master
revision. Read the implementations and tests relevant to the task.
For technical findings, provide the path, symbol, and commit-pinned evidence.
Distinguish verified facts, inferences, proposals, and unverified points.
Do not invent earlier decisions, API signatures, or test results.
Maintain repository files, documentation, prompts, and handovers in English.

Start with a brief summary of:
- the source baseline and current working state;
- verified previous work relevant to my task;
- actual open questions or blockers;
- the next step that follows from that evidence.

If a file or previous finding is inaccessible, identify the gap precisely.
Ask when a missing decision would materially change the task.
Otherwise continue within the authorized scope.

At the end, update the current working state and relevant decisions so the
next session can continue from the saved results.

My specific task: [Enter the task here.]
```
