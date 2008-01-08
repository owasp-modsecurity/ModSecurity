dnl Check for APU Libraries
dnl CHECK_APU(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  APU_CFLAGS
dnl  APU_LDFLAGS
dnl  APU_LIBS
dnl  APU_LINK_LD

APU_CONFIG="apu-1-config"
APU_CFLAGS=""
APU_LDFLAGS=""
APU_LIBS=""
APU_LINK_LD=""

AC_DEFUN([CHECK_APU],
[dnl

AC_ARG_WITH(
    apu,
    [AC_HELP_STRING([--with-apu=PATH],[Path to the apu prefix])],
    apu_path="$withval",
    :)

dnl # Determine apu lib directory
if test -z "${apu_path}"; then
    test_paths="/usr/local /usr"
else
    test_paths="${apu_path}"
fi

AC_MSG_CHECKING([for libapr-util config script])
for x in ${test_paths}; do
    if test -e "${x}/bin/${APU_CONFIG}"; then
        with_apu="${x}/bin"
        break
    elif test -e "${x}/${APU_CONFIG}"; then
        with_apu="${x}"
        break
    else
        with_apu=""
    fi
done
if test -n "${with_apu}"; then
    APU_CONFIG="${with_apu}/${APU_CONFIG}"
    AC_MSG_RESULT([${APU_CONFIG}])
    APU_CFLAGS="`${APU_CONFIG} --includes`"
    APU_LDFLAGS="`${APU_CONFIG} --ldflags`"
    APU_LIBS="`${APU_CONFIG} --libs`"
    APU_LINK_LD="`${APU_CONFIG} --link-ld`"
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
  AC_MSG_NOTICE([using '${APU_LINK_LD}' for apu Library])
  ifelse([$1], , , $1) 
fi 
])
