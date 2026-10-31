# Working on ModSecurity v3

## Scope and branches

This repository contains libmodsecurity, the C++17 core library for ModSecurity
v3, with public C and C++ interfaces. Web-server connectors and the OWASP Core
Rule Set are separate projects; a core-library test does not test a connector.

Target `v3/master` for changes to this library. Keep implementation guidance,
build instructions, and tests focused on ModSecurity v3.

ModSecurity v2, the Apache module, lives on the `v2/master` branch of this
repository. It is a separate codebase; the instructions in this file do not
apply to it.

Use the [README](README.md) for dependency and contribution details, the
[Windows guide](build/win32/README.md) for Windows builds, and the
[compilation recipes](https://github.com/owasp-modsecurity/ModSecurity/wiki/Compilation-recipes)
for platform-specific setup.

## Source map

| Area | Location |
| --- | --- |
| Public C/C++ interfaces | [headers/modsecurity/](headers/modsecurity/) |
| Transaction processing and phases | [src/transaction.cc](src/transaction.cc) |
| Rule loading and evaluation | [src/parser/driver.cc](src/parser/driver.cc), [src/rules_set.cc](src/rules_set.cc), [src/rule_with_operator.cc](src/rule_with_operator.cc) |
| Operators | [src/operators/](src/operators/) |
| Actions, including `ctl` and disruptive actions | [src/actions/](src/actions/) |
| Transformations | [src/actions/transformations/](src/actions/transformations/) |
| Variables | [src/variables/](src/variables/) |
| SecLang grammar and scanner | [src/parser/](src/parser/) |
| Unit and regression test cases | [test/test-cases/](test/test-cases/) |
| Test executables and harness | [test/unit/](test/unit/), [test/regression/](test/regression/), [test/test-suite.sh](test/test-suite.sh) |
| Existing build/test CI configurations | [.github/workflows/](.github/workflows/) |

The rule hierarchy starts with [Rule](headers/modsecurity/rule.h), followed by
[RuleWithActions](headers/modsecurity/rule_with_actions.h). Both
[RuleWithOperator](headers/modsecurity/rule_with_operator.h) and
[RuleUnconditional](headers/modsecurity/rule_unconditional.h) derive from
`RuleWithActions`.

Operators, actions, transformations, and variables are connected to the rule
language through [seclang-parser.yy](src/parser/seclang-parser.yy) and scanner
tokens in [seclang-scanner.ll](src/parser/seclang-scanner.ll). For a new feature,
follow an existing analogous implementation through its grammar, factories,
source lists, and tests; adding a class alone may not expose it in SecRules.

For operators and transformations, also check
[`Operator::instantiate`](src/operators/operator.cc) and
[`Transformation::instantiate`](src/actions/transformations/transformation.cc).
The [unit tests](test/unit/unit.cc) construct these objects through those
factories, so grammar changes alone do not make a new implementation available
to the unit tests.

## Prepare and build

From a Git checkout, initialize the pinned submodules before building:

```sh
git submodule update --init --recursive
git submodule status --recursive
```

Recursive initialization is important for nested Mbed TLS dependencies. See
[.gitmodules](.gitmodules) and the README's Git-submodules section. A leading
`-` in submodule status means the submodule is not initialized.

With the development dependencies from the README installed, run these commands
from the repository root on Unix-like systems:

```sh
./build.sh && ./configure && make && make check
```

`build.sh` generates the Autotools build files and the internal header list.
After changing [configure.ac](configure.ac), `Makefile.am` files, or adding
headers, rerun `build.sh`, configure, and build. Keep configuration options
appropriate to the feature being tested; optional libraries determine which
functionality is available.

## Tests and build prerequisites

YAJL is needed for the JSON-based test utilities. Inspect the YAJL and test-utility
configuration summary: [configure.ac](configure.ac) disables the test utilities
when YAJL is not found. Keep debug logs enabled for regression tests:
with YAJL available, `--disable-debug-logs` makes the summary report the test
utilities as `partially` enabled, and the regression utility skips testing.
Confirm that the summary reports `Test Utilities ... enabled`. A successful
command with zero tests or only skipped tests does not establish that the
changed behavior was tested.

Run `make check` for the normal Automake test suite. Automake reads the `.trs`
results and fails the target when tests report failures. The exit status of
`test/test-suite.sh` or `test/custom-test-driver` run on its own is not equivalent
to the result of `make check`. Inspect failure details in the generated `.log`
and `.trs` files and in `test-suite.log` when diagnosing a failed run.

For new or changed behavior:

- New operators need unit tests under [test/test-cases/unit/](test/test-cases/unit/);
  other behavior changes need regression tests under
  [test/test-cases/regression/](test/test-cases/regression/).
- When adding a new JSON test file, register its repository-relative path with a
  `TESTS+=test/test-cases/.../file.json` entry in
  [test/test-suite.in](test/test-suite.in). An unregistered file is not selected
  by the normal test harness.
- After changing that list, rerun `./build.sh`, `./configure` with the intended
  options, `make`, and `make check` so the generated test list is updated.
- The shared [SecRules test suite](https://github.com/owasp-modsecurity/secrules-language-tests)
  is a submodule. Its `operators/` and `transformations/` directories contain
  shared unit tests. Changes inside it belong to that separate project.

## Parser changes

Edit the source grammar [seclang-parser.yy](src/parser/seclang-parser.yy) and
scanner [seclang-scanner.ll](src/parser/seclang-scanner.ll). Their generated
`.cc` and `.hh` files are also committed. Parser generation is disabled by
default, so a normal build alone does not regenerate them.

The grammar declares Bison 3.0.2 as its minimum version. The checked-in
[parser header](src/parser/seclang-parser.hh) was generated with Bison 3.8.2;
prefer that version to limit unrelated generator changes. The CI workflows
install Bison without an exact version pin. Check `bison --version` before
regenerating.

On macOS, select [Homebrew Bison](https://formulae.brew.sh/formula/bison) ahead of
the system tool, then check the selected version:

```sh
brew install bison
export PATH="$(brew --prefix bison)/bin:$PATH"
bison --version
```

With Flex and the selected Bison installed, enable generation through the build:

```sh
./configure --enable-parser-generation
make
make check
```

Use the repository's build rules, including their generated-header adjustment,
rather than hand-editing generated output. Include the corresponding generated
parser/scanner files when committing grammar or scanner changes. Review the
generated diff for unrelated generator-version churn.

The header-adjustment rule in [src/parser/Makefile.am](src/parser/Makefile.am)
leaves an untracked `src/parser/test.cc` stamp file. Do not commit it. Stage the
intended grammar/scanner sources and generated files by explicit path; avoid
`git add -A` after generation. Review `git diff --cached --check` and
`git diff --cached` before committing.

Avoid `make clean` in a parser-enabled working tree that you intend to commit
from: the parser's `CLEANFILES` list removes the committed `seclang-parser.cc`,
`seclang-parser.hh`, `seclang-scanner.cc`, `location.hh`, `position.hh`, and
`stack.hh` files as well as the stamp file. Check the diff for unintended
deletions.

## Static checks and contributing

After configuring the build, the existing additional checks are:

| Command | Tool and result |
| --- | --- |
| `make check-static` | Runs cppcheck; requires cppcheck to be installed. |
| `make check-style` | Requires an executable named exactly `cpplint.py` on `PATH` and writes `coding-style.txt`; inspect that file for findings or missing-tool errors, since the target's exit status alone is not a style gate. |

See [Makefile.am](Makefile.am) for these targets. Follow the
[Chromium/Blink C++ style guide](https://www.chromium.org/blink/coding-style)
named in the README, and use [.editorconfig](.editorconfig) plus nearby code for
repository formatting settings.

Keep each PR to one logical change and each commit to one fix or feature.
Include tests with behavior changes and explain the change in the commit
message. Follow the README's contribution and testing guidance; avoid unrelated
formatting or dependency/submodule updates.
