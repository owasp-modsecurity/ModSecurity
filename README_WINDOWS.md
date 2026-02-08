
## ModSecurity 2.x  Command-line build notes for Windows       

by Tom Donovam, 4/2/2011


## Prerequisites:

Dependency | Tested with | Note
----|------|----
Microsoft Visual Studio C++ | Visual Studio 2019 (aka VS16) |
[CMake build system](http://www.cmake.org/) | CMake v4.2.3 |
[Apache 2.4.x](http://httpd.apache.org/) | Apache 2.4.66 | Apache must be built from source using the same Visual Studio compiler as mod_security.
[PCRE2, Perl Compatible Regular Expression library](https://www.pcre.org/) | PCRE2 v10.47 | ModSecurity v2 now uses PCRE2 by default (not legacy PCRE)
[LibXML2](http://xmlsoft.org/) | LibXML2 v2.15.1 |
[Lua Scripting Language](http://www.lua.org/) | Lua v5.4.8
[cURL multiprotocol file transfer library](http://curl.haxx.se/) | cURL v8.18.0
[zlib compression library](https://zlib.net/) | zlib v1.3.1


## Before building

The directory where you build software from source ( ``C:\work`` in this example)
must contain the Apache source you used to build the Apache web server and the mod_security source

    Apache source is in             C:\work\httpd-2.4.66    in this example.
    Apache has been installed to    C:\Apache2466           in this example.
    Mod_security source is in       C:\work\mod_security    in this example.

## Download and untar the prerequisite library sources:

    Download pcre2-10.47.zip from https://github.com/PCRE2Project/pcre2/releases/download/pcre2-10.47/
    unzip it into C:\work\  creating C:\work\pcre2-10.47

    Download libxml2-2.15.1.tar.gz from https://download.gnome.org/sources/libxml2/2.15/
    untar it into C:\work\ creating C:\work\libxml2-2.15.1

    Download lua-5.4.8.tar.gz from http://www.lua.org/ftp/
    untar it into C:\work\ creating C:\work\lua-5.4.8

    Download curl-8.18.0.zip from https://curl.se/download/
    unzip it into C:\work\ creating C:\work\curl-8.18.0

    Download zlib-1.3.1.tar.gz from https://github.com/madler/zlib/releases/download/v1.3.1/
    untar it into C:\work\ creating C:\work\zlib-1.3.1

## Setup your build environment:

1. The ``PATH`` environment variable must include the Visual Studio variables as set by ``vsvars32.bat``

2. The ``PATH`` environment variable must also include the CMAKE ``bin\`` directory

3. Set an environment variable to the Apache source code directory:

```
    SET HTTPD_BUILD=C:\work\httpd-2.4.66
```

### Optional:

If OpenSSL and zlib support were included when you built Apache 2.4, and you want them available to LibXML2 and cURL

1. Ensure that cURL and LibXML2 can find the OpenSSL and zlib includes and libraries that Apache was built with.

```
    SET INCLUDE=%INCLUDE%;%HTTPD_BUILD%\srclib\openssl\inc32;%HTTPD_BUILD%\srclib\zlib
    SET LIB=%LIB%;%HTTPD_BUILD%\srclib\openssl\out32dll;%HTTPD_BUILD%\srclib\zlib
```

2. Ensure that cURL and libXML2 don't use the static zlib library: ``zlib.lib``. Force cURL and libXML2 to use ``zdll.lib`` instead, requiring ``zlib1.dll`` at runtime:

```
    IF EXIST %HTTPD_BUILD%\srclib\zlib\zlib.lib  DEL %HTTPD_BUILD%\srclib\zlib\zlib.lib
```

## Build

### PCRE2-10.47

    CD C:\work\pcre2-10.47
    CMAKE -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DPCRE2_BUILD_PCRE2_8=ON -DPCRE2_SUPPORT_JIT=ON
    NMAKE

Note: PCRE2 with JIT support provides better performance and fixes stack overflow issues present in older PCRE versions.

### LibXML2-2.15.1

    CD C:\work\libxml2-2.15.1\win32
    CSCRIPT configure.js iconv=no vcmanifest=yes zlib=yes
    NMAKE -f Makefile.msvc

### Lua-5.4.8

    CD C:\work\lua-5.4.8\src
    CL /Ox /arch:SSE2 /GF /GL /Gy /FD /EHsc /MD  /Zi /TC /wd4005 /D "_MBCS" /D "LUA_CORE" /D "LUA_BUILD_AS_DLL" /D "_CRT_SECURE_NO_WARNINGS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WIN32" /D "_WINDLL" /c *.c
    DEL lua.obj luac.obj
    LINK /DLL /LTCG /DEBUG /OUT:lua54.dll *.obj
    IF EXIST lua54.dll.manifest MT  -manifest lua54.dll.manifest -outputresource:lua54.dll;2

### cURL-8.18.0

    CD C:\work\curl-8.18.0\winbuild
    SET ARCH=x64
    NMAKE /f Makefile.vc mode=dll ENABLE_WINSSL=yes MACHINE=%ARCH% WITH_ZLIB=dll

### ModSecurity-2.9.x

    CD C:\work\mod_security\apache2
    NMAKE -f Makefile.win APACHE=C:\Apache2466 PCRE=C:\work\pcre2-10.47 LIBXML2=C:\work\libxml2-2.15.1 LUA=C:\work\lua-5.4.8\src CURL=C:\work\curl-8.18.0

## Install ModSecurity and run Apache

Copy these files to ``C:\Apache2466\bin``:

    C:\work\pcre2-10.47\pcre2-8.dll C:\Apache2466\bin\
    C:\work\lua-5.4.8\src\lua54.dll C:\Apache2466\bin\
    C:\work\libxml2-2.15.1\win32\bin.msvc\libxml2.dll C:\Apache2466\bin\
    C:\work\curl-8.18.0\builds\libcurl-vc-x64-release-dll-zlib-dll-ipv6-sspi-schannel-obj-lib\libcurl.dll C:\Apache2466\bin\
    C:\work\mod_security\apache2\mlogc-src\mlogc.exe C:\Apache2466\bin\

Copy this one file to ``C:\Apache2466\modules``:

    C:\work\mod_security\apache2\mod_security2.so

You may also copy ``C:\work\curl-8.18.0\curl.exe`` to ``C:\Apache2466\bin``, if you want to use the cURL command-line program.

Download OWASP CRS from https://github.com/coreruleset/coreruleset/releases/latest and unzip it into ``C:\Apache2466\conf\owasp_crs``

For example, download ``coreruleset-4.x.x.zip``, extract it, and rename the extracted directory to ``owasp_crs``.

Add configuration directives to your Apache conf\httpd.conf:

    # mod_security requires mod_unique_id
    LoadModule unique_id_module modules/mod_unique_id.so

    # mod_security
    LoadModule security2_module  modules/mod_security2.so
    <IfModule security2_module>
        SecRuleEngine On
        SecDataDir   logs
        # Include OWASP CRS configuration
        Include conf/owasp_crs/crs-setup.conf
        Include conf/owasp_crs/rules/*.conf
        SecAuditEngine RelevantOnly
        SecAuditLogRelevantStatus "^(?:5|4\d[^4])"
        SecAuditLogType Serial
        SecAuditLogParts ABCDEFGHZ
        SecAuditLog logs/modsecurity.log
    </IfModule>

## Optional: Build and configure the ModSecurity-2.x MLOGC piped-logging program

Edit the top of ``C:\work\mod_security\apache2\mlogc-src\Makefile.win`` and set your local paths

        # Path to Apache httpd installation
        BASE = C:\Apache2466

        # Paths to required libraries
        PCRE = C:\work\pcre2-10.47
        CURL = C:\work\curl-8.18.0

        # Linking libraries
        LIBS = $(BASE)\lib\libapr-1.lib \
               $(BASE)\lib\libaprutil-1.lib \
               $(PCRE)\pcre2-8.lib \
               $(CURL)\libcurl.lib \
               wsock32.lib

Build the ``mlogc.exe`` program:

        CD  C:\work\mod_security_trunk\mlogc
        NMAKE -f Makefile.win

Copy ``mlocg.exe`` to ``C:\Apache2466\bin\``

Create a new command file ``C:\Apache2466\bin\mlogc.bat`` with one line:

        C:\Apache2466\bin\mlogc.exe C:\Apache2466\conf\mlogc.conf

Create a new configuration file ``C:\Apache2466\conf\mlogc.conf`` to control the piped-logging program ``mlogc.exe``.
Here is an example ``conf\mlogc.conf``:

    CollectorRoot       "C:/Apache2466/logs"
    ConsoleURI          "https://localhost:8888/rpc/auditLogReceiver"
    SensorUsername      "test"
    SensorPassword      "testtest"
    LogStorageDir       "data"
    TransactionLog      "mlogc-transaction.log"
    QueuePath           "mlogc-queue.log"
    ErrorLog            "mlogc-error.log"
    LockFile            "mlogc.lck"
    KeepEntries         0
    ErrorLogLevel       2
    MaxConnections      10
    MaxWorkerRequests   1000
    TransactionDelay    50
    StartupDelay        5000
    CheckpointInterval  15
    ServerErrorTimeout  60

Change the SecAuditLog directive in ``conf\httpd.conf`` to pipe the log data to mlogc instead of writing them to a file:

    SecAuditLog |C:/Apache2466/bin/mlogc.bat
