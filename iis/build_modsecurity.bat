
set DEPENDENCIES_DIR=dependencies\build_dir
set OUTPUT_DIR=dependencies\release_files

set CURRENT_DIR=%cd%

@echo Checking for vcargs... %1
@if NOT (%1) == "" call %1
@if (%ERRORLEVEL%) == (1) goto build_wrong_vcarg

@echo Deleting old stuff...
@del *.obj *.dll *.lib

@echo apache2...
cd ..\apache2
del *.obj *.dll *.lib
del libinjection\*.obj libinjection\*.dll libinjection\*.lib
NMAKE -f Makefile.win APACHE=..\iis\%DEPENDENCIES_DIR%\Apache24 PCRE=..\iis\%DEPENDENCIES_DIR%\pcre LIBXML2=..\iis\%DEPENDENCIES_DIR%\libxml2 LUA=..\iis\%DEPENDENCIES_DIR%\lua\src VERSION=VERSION_IIS YAJL=..\iis\%DEPENDENCIES_DIR%\yajl\build\%YAJL_DIR%\yajl-2.1.0 SSDEEP=..\iis\%DEPENDENCIES_DIR%\ssdeep CURL=..\iis\%DEPENDENCIES_DIR%\curl IIS_BUILD=yes
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

@echo mlogc...
cd ..\mlogc
del *.obj *.dll *.lib
nmake -f Makefile.win clean
nmake -f Makefile.win APACHE=..\iis\%DEPENDENCIES_DIR%\Apache24 PCRE=..\iis\%DEPENDENCIES_DIR%\pcre CURL=..\iis\%DEPENDENCIES_DIR%\curl YAJL=..\iis\%DEPENDENCIES_DIR%\yajl\build\%YAJL_DIR%\yajl-2.1.0 SSDEEP=..\iis\%DEPENDENCIES_DIR%\ssdeep VERSION=VERSION_IIS
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

@echo iis...
cd ..\iis
del *.obj *.dll *.lib
nmake -f Makefile.win clean
NMAKE -f Makefile.win APACHE=..\iis\%DEPENDENCIES_DIR%\Apache24 PCRE=..\iis\%DEPENDENCIES_DIR%\pcre LIBXML2=..\iis\%DEPENDENCIES_DIR%\libxml2 LUA=..\iis\%DEPENDENCIES_DIR%\lua\src VERSION=VERSION_IIS YAJL=..\iis\%DEPENDENCIES_DIR%\yajl\build\%YAJL_DIR%\yajl-2.1.0 SSDEEP=..\iis\%DEPENDENCIES_DIR%\ssdeep CURL=..\iis\%DEPENDENCIES_DIR%\curl
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

cd %CURRENT_DIR%

@echo Copy...
copy /y ..\mlogc\mlogc.exe %OUTPUT_DIR%
copy /y ..\iis\modsecurityiis.dll %OUTPUT_DIR%
copy /y ..\iis\modsecurityiis.pdb %OUTPUT_DIR%

exit /B 0

:build_wrong_vcargs
@echo Please specify a valid vcargs
@goto failed

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@cd %CURRENT_DIR%
@exit /B 1

