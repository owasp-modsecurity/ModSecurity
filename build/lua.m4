dnl Check for LUA Libraries
dnl CHECK_LUA(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])


AC_DEFUN([CHECK_LUA],
[dnl

# Possible names for the lua library/package (pkg-config)
LUA_POSSIBLE_LIB_NAMES="lua"

# Possible extensions for the library
LUA_POSSIBLE_EXTENSIONS="so so0 la sl dll dylib so.0.0.0"

# Possible paths (if pkg-config was not found, proceed with the file lookup)
LUA_POSSIBLE_PATHS="/usr/lib /usr/local/lib /usr/local/lua /usr/local/liblua /usr/local /opt /usr /usr/lib64 /opt/local"

# Variables to be set by this very own script.
LUA_CFLAGS=""
LUA_LDFLAGS=""
LUA_LDADD=""
LUA_DISPLAY=""

AC_ARG_WITH(
    lua,
    AC_HELP_STRING(
    [--with-lua=PATH],
    [Path to lua prefix]
    )
)


if test "x${with_lua}" == "xno"; then
    AC_DEFINE(HAVE_LUA, 0, [Support for LUA was disabled by the utilization of --without-lua or --with-lua=no])
    AC_MSG_NOTICE([Support for LUA was disabled by the utilization of --without-lua or --with-lua=no])
    LUA_DISABLED=yes
else
    if test "x${with_lua}" == "xyes"; then
        LUA_MANDATORY=yes
        AC_MSG_NOTICE([LUA support was marked as mandatory by the utilization of --with-lua=yes])
    else
        LUA_MANDATORY=no
    fi
    for x in ${LUA_POSSIBLE_PATHS}; do
        CHECK_FOR_LUA_AT(${x})
        if test -n "${LUA_CFLAGS}"; then
            break
        fi
    done
fi


if test -z "${LUA_CFLAGS}"; then
    if test -z "${LUA_MANDATORY}" || test "x${LUA_MANDATORY}" == "xno"; then
        if test -z "${LUA_DISABLED}"; then
            AC_MSG_NOTICE([LUA library was not found])
            LUA_FOUND=0
        else
            LUA_FOUND=2
        fi
    else
        AC_MSG_ERROR([LUA was explicitly referenced but it was not found])
        LUA_FOUND=-1
    fi
else
    if test -z "${LUA_MANDATORY}" || test "x${LUA_MANDATORY}" == "xno"; then
        LUA_FOUND=1
        AC_MSG_NOTICE([using LUA v${LUA_VERSION}])
        LUA_CFLAGS="-DWITH_LUA ${LUA_CFLAGS}"
        LUA_DISPLAY="${LUA_LDADD} ${LUA_LDFLAGS}, ${LUA_CFLAGS}"
        AC_SUBST(LUA_LDFLAGS)
        AC_SUBST(LUA_LDADD)
        AC_SUBST(LUA_CFLAGS)
        AC_SUBST(LUA_DISPLAY)
    else
        LUA_FOUND=1
        AC_MSG_NOTICE([using LUA v${LUA_VERSION}])
        LUA_CFLAGS="-DWITH_LUA ${LUA_CFLAGS}"
        LUA_DISPLAY="${LUA_LDADD} ${LUA_LDFLAGS}, ${LUA_CFLAGS}"
        AC_SUBST(LUA_LDFLAGS)
        AC_SUBST(LUA_LDADD)
        AC_SUBST(LUA_CFLAGS)
        AC_SUBST(LUA_DISPLAY)
    fi
fi


AC_SUBST(LUA_FOUND)

]) # AC_DEFUN [CHECK_LUA]


AC_DEFUN([CHECK_FOR_LUA_AT], [
    path=$1
    echo "*** LOOKING AT PATH: " ${path}
    for y in ${LUA_POSSIBLE_EXTENSIONS}; do
        for z in ${LUA_POSSIBLE_LIB_NAMES}; do
           if test -e "${path}/${z}.${y}"; then
               lua_lib_path="${path}/"
               lua_lib_name="${z}"
               lua_lib_file="${lua_lib_path}/${z}.${y}"
               break
           fi
           if test -e "${path}/lib${z}.${y}"; then
               lua_lib_path="${path}/"
               lua_lib_name="${z}"
               lua_lib_file="${lua_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/lib${z}.${y}"; then
               lua_lib_path="${path}/lib/"
               lua_lib_name="${z}"
               lua_lib_file="${lua_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/x86_64-linux-gnu/lib${z}.${y}"; then
               lua_lib_path="${path}/lib/x86_64-linux-gnu/"
               lua_lib_name="${z}"
               lua_lib_file="${lua_lib_path}/lib${z}.${y}"
               break
           fi
           if test -e "${path}/lib/i386-linux-gnu/lib${z}.${y}"; then
               lua_lib_path="${path}/lib/i386-linux-gnu/"
               lua_lib_name="${z}"
               lua_lib_file="${lua_lib_path}/lib${z}.${y}"
               break
           fi
       done
       if test -n "$lua_lib_path"; then
           break
       fi
    done
    if test -e "${path}/include/fuzzy.h"; then
        lua_inc_path="${path}/include"
    elif test -e "${path}/fuzzy.h"; then
        lua_inc_path="${path}"
    elif test -e "${path}/include/fuzzy/fuzzy.h"; then
        lua_inc_path="${path}/include"
    fi

    if test -n "${lua_lib_path}"; then
        AC_MSG_NOTICE([LUA library found at: ${lua_lib_file}])
    fi

    if test -n "${lua_inc_path}"; then
        AC_MSG_NOTICE([LUA headers found at: ${lua_inc_path}])
    fi

    if test -n "${lua_lib_path}" -a -n "${lua_inc_path}"; then
        # TODO: Compile a piece of code to check the version.
        LUA_CFLAGS="-I${lua_inc_path}"
        LUA_LDADD="-l${lua_lib_name}"
        LUA_LDFLAGS="-L${lua_lib_path}"
        LUA_DISPLAY="${lua_lib_file}, ${lua_inc_path}"
    fi
]) # AC_DEFUN [CHECK_FOR_LUA_AT]



