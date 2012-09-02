cd %WORK%
rmdir /s /q %APACHE%
rmdir /s /q %HTTPD%
IF NOT DEFINED BUILD_APACHE GOTO USE_APACHE_24
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
IF NOT DEFINED FULLBUILD pause
GOTO END

:USE_APACHE_24
set APACHE=%WORK%\Apache24
del /q /f --*.*
del /q /f readme.txt
rmdir /s /q %APACHE%
7z.exe x %APACHE24_ZIP%
7z.exe x %HTTPD%.zip

:END
copy /y %APACHE%\bin\libapr-1.dll %DROP%
copy /y %APACHE%\bin\libapr-1.pdb %DROP%
copy /y %APACHE%\lib\libapr-1.lib %DROP%
copy /y %APACHE%\bin\libapriconv-1.dll %DROP%
copy /y %APACHE%\bin\libapriconv-1.pdb %DROP%
copy /y %APACHE%\lib\libapriconv-1.lib %DROP%
copy /y %APACHE%\bin\libaprutil-1.dll %DROP%
copy /y %APACHE%\bin\libaprutil-1.pdb %DROP%
copy /y %APACHE%\lib\libaprutil-1.lib %DROP%
SET HTTPD_BUILD=%WORK%\%HTTPD%
cd %WORK%
