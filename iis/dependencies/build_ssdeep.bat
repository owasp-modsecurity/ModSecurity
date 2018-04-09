cd "%WORK_DIR%"

echo "%SOURCE_DIR%\%SSDEEP%"
echo "%SOURCE_DIR%\%SSDEEP_BIN%"

@if NOT EXIST "%SOURCE_DIR%\%SSDEEP%" goto build_failed

@7z.exe x "%SOURCE_DIR%\%SSDEEP_BIN%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
@7z.exe x "%SOURCE_DIR%\%SSDEEP%" -so | 7z.exe x -aoa -si -ttar 
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

set SSDEEP_DIR=%SSDEEP_BIN:~0,-4%

move "%SSDEEP_DIR%" "ssdeep"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
cd "%WORK_DIR%\ssdeep\"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

@set SSDEEP_ARCH="x86"
@call cl 2>&1 | findstr /C:"x64"
@if (%ERRORLEVEL%) == (0) set SSDEEP_ARCH="x64"

lib /machine:%SSDEEP_ARCH% /def:fuzzy.def
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

copy /y "%WORK_DIR%\ssdeep\fuzzy.dll" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
copy /y "%WORK_DIR%\ssdeep\fuzzy.def" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
copy /y "%WORK_DIR%\ssdeep\fuzzy.lib" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed


@exit /B 0

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1
