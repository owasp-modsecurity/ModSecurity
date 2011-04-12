=====================================================================
MOD_SECURITY 2.6  Command-line Build notes for Windows       4/2/2011
by Tom Donovam
=====================================================================

PREREQUISITES:

    Microsoft Visual Studio C++                                                     tested with Visual Studio 2008 (aka VC9)

    CMake build system from:   http://www.cmake.org/                                tested with CMake v2.8.0

    Apache 2.2.x  from:        http://httpd.apache.org/                             tested with Apache 2.2.17
        Apache must be built from source using the same Visual Studio compiler as mod_security.

    PCRE  Perl Compatible Regular Expression library from: http://www.pcre.org/     tested with PCRE v8.12

    LibXML2 from: http://xmlsoft.org/                       tested with LibXML2 v2.7.7
        Note that LibXML2 v2.7.8 does not build correctly for Windows

    Lua Scripting Language from:  http://www.lua.org/       tested with Lua v5.1.4

    cURL multiprotocol file transfer library from: http://curl.haxx.se/             tested with cURL v7.21.4


BEFORE BUILDING

The directory where you build software from source ( C:\work in this exmaple)
must contain the Apache source you used to build the Apache web serverand the mod_security source

    Apache source is in             C:\work\httpd-2.2.17    in this example.
    Apache has been installed to    C:\Apache2217           in this example.
    Mod_security source is in       C:\work\mod_security    in this example.

Download and untar the prerequite library sources:

    Download pcre-8.12.tar.gz     from ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/
    untar it into C:\work\  creating C:\work\pcre-8.12

    Download libxml2-2.7.7.tar.gz    from ftp://xmlsoft.org/libxml2/
    untar it into C:\work\ creating C:\work\libxml2-2.7.7

    Download lua-5.1.4.tar.gz from http://www.lua.org/ftp/
    untar it into C:\work\ creating C:\work\lua-5.1.4

    Download curl-7.21.4.tar.gz from http://curl.haxx.se/download.html
    untar it into C:\work\ creating C:\work\curl-7.21.4

Setup your build environment:

    The PATH environment variable must include the Visual Studio variables as set by vsvars32.bat
    The PATH environment variable must also include the CMAKE bin\ directory

    Set an environment variable to the Apache source code directory:

        SET HTTPD_BUILD=C:\work\httpd-2.2.17

    If OpenSSL and Zlib support were included when you built Apache 2.2, and you want them available to LIBXML2 and CURL

        Ensure that cURL and libXML2 can find the OpenSSL and Zlib includes and libraries that Apache was built with.

            SET INCLUDE=%INCLUDE%;%HTTPD_BUILD%\srclib\openssl\inc32;%HTTPD_BUILD%\srclib\zlib
            SET LIB=%LIB%;%HTTPD_BUILD%\srclib\openssl\out32dll;%HTTPD_BUILD%\srclib\zlib

        Ensure that cURL and libXML2 don't use the static zlib library: zlib.lib.
        Force cURL and libXML2 to use zdll.lib instead, requiring zlib1.dll at runtime:

            IF EXIST %HTTPD_BUILD%\srclib\zlib\zlib.lib  DEL %HTTPD_BUILD%\srclib\zlib\zlib.lib

BUILD PCRE-8.12

    CD C:\work\pcre-8.12
    CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
    NMAKE

BUILD LIBXML2-2.7.7  (note: the more recent version: 2.7.8 does not build correctly on Windows)

    CD C:\work\libxml2-2.7.7\win32
    CSCRIPT configure.js iconv=no vcmanifest=yes zlib=yes
    NMAKE -f Makefile.msvc

BUILD LUA-5.1.4

    CD C:\work\lua-5.1.4\src
    CL /Ox /arch:SSE2 /GF /GL /Gy /FD /EHsc /MD  /Zi /TC /wd4005 /D "_MBCS" /D "LUA_CORE" /D "LUA_BUILD_AS_DLL" /D "_CRT_SECURE_NO_WARNINGS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WIN32" /D "_WINDLL" /c *.c
    DEL lua.obj luac.obj
    LINK /DLL /LTCG /DEBUG /OUT:lua5.1.dll *.obj
    IF EXIST lua5.1.dll.manifest MT  -manifest lua5.1.dll.manifest -outputresource:lua5.1.dll;2

BUILD CURL-7.21.4

    CD C:\work\curl-7.21.4
    CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DCURL_ZLIB=True
    NMAKE

BUILD MOD_SECURITY-2.6

    CD C:\work\mod_security\apache2
    NMAKE -f Makefile.win APACHE=C:\Apache2217 PCRE=C:\work\pcre-8.12 LIBXML2=C:\work\libxml2-2.7.7 LUA=C:\work\lua-5.1.4\src

INSTALL MOD_SECURITY AND RUN APACHE

Copy these five files to C:\Apache2217\bin:
    C:\work\pcre-8.12\pcre.dll C:\Apache2217\bin\
    C:\work\lua-5.1.4\src\lua5.1.dll C:\Apache2217\bin\
    C:\work\libxml2-2.7.7\win32\bin.msvc\libxml2.dll  C:\Apache2217\bin\
    C:\work\curl-7.21.4\libcurl.dll  C:\Apache2217\bin\
    C:\work\mod_security\apache2\mlogc-src\mlogc.exe

Copy this one file to C:\Apache2217\modules:

    C:\work\mod_security\apache2\mod_security2.so

You may also copy C:\work\curl-7.21.4\curl.exe to C:\Apache2217\bin, if you want to use the cURL command-line program.

Download the core rules from http://sourceforge.net/projects/mod-security/files/modsecurity-crs/0-CURRENT/
and unzip them into C:\Apache2217\conf\modsecurity_crs

Add configuration directives to your Apache conf\httpd.conf:

    # mod_security requires mod_unique_id
    LoadModule unique_id_module modules/mod_unique_id.so

    # mod_security
    LoadModule security2_module  modules/mod_security2.so
    <IfModule security2_module>
        SecRuleEngine On
        SecDataDir   logs
        Include conf/modsecurity_crs/*.conf
        Include conf/modsecurity_crs/base_rules/*.conf
        SecAuditEngine RelevantOnly
        SecAuditLogRelevantStatus "^(?:5|4\d[^4])"
        SecAuditLogType Serial
        SecAuditLogParts ABCDEFGHZ
        SecAuditLog logs/modsecurity.log
    </IfModule>


==============================================================================================
OPTIONAL:   BUILD AND CONFIGURE THE MOD_SECURITY-2.6 MLOGC piped-logging program

Edit the top of C:\work\mod_security\apache2\mlogc-src\Makefile.win and set your local paths

        # Path to Apache httpd installation
        BASE = C:\Apache2217

        # Paths to required libraries
        PCRE = C:\work\pcre-8.12
        CURL = C:\work\curl-7.21.4

        # Linking libraries
        LIBS = $(BASE)\lib\libapr-1.lib \
               $(BASE)\lib\libaprutil-1.lib \
               $(PCRE)\pcre.lib \
               $(CURL)\libcurl_imp.lib \
               wsock32.lib

Build the mlogc.exe program:

        CD  C:\work\mod_security_trunk\mlogc
        NMAKE -f Makefile.win

Copy mlocg.exe to C:\Apache2217\bin\

Create a new command file C:\Apache2217\bin\mlogc.bat with one line:

        C:\Apache2217\bin\mlogc.exe C:\Apache2217\conf\mlogc.conf

Create a new configuration file C:\Apache2217\conf\mlogc.conf to control the piped-logging program mlogc.exe.
Here is an example conf\mlogc.conf:

    CollectorRoot       "C:/Apache2217/logs"
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

Change the SecAuditLog directive in conf\httpd.conf to pipe the log data to mlogc
instead of writing them to a file:

    SecAuditLog |C:/Apache2217/bin/mlogc.bat
