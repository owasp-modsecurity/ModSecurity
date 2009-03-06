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
    [AC_HELP_STRING([--with-apr=PATH],[Path to apr prefix or config script])],
    apr_path="${withval}",
    :)

AC_MSG_CHECKING([for libapr config script])

dnl # Determine if the script was specified and use it directly
if test ! -d "${withval}" -a -e "${withval}"; then
    APR_CONFIG="`basename $withval`"
    with_apr=`echo ${withval} | sed "s/\/\?${APR_CONFIG}\$//"`
fi

dnl # Look for the config script
if test -z "${with_apr}"; then
    dnl # Determine apr lib directory
    if test -z "${apr_path}"; then
        test_paths="/usr/local/apr /usr/local /usr"
    else
        test_paths="${apr_path}"
    fi

    for x in ${test_paths}; do
        for APR_CONFIG in apr-1-mt-config apr-1-config apr-mt-config apr-config; do
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
        if test -n "$with_apr"; then
            break
        fi
    done
fi

if test -n "${with_apr}"; then
    APR_CONFIG="${with_apr}/${APR_CONFIG}"
    AC_MSG_RESULT([${APR_CONFIG}])
    APR_CFLAGS="`${APR_CONFIG} --includes --cppflags --cflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apr CFLAGS: $APR_CFLAGS); fi
    APR_LDFLAGS="`${APR_CONFIG} --ldflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apr LDFLAGS: $APR_LDFLAGS); fi
    APR_LIBS="`${APR_CONFIG} --libs`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apr LIBS: $APR_LIBS); fi
    APR_LINK_LD="`${APR_CONFIG} --link-ld`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apr LINK_LD: $APR_LINK_LD); fi
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
