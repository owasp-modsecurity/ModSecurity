dnl Check for PCRE Libraries
dnl CHECK_PCRE(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  PCRE_CFLAGS
dnl  PCRE_LIBS

PCRE_CONFIG="pcre-config"
PCRE_CFLAGS=""
PCRE_LIBS=""

AC_DEFUN([CHECK_PCRE],
[dnl

AC_ARG_WITH(
    pcre,
    [AC_HELP_STRING([--with-pcre=PATH],[Path to the pcre prefix])],
    pcre_path="$withval",
    :)

dnl # Determine pcre lib directory
if test -z "${pcre_path}"; then
    test_paths="/usr/local /usr"
else
    test_paths="${pcre_path}"
fi

AC_MSG_CHECKING([for libpcre config script])
for x in ${test_paths}; do
    if test -e "${x}/bin/${PCRE_CONFIG}"; then
        with_pcre="${x}/bin"
        break
    elif test -e "${x}/${PCRE_CONFIG}"; then
        with_pcre="${x}"
        break
    else
        with_pcre=""
    fi
done
if test -n "${with_pcre}"; then
    PCRE_CONFIG="${with_pcre}/${PCRE_CONFIG}"
    AC_MSG_RESULT([${PCRE_CONFIG}])
    PCRE_CFLAGS="`${PCRE_CONFIG} --cflags`"
    PCRE_LIBS="`${PCRE_CONFIG} --libs`"
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
