/*
 * Copyright (C) 2013-2016 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include <pthread.h>

#include "include/helper.h"

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
	size_t n_map;

	ping_count = argc > 1 ? atoi(argv[1]) : 1;

	if (ping_count > BUS1_DEFAULT_SLICES_MAX) {
		printf("sorry, %d is greater than the default %d.\n",
		       ping_count, BUS1_DEFAULT_SLICES_MAX);
		printf("Running it could exhausted your quota.\n");
		printf("defaulting to 1");
		ping_count = 1;
	}

	fd = test_open(&map, &n_map);
	cmd_send = (struct bus1_cmd_send){
		.ptr_destinations	= (unsigned long)&id,
		.n_destinations		= 1,
		.ptr_vecs		= (unsigned long)&vec,
		.n_vecs			= 1,
	};

	for (i = 0; i < ping_count; i++) {
		if (bus1_ioctl_send(fd, &cmd_send))
			fail("bus1_ioctl_send");
	}

	cmd_recv = (struct bus1_cmd_recv){
		.flags = 0,
		.max_offset = n_map,
	};
	while (bus1_ioctl_recv(fd, &cmd_recv) != -EAGAIN
	       || ping_count > 0) {
		char *msg_data = (char *)map + cmd_recv.msg.offset;
		log_msg_type(cmd_recv);
		printf("%s\n", msg_data);

		if (!strcmp(msg_data, "ping")) {
			printf("pong\n");
			ping_count--;
		}
	}

	test_close(fd, map, n_map);

	return EXIT_SUCCESS;
}
