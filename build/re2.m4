dnl Check for RE2 Libraries
dnl CHECK_RE2(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])

AC_DEFUN([PROG_RE2], [

# Needed if pkg-config will be used.
AC_REQUIRE([PKG_PROG_PKG_CONFIG])

# Possible names for the re2 library/package (pkg-config)
RE2_POSSIBLE_LIB_NAMES="re2"

# Possible extensions for the library
RE2_POSSIBLE_EXTENSIONS="so la sl dll dylib"

# Possible paths (if pkg-config was not found, proceed with the file lookup)
RE2_POSSIBLE_PATHS="/usr/lib /usr/local/lib /usr/local/libre2 /usr/local/re2 /usr/local /opt/libre2 /opt/re2 /opt /usr /usr/lib64"

# Variables to be set by this very own script.
RE2_VERSION=""
RE2_CFLAGS=""
RE2_CPPFLAGS=""
RE2_LDADD=""
RE2_LDFLAGS=""

AC_ARG_WITH(
    re2,
    AC_HELP_STRING(
      [--with-re2=PATH],
      [Path to re2 prefix or config script]
    )
)

if test "x${with_re2}" == "xno"; then
    AC_DEFINE(HAVE_RE2, 0, [Support for RE2 was disabled by the utilization of --without-re2 or --with-re2=no])
    AC_MSG_NOTICE([Support for RE2 was disabled by the utilization of --without-re2 or --with-re2=no])
    RE2_DISABLED=yes
else
    if test "x${with_re2}" == "xyes"; then
        RE2_MANDATORY=yes
        AC_MSG_NOTICE([RE2 support was marked as mandatory by the utilization of --with-re2=yes])
    fi
#        for x in ${RE2_POSSIBLE_LIB_NAMES}; do
#            CHECK_FOR_RE2_AT(${x})
#            if test -n "${RE2_VERSION}"; then
#                break
#            fi
#        done

#    if test "x${with_re2}" != "xyes" or test "x${with_re2}" == "xyes"; then
        if test "x${with_re2}" == "x" || test "x${with_re2}" == "xyes"; then
            # Nothing about RE2 was informed, using the pkg-config to figure things out.
            if test -n "${PKG_CONFIG}"; then
                RE2_PKG_NAME=""
                for x in ${RE2_POSSIBLE_LIB_NAMES}; do
                    if ${PKG_CONFIG} --exists ${x}; then
                        RE2_PKG_NAME="$x"
                        break
                    fi
                done
            fi
            AC_MSG_NOTICE([Nothing about RE2 was informed during the configure phase. Trying to detect it on the platform...])
            if test -n "${RE2_PKG_NAME}"; then
                # Package was found using the pkg-config scripts
                RE2_VERSION="`${PKG_CONFIG} ${RE2_PKG_NAME} --modversion`"
                RE2_CFLAGS="`${PKG_CONFIG} ${RE2_PKG_NAME} --cflags`"
                RE2_LDADD="`${PKG_CONFIG} ${RE2_PKG_NAME} --libs-only-l`"
                RE2_LDFLAGS="`${PKG_CONFIG} ${RE2_PKG_NAME} --libs-only-L --libs-only-other`"
                RE2_DISPLAY="${RE2_LDADD}, ${RE2_CFLAGS}"
            else
                # If pkg-config did not find anything useful, go over file lookup.
                for x in ${RE2_POSSIBLE_PATHS}; do
                    CHECK_FOR_RE2_AT(${x})
                    if test -n "${RE2_VERSION}"; then
                        break
                    fi
                done
            fi
        fi
        if test "x${with_re2}" != "x"; then
            # An specific path was informed, lets check.
            RE2_MANDATORY=yes
            CHECK_FOR_RE2_AT(${with_re2})
        fi
#    fi
fi

if test -z "${RE2_LDADD}"; then
    if test -z "${RE2_MANDATORY}"; then
        if test -z "${RE2_DISABLED}"; then
            AC_MSG_NOTICE([RE2 library was not found])
            RE2_FOUND=0
        else
            RE2_FOUND=2
        fi
    else
        AC_MSG_ERROR([RE2 was explicitly referenced but it was not found])
        RE2_FOUND=-1
    fi
else
    RE2_FOUND=1
    AC_MSG_NOTICE([using RE2 v${RE2_VERSION}])
    RE2_CFLAGS="-DWITH_RE2 ${RE2_CFLAGS}"
    RE2_DISPLAY="${RE2_LDADD}, ${RE2_CFLAGS}"
    AC_SUBST(RE2_VERSION)
    AC_SUBST(RE2_LDADD)
    AC_SUBST(RE2_LIBS)
    AC_SUBST(RE2_LDFLAGS)
    AC_SUBST(RE2_CFLAGS)
    AC_SUBST(RE2_DISPLAY)
fi



AC_SUBST(RE2_FOUND)

]) # AC_DEFUN [PROG_RE2]


AC_DEFUN([CHECK_FOR_RE2_AT], [
    path=$1
    for y in ${RE2_POSSIBLE_EXTENSIONS}; do
        for z in ${RE2_POSSIBLE_LIB_NAMES}; do
           if test -e "${path}/${z}.${y}"; then
               re2_lib_path="${path}/"
               re2_lib_name="${z}"
               re2_lib_file="${re2_lib_path}/${z}.${y}"
               break
           fi
           if test -e "${path}/lib${z}.${y}"; then
               re2_lib_path="${path}/"
               re2_lib_name="${z}"
               re2_lib_file="${re2_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/lib${z}.${y}"; then
               re2_lib_path="${path}/lib/"
               re2_lib_name="${z}"
               re2_lib_file="${re2_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/x86_64-linux-gnu/lib${z}.${y}"; then
               re2_lib_path="${path}/lib/x86_64-linux-gnu/"
               re2_lib_name="${z}"
               re2_lib_file="${re2_lib_path}/lib${z}.${y}"
               break
           fi
       done
       if test -n "$re2_lib_path"; then
           break
       fi
    done
    if test -e "${path}/include/re2.h"; then
        re2_inc_path="${path}/include"
    elif test -e "${path}/re2.h"; then
        re2_inc_path="${path}"
    elif test -e "${path}/include/re2/re2.h"; then
        re2_inc_path="${path}/include"
    fi

    if test -n "${re2_lib_path}"; then
        AC_MSG_NOTICE([RE2 library found at: ${re2_lib_file}])
    fi

    if test -n "${re2_inc_path}"; then
        AC_MSG_NOTICE([RE2 headers found at: ${re2_inc_path}])
    fi

    if test -n "${re2_lib_path}" -a -n "${re2_inc_path}"; then
        # TODO: Compile a piece of code to check the version.
        RE2_CFLAGS="-I${re2_inc_path}"
        RE2_LDADD="-l${re2_lib_name}"
        RE2_LDFLAGS="-L${re2_lib_path}"
        RE2_DISPLAY="${re2_lib_file}, ${re2_inc_path}"
    fi
]) # AC_DEFUN [CHECK_FOR_RE2_AT]
