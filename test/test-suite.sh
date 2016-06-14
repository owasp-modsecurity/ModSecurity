#!/bin/bash

TEST=$1 1> /dev/null
cd test 1> /dev/null
if [[ $TEST == *"test-cases/regression/"* ]]
then
      ./regression_tests ../$*
else
      ./unit_tests ../$*
fi
cd - 1> /dev/null
