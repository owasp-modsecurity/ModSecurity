#! /bin/sh

echo "buildconf: checking installation..."

# autoconf 2.50 or newer
ac_version=`${AUTOCONF:-autoconf} --version 2>/dev/null|sed -e 's/^[^0-9]*//;s/[a-z]* *$//;q'`
if test -z "$ac_version"; then
echo "buildconf: autoconf not found."
echo "           You need autoconf version 2.50 or newer installed"
echo "           to build APR from SVN."
exit 1
fi
IFS=.; set $ac_version; IFS=' '
if test "$1" = "2" -a "$2" -lt "50" || test "$1" -lt "2"; then
echo "buildconf: autoconf version $ac_version found."
echo "           You need autoconf version 2.50 or newer installed"
echo "           to build APR from SVN."
exit 1
else
echo "buildconf: autoconf version $ac_version (ok)"
fi

# Sample libtool --version outputs:
# ltmain.sh (GNU libtool) 1.3.3 (1.385.2.181 1999/07/02 15:49:11)
# ltmain.sh (GNU libtool 1.1361 2004/01/02 23:10:52) 1.5a
# output is multiline from 1.5 onwards

# Require libtool 1.4 or newer
libtool=`build/PrintPath glibtool libtool libtool15 libtool14`
lt_pversion=`$libtool --version 2>/dev/null|sed -e 's/([^)]*)//g;s/^[^0-9]*//;s/[- ].*//g;q'`
if test -z "$lt_pversion"; then
echo "buildconf: libtool not found."
echo "           You need libtool version 1.4 or newer installed"
echo "           to build APR from SVN."
exit 1
fi
lt_version=`echo $lt_pversion|sed -e 's/\([a-z]*\)$/.\1/'`
IFS=.; set $lt_version; IFS=' '
lt_status="good"
if test "$1" = "1"; then
   if test "$2" -lt "4"; then
      lt_status="bad"
   fi
fi
if test $lt_status = "good"; then
   echo "buildconf: libtool version $lt_pversion (ok)"
   exit 0
fi

echo "buildconf: libtool version $lt_pversion found."
echo "           You need libtool version 1.4 or newer installed"
echo "           to build APR from SVN."

exit 1
