dnl Check for SSDEEP Libraries
dnl CHECK_SSDEEP(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  SSDEEP_CFLAGS
dnl  SSDEEP_LDFLAGS

AC_DEFUN([CHECK_SSDEEP],
[dnl

SSDEEP_CFLAGS=""
SSDEEP_LDFLAGS=""
SSDEEP_LDADD=""

AC_ARG_WITH(
    ssdeep,
    [AC_HELP_STRING([--with-ssdeep=PATH],[Path to ssdeep prefix])]
    ,, with_ssdeep=yes)

AS_CASE(["${with_ssdeep}"],
    [no], [test_paths=],
    [yes], [test_paths="/usr/lib /usr/local/lib /usr/local/libfuzzy /usr/local/fuzzy /usr/local /opt/libfuzzy /opt/fuzzy /opt /usr"])
    [test_paths="${with_ssdeep}"],

AS_IF([test "x${test_paths}" != "x"], [
AC_MSG_CHECKING([for ssdeep path])
  
  SSDEEP_LIB_NAME="fuzzy"
  SSDEEP_LIB_FILENAME="lib$SSDEEP_LIB_NAME.so"

  if test -z "$withssdeep" -o "$withssdeep" = "yes"; then
    for i in ${test_paths}; do
      if test -f "$i/$SSDEEP_LIB_FILENAME"; then
        SSDEEP_LIB_DIR="$i"
      fi
    done
  else
    if test -f "$withssdeep/$SSDEEP_LIB_FILENAME"; then
      SSDEEP_LIB_DIR="$withssdeep"
    else
      if test -f "$withssdeep/.libs/$SSDEEP_LIB_FILENAME"; then
        SSDEEP_LIB_DIR="$withssdeep/.libs/"
      fi
    fi
  fi

  SSDEEP_LDFLAGS="-l$SSDEEP_LIB_NAME"
  SSDEEP_LDADD="-l$SSDEEP_LIB_NAME"

  if test -z "$withssdeep" -o "$withssdeep" = "yes"; then
    for i in /usr/include /usr/local/include; do
      if test -f "$i/$SSDEEP_LIB_NAME.h"; then
        SSDEEP_CFLAGS="-I$i"
      fi
    done
  else
    if test -f "$withssdeep/../$SSDEEP_LIB_NAME.h"; then
      SSDEEP_CFLAGS="-I$withssdeep/../"
    else
      if test -f "$withssdeep/$SSDEEP_LIB_NAME.h"; then
        SSDEEP_CFLAGS="-I$withssdeep"
      fi
    fi
  fi

])  
 
    if test -z "${SSDEEP_CFLAGS}"; then
        AC_MSG_RESULT([no])
	SSDEEP_LDFLAGS=""
	SSDEEP_LDADD=""
	#ifelse([$2], , AC_MSG_NOTICE([optional ssdeep library not found]), $2)
    else
      SSDEEP_CFLAGS="-DWITH_SSDEEP ${SSDEEP_CFLAGS}"
        AC_MSG_RESULT([${SSDEEP_LDFLAGS} ${SSDEEP_CFLAGS}]) 
#AC_MSG_NOTICE([using ssdeep: ${SSDEEP_CFLAGS} ${SSDEEP_LDFLAGS}])
#ifelse([$1], , , $1) 
    fi 

AC_SUBST(SSDEEP_LDFLAGS)
AC_SUBST(SSDEEP_LDADD)
AC_SUBST(SSDEEP_CFLAGS)


])
