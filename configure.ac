# ModSecurity configure.ac


# Get the hash of the last commit, to be used if it is not an
# official release.
AC_DEFUN([MSC_GIT_HASH], m4_esyscmd_s(git log -1 --format="%h" --abbrev-commit))


# Version definition to be further used by AC_INIT and 
# .so file naming.
m4_define([msc_version_major], [3])
m4_define([msc_version_minor], [0])
m4_define([msc_version_patchlevel], [0])

m4_define([msc_version],
    [msc_version_major.msc_version_minor])

m4_define([msc_version_git],
    [msc_version_major.msc_version_minor.msc_version_patchlevel+MSC_GIT_HASH])

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


# Check if the compiler is c++11 compatible.
AX_CXX_COMPILE_STDCXX_11(,mandatory)

# Check for libinjection
AC_CHECK_FILE("others/libinjection/src/libinjection_html5.c", HAS_LIBINJECTION=1)
test "x$HAS_LIBINJECTION" = "x" && AC_MSG_ERROR([\


  libInjection was not found within ModSecurity source directory.

  libInjection code is available as part of ModSecurity source code in a format
  of a git-submodule. git-submodule allow us to specify the correct version of
  libInjection and still uses the libInjection repository to download it.

  You can download libInjection using git:

     $ git submodule init
     $ git submodule update

   ])
# Libinjection version
AC_DEFUN([LIBINJECTION_VERSION], m4_esyscmd_s(PWD="others/libinjection" git describe))

# SecLang test version
AC_DEFUN([SECLANG_TEST_VERSION], m4_esyscmd_s(PWD="test/test-cases/secrules-language-tests" git log -1 --format="%h" --abbrev-commit))


# Check for yajl
PROG_YAJL

AM_CONDITIONAL([YAJL_VERSION], [test "$YAJL_VERSION" != ""])

# Check for LibGeoIP
PROG_GEOIP
AM_CONDITIONAL([GEOIP_CFLAGS], [test "GEOIP_CFLAGS" != ""])


#
# Check for curl
#
CHECK_CURL

if ! test -z "${CURL_VERSION}"; then
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

MSC_VERSION=msc_version
AC_SUBST([MSC_VERSION])

MSC_GIT_VERSION=msc_version_git
AC_SUBST([MSC_GIT_VERSION])


# MODSEC_NO_LOGS="-DNO_LOGS=1"
# AC_SUBST(MODSEC_NO_LOGS)


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


# Print a fancy summary
echo " "
echo " "
echo "ModSecurity - v${MSC_GIT_VERSION}"
echo " "
echo " Mandatory dependencies"
echo -n "   + libInjection                                  ...."
echo LIBINJECTION_VERSION
echo -n "   + SecLang tests                                 ...."
echo SECLANG_TEST_VERSION

echo " "
echo " Optional dependencies"
if test "x$GEOIP_FOUND" = "x0"; then
    echo "   + GeoIP                                         ....not found"
fi
if test "x$GEOIP_FOUND" = "x1"; then
    echo -n "   + GeoIP                                         ....found "
    if ! test "x$GEOIP_VERSION" = "x"; then
        echo "v${GEOIP_VERSION}"
    else
        echo ""
    fi
    echo "      ${GEOIP_DISPLAY}"
fi
if test "x$GEOIP_FOUND" = "x2"; then
    echo "   + GeoIP                                         ....disabled"
fi
echo " "