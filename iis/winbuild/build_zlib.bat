cd %WORK%
rmdir /s /q %ZLIB%
7z.exe x %ZLIB%.zip
cd %ZLIB%
nmake -f win32\Makefile.msc
SET INCLUDE=%INCLUDE%;%WORK%\%ZLIB%
SET LIB=%LIB%;%WORK%\%ZLIB%
IF NOT DEFINED FULLBUILD pause
cd %WORK%

copy /y %WORK%\%ZLIB%\zlib1.dll %DROP%
copy /y %WORK%\%ZLIB%\zlib1.pdb %DROP%
copy /y %WORK%\%ZLIB%\zdll.lib %DROP%
