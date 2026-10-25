# Optional instruction reuse

This directory mirrors the template repository's explicit inheritance area.
There are **no active child repositories inheriting these policies** in this
setup. Upstream submodule contents have not been modified.

[../inheritance-manifest.toml](../inheritance-manifest.toml) is the digest catalog
for the local primary policies. The catalog supports reviewable reuse; it does
not cause Codex or ChatGPT to load instructions across repositories.

[child-inheritance-manifest.toml.example](child-inheritance-manifest.toml.example)
is a disabled example for a future, explicitly scoped child repository. Before
using it, read that repository's own instructions, verify its actual relative
paths, select only applicable policies, and replace each digest with the reviewed
catalog value. Record conflicts and the selected parent commit. Do not place this
example in an upstream dependency as part of routine setup.

The current validator checks only this repository's active local catalog. It does
not validate or activate hypothetical child manifests. Supporting a real child
repository would require a separate reviewed schema and validator change.
