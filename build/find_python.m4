dnl Check for PYTHON Libraries
dnl Sets:


AC_DEFUN([CHECK_PYTHON],
[dnl

AC_REQUIRE([PKG_PROG_PKG_CONFIG])

PYTHON_CONFIG=""
PYTHON_VERSION=""
PYTHON_CFLAGS=""
PYTHON_CPPFLAGS=""
PYTHON_LDADD=""
PYTHON_LDFLAGS=""
PYTHON_CONFIG=${PKG_CONFIG}
PYTHON_PKGNAMES="python3"
PYTHON_SONAMES="so la sl dll dylib"

AC_ARG_WITH(
    python,
    [AC_HELP_STRING([--with-python=PATH],[Path to Python prefix or config script])]
    ,, with_python=yes)

AS_CASE(["${with_pythonl}"],
  [no], [test_paths=],
  [yes], [test_paths="/usr/local/libpython /usr/local/python /usr/local /opt/libpython /opt/python /opt /usr"],
  [test_paths="${with_python}"])

AS_IF([test "x${test_paths}" != "x"], [
AC_MSG_CHECKING([for Python config script])
for x in ${test_paths}; do
    dnl # Determine if the script was specified and use it directly
    if test ! -d "$x" -a -e "$x"; then
        PYTHON_CONFIG=$x
        break
    fi

    dnl # Try known config script names/locations
    for y in $PYTHON_CONFIG; do
        if test -e "${x}/bin/${y}"; then
            PYTHON_CONFIG="${x}/bin/${y}"
            python_config="${PYTHON_CONFIG}"
            break
        elif test -e "${x}/${y}"; then
            PYTHON_CONFIG="${x}/${y}"
            python_config="${PYTHON_CONFIG}"
            break
        fi
    done
    if test -n "${python_config}"; then
        break
    fi
done

dnl # Try known package names
if test -n "${PYTHON_CONFIG}"; then
    PYTHON_PKGNAME=""
    for x in ${PYTHON_PKGNAMES}; do
        if ${PYTHON_CONFIG} --exists ${x}; then
            PYTHON_PKGNAME="$x"
            break
        fi
    done
fi

if test -n "${PYTHON_PKGNAME}"; then
    AC_MSG_RESULT([${PYTHON_CONFIG}])
    PYTHON_VERSION="`${PYTHON_CONFIG} ${PYTHON_PKGNAME} --modversion`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(python VERSION: $PYTHON_VERSION); fi
    PYTHON_CFLAGS="`${PYTHON_CONFIG} ${PYTHON_PKGNAME} --cflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(python CFLAGS: $PYTHON_CFLAGS); fi
    PYTHON_LDADD="`${PYTHON_CONFIG} ${PYTHON_PKGNAME} --libs-only-l`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(python LDADD: $PYTHON_LDADD); fi
    PYTHON_LDFLAGS="`${PYTHON_CONFIG} ${PYTHON_PKGNAME} --libs-only-L --libs-only-other`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(python LDFLAGS: $PYTHON_LDFLAGS); fi
else
    AC_MSG_RESULT([no])

    dnl Hack to just try to find the lib and include
    AC_MSG_CHECKING([for python install])
    for x in ${test_paths}; do
        for y in ${PYTHON_SONAMES}; do
           if test -e "${x}/libpython.${y}"; then
                python_lib_path="${x}/"
                python_lib_name="python"
                break
            else
                python_lib_path=""
                python_lib_name=""
            fi
        done
        if test -n "$python_lib_path"; then
            break
        fi
    done
    for x in ${test_paths}; do
        if test -e "${x}/include/Python.h"; then
            python_inc_path="${x}/include"
            break
        elif test -e "${x}/Python.h"; then
            python_inc_path="${x}"
            break
        fi

        dnl # Check some sub-paths as well
        for python_pkg_name in ${python_lib_name} ${PYTHON_PKGNAMES}; do
            if test -e "${x}/include/${python_pkg_name}/Python.h"; then
                python_inc_path="${x}/include"
                break
            elif test -e "${x}/${python_pkg_name}/Python.h"; then
                python_inc_path="${x}"
                break
            else
                python_inc_path=""
            fi
        done
        if test -n "$python_inc_path"; then
            break
        fi
    done
    if test -n "${python_lib_path}" -a -n "${python_inc_path}"; then
        PYTHON_CONFIG=""
        AC_MSG_RESULT([${python_lib_path} ${python_inc_path}])
        PYTHON_VERSION="2"
        PYTHON_CFLAGS="-I${python_inc_path}"
        PYTHON_LDADD="-l${python_lib_name}"
        PYTHON_LDFLAGS="-L${python_lib_path}"
    else
        PYTHON_VERSION=""
        AC_MSG_RESULT([no])
    fi
fi

])

PYTHON_LIBS=${PYTHON_LDADD}
AC_SUBST(PYTHON_CFLAGS)
AC_SUBST(PYTHON_LDADD)
AC_SUBST(PYTHON_LIBS)
AC_SUBST(PYTHON_LDFLAGS)
    if test -z "${PYTHON_VERSION}"; then
      ifelse([$2], , AC_MSG_NOTICE([optional python library not found]), $2)
    else
      AC_MSG_NOTICE([using python v${PYTHON_VERSION}])
      PYTHON_CFLAGS="-DWITH_PYTHON ${PYTHON_CFLAGS}"
      ifelse([$1], , , $1) 
    fi 
])
