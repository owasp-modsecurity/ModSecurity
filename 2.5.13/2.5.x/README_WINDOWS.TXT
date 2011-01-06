============================================================
Build notes for Windows from Tom Donovan
============================================================
These are the raw build notes from Tom Donovan for building
ModSecurity 2.5.13 with Apache httpd 2.2.14 on Windows. Some
day these should be incorporated into the official docs, but
there has not yet been time, so they are included here in 
their raw format for now.
============================================================

I build Apache 2.2.14 from source in C:\work\httpd-2.2.14
I have a VC9 build of Apache 2.2.14 installed in C:\Apache2214

My PATH includes VC9 and CMAKE 2.6

BEFORE BUILDING - if OpenSSL and Zlib support is desired in LIBXML2 and CURL

    REM ####   set an env variable to my Apache build directory
    SET HTTPD_BUILD=C:\work\httpd-2.2.14

    REM  ####   ensure that CURL and LIBXML2 can find the OpenSSL and Zlib includes and libraries that Apache was built with
    SET INCLUDE=%INCLUDE%;%HTTPD_BUILD%\srclib\openssl\inc32;%HTTPD_BUILD%\srclib\zlib
    SET LIB=%LIB%;%HTTPD_BUILD%\srclib\openssl\out32dll;%HTTPD_BUILD%\srclib\zlib

    REM  ####   ensure that CURL doesn't use the static zlib library: zlib.lib. Force it to use zdll.lib instead, which points to zlib1.dll
    IF EXIST %HTTPD_BUILD%\srclib\zlib\zlib.lib  DEL %HTTPD_BUILD%\srclib\zlib\zlib.lib

BUILD PCRE-7.9

    Downloaded pcre-7.9.tar.gz     from ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/
    untar'd into C:\work\  creating C:\work\pcre-7.9

    CD C:\work\pcre-7.9
    CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True
    NMAKE

BUILD LIBXML2-2.7.6

    Downloaded libxml2-2.7.6.tar.gz    from ftp://xmlsoft.org/libxml2/
    untar'd into C:\work\ creating C:\work\libxml2-2.7.6

    CD C:\work\libxml2-2.7.6\win32
    CSCRIPT configure.js iconv=no vcmanifest=yes zlib=yes
    NMAKE -f Makefile.msvc

BUILD LUA-5.1.4

    Downloaded lua-5.1.4.tar.gz from http://www.lua.org/ftp/
    untar'd into C:\work\ creating C:\work\lua-5.1.4

    CD C:\work\lua-5.1.4\src
    CL /Ox /arch:SSE2 /GF /GL /Gy /FD /EHsc /MD  /Zi /TC /wd4005 /D "_MBCS" /D "LUA_CORE" /D "LUA_BUILD_AS_DLL" /D "_CRT_SECURE_NO_WARNINGS" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WIN32" /D "_WINDLL" /c *.c
    DEL lua.obj luac.obj
    LINK /DLL /LTCG /DEBUG /OUT:lua5.1.dll *.obj
    IF EXIST lua5.1.dll.manifest MT  -manifest lua5.1.dll.manifest -outputresource:lua5.1.dll;2

BUILD CURL-7.20.0
    Downloaded curl-7.20.0.tar.gz from http://curl.haxx.se/download.html
    untar'd into C:\work\ creating C:\work\curl-7.20.0

    CD C:\work\curl-7.20.0

    *** Fixed Bug:    https://sourceforge.net/tracker/?func=detail&aid=2951269&group_id=976&atid=100976   ***
    Edited the file  include\curl\curlbuild.h.cmake   near line 160 - put double-quotes around all CURL_FORMAT*  values.
        e.g.  change:   ${CURL_FORMAT_CURL_OFF_T}   to:   "${CURL_FORMAT_CURL_OFF_T}"

                /* curl_off_t formatting string directive without "%" conversion specifier. */
                #cmakedefine CURL_FORMAT_CURL_OFF_T "${CURL_FORMAT_CURL_OFF_T}"

                /* unsigned curl_off_t formatting string without "%" conversion specifier. */
                #cmakedefine CURL_FORMAT_CURL_OFF_TU "${CURL_FORMAT_CURL_OFF_TU}"

                /* curl_off_t formatting string directive with "%" conversion specifier. */
                #cmakedefine CURL_FORMAT_OFF_T "${CURL_FORMAT_OFF_T}"

    CMAKE   -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_SHARED_LIBS=True -DCURL_ZLIB=True

    NMAKE

BUILD MOD_SECURITY-2.5.13

    Edited the top of C:\work\mod_security-2.5.13\apache2\Makefile.win and set my local paths
    (note that pcre.lib is not in $(PCRE)\LibR as it is in the original Makefile.win )

            # Path to Apache httpd installation
            BASE = C:\Apache2214

            # Paths to required libraries
            LIBXML2 = C:\work\libxml2-2.7.6
            LUA = C:\work\lua-5.1.4\src
            PCRE = C:\work\pcre-7.9

            # Linking libraries
            LIBS = $(BASE)\lib\libhttpd.lib \
                   $(BASE)\lib\libapr-1.lib \
                   $(BASE)\lib\libaprutil-1.lib \
                   $(PCRE)\pcre.lib \
                   $(LIBXML2)\win32\bin.msvc\libxml2.lib \
                   $(LUA)\lua5.1.lib \
                   wsock32.lib

    CD C:\work\mod_security-2.5.13\apache2
    NMAKE -f Makefile.win

BUILD MOD_SECURITY-2.5.13 MLOGC program

    Edited the top of C:\work\mod_security-2.5.13\apache2\mlogc-src\Makefile.win and set my local paths

            # Path to Apache httpd installation
            BASE = C:\Apache2214

            # Paths to required libraries
            PCRE = C:\work\pcre-7.9
            CURL = C:\work\curl-7.20.0

            # Linking libraries
            LIBS = $(BASE)\lib\libapr-1.lib \
                   $(BASE)\lib\libaprutil-1.lib \
                   $(PCRE)\pcre.lib \
                   $(CURL)\libcurl_imp.lib \
                   wsock32.lib


    CD  C:\work\mod_security-2.5.13\apache2\mlogc-src
    NMAKE -f Makefile.win

INSTALL AND RUN

    Copied these five files to C:\Apache2214\bin:
        C:\work\pcre-7.9\pcre.dll
        C:\work\lua-5.1.4\src\lua5.1.dll
        C:\work\libxml2-2.7.6\win32\bin.msvc\libxml2.dll
        C:\work\curl-7.20.0\libcurl.dll
        C:\work\mod_security-2.5.13\apache2\mlogc-src\mlogc.exe

    Copied this one file to C:\Apache2214\modules:

        C:\work\mod_security-2.5.13\apache2\mod_security2.so

    You could also copy C:\work\curl-7.20.0\\curl.exe to C:\Apache2214\bin, if you want to use the cURL command-line.

    Downloaded the core rules from http://sourceforge.net/projects/mod-security/files/modsecurity-crs/0-CURRENT/
    and unzipped them in C:\Apache2214\conf\modsecurity_crs

    Added this to my conf\httpd.conf:

        LoadModule unique_id_module modules/mod_unique_id.so
        LoadModule security2_module  modules/mod_security2.so
        <IfModule security2_module>
            Include conf/modsecurity_crs/*.conf
            Include conf/modsecurity_crs/base_rules/*.conf
            SecDataDir   logs
            SecAuditEngine RelevantOnly
            SecAuditLogRelevantStatus "^(?:5|4\d[^4])"
            SecAuditLogType Concurrent
            SecAuditLogParts ABCDEFGHZ
            SecAuditLogStorageDir logs/data/
            SecAuditLog "|bin/mlogc.exe"
        </IfModule>

    My conf\mlogc.conf has this:
        CollectorRoot       "C:/Apache2214/logs"
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


Mod_security appears to work OK with the "ModSecurity Community Console".
