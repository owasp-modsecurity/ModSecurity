cd %WORK%
rmdir /s /q %PCRE%
7z.exe x %PCRE%.zip
cd %PCRE%
CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
NMAKE
IF NOT DEFINED FULLBUILD pause
cd %WORK%

copy /y %WORK%\%PCRE%\pcre.dll %DROP%
copy /y %WORK%\%PCRE%\pcre.pdb %DROP%
copy /y %WORK%\%PCRE%\pcre.lib %DROP%
