# Build, CI, and supply-chain assessment

## Trigger and scope

Use when the user asks to assess build reproducibility, optional-feature identity, dependencies, CI, or artifact provenance. Scope the actual platforms, workflows, toolchains, and source revisions before choosing commands.

## Read

[Build and tests](../build-and-test.md), [build/dependency component](../security/components/build-dependencies.md), [supply chain](../security/checks/supply-chain.md), [configuration/input injection](../security/checks/config-injection.md), [disclosure](../security/checks/logging-disclosure.md), and [evidence integrity](../security/checks/evidence-integrity.md). Use [advisory research](../security/checks/advisory-research.md) only when selected.

## Questions

- Which source, gitlinks, generated files, headers, library, and test binaries belong to the result?
- Which dependencies and feature switches were actually detected, and which targets consequently exist?
- Do workflow events, permissions, inputs, caches, artifacts, actions/images, and downloads preserve the intended trust boundary?
- Does an executed local check correspond to the claimed remote job and revision, or only a subset?
- Are missing tests, skipped suites, caught failures, and stale artifacts visible?

Inspect actual repository definitions; do not invent a matrix, package pin, Python requirement, scanner, or Quality Gate. Read-only remote CI inspection does not authorize triggering delivery, changing credentials, or upgrading dependencies.

## Output

Report the source/tool/dependency inventory, selected targets, actual command results, optional-feature identity, relevant workflow observations, and unresolved gaps. Keep Unix, Windows, normal, diagnostic, and remote-job results separate.
