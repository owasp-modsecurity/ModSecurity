# Dependency and build provenance questions

Inventory only the selected build's actual compiler, libraries, submodule commits, tools, workflows, downloaded sources, containers, and artifacts. Separate main-repository source from submodule and system-package contents.

- What immutable revision, package version, source, or digest identifies each input?
- Does the checkout match its gitlinks, and are generated sources traceable to their inputs?
- Which optional dependencies and feature macros are really enabled?
- How are downloads, archive extraction, build scripts, caches, and artifacts selected and verified?
- Can untrusted workflow input or a pull request reach a privileged credential, command, or shared artifact?
- Does the built/loaded library match the headers and revision used by the test?

A pin is not a completed audit; a newer release or advisory is a research lead. Read-only workflow inspection does not authorize launching delivery or changing dependencies.

Entry points: [build/dependencies](../components/build-dependencies.md), [configuration/input injection](config-injection.md), [advisory research](advisory-research.md), and [evidence integrity](evidence-integrity.md).
