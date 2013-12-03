cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%YAJL%" goto file_not_found_bin

7z.exe x "%SOURCE_DIR%\%YAJL%"
set YAJL_DIR=%YAJL:~0,-4%

move "%YAJL_DIR%" "yajl"

cd "yajl"

mkdir build
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cd build
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cmake -G"NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
nmake
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

cd "%WORK%"

copy /y "%WORK_DIR%\yajl\build\yajl-2.0.1\lib\yajl.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\yajl\build\yajl-2.0.1\lib\yajl.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\yajl\build\yajl-2.0.1\lib\yajl.lib" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\yajl\build\yajl-2.0.1\lib\yajl_s.lib" "%OUTPUT_DIR%"

@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%PCRE%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1
