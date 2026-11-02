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

AC_DEFUN([CHECK_PCRE2],
[dnl

dnl This macro runs for both PCRE1 and PCRE2 selections.
dnl Linux Apache builds may opt in to a trusted, explicit runtime directory.
dnl Validate as a single shell-safe, absolute directory. It need not exist on
dnl the build host: staged installs and cross-compilation may use target paths.
AC_ARG_WITH([pcre-rpath],
    [AS_HELP_STRING([--with-pcre-rpath=DIR],
        [explicit PCRE/PCRE2 runtime search directory for Linux (default: none)])],
    [if test -z "$withval"; then
        AC_MSG_ERROR([--with-pcre-rpath requires an absolute directory or 'no'])
     fi
     pcre_rpath_dir="$withval"],
    [pcre_rpath_dir=""])
PCRE_RPATH_FLAGS=""
case "$pcre_rpath_dir" in
    ""|no) ;;
    /*)
        clean_pcre_rpath=`printf '%s' "$pcre_rpath_dir" | LC_ALL=C tr -cd 'A-Za-z0-9_./+-'`
        if test "x$clean_pcre_rpath" != "x$pcre_rpath_dir"; then
            AC_MSG_ERROR([--with-pcre-rpath requires one absolute directory without whitespace or shell metacharacters])
        fi
        PCRE_RPATH_FLAGS="-R $pcre_rpath_dir"
        ;;
    *) AC_MSG_ERROR([--with-pcre-rpath must be an absolute directory or 'no']) ;;
esac
AC_SUBST([PCRE_RPATH_FLAGS])

AC_ARG_WITH(
    pcre2,
    [AS_HELP_STRING([--with-pcre2=PATH],[Path to pcre2 prefix or config script])],
    [test_paths="${with_pcre2}"],
    [with_pcre2="yes"])

AS_CASE(["${with_pcre2}"],
    [no], [test_paths=],
    [yes], [test_paths="/usr/local/libpcre2 /usr/local/pcre2 /usr/local /opt/libpcre2 /opt/pcre2 /opt /usr"],
    [test_paths="${with_pcre2}"])

if test "x${with_pcre}" != "x" && test "x${with_pcre}" != "xno"; then
    AC_MSG_NOTICE([pcre specified; omitting check for pcre2])
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
        dnl Do not derive a runtime search directory from PCRE2 linker flags.
    else
        AC_MSG_RESULT([no])
    fi

    AC_SUBST(PCRE2_CONFIG)
    AC_SUBST(PCRE2_VERSION)
    AC_SUBST(PCRE2_CPPFLAGS)
    AC_SUBST(PCRE2_CFLAGS)
    AC_SUBST(PCRE2_LDFLAGS)
    AC_SUBST(PCRE2_LDADD)

    if test -z "${PCRE2_VERSION}"; then
        AC_MSG_NOTICE([*** pcre2 library not found.])
        ifelse([$2], , AC_MSG_ERROR([pcre2 library is required]), $2)
    else
        AC_MSG_NOTICE([using pcre2 v${PCRE2_VERSION}])
        PCRE2_CFLAGS="${PCRE2_CFLAGS}"
        ifelse([$1], , , $1)
    fi
fi
])
