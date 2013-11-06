cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%APACHE_BIN%" goto file_not_found_bin

7z.exe x "%SOURCE_DIR%\%PCRE%"
set PCRE_DIR=%PCRE:~0,-4%

mklink /D "pcre" "%PCRE_DIR%"

cd "%PCRE_DIR%"
CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
NMAKE
cd "%WORK%"

copy /y "%WORK_DIR%\%PCRE_DIR%\pcre.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\%PCRE_DIR%\pcre.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\%PCRE_DIR%\pcre.lib" "%OUTPUT_DIR%"
echo "a"
@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%PCRE%"
@goto failed

:failed
@exit /B 1
