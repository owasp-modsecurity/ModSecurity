dnl Check for LIBMEMCACHED Libraries
dnl CHECK_LIBMEMCACHED(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  LIBMEMCACHED_CFLAGS
dnl  LIBMEMCACHED_LDFLAGS
dnl  LIBMEMCACHED_LDADD

AC_DEFUN([CHECK_LIBMEMCACHED],
[dnl

LIBMEMCACHED_CFLAGS=""
LIBMEMCACHED_LDFLAGS=""
LIBMEMCACHED_LDADD=""

AC_ARG_WITH(
    libmemcached,
    [AC_HELP_STRING([--with-libmemcached=PATH],[Path to libmemcached prefix])]
    ,, with_libmemcached=yes)

AS_CASE(["${with_libmemcached}"],
    [no], [test_paths=],
    [yes], [test_paths="/usr/lib /usr/local/lib /usr/local/libmemcached /usr/local/libmemcached /usr/local /opt/libmemcached /opt/libmemcached /opt /usr /usr/lib/x86_64-linux-gnu/"],
    [test_paths="${with_libmemcached}"])

AS_IF([test "x${test_paths}" != "x"], [
AC_MSG_CHECKING([for libmemcached path])
  
  LIBMEMCACHED_LIB_NAME="memcached"
  LIBMEMCACHED_LIB_FILENAME="lib$LIBMEMCACHED_LIB_NAME.so"

  if test -z "$withlibmemcached" -o "$withlibmemcached" = "yes"; then
    for i in ${test_paths}; do
      if test -f "$i/$LIBMEMCACHED_LIB_FILENAME"; then
        LIBMEMCACHED_LIB_DIR="$i"
      fi
    done
  else
    if test -f "$withlibmemcached/$LIBMEMCACHED_LIB_FILENAME"; then
      LIBMEMCACHED_LIB_DIR="$withlibmemcached"
    else
      if test -f "$withlibmemcached/.libs/$LIBMEMCACHED_LIB_FILENAME"; then
        LIBMEMCACHED_LIB_DIR="$withlibmemcached/.libs/"
      fi
    fi
  fi

  LIBMEMCACHED_LDFLAGS="-l$LIBMEMCACHED_LIB_NAME"
  LIBMEMCACHED_LDADD="-l$LIBMEMCACHED_LIB_NAME"

  if test -z "$withlibmemcached" -o "$withlibmemcached" = "yes"; then
    for i in /usr/include /usr/local/include /usr/include/libmemcached; do
      if test -f "$i/$LIBMEMCACHED_LIB_NAME.h"; then
        LIBMEMCACHED_CFLAGS="-I$i"
      fi
    done
  else
    if test -f "$withlibmemcached/../$LIBMEMCACHED_LIB_NAME.h"; then
      LIBMEMCACHED_CFLAGS="-I$withlibmemcached/../"
    else
      if test -f "$withlibmemcached/$LIBMEMCACHED_LIB_NAME.h"; then
        LIBMEMCACHED_CFLAGS="-I$withlibmemcached"
      fi
    fi
  fi

])  
 
    if test -z "${LIBMEMCACHED_CFLAGS}"; then
        AC_MSG_NOTICE([optional libmemcached library not found])
	LIBMEMCACHED_LDFLAGS=""
        LIBMEMCACHED_LDADD=""
        AC_MSG_RESULT([no])
    else
        LIBMEMCACHED_CFLAGS="-DWITH_LIBMEMCACHED ${LIBMEMCACHED_CFLAGS}"
        AC_SUBST(LIBMEMCACHED_LDFLAGS)
        AC_SUBST(LIBMEMCACHED_LDADD)
        AC_SUBST(LIBMEMCACHED_CFLAGS)

        AC_MSG_RESULT([${LIBMEMCACHED_LDFLAGS} ${LIBMEMCACHED_CFLAGS}]) 
    fi 

])
