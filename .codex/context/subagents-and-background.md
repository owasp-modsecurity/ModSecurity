# Subagents and background work

## Responsibility and scope

This working policy applies when the active environment provides subagents
or background execution. It does not assume that either capability is available.
The coordinating agent remains responsible for task scope, evidence, integration,
validation, delivery decisions, and the final report.

Delegate independent, bounded work when it improves speed or review quality
enough to justify coordination. Read-only source investigation and independent
review are useful candidates. A helper receives no greater authority than the
main task, and delegation does not transfer a material user decision to an agent.

## Define assignments

Each assignment should identify:

- The repository root and relevant source revision.
- The concrete question or deliverable and completion criteria.
- Read-only mode or the exact files the agent may edit.
- Relevant existing instructions and source/evidence requirements.
- Temporary output ownership, if temporary files are necessary.
- Prohibited actions, including any task-specific limits on builds, network
  use, dependency changes, commits, pushes, or external messages.

Give parallel writers disjoint files. Assign a single integrator for shared
indexes, manifests, state records, and final consistency checks. Do not overwrite
another agent's changes or revert unrelated edits to make a local result simpler.

## Core and submodule boundaries

The declared submodules are `others/libinjection`, `others/mbedtls`,
`test/test-cases/secrules-language-tests`, and `bindings/python`.
A delegated investigation of one of these paths must distinguish its repository
and pinned revision from the core repository. Reading a dependency does not
authorize changes to it or to the core gitlink.
Source: [submodule declarations][submodules].

There is no Framework/MRTS assignment model introduced by this policy. Use the
actual core boundaries and the relevant dependency's instructions when its
source is within task scope. The technical guides
[cpp-guide.md](cpp-guide.md), [build-and-test.md](build-and-test.md), and
[configuration-and-rules.md](configuration-and-rules.md) provide reading routes.

## Background commands

Before starting a long-running task, record its purpose, working directory,
process/session reference, relevant output location, expected duration or
timeout, and completion/cancellation condition. Use the environment's supported
process control; do not detach a process in a way that loses its status or owner.

Check [resources-and-storage.md](resources-and-storage.md) for capacity and
output isolation. Concurrent builds or tests must not reconfigure the same
checkout, overwrite shared logs/fixtures, or compete for a runtime port or
mutable service state. Read-only searches can usually be handled independently
without creating a background service.

Monitor progress and collect the terminal exit status. Stop or adjust work
that exceeds its scope, conflicts with another writer, grows unexpectedly, or
cannot be observed. Keep the user informed during sustained work.

## Integration and completion

Inspect each helper's result and supporting evidence. A summary from another
agent is not by itself proof that a source claim or test result is correct;
verify material findings against the affected files or recorded execution.

Before reporting completion, collect required results, finish or stop task-owned
helpers and commands, and confirm no unintended process remains. Retain any
necessary handover evidence and remove only eligible task-owned temporary
files through [cleanup-and-restoration.md](cleanup-and-restoration.md).

A separately authorized durable service or monitoring task must have an
explicit owner and lifecycle; it must not be left behind as an undocumented
side effect of ordinary repository work.

[submodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
