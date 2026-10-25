# Integration into the fork's v3/master

This filename follows the chosen policy layout. The target described here is
`Easton97-Jens/ModSecurity:v3/master`, not an assumed branch named `master`.
The context work is prepared on `codex/kontext-v3`. Check the actual branch and
PR state before acting.

## Authorization

Merging into the fork's `v3/master` must be within the user's authorized task.
An explicit merge instruction already given in the conversation remains valid
for that scope unless the user changes or revokes it. Do not ask for the same
authorization again at each phase.

An instruction only to prepare changes, push a branch, or create a PR does not
by itself request a merge. If integration is desired but not yet authorized,
finish the concrete changes, review, and validation first so the user can
decide on a reviewable result. State the specific remaining action and why
confirmation is needed.

Authorization for this fork does not authorize writes or integration into the
official upstream repository or a submodule repository.

## Before an authorized merge

Verify the conditions applicable to the actual repository and current PR:

1. The repository, selected PR, head branch, and `v3/master` target are correct.
2. The final diff and PR description reflect the intended scope and current code.
3. Required local and remote checks have meaningful results for the current
   head; outstanding required failures or missing evidence are resolved.
4. Required reviews, conversations, branch protections, and mergeability
   conditions are satisfied.
5. Material task-owned feedback is addressed, with any remaining uncertainty
   reported under [pr-remediation.md](pr-remediation.md).
6. Concurrent task-owned work will not change the branch during the operation.
7. The merge method follows the user's instruction and actual repository settings.

Determine required checks and services from the repository's real configuration.
Do not invent a required scanner, external quality gate, deployment, or review
that the repository does not use. Do not disable a real required gate or use an
administrative bypass to obtain a merge.

## Bind the action to the reviewed revision

Immediately before merging, re-read the current head SHA. Use a supported
head-matching mechanism when the available API or tool provides one. If the
head changed, review the difference and repeat checks whose evidence is no
longer applicable. Do not merge an unseen revision based on an older review.

If the base moved or a conflict was resolved, assess the resulting diff and
required checks again. Do not treat previous results as evidence for code that
they did not exercise. A target-branch update is not permission to include
unrelated changes or advance dependency pins.

## After integration

Confirm the authoritative PR merge state and resulting target-branch commit.
Check applicable post-merge workflows when they are part of the requested
outcome, and distinguish completed, pending, failed, and unobserved results.

Record the observed PR, head and target commits, checks, and remaining work in
[current-state.md](current-state.md) and a useful
[session record](../audit/sessions/). Do not claim complete integration from a
button click or tool return without confirming the outcome.

Preserve unrelated local work. Retain task branches or worktrees unless cleanup
is requested or already included in the authorization; remove only identified
task-owned objects when it is safe to do so.
