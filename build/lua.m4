dnl Check for LUA Libraries
dnl Sets:
dnl  LUA_CFLAGS
dnl  LUA_LDADD
dnl  LUA_LDFLAGS
dnl  LUA_DISPLAY
dnl  LUA_FOUND

AC_DEFUN([CHECK_LUA], [
MSC_CHECK_LIB([LUA], [lua54 lua5.4 lua-5.4 lua53 lua5.3 lua-5.3 lua52 lua5.2 lua-5.2 lua51 lua5.1 lua-5.1 luajit lua], [lua.h], [lua5.4 lua5.3 lua5.2 lua5.1 luajit-5.1 lua], [-DWITH_LUA])

# Post-processing: detect Lua version and add version-specific defines
if test "x${LUA_FOUND}" = "x1"; then
    LUA_VERSION=""

    # First, try to get version from pkg-config modversion
    if test -n "${PKG_CONFIG}"; then
        _msc_lua_pkg_name=""
        for _msc_lua_p in lua54 lua5.4 lua-5.4 lua53 lua5.3 lua-5.3 lua52 lua5.2 lua-5.2 lua51 lua5.1 lua-5.1 luajit lua; do
            if ${PKG_CONFIG} --exists ${_msc_lua_p} 2>/dev/null; then
                _msc_lua_pkg_name="${_msc_lua_p}"
                break
            fi
        done
        if test -n "${_msc_lua_pkg_name}"; then
            _msc_lua_pkg_version="`${PKG_CONFIG} ${_msc_lua_pkg_name} --modversion`"
            case $_msc_lua_pkg_version in
                5.1*) LUA_CFLAGS="-DWITH_LUA_5_1 ${LUA_CFLAGS}" ; LUA_VERSION="${_msc_lua_pkg_version}" ;;
                5.2*) LUA_CFLAGS="-DWITH_LUA_5_2 ${LUA_CFLAGS}" ; LUA_VERSION="${_msc_lua_pkg_version}" ;;
                5.3*) LUA_CFLAGS="-DWITH_LUA_5_3 ${LUA_CFLAGS}" ; LUA_VERSION="${_msc_lua_pkg_version}" ;;
                5.4*) LUA_CFLAGS="-DWITH_LUA_5_4 ${LUA_CFLAGS}" ; LUA_VERSION="${_msc_lua_pkg_version}" ;;
                2.0*) LUA_CFLAGS="-DWITH_LUA_5_1 ${LUA_CFLAGS}" ; LUA_VERSION="${_msc_lua_pkg_version}" ;;
                2.1*) LUA_CFLAGS="-DWITH_LUA_5_1 -DWITH_LUA_JIT_2_1 ${LUA_CFLAGS}" ; LUA_VERSION="${_msc_lua_pkg_version}" ;;
            esac
            AC_MSG_NOTICE([LUA pkg-config version: ${_msc_lua_pkg_version}])
        fi
    fi

    # If no version detected via pkg-config, try compile tests
    if test -z "${LUA_VERSION}"; then
        _msc_save_CFLAGS=$CFLAGS
        CFLAGS="${LUA_CFLAGS} ${CFLAGS}"

        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[ #include <lua.h> ]],
            [[ #if (LUA_VERSION_NUM == 504)
               return 0;
               #else
               #error not 5.4
               #endif ]])],
            [ _msc_lua_ver=504 ], [ _msc_lua_ver="" ])

        if test -z "$_msc_lua_ver"; then
            AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[ #include <lua.h> ]],
                [[ #if (LUA_VERSION_NUM == 503)
                   return 0;
                   #else
                   #error not 5.3
                   #endif ]])],
                [ _msc_lua_ver=503 ], [ _msc_lua_ver="" ])
        fi

        if test -z "$_msc_lua_ver"; then
            AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[ #include <lua.h> ]],
                [[ #if (LUA_VERSION_NUM == 502)
                   return 0;
                   #else
                   #error not 5.2
                   #endif ]])],
                [ _msc_lua_ver=502 ], [ _msc_lua_ver="" ])
        fi

        if test -z "$_msc_lua_ver"; then
            AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[ #include <lua.h> ]],
                [[ #if (LUA_VERSION_NUM == 501)
                   return 0;
                   #else
                   #error not 5.1
                   #endif ]])],
                [ _msc_lua_ver=501 ], [ _msc_lua_ver="" ])
        fi

        CFLAGS=$_msc_save_CFLAGS

        case $_msc_lua_ver in
            501) LUA_CFLAGS="-DWITH_LUA_5_1 ${LUA_CFLAGS}" ;;
            502) LUA_CFLAGS="-DWITH_LUA_5_2 ${LUA_CFLAGS}" ;;
            503) LUA_CFLAGS="-DWITH_LUA_5_3 ${LUA_CFLAGS}" ;;
            504) LUA_CFLAGS="-DWITH_LUA_5_4 ${LUA_CFLAGS}" ;;
        esac
        if test -n "$_msc_lua_ver"; then
            AC_MSG_NOTICE([LUA version from compile test: $_msc_lua_ver])
        fi
    fi

    LUA_DISPLAY="${LUA_LDADD} ${LUA_LDFLAGS}, ${LUA_CFLAGS}"
fi

]) # AC_DEFUN [CHECK_LUA]
