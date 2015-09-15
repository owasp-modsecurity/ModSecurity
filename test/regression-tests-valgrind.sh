#!/usr/bin/env bash

valgrind --tool=massif ./regression-tests $*
valgrind --leak-check=full --suppressions=./valgrind_suppressions.txt ./regression-tests $*
