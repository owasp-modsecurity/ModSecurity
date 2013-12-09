cd "%WORK_DIR%"

echo "Checking for file... %SOURCE_DIR%\%SSDEEP%"
@if NOT EXIST "%SOURCE_DIR%\%SSDEEP%" goto build_failed

echo "7z..."
@7z.exe x "%SOURCE_DIR%\%SSDEEP_BIN%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
echo "7z..."
@7z.exe x "%SOURCE_DIR%\%SSDEEP%" -so | 7z.exe x -aoa -si -ttar 
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

echo "Set SSDEEP_DIR..."
set SSDEEP_DIR=%SSDEEP_BIN:~0,-4%

echo "Move..."
move "%SSDEEP_DIR%" "ssdeep"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
echo "Cd..."
cd "%WORK_DIR%\ssdeep\"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

echo "Set SSDEEP_ARCH..."
@set SSDEEP_ARCH="x86"
@call cl 2>&1 | findstr /C:"x64"
@if (%ERRORLEVEL%) == (0) set SSDEEP_ARCH="x64"

echo "lib..."
lib /machine:%SSDEEP_ARCH% /def:fuzzy.def
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

echo "copy..."
copy /y "%WORK_DIR%\ssdeep\fuzzy.dll" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
echo "copy..."
copy /y "%WORK_DIR%\ssdeep\fuzzy.def" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
echo "copy..."
copy /y "%WORK_DIR%\ssdeep\fuzzy.lib" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

echo "done"

@exit /B 0

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1
