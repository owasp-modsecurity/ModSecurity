# Local YAJL recipe for Windows builds

`vcbuild.bat` exports this recipe as `yajl/2.1.0@modsecurity/ci`, and the Windows consumer selects that reference. The YAJL library version remains 2.1.0; this isolates the compatibility patches from the ConanCenter recipe in the user's cache.

## Why this copy is needed

YAJL's build reads the `LOCATION` property of `json_reformat` and `json_verify` during configuration. ConanCenter's existing patch also selects the old `CMP0026` policy behavior. CMake 4 removes that behavior, while the Visual Studio 18 2026 generator requires CMake 4.2 or newer.

The additional patch removes the obsolete policy selection and uses quoted `$<TARGET_FILE:...>` expressions with `VERBATIM`. CMake resolves the executable path for the active build configuration when the post-build copy command runs.

## Patch provenance and upstream work

- `2.1.0-0001-fix-cmake.patch` comes from the [ConanCenter recipe at the pinned base revision](https://github.com/conan-io/conan-center-index/tree/4c4ac13f1ee7ce578941859ae3d2feff7189796d/recipes/yajl/all).
- `2.1.0-0002-cmake4-target-file.patch` adds CMake 4 compatibility on top of that patch. It uses the same `LOCATION` to `TARGET_FILE` approach as [YAJL PR #256](https://github.com/lloyd/yajl/pull/256); [YAJL issue #257](https://github.com/lloyd/yajl/issues/257) reports the corresponding failures.
- [ConanCenter CMake 4 tracking](https://github.com/conan-io/conan-center-index/issues/26878).

## When to remove this copy

Once ConanCenter provides a YAJL recipe with the required CMake 4 fixes, verify that it builds with the supported Windows toolchains. Then switch the consumer back to the ConanCenter reference, remove the local export from `vcbuild.bat`, remove this recipe and its patches, and update the Windows guide together. Validate both Windows CI runners and the x86/x64 Release/Debug release-packaging matrix before dropping the copy.

## Technical references

- [CMake CMP0026: removal of the old LOCATION behavior in CMake 4](https://cmake.org/cmake/help/latest/policy/CMP0026.html).
- [CMake generator expressions: TARGET_FILE, quoting and configuration-dependent evaluation](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_FILE).
- [Visual Studio 18 2026 generator, added in CMake 4.2](https://cmake.org/cmake/help/latest/generator/Visual%20Studio%2018%202026.html).
- [Conan CMakeToolchain and generated presets](https://docs.conan.io/2/reference/tools/cmake/cmaketoolchain.html).
