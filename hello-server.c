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
	struct bus1_cmd_send cmd_send;
	const uint8_t *map1;
	uint64_t id = 0x100;
	size_t n_map1;
	int fd;

	fd = test_open(&map1, &n_map1);
	cmd_send = (struct bus1_cmd_send){
		.flags			= 0,
		.ptr_destinations	= (unsigned long)&id,
		.ptr_errors		= 0,
		.n_destinations		= 1,
		.ptr_vecs		= 0,
		.n_vecs			= 0,
		.ptr_handles		= 0,
		.n_handles		= 0,
		.ptr_fds		= 0,
		.n_fds			= 0,
	};

	// TODO: Wait for client

	if (!bus1_ioctl_send(fd, &cmd_send))
		printf("cmd send\n");

	test_close(fd, map1, n_map1);

	return EXIT_SUCCESS;
}

