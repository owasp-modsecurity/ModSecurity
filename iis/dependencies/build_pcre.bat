::@if NOT (%ERRORLEVEL%) == (1) Echo "Patch successfull... For more info on patch see: https://vcs.pcre.org/pcre/code/trunk/CMakeLists.txt?r1=1659&r2=1677&view=patch"

cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%APACHE_BIN%" goto file_not_found_bin

7z.exe x "%SOURCE_DIR%\%PCRE%"
set PCRE_DIR=%PCRE:~0,-4%

move "%PCRE_DIR%" "pcre2"

cd "pcre2"
@echo Building PCRE2 with JIT support...
CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DPCRE2_BUILD_PCRE2_8=ON -DPCRE2_SUPPORT_JIT=ON
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
NMAKE
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cd "%WORK%"

copy /y "%WORK_DIR%\pcre2\pcre2-8.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre2\pcre2-8.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\pcre2\pcre2-8.lib" "%OUTPUT_DIR%"
@if not exist "%WORK_DIR%\pcre2\include" mkdir "%WORK_DIR%\pcre2\include"
xcopy /y "%WORK_DIR%\pcre2\src\pcre2*.h" "%WORK_DIR%\pcre2\include\" >nul 2>&1
copy /y "%WORK_DIR%\pcre2\pcre2.h" "%WORK_DIR%\pcre2\include\" >nul 2>&1
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
