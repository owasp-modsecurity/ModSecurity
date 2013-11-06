cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%LIBXML2%" goto file_not_found_bin

@7z.exe x "%SOURCE_DIR%\%LIBXML2%" -so | 7z.exe x -aoa -si -ttar 

set LIBXML2_DIR=%LIBXML2:~0,-7%

fart.exe -r -i -C "%WORK_DIR%\%LIBXML2_DIR%\win32\*.*" \x2Fopt:nowin98 " "
cd "%LIBXML2_DIR%\win32"
CSCRIPT configure.js iconv=no vcmanifest=yes zlib=yes
NMAKE -f Makefile.msvc

cd "%WORK%"

copy /y "%WORK_DIR%\%LIBXML2_DIR%\win32\bin.msvc\libxml2.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\%LIBXML2_DIR%\win32\bin.msvc\libxml2.lib" "%OUTPUT_DIR%"

@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%LIBXML2%"
@goto failed

:failed
@exit /B 1

