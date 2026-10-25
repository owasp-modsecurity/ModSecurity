# Working with ModSecurity checkouts and pull requests

The canonical project is
[`owasp-modsecurity/ModSecurity`](https://github.com/owasp-modsecurity/ModSecurity),
with the documented development line `v3/master`. These guides also work in a
contributor fork. The filename is retained for existing links; it does not select
a particular fork, branch, pull request, or delivery destination.

## 1. Select the checkout required by the task

For a new checkout of the canonical development branch:

```sh
git clone --branch v3/master https://github.com/owasp-modsecurity/ModSecurity.git ModSecurity
cd ModSecurity
git status --short
git branch --show-current
git log -1 --oneline
```

This selects the published development branch. It does not include an unmerged
pull request merely because that PR proposes these context files. If the task is
to review or correct a PR, resolve its current head first instead of defaulting
to the development branch or a contributor branch from a saved session.

For an existing working copy, preserve its current changes. Inspect the repository
and effective fetch/push destinations with the redacted inspection in
[git-policy.md](git-policy.md) before fetching or switching. The name `origin`
alone does not establish a repository identity or permission to publish.

## 2. Resolve a pull request before choosing its branch

For the PR named in the current task:

1. Read its current base repository/branch, head repository/branch, and full head
   commit from the hosting service. A contributor fork may supply the head while
   the PR targets the canonical project.
2. Verify that the checkout or selected remote matches the intended head source.
   Fetch that verified ref and select a suitable branch or isolated worktree only
   when the existing changes allow it. Preserve the user's unrelated work.
3. Compare `git rev-parse HEAD` with the observed PR head commit before treating
   the checkout as that PR. Recheck the remote head before publishing corrections.
4. Follow the task's authorized destination. Updating a PR head is separate from
   creating another PR, pushing directly to its base, or merging it.

Historical entries in [current-state.md](current-state.md), decisions, and session
records identify earlier work; they are not unconditional checkout instructions.
A repository link alone does not guarantee that a chat opened the correct branch.

When the selected checkout contains these files, read
[CODEX_START.md](../../CODEX_START.md) and use the
[start prompt](prompts/start-new-chat.md). The source consistency check is:

```sh
python3 .codex/bin/verify-context.py --strict-source
```

It requires Python 3 and Git 2.45 or later for `--no-lazy-fetch`. Use the strict
mode only when product files are expected to match the pinned baseline exactly.
See [bin/README.md](../bin/README.md) for all checks and their limits.

## 3. Understand the source baseline and history

The technical documentation describes official v3 commit
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`. The first contributor-fork comparison
and its later update to this baseline are recorded in
[provenance.md](provenance.md) and [upstream-lock.json](upstream-lock.json).
Those observations remain historical evidence rather than a configured working
repository. Context additions do not change product code at this baseline.

Before these files are merged into a selected base branch, a session must read
the actual PR head to access them there. After a merge, verify the resulting base
revision rather than assuming it contains the latest context or passed tests.

## 4. Build and submodules

Before building, provide the tools and libraries described in
[build-and-test.md](build-and-test.md) for the actual platform.
Initialize the submodules pinned by the main repository with
`git submodule update --init --recursive`. This step is described in the official
[Git checkout workflow](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L65-L98).
The exact gitlinks are recorded in [upstream-lock.json](upstream-lock.json).

The included instructions were derived from source. Record actual build and test
results for the target environment as new evidence. A checkout, source-linked
explanation, or successful context check is not a completed C++ build.

## 5. New chats and subsequent changes

At the start of a chat, identify or provide the selected repository, branch and
commit, `AGENTS.md`, the index, and `current-state.md`. Then state the specific task.
After a session, save the actual work state, decisions and open questions.
[session-workflow.md](session-workflow.md) describes the process; use
[upstream-sync.md](upstream-sync.md) when updating the upstream baseline.
