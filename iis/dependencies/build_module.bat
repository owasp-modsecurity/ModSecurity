cd %WORK%
CD mod_security\apache2
del *.obj *.dll *.lib
NMAKE -f Makefile.win APACHE=%APACHE% PCRE=%WORK%\%PCRE% LIBXML2=%WORK%\%LIBXML2% LUA=%WORK%\%LUA%\src VERSION=VERSION_IIS
cd ..\mlogc
copy /y %WORK%\Makefile.win .
nmake -f Makefile.win clean
nmake -f Makefile.win APACHE=%APACHE% PCRE=%WORK%\%PCRE% CURL=%WORK%\%CURL% VERSION=VERSION_IIS
cd ..\iis
nmake -f Makefile.win clean
NMAKE -f Makefile.win APACHE=%APACHE% PCRE=%WORK%\%PCRE% LIBXML2=%WORK%\%LIBXML2% LUA=%WORK%\%LUA%\src VERSION=VERSION_IIS
cd %WORK%

copy /y %WORK%\mod_security\mlogc\mlogc.exe %DROP%
copy /y %WORK%\mod_security\iis\modsecurityiis.dll %DROP%
copy /y %WORK%\mod_security\iis\modsecurityiis.pdb %DROP%
