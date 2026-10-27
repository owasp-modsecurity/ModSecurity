# Optional token reduction with RTK

[RTK](https://github.com/rtk-ai/rtk) is an optional tool that can reduce token
usage in AI-assisted development. It shortens the output of supported terminal
commands before a coding assistant reads it, reducing the command output
included in the model's context.

RTK supports assistants from multiple providers, including OpenAI's Codex,
Anthropic's Claude Code, Google's Gemini CLI, and GitHub Copilot. Available
integrations and capabilities depend on the assistant and RTK version; see the
[supported assistants](https://github.com/rtk-ai/rtk/blob/master/docs/guide/getting-started/supported-agents.md).

Actual savings depend on the workflow. Reduced command output does not guarantee
an equal reduction in total token usage or cost. See
[how RTK savings work](https://github.com/rtk-ai/rtk/blob/master/docs/guide/resources/savings-explained.md).
