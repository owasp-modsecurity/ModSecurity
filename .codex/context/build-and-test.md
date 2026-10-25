# Building, testing, and investigating failures

## Scope and how to read this guide

This guide describes the inspected upstream source revision
`effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729` of `owasp-modsecurity/ModSecurity`,
branch `v3/master`. Source links point to this immutable commit.
The commands below are derived from build files, test programs, and CI
configuration. Including a command here does **not** mean it was run in this
session or already works on your machine. The project's current state records
which checks were actually performed.

## 1. What the build produces

On Unix, the project uses Autotools to build `libmodsecurity.la` and the
associated shared library. The top-level build visits `others`, `src`, `doc`,
and `tools`, plus `examples` and `test` when enabled by the configuration.
This does not automatically build a web server connector. The examples and
rule checker are separate programs that use the library.
Sources: [top-level Makefile][root-make], [library target and public headers][src-make],
[connector boundaries][readme-connectors].

| Component | Actual requirement or purpose | Source |
| --- | --- | --- |
| C and C++ compilers | `AC_PROG_CC` and `AC_PROG_CXX`; `AX_CXX_COMPILE_STDCXX(17, noext, mandatory)` makes C++17 mandatory. | [configure.ac, 49–63][compiler] |
| Autotools | `build.sh` invokes `libtoolize`, or `glibtoolize` on Darwin, followed by `autoreconf`, `autoheader`, `automake`, and `autoconf`. | [build.sh][bootstrap] |
| `make`, archiver, `pkg-config` | Detected during configuration; `pkg-config` helps locate external libraries. | [configure.ac, 49–63][compiler], [library detection][find-lib] |
| Flex and Bison | Required when regenerating the parser with `--enable-parser-generation=yes`. This option is disabled by default; pregenerated parser files are included in the repository. | [configure.ac, 284–310][parser-option], [src/Makefile.am, 17–28][src-make] |
| Git submodules | `configure.ac` checks for specific files from libinjection and the nested mbedTLS source tree. Missing files stop configuration. | [configure.ac, 65–107][submodule-checks] |

`build.sh` changes generated files: among other operations, it removes
Autotools intermediates and regenerates `src/headers.mk`. Inspect existing
changes with `git status --short` before running it. This is a working rule
introduced by this guide; the file operations themselves are defined in
[build.sh][bootstrap].

## 2. Submodules and external libraries

The main repository declares four submodules:

| Path | Role supported by the source tree |
| --- | --- |
| `others/libinjection` | Bundled libinjection sources; configuration requires `src/libinjection_html5.c`. |
| `others/mbedtls` | Bundled Mbed TLS sources; configuration specifically requires `tf-psa-crypto/utilities/base64.c`, which is inside a nested source tree. |
| `test/test-cases/secrules-language-tests` | Additional shared SecLang test cases; the default unit-test paths and test suite refer to this location. |
| `bindings/python` | Separate Python bindings submodule; it does not appear in the top-level library build's `SUBDIRS` list. |

Sources: [.gitmodules][submodules], [configuration checks][submodule-checks],
[default unit-test path][unit-default], [test list][test-list], [build subdirectories][root-make].

From the repository root:

```sh
git submodule update --init --recursive
git submodule status --recursive
```

The upstream README requires recursive initialization. A leading `-` in
the status output identifies an uninitialized submodule. Using recursive
status output as well is a workflow choice made here to expose nested
dependencies.
Source: [README: preparing a Git checkout][readme-build].

### Which library is needed for which purpose?

The table distinguishes actual build logic from the intended test
environment. Here, “optional” means that Unix configuration does not
always require the library; it does not promise unchanged functionality
when that library is absent.

| Library | Status in the inspected revision | Evidence |
| --- | --- | --- |
| PCRE2 or explicitly selected legacy PCRE | The default path searches for PCRE2; `--with-pcre` selects the other path. Without `WITH_PCRE`, `regex.h` directly includes `pcre2.h`. A usable build therefore needs the selected regex dependency. | [configure.ac, 147–157][regex-selection], [regex.h, 15–20][regex-header] |
| YAJL | The README calls YAJL mandatory. However, the Unix macros allow automatic detection to continue when it is absent; `TEST_UTILITIES` is enabled only when `YAJL_FOUND=1`. Explicitly request it for the test build described here. | [README: Dependencies][readme-deps], [YAJL macro][yajl], [generic detection][find-lib], [test activation][test-enable] |
| libcurl | Configuration defines `MSC_WITH_CURL` when curl is detected; upstream associates curl with `SecRemoteRules`. | [configure.ac, 130–138][curl-config], [README: operator dependencies][readme-deps] |
| libxml2 | Optional build component for XML processing; the XML implementation and transaction path are conditional on `WITH_LIBXML2`. | [configure.ac, 141–144][xml-config], [XML implementation][xml-code] |
| LMDB | Controls the collection backend: LMDB with `WITH_LMDB`, otherwise `InMemoryPerProcess`. | [ModSecurity constructor][collections] |
| Lua, SSDEEP, GeoIP/MaxMind | Detected as separate capabilities. Tests may be skipped depending on compiled capabilities; runners check `resource` against their own resource lists. | [configure.ac, 115–128][optional-config], [regression-test resources][regression-resources] |

**Practical consequence of the YAJL discrepancy:** Successful configuration
alone does not establish that the unit-test and regression-test programs
were built. For a test build, check for `Test Utilities ... enabled` in the
summary and inspect the actual test counts. When debug logging is disabled,
`regression_tests` also has a path that prints a message and exits with `0`
without running the regressions. Sources:
[test activation][test-enable], [configuration summary][configure-summary],
[NO_LOGS path][regression-main], [suite skip handling][test-driver].

## 3. A Unix development build

Package names depend on the operating system. This document does not
provide a universal package installation command. The inspected GitHub
workflows contain concrete Ubuntu and macOS examples. They build with
GCC or Clang, enable assertions, and run `make check`. The workflow files
establish which checks are configured, not whether they succeed on an
arbitrary machine or whether a CI run was performed here.
Sources: [Linux CI][linux-ci], [additional Linux/macOS CI][new-ci].

After installing suitable build tools and development libraries, run these
commands from the checkout root:

```sh
git status --short
git submodule update --init --recursive
git submodule status --recursive
./build.sh
./configure --with-yajl=yes --with-pcre2=yes --enable-assertions=yes
make -j2
make check
```

The `build.sh`, `configure`, `make`, and `make check` sequence comes from
the upstream README and CI. Explicit YAJL/PCRE2 flags and `-j2` are the
development variant chosen here: missing libraries should be detected
during configuration, and the number of parallel compiler processes
remains modest. `MSC_CHECK_LIB` treats `yes` as a mandatory requirement.
Sources: [README build sequence][readme-build],
[Linux CI][linux-ci], [library options][find-lib], [PCRE2 macro][pcre2].

For most source work, an installation step is not initially necessary:
the test programs link against the build under `src/.libs`. If a local
installation is needed, set the installation prefix to a writable path
during configuration, for example:

```sh
./configure --prefix="$HOME/.local/modsecurity" \
  --with-yajl=yes --with-pcre2=yes --enable-assertions=yes
make -j2
make check
make install
```

This local prefix is a choice made by this guide. The default prefix
in the source is `/usr/local/modsecurity`; `make install` installs build
products and is not a verification step.
Sources: [default prefix][prefix], [test linker paths][test-make],
[installable library and headers][src-make], [upstream installation sequence][readme-build].

### Debug builds and parser work

For the debug variant derived here, set both C and C++ flags during
configuration:

```sh
CFLAGS='-g -O0' CXXFLAGS='-g -O0' ./configure \
  --with-yajl=yes --with-pcre2=yes --enable-assertions=yes
make -j2
```

The assertion option is implemented: it sets `-UNDEBUG` instead of
`-DNDEBUG`. The additional C++ debug flags are a workflow choice made
by this guide. Inspect the actual compiler command to determine whether
other flags are added in a particular build.
Source: [assertions and global flags][assertions].

For grammar or scanner changes, edit the inputs
`src/parser/seclang-parser.yy` and `src/parser/seclang-scanner.ll`.
Generation is enabled with `--enable-parser-generation=yes`. The parser
Makefile also includes a postprocessing step for the generated header;
an arbitrary standalone Bison command should not be assumed to replace
the project build.
Sources: [parser option and tool detection][parser-option],
[parser Makefile][parser-make].

## 4. Selecting focused tests

After a successful build with `TEST_UTILITIES` enabled, as in Section 3
with YAJL explicitly requested, `test` contains the `unit_tests` and
`regression_tests` programs. Their shared argument parser accepts a
file path, among other inputs; regression tests also support the
`file.json:N` selection form. Regressions are sorted by their loaded
keys, and the runner counts `N` from `1`.
Sources: [program names][test-make],
[argument parser][test-cli], [regression selection][regression-main].

The upstream README uses `tests/` once in its general compilation
section. The actual build definitions and files use `test/`, without
a final `s`; commands must use that real path.
Sources: [inconsistent README passage][readme-test-path],
[test integration][test-integration].

To run the full configured test suite from the repository root:

```sh
make check
```

For the direct invocations below, first change into `test`. The working
directory matters because fixtures and default paths are relative to
it; the shell test driver makes this same directory change.
Source: [test/test-suite.sh][test-driver].

```sh
cd test
./unit_tests test-cases/unit/transformation-hash-base64.json
./regression_tests test-cases/regression/secruleengine.json
./regression_tests test-cases/regression/secruleengine.json:1
```

The first example selects an existing local transformation test file.
The second checks the existing rule-engine cases; the third selects
one entry from that file. These are documented invocations, not a test
execution record.
Sources: [existing unit-test file][hash-test],
[existing engine regressions][engine-test], [argument handling][test-cli].

Without a file argument, `unit_tests` defaults to operator cases under
`test-cases/secrules-language-tests/operators` and adds `transformations`
when using that default target. Additional local files under
`test-cases/unit` are therefore not automatically covered by this
direct default invocation. For the full suite listed by the project,
`make check` and `test/test-suite.in` define the scope.
Sources: [default unit-test path][unit-default], [additional load path][unit-main],
[test list][test-list], [Makefile test integration][test-integration].

### What a test result establishes

- Record the exit code, number of executed cases, skipped cases, and
  disabled cases together. The programs exclude skipped cases from
  failure counts; exit code `0` therefore does not establish that all
  possible capabilities were exercised. [Unit result calculation][unit-main],
  [regression result calculation][regression-main].
- When adding a regression test, also check whether it is listed in
  `test/test-suite.in`. The suite is included through that file.
  [Makefile integration][test-integration], [test list][test-list].
- `format` is a writing mode of the regression-test program: it opens
  JSON files for output. It is not an alternative read-only test command.
  [Formatting branch][regression-main].
- An existing JSON expectation defines a test contract; only a
  documented execution establishes whether the current build meets it.
  Keeping that distinction is a working rule for this repository.

## 5. Loading rule files and running static checks

The Unix build defines `tools/rules-check/modsec-rules-check`.
It treats an existing filesystem object as an input file
(`RulesSet::loadFromUri`); otherwise, it treats the argument as SecLang
text (`RulesSet::load`). A negative load result is reported as an error.
Sources: [program target][rules-check-make], [implementation][rules-check].

Example from the repository root after building:

```sh
./tools/rules-check/modsec-rules-check 'SecRuleEngine DetectionOnly'
```

The checker loads rules and reports loading errors; its `main` does
not simulate an HTTP transaction. Successful loading therefore does
not replace a regression with appropriate request and response data.
Source: [the checker's complete control flow][rules-check].

`make check-static` invokes `cppcheck`; its options and suppressions
are in the top-level Makefile. `make check-style` invokes
`check-coding-style`, uses `cpplint.py`, and writes `coding-style.txt`.
The relevant tools must be installed for these additional checks.
Source: [static analysis and style checks][static-checks].

## 6. Windows uses a separate build path

The inspected Windows path uses Visual Studio 2022 Build Tools, Conan,
and CMake. `vcbuild.bat` runs `conan install` with `compiler.cppstd=17`,
generates a project with `Visual Studio 17 2022`, and builds the chosen
configuration. `build/win32/CMakeLists.txt` requires at least CMake 3.24
and C++17.
Sources: [Windows README][windows-readme], [vcbuild.bat][vcbuild],
[Windows CMake][windows-cmake].

Source-backed example invocations from a suitably prepared Windows
environment, at the repository root:

```bat
vcbuild.bat Release x86_64 NO_ASAN
vcbuild.bat Debug x86_64 USE_ASAN
```

According to the Windows guide, build products are placed under
`build\win32\build\<configuration>`. Additional CMake options are
forwarded starting with the fourth argument; CI uses
`-DWITH_LMDB=ON`, for example.
Sources: [vcbuild.bat][vcbuild],
[Windows README][windows-readme], [Windows CI][windows-ci].

**Check the source revision rather than generalizing the guide:** The
Windows README includes LMDB in a list of features enabled by default.
The actual CMake option `WITH_LMDB` is `OFF`, while Lua, libxml2,
MaxMind, and curl are `ON`. For this revision, the CMake options define
those defaults. Conan dependencies have fixed versions in the repository;
their resolvability was not checked by writing this documentation.
Sources: [CMake options][windows-cmake], [conflicting README passage][windows-readme],
[conanfile.txt][conan].

Before `ctest -C Release --output-on-failure`, Windows CI prepares DLLs,
test programs, and fixtures, among other items, and disables several
named cases. Copying the CTest command alone does not establish
equivalence with the full Linux check.
Source: [Windows test preparation][windows-ci].

## 7. Recording failures for the next session

To make subsequent work reproducible, save these details:

1. Commit, branch, operating system, and compiler.
2. Output of `git submodule status --recursive`.
3. Exact `configure` or `vcbuild.bat` invocation and detected capabilities.
4. First failing command, exit code, and relevant error message.
5. Specific test file, selected cases, case counts, and skipped/disabled counts.
6. Confirmed cause, or an explicitly unresolved hypothesis.

This list defines the workflow introduced here. It does not assert that
any builds have already run. Its commands and technical details can be
checked against the build and test sources linked above.

## Sources
[root-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L1-L17
[src-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/Makefile.am#L1-L67
[readme-connectors]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L35-L49
[compiler]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L49-L63
[bootstrap]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build.sh#L1-L33
[find-lib]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/msc_find_lib.m4#L21-L99
[parser-option]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L284-L310
[submodule-checks]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L65-L107
[submodules]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.gitmodules#L1-L12
[unit-default]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/unit/unit.cc#L52-L53
[test-list]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-suite.in
[readme-build]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L58-L95
[regex-selection]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L147-L157
[regex-header]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/utils/regex.h#L15-L20
[readme-deps]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L101-L134
[yajl]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/yajl.m4#L10-L11
[test-enable]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L313-L335
[curl-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L130-L138
[xml-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L141-L144
[xml-code]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/request_body_processor/xml.cc
[collections]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/modsecurity.cc#L62-L78
[optional-config]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L115-L128
[regression-resources]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L160-L185
[configure-summary]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L479-L498
[regression-main]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/regression/regression.cc#L395-L561
[test-driver]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-suite.sh#L1-L40
[linux-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci.yml#L8-L106
[new-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci_new.yml#L75-L171
[pcre2]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/pcre2.m4#L11-L13
[prefix]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L40-L42
[test-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/Makefile.am#L29-L145
[assertions]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/configure.ac#L350-L358
[parser-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/src/parser/Makefile.am#L1-L43
[test-cli]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/common/modsecurity_test.cc#L95-L173
[hash-test]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/unit/transformation-hash-base64.json
[engine-test]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/test-cases/regression/secruleengine.json#L1-L123
[unit-main]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/test/unit/unit.cc#L307-L379
[test-integration]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L88-L96
[readme-test-path]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/README.md#L53-L54
[rules-check-make]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/Makefile.am#L3-L9
[rules-check]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/tools/rules-check/rules-check.cc#L32-L93
[static-checks]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/Makefile.am#L58-L85
[windows-readme]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/README.md#L3-L83
[vcbuild]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/vcbuild.bat#L1-L28
[windows-cmake]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/CMakeLists.txt#L1-L149
[windows-ci]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/.github/workflows/ci_new.yml#L173-L234
[conan]: https://github.com/owasp-modsecurity/ModSecurity/blob/effb6c0fa031a7bddb3253e0b7ab9ca9a18b5729/build/win32/conanfile.txt#L1-L14
