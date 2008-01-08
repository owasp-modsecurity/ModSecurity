dnl Check for APR Libraries
dnl CHECK_APR(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  APR_CFLAGS
dnl  APR_LDFLAGS
dnl  APR_LIBS
dnl  APR_LINK_LD

APR_CONFIG=""
APR_CFLAGS=""
APR_LDFLAGS=""
APR_LIBS=""
APR_LINK_LD=""

AC_DEFUN([CHECK_APR],
[dnl

AC_ARG_WITH(
    apr,
    [AC_HELP_STRING([--with-apr=PATH],[Path to the apr prefix])],
    apr_path="$withval",
    :)

dnl # Determine apr lib directory
if test -z "${apr_path}"; then
    test_paths="/usr/local /usr"
else
    test_paths="${apr_path}"
fi

AC_MSG_CHECKING([for libapr config script])
for x in ${test_paths}; do
    for APR_CONFIG in apr-config apr-1-config; do
        if test -e "${x}/bin/${APR_CONFIG}"; then
            with_apr="${x}/bin"
            break
        elif test -e "${x}/${APR_CONFIG}"; then
            with_apr="${x}"
            break
        else
            with_apr=""
        fi
    done
done
if test -n "${with_apr}"; then
    APR_CONFIG="${with_apr}/${APR_CONFIG}"
    AC_MSG_RESULT([${APR_CONFIG}])
    APR_CFLAGS="`${APR_CONFIG} --includes`"
    APR_LDFLAGS="`${APR_CONFIG} --ldflags`"
    APR_LIBS="`${APR_CONFIG} --libs`"
    APR_LINK_LD="`${APR_CONFIG} --link-ld`"
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(APR_LIBS)
AC_SUBST(APR_CFLAGS)
AC_SUBST(APR_LDFLAGS)
AC_SUBST(APR_LINK_LD)

if test -z "${APR_LIBS}"; then
  AC_MSG_NOTICE([*** apr library not found.])
  ifelse([$2], , AC_MSG_ERROR([apr library is required]), $2)
else
  AC_MSG_NOTICE([using '${APR_LIBS}' for apr Library])
  ifelse([$1], , , $1) 
fi 
])
