set PATH=%PATH%;c:\work\cmake-2.8.7-win32-x86\bin;"c:\program files\7-zip"

rem set BUILD_APACHE=YES
set HTTPD=httpd-2.2.22
IF NOT DEFINED BUILD_APACHE SET HTTPD=httpd-2.4.6

set WORK=c:\work
set APACHE=c:\Apache22
set PCRE=pcre-8.33
set ZLIB=zlib-1.2.8
set LIBXML2=libxml2-2.9.1
set LUA=lua-5.1.5
set CURL=curl-7.33.0
