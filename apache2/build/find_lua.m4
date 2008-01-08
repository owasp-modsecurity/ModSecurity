dnl Check for LUA Libraries
dnl CHECK_LUA(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  LUA_CFLAGS
dnl  LUA_LIBS

LUA_CONFIG="pkg-config"
LUA_PKGNAME="lua5.1"
LUA_CFLAGS=""
LUA_LIBS=""

AC_DEFUN([CHECK_LUA],
[dnl

AC_ARG_WITH(
    lua,
    [AC_HELP_STRING([--with-lua=PATH],[Path to the lua prefix])],
    lua_path="$withval",
    :)

dnl # Determine lua lib directory
if test -z "${lua_path}"; then
    test_paths="/usr/local /usr"
else
    test_paths="${lua_path}"
fi

AC_MSG_CHECKING([for pkg-config script for liblua])
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
    AC_MSG_RESULT([${LUA_CONFIG} ${LUA_PKGNAME}])
    LUA_CFLAGS="`${LUA_CONFIG} ${LUA_PKGNAME} --cflags`"
    LUA_LIBS="`${LUA_CONFIG} ${LUA_PKGNAME} --libs`"
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(LUA_LIBS)
AC_SUBST(LUA_CFLAGS)

if test -z "${LUA_LIBS}"; then
  AC_MSG_NOTICE([*** lua library not found.])
  ifelse([$2], , AC_MSG_ERROR([lua library is required]), $2)
else
  AC_MSG_NOTICE([using '${LUA_LIBS}' for lua Library])
  ifelse([$1], , , $1) 
fi 
])
