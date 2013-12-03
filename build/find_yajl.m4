dnl Check for YAJL Libraries
dnl CHECK_YAJL(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  YAJL_CFLAGS
dnl  YAJL_LDADD
dnl  YAJL_LDFLAGS
dnl  YAJL_LIBS

AC_DEFUN([CHECK_YAJL],
[dnl

AC_REQUIRE([PKG_PROG_PKG_CONFIG])

YAJL_CONFIG=""
YAJL_VERSION=""
YAJL_CFLAGS=""
YAJL_CPPFLAGS=""
YAJL_LDADD=""
YAJL_LDFLAGS=""
YAJL_CONFIG=${PKG_CONFIG}
YAJL_PKGNAMES="yajl2 yajl"
YAJL_SONAMES="so la sl dll dylib"

AC_ARG_WITH(
    yajl,
    [AC_HELP_STRING([--with-yajl=PATH],[Path to yajl prefix or config script])]
    ,, with_yajl=yes)

AS_CASE(["${with_yajl}"],
  [no], [test_paths=],
  [yes], [test_paths="/usr/local/libyajl /usr/local/yajl /usr/local /opt/libyajl /opt/yajl /opt /usr"],
  [test_paths="${with_yajl}"])

AS_IF([test "x${test_paths}" != "x"], [
AC_MSG_CHECKING([for libyajl config script])
for x in ${test_paths}; do
    dnl # Determine if the script was specified and use it directly
    if test ! -d "$x" -a -e "$x"; then
        YAJL_CONFIG=$x
        break
    fi

    dnl # Try known config script names/locations
    for y in $YAJL_CONFIG; do
        if test -e "${x}/bin/${y}"; then
            YAJL_CONFIG="${x}/bin/${y}"
            yajl_config="${YAJL_CONFIG}"
            break
        elif test -e "${x}/${y}"; then
            YAJL_CONFIG="${x}/${y}"
            yajl_config="${YAJL_CONFIG}"
            break
        fi
    done
    if test -n "${yajl_config}"; then
        break
    fi
done

dnl # Try known package names
if test -n "${YAJL_CONFIG}"; then
    YAJL_PKGNAME=""
    for x in ${YAJL_PKGNAMES}; do
        if ${YAJL_CONFIG} --exists ${x}; then
            YAJL_PKGNAME="$x"
            break
        fi
    done
fi

if test -n "${YAJL_PKGNAME}"; then
    AC_MSG_RESULT([${YAJL_CONFIG}])
    YAJL_VERSION="`${YAJL_CONFIG} ${YAJL_PKGNAME} --modversion`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(yajl VERSION: $YAJL_VERSION); fi
    YAJL_CFLAGS="`${YAJL_CONFIG} ${YAJL_PKGNAME} --cflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(yajl CFLAGS: $YAJL_CFLAGS); fi
    YAJL_LDADD="`${YAJL_CONFIG} ${YAJL_PKGNAME} --libs-only-l`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(yajl LDADD: $YAJL_LDADD); fi
    YAJL_LDFLAGS="`${YAJL_CONFIG} ${YAJL_PKGNAME} --libs-only-L --libs-only-other`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(yajl LDFLAGS: $YAJL_LDFLAGS); fi
else
    AC_MSG_RESULT([no])

    dnl Hack to just try to find the lib and include
    AC_MSG_CHECKING([for yajl install])
    for x in ${test_paths}; do
        for y in ${YAJL_SONAMES}; do
           if test -e "${x}/libyajl.${y}"; then
                yajl_lib_path="${x}/"
                yajl_lib_name="yajl"
                break
            else
                yajl_lib_path=""
                yajl_lib_name=""
            fi
        done
        if test -n "$yajl_lib_path"; then
            break
        fi
    done
    for x in ${test_paths}; do
        if test -e "${x}/include/yajl.h"; then
            yajl_inc_path="${x}/include"
            break
        elif test -e "${x}/yajl.h"; then
            yajl_inc_path="${x}"
            break
        fi

        dnl # Check some sub-paths as well
        for yajl_pkg_name in ${yajl_lib_name} ${YAJL_PKGNAMES}; do
            if test -e "${x}/include/${yajl_pkg_name}/yajl.h"; then
                yajl_inc_path="${x}/include"
                break
            elif test -e "${x}/${yajl_pkg_name}/yajl.h"; then
                yajl_inc_path="${x}"
                break
            else
                yajl_inc_path=""
            fi
        done
        if test -n "$yajl_inc_path"; then
            break
        fi
    done
    if test -n "${yajl_lib_path}" -a -n "${yajl_inc_path}"; then
        YAJL_CONFIG=""
        AC_MSG_RESULT([${yajl_lib_path} ${yajl_inc_path}])
        YAJL_VERSION="2"
        YAJL_CFLAGS="-I${yajl_inc_path}"
        YAJL_LDADD="-l${yajl_lib_name}"
        YAJL_LDFLAGS="-L${yajl_lib_path}"
    else
        YAJL_VERSION=""
        AC_MSG_RESULT([no])
    fi
fi

])

YAJL_LIBS=${YAJL_LDADD}
AC_SUBST(YAJL_CFLAGS)
AC_SUBST(YAJL_LDADD)
AC_SUBST(YAJL_LIBS)
AC_SUBST(YAJL_LDFLAGS)
    if test -z "${YAJL_VERSION}"; then
      ifelse([$2], , AC_MSG_NOTICE([optional yajl library not found]), $2)
    else
      AC_MSG_NOTICE([using yajl v${YAJL_VERSION}])
      YAJL_CFLAGS="-DWITH_YAJL ${YAJL_CFLAGS}"
      ifelse([$1], , , $1) 
    fi 
])
