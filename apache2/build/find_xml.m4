dnl Check for LIBXML2 Libraries
dnl CHECK_LIBXML2(ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND])
dnl Sets:
dnl  LIBXML2_CFLAGS
dnl  LIBXML2_LIBS

LIBXML2_CONFIG=""
LIBXML2_CFLAGS=""
LIBXML2_LIBS=""

AC_DEFUN([CHECK_LIBXML2],
[dnl

AC_ARG_WITH(
    xml,
    [AC_HELP_STRING([--with-xml=PATH],[Path to xml prefix or config script])],
    [test_paths="${with_xml}"],
    [test_paths="/usr/local/libxml2 /usr/local/xml2 /usr/local/xml /usr/local /opt/libxml2 /opt/libxml /opt/xml2 /opt/xml /opt /usr"])

AC_MSG_CHECKING([for libxml2 config script])

for x in ${test_paths}; do
    dnl # Determine if the script was specified and use it directly
    if test ! -d "$x" -a -e "$x"; then
        LIBXML2_CONFIG="`basename $x`"
        xml_path=`echo $x | sed "s/\/\?${LIBXML2_CONFIG}\$//"`
        break
    fi

    dnl # Try known config script names/locations
    for LIBXML2_CONFIG in xml2-config xml-2-config xml-config; do
        if test -e "${x}/bin/${LIBXML2_CONFIG}"; then
            xml_path="${x}/bin"
            break
        elif test -e "${x}/${LIBXML2_CONFIG}"; then
            xml_path="${x}"
            break
        else
            xml_path=""
        fi
    done
    if test -n "$xml_path"; then
        break
    fi
done
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS

if test -n "${xml_path}"; then
    LIBXML2_CONFIG="${xml_path}/${LIBXML2_CONFIG}"
    AC_MSG_RESULT([${LIBXML2_CONFIG}])
    LIBXML2_CFLAGS="`${LIBXML2_CONFIG} --cflags`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(xml CFLAGS: $LIBXML2_CFLAGS); fi
    LIBXML2_LIBS="`${LIBXML2_CONFIG} --libs`"
    if test "$verbose_output" -eq 1; then AC_MSG_NOTICE(xml LIBS: $LIBXML2_LIBS); fi
    CFLAGS=$save_CFLAGS
    LDFLAGS=$save_LDFLAGS
else
    AC_MSG_RESULT([no])
fi

AC_SUBST(LIBXML2_LIBS)
AC_SUBST(LIBXML2_CFLAGS)

if test -z "${LIBXML2_LIBS}"; then
    AC_MSG_NOTICE([*** xml library not found.])
    ifelse([$2], , AC_MSG_ERROR([xml library is required]), $2)
else
    AC_MSG_NOTICE([using '${LIBXML2_LIBS}' for xml Library])
    ifelse([$1], , , $1) 
fi 
])
