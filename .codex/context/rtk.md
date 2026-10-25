# RTK: optional command-output reduction

Use this guide when RTK is available or its setup is part of the task. It supports
[command-execution-policy.md](command-execution-policy.md); it is not a mandatory
startup read. The reusable root agreement prefers supported RTK summaries and
allows native tools when RTK is absent or full output is needed.

## Sources and scope

Inspected on **2026-10-25**: RTK tag `v1.0.1`, commit
[`4ef8730cf3a0dea11c8d64a9b90bc0616fe52100`][rtk-tree]. This reference revision is
not a minimum-version requirement or an installed dependency. Check the actual
installed version and its help before using these examples.

RTK provides commands that reduce terminal output presented to an assistant.
Its [README][rtk-readme] explains the intended output reduction. For Codex
behavior, use OpenAI's official [AGENTS.md guide][codex-agents] and
[hooks reference][codex-hooks]. RTK is an external tool; these OpenAI sources do
not establish an OpenAI recommendation or guarantee for RTK. Neither source
changes the official ModSecurity product baseline in [upstream-lock.json](upstream-lock.json).

## Manual use with an available executable

Check availability through the current environment, then inspect the installed
CLI. These examples are commands to run deliberately, not recorded test results:

```sh
rtk --version
rtk --help
rtk git --help
```

For routine exploration in this checkout, the inspected release implements:

| Command | Useful output and limit | Evidence |
| --- | --- | --- |
| `rtk git status` | Working-tree overview; use native status when exact entries or a machine-readable format matter. | [`run_status`][rtk-status] |
| `rtk git diff -- src/transaction.cc` | A focused change overview. The default path compacts the diff and may elide content. Inspect the complete native diff before approving changes. | [`run_diff`][rtk-diff] |
| `rtk git log -5` | Recent commit overview with an explicit count; inspect a full commit when its body or exact metadata matters. | [`run_log` argument handling][rtk-log] |
| `rtk gain --project --text` | Static command-output figures for the current directory scope; these counts are estimates. A separate proxy section can have a different scope. | [Analytics guide][rtk-gain], [scope flag][rtk-gain-cli] |
| `rtk gain --project --history` | Recent tracked commands in that scope; earlier tasks can still be included. | [Analytics guide][rtk-gain], [scope handling][rtk-gain-scope] |

Start with scoped searches and bounded file reads. Do not load every context
chapter or collect repetitive logs merely because a filter is available.
Use native `rg` and exact source ranges when establishing code behavior.
In this release, `rtk read` defaults to `--level none`, so its presence alone
does not imply compressed source. See the [CLI definition][rtk-read].

## Keep exact evidence available

RTK summaries are for orientation. Use full source, relevant raw diagnostics,
complete diffs, and real exit statuses for conclusions. Do not pipe a filtered
summary into a parser, generate a patch from it, or interpret a missing detail
as evidence that the underlying output contained none.

With this repository's manual setup, run the native command directly. If an
existing RTK integration rewrites it, the inspected release also provides an
unfiltered execution command:

```sh
git --no-pager diff -- src/transaction.cc
rtk bypass git --no-pager diff -- src/transaction.cc
```

Choose one suitable form; both lines are alternatives. `rtk bypass` still tracks
usage and remains subject to normal execution permissions. In this release,
`rtk proxy` is a deprecated alias for it; the separate `llm-proxy` feature has a
different purpose. See the [CLI definition][rtk-bypass]. For a command with side
effects, retain its original output instead of rerunning it just to obtain a
longer log. Do not assume every filter stores complete recoverable output.

Keep ModSecurity builds/tests and the three context checks on their documented
native paths in [build-and-test.md](build-and-test.md) and
[bin/README.md](../bin/README.md). Do not invent a `rtk make` command or substitute
`rtk test make check`: the inspected generic runner classifies that test command
as unknown, with a limited-output fallback. See [test-runner classification][rtk-test]
and [fallback output][rtk-test-fallback].
RTK does not add ModSecurity test coverage or prove that a test executed.

## Codex integration follows official OpenAI documentation

This repository uses explicit instructions in `AGENTS.md`, with details routed
through the context index. OpenAI documents the instruction discovery chain;
arbitrary `.codex` Markdown files are not all automatic startup instructions.
Do not rely on an undocumented `@file` import. See [the official guide][codex-agents].

Optional automatic rewriting is a separate setup choice. OpenAI documents
`PreToolUse` input rewriting, hook configuration, and review/trust through `/hooks`
for supported Codex workflows. Project hooks require a trusted project layer.
Local command hooks are not supported with cloud orchestration, even when tools
execute locally. Check the actual client and current [hooks documentation][codex-hooks].

RTK's own [Codex setup guide][rtk-codex] describes its initializer. A deliberate
setup review can start with `rtk init --codex --dry-run` after checking local help.
The actual initializer writes `RTK.md`, patches `AGENTS.md`, and configures
`.codex/hooks.json`; global mode targets personal Codex configuration instead.
These are configuration changes, not prerequisites for manual invocation. See
the [target paths][rtk-init], [write operations][rtk-init-writes], and
[dry-run option][rtk-init-cli].

This addition does not run that initializer, install hooks, enable a proxy, or
change personal configuration. A later project-hook adoption must review its
generated files and the context boundary: a new root `RTK.md` is outside the
current `--strict-source` additions. Review actual loaded instructions and hooks
in that client before claiming automatic use. Keep Codex permissions in effect.

## Measure output reduction without inventing savings

The inspected implementation estimates tokens as UTF-8 byte length divided by
four, rounded up; it is not a model tokenizer. The [estimator][rtk-estimator]
and [tracking calls][rtk-tracking] establish that calculation. The command-output
figures in `rtk gain` use RTK's tracked estimates; output compression does not
translate directly to the same percentage of total conversation tokens,
subscription usage, or charges.

An existing LLM Proxy can add a separate section to `rtk gain`. In this revision,
that summary is loaded independently of the command-output project filter and
aggregates proxy records without that project restriction. Do not attribute its
values to this checkout merely because `--project` was used. See the
[summary loading][rtk-gain-proxy] and [proxy aggregation][rtk-proxy-stats].

When reporting measurements, identify the RTK version, commands, scope/time
window, and whether the evidence measures output bytes, estimated tokens, or
actual provider usage. Keep results from other projects or earlier runs separate.
Do not run expensive or state-changing commands twice solely to compare sizes.
No RTK execution, token saving, or billing reduction was measured while preparing
this guide. The selected source and documentation were inspected; runtime
integration and savings in a real development session remain unverified.

[rtk-tree]: https://github.com/rtk-ai/rtk/tree/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100
[rtk-readme]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/README.md#L45-L76
[rtk-status]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/cmds/git/git_cmd.rs#L2290-L2374
[rtk-diff]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/cmds/git/git_cmd.rs#L261-L387
[rtk-log]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/cmds/git/git_cmd.rs#L1711-L1759
[rtk-gain]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/docs/guide/analytics/gain.md#L8-L125
[rtk-gain-cli]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/lib.rs#L106-L119
[rtk-gain-scope]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/analytics/gain.rs#L1454-L1462
[rtk-gain-proxy]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/analytics/gain.rs#L675-L688
[rtk-proxy-stats]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/llm_proxy.rs#L283-L312
[rtk-read]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/lib.rs#L197-L218
[rtk-bypass]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/lib.rs#L812-L821
[rtk-test]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/core/runner.rs#L767-L799
[rtk-test-fallback]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/core/runner.rs#L1170-L1224
[rtk-codex]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/docs/guide/getting-started/supported-agents.md#L249-L270
[rtk-init]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/hooks/init/codex.rs#L541-L572
[rtk-init-writes]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/hooks/init/codex.rs#L607-L650
[rtk-init-cli]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/lib.rs#L535-L544
[rtk-estimator]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/text/src/lib.rs#L75-L85
[rtk-tracking]: https://github.com/rtk-ai/rtk/blob/4ef8730cf3a0dea11c8d64a9b90bc0616fe52100/src/core/tracking.rs#L2148-L2163
[codex-agents]: https://learn.chatgpt.com/docs/agent-configuration/agents-md
[codex-hooks]: https://learn.chatgpt.com/docs/hooks
