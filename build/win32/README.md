# libModSecurity Windows build information <!-- omit from toc -->

The Windows build of libModSecurity uses Build Tools for Visual Studio 2022 or 2026 (for Visual C++) and Conan 2.x. Conan's generated CMake presets select the Visual Studio generator from the detected compiler profile.

## Contents <!-- omit from toc -->

- [Prerequisites](#prerequisites)
- [Build](#build)
  - [Optional features](#optional-features)
  - [Address Sanitizer](#address-sanitizer)
  - [Docker container](#docker-container)

## Prerequisites

 * Either [Build Tools for Visual Studio 2022](https://aka.ms/vs/17/release/vs_buildtools.exe) or [Build Tools for Visual Studio 2026](https://visualstudio.microsoft.com/downloads/) (under *Tools for Visual Studio*).
    * In either installer, select the *Desktop development with C++* workload and ensure these components are installed:
        * MSVC C++ x64/x86 build tools for the selected Visual Studio version
        * Windows SDK
        * CMake
        * Address Sanitizer
 * [Conan 2 package manager](https://docs.conan.io/2/installation.html) and CMake
    * Windows CI uses the validated Conan 2.33.0 and CMake 4.4.3 versions. With Python available on `PATH`, install the same versions from binary wheels:
      * `python -m pip install --upgrade --only-binary=:all: "conan==2.33.0" "cmake==4.4.3"`
    * Set up the default Conan profile to use the selected MSVC C++ compiler:
      1. Open the *x64 Native Tools Command Prompt* for VS 2022 or VS 2026 from the Start menu. Alternatively, in a regular `cmd.exe` prompt, execute the quoted full path to `VC\Auxiliary\Build\vcvars64.bat` inside your actual Visual Studio installation. The installation directory depends on the version, edition and location you selected; use the toolchain you intend to build with.
      2. In that same initialized prompt, execute: `conan profile detect --force`
 * [Git for Windows 2.53.0](https://github.com/git-for-windows/git/releases/download/v2.53.0.windows.1/Git-2.53.0-64-bit.exe)
    * To clone the libModSecurity repository.
    * NOTE: Make sure to initialize and update submodules (to get `libinjection`, `mbedtls` and regression tests)
      * `git submodule update --init --recursive`
      * `git submodule status`

## Build

Install the prerequisites listed in the previous section, checkout libModSecurity and from the directory where it's located execute:

```
vcbuild.bat [build_configuration] [arch] [USE_ASAN]
```

where `[build_configuration]` can be: `Release` (default), `RelWithDebInfo`, `MinSizeRel` or `Debug`, and `[arch]` can be: `x86_64` (default) or `x86`.

`build/win32/conanfile.py` uses the Conan 2 `CMakeDeps`, `CMakeToolchain` and `cmake_layout` APIs. Generated files are placed in `build/win32/build/generators`. The build script first exports the local YAJL recipe as `yajl/2.1.0@modsecurity/ci`; this recipe fixes the CMake 4 policy and target-path incompatibilities without changing the Conan Center cache recipe.

Built files will be located in the directory: `build\win32\build\[build_configuration]` and include:

 * `libModSecurity.dll`
 * Executable files for test projects
    * `unit_tests.exe`
    * `regression_tests.exe`
    * `benchmark.exe` 
    * `rules_optimization.exe`
 * Executable files for examples
    * `simple_example_using_c.exe`
    * `using_bodies_in_chunks.exe`
    * `reading_logs_via_rule_message.exe`
    * `reading_logs_with_offset.exe`
    * `multithread.exe`
 * Executable files for tools
    * `rules_check.exe`

NOTE: When building a different configuration, it's recommended to reset:

 * the build directory: `build\win32\build`
 * previously built conan packages executing the command:
    * `conan remove * -c`

### Optional features

By default the following all the following features are enabled by including the associated third-party library through a Conan package:

 * libxml2 2.15.2 for XML processing support
 * libcurl 8.19.0 to support http requests from rules
 * libmaxminddb 1.12.2 to support reading MaxMind DB files.
 * LUA 5.5.0 to enable rules to run scripts in this language for extensibility
 * lmdb 0.9.32 in-memory database

Each of these can be turned off by updating the associated `HAVE_xxx` variable (setting it to zero) in the beginning of the libModSecurity section of `CMakeLists.txt`.

### Address Sanitizer

[AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer) (aka ASan) is a memory error detector for C/C++.

To generate a build with *Address Sanitizer*, add the `USE_ASAN` optional third argument to `vcbuild.bat`. For example:
   * `vcbuild.bat Debug x86_64 USE_ASAN`

NOTE: `USE_ASAN` does not work with `Release` & `MinSizeRel` configurations because they do not include debug info (it is only compatible with `Debug` & `RelWithDebInfo` builds).

 * References
   * [AddressSanitizer | Microsoft Learn](https://learn.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170)
   * [AddressSanitizer for Windows: x64 and Debug Build Support - C++ Team Blog (microsoft.com)](https://devblogs.microsoft.com/cppblog/asan-for-windows-x64-and-debug-build-support/)
   * [AddressSanitizer language, build, and debugging reference | Microsoft Learn](https://learn.microsoft.com/en-us/cpp/sanitizers/asan-building?view=msvc-170)

### Docker container

A `Dockerfile` configuration file is provided in the `docker` subdir that creates a Windows container image which installs the [prerequisites](#prerequisites) and builds libModSecurity and other binaries.

NOTE: Windows containers are supported in Docker Desktop for Windows, using the *Switch to Windows containers...* option on the context menu of the system tray icon.

To build the docker image, execute the following command (from the `build\win32\docker` directory):

 * `docker build -t libmodsecurity:latest -m 4GB .`
   * Build type, architecture and build with Address Sanitizer can be configured through build arguments (`BUILD_TYPE`, `ARCH` & `USE_ASAN` respectively). For example, to generate a debug build, add the following argument:
     * `--build-arg BUILD_TYPE=Debug`

Once the image is generated, the library and associated binaries (tests & examples) are located in the `C:\src\ModSecurity\build\win32\build\[build_type]` directory.

To extract the library (`libModSecurity.dll`) from the image, you can execute the following commands:

 * `docker container create --name [container_name] libmodsecurity`
 * `docker cp [container_name]:C:\src\ModSecurity\build\win32\build\[build_type]\libModSecurity.dll .`
   * NOTE: If you leave out the `libModSecurity.dll` filename out, you can copy all the built binaries (including examples & tests).

Additionally, the image can be used interactively for additional development work by executing:

 * `docker run -it libmodsecurity`
