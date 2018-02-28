:: Those variables should be edited as needed.
:: Use full paths.

:: General paths
@set WORK_DIR=%cd%\dependencies\build_dir
@set OUTPUT_DIR=%cd%\dependencies\release_files
@set SOURCE_DIR=%USERPROFILE%\Downloads

:: Dependencies
@set CMAKE=cmake-3.8.2-win32-x86.zip
@set PCRE=pcre-8.40.zip
@set ZLIB=zlib-1.2.11.tar.gz
@set LIBXML2=libxml2-2.9.4.tar.gz
@set LUA=lua-5.3.4.tar.gz
@set CURL=curl-7.54.1.zip
@set APACHE_SRC=httpd-2.4.27.tar.gz
@set APACHE_BIN32=httpd-2.4.27-win32-VC11.zip
@set APACHE_BIN64=httpd-2.4.27-win64-VC11.zip
@set YAJL=yajl-2.1.0.zip
@set SSDEEP=ssdeep-2.13.tar.gz
@set SSDEEP_BIN=ssdeep-2.13.zip

@set CMAKE_DIR=%WORK_DIR%\%CMAKE:~0,-4%\bin

:: Aditional paths.
@set PATH=%PATH%;%CMAKE_DIR%;"c:\program files\7-zip"


:: @set VCARGS32="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
:: @set VCARGS64="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"


:: Do not edit bellow this line.
@set CURRENT_DIR=%cd%

@echo Cleaning directories...
if EXIST "%WORK_DIR%" @rmdir /s /q "%WORK_DIR%"
if EXIST "%OUTPUT_DIR%" @rmdir /s /q "%OUTPUT_DIR%"
@mkdir "%WORK_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto failed_to_cleanup
@mkdir "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto failed_to_cleanup

@echo Checking for vcargs... %1
@if NOT (%1) == "" call %1
@if (%ERRORLEVEL%) == (1) goto build_wrong_vcargs

set APACHE_BIN=%APACHE_BIN32%
call cl 2>&1 | findstr /C:"x64"
@if (%ERRORLEVEL%) == (0) set APACHE_BIN=%APACHE_BIN64%

@echo Starting with the depdendencies...
@echo # CMake. - %CMAKE%
@call dependencies/build_cmake.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_cmake
@cd "%CURRENT_DIR%"


@echo # Apache - %HTTPD%/%APACHE24_ZIP%
@call dependencies/build_apache.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_apache
@cd "%CURRENT_DIR%"

@echo # pcre. - %PCRE%
@call dependencies/build_pcre.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_pcre
@cd "%CURRENT_DIR%"

@echo # zlib - %ZLIB%
@call dependencies/build_zlib.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_zlib
@cd "%CURRENT_DIR%"

@echo # libxml2 - %LIBXML2%
@call dependencies/build_libxml2.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_libxml2
@cd "%CURRENT_DIR%"

@echo # lua - %LUA%
@call dependencies/build_lua.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_lua
@cd "%CURRENT_DIR%"

@echo # curl - %CURL%
@call dependencies/build_curl.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_curl
@cd "%CURRENT_DIR%"

@echo # yajl - %YAJL%
@call dependencies/build_yajl.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_yajl
@cd "%CURRENT_DIR%"

@echo # ssdeep - %SSDEEP%
@call dependencies/build_ssdeep.bat
@if NOT (%ERRORLEVEL%) == (0) goto build_failed_ssdeep
@cd "%CURRENT_DIR%"

@echo All dependencies were built successfully.
@cd "%CURRENT_DIR%"
@exit /B 0

:failed_to_cleanup
@echo Failed to cleanup
@goto failed

:build_wrong_vcargs
@echo Please specify a valid vcargs
@goto failed

:build_failed_apache
@echo Failed to setup %HTTPD%/%APACHE24_ZIP%...
@goto failed

:build_failed_pcre
@echo Failed to setup %PCRE%...
@goto failed

:build_failed_zlib
@echo Failed to setup %ZLIB%...
@goto failed

:build_failed_libxml2
@echo Failed to setup %LIBXML2%...
@goto failed

:build_failed_lua
@echo Failed to setup %LUA%...
@goto failed

:build_failed_curl
@echo Failed to setup %CURL%...
@goto failed

:build_failed_yajl
@echo Failed to setup %YAJL%...
@goto failed

:build_failed_ssdeep
@echo Failed to setup %SSDEEP%...
@goto failed

:build_failed_cmake
@echo Failed to setup %CMAKE%...
@goto failed

:failed
@cd %CURRENT_DIR%
@exit /B 1

