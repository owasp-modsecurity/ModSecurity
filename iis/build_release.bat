
echo "Building release..."

set RELEASE_DIR=release
set OUTPUT_DIR=%cd%\dependencies\release_files
set CURRENT_DIR=%cd%

set AMD64=%RELEASE_DIR%\amd64
set X86=%RELEASE_DIR%\x86

mkdir "%RELEASE_DIR%"

mkdir "%AMD64%"

mkdir "%X86%"

set VCARGS32="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
set VCARGS64="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\x86_amd64\vcvarsx86_amd64.bat"

call build_dependencies.bat %VCARGS64%
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
call build_modsecurity.bat %VCARGS64%
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

copy "%OUTPUT_DIR%\libapr-1.dll" "%AMD64%"
copy "%OUTPUT_DIR%\libapriconv-1.dll" "%AMD64%"
copy "%OUTPUT_DIR%\libaprutil-1.dll" "%AMD64%"
copy "%OUTPUT_DIR%\libcurl.dll" "%AMD64%"
copy "%OUTPUT_DIR%\libxml2.dll" "%AMD64%"
copy "%OUTPUT_DIR%\lua5.1.dll" "%AMD64%"
copy "%OUTPUT_DIR%\mlogc.exe" "%AMD64%"
copy "%OUTPUT_DIR%\ModSecurityIIS.dll" "%AMD64%"
copy "%OUTPUT_DIR%\pcre.dll" "%AMD64%"
copy "%OUTPUT_DIR%\zlib1.dll" "%AMD64%"


call build_dependencies.bat %VCARGS32%
@if NOT (%ERRORLEVEL%) == (0) goto build_failed
call build_modsecurity.bat %VCARGS32%
@if NOT (%ERRORLEVEL%) == (0) goto build_failed

copy "%OUTPUT_DIR%\libapr-1.dll" "%X86%"
copy "%OUTPUT_DIR%\libapriconv-1.dll" "%X86%"
copy "%OUTPUT_DIR%\libaprutil-1.dll" "%X86%"
copy "%OUTPUT_DIR%\libcurl.dll" "%X86%"
copy "%OUTPUT_DIR%\libxml2.dll" "%X86%"
copy "%OUTPUT_DIR%\lua5.1.dll" "%X86%"
copy "%OUTPUT_DIR%\mlogc.exe" "%X86%"
copy "%OUTPUT_DIR%\ModSecurityIIS.dll" "%X86%"
copy %OUTPUT_DIR%\pcre.dll "%X86%"
copy %OUTPUT_DIR%\zlib1.dll "%X86%"


:: copy %OUTPUT_DIR%\Installer.exe %RELEASE_DIR%
:: copy %OUTPUT_DIR%\ModSecurity.xml %RELEASE_DIR%
:: copy %OUTPUT_DIR%\owasp_csr.zip %RELEASE_DIR%
:: copy %OUTPUT_DIR%\README.txt %RELEASE_DIR%


exit /B 0

:build_failed
@echo Problems during the building phase
@goto failed

:failed
@cd %CURRENT_DIR%
@exit /B 1

