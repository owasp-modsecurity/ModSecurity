# Working with the existing ModSecurity fork

This working context targets the existing fork
[`Easton97-Jens/ModSecurity`](https://github.com/Easton97-Jens/ModSecurity).
The context files are provided on the working branch `codex/kontext-v3`.
Review [draft PR #120](https://github.com/Easton97-Jens/ModSecurity/pull/120)
for integration into the existing fork's `v3/master`.
The pull request targeting `v3/master` makes the proposed changes
reviewable; its actual status is recorded in
[current-state.md](current-state.md).

## 1. Select the correct branch

For a new local checkout:

```sh
git clone --branch codex/kontext-v3 https://github.com/Easton97-Jens/ModSecurity.git ModSecurity
cd ModSecurity
git status --short
git log -1 --oneline
python3 .codex/bin/verify-context.py --strict-source
```

For an existing working copy, inspect its changes and remotes first.
If `origin` actually points to this fork and the working tree allows
a branch switch:

```sh
git fetch origin
git switch codex/kontext-v3
```

These steps assume that the remote working branch already exists.
The context check requires Python 3 and Git with `--no-lazy-fetch`.
Then read [CODEX_START.md](../../CODEX_START.md) and use the
[start prompt](prompts/start-new-chat.md) for a new chat.

## 2. Understand the source baseline and pull request

The technical documentation describes the official v3 commit
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`. When first observed, the
fork's `v3/master` branch was at
`2e08f23dd50e7c0aa7c969b244b9d3a6dce4d641`, 22 upstream commits
behind. The working branch therefore contains the documented official
baseline and the additional context files.

The target branch was checked again before opening the pull request:
`v3/master` had meanwhile reached the same documented official
commit. The pull request against that baseline therefore contains only
the context additions. [provenance.md](provenance.md) distinguishes the initial
comparison from the fresh one. Product-code changes are not part of
the context additions.

Until the pull request is merged, a new chat must select the working
branch to read `AGENTS.md` and `.codex/`. After it is merged, the files
will also be available on the updated target branch. A link to the fork
alone does not guarantee that a chat has opened the correct branch;
check the branch and commit explicitly.

## 3. Build and submodules

Before building, provide the tools and libraries described in
[build-and-test.md](build-and-test.md) for the actual platform.
Initialize the submodules pinned by the main repository with
`git submodule update --init --recursive`. This step is described in the
official
[Git checkout workflow](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L65-L98).
The exact gitlinks are recorded in
[upstream-lock.json](upstream-lock.json).

The included build instructions were derived from the sources. Actual
build and test results for the target environment must be recorded as
new verification evidence. The existence of this branch does not prove
that a C++ build has been run.

## 4. New chats and subsequent changes

At the start of a chat, identify or provide the selected branch,
`AGENTS.md`, the index and `current-state.md`. Then state the specific task.
After a session, save the actual work state, decisions and open questions.
[session-workflow.md](session-workflow.md) describes the process; use
[upstream-sync.md](upstream-sync.md) when updating the upstream baseline.
