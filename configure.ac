# ModSecurity configure.ac

AC_PREREQ([2.69])

# Get the hash of the last commit, to be used if it is not an
# official release.
AC_DEFUN([MSC_GIT_HASH], m4_esyscmd_s(git log -1 --format="%h" --abbrev-commit))
AC_DEFUN([MSC_MAJOR], m4_esyscmd_s(cat headers/modsecurity/modsecurity.h | grep "define MODSECURITY_MAJOR " | awk {'print $3'} | sed 's/\"//g'))
AC_DEFUN([MSC_MINOR], m4_esyscmd_s(cat headers/modsecurity/modsecurity.h | grep "define MODSECURITY_MINOR " | awk {'print $3'} | sed 's/\"//g'))
AC_DEFUN([MSC_PATCHLEVEL], m4_esyscmd_s(cat headers/modsecurity/modsecurity.h | grep "define MODSECURITY_PATCHLEVEL " | awk {'print $3'} | sed 's/\"//g'))
AC_DEFUN([MSC_TAG], m4_esyscmd_s(cat headers/modsecurity/modsecurity.h | grep "define MODSECURITY_FTAG " | awk {'print $3'} | sed 's/\"//g'))


# Version definition to be further used by AC_INIT and 
# .so file naming.
m4_define([msc_version_major], [MSC_MAJOR])
m4_define([msc_version_minor], [MSC_MINOR])
m4_define([msc_version_patchlevel], [MSC_PATCHLEVEL])

m4_define([msc_version_c_plus_a], [m4_eval(msc_version_major + msc_version_minor)])


m4_define([msc_version],
    [msc_version_major.msc_version_minor])

m4_define([msc_version_with_patchlevel],
    [msc_version_major.msc_version_minor.msc_version_patchlevel])

m4_define([msc_version_git],
    [m4_esyscmd_s(git describe)])

m4_define([msc_version_info],
    [msc_version_c_plus_a:msc_version_patchlevel:msc_version_minor])


# Project Information
AC_INIT([modsecurity], [3.0], [security@modsecurity.org])


# General definitions
AC_CONFIG_MACRO_DIR([build])
AC_PREFIX_DEFAULT([/usr/local/modsecurity])


# General automake options.
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])


# Check for dependencies (C++, AR, Lex, Yacc and Make)
AC_PROG_CXX
AM_PROG_AR
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
PKG_PROG_PKG_CONFIG


# Set C++ standard version and check if compiler supports it.
AX_CXX_COMPILE_STDCXX(17, noext, mandatory)

# Check for libinjection
if ! test -f "${srcdir}/others/libinjection/src/libinjection_html5.c"; then
AC_MSG_ERROR([\


  libInjection was not found within ModSecurity source directory.

  libInjection code is available as part of ModSecurity source code in a format
  of a git-submodule. git-submodule allow us to specify the correct version of
  libInjection and still uses the libInjection repository to download it.

  You can download libInjection using git:

     $ git submodule update --init --recursive

   ])
fi
# Libinjection version
AC_DEFUN([LIBINJECTION_VERSION], m4_esyscmd_s(cd "others/libinjection" && git describe && cd ../..))
AC_SUBST([LIBINJECTION_VERSION])

# Check for Mbed TLS
if ! test -f "${srcdir}/others/mbedtls/library/base64.c"; then
AC_MSG_ERROR([\


  Mbed TLS was not found within ModSecurity source directory.

  Mbed TLS code is available as part of ModSecurity source code in a format
  of a git-submodule. git-submodule allow us to specify the correct version of
  Mbed TLS and still uses the Mbed TLS repository to download it.

  You can download Mbed TLS using git:

     $ git submodule update --init --recursive

   ])
fi
# Mbed TLS version
AC_DEFUN([MBEDTLS_VERSION], m4_esyscmd_s(cd "others/mbedtls" && git describe && cd ../..))

# SecLang test version
AC_DEFUN([SECLANG_TEST_VERSION], m4_esyscmd_s(cd "test/test-cases/secrules-language-tests" && git log -1 --format="%h" --abbrev-commit && cd ../../..))


# Check for yajl
PROG_YAJL

AM_CONDITIONAL([YAJL_VERSION], [test "$YAJL_VERSION" != ""])

AC_ARG_WITH([json-c],
    [AS_HELP_STRING([--with-json-c=auto|yyjson|json-c|jansson|cjson|none],
    [Select the C JSON backend (default: auto)])],
    [with_json_c="$withval"], [with_json_c="auto"])

AC_ARG_WITH([json-cxx],
    [AS_HELP_STRING([--with-json-cxx=auto|nlohmannjson|jsoncpp|jsoncons|simdjson|glaze|none],
    [Select the C++ JSON backend (default: auto)])],
    [with_json_cxx="$withval"], [with_json_cxx="auto"])

AC_ARG_WITH([json-prefer],
    [AS_HELP_STRING([--with-json-prefer=system|bundled|auto],
    [Prefer system or bundled JSON dependencies when both are available (default: auto)])],
    [with_json_prefer="$withval"], [with_json_prefer="auto"])

AC_ARG_WITH([json-mode],
    [AS_HELP_STRING([--with-json-mode=auto|dom|events],
    [Preferred internal JSON mode (default: auto)])],
    [with_json_mode="$withval"], [with_json_mode="auto"])

AC_ARG_ENABLE([json-backend-debug],
    [AS_HELP_STRING([--enable-json-backend-debug],
    [Enable JSON backend debug traces and checks])],
    [enable_json_backend_debug="$enableval"], [enable_json_backend_debug="no"])

case "x$with_json_prefer" in
  xsystem|xbundled|xauto) ;;
  *) AC_MSG_ERROR([Invalid value for --with-json-prefer: $with_json_prefer]) ;;
esac

case "x$with_json_mode" in
  xauto|xdom|xevents) ;;
  *) AC_MSG_ERROR([Invalid value for --with-json-mode: $with_json_mode]) ;;
esac

JSON_BACKEND_CFLAGS=""
JSON_BACKEND_LDADD=""
JSON_BACKEND_LDFLAGS=""
JSON_C_BACKEND_SOURCE=""
JSON_CXX_BACKEND_SOURCE=""
JSON_C_BACKEND=""
JSON_CXX_BACKEND="none"

AC_MSG_NOTICE([Resolving C JSON backend (${with_json_c})])
case "x$with_json_c" in
  xauto)
    _msc_json_c_order="yyjson json-c jansson cjson"
    if test "x$with_json_prefer" = "xbundled"; then
      if test -f "${srcdir}/third_party/json-c/include/json-c/json.h"; then
        JSON_C_BACKEND="json-c"
        JSON_C_BACKEND_SOURCE="vendored"
        JSON_BACKEND_CFLAGS="-I\$(top_srcdir)/third_party/json-c/include"
      fi
    fi
    for _msc_cand in $_msc_json_c_order; do
      if test "x$JSON_C_BACKEND" != "x"; then
        break
      fi
      case "$_msc_cand" in
        yyjson)
          MSC_CHECK_LIB([YYJSON], [yyjson], [yyjson.h], [yyjson], [])
          if test "x$YYJSON_FOUND" = "x1"; then
            JSON_C_BACKEND="yyjson"
            JSON_BACKEND_CFLAGS="$YYJSON_CFLAGS"
            JSON_BACKEND_LDFLAGS="$YYJSON_LDFLAGS"
            JSON_BACKEND_LDADD="$YYJSON_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          fi
          ;;
        json-c)
          MSC_CHECK_LIB([JSONC], [json-c], [json-c/json.h], [json-c], [-DWITH_JSON_C])
          if test "x$JSONC_FOUND" = "x1"; then
            JSON_C_BACKEND="json-c"
            JSON_BACKEND_CFLAGS="$JSONC_CFLAGS"
            JSON_BACKEND_LDFLAGS="$JSONC_LDFLAGS"
            JSON_BACKEND_LDADD="$JSONC_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          elif test "x$with_json_prefer" != "xsystem" && test -f "${srcdir}/third_party/json-c/include/json-c/json.h"; then
            JSON_C_BACKEND="json-c"
            JSON_C_BACKEND_SOURCE="vendored"
            JSON_BACKEND_CFLAGS="-I\$(top_srcdir)/third_party/json-c/include"
          fi
          ;;
        jansson)
          MSC_CHECK_LIB([JANSSON], [jansson], [jansson.h], [jansson], [])
          if test "x$JANSSON_FOUND" = "x1"; then
            JSON_C_BACKEND="jansson"
            JSON_BACKEND_CFLAGS="$JANSSON_CFLAGS"
            JSON_BACKEND_LDFLAGS="$JANSSON_LDFLAGS"
            JSON_BACKEND_LDADD="$JANSSON_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          fi
          ;;
        cjson)
          MSC_CHECK_LIB([CJSON], [libcjson cjson], [cjson/cJSON.h], [cjson], [])
          if test "x$CJSON_FOUND" = "x1"; then
            JSON_C_BACKEND="cjson"
            JSON_BACKEND_CFLAGS="$CJSON_CFLAGS"
            JSON_BACKEND_LDFLAGS="$CJSON_LDFLAGS"
            JSON_BACKEND_LDADD="$CJSON_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          fi
          ;;
      esac
    done
    if test "x$JSON_C_BACKEND" = "x"; then
      JSON_C_BACKEND="none"
      JSON_C_BACKEND_SOURCE="disabled"
    fi
    ;;
  xyyjson|xjson-c|xjansson|xcjson)
    _msc_explicit_c="$with_json_c"
    if test "x$with_json_prefer" = "xbundled" && test "x$_msc_explicit_c" = "xjson-c"; then
      if test -f "${srcdir}/third_party/json-c/include/json-c/json.h"; then
        JSON_C_BACKEND="json-c"
        JSON_C_BACKEND_SOURCE="vendored"
        JSON_BACKEND_CFLAGS="-I\$(top_srcdir)/third_party/json-c/include"
      else
        AC_MSG_ERROR([Vendored json-c selected but headers missing: third_party/json-c/include/json-c/json.h])
      fi
    fi
    if test "x$JSON_C_BACKEND" = "x"; then
      case "$_msc_explicit_c" in
        yyjson)
          MSC_CHECK_LIB([YYJSON], [yyjson], [yyjson.h], [yyjson], [])
          if test "x$YYJSON_FOUND" = "x1"; then
            JSON_C_BACKEND="yyjson"
            JSON_BACKEND_CFLAGS="$YYJSON_CFLAGS"
            JSON_BACKEND_LDFLAGS="$YYJSON_LDFLAGS"
            JSON_BACKEND_LDADD="$YYJSON_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          fi
          ;;
        json-c)
          MSC_CHECK_LIB([JSONC], [json-c], [json-c/json.h], [json-c], [-DWITH_JSON_C])
          if test "x$JSONC_FOUND" = "x1"; then
            JSON_C_BACKEND="json-c"
            JSON_BACKEND_CFLAGS="$JSONC_CFLAGS"
            JSON_BACKEND_LDFLAGS="$JSONC_LDFLAGS"
            JSON_BACKEND_LDADD="$JSONC_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          elif test "x$with_json_prefer" != "xsystem" && test -f "${srcdir}/third_party/json-c/include/json-c/json.h"; then
            JSON_C_BACKEND="json-c"
            JSON_C_BACKEND_SOURCE="vendored"
            JSON_BACKEND_CFLAGS="-I\$(top_srcdir)/third_party/json-c/include"
          fi
          ;;
        jansson)
          MSC_CHECK_LIB([JANSSON], [jansson], [jansson.h], [jansson], [])
          if test "x$JANSSON_FOUND" = "x1"; then
            JSON_C_BACKEND="jansson"
            JSON_BACKEND_CFLAGS="$JANSSON_CFLAGS"
            JSON_BACKEND_LDFLAGS="$JANSSON_LDFLAGS"
            JSON_BACKEND_LDADD="$JANSSON_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          fi
          ;;
        cjson)
          MSC_CHECK_LIB([CJSON], [libcjson cjson], [cjson/cJSON.h], [cjson], [])
          if test "x$CJSON_FOUND" = "x1"; then
            JSON_C_BACKEND="cjson"
            JSON_BACKEND_CFLAGS="$CJSON_CFLAGS"
            JSON_BACKEND_LDFLAGS="$CJSON_LDFLAGS"
            JSON_BACKEND_LDADD="$CJSON_LDADD"
            JSON_C_BACKEND_SOURCE="system"
          fi
          ;;
      esac
    fi
    if test "x$JSON_C_BACKEND" = "x"; then
      AC_MSG_ERROR([Requested C JSON backend '$with_json_c' is not available with current --with-json-prefer=${with_json_prefer}.])
    fi
    ;;
  xnone)
    JSON_C_BACKEND="none"
    JSON_C_BACKEND_SOURCE="disabled"
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --with-json-c: $with_json_c])
    ;;
esac

case "x$JSON_C_BACKEND" in
  xyyjson) JSON_C_BACKEND_DEFINE="-DMSC_JSON_C_BACKEND_YYJSON" ;;
  xjson-c) JSON_C_BACKEND_DEFINE="-DMSC_JSON_C_BACKEND_JSONC" ;;
  xjansson) JSON_C_BACKEND_DEFINE="-DMSC_JSON_C_BACKEND_JANSSON" ;;
  xcjson) JSON_C_BACKEND_DEFINE="-DMSC_JSON_C_BACKEND_CJSON" ;;
  xnone) JSON_C_BACKEND_DEFINE="-DMSC_JSON_C_BACKEND_NONE" ;;
esac
if test "x$JSON_C_BACKEND_SOURCE" = "xsystem"; then
  JSON_C_BACKEND_DEFINE="$JSON_C_BACKEND_DEFINE -DMSC_JSON_C_BACKEND_SYSTEM"
elif test "x$JSON_C_BACKEND_SOURCE" = "xvendored"; then
  JSON_C_BACKEND_DEFINE="$JSON_C_BACKEND_DEFINE -DMSC_JSON_C_BACKEND_VENDORED"
fi

AC_MSG_NOTICE([Resolving C++ JSON backend (${with_json_cxx})])
JSON_CXX_BACKEND="none"
JSON_CXX_BACKEND_SOURCE="disabled"
case "x$with_json_cxx" in
  xauto)
    if test "x$with_json_prefer" = "xbundled" && test -f "${srcdir}/third_party/nlohmann_json/include/nlohmann/json.hpp"; then
      JSON_CXX_BACKEND="nlohmannjson"
      JSON_CXX_BACKEND_SOURCE="vendored"
      JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -I\$(top_srcdir)/third_party/nlohmann_json/include"
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone"; then
      AC_CHECK_HEADER([simdjson.h], [JSON_CXX_BACKEND="simdjson"; JSON_CXX_BACKEND_SOURCE="system"])
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone"; then
      AC_CHECK_HEADER([nlohmann/json.hpp], [JSON_CXX_BACKEND="nlohmannjson"; JSON_CXX_BACKEND_SOURCE="system"])
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone"; then
      AC_CHECK_HEADER([jsoncons/json.hpp], [JSON_CXX_BACKEND="jsoncons"; JSON_CXX_BACKEND_SOURCE="system"])
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone"; then
      AC_CHECK_HEADER([json/json.h], [JSON_CXX_BACKEND="jsoncpp"; JSON_CXX_BACKEND_SOURCE="system"])
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone"; then
      AC_CHECK_HEADER([glaze/glaze.hpp], [JSON_CXX_BACKEND="glaze"; JSON_CXX_BACKEND_SOURCE="system"])
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone" && test "x$with_json_prefer" != "xsystem" && test -f "${srcdir}/third_party/nlohmann_json/include/nlohmann/json.hpp"; then
      JSON_CXX_BACKEND="nlohmannjson"
      JSON_CXX_BACKEND_SOURCE="vendored"
      JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -I\$(top_srcdir)/third_party/nlohmann_json/include"
    fi
    ;;
  xsimdjson|xnlohmannjson|xjsoncons|xjsoncpp|xglaze)
    _msc_explicit_cxx="$with_json_cxx"
    if test "x$_msc_explicit_cxx" = "xnlohmannjson"; then
      if test "x$with_json_prefer" = "xbundled"; then
        if test -f "${srcdir}/third_party/nlohmann_json/include/nlohmann/json.hpp"; then
          JSON_CXX_BACKEND="nlohmannjson"
          JSON_CXX_BACKEND_SOURCE="vendored"
          JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -I\$(top_srcdir)/third_party/nlohmann_json/include"
        else
          AC_MSG_ERROR([Vendored nlohmann/json selected but header missing: third_party/nlohmann_json/include/nlohmann/json.hpp])
        fi
      fi
      if test "x$JSON_CXX_BACKEND" = "xnone"; then
        AC_CHECK_HEADER([nlohmann/json.hpp], [JSON_CXX_BACKEND="nlohmannjson"; JSON_CXX_BACKEND_SOURCE="system"])
      fi
      if test "x$JSON_CXX_BACKEND" = "xnone" && test "x$with_json_prefer" != "xsystem" && test -f "${srcdir}/third_party/nlohmann_json/include/nlohmann/json.hpp"; then
        JSON_CXX_BACKEND="nlohmannjson"
        JSON_CXX_BACKEND_SOURCE="vendored"
        JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -I\$(top_srcdir)/third_party/nlohmann_json/include"
      fi
    elif test "x$_msc_explicit_cxx" = "xsimdjson"; then
      AC_CHECK_HEADER([simdjson.h], [JSON_CXX_BACKEND="simdjson"; JSON_CXX_BACKEND_SOURCE="system"])
    elif test "x$_msc_explicit_cxx" = "xjsoncons"; then
      AC_CHECK_HEADER([jsoncons/json.hpp], [JSON_CXX_BACKEND="jsoncons"; JSON_CXX_BACKEND_SOURCE="system"])
    elif test "x$_msc_explicit_cxx" = "xjsoncpp"; then
      AC_CHECK_HEADER([json/json.h], [JSON_CXX_BACKEND="jsoncpp"; JSON_CXX_BACKEND_SOURCE="system"])
    elif test "x$_msc_explicit_cxx" = "xglaze"; then
      AC_CHECK_HEADER([glaze/glaze.hpp], [JSON_CXX_BACKEND="glaze"; JSON_CXX_BACKEND_SOURCE="system"])
    fi
    if test "x$JSON_CXX_BACKEND" = "xnone"; then
      AC_MSG_ERROR([Requested C++ JSON backend '$with_json_cxx' is not available with current --with-json-prefer=${with_json_prefer}.])
    fi
    ;;
  xnone)
    ;;
  *)
    AC_MSG_ERROR([Invalid value for --with-json-cxx: $with_json_cxx])
    ;;
esac

case "x$JSON_CXX_BACKEND" in
  xsimdjson) JSON_CXX_BACKEND_DEFINE="-DMSC_JSON_CXX_BACKEND_SIMDJSON" ;;
  xnlohmannjson) JSON_CXX_BACKEND_DEFINE="-DMSC_JSON_CXX_BACKEND_NLOHMANNJSON" ;;
  xjsoncons) JSON_CXX_BACKEND_DEFINE="-DMSC_JSON_CXX_BACKEND_JSONCONS" ;;
  xjsoncpp) JSON_CXX_BACKEND_DEFINE="-DMSC_JSON_CXX_BACKEND_JSONCPP" ;;
  xglaze) JSON_CXX_BACKEND_DEFINE="-DMSC_JSON_CXX_BACKEND_GLAZE" ;;
  *) JSON_CXX_BACKEND_DEFINE="-DMSC_JSON_CXX_BACKEND_NONE" ;;
esac
if test "x$JSON_CXX_BACKEND_SOURCE" = "xsystem"; then
  JSON_CXX_BACKEND_DEFINE="$JSON_CXX_BACKEND_DEFINE -DMSC_JSON_CXX_BACKEND_SYSTEM"
elif test "x$JSON_CXX_BACKEND_SOURCE" = "xvendored"; then
  JSON_CXX_BACKEND_DEFINE="$JSON_CXX_BACKEND_DEFINE -DMSC_JSON_CXX_BACKEND_VENDORED"
fi

JSON_SUPPORT_ENABLED="no"
JSON_SUPPORT_REASON="none"
if test "x$JSON_C_BACKEND" != "xnone"; then
  JSON_SUPPORT_ENABLED="yes"
  JSON_SUPPORT_REASON="c-backend:${JSON_C_BACKEND}"
elif test "x$JSON_CXX_BACKEND" != "xnone"; then
  JSON_SUPPORT_ENABLED="yes"
  JSON_SUPPORT_REASON="cxx-backend:${JSON_CXX_BACKEND}"
elif test "x$YAJL_FOUND" = "x1"; then
  JSON_SUPPORT_ENABLED="yes"
  JSON_SUPPORT_REASON="legacy-yajl"
fi

if test "x$JSON_SUPPORT_ENABLED" = "xyes"; then
  AC_DEFINE([WITH_JSON_SUPPORT], [1], [Define if a JSON backend is enabled for JSON generation])
  JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS $JSON_C_BACKEND_DEFINE $JSON_CXX_BACKEND_DEFINE -DWITH_JSON_SUPPORT"
else
  JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS $JSON_C_BACKEND_DEFINE $JSON_CXX_BACKEND_DEFINE"
fi

AC_SUBST([JSON_BACKEND_CFLAGS])
AC_SUBST([JSON_BACKEND_LDADD])
AC_SUBST([JSON_BACKEND_LDFLAGS])
AC_SUBST([JSON_C_BACKEND])
AC_SUBST([JSON_C_BACKEND_SOURCE])
AC_SUBST([JSON_CXX_BACKEND])
AC_SUBST([JSON_CXX_BACKEND_SOURCE])
AC_SUBST([JSON_SUPPORT_ENABLED])
AC_SUBST([JSON_SUPPORT_REASON])
AC_SUBST([JSON_PREFER_MODE], [$with_json_prefer])
AC_SUBST([JSON_OPERATING_MODE], [$with_json_mode])

if test "x$enable_json_backend_debug" = "xyes"; then
  JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -DMSC_JSON_BACKEND_DEBUG=1"
  AC_DEFINE([MSC_JSON_BACKEND_DEBUG], [1], [Define to enable JSON backend debug logging])
fi

case "x$with_json_mode" in
  xdom)
    JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -DMSC_JSON_MODE_DOM=1"
    ;;
  xevents)
    JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -DMSC_JSON_MODE_EVENTS=1"
    ;;
  *)
    JSON_BACKEND_CFLAGS="$JSON_BACKEND_CFLAGS -DMSC_JSON_MODE_AUTO=1"
    ;;
esac

# Check for LibGeoIP
PROG_GEOIP

# Check for MaxMind
PROG_MAXMIND

# Check for LMDB
PROG_LMDB

# Check for SSDEEP
CHECK_SSDEEP

# Check for LUA
CHECK_LUA


#
# Check for curl
#
CHECK_CURL

if ! test -z "${CURL_VERSION}"; then
  AC_DEFINE([MSC_WITH_CURL], [1], [Define if libcurl is available])
fi


#
# Check for LibXML
#
CHECK_LIBXML2


#
# Check for libpcre only if explicitly requested
#
if test "x${with_pcre}" != "x" && test "x${with_pcre}" != "xno"; then
  CHECK_PCRE
else
  #
  # Check for pcre2
  #
  PROG_PCRE2
fi


# Checks for header files.
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([sys/utsname.h])
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h libintl.h malloc.h netdb.h netinet/in.h stdint.h sys/ioctl.h sys/param.h sys/socket.h sys/time.h unistd.h])



# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_TYPES([time_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([alarm clock_gettime gethostname gettimeofday inet_ntoa localtime_r memmove memset mkdir select setenv socket strcasecmp strchr strdup strerror strncasecmp strspn strstr strtol strtoul strtoull uname])

# Initialize libtool
LT_INIT

# Identify platform
AC_CANONICAL_HOST

case $host in
  *-*-aix*)
    echo "Checking platform... Identified as AIX"
    AC_DEFINE([AIX], [1], [Define if the operating system is AIX])
    PLATFORM="AIX"
    ;;
  *-*-hpux*)
    echo "Checking platform... Identified as HPUX"
    AC_DEFINE([HPUX], [1], [Define if the operating system is HPUX])
    PLATFORM="HPUX"
    ;;
  *-*-darwin*)
    echo "Checking platform... Identified as Macintosh OS X"
    AC_DEFINE([MACOSX], [1], [Define if the operating system is Macintosh OSX])
    PLATFORM="MacOSX"
    ;;
  *-*-linux* | *-*uclinux*)
    echo "Checking platform... Identified as Linux"
    AC_DEFINE([LINUX], [1], [Define if the operating system is LINUX])
    PLATFORM="Linux"
    ;;
  *-*-solaris*)
    echo "Checking platform... Identified as Solaris"
    AC_DEFINE([SOLARIS], [1], [Define if the operating system is SOLARIS])
    PLATFORM="Solaris"
    ;;
  *-*-freebsd*)
    echo "Checking platform... Identified as FreeBSD"
    AC_DEFINE([FREEBSD], [1], [Define if the operating system is FREEBSD])
    PLATFORM="FreeBSD"
    ;;
  *-*-netbsd*)
    echo "Checking platform... Identified as NetBSD"
    AC_DEFINE([NETBSD], [1], [Define if the operating system is NETBSD])
    PLATFORM="NetBSD"
    ;;
  *-*-openbsd*)
    echo "Checking platform... Identified as OpenBSD"
    AC_DEFINE([OPENBSD], [1], [Define if the operating system is OPENBSD])
    PLATFORM="OpenBSD"
    ;;
  *-*-kfreebsd*)
    echo "Checking platform... Identified as kFreeBSD, treating as linux"
    AC_DEFINE([FREEBSD], [1], [Define if the operating system is FREEBSD])
    PLATFORM="kFreeBSD"
    ;;
  *-*-dragonfly*)
    echo "Checking platform... Identified as DragonFlyBSD, treating as linux"
    AC_DEFINE([DRAGONFLY], [1], [Define if the operating system is DRAGONFLY])
    PLATFORM="DragonFly"
    ;;
  *-*-gnu*.*)
    echo "Checking platform... Identified as HURD, treating as linux"
    AC_DEFINE([LINUX], [1], [Define if the operating system is LINUX])
    PLATFORM="HURD"
    ;;
       *)
    echo "Unknown CANONICAL_HOST $host"
    exit 1
    ;;
esac

# Variables to be used inside the Makefile.am files.
MSC_BASE_DIR=`pwd`
AC_SUBST([MSC_BASE_DIR])

MSC_VERSION_INFO=msc_version_info
AC_SUBST([MSC_VERSION_INFO])

MSC_VERSION_WITH_PATCHLEVEL=msc_version_with_patchlevel
AC_SUBST([MSC_VERSION_WITH_PATCHLEVEL])

MSC_VERSION=msc_version
AC_SUBST([MSC_VERSION])

MSC_GIT_VERSION=msc_version_git
AC_SUBST([MSC_GIT_VERSION])

MSC_ARG_ENABLE_BOOL([assertions], [Turn on assertions feature: undefine NDEBUG], [false], [assertions])
MSC_ARG_ENABLE_BOOL([debug-logs], [Turn off the SecDebugLog feature], [true], [debugLogs])
if test "$debugLogs" != "true"; then
    MODSEC_NO_LOGS="-DNO_LOGS=1"
    AC_SUBST(MODSEC_NO_LOGS)
fi

MSC_ARG_ENABLE_BOOL([afl-fuzz], [Turn on the afl fuzzer compilation utilities], [false], [aflFuzzer])
MSC_ARG_ENABLE_BOOL([examples], [Turn on the examples compilation (default option)], [true], [buildExamples])
MSC_ARG_ENABLE_BOOL([parser-generation], [Enables parser generation during the build], [false], [buildParser])
MSC_ARG_ENABLE_BOOL([mutex-on-pm], [Treat pm operations as critical section], [false], [mutexPm])


if test $buildParser = true; then
    AC_PROG_YACC
    AC_PROG_LEX(noyywrap)
    AC_PATH_PROG([FLEX], [flex])
    test "x$FLEX" = "x" && AC_MSG_ERROR([flex is needed to build ModSecurity])

    AC_PATH_PROG([BISON], [bison])
    test "x$BISON" = "x" && AC_MSG_ERROR([bison is needed to build ModSecurity])

    AC_PATH_PROG([YACC_INST], $YACC)
    if test ! -f "$srcdir/gram.c"; then
        if test -z "$YACC_INST"; then
            AC_MSG_ERROR([yacc not found - unable to compile ModSecurity])
        fi
    fi
fi


# Decide if we want to build the tests or not.
buildTestUtilities=false
if test "x$YAJL_FOUND" = "x1"; then
    # Regression tests will not be able to run without the logging support.
    # But we still have the unit tests.
    # if test "$debugLogs" = "true"; then
        buildTestUtilities=true
    # fi
fi


AM_CONDITIONAL([TEST_UTILITIES], [test $buildTestUtilities = true])
if test $buildTestUtilities = true; then
    if test $debugLogs = true; then
        if test -f ./test/test-list.sh; then
            TEST_CASES=`./test/test-list.sh`
        fi
    fi
fi

AM_CONDITIONAL([EXAMPLES], [test $buildExamples = true])
AM_CONDITIONAL([BUILD_PARSER], [test $buildParser = true])
AM_CONDITIONAL([USE_MUTEX_ON_PM], [test $mutexPm = true])


# General link options
if test "$PLATFORM" != "MacOSX" -a "$PLATFORM" != "OpenBSD"; then
    GLOBAL_LDADD="-lrt  "
fi

if test "$aflFuzzer" == "true"; then
    FUZZ_CPPCFLAGS="-fsanitize=address -fsanitize-coverage=4 "
    GLOBAL_LDADD="$GLOBAL_LDADD -fsanitize=address "
    GLOBAL_CPPFLAGS="$GLOBAL_CPPFLAGS $FUZZ_CPPCFLAGS"
    $buildExamples = false
fi

case $assertions in
    false) ASSERTIONS_CPPCFLAGS="-DNDEBUG" ;;
    true) ASSERTIONS_CPPCFLAGS="-UNDEBUG" ;;
    *) AC_MSG_ERROR(bad value ${assertions} for assertions) ;;
esac
GLOBAL_CPPFLAGS="$GLOBAL_CPPFLAGS $ASSERTIONS_CPPCFLAGS"

AC_SUBST(GLOBAL_LDADD)
AC_SUBST(GLOBAL_CPPFLAGS)

AM_CONDITIONAL([AFL_FUZZER], [test $aflFuzzer = true])

GLOBAL_CFLAGS=""
AC_SUBST(GLOBAL_CFLAGS)

# Files to be generated via autotools.
AC_CONFIG_FILES([\
    modsecurity.pc \
    Makefile \
    doc/Makefile \
    src/Makefile \
    others/Makefile \
    tools/Makefile \
    tools/rules-check/Makefile
    ])

AM_COND_IF([TEST_UTILITIES],
       [AC_CONFIG_FILES([test/Makefile test/benchmark/Makefile])])

AM_COND_IF([EXAMPLES],
       [AC_CONFIG_FILES([ \
            examples/Makefile \
            examples/simple_example_using_c/Makefile \
            examples/multiprocess_c/Makefile \
            examples/multithread/Makefile \
            examples/reading_logs_with_offset/Makefile \
            examples/reading_logs_via_rule_message/Makefile \
            examples/using_bodies_in_chunks/Makefile \
            ])])

AM_COND_IF([AFL_FUZZER],
       [AC_CONFIG_FILES([test/fuzzer/Makefile])])

AM_COND_IF([BUILD_PARSER],
       [AC_CONFIG_FILES([src/parser/Makefile])])


AC_CONFIG_HEADERS([src/config.h])


# Doxygen support
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([ModSecurity],[doc/doxygen.cfg])

# make check-valgrind
AX_VALGRIND_DFLT([sgcheck], [off])
AX_VALGRIND_CHECK

# Generate the files.
AC_OUTPUT


# Print a fancy summary
echo " "
echo " "
echo "ModSecurity - ${MSC_GIT_VERSION} for $PLATFORM"
echo " "
echo " Mandatory dependencies"
AS_ECHO_N("   + libInjection                                  ....")
echo LIBINJECTION_VERSION
AS_ECHO_N("   + Mbed TLS                                      ....")
echo MBEDTLS_VERSION
AS_ECHO_N("   + SecLang tests                                 ....")
echo SECLANG_TEST_VERSION

echo " "
echo " Optional dependencies"



## GeoIP - MaxMind (combined display)
if test "x$GEOIP_FOUND" = "x0" && test "x$MAXMIND_FOUND" = "x0"; then
    echo "   + GeoIP/MaxMind                                 ....not found"
fi
if test "x$GEOIP_FOUND" = "x1" || test "x$MAXMIND_FOUND" = "x1"; then
    AS_ECHO_N(["   + GeoIP/MaxMind                                 ....found "])
    echo ""
    if test "x$MAXMIND_FOUND" = "x1"; then
        echo "      * (MaxMind) v${MAXMIND_VERSION}"
        echo "         ${MAXMIND_DISPLAY}"
    fi
    if test "x$GEOIP_FOUND" = "x1"; then
        echo "      * (GeoIP) v${GEOIP_VERSION}"
        echo "         ${GEOIP_DISPLAY}"
    fi
fi
if test "x$GEOIP_FOUND" = "x2" && test "x$MAXMIND_FOUND" = "x2"; then
    echo "   + GeoIP/MaxMind                                 ....disabled"
fi

MSC_STATUS_LIB([LibCURL                                      ], [CURL])
MSC_STATUS_LIB([YAJL                                         ], [YAJL])
echo "   + JSON C backend                                 ....${JSON_C_BACKEND} (${JSON_C_BACKEND_SOURCE})"
echo "   + JSON C++ backend                               ....${JSON_CXX_BACKEND} (${JSON_CXX_BACKEND_SOURCE})"
echo "   + JSON backend                                   ....${JSON_C_BACKEND} (${JSON_C_BACKEND_SOURCE})"
echo "   + JSON C++ backend (effective)                   ....${JSON_CXX_BACKEND} (${JSON_CXX_BACKEND_SOURCE})"
echo "   + JSON support (overall)                         ....${JSON_SUPPORT_ENABLED} (${JSON_SUPPORT_REASON})"
echo "   + JSON preference                                ....${with_json_prefer}"
echo "   + JSON mode                                      ....${with_json_mode}"
if test "x$enable_json_backend_debug" = "xyes"; then
    echo "   + JSON backend debug                             ....enabled"
else
    echo "   + JSON backend debug                             ....disabled"
fi
MSC_STATUS_LIB([LMDB                                         ], [LMDB])
MSC_STATUS_LIB([LibXML2                                      ], [LIBXML2])
MSC_STATUS_LIB([SSDEEP                                       ], [SSDEEP])
MSC_STATUS_LIB([LUA                                          ], [LUA])

## PCRE (only shown when explicitly requested)
if test "x${with_pcre}" != "x" && test "x${with_pcre}" != "xno"; then
    if test -n "${PCRE_VERSION}"; then
        echo "   + PCRE                                          ....found "
        echo "     using pcre v${PCRE_VERSION}"
        echo "      ${PCRE_LDADD}, ${PCRE_CFLAGS}"
    else
        AC_MSG_NOTICE([*** pcre library not found.])
    fi
fi

MSC_STATUS_LIB([PCRE2                                         ], [PCRE2])

echo " "
echo " Other Options"
if test $buildTestUtilities = true; then
    if test $debugLogs = true; then
        echo "   + Test Utilities                                ....enabled"
    else
        echo "   + Test Utilities                                ....partially"
    fi
else
    echo "   + Test Utilities                                ....disabled"
fi
if test $assertions = true; then
    echo "   + Assertions                                    ....enabled"
else
    echo "   + Assertions                                    ....disabled"
fi
if test $debugLogs = true; then
    echo "   + SecDebugLog                                   ....enabled"
else
    echo "   + SecDebugLog                                   ....disabled"
fi

if test "$aflFuzzer" = "true"; then
    echo "   + afl fuzzer                                    ....enabled"
    echo "    ($FUZZ_CPPCFLAGS)"
else
    echo "   + afl fuzzer                                    ....disabled"
fi

if test "$buildExamples" = "true"; then
    echo "   + library examples                              ....enabled"
else
    echo "   + library examples                              ....disabled"
fi

if test "$buildParser" = "true"; then
    echo "   + Building parser                               ....enabled"
else
    echo "   + Building parser                               ....disabled"
fi

if test "$mutexPm" = "true"; then
    echo "   + Treating pm operations as critical section    ....enabled"
else
    echo "   + Treating pm operations as critical section    ....disabled"
fi


echo " "


if test "$aflFuzzer" = "true"; then
    echo "WARNING: afl fuzzer was enabled. Make sure you are using the"
    echo " 'afl-clang-fast' as the compiler, otherwise the compilation"
    echo "  will fail."
    echo " "
    echo " You can set the compiler using:"
    echo " "
    echo "  $ export CXX=afl-clang-fast++ "
    echo "  $ export CC=afl-clang-fast "
    echo " "
fi
