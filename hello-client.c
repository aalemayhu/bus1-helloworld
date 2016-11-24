/*
 * Copyright (C) 2013-2016 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */


#include "include/helper.h"

int main(int argc, const char *argv[])
{
	struct bus1_cmd_recv cmd_recv;
	const uint8_t *map1;
	size_t n_map1;
	int fd;

	// TODO: Wait for server
	fd = test_open(&map1, &n_map1);
	cmd_recv = (struct bus1_cmd_recv){
		.flags = 0,
	};

	if (!bus1_ioctl_recv(fd, &cmd_recv))
		printf("got %llu bytes", cmd_recv.msg.n_bytes);
	else
		fail("bus1_ioctl_recv");

	return EXIT_SUCCESS;
}

