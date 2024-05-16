dnl Check for PCRE2 Libraries
dnl CHECK_PCRE2(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  PCRE2_CFLAGS
dnl  PCRE2_LIBS

PCRE2_CONFIG=""
PCRE2_VERSION=""
PCRE2_CPPFLAGS=""
PCRE2_CFLAGS=""
PCRE2_LDFLAGS=""
PCRE2_LDADD=""
PCRE_LD_PATH=""

AC_DEFUN([CHECK_PCRE2],
[dnl

AC_ARG_WITH(
    pcre2,
    [AS_HELP_STRING([--with-pcre2=PATH],[Path to pcre2 prefix or config script])],
    , with_pcre2=no)

AS_CASE(["${with_pcre2}"],
  [no], [test_paths=],
  [yes], [test_paths="/usr/local/libpcre2 /usr/local/pcre2 /usr/local /opt/libpcre2 /opt/pcre2 /opt /usr"],
  [test_paths="${with_pcre2}"])

if test "x${with_pcre2}" = "x" || test "x${with_pcre2}" = "xno"; then
    AC_MSG_NOTICE([pcre2 not specified; omitting check])
else

    AC_MSG_CHECKING([for libpcre2 config script])

    for x in ${test_paths}; do
        dnl # Determine if the script was specified and use it directly
        if test ! -d "$x" -a -e "$x"; then
            PCRE2_CONFIG=$x
            pcre2_path="no"
            break
        fi

        dnl # Try known config script names/locations
        for PCRE2_CONFIG in pcre2-config; do
            if test -e "${x}/bin/${PCRE2_CONFIG}"; then
                pcre2_path="${x}/bin"
                break
            elif test -e "${x}/${PCRE2_CONFIG}"; then
                pcre2_path="${x}"
                break
            else
                pcre2_path=""
            fi
        done
        if test -n "$pcre2_path"; then
            break
        fi
    done

    if test -n "${pcre2_path}"; then
        if test "${pcre2_path}" != "no"; then
            PCRE2_CONFIG="${pcre2_path}/${PCRE2_CONFIG}"
        fi
        AC_MSG_RESULT([${PCRE2_CONFIG}])
        PCRE2_VERSION="`${PCRE2_CONFIG} --version`"
        if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(pcre2 VERSION: $PCRE2_VERSION); fi
        PCRE2_CFLAGS="`${PCRE2_CONFIG} --cflags`"
        if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(pcre2 CFLAGS: $PCRE2_CFLAGS); fi
        PCRE2_LDADD="`${PCRE2_CONFIG} --libs8`"
        if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(pcre2 LDADD: $PCRE2_LDADD); fi
        PCRE_LD_PATH="/`${PCRE2_CONFIG} --libs8 | cut -d'/' -f2,3,4,5,6 | cut -d ' ' -f1`"
        if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(pcre2 PCRE_LD_PATH: $PCRE_LD_PATH); fi
    else
        AC_MSG_RESULT([no])
    fi

    AC_SUBST(PCRE2_CONFIG)
    AC_SUBST(PCRE2_VERSION)
    AC_SUBST(PCRE2_CPPFLAGS)
    AC_SUBST(PCRE2_CFLAGS)
    AC_SUBST(PCRE2_LDFLAGS)
    AC_SUBST(PCRE2_LDADD)
    AC_SUBST(PCRE_LD_PATH)

    if test -z "${PCRE2_VERSION}"; then
        AC_MSG_NOTICE([*** pcre2 library not found.])
        ifelse([$2], , AC_MSG_ERROR([pcre2 library is required]), $2)
    else
        AC_MSG_NOTICE([using pcre2 v${PCRE2_VERSION}])
        PCRE2_CFLAGS="-DWITH_PCRE2 ${PCRE2_CFLAGS}"
        ifelse([$1], , , $1) 
    fi 
fi
])
