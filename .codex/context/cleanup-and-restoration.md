# Cleanup and workspace restoration

## Preserve ownership

This working policy covers cleanup after a scoped task. A worktree, local
branch, remote branch, PR, external clone, submodule checkout, gitlink, and
GitHub repository are distinct objects. Authorization to change one does not
authorize deletion or replacement of another.

Preserve the main checkout, existing user work, other agents' changes, and
unknown files. An ignored filename or a path named `tmp` is not enough to
establish that the task owns it. Do not use broad deletion, `git clean`, a
hard reset, forced checkout, or an automatic stash as routine cleanup.

## Temporary data and generated files

Keep an inventory appropriate to the work: temporary fixture paths, build or
runtime roots, process references, retained logs, and any worktrees created
for the task. Remove only clearly owned data after its retention needs and
active users are resolved.

The source tree has generated and ignored build products, but build cleanup
also has specific semantics. The root Makefile lists maintainer-clean files;
the parser Makefile's `CLEANFILES` includes generated scanner/parser files.
Inspect the applicable target before using it, especially when parser changes
or generated output are part of the task.
Sources: [ignored files][ignore], [maintainer-clean list][maintainer],
[parser cleanup list][parser-clean].

Do not delete generated output merely because it can normally be regenerated
when it contains unreviewed work or the required generator is unavailable.
Leave it intact and report the reason if safe removal cannot be established.

## Worktrees and branches

Before removing a task-owned worktree, verify its exact path and repository,
tracked/staged/untracked state, unique or unpublished work, active processes,
and whether evidence or another task still depends on it. Use the normal Git
worktree mechanism only when removal is safe and within scope. Do not manually
delete a registered worktree or force removal to bypass unresolved state.

Local branch deletion requires that the branch is no longer needed, has no
required unique work, and is not in use by another worktree. Prefer safe
non-forcing deletion. A remote branch or PR requires its own task disposition;
completion of local editing alone does not authorize remote deletion.

## Restore only a state actually requested

Do not switch to a branch named `master` or `v3/master` merely to make the
workspace look finished. Preserve the branch chosen for the task unless
restoration or integration is part of the user's request.

When a return to another branch is authorized, first inspect the actual root,
branch, HEAD, status, and intended remote. Switch without forcing, preserve
uncommitted work, and verify the resulting state. A remote update is a separate
network action; use it only when included in the task and avoid rewriting
unrelated history.

## Submodule restoration

The core repository pins four declared submodule paths. Treat the gitlink and
the nested checkout separately; a nested detached HEAD can simply reflect the
recorded dependency revision. Do not switch dependencies to their own default
branches or update them to newer commits as cleanup.
Source: [submodule declarations][submodules].

If task work changed a nested checkout, inspect its own status and preserve
required work before returning it to the revision recorded by the parent.
Dependency initialization can require network access and must not be hidden
inside a supposedly offline cleanup step.

Finish by reporting any retained temporary state, unfinished process cleanup,
or blocked restoration with its concrete reason. Follow
[subagents-and-background.md](subagents-and-background.md) for processes and
[resources-and-storage.md](resources-and-storage.md) for storage.

[ignore]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitignore
[maintainer]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L26-L51
[parser-clean]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L35-L43
[submodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
