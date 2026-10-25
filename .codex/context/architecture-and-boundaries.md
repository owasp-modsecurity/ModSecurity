# Architecture and repository boundaries

This policy assigns work to the correct source and evidence boundary. It is a
working convention for this fork. Read [project-context.md](project-context.md),
[architecture.md](architecture.md), and [repository-map.md](repository-map.md)
for the source-backed technical explanation.

## Repository identity

The working repository is `Easton97-Jens/ModSecurity`. Its context branch is
`codex/kontext-v3`, with integration targeting the fork's `v3/master`.
The official source is `owasp-modsecurity/ModSecurity`, branch `v3/master`.
Use [upstream-lock.json](upstream-lock.json) for the documented source revision
and [current-state.md](current-state.md) for the observed working state.
Read the actual branch and commit before acting; branch names are moving refs.

The core library consumes HTTP data supplied by an application or connector
and evaluates configured rules. Web-server connectors are separate projects.
The upstream [overview](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md)
and the [Transaction interface](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/headers/modsecurity/transaction.h)
establish that boundary. Do not attribute a connector's host hooks, transport
handling, or deployment behavior to this library without inspecting the
separately identified connector revision.

## Work ownership

| Area | What belongs here | Boundary to preserve |
| --- | --- | --- |
| `headers/modsecurity/` and corresponding `src/` definitions | Core types, C++ and C interfaces, library behavior | Inspect declarations, definitions, and callers together; an installed header does not promise ABI stability |
| `src/parser/` | Rule-language scanner, grammar, parser driver, and generated parser files | Distinguish `.ll`/`.yy` inputs from generated output |
| Other `src/` areas | Rule evaluation, actions, transformations, operators, variables, body processing, collections, logs, and utilities | Follow the affected implementation and build conditions instead of inferring support from a filename |
| `configure.ac`, `Makefile.am`, component build files, and `build/` | Compiler/dependency detection and platform build definitions | Build instructions depend on the actual platform and configured features |
| `test/` | Main-repository tests, test programs, and their data | Keep the separately pinned test-corpus submodule distinct |
| `examples/` and `tools/` | Library examples and supporting utilities | An example or tool result establishes only its demonstrated behavior |
| `AGENTS.md`, `CODEX_START.md`, and `.codex/` | Assistant instructions, technical explanations, workflows, and saved context | These are local additions, not upstream runtime features |

The source mappings and installed-header list are recorded in
[src/Makefile.am](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am).
The complete directory inventory and further evidence are in the
[repository map](repository-map.md).

## Submodules and external projects

The main source records four direct submodule paths:
`bindings/python`, `others/libinjection`, `others/mbedtls`, and
`test/test-cases/secrules-language-tests`. The registrations are in
[.gitmodules](https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules);
their actual commit pins are recorded in [provenance.md](provenance.md).

Treat submodule source and Git history as read-only unless the user's task
includes changes there. Reading relevant pinned source or initializing pinned
dependencies for an authorized build does not authorize editing that source,
advancing its branch, or changing the main repository's gitlink. Avoid
`git submodule update --remote` as routine setup. If a fix belongs inside a
submodule, establish the required scope before modifying it and keep its
evidence and delivery state separate.

External connectors, independently supplied rulesets such as the Core Rule Set,
and ModSecurity v2 are separate investigation scopes. Record their revisions
when they are relevant. A core-library change does not establish their
compatibility, runtime behavior, or test outcome.

## Evidence boundary

Distinguish source reading, build success, test definitions, executed tests,
and connector integration results. None automatically establishes the others.
Keep findings tied to the source revision, build options, dependencies, input,
and environment actually inspected or exercised.

For a cross-boundary symptom, start with the observable input and the relevant
core calls. A focused library reproduction may locate the fault more precisely;
failure to reproduce in the core alone is not proof that an external component
is defective. Record that uncertainty under [findings-policy.md](findings-policy.md).
