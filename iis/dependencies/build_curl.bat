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
:: copy /y CMakeLists.txt "curl"
CD "curl"
echo "Cmake..."
CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DCURL_ZLIB=True
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
:: "%WORK_DIR%\fart.exe" -r -C "%WORK_DIR%\curl\include\curl\curlbuild.h" LLU ULL
NMAKE
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cd "%WORK_DIR%"

copy /y "%WORK_DIR%\curl\lib\libcurl.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\curl\lib\libcurl.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\curl\lib\libcurl_imp.lib" "%OUTPUT_DIR%"

exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%CURL%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1

