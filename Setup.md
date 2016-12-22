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
