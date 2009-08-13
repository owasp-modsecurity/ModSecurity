dnl Check for PCRE Libraries
dnl CHECK_PCRE(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  PCRE_CFLAGS
dnl  PCRE_LIBS

PCRE_CONFIG=""
PCRE_CFLAGS=""
PCRE_LIBS=""

AC_DEFUN([CHECK_PCRE],
[dnl

AC_ARG_WITH(
    pcre,
    [AC_HELP_STRING([--with-pcre=PATH],[Path to pcre prefix or config script])],
    [test_paths="${with_pcre}"],
    [test_paths="/usr/local/libpcre /usr/local/pcre /usr/local /opt/libpcre /opt/pcre /opt /usr"])

AC_MSG_CHECKING([for libpcre config script])

dnl # Determine pcre lib directory
if test -z "${with_pcre}"; then
    test_paths="/usr/local/pcre /usr/local /usr"
else
    test_paths="${with_pcre}"
fi

for x in ${test_paths}; do
    dnl # Determine if the script was specified and use it directly
    if test ! -d "$x" -a -e "$x"; then
        PCRE_CONFIG=$x
        pcre_path="no"
        break
    fi

    dnl # Try known config script names/locations
    for PCRE_CONFIG in pcre-config; do
        if test -e "${x}/bin/${PCRE_CONFIG}"; then
            pcre_path="${x}/bin"
            break
        elif test -e "${x}/${PCRE_CONFIG}"; then
            pcre_path="${x}"
            break
        else
            pcre_path=""
        fi
    done
    if test -n "$pcre_path"; then
        break
    fi
done
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS

if test -n "${pcre_path}"; then
    if test "${pcre_path}" != "no"; then
        PCRE_CONFIG="${pcre_path}/${PCRE_CONFIG}"
    fi
    AC_MSG_RESULT([${PCRE_CONFIG}])
    PCRE_CFLAGS="`${PCRE_CONFIG} --cflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(pcre CFLAGS: $PCRE_CFLAGS); fi
    PCRE_LIBS="`${PCRE_CONFIG} --libs`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(pcre LIBS: $PCRE_LIBS); fi
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(PCRE_LIBS)
AC_SUBST(PCRE_CFLAGS)

if test -z "${PCRE_LIBS}"; then
    AC_MSG_NOTICE([*** pcre library not found.])
    ifelse([$2], , AC_MSG_ERROR([pcre library is required]), $2)
else
    AC_MSG_NOTICE([using '${PCRE_LIBS}' for pcre Library])
    ifelse([$1], , , $1) 
fi 
])
