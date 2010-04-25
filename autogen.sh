#!/bin/sh

rm -rf autom4te.cache

automake --add-missing --copy --foreign
autoreconf --install
autoheader
