#!/bin/bash

cd test 1> /dev/null

length=$(($#-1))
array=${@:1:$length}

PARAM=$array
FILE=${@: -1}

if [[ $FILE == *"test-cases/regression/"* ]]
then
      $VALGRIND $PARAM ./regression_tests ../$FILE
      echo $VALGRIND $PARAM ./regression_tests ../$FILE
else
      $VALGRIND $PARAM ./unit_tests ../$FILE
fi
cd - 1> /dev/null
