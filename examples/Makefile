CC = $(CROSS_COMPILE)gcc
CFLAGS += -D_FILE_OFFSET_BITS=64 -Wall -g -O2
CFLAGS += -I../../bus1/include/uapi/
CFLAGS += -I../../bus1/include/
CFLAGS += -I../../bus1/usr/include/
CFLAGS += -ljansson
CFLAGS += -lcurl
CONTAINER_NAME=scanf/bus1-helloworld
BUS1_REPO ?=https://github.com/bus1/bus1
BUS1_DIR ?=../../bus1

PROGS := peer-query 			\
	 ping-pong  			\
	 fork-a-b   			\
	 send-recv-numbers 		\
	 rpd				\
	 peek				\
	 print-cmds

all: $(PROGS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

analyze:
	clang $(CFLAGS) --analyze *.c

te%:
	tests/run

tt: clean
	if ! test -d $(BUS1_DIR); then \
	  git clone $(BUS1_REPO) $(BUS1_DIR); \
	fi
	$(MAKE) test

clean:
	$(RM) $(PROGS)

container:
	docker build -t ${CONTAINER_NAME} .
container_deploy: container container_push
	echo "Pushed to docker"
container_run: container
	docker run -t -i ${CONTAINER_NAME} /bin/bash
container_push:
	docker push ${CONTAINER_NAME}

.PHONY: tests
