# Feasibility policy

Assess feasibility in proportion to the task. The purpose is to identify a
workable next step, not to require permission for every reversible action or a
formal plan for every small edit.

## Establish what the task needs

Before substantial implementation, a build, an external operation, or delivery,
resolve the relevant questions:

- Is the desired outcome clear enough to implement and check?
- Which repository and source revision own the affected behavior?
- Which source files, interfaces, or existing tests provide evidence?
- Are the needed tools, dependencies, and configured features available?
- Can the work fit the actual storage, memory, CPU, and process constraints?
- Is the action within the user's authorization and available permissions?
- Is there a meaningful way to validate the requested result?

Use [architecture-and-boundaries.md](architecture-and-boundaries.md) for scope,
[build-and-test.md](build-and-test.md) for build prerequisites, and
[git-policy.md](git-policy.md) for delivery destinations.

## Describe the actual state

These labels can make substantial work easier to track; a short explanation is
enough when a formal status table would add no value:

| State | Meaning and next action |
| --- | --- |
| `feasible_now` | The relevant prerequisites exist; proceed within scope |
| `feasible_after_local_setup` | Reversible, task-owned setup can supply the prerequisites; perform it when already authorized |
| `requires_user_decision` | A material choice cannot be resolved from the task or evidence; prepare the concrete options |
| `blocked_environment` | A needed local capability is unavailable; finish independent work and explain the gap |
| `blocked_external_dependency` | A necessary external input or service is unavailable |
| `blocked_permissions` | A required action is outside granted authority or permitted access |
| `blocked_missing_evidence` | The available evidence cannot establish a necessary claim |
| `already_fixed` | The identified revision already resolves the reported issue; cite the evidence |
| `unsupported_with_evidence` | Specific source, build, or execution evidence establishes the stated limitation |
| `not_applicable` or `out_of_scope` | The item does not belong to the current task; explain why if material |

A missing compiler, unavailable dependency, uninitialized submodule, or failed
network request does not by itself prove that ModSecurity lacks the feature.
Keep environment limitations separate from product limitations.

## Continue useful work

Authorization and preferences already established in the conversation persist
unless the user changes them. Do not request them again merely because the work
enters a new file, phase, or chat tool. Reversible inspection, edits, and setup
that are necessary and within the authorized task can proceed.

Complete independent research, edits, and reviewable preparation when another
part is blocked. Ask only for a missing decision or permission that actually
prevents the remaining work. Explain the concrete action and why it needs the
user's input; do not make the user approve an unprepared proposal.

Optional unavailable tools do not block work that can be adequately checked
with available repository-native methods. Do not install system packages,
change services, or use paid external resources merely to satisfy a copied
tool list. Determine whether such setup is necessary and already authorized.

When the evidence or environment changes, revise the assessment. Report the
actual remaining gap under [definition-of-done.md](definition-of-done.md).
