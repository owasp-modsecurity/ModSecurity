cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%APACHE_BIN%" goto file_not_found_bin

7z.exe x "%SOURCE_DIR%\%PCRE%"
set PCRE_DIR=%PCRE:~0,-4%

move "%PCRE_DIR%" "pcre"

cd "pcre"
CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
NMAKE
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cd "%WORK%"

copy /y "%WORK_DIR%\pcre\pcre.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre\pcre.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre\pcre.lib" "%OUTPUT_DIR%"
echo "a"
@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%PCRE%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1
