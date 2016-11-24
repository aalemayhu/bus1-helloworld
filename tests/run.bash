#!/bin/bash

source tests/lib.bash

test_dir="tests"

if [ ! -d $test_dir ]; then
  echo "error: Should be in the top level directory!"
  exit
fi

make clean -s
make -s

for t in $test_dir/test-*
do
  echo "$t"
  $t && pass ok || fail fail
done
