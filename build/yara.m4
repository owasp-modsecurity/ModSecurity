dnl Check for YARA Libraries
dnl CHECK_YARA(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])


AC_DEFUN([CHECK_YARA],
[dnl

# Possible names for the yara library/package (pkg-config)
YARA_POSSIBLE_LIB_NAMES="yara"

# Possible extensions for the library
YARA_POSSIBLE_EXTENSIONS="so so0 la sl dll dylib so.0.0.0"

# Possible paths (if pkg-config was not found, proceed with the file lookup)
YARA_POSSIBLE_PATHS="/usr/lib /usr/local/lib /usr/local/yara /usr/local/libyara /usr/local /opt /usr /usr/lib64 /opt/local"

# Variables to be set by this very own script.
YARA_CFLAGS=""
YARA_LDFLAGS=""
YARA_LDADD=""
YARA_DISPLAY=""

AC_ARG_WITH(
    yara,
    AC_HELP_STRING(
    [--with-yara=PATH],
    [Path to yara prefix]
    )
)


if test "x${with_yara}" == "xno"; then
    AC_DEFINE(HAVE_YARA, 0, [Support for YARA was disabled by the utilization of --without-yara or --with-yara=no])
    AC_MSG_NOTICE([Support for YARA was disabled by the utilization of --without-yara or --with-yara=no])
    YARA_DISABLED=yes
else
    if test "x${with_yara}" == "xyes"; then
        YARA_MANDATORY=yes
        AC_MSG_NOTICE([YARA support was marked as mandatory by the utilization of --with-yara=yes])
    else
        YARA_MANDATORY=no
    fi
    for x in ${YARA_POSSIBLE_PATHS}; do
        CHECK_FOR_YARA_AT(${x})
        if test -n "${YARA_CFLAGS}"; then
            break
        fi
    done
fi


if test -z "${YARA_CFLAGS}"; then
    if test -z "${YARA_MANDATORY}" || test "x${YARA_MANDATORY}" == "xno"; then
        if test -z "${YARA_DISABLED}"; then
            AC_MSG_NOTICE([YARA library was not found])
            YARA_FOUND=0
        else
            YARA_FOUND=2
        fi
    else
        AC_MSG_ERROR([YARA was explicitly referenced but it was not found])
        YARA_FOUND=-1
    fi
else
    YARA_FOUND=1
    AC_MSG_NOTICE([using YARA v${YARA_VERSION}])
    YARA_CFLAGS="-DWITH_YARA ${YARA_CFLAGS}"
    YARA_DISPLAY="${YARA_LDADD} ${YARA_LDFLAGS}, ${YARA_CFLAGS}"
    AC_SUBST(YARA_LDFLAGS)
    AC_SUBST(YARA_LDADD)
    AC_SUBST(YARA_CFLAGS)
    AC_SUBST(YARA_DISPLAY)
fi


AC_SUBST(YARA_FOUND)

]) # AC_DEFUN [CHECK_YARA]


AC_DEFUN([CHECK_FOR_YARA_AT], [
    path=$1
    echo "*** LOOKING AT PATH: " ${path}
    for y in ${YARA_POSSIBLE_EXTENSIONS}; do
        for z in ${YARA_POSSIBLE_LIB_NAMES}; do
           if test -e "${path}/${z}.${y}"; then
               yara_lib_path="${path}/"
               yara_lib_name="${z}"
               yara_lib_file="${yara_lib_path}/${z}.${y}"
               break
           fi
           if test -e "${path}/lib${z}.${y}"; then
               yara_lib_path="${path}/"
               yara_lib_name="${z}"
               yara_lib_file="${yara_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/lib${z}.${y}"; then
               yara_lib_path="${path}/lib/"
               yara_lib_name="${z}"
               yara_lib_file="${yara_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/x86_64-linux-gnu/lib${z}.${y}"; then
               yara_lib_path="${path}/lib/x86_64-linux-gnu/"
               yara_lib_name="${z}"
               yara_lib_file="${yara_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/i386-linux-gnu/lib${z}.${y}"; then
               yara_lib_path="${path}/lib/i386-linux-gnu/"
               yara_lib_name="${z}"
               yara_lib_file="${yara_lib_path}/lib${z}.${y}"
               break
           fi
       done
       if test -n "$yara_lib_path"; then
           break
       fi
    done
    if test -e "${path}/include/yara.h"; then
        yara_inc_path="${path}/include"
    elif test -e "${path}/yara.h"; then
        yara_inc_path="${path}"
    elif test -e "${path}/include/yara/yara.h"; then
        yara_inc_path="${path}/include"
    fi

    if test -n "${yara_lib_path}"; then
        AC_MSG_NOTICE([YARA library found at: ${yara_lib_file}])
    fi

    if test -n "${yara_inc_path}"; then
        AC_MSG_NOTICE([YARA headers found at: ${yara_inc_path}])
    fi

    if test -n "${yara_lib_path}" -a -n "${yara_inc_path}"; then
        # TODO: Compile a piece of code to check the version.
        YARA_CFLAGS="-I${yara_inc_path}"
        YARA_LDADD="-l${yara_lib_name}"
        YARA_LDFLAGS="-L${yara_lib_path}"
        YARA_DISPLAY="${yara_lib_file}, ${yara_inc_path}"
    fi
]) # AC_DEFUN [CHECK_FOR_YARA_AT]



