cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%ZLIB%" goto file_not_found_bin


@7z.exe x "%SOURCE_DIR%\%ZLIB%" -so | 7z.exe x -aoa -si -ttar 

set ZLIB_DIR=%ZLIB:~0,-7%

move "%ZLIB_DIR%" "zlib"

cd "zlib"
nmake -f win32\Makefile.msc
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
SET INCLUDE=%INCLUDE%;%WORK_DIR%\zlib
SET LIB=%LIB%;%WORK_DIR%\zlib
cd "%WORK_DIR%"

copy /y "%WORK_DIR%\zlib\zlib1.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\zlib\zlib1.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\zlib\zdll.lib" "%OUTPUT_DIR%"

@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%ZLIB%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1
