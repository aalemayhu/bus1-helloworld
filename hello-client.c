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
	FILE *f;

	f = fopen(CLIENT_PID_FILE, "w");
	if (f == NULL)
		fail("fopen");
	fprintf(f, "%d", getpid());
	if(0 > fclose(f))
		perror("fclose");

	fd = test_open(&map1, &n_map1);
	if (0 > fd)
		fail("failed to open the character device");

	cmd_recv = (struct bus1_cmd_recv){
		.flags = 0,
		.max_offset = n_map1,
	};

	while (0) {
		if (0 > bus1_ioctl_recv(fd, &cmd_recv))
			perror("hello-client");
		else
			printf("got %llu bytes", cmd_recv.msg.n_bytes);
	}

	test_close(fd, map1, n_map1);
	unlink(CLIENT_PID_FILE);

	return EXIT_SUCCESS;
}
