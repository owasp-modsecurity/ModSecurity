@cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%CMAKE%" goto file_not_found


@7z.exe x "%SOURCE_DIR%\%CMAKE%"
@if NOT (%ERRORLEVEL%) == (0) goto something_went_wrong

@exit /B 0

:file_not_found
@echo File not found: %SOURCE_DIR%\%CMAKE%
@goto failed

:something_went_wrong
@echo Something went wrong while unzip CMake files.
@goto failed

:failed
@exit /B 1