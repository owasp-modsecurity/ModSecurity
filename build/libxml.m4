heck for LIBXML2 Libraries
dnl Sets:
dnl  LIBXML2_VERSION
dnl  LIBXML2_CFLAGS
dnl  LIBXML2_CPPFLAGS
dnl  LIBXML2_LDADD
dnl  LIBXML2_LDFLAGS

LIBXML2_VERSION=""
LIBXML2_CFLAGS=""
LIBXML2_CPPFLAGS=""
LIBXML2_LDADD=""
LIBXML2_LDFLAGS=""

AC_DEFUN([CHECK_LIBXML2], [

if test "x${with_libxml}" == "xno"; then
    LIBXML2_DISABLED=yes
else
    if test "x${with_libxml}" != "x"; then
        LIBXML2_MANDATORY=yes
    fi
fi

AC_REQUIRE([PKG_PROG_PKG_CONFIG])
PKG_CHECK_MODULES([LIBXML2], [libxml-2.0], [
    LIBXML2_FOUND=1
    LIBXML2_VERSION="`${PKG_CONFIG} libxml-2.0 --modversion`"
    LIBXML2_CFLAGS="`${PKG_CONFIG} libxml-2.0 --cflags -DWITH_LIBXML2`"
    LIBXML2_LDADD="`${PKG_CONFIG} libxml-2.0 --libs`"
    LIBXML2_DISPLAY="${LIBXML2_LDADD}, ${LIBXML2_CFLAGS}"

    AC_MSG_CHECKING([if libxml2 is at least v2.6.29])
    libxml2_min_ver=`echo 2.6.29 | awk -F. '{print (\$ 1 * 1000000) + (\$ 2 * 1000) + \$ 3}'`
    libxml2_ver=`echo ${LIBXML2_VERSION} | awk -F. '{print (\$ 1 * 1000000) + (\$ 2 * 1000) + \$ 3}'`
    if test "$libxml2_ver" -ge "$libxml2_min_ver"; then
        AC_MSG_RESULT([yes, $LIBXML2_VERSION])
        AC_MSG_NOTICE([using libxml2 v${LIBXML2_VERSION}])
    else
        AC_MSG_RESULT([no, $LIBXML2_VERSION])
        AC_MSG_ERROR([NOTE: libxml2 library must be at least 2.6.29])
    fi

], [
    AC_MSG_RESULT([no])
    AC_MSG_NOTICE([*** libxml2 library not found.])
    if test -z "${LIBXML2_MANDATORY}"; then
        if test -z "${LIBXML2_DISABLED}"; then
            LIBXML2_FOUND=0
        else
            LIBXML2_FOUND=2
        fi
    else
        AC_MSG_ERROR([Libxml2 was explicitly referenced but it was not found])
        LIBXML2_FOUND=-1
    fi
])

AC_SUBST(LIBXML2_VERSION)
AC_SUBST(LIBXML2_CFLAGS)
AC_SUBST(LIBXML2_CPPFLAGS)
AC_SUBST(LIBXML2_LDADD)
AC_SUBST(LIBXML2_LDFLAGS)
AC_SUBST(LIBXML2_FOUND)
AC_SUBST(LIBXML2_DISPLAY)
])
