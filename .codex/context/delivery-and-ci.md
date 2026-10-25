# Delivery and continuous integration

## Scope and authority

This working policy separates preparation, remote delivery, and verified
results. Follow the user's current target repository and branch. Existing
authorization carries forward; a local-only task does not automatically become
a commit, push, pull request, merge, release, or deployment.

Prepare and review the requested changes fully before asking for any genuinely
missing final authorization. Do not require repeated confirmation for actions
already included in the task. Conversely, a successful check does not grant
authority for a new external destination or branch.

## Prepare a reviewable change

Before an authorized commit or upload:

1. Verify the intended repository, branch, current HEAD, and remote identity.
2. Review the complete task diff, including added files and submodule pointers.
3. Run `git diff --check` and the applicable checks from
   [testing-and-evidence.md](testing-and-evidence.md). Record failures and limits.
4. Stage only the intended files. Exclude credentials, local caches, generated
   runtime logs, and other unrelated or temporary artifacts.
5. Update the English context and handover with the actual state and next step.
6. Confirm that any task-owned background work has completed or been stopped.

Use a focused commit when committing is within scope. Preserve unrelated
working-tree changes and other contributors' work. Do not force-push, rewrite
shared history, merge, or publish a release as an incidental delivery step.

## What the pinned workflow files establish

The upstream files below define CI procedures at the pinned source revision.
They do not establish that Actions are enabled in the selected repository, that a run passed,
or that a particular job is required by current branch protection.

| Workflow | Source-backed role |
| --- | --- |
| `.github/workflows/ci.yml` | Push/PR workflows include Linux build/test configurations and a separate Windows path. |
| `.github/workflows/ci_new.yml` | Push/PR workflows include Linux, macOS, Windows, and cppcheck jobs. |
| `.github/workflows/release-windows.yml` | Defines Windows build/package jobs and a publication job gated on a pushed tag; it also supports manual dispatch. |

Sources: [CI definitions][ci], [additional CI definitions][ci-new],
[Windows release triggers][release-trigger], [publication condition][release-publish].

Use [build-and-test.md](build-and-test.md) for local build details. Do not copy
workflow package installation, machine-wide changes, or release commands into
a documentation task. A push may trigger configured workflows; account for
that real destination behavior when preparing an authorized delivery.

## Bind remote results to the delivered revision

After an authorized push, verify the remote branch points to the intended
commit. When a PR is in scope, verify its target branch and head SHA as well.
Describe the problem, resulting behavior, actual validation, and material
limitations in its text.

Read applicable check results for that exact head. An older successful run,
a workflow file, or a successful upload is not a successful current-head check.
Report pending, failed, cancelled, skipped, inaccessible, and unknown results
accurately. Resolve required review feedback within scope and inspect the new
head after a follow-up push.

SonarQube applies only when the task or actual repository configuration makes
it relevant; use [sonarqube-policy.md](sonarqube-policy.md). Do not invent a
server, project identifier, mandatory quality gate, or passing analysis.

## Report the state reached

Use precise descriptions such as `local_complete`, `committed`, `pushed`,
`pr_open`, `checks_pending`, `remediation_required`, `blocked`, or `failed`.
These are reporting conventions, not claims that a delivery service exists.

Use `verified_pr` only when a PR exists and all applicable required checks and
task-owned feedback for its current head have been resolved with evidence.
State any check whose applicability could not be established. This status
does not authorize merge or release. For a task limited to local preparation,
a complete reviewable diff and truthful validation report can be the final
deliverable.

[ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci.yml#L1-L163
[ci-new]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci_new.yml
[release-trigger]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/release-windows.yml#L9-L31
[release-publish]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/release-windows.yml#L136-L171
