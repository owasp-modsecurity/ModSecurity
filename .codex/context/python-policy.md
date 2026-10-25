# Python environments and dependencies

## Scope

This working policy covers documentation helpers, source generators, optional
Python tools, and explicitly scoped bindings work. It does not make Python
the language of the core engine or assume that a particular virtual environment
already exists.

## Identify the actual Python task

| Task | Entry point and boundary |
| --- | --- |
| Documentation validation | Run `python3 .codex/bin/verify-context.py --strict-source` and `python3 .codex/bin/validate-structure.py` from the repository root, as described in [testing-and-evidence.md](testing-and-evidence.md). These are working-repository helpers. |
| HTML entity generation | `tools/gen-html-entities.py` reads a JSON input and prints generated table content. Its imports are `json`, `sys`, and `collections.defaultdict`. Inspect the input and output destination before regeneration. |
| Python bindings | `bindings/python` is a separate declared submodule. Its own source revision, instructions, and dependency contract must be inspected before changing it. |
| Windows build tooling | The checked-in Windows workflows use Conan. Choose a compatible environment from the actual build route rather than assuming a globally installed package manager. |

Upstream evidence: [entity generator][generator], [bindings declaration][bindings],
[Windows CI tooling][conan-ci], [Windows release tooling][conan-release].
The two cited Windows workflows use different Conan installation commands;
do not infer one universal version contract from only one of them.

## Select an interpreter deliberately

Use an available Python 3 interpreter compatible with the script and its
documented environment. Read imports and version-sensitive syntax when
compatibility is uncertain. A local `.venv` or interpreter selection file is
evidence about that checkout, not proof of a new upstream compatibility promise.

Running a script that only needs the standard library does not require
creating a package manifest or installing a third-party package. If a task
requires external dependencies, use an appropriate isolated environment within
the current filesystem permissions. Record the interpreter, required package
versions, and any dependency changes that affect reproducibility.

Keep system Python and the user site unchanged during ordinary repository
work. For an authorized package installation, address the selected interpreter
explicitly with its `-m pip` interface. Environment controls such as
`PYTHONNOUSERSITE=1`, `PIP_REQUIRE_VIRTUALENV=true`, and
`PIP_DISABLE_PIP_VERSION_CHECK=1` can help keep an isolated installation scoped;
they do not create or activate an environment by themselves.

## Dependency and tool changes

Distinguish packages imported by maintained code or tests from standalone
analysis tools. A new maintained dependency requires an explicit purpose,
compatible version selection, appropriate manifest/lock changes where the
component uses them, and a check of the affected behavior.

An optional external CLI should not silently become a product dependency.
Do not install or upgrade tooling merely because a checker suggests it. When
installation is necessary and within scope, use the trusted package source and
version selection applicable to that task. Record unresolved availability or
compatibility as a limitation instead of substituting a system-wide install.

Do not copy credentials into package URLs, commands, reports, or manifests.
Do not change the bindings submodule or its gitlink just because a documentation
helper needs Python. See [resources-and-storage.md](resources-and-storage.md)
for the actual submodule boundaries.

## Validation

Syntax checks, import checks, helper behavior, formatting, type analysis,
package consistency, and dependency audits establish different facts. Select
the ones relevant to the change and report their actual outcome. No linter,
type checker, package audit service, or installation workflow is made mandatory
merely by listing it here.

For helper changes, exercise the meaningful success or failure behavior that
changed without broadening a Markdown task into unrelated environment work.
Keep generated output and temporary fixtures under known ownership, and clean
them according to [cleanup-and-restoration.md](cleanup-and-restoration.md).

[generator]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/gen-html-entities.py#L20-L86
[bindings]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L7-L9
[conan-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci_new.yml#L197-L206
[conan-release]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/release-windows.yml#L45-L53
