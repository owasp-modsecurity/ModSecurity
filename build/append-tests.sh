#!/bin/sh

printf "TESTS=" >> Makefile.am

T=$(find test/test-cases -iname *.json)

echo $T >> Makefile.am
