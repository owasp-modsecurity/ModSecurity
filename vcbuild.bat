@rem For Windows build information, see build\win32\README.md

@echo off
setlocal

if not "%1"=="" (set build_type=%1) else (set build_type=Release)
echo Build type: %build_type%

if not "%2"=="" (set arch=%2) else (set arch=x86_64)
echo Arch: %arch%

if "%3"=="USE_ASAN" (
    echo Address Sanitizer: Enabled
    set CI_ASAN=-c:h tools.build:cxxflags="[""/fsanitize=address""]"
    set ASAN_FLAG=ON
) else (
    echo Address Sanitizer: Disabled
    set CI_ASAN=
    set ASAN_FLAG=OFF
)

pushd "%~dp0build\win32"
if errorlevel 1 exit /b %errorlevel%

conan export conan\yajl --user=modsecurity --channel=ci
if errorlevel 1 goto error

conan install . -pr:h default -pr:b default -s:h compiler.cppstd=17 %CI_ASAN% --build=missing -s:h build_type=%build_type% -s:h arch=%arch%
if errorlevel 1 goto error

cmake --fresh --preset conan-default -DUSE_ASAN=%ASAN_FLAG% %~4 %~5 %~6 %~7 %~8 %~9
if errorlevel 1 goto error

cmake --build build --config %build_type%
if errorlevel 1 goto error

popd
exit /b 0

:error
set "build_exit_code=%errorlevel%"
popd
exit /b %build_exit_code%
