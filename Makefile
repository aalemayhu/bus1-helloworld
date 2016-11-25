CC = $(CROSS_COMPILE)gcc
CFLAGS += -D_FILE_OFFSET_BITS=64 -Wall -g -O2
CFLAGS += -I../bus1/include/uapi/
CFLAGS += -I../bus1/include/
CFLAGS += -I../bus1/usr/include/

PROGS := hello-server hello-client peer-query ping-pong fork-a-b

all: $(PROGS)

%: %.c ../bus1/usr/include/linux/bus1.h
	$(CC) $(CFLAGS) $< -o $@

t%:
	tests/run

s: clean 
	$(MAKE) all

clean:
	$(RM) $(PROGS)

.PHONY: tests
