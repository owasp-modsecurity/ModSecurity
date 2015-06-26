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

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string])
AC_CHECK_HEADERS([iostream])


# ??
LT_INIT([dlopen])


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


