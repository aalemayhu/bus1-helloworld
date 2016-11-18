#!/bin/bash
# Should be in the top level directory!

assert() {
  program_name=$1
  actual=`./$1`
  expected=$2

  if [[ $expected != $actual ]]; then
    echo "$program_name: fail"
    echo "expected \($expected) got \($actual)"
  else
    echo "$program_name: pass"
  fi
}

make clean -s
make -s

assert "hello-client" "hello client"
assert "hello-server" "hello server"
