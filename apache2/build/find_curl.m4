dnl Check for CURL Libraries
dnl CHECK_CURL(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  CURL_CFLAGS
dnl  CURL_LIBS

CURL_CONFIG="curl-config"
CURL_CFLAGS=""
CURL_LIBS=""

AC_DEFUN([CHECK_CURL],
[dnl

AC_ARG_WITH(
    curl,
    [AC_HELP_STRING([--with-curl=PATH],[Path to the curl prefix])],
    curl_path="$withval",
    :)

dnl # Determine curl lib directory
if test -z "${curl_path}"; then
    test_paths="/usr/local /usr"
else
    test_paths="${curl_path}"
fi

AC_MSG_CHECKING([for libcurl config script])
for x in ${test_paths}; do
    if test -e "${x}/bin/${CURL_CONFIG}"; then
        with_curl="${x}/bin"
        break
    else
        with_curl=""
    fi
done
if test -n "${with_curl}"; then
    CURL_CONFIG="${with_curl}/${CURL_CONFIG}"
    AC_MSG_RESULT([${CURL_CONFIG}])
    CURL_CFLAGS="`${CURL_CONFIG} --cflags`"
    CURL_LIBS="`${CURL_CONFIG} --libs`"
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(CURL_LIBS)
AC_SUBST(CURL_CFLAGS)

if test -z "${CURL_LIBS}"; then
  AC_MSG_NOTICE([*** curl library not found.])
  ifelse([$2], , AC_MSG_NOTICE([NOTE: curl library is required for building mlogc]), $2)
else
  AC_MSG_NOTICE([using '${CURL_LIBS}' for curl Library])
  ifelse([$1], , , $1) 
fi 
])
