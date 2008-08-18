dnl Check for LUA Libraries
dnl CHECK_LUA(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  LUA_CFLAGS
dnl  LUA_LIBS

LUA_CONFIG="pkg-config"
LUA_PKGNAMES="lua5.1 lua5 lua"
LUA_CFLAGS=""
LUA_LIBS=""
LUA_SONAMES="so la sl dll dylib"

AC_DEFUN([CHECK_LUA],
[dnl

AC_ARG_WITH(
    lua,
    [AC_HELP_STRING([--with-lua=PATH],[Path to the lua prefix])],
    lua_path="$withval",
    :)

if test "${lua_path}" != "no"; then
    dnl # Determine lua lib directory
    if test -z "${lua_path}"; then
        test_paths="/usr /usr/local /opt"
    else
        test_paths="${lua_path}"
    fi

    AC_MSG_CHECKING([for pkg-config script for lua library])
    for x in ${test_paths}; do
        if test -e "${x}/bin/${LUA_CONFIG}"; then
            with_lua="${x}/bin"
            break
        else
            with_lua=""
        fi
    done
    if test -n "${with_lua}"; then
        LUA_CONFIG="${with_lua}/${LUA_CONFIG}"
        for LUA_PKGNAME in ${LUA_PKGNAMES}; do
            if ${LUA_CONFIG} --exists ${LUA_PKGNAME}; then
                break
            fi
            LUA_PKGNAME=""
        done
        if test -n "$LUA_PKGNAME"; then
            AC_MSG_RESULT([${LUA_CONFIG} ${LUA_PKGNAME}])
            LUA_CFLAGS="`${LUA_CONFIG} ${LUA_PKGNAME} --cflags`"
            LUA_LIBS="`${LUA_CONFIG} ${LUA_PKGNAME} --libs`"
            CFLAGS=$save_CFLAGS
            LDFLAGS=$save_LDFLAGS
        else
            AC_MSG_RESULT([no])
        fi
    else
        AC_MSG_RESULT([no])

        dnl Hack to just try to find the lib and include
        AC_MSG_CHECKING([for lua install])
        for x in ${test_paths}; do
            for y in ${LUA_SONAMES}; do
                if test -e "${x}/liblua5.1.${y}"; then
                    with_lua_lib="${x}"
                    lua_lib_name="lua5.1"
                    break
                elif test -e "${x}/lib/liblua5.1.${y}"; then
                    with_lua_lib="${x}/lib"
                    lua_lib_name="lua5.1"
                    break
                elif test -e "${x}/lib64/liblua5.1.${y}"; then
                    with_lua_lib="${x}/lib64"
                    lua_lib_name="lua5.1"
                    break
                elif test -e "${x}/lib32/liblua5.1.${y}"; then
                    with_lua_lib="${x}/lib32"
                    lua_lib_name="lua5.1"
                    break
                elif test -e "${x}/liblua51.${y}"; then
                    with_lua_lib="${x}"
                    lua_lib_name="lua51"
                    break
                elif test -e "${x}/lib/liblua51.${y}"; then
                    with_lua_lib="${x}/lib"
                    lua_lib_name="lua51"
                    break
                elif test -e "${x}/lib64/liblua51.${y}"; then
                    with_lua_lib="${x}/lib64"
                    lua_lib_name="lua51"
                    break
                elif test -e "${x}/lib32/liblua51.${y}"; then
                    with_lua_lib="${x}/lib32"
                    lua_lib_name="lua51"
                    break
                elif test -e "${x}/liblua.${y}"; then
                    with_lua_lib="${x}"
                    lua_lib_name="lua"
                    break
                elif test -e "${x}/lib/liblua.${y}"; then
                    with_lua_lib="${x}/lib"
                    lua_lib_name="lua"
                    break
                elif test -e "${x}/lib64/liblua.${y}"; then
                    with_lua_lib="${x}/lib64"
                    lua_lib_name="lua"
                    break
                elif test -e "${x}/lib32/liblua.${y}"; then
                    with_lua_lib="${x}/lib32"
                    lua_lib_name="lua"
                    break
                else
                    with_lua_lib=""
                    lua_lib_name=""
                fi
            done
            if test -n "$with_lua_lib"; then
                break
            fi
        done
        for x in ${test_paths}; do
            if test -e "${x}/include/lua.h"; then
                with_lua_inc="${x}/include"
                break
            elif test -e "${x}/lua.h"; then
                with_lua_inc="${x}"
                break
            fi

            dnl # Check some sub-paths as well
            for lua_pkg_name in ${lua_lib_name} ${LUA_PKGNAMES}; do
                if test -e "${x}/include/${lua_pkg_name}/lua.h"; then
                    with_lua_inc="${x}/include"
                    break
                elif test -e "${x}/${lua_pkg_name}/lua.h"; then
                    with_lua_inc="${x}"
                    break
                else
                    with_lua_inc=""
                fi
            done
            if test -n "$with_lua_inc"; then
                break
            fi
        done
        if test -n "${with_lua_lib}" -a -n "${with_lua_inc}"; then
            LUA_CONFIG=""
            AC_MSG_RESULT([${with_lua_lib} ${with_lua_inc}])
            LUA_CFLAGS="-I${with_lua_inc}"
            LUA_LIBS="-L${with_lua_lib} -l${lua_lib_name}"
            CFLAGS=$save_CFLAGS
            LDFLAGS=$save_LDFLAGS
        else
            AC_MSG_RESULT([no])
        fi
    fi
else
    AC_MSG_NOTICE([not using optional lua library])
fi

if test -n "${LUA_LIBS}"; then
    LUA_CFLAGS="-DWITH_LUA ${LUA_CFLAGS}"
fi

AC_SUBST(LUA_LIBS)
AC_SUBST(LUA_CFLAGS)

if test "${lua_path}" != "no"; then
    if test -z "${LUA_LIBS}"; then
      ifelse([$2], , AC_MSG_NOTICE([optional lua library not found]), $2)
    else
      AC_MSG_NOTICE([using '${LUA_LIBS}' for lua Library])
      ifelse([$1], , , $1) 
    fi 
fi
])
