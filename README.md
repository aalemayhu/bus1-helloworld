# bus1-helloworld

WIP: A simple example project for a bus1 project.

## Setup

    git clone https://github.com/bus1/bus1 ../bus1
    # TODO: list dependencies required for Fedora and Debian based
    # distrobutions.
    make tests

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
