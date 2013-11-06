cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%CURL%" goto file_not_found_bin

7z.exe x "%SOURCE_DIR%\%CURL%"

set CURL_DIR=%CURL:~0,-4%

copy /y CMakeLists.txt "%CURL_DIR%"
CD "%CURL_DIR%"
CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DCURL_ZLIB=True
"%WORK_DIR%\fart.exe" -r -C "%WORK_DIR%\%CURL_DIR%\include\curl\curlbuild.h" LLU ULL
NMAKE

cd "%WORK_DIR%"

copy /y "%WORK_DIR%\%CURL_DIR%\lib\libcurl.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\%CURL_DIR%\lib\libcurl.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\%CURL_DIR%\lib\libcurl_imp.lib" "%OUTPUT_DIR%"

exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%CURL%"
@goto failed

:failed
@exit /B 1

