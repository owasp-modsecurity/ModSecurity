cd "%WORK_DIR%"
@if NOT EXIST "%SOURCE_DIR%\%CURL%" goto file_not_found_bin
echo "7z..."
7z.exe x "%SOURCE_DIR%\%CURL%"
echo "Timeout..."
timeout 5
echo "Curl..."
set CURL_DIR=%CURL:~0,-4%
echo "Move..."
move "%CURL_DIR%" "curl"
echo "Cd..."
CD "curl\winbuild"

@set ARCH=x86
@call cl 2>&1 | findstr /C:"x64"
@if (%ERRORLEVEL%) == (0) set ARCH=x64

nmake /f Makefile.vc mode=dll ENABLE_WINSSL=yes MACHINE=%ARCH% WITH_ZLIB=dll
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

cd "%WORK_DIR%"

copy /y "%WORK_DIR%\curl\builds\libcurl-vc-%ARCH%-release-dll-zlib-dll-ipv6-sspi-winssl-obj-lib\libcurl.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\curl\builds\libcurl-vc-%ARCH%-release-dll-zlib-dll-ipv6-sspi-winssl-obj-lib\libcurl.lib" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\curl\builds\libcurl-vc-%ARCH%-release-dll-zlib-dll-ipv6-sspi-winssl-obj-lib\libcurl.lib" "%WORK_DIR%\curl\libcurl.lib"

exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%CURL%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1


-> 
