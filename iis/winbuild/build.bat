c:
set WORK=c:\work
set HTTPD=httpd-2.2.22
set APACHE=c:\Apache22
set PCRE=pcre-8.30
set ZLIB=zlib-1.2.7
set LIBXML2=libxml2-2.7.7
set LUA=lua-5.1.5
set CURL=curl-7.24.0

cd %WORK%
rmdir /s /q %APACHE%
rmdir /s /q %HTTPD%
7z.exe x %HTTPD%-win32-src.zip
fart.exe -r -i -C %WORK%\%HTTPD%\*.mak \x2Fmachine:x86 " "
fart.exe -r -i -C %WORK%\%HTTPD%\*.mk.win \x2Fmachine:x86 " "
copy /y %WORK%\libhttpd.mak %WORK%\%HTTPD%
copy /y %WORK%\mod_ssl.mak %WORK%\%HTTPD%\modules\ssl
copy /y %WORK%\apr.mak %WORK%\%HTTPD%\srclib\apr
copy /y %WORK%\libapr.mak %WORK%\%HTTPD%\srclib\apr
copy /y %WORK%\aprutil.mak %WORK%\%HTTPD%\srclib\apr-util
copy /y %WORK%\libaprutil.mak %WORK%\%HTTPD%\srclib\apr-util
copy /y %WORK%\dftables.mak %WORK%\%HTTPD%\srclib\pcre
copy /y %WORK%\pcre.mak %WORK%\%HTTPD%\srclib\pcre
cd %HTTPD%
nmake -f Makefile.win installr
SET HTTPD_BUILD=%WORK%\%HTTPD%
IF NOT DEFINED FULLBUILD pause

cd %WORK%
rmdir /s /q %PCRE%
7z.exe x %PCRE%.zip
cd %PCRE%
CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
NMAKE
IF NOT DEFINED FULLBUILD pause

cd %WORK%
rmdir /s /q %ZLIB%
7z.exe x %ZLIB%.zip
cd %ZLIB%
nmake -f win32\Makefile.msc
SET INCLUDE=%INCLUDE%;%WORK%\%ZLIB%
SET LIB=%LIB%;%WORK%\%ZLIB%
IF NOT DEFINED FULLBUILD pause

cd %WORK%
rmdir /s /q %LIBXML2%
7z.exe x %LIBXML2%.zip
fart.exe -r -i -C %WORK%\%LIBXML2%\win32\*.* \x2Fopt:nowin98 " "
cd %LIBXML2%\win32
CSCRIPT configure.js iconv=no vcmanifest=yes zlib=yes
NMAKE -f Makefile.msvc
IF NOT DEFINED FULLBUILD pause

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
rmdir /s /q %CURL%
7z.exe x %CURL%.zip
copy /y CMakeLists.txt %CURL%
CD %CURL%
CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DCURL_ZLIB=True
%WORK%\fart.exe -r -C %WORK%\%CURL%\include\curl\curlbuild.h LLU ULL
NMAKE
IF NOT DEFINED FULLBUILD pause

cd %WORK%
CD mod_security\apache2
del *.obj *.dll *.lib
NMAKE -f Makefile.win APACHE=C:\Apache22 PCRE=%WORK%\%PCRE% LIBXML2=%WORK%\%LIBXML2% LUA=%WORK%\%LUA%\src VERSION=VERSION_IIS
cd ..\mlogc
copy /y %WORK%\Makefile.win .
nmake -f Makefile.win clean
nmake -f Makefile.win
cd ..\iis
nmake -f Makefile.win clean
NMAKE -f Makefile.win APACHE=C:\Apache22 PCRE=%WORK%\%PCRE% LIBXML2=%WORK%\%LIBXML2% LUA=%WORK%\%LUA%\src VERSION=VERSION_IIS

cd %WORK%

copy /y %APACHE%\bin\libapr-1.dll %DROP%
copy /y %APACHE%\bin\libapr-1.pdb %DROP%
copy /y %APACHE%\lib\libapr-1.lib %DROP%
copy /y %APACHE%\bin\libapriconv-1.dll %DROP%
copy /y %APACHE%\bin\libapriconv-1.pdb %DROP%
copy /y %APACHE%\lib\libapriconv-1.lib %DROP%
copy /y %APACHE%\bin\libaprutil-1.dll %DROP%
copy /y %APACHE%\bin\libaprutil-1.pdb %DROP%
copy /y %APACHE%\lib\libaprutil-1.lib %DROP%
copy /y %WORK%\%LIBXML2%\win32\bin.msvc\libxml2.dll %DROP%
copy /y %WORK%\%LIBXML2%\win32\bin.msvc\libxml2.lib %DROP%
copy /y %WORK%\%LUA%\src\lua5.1.dll %DROP%
copy /y %WORK%\%LUA%\src\lua5.1.pdb %DROP%
copy /y %WORK%\%LUA%\src\lua5.1.lib %DROP%
copy /y %WORK%\%PCRE%\pcre.dll %DROP%
copy /y %WORK%\%PCRE%\pcre.pdb %DROP%
copy /y %WORK%\%PCRE%\pcre.lib %DROP%
copy /y %WORK%\%ZLIB%\zlib1.dll %DROP%
copy /y %WORK%\%ZLIB%\zlib1.pdb %DROP%
copy /y %WORK%\%ZLIB%\zdll.lib %DROP%
copy /y %WORK%\%CURL%\libcurl.dll %DROP%
copy /y %WORK%\%CURL%\libcurl.pdb %DROP%
copy /y %WORK%\%CURL%\libcurl_imp.lib %DROP%
copy /y %WORK%\mod_security\mlogc\mlogc.exe %DROP%
copy /y %WORK%\mod_security\iis\modsecurityiis.dll %DROP%
copy /y %WORK%\mod_security\iis\modsecurityiis.pdb %DROP%
