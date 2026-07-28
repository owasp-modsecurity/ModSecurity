@rem For Windows build information, see build\win32\README.md

@echo off
pushd %CD%

if not "%1"=="" (set build_type=%1) else (set build_type=Release)
echo Build type: %build_type%

if not "%2"=="" (set arch=%2) else (set arch=x86_64)
echo Arch: %arch%

if "%3"=="USE_ASAN" (
    echo Address Sanitizer: Enabled
    set CI_ASAN=-c tools.build:cxxflags="[""/fsanitize=address""]"
    set ASAN_FLAG=ON
) else (
    echo Address Sanitizer: Disabled
    set CI_ASAN=
    set ASAN_FLAG=OFF
)

cd build\win32
@rem Pin Conan's own CMake generator for dependency builds (e.g. yajl, built
@rem from source on --build=missing) to match the generator used below for
@rem the main project. Without this, Conan auto-detects a generator from
@rem whatever Visual Studio version its profile picks up, which on a machine
@rem with more than one VS install can differ from -G below -- and an older
@rem dependency's CMakeLists.txt (e.g. yajl's, which still sets deprecated
@rem policies like CMP0026 to OLD) may not configure under a newer,
@rem auto-detected generator/CMake even though it configures fine under the
@rem one this project actually targets.
conan install . -s compiler.cppstd=17 %CI_ASAN% -c tools.cmake.cmaketoolchain:generator="Visual Studio 17 2022" --output-folder=build --build=missing --settings=build_type=%build_type% --settings=arch=%arch%
cd build
cmake --fresh .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DUSE_ASAN=%ASAN_FLAG% %4 %5 %6 %7 %8 %9
cmake --build . --config %build_type%

popd
