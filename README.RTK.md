# Optional RTK usage

[RTK](https://github.com/rtk-ai/rtk) can shorten supported terminal-command
output before a coding assistant reads it. This can reduce the command output
included in its context. RTK is optional: ModSecurity builds, tests, and
contributions do not require it.

## Installation and availability

If you choose to install RTK, follow its maintained
[installation guide](https://github.com/rtk-ai/rtk/blob/master/docs/guide/getting-started/installation.md)
and review the available editions. Install the `rtk-ai/rtk` project; an
unrelated package also uses the name `rtk`.

Check the installed executable and supported commands before using it:

```sh
rtk --version
rtk --help
```

Continue with native tools when RTK is absent or unsuitable. Installation and
initialization are separate choices, not repository setup requirements.

## Manual command summaries

Run these examples from the repository root for routine orientation:

```sh
rtk git status
rtk git log -5
rtk git diff -- src/transaction.cc
```

Filtered diffs can omit context and content. Before approving or committing a
change, inspect the complete native diff:

```sh
git --no-pager diff
git --no-pager diff --cached
```

If an existing integration rewrites native commands, RTK also provides an
explicit unfiltered alternative, for example:

```sh
rtk bypass git --no-pager diff -- src/transaction.cc
```

Use complete source, raw diagnostics, and actual exit statuses for exact
verification. Do not feed filtered summaries to scripts or generate patches
from them. Keep the native build and test commands in [AGENTS.md](AGENTS.md)
and [README.md](README.md); do not replace `make check` with an RTK wrapper.
RTK does not add test coverage or establish that any tests ran.

## Measuring output reduction

For tracked command-output statistics scoped to the current directory, use:

```sh
rtk gain --project --text
rtk gain --project --history
```

RTK estimates tokens from output bytes; these are not provider billing counts.
Reduced command output does not guarantee the same reduction in total tokens,
subscription usage, or cost. Project-scoped history can include earlier runs.
A separate proxy section is not restricted by the command-output project
filter; do not attribute those figures to this checkout.

When reporting results, identify the RTK version, commands, scope, and time
window. No runtime integration or measured savings are established by this
README. See RTK's [analytics guide](https://github.com/rtk-ai/rtk/blob/master/docs/guide/analytics/gain.md)
and [savings explanation](https://github.com/rtk-ai/rtk/blob/master/docs/guide/resources/savings-explained.md).

## Optional agent integration

Manual commands above do not require hooks, a proxy, or changes to personal
agent settings. Automatic rewriting is a separate setup choice described in
RTK's [supported-agent guide](https://github.com/rtk-ai/rtk/blob/master/docs/guide/getting-started/supported-agents.md).
Review generated files before adopting it; RTK initialization can change
repository instructions and create tool-specific configuration.

For Codex, follow OpenAI's official [AGENTS.md documentation](https://learn.chatgpt.com/docs/agent-configuration/agents-md)
and [hooks documentation](https://learn.chatgpt.com/docs/hooks). Check support in
the actual client rather than assuming an automatic integration is active.
This README configures no hooks, proxy, or Codex settings.
