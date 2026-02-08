@set CMAKE=cmake-4.2.3-windows-x86_64.zip
@set PCRE=pcre2-10.47.zip
@set ZLIB=zlib-1.3.1.tar.gz
@set LIBXML2=libxml2-2.15.1.tar.gz
@set LUA=lua-5.4.8.tar.gz
@set CURL=curl-8.18.0.zip
@set APACHE_SRC=httpd-2.4.66.tar.gz
@set APACHE_BIN32=httpd-2.4.66-win32-VS16.zip
@set APACHE_BIN64=httpd-2.4.66-win64-VS16.zip
@set YAJL=yajl-2.1.0.zip
@set SSDEEP=ssdeep-2.14.1.tar.gz
@set SSDEEP_BIN=ssdeep-2.14.1.zip

:: BITSAdmin refuses to download YAJL from GitHub URL
:: @set YAJL_URL=https://github.com/lloyd/yajl/archive/%YAJL:~-9%
@set YAJL_URL=http://http.debian.net/debian/pool/main/y/yajl/yajl_2.1.0.orig.tar.gz

@set CMAKE_URL=https://github.com/Kitware/CMake/releases/download/v4.2.3/%CMAKE%
@set PCRE_URL=https://github.com/PCRE2Project/pcre2/releases/download/pcre2-10.47/%PCRE%
@set ZLIB_URL=https://github.com/madler/zlib/releases/download/v1.3.1/%ZLIB%
@set LIBXML2_URL=https://download.gnome.org/sources/libxml2/2.15/%LIBXML2%
@set LUA_URL=https://www.lua.org/ftp/%LUA%
@set CURL_URL=https://curl.se/download/%CURL%
@set APACHE_SRC_URL=https://www.apache.org/dist/httpd/%APACHE_SRC%
@set APACHE_BIN_URL=https://www.apachelounge.com/download/VS16/binaries
@set SSDEEP_URL=https://downloads.sourceforge.net/project/ssdeep/ssdeep-2.14.1

bitsadmin.exe /transfer "Downloading dependencies..." %CMAKE_URL% %SOURCE_DIR%\%CMAKE% %PCRE_URL% %SOURCE_DIR%\%PCRE% %ZLIB_URL% %SOURCE_DIR%\%ZLIB% %LIBXML2_URL% %SOURCE_DIR%\%LIBXML2% %LUA_URL% %SOURCE_DIR%\%LUA% %CURL_URL% %SOURCE_DIR%\%CURL% %APACHE_SRC_URL% %SOURCE_DIR%\%APACHE_SRC% %APACHE_BIN_URL%/%APACHE_BIN32% %SOURCE_DIR%\%APACHE_BIN32% %APACHE_BIN_URL%/%APACHE_BIN64% %SOURCE_DIR%\%APACHE_BIN64% %YAJL_URL% %SOURCE_DIR%\%YAJL% %SSDEEP_URL%/%SSDEEP% %SOURCE_DIR%\%SSDEEP% %SSDEEP_URL%/%SSDEEP_BIN% %SOURCE_DIR%\%SSDEEP_BIN%

@if NOT (%ERRORLEVEL%) == (0) goto :failed_to_download
@exit /B 0

:failed_to_download
@echo. && echo Failed to download dependency files... Try again or manually download the files to %SOURCE_DIR% and comment "@call download_files.bat" from build_dependencies.bat
@goto failed

:failed
@exit /B 1

