#!/bin/bash

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

assert() {
  program_name=$1
  actual=`./$1`
  expected="$2"

  if [[ $expected != $actual ]]; then
    echo "expected \($expected) got \($actual)"
    exit 1
  fi
  exit 0
}

fail() {
  text=FAILED
  echo "${red}${text}${reset}"
}

pass() {
  text=PASS
  echo "${green}${text}${reset}"
}
