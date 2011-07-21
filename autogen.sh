#!/bin/sh

#rm -rf autom4te.cache
#automake --add-missing --copy --foreign
#autoreconf --install
#autoheader

rm -rf autom4te.cache
rm -f aclocal.m4
libtoolize --force --copy
autoreconf --install
autoheader
automake --add-missing --foreign --copy --force-missing
autoconf --force
rm -rf autom4te.cache

