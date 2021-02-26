dnl Check for HyperScan Libraries
dnl CHECK_YAJL(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])

AC_DEFUN([PROG_HS], [

# Needed if pkg-config will be used.
AC_REQUIRE([PKG_PROG_PKG_CONFIG])


# Possible names for the hs library/package (pkg-config)
HS_POSSIBLE_LIB_NAMES="hs libhs"

# Possible extensions for the library
HS_POSSIBLE_EXTENSIONS="so la sl dll dylib"

# Possible paths (if pkg-config was not found, proceed with the file lookup)
HS_POSSIBLE_PATHS="/usr/lib /usr/local/lib /usr/local/libhs /usr/local/hyperscan /usr/local /opt/libhs /opt/hs /opt /usr /usr/lib64"

# Variables to be set by this very own script.
HS_VERSION=""
HS_CFLAGS=""
HS_CPPFLAGS=""
HS_LDADD=""
HS_LDFLAGS=""

AC_ARG_WITH(
    hyperscan,
    AC_HELP_STRING(
      [--with-hyperscan=PATH],
      [Path to HyperScan prefix or config script]
    )
)

if test "x${with_hyperscan}" == "xno"; then
    AC_DEFINE(HAVE_HS, 0, [Support for HS was disabled by the utilization of --without-hyperscan or --with-hyperscan=no])
    AC_MSG_NOTICE([Support for HyperScan was disabled by the utilization of --without-hyperscan or --with-hyperscan=no])
    HS_DISABLED=yes
else
    if test "x${with_hyperscan}" == "xyes"; then
        HS_MANDATORY=yes
        AC_MSG_NOTICE([HyperScan support was marked as mandatory by the utilization of --with-hyperscan=yes])
    fi
#        for x in ${HS_POSSIBLE_LIB_NAMES}; do
#            CHECK_FOR_HS_AT(${x})
#            if test -n "${HS_VERSION}"; then
#                break
#            fi
#        done

#    if test "x${with_hs}" != "xyes" or test "x${with_hs}" == "xyes"; then
        if test "x${with_hyperscan}" == "x" || test "x${with_hyperscan}" == "xyes"; then
            # Nothing about HyperScan was informed, using the pkg-config to figure things out.
            if test -n "${PKG_CONFIG}"; then
                HS_PKG_NAME=""
                for x in ${HS_POSSIBLE_LIB_NAMES}; do
                    if ${PKG_CONFIG} --exists ${x}; then
                        HS_PKG_NAME="$x"
                        break
                    fi
                done
            fi
            AC_MSG_NOTICE([Nothing about HyperScan was informed during the configure phase. Trying to detect it on the platform...])
            if test -n "${HS_PKG_NAME}"; then
                # Package was found using the pkg-config scripts
                HS_VERSION="`${PKG_CONFIG} ${HS_PKG_NAME} --modversion`"
                HS_CFLAGS="`${PKG_CONFIG} ${HS_PKG_NAME} --cflags`"
                HS_LDADD="`${PKG_CONFIG} ${HS_PKG_NAME} --libs-only-l`"
                HS_LDFLAGS="`${PKG_CONFIG} ${HS_PKG_NAME} --libs-only-L --libs-only-other`"
                HS_DISPLAY="${HS_LDADD}, ${HS_CFLAGS}"
            else
                # If pkg-config did not find anything useful, go over file lookup.
                for x in ${HS_POSSIBLE_LIB_NAMES}; do
                    CHECK_FOR_HS_AT(${x})
                    if test -n "${HS_VERSION}"; then
                        break
                    fi
                done
            fi
        fi
        if test "x${with_hyperscan}" != "x"; then
            # An specific path was informed, lets check.
            HS_MANDATORY=yes
            CHECK_FOR_HS_AT(${with_hs})
        fi
#    fi
fi

if test -z "${HS_LDADD}"; then
    if test -z "${HS_MANDATORY}"; then
        if test -z "${HS_DISABLED}"; then
            AC_MSG_NOTICE([HyperScan library was not found])
            HS_FOUND=0
        else
            HS_FOUND=2
        fi
    else
        AC_MSG_ERROR([HyperScan was explicitly referenced but it was not found])
        HS_FOUND=-1
    fi
else
    HS_FOUND=1
    AC_MSG_NOTICE([using HyperScan v${HS_VERSION}])
    HS_CFLAGS="-DWITH_HS ${HS_CFLAGS}"
    HS_DISPLAY="${HS_LDADD}, ${HS_CFLAGS}"
    AC_SUBST(HS_VERSION)
    AC_SUBST(HS_LDADD)
    AC_SUBST(HS_LIBS)
    AC_SUBST(HS_LDFLAGS)
    AC_SUBST(HS_CFLAGS)
    AC_SUBST(HS_DISPLAY)
fi



AC_SUBST(HS_FOUND)

]) # AC_DEFUN [PROG_HS]


AC_DEFUN([CHECK_FOR_HS_AT], [
    path=$1
    for y in ${HS_POSSIBLE_EXTENSIONS}; do
        for z in ${HS_POSSIBLE_LIB_NAMES}; do
           if test -e "${path}/${z}.${y}"; then
               hs_lib_path="${path}/"
               hs_lib_name="${z}"
               hs_lib_file="${hs_lib_path}/${z}.${y}"
               break
           fi
           if test -e "${path}/lib${z}.${y}"; then
               hs_lib_path="${path}/"
               hs_lib_name="${z}"
               hs_lib_file="${hs_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/lib${z}.${y}"; then
               hs_lib_path="${path}/lib/"
               hs_lib_name="${z}"
               hs_lib_file="${hs_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/x86_64-linux-gnu/lib${z}.${y}"; then
               hs_lib_path="${path}/lib/x86_64-linux-gnu/"
               hs_lib_name="${z}"
               hs_lib_file="${hs_lib_path}/lib${z}.${y}"
               break
           fi
       done
       if test -n "$hs_lib_path"; then
           break
       fi
    done
    if test -e "${path}/include/hs.h"; then
        hs_inc_path="${path}/include"
    elif test -e "${path}/hs_common.h"; then
        hs_inc_path="${path}"
    elif test -e "${path}/include/hs/hs.h"; then
        hs_inc_path="${path}/include"
    fi

    if test -n "${hs_lib_path}"; then
        AC_MSG_NOTICE([HyperScan library found at: ${hs_lib_file}])
    fi

    if test -n "${hs_inc_path}"; then
        AC_MSG_NOTICE([HyperScan headers found at: ${hs_inc_path}])
    fi

    if test -n "${hs_lib_path}" -a -n "${hs_inc_path}"; then
        # TODO: Compile a piece of code to check the version.
        HS_CFLAGS="-I${hs_inc_path}"
        HS_LDADD="-l${hs_lib_name}"
        HS_LDFLAGS="-L${hs_lib_path}"
        HS_DISPLAY="${hs_lib_file}, ${hs_inc_path}"
    fi
]) # AC_DEFUN [CHECK_FOR_HS_AT]
