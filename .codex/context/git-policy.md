# Git policy for the ModSecurity core fork

This policy supports reviewable work in the existing fork. It does not replace
the user's task instructions or authorization already established in the
conversation. Use [master-integration.md](master-integration.md) for merging
into the target branch.

## Repository and branch identity

| Role | Repository | Branch used by this context task |
| --- | --- | --- |
| Official source | `owasp-modsecurity/ModSecurity` | `v3/master` |
| User's working fork | `Easton97-Jens/ModSecurity` | `codex/kontext-v3` |
| Integration target in the fork | `Easton97-Jens/ModSecurity` | `v3/master` |

The source baseline is separately recorded in
[upstream-lock.json](upstream-lock.json). A branch name is not a fixed source
revision. See [using-this-fork.md](using-this-fork.md) and
[provenance.md](provenance.md) for context.

Before editing or delivering, inspect the relevant repository state:

```sh
git status --short
git branch --show-current
git rev-parse HEAD
git remote -v
```

Use the established working branch for this task. For later independent work,
choose a suitable task branch or isolated worktree when it helps preserve other
work. Do not make a new worktree or branch a mandatory ritual. Do not switch,
reset, or recreate the user's active branch merely because a generic guide
suggests a different starting point.

## Preserve work and scope

Inspect unstaged and staged changes before staging or committing. Preserve
pre-existing tracked and untracked work. Stage explicit task-owned paths, for
example `git add -- <task-files>` with the actual intended paths supplied.
Do not use broad staging in a mixed working tree.

Do not perform destructive cleanup, discard changes, rewrite published history,
force-push, or delete branches as routine housekeeping. Those actions need to
be part of a clearly authorized task and must respect repository protections.
Resolve ordinary conflicts within an authorized change when the intended
result is clear; ask when a conflict exposes an unresolved product decision.

Submodule source, history, and gitlinks remain bounded by
[architecture-and-boundaries.md](architecture-and-boundaries.md). A core commit
does not authorize publishing a change to a dependency repository.

## Remote delivery

When commit, push, or PR work is in scope, retain that authorization across the
task. Verify the actual repository identity, fetch and effective push
destinations, selected branch, and PR base before a remote write. Do not assume
that a remote named `origin` points to this fork or that one named `upstream`
has the intended meaning.

A destination mismatch requires investigation. Do not silently choose a
different repository, create a replacement repository, or rewrite remotes to
hide the mismatch. Complete the reviewable local work while any genuinely
missing destination decision is resolved.

Keep commits coherent and their messages factual. After an authorized push,
check that the intended remote branch and PR head identify the delivered
commit. A local commit or a successful command invocation alone is insufficient
evidence of the final remote state.

Creating a PR does not by itself authorize merging it. Do not bypass branch
protections, push directly to `v3/master` as a shortcut, or claim a merge before
it is observed. Record actual delivery facts in
[current-state.md](current-state.md) or the relevant
[session record](../audit/sessions/).
