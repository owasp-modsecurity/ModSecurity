# ModSecurity configure.ac


# Version definition to be further used by AC_INIT and 
# .so file naming.
m4_define([msc_version_major], [3])
m4_define([msc_version_minor], [0])
m4_define([msc_version_patchlevel], [0])

m4_define([msc_version],
    [msc_version_major.msc_version_minor])

m4_define([msc_version_info],
    [msc_version_major:msc_version_minor:msc_version_patchlevel])


# Project Information
AC_INIT([modsecurity], [msc_version], [security@modsecurity.org])


# General definitions
AC_CONFIG_MACRO_DIR([build])
AC_PREFIX_DEFAULT([/usr/local/modsecurity])


# General automake options.
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])


# Check for dependencies (C++, AR, Lex, Yacc and Make)
AC_PROG_CXX
AM_PROG_AR
AC_PROG_YACC
AC_PROG_LEX
AC_PROG_MAKE_SET

# Check for yajl
PROG_YAJL

AM_CONDITIONAL([YAJL_VERSION], [test "$YAJL_VERSION" != ""])

# Check for libgeoip
PROG_GEOIP

AM_CONDITIONAL([GEOIP_VERSION], [test "$GEOIP_VERSION" != ""])

#
# Check for curl
#
CHECK_CURL

if test -z "${CURL_VERSION}"; then
  AC_MSG_NOTICE([NOTE: curl was not found. SecRemoteRules support was disabled.])
else
  AC_DEFINE([MSC_WITH_CURL], [1], [Define if libcurl is available])
fi


#
# Check for libpcre
#
CHECK_PCRE



# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([sys/utsname.h])


# ??
LT_INIT([dlopen])

# Identify platform
AC_CANONICAL_HOST

case $host in
  *-*-aix*)
    echo "Checking platform... Identified as AIX"
    AC_DEFINE([AIX], [1], [Define if the operating system is AIX])
    ;;
  *-*-hpux*)
    echo "Checking platform... Identified as HPUX"
    AC_DEFINE([HPUX], [1], [Define if the operating system is HPUX])
    ;;
  *-*-darwin*)
    echo "Checking platform... Identified as Macintosh OS X"
    AC_DEFINE([MACOSX], [1], [Define if the operating system is Macintosh OSX])
    ;;
  *-*-linux*)
    echo "Checking platform... Identified as Linux"
    AC_DEFINE([LINUX], [1], [Define if the operating system is LINUX])
    ;;
  *-*-solaris*)
    echo "Checking platform... Identified as Solaris"
    AC_DEFINE([SOLARIS], [1], [Define if the operating system is SOLARIS])
    ;;
  *-*-freebsd*)
    echo "Checking platform... Identified as FreeBSD"
    AC_DEFINE([FREEBSD], [1], [Define if the operating system is FREEBSD])
    ;;
  *-*-netbsd*)
    echo "Checking platform... Identified as NetBSD"
    AC_DEFINE([NETBSD], [1], [Define if the operating system is NETBSD])
    ;;
  *-*-openbsd*)
    echo "Checking platform... Identified as OpenBSD"
    AC_DEFINE([OPENBSD], [1], [Define if the operating system is OPENBSD])
    ;;
  *-*-kfreebsd*)
    echo "Checking platform... Identified as kFreeBSD, treating as linux"
    AC_DEFINE([FREEBSD], [1], [Define if the operating system is FREEBSD])
    ;;
  *-*-gnu*.*)
    echo "Checking platform... Identified as HURD, treating as linux"
    AC_DEFINE([LINUX], [1], [Define if the operating system is LINUX])
    ;;
       *)
    echo "Unknown CANONICAL_HOST $host"
    exit
    ;;
esac

# Variables to be used inside the Makefile.am files.
MSC_BASE_DIR=`pwd`
AC_SUBST([MSC_BASE_DIR])

MSC_VERSION_INFO=msc_version_info
AC_SUBST([MSC_VERSION_INFO])



# Files to be generated via autotools.
AC_CONFIG_FILES([\
    Makefile \
    doc/Makefile \
    src/Makefile \
    test/Makefile \
    test/benchmark/Makefile \
    examples/Makefile \
    examples/simple_example_using_c/Makefile \
    ])

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


# Generate the files.
AC_OUTPUT


