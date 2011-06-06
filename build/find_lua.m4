dnl Check for LUA Libraries
dnl CHECK_LUA(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  LUA_CFLAGS
dnl  LUA_LIBS

AC_DEFUN([CHECK_LUA],
[dnl

AC_REQUIRE([PKG_PROG_PKG_CONFIG])

LUA_CONFIG=""
LUA_VERSION=""
LUA_CFLAGS=""
LUA_CPPFLAGS=""
LUA_LDADD=""
LUA_LDFLAGS=""
LUA_CONFIG=${PKG_CONFIG}
LUA_PKGNAMES="lua5.1 lua-5.1 lua_5.1 lua-51 lua_51 lua51 lua5 lua"
LUA_SONAMES="so la sl dll dylib"

AC_ARG_WITH(
    lua,
    [AC_HELP_STRING([--with-lua=PATH],[Path to lua prefix or config script])]
    ,, with_lua=yes)

AS_CASE(["${with_lua}"],
  [no], [test_paths=],
  [yes], [test_paths="/usr/local/liblua /usr/local/lua /usr/local /opt/liblua /opt/lua /opt /usr"],
  [test_paths="${with_lua}"])

AS_IF([test "x${test_paths}" != "x"], [
AC_MSG_CHECKING([for liblua config script])

for x in ${test_paths}; do
    dnl # Determine if the script was specified and use it directly
    if test ! -d "$x" -a -e "$x"; then
        LUA_CONFIG=$x
        break
    fi

    dnl # Try known config script names/locations
    for y in $LUA_CONFIG; do
        if test -e "${x}/bin/${y}"; then
            LUA_CONFIG="${x}/bin/${y}"
            lua_config="${LUA_CONFIG}"
            break
        elif test -e "${x}/${y}"; then
            LUA_CONFIG="${x}/${y}"
            lua_config="${LUA_CONFIG}"
            break
        fi
    done
    if test -n "${lua_config}"; then
        break
    fi
done

dnl # Try known package names
if test -n "${LUA_CONFIG}"; then
    LUA_PKGNAME=""
    for x in ${LUA_PKGNAMES}; do
        if ${LUA_CONFIG} --exists ${x}; then
            LUA_PKGNAME="$x"
            break
        fi
    done
fi

if test -n "${LUA_PKGNAME}"; then
    AC_MSG_RESULT([${LUA_CONFIG}])
    LUA_VERSION="`${LUA_CONFIG} ${LUA_PKGNAME} --modversion`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(lua VERSION: $LUA_VERSION); fi
    LUA_CFLAGS="`${LUA_CONFIG} ${LUA_PKGNAME} --cflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(lua CFLAGS: $LUA_CFLAGS); fi
    LUA_LDADD="`${LUA_CONFIG} ${LUA_PKGNAME} --libs-only-l`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(lua LDADD: $LUA_LDADD); fi
    LUA_LDFLAGS="`${LUA_CONFIG} ${LUA_PKGNAME} --libs-only-L --libs-only-other`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(lua LDFLAGS: $LUA_LDFLAGS); fi
else
    AC_MSG_RESULT([no])

    dnl Hack to just try to find the lib and include
    AC_MSG_CHECKING([for lua install])
    for x in ${test_paths}; do
        for y in ${LUA_SONAMES}; do
            if test -e "${x}/liblua5.1.${y}"; then
                lua_lib_path="${x}"
                lua_lib_name="lua5.1"
                break
            elif test -e "${x}/lib/liblua5.1.${y}"; then
                lua_lib_path="${x}/lib"
                lua_lib_name="lua5.1"
                break
            elif test -e "${x}/lib64/liblua5.1.${y}"; then
                lua_lib_path="${x}/lib64"
                lua_lib_name="lua5.1"
                break
            elif test -e "${x}/lib32/liblua5.1.${y}"; then
                lua_lib_path="${x}/lib32"
                lua_lib_name="lua5.1"
                break
            elif test -e "${x}/liblua51.${y}"; then
                lua_lib_path="${x}"
                lua_lib_name="lua51"
                break
            elif test -e "${x}/lib/liblua51.${y}"; then
                lua_lib_path="${x}/lib"
                lua_lib_name="lua51"
                break
            elif test -e "${x}/lib64/liblua51.${y}"; then
                lua_lib_path="${x}/lib64"
                lua_lib_name="lua51"
                break
            elif test -e "${x}/lib32/liblua51.${y}"; then
                lua_lib_path="${x}/lib32"
                lua_lib_name="lua51"
                break
            elif test -e "${x}/liblua.${y}"; then
                lua_lib_path="${x}"
                lua_lib_name="lua"
                break
            elif test -e "${x}/lib/liblua.${y}"; then
                lua_lib_path="${x}/lib"
                lua_lib_name="lua"
                break
            elif test -e "${x}/lib64/liblua.${y}"; then
                lua_lib_path="${x}/lib64"
                lua_lib_name="lua"
                break
            elif test -e "${x}/lib32/liblua.${y}"; then
                lua_lib_path="${x}/lib32"
                lua_lib_name="lua"
                break
            else
                lua_lib_path=""
                lua_lib_name=""
            fi
        done
        if test -n "$lua_lib_path"; then
            break
        fi
    done
    for x in ${test_paths}; do
        if test -e "${x}/include/lua.h"; then
            lua_inc_path="${x}/include"
            break
        elif test -e "${x}/lua.h"; then
            lua_inc_path="${x}"
            break
        fi

        dnl # Check some sub-paths as well
        for lua_pkg_name in ${lua_lib_name} ${LUA_PKGNAMES}; do
            if test -e "${x}/include/${lua_pkg_name}/lua.h"; then
                lua_inc_path="${x}/include"
                break
            elif test -e "${x}/${lua_pkg_name}/lua.h"; then
                lua_inc_path="${x}"
                break
            else
                lua_inc_path=""
            fi
        done
        if test -n "$lua_inc_path"; then
            break
        fi
    done
    if test -n "${lua_lib_path}" -a -n "${lua_inc_path}"; then
        LUA_CONFIG=""
        AC_MSG_RESULT([${lua_lib_path} ${lua_inc_path}])
        LUA_VERSION="5.1"
        LUA_CFLAGS="-I${lua_inc_path}"
        LUA_LDADD="-l${lua_lib_name}"
        LUA_LDFLAGS="-L${lua_lib_path}"
    else
        LUA_VERSION=""
        AC_MSG_RESULT([no])
    fi
fi

])

AC_SUBST(LUA_CFLAGS)
AC_SUBST(LUA_LDADD)
AC_SUBST(LUA_LDFLAGS)

    if test -z "${LUA_VERSION}"; then
      ifelse([$2], , AC_MSG_NOTICE([optional lua library not found]), $2)
    else
      AC_MSG_NOTICE([using lua v${LUA_VERSION}])
      LUA_CFLAGS="-DWITH_LUA ${LUA_CFLAGS}"
      ifelse([$1], , , $1) 
    fi 
])
