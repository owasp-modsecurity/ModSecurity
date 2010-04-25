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
APU_LDADD=""

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
        APU_CONFIG=$x
        apu_path="no"
        break
    fi

    dnl # Try known config script names/locations
    for APU_CONFIG in apu-1-mt-config apu-1-config apu-config-1 apu-mt-config-1 apu-mt-config apu-config; do
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
    if test "${apu_path}" != "no"; then
        APU_CONFIG="${apu_path}/${APU_CONFIG}"
    fi
    AC_MSG_RESULT([${APU_CONFIG}])
    APU_VERSION="`${APU_CONFIG} --version`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu VERSION: $APU_VERSION); fi
    APU_CFLAGS="`${APU_CONFIG} --includes`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu CFLAGS: $APU_CFLAGS); fi
    APU_LDFLAGS="`${APU_CONFIG} --libs`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu LDFLAGS: $APU_LDFLAGS); fi
    APU_LDADD="`${APU_CONFIG} --link-libtool`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(apu LDADD: $APU_LDADD); fi
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(APU_CONFIG)
AC_SUBST(APU_VERSION)
AC_SUBST(APU_CFLAGS)
AC_SUBST(APU_LDFLAGS)
AC_SUBST(APU_LDADD)

if test -z "${APU_VERSION}"; then
    AC_MSG_NOTICE([*** apu library not found.])
    ifelse([$2], , AC_MSG_ERROR([apu library is required]), $2)
else
    AC_MSG_NOTICE([using apu v${APU_VERSION}])
    ifelse([$1], , , $1) 
fi 
])
