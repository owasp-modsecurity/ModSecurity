@rem For Windows build information, see build\win32\README.md

@echo off
pushd %CD%

if not "%1"=="" (set build_type=%1) else (set build_type=Release)
echo Build type: %build_type%

if not "%2"=="" (set arch=%2) else (set arch=x86_64)
echo Arch: %arch%

@rem Single source of truth for the targeted Visual Studio version: both the
@rem Conan-side generator pin below and the CMake -G flag must agree, or
@rem dependencies built from source (e.g. yajl, on --build=missing) can end
@rem up configured under a different VS/CMake than the main project.
set VS_GENERATOR=Visual Studio 17 2022

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
@rem Pin Conan's own generator for dependency builds (e.g. yajl, built from
@rem source on --build=missing) to VS_GENERATOR too -- see the note above.
conan install . -s compiler.cppstd=17 %CI_ASAN% -c tools.cmake.cmaketoolchain:generator="%VS_GENERATOR%" --output-folder=build --build=missing --settings=build_type=%build_type% --settings=arch=%arch%
cd build
cmake --fresh .. -G "%VS_GENERATOR%" -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DUSE_ASAN=%ASAN_FLAG% %4 %5 %6 %7 %8 %9
cmake --build . --config %build_type%

popd
