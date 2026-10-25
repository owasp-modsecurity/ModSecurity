# Codex and ChatGPT context for ModSecurity v3

Start with [AGENTS.md](../AGENTS.md), then the
[context routing index](context/index.md). The detailed guides, working policies,
profiles, and current state live under `context/`, following the layout requested
from the owner's connector repository.

| Area | Purpose |
| --- | --- |
| [context/index.md](context/index.md) | Reading paths, policy owners, technical guides, and task profiles |
| [context/current-state.md](context/current-state.md) | Actual work, verification, and next steps |
| [context/template-adaptation.md](context/template-adaptation.md) | Exact template revision and core-specific changes |
| [context/manifest.toml](context/manifest.toml) | Machine-readable routing |
| [audit/README.md](audit/README.md) | Source inventory and dated handover evidence |
| [bin/README.md](bin/README.md) | Read-only consistency validators and their limits |
| [inheritance-templates/README.md](inheritance-templates/README.md) | Explicit, optional reuse; no automatic submodule inheritance |

These files are versioned project context. They do not change library behavior,
select a model, configure an account, or make old chats accessible. New sessions
must actually read the relevant files. All maintained additions are English.
