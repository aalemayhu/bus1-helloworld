#!/bin/bash

source tests/lib.bash
program=peer-query
expected="flags = 0
peer_flags = 0
max_slices = 16384
max_handles = 65536
max_inflight_bytes = 16777216
max_inflight_fds = 4096"

assert $program  "$expected"
