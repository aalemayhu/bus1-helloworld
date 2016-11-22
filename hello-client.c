/*
 * Copyright (C) 2013-2016 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include <errno.h>
#include <linux/bus1.h>
#include <sys/ioctl.h>

#include "include/helper.h"

int main(int argc, const char *argv[])
{
	struct bus1_cmd_recv cmd_recv;
	const uint8_t *map1;
	size_t n_map1;
	int fd1;

	printf("hello client\n");

	fd1 = test_open(&map1, &n_map1);
	printf("fd1=%d\n",fd1);
	if (0 > fd1) {
		fprintf(stderr, "failed to open the character device %s\n",
			CHAR_DEVICE);
		return EXIT_FAILURE;
	}

	cmd_recv = (struct bus1_cmd_recv){
		.flags = 0,
		.max_offset = n_map1,
	};

	while (1) {
		if (0 > bus1_ioctl_recv(fd1, &cmd_recv))
			perror("hello-client");
		else
			printf("to be implemented");
	}

	test_close(fd1, map1, n_map1);

	return EXIT_SUCCESS;
}
