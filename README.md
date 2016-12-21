A small collection of programs using bus1.

## Setup

Other than the regular developer tools the only extra dependencies are
[Jansson](https://jansson.readthedocs.io/en/2.9/) and
[libcurl](https://curl.haxx.se/libcurl/). The below recipe should cover the
requirements to build the examples.

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
- [print-cmds][print-cmds] - print out the bus1 specific commands.

<hr>

- TODO: daemon
- TODO: independent-process
- TODO: three peers

## Reading Material

- [Bus1: a new Linux interprocess communication proposal](https://lwn.net/Articles/697191/)
- [Ksummit-discuss](https://lists.linuxfoundation.org/pipermail/ksummit-discuss/2016-July/003047.html)
- [Playing with Bus1](http://blog.peter-b.co.uk/2016/10/playing-with-bus1.html)
- [RFC v1](http://lkml.iu.edu/hypermail/linux/kernel/1610.3/02995.html)
- [The Internal Bus1 API](http://www.bus1.org/bus1.kernel-api.html)
- [bus1 — Kernel Message Bus](http://www.bus1.org/bus1.html)
- [bus1/documentation/wiki](https://github.com/bus1/documentation/wiki)

[0]: https://github.com/bus1/bus1/search?utf8=%E2%9C%93&q=BUS1_RECV_FLAG_PEEK
[fork-a-b]: ./examples/fork-a-b.c
[peer-query]: ./examples/peer-query.c
[ping-pong]: ./examples/ping-pong.c
[rpd]: ./examples/rpd.c
[send-recv-numbers]: ./examples/send-recv-numbers.c
[peek]: ./examples/peek.c
[print-cmds]: ./examples/print-cmds.c
