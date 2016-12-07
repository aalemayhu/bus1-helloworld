A small collection of programs using bus1.

## Setup

### Fedora

    dnf install -y git kernel-devel-`uname -r` gcc

### Debian

TODO: fill out

### Installing bus1

    git clone https://github.com/bus1/bus1 ../bus1
    cd ../bus1
    make tt # You need enough priviliges to install the module
    cd -

## Examples

- [fork-a-b][fork-a-b] - sending message from parent and receiving message in child.
- [peer-query][peer-query] - reset a peer context.
- [ping-pong][ping-pong] - send and receive message in the same process.
- [rpd][rpd] - use reddit api to fetch links and open them.
- [send-recv-numbers][send-recv-numbers] - send more messages.

TODO: daemon
TODO: independent-process

[fork-a-b]: ./fork-a-b.c
[peer-query]: ./peer-query.c
[ping-pong]: ./ping-pong.c
[rpd]: ./rpd.c
[send-recv-numbers]: ./send-recv-numbers.c
