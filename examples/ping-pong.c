/*
 * Copyright (C) 2013-2016 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <linux/bus1.h>

#define BUS1_DEFAULT_SLICES_MAX (16384)

int main(int argc, const char *argv[])
{
	char *data = "ping";
	struct iovec vec = { data, strlen(data) };
	struct bus1_cmd_send cmd_send;
	struct bus1_cmd_recv cmd_recv;
	int fd, ping_count, i;
	uint64_t id = 0x100;
	const uint8_t *map;
	const size_t n_map = 16UL * 1024UL * 1024UL;

	ping_count = argc > 1 ? atoi(argv[1]) : 1;

	if (ping_count > BUS1_DEFAULT_SLICES_MAX) {
		printf("sorry, %d is greater than the default %d.\n",
		       ping_count, BUS1_DEFAULT_SLICES_MAX);
		printf("Running it could exhausted your quota.\n");
		printf("defaulting to 1");
		ping_count = 1;
	}

	fd = open("/dev/bus1", O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);
	map = mmap(NULL, n_map, PROT_READ, MAP_PRIVATE, fd, 0);
	cmd_send = (struct bus1_cmd_send){
		.ptr_destinations	= (unsigned long)&id,
		.n_destinations		= 1,
		.ptr_vecs		= (unsigned long)&vec,
		.n_vecs			= 1,
	};

	for (i = 0; i < ping_count; i++) {
		if (!ioctl(fd, BUS1_CMD_SEND, &cmd_send))
			continue;
		perror("bus1_ioctl_send");
		exit(EXIT_FAILURE);
	}

	cmd_recv = (struct bus1_cmd_recv){
		.flags = 0,
		.max_offset = n_map,
	};
	while (ioctl(fd, BUS1_CMD_RECV, &cmd_recv) != -EAGAIN && ping_count > 0) {
		if (BUS1_MSG_DATA == cmd_recv.msg.type)
			printf("BUS1_MSG_DATA\n");
		else
			break;
		char *msg_data = (char *)map + cmd_recv.msg.offset;
		printf("%s\n", msg_data);

		if (!strcmp(msg_data, "ping")) {
			printf("pong\n");
			ping_count--;
		}
	}

	munmap((void *)map, n_map);
	close(fd);

	return EXIT_SUCCESS;
}
