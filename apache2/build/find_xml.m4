dnl Check for XML Libraries
dnl CHECK_LIBXML(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  LIBXML_CFLAGS
dnl  LIBXML_LIBS

LIBXML_CONFIG="xml2-config"
LIBXML_CFLAGS=""
LIBXML_LIBS=""

AC_DEFUN([CHECK_LIBXML],
[dnl

AC_ARG_WITH(
    libxml,
    [AC_HELP_STRING([--with-libxml=PATH],[Path to the libxml2 prefix])],
    libxml_path="$withval",
    :)

dnl # Determine xml lib directory
if test -z "${libxml_path}"; then
    test_paths="/usr/local /usr"
else
    test_paths="${libxml_path}"
fi

AC_MSG_CHECKING([for ${LIBXML_CONFIG}])
for x in ${test_paths}; do
    if test -e "${x}/bin/${LIBXML_CONFIG}"; then
        with_libxml="${x}"
        break
    else
        with_libxml=""
    fi
done
if test -n "${with_libxml}"; then
    LIBXML_CONFIG="${with_libxml}/bin/${LIBXML_CONFIG}"
    AC_MSG_RESULT([${LIBXML_CONFIG}])
    LIBXML_CFLAGS="`${LIBXML_CONFIG} --cflags`"
    LIBXML_LIBS="`${LIBXML_CONFIG} --libs`"
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(LIBXML_LIBS)
AC_SUBST(LIBXML_CFLAGS)

if test -z "${LIBXML_LIBS}"; then
  AC_MSG_NOTICE([*** libxml2 library not found.])
  ifelse([$2], , AC_MSG_ERROR([libxml2 library is required]), $2)
else
  AC_MSG_NOTICE([using '${LIBXML_LIBS}' for libxml Library])
  ifelse([$1], , , $1) 
fi 
])
