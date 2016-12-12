A small collection of programs using bus1.

## Setup

### Fedora

    dnf install -y git kernel-devel-`uname -r` gcc
    dnf install -y jansson-devel libcurl-devel

### Debian

    apt-get update
    apt-get install -y git build-essential linux-headers-$(uname -r)
    apt-get install -y libjansson-dev libcurl4-gnutls-dev

Depending on which kernel you have and library versions, you might experience
errors triggered by outdated versions. The recipe above has been tested with
kernel (4.9.0) and gcc (Debian 6.2.1-5).

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
- [peek][peek] - peek message.

<hr>

- TODO: daemon
- TODO: independent-process
- TODO: three peers

[0]: https://github.com/bus1/bus1/search?utf8=%E2%9C%93&q=BUS1_RECV_FLAG_PEEK
[fork-a-b]: ./fork-a-b.c
[peer-query]: ./peer-query.c
[ping-pong]: ./ping-pong.c
[rpd]: ./rpd.c
[send-recv-numbers]: ./send-recv-numbers.c
[peek]: ./peek.c
