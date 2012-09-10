cd %WORK%
rmdir /s /q %LIBXML2%
7z.exe x %LIBXML2%.zip
fart.exe -r -i -C %WORK%\%LIBXML2%\win32\*.* \x2Fopt:nowin98 " "
cd %LIBXML2%\win32
CSCRIPT configure.js iconv=no vcmanifest=yes zlib=yes
NMAKE -f Makefile.msvc
IF NOT DEFINED FULLBUILD pause
cd %WORK%

copy /y %WORK%\%LIBXML2%\win32\bin.msvc\libxml2.dll %DROP%
copy /y %WORK%\%LIBXML2%\win32\bin.msvc\libxml2.lib %DROP%
