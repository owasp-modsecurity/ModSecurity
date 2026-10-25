# Tooling and language

## Scope

This is a working policy for this core repository. It selects appropriate tools
and checks; [command-execution-policy.md](command-execution-policy.md) governs
their execution. The cited source describes the pinned upstream revision, while
the instructions below define how this working repository should be maintained.

## Use the actual core build

Read [build-and-test.md](build-and-test.md) before choosing build or test
commands. The Unix build uses Autotools and includes `others`, `src`, `doc`,
`tools`, and conditionally `examples` and `test`. Windows has a separate
`vcbuild.bat`/CMake route. Select the route matching the task and environment.
Sources: [build subdirectories][subdirs], [bootstrap script][bootstrap],
[Windows build script][windows].

Prefer existing build definitions and scripts over a new parallel build system.
Inspect tool availability and version when the task depends on them. A command
copied from CI describes that workflow's environment; it does not establish
that the same packages, privileges, or binaries exist locally.

## C and C++

`configure.ac` probes C and C++ compilers and explicitly requires C++17 through
`AX_CXX_COMPILE_STDCXX(17, noext, mandatory)`. The Windows CMake configuration
also requires C++17. Use the real component flags for compilation and analysis;
do not import a different repository's C17 requirement or silently raise the
language baseline. Sources: [compiler checks][compilers], [Windows CMake][cmake].

Read [cpp-guide.md](cpp-guide.md) for public interfaces, ownership, and lifetime.
Inspect declarations, implementations, and relevant callers together. Diagnostic
builds, sanitizers, and a newer compiler mode are separate configurations; report
their actual flags and results without presenting them as the normal build.

Derive a compilation database from a real applicable build when one is needed.
Record its source revision and configuration, and keep transient analysis
artifacts out of commits. An editor or analyzer finding is a lead to investigate;
it is not a successful compilation or runtime test.

## Parser inputs and source style

Parser generation is a build option disabled by default. When enabled, the
configuration checks Flex and Bison; the parser Makefile uses the `.ll`/`.yy`
inputs and performs additional header processing. Coordinate changes to these
inputs with generated files rather than treating generated output as the sole
language definition. Sources: [generation option][parser-option],
[parser Makefile][parser-make].

Follow the existing [EditorConfig][editorconfig] and surrounding file style.
Review formatter output before applying it and keep unrelated reformatting out
of a focused change. Read [configuration-and-rules.md](configuration-and-rules.md)
when a change affects SecLang rather than only the C++ implementation.

## Scripts, bindings, and optional helpers

Python work follows [python-policy.md](python-policy.md). The Python bindings
are a separate submodule, and the top-level build's subdirectory list does not
include that path. Inspect the bindings' own revision and instructions before
working there. Sources: [declared submodules][submodules], [subdirectories][subdirs].

Use available search tools such as `rg` for navigation. Apply shell linting,
formatters, or additional analyzers only when relevant to the changed files.
The core Makefile defines `check-static` through cppcheck and `check-style`
through `check-coding-style`; these require their actual tools and are not
substitutes for behavior tests. Source: [analysis and style targets][analysis].

Do not install or update compilers, interpreters, analyzers, or assistant tools
as incidental cleanup. A necessary tool or dependency change belongs in the
task scope, with its compatibility, provenance, and validation recorded.

[subdirs]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L2-L17
[bootstrap]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build.sh#L1-L33
[windows]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/vcbuild.bat
[compilers]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[cmake]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt
[parser-option]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L284-L310
[parser-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L1-L43
[editorconfig]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.editorconfig#L1-L25
[submodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
[analysis]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L58-L85
