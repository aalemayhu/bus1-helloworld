FROM fedora

MAINTAINER Alexander Alemayhu <alexander@alemayhu.com>

RUN dnf install -y git gcc
#kernel-devel-`uname -r`
RUN dnf install -y jansson-devel libcurl-devel

ENV SRC_DIR /src/github.com/
RUN mkdir -pv $SRC_DIR
RUN git clone https://github.com/bus1/bus1 $SRC_DIR/bus1/
RUN git clone https://github.com/scanf/bus1-helloworld $SRC_DIR/bus1-helloworld
