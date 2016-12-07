CC = $(CROSS_COMPILE)gcc
CFLAGS += -D_FILE_OFFSET_BITS=64 -Wall -g -O2
CFLAGS += -I../bus1/include/uapi/
CFLAGS += -I../bus1/include/
CFLAGS += -I../bus1/usr/include/
CFLAGS += -ljansson
CFLAGS += -lcurl
CONTAINER_NAME=scanf/bus1-helloworld

PROGS := peer-query 			\
	 ping-pong  			\
	 fork-a-b   			\
	 send-recv-numbers 		\
	 send-recv-numbers		\
	 rpd

all: $(PROGS)

%: %.c ../bus1/usr/include/linux/bus1.h
	$(CC) $(CFLAGS) $< -o $@

te%:
	tests/run

tt: clean
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
