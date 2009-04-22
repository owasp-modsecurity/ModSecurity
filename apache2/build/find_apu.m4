dnl Check for APU Libraries
dnl CHECK_APU(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  APU_CFLAGS
dnl  APU_LDFLAGS
dnl  APU_LIBS
dnl  APU_LINK_LD

APU_CONFIG=""
APU_CFLAGS=""
APU_LDFLAGS=""
APU_LIBS=""
APU_LINK_LD=""

AC_DEFUN([CHECK_APU],
[dnl

AC_ARG_WITH(
    apu,
    [AC_HELP_STRING([--with-apu=PATH],[Path to apu prefix or config script])],
    [test_paths="${with_apu}"],
    [test_paths="/usr/local/libapr-util /usr/local/apr-util /usr/local/libapu /usr/local/apu /usr/local /opt/libapr-util /opt/apr-util /opt/libapu /opt/apu /opt /usr"])

AC_MSG_CHECKING([for libapu config script])

for x in ${test_paths}; do
    dnl # Determine if the script was specified and use it directly
    if test ! -d "$x" -a -e "$x"; then
        APU_CONFIG="`basename $x`"
        apu_path=`echo $x | sed "s/\/\?${APU_CONFIG}\$//"`
        break
    fi

    dnl # Try known config script names/locations
    for APU_CONFIG in apu-1-mt-config apu-1-config apu-mt-config apu-config; do
        if test -e "${x}/bin/${APU_CONFIG}"; then
            apu_path="${x}/bin"
            break
        elif test -e "${x}/${APU_CONFIG}"; then
            apu_path="${x}"
            break
        else
            apu_path=""
        fi
    done
    if test -n "$apu_path"; then
        break
    fi
done

if test -n "${apu_path}"; then
    APU_CONFIG="${apu_path}/${APU_CONFIG}"
    AC_MSG_RESULT([${APU_CONFIG}])
    APU_CFLAGS="`${APU_CONFIG} --includes`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu CFLAGS: $APU_CFLAGS); fi
    APU_LDFLAGS="`${APU_CONFIG} --ldflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu LDFLAGS: $APU_LDFLAGS); fi
    APU_LIBS="`${APU_CONFIG} --libs`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu LIBS: $APU_LIBS); fi
    APU_LINK_LD="`${APU_CONFIG} --link-ld`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu LINK_LD: $APU_LINK_LD); fi
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(APU_LIBS)
AC_SUBST(APU_CFLAGS)
AC_SUBST(APU_LDFLAGS)
AC_SUBST(APU_LINK_LD)

if test -z "${APU_LIBS}"; then
    AC_MSG_NOTICE([*** apu library not found.])
    ifelse([$2], , AC_MSG_ERROR([apu library is required]), $2)
else
    AC_MSG_NOTICE([using '${APU_LIBS}' for apu Library])
    ifelse([$1], , , $1) 
fi 
])
