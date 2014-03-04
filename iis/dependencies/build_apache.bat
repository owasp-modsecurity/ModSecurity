@cd "%WORK_DIR%"
@set APACHE=%WORK_DIR%\Apache24

@if NOT EXIST "%SOURCE_DIR%\%APACHE_BIN%" goto file_not_found_bin
@if NOT EXIST "%SOURCE_DIR%\%APACHE_SRC%" goto file_not_found_src

@7z.exe x "%SOURCE_DIR%\%APACHE_BIN%"
@if NOT (%ERRORLEVEL%) == (0) goto something_went_wrong

@7z.exe x "%SOURCE_DIR%\%APACHE_SRC%" -so | 7z.exe x -aoa -si -ttar 
@if NOT (%ERRORLEVEL%) == (0) goto something_went_wrong

:: Missing:  %APACHE%\bin\libapr-1.pdb %APACHE%\bin\libapr-1.lib ?
copy /y "%APACHE%\bin\libapr-1.dll" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto something_went_wrong_copy

:: Missing: %APACHE%\bin\libapriconv-1.pdb %APACHE%\lib\libapriconv-1.lib ?
copy /y "%APACHE%\bin\libapriconv-1.dll" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto something_went_wrong_copy

:: Missing: %APACHE%\bin\libaprutil-1.pdb %APACHE%\lib\libaprutil-1.lib ?
copy /y "%APACHE%\bin\libaprutil-1.dll" "%OUTPUT_DIR%"
@if NOT (%ERRORLEVEL%) == (0) goto something_went_wrong_copy

@cd "%WORK_DIR%"

@exit /B 0

:something_went_wrong_copy
@echo Something went wrong while trying to copy Apache binaries files.
@goto failed

:file_not_found_bin
@echo File not found: %SOURCE_DIR%\%APACHE_BIN%
@goto failed

:file_not_found_src
@echo File not found: %SOURCE_DIR%\%APACHE_SRC%
@goto failed

:something_went_wrong
@echo Something went wrong while unzip Apache files.
@goto failed

:failed
@exit /B 1

