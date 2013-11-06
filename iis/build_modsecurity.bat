
set DEPENDENCIES_DIR=dependencies\build_dir
set OUTPUT_DIR=dependencies\release_files

set CURRENT_DIR=%cd%

@echo Checking for vcargs... %1
@if NOT (%1) == "" call %1
@if (%ERRORLEVEL%) == (1) goto build_wrong_vcarg


cd ..\apache2
del *.obj *.dll *.lib
NMAKE -f Makefile.win APACHE=..\iis\%DEPENDENCIES_DIR%\Apache24 PCRE=..\iis\%DEPENDENCIES_DIR%\pcre LIBXML2=..\iis\%DEPENDENCIES_DIR%\libxml2 LUA=..\iis\%DEPENDENCIES_DIR%\lua\src VERSION=VERSION_IIS

cd ..\mlogc
nmake -f Makefile.win clean
nmake -f Makefile.win APACHE=..\iis\%DEPENDENCIES_DIR%\Apache24 PCRE=..\iis\%DEPENDENCIES_DIR%\pcre CURL=..\iis\%DEPENDENCIES_DIR%\curl VERSION=VERSION_IIS

cd ..\iis
nmake -f Makefile.win clean
NMAKE -f Makefile.win APACHE=..\iis\%DEPENDENCIES_DIR%\Apache24 PCRE=..\iis\%DEPENDENCIES_DIR%\pcre LIBXML2=..\iis\%DEPENDENCIES_DIR%\libxml2 LUA=..\iis\%DEPENDENCIES_DIR%\lua\src VERSION=VERSION_IIS

cd %CURRENT_DIR%

copy /y ..\mlogc\mlogc.exe %OUTPUT_DIR%
copy /y ..\iis\modsecurityiis.dll %OUTPUT_DIR%
copy /y ..\iis\modsecurityiis.pdb %OUTPUT_DIR%

exit /B 0

:build_wrong_vcargs
@echo Please specify a valid vcargs
@goto failed

:failed
@cd %CURRENT_DIR%
@exit /B 
