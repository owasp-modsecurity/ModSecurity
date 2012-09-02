cd %WORK%
rmdir /s /q %CURL%
7z.exe x %CURL%.zip
copy /y CMakeLists.txt %CURL%
CD %CURL%
CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DCURL_ZLIB=True
%WORK%\fart.exe -r -C %WORK%\%CURL%\include\curl\curlbuild.h LLU ULL
NMAKE
IF NOT DEFINED FULLBUILD pause
cd %WORK%

copy /y %WORK%\%CURL%\libcurl.dll %DROP%
copy /y %WORK%\%CURL%\libcurl.pdb %DROP%
copy /y %WORK%\%CURL%\libcurl_imp.lib %DROP%
