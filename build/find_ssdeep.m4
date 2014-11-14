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
    [yes], [test_paths="/usr/ /usr/local/ /usr/local/libfuzzy /usr/local/fuzzy /opt/libfuzzy /opt/fuzzy /opt /opt/local"],
    [test_paths="${with_ssdeep}"])

AS_IF([test "x${test_paths}" != "x"], [
AC_MSG_CHECKING([for ssdeep path])
  
  SSDEEP_LIB_NAME="fuzzy"

  if test -z "$withssdeep" -o "$withssdeep" = "yes"; then
    for i in ${test_paths}; do
      if test -f "$i/include/fuzzy.h"; then
        SSDEEP_CFLAGS="-I$i"
      fi
    done
  else
    if test -f "$withssdeep/include/fuzzy.h"; then
      SSDEEP_CFLAGS="-I$i"
    fi
  fi

  SSDEEP_LDFLAGS="-lfuzzy"
  SSDEEP_LDADD="-lfuzzy"

])  
 
    if test -z "${SSDEEP_CFLAGS}"; then
        AC_MSG_RESULT([no])
	SSDEEP_LDFLAGS=""
	SSDEEP_LDADD=""
       AC_MSG_NOTICE([optional ssdeep library not found])
    else
      SSDEEP_CFLAGS="-DWITH_SSDEEP ${SSDEEP_CFLAGS}"
        AC_MSG_RESULT([${SSDEEP_LDFLAGS} ${SSDEEP_CFLAGS}]) 
    fi 

AC_SUBST(SSDEEP_LDFLAGS)
AC_SUBST(SSDEEP_LDADD)
AC_SUBST(SSDEEP_CFLAGS)


])
