cd %WORK%
rmdir /s /q %LUA%
7z.exe x %LUA%.zip
CD %LUA%\src
CL /Ox /arch:SSE2 /GF /GL /Gy /FD /EHsc /MD  /Zi /TC /wd4005 /D "_MBCS" /D "LUA_CORE" /D "LUA_BUILD_AS_DLL" /D "_CRT_SECURE_NO_WARNINGS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WIN32" /D "_WINDLL" /c *.c
DEL lua.obj luac.obj
LINK /DLL /LTCG /DEBUG /OUT:lua5.1.dll *.obj
IF EXIST lua5.1.dll.manifest MT  -manifest lua5.1.dll.manifest -outputresource:lua5.1.dll;2
IF NOT DEFINED FULLBUILD pause
cd %WORK%

copy /y %WORK%\%LUA%\src\lua5.1.dll %DROP%
copy /y %WORK%\%LUA%\src\lua5.1.pdb %DROP%
copy /y %WORK%\%LUA%\src\lua5.1.lib %DROP%
