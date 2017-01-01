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

## Quick setup

If you are not interested in running on bare metal up or just want a test bed.
The nice folks at [Dply](https://dply.co/) offer free VPS for 2 hours. Bus1
compiles with their Fedora 25 image. You can preconfigure a instance with
[![Dply](https://dply.co/b.svg)](https://dply.co/b/0eFTNyjS). Then after
you have ssh access you can compile with:

    export SRC_DIR=/src/github.com/scanf
    make -C $SRC_DIR/bus1 tt
    make -C $SRC_DIR/bus1-helloworld/examples tt
