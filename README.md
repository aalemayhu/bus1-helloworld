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

## Resources

### Reading Material

- [Bus1: a new Linux interprocess communication proposal](https://lwn.net/Articles/697191/)
- [Ksummit-discuss](https://lists.linuxfoundation.org/pipermail/ksummit-discuss/2016-July/003047.html)
- [Playing with Bus1](http://blog.peter-b.co.uk/2016/10/playing-with-bus1.html)
- [RFC v1](http://lkml.iu.edu/hypermail/linux/kernel/1610.3/02995.html)
- [The Internal Bus1 API](http://www.bus1.org/bus1.kernel-api.html)
- [bus1 — Kernel Message Bus](http://www.bus1.org/bus1.html)
- [bus1/documentation/wiki](https://github.com/bus1/documentation/wiki)

### Presentations

- David Herrmann: Capability-based IPC on Linux, [Video](https://www.youtube.com/watch?v=6zN0b6BfgLY)
- David Herrmann and Tom Gundersen: Capability-based IPC on Linux, [Slides](http://linuxplumbersconf.org/2016/ocw//system/presentations/3819/original/bus3.pdf)
