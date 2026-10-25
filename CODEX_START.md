# Working on ModSecurity with Codex and ChatGPT

Start here for the additional English project documentation.

**Technical baseline:** the official
[`owasp-modsecurity/ModSecurity` repository, `v3/master`](https://github.com/owasp-modsecurity/ModSecurity/tree/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729),
commit `effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729`, inspected on 2026-10-25.
The original files at that revision are the foundation of the explanations.
Working instructions and context files are identified as additions for this project.

## What is included

| Entry point | Content |
| --- | --- |
| [AGENTS.md](AGENTS.md) | Assistant instructions, evidence requirements, and session startup |
| [.codex/context/index.md](.codex/context/index.md) | Technical guides, policy owners, task profiles, and reading paths |
| [Project context](.codex/context/project-context.md) | What libmodsecurity provides and the scope of this repository |
| [Repository map](.codex/context/repository-map.md) | Directories, languages, build files, and useful entry points |
| [Architecture](.codex/context/architecture.md) | HTTP transaction data flow, central classes, and processing |
| [C++ guide](.codex/context/cpp-guide.md) | C++17, the C interface, ownership, and reading the existing code |
| [Feature map](.codex/context/feature-map.md) | Where features are implemented and relevant tests are defined |
| [Configuration and rules](.codex/context/configuration-and-rules.md) | SecRules, configuration, operators, and actual limitations |
| [Build and test guide](.codex/context/build-and-test.md) | Source-backed preparation, build paths, and meaningful checks |
| [Current state](.codex/context/current-state.md) | Completed work, verification, open questions, and blockers |
| [Provenance](.codex/context/provenance.md) | Upstream revision, fork comparison, submodules, and evidence rules |

## Layout adapted to your connector repository

The structure follows the inspected `.codex` layout of
[`Easton97-Jens/ModSecurity-conector`](https://github.com/Easton97-Jens/ModSecurity-conector/tree/42fd5a8e895aca3a36b09eb09fe5c7cf9c3399e3/.codex):
`context/`, `context/profiles/`, `context/security/`, `bin/`, `audit/`, and
explicit manifests. Core component guides replace connector-specific guides.
The [adaptation record](.codex/context/template-adaptation.md) explains the mapping
and distinguishes reusable workflow from machine- or connector-specific rules.

## Why AGENTS.md and .codex?

`AGENTS.md` supplies concise instructions at the repository entry point. Codex
uses these files along its project instruction path. The longer chapters are
stored in `.codex/`, and the startup instructions direct the assistant to the
relevant files. The official
[AGENTS.md guide](https://developers.openai.com/codex/agent-configuration/agents-md)
describes instruction discovery.

In this repository, `.codex/` is a versioned documentation directory. Names such
as `current-state.md` are project conventions. They do not promise that every
product automatically loads every file. This setup does not select a model or
overwrite personal Codex configuration.

A new chat can continue from these files when it has access to them and reads
them. For ordinary ChatGPT use, provide repository access or the current files.
Decisions that exist only in a previous conversation are not present in this
system. Updating the saved working state is therefore part of finishing a session.

## Starting a new chat

Open the repository in your working environment, or provide the relevant files
in the chat. Then use a prompt such as:

> Work on this ModSecurity repository. First read AGENTS.md,
> .codex/context/index.md, .codex/context/current-state.md, and
> .codex/context/decisions.md.
> Check the actual checkout against .codex/context/upstream-lock.json.
> Summarize the verified working state and open questions. Do not invent
> missing information. My next task is: ...

A longer ready-to-copy prompt is in
[start-new-chat.md](.codex/context/prompts/start-new-chat.md). Use
[end-session.md](.codex/context/prompts/end-session.md) when handing work over.

These files belong in the existing `Easton97-Jens/ModSecurity` fork.
[using-this-fork.md](.codex/context/using-this-fork.md) explains how to open the
`codex/kontext-v3` working branch and use it in new chats.

## Checking context consistency

From the repository root:

```sh
python3 .codex/bin/verify-context.py
python3 .codex/bin/validate-structure.py
python3 .codex/bin/validate-codex-inheritance.py --check
```

When the upstream source is expected to be unchanged, also use:

```sh
python3 .codex/bin/verify-context.py --strict-source
```

The structure and catalog validators require Python 3.11 or later. The source
checker requires Python 3 and Git with `--no-lazy-fetch`,
which prevents automatic object downloads in partial clones. It checks the
pinned commit, source inventory, local document links, and the existence and
line ranges of commit-pinned ModSecurity references. It reports worktree
differences. It does not replace reading the code, building, or testing.
See [validator instructions](.codex/bin/README.md) for scope and policy-digest maintenance.

This documentation explains repository structure and central behavior in detail.
It is not a verified description of every C++ function or evidence that a
particular installation is secure.
