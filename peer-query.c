/*
 * Copyright (C) 2013-2016 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <linux/bus1.h>

int main(int argc, const char *argv[])
{
	struct bus1_cmd_peer_reset query;
	const uint8_t *map;
	size_t size;
	int fd;

	size = 16UL * 1024UL * 1024UL;
	fd = open("/dev/bus1", O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);
	map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

	if (0 > fd) {
		perror("test_open");
		goto error;
	}

	if (0 > ioctl(fd, BUS1_CMD_PEER_QUERY, &query)) {
		perror("bus1_ioctl_peer_query");
		munmap((void *)map, size);
		close(fd);
		goto error;
	}

	printf("flags = %llu\n", query.flags);
	printf("peer_flags = %llu\n", query.peer_flags);
	printf("max_slices = %u\n", query.max_slices);
	printf("max_handles = %u\n", query.max_handles);
	printf("max_inflight_bytes = %u\n", query.max_inflight_bytes);
	printf("max_inflight_fds = %u\n", query.max_inflight_fds);

	munmap((void *)map, size);
	close(fd);

	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}

