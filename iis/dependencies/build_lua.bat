cd "%WORK_DIR%"

@if NOT EXIST "%SOURCE_DIR%\%LUA%" goto file_not_found_bin

@7z.exe x "%SOURCE_DIR%\%LUA%" -so | 7z.exe x -aoa -si -ttar 

set LUA_DIR=%LUA:~0,-7%

move "%LUA_DIR%" "lua"

cd "lua\src"

CL /Ox /arch:SSE2 /GF /GL /Gy /FD /EHsc /MD  /Zi /TC /wd4005 /D "_MBCS" /D "LUA_CORE" /D "LUA_BUILD_AS_DLL" /D "_CRT_SECURE_NO_WARNINGS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WIN32" /D "_WINDLL" /c *.c
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
DEL lua.obj luac.obj
LINK /DLL /LTCG /DEBUG /OUT:lua54.dll *.obj
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
IF EXIST lua54.dll.manifest MT  -manifest lua54.dll.manifest -outputresource:lua54.dll;2
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

cd "%WORK_DIR%"

copy /y "%WORK_DIR%\lua\src\lua54.dll" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\lua\src\lua54.pdb" "%OUTPUT_DIR%"
copy /y "%WORK_DIR%\lua\src\lua54.lib" "%OUTPUT_DIR%"

@exit /B 0

:file_not_found_bin
@echo File not found: "%SOURCE_DIR%\%LUA%"
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@exit /B 1

