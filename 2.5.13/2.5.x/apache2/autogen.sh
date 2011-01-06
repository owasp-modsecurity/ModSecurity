#!/bin/sh

rm -rf autom4te.cache

#automake --add-missing --copy
autoreconf --install
