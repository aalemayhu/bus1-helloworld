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
	struct bus1_cmd_peer_reset query;
	const uint8_t *map1;
	size_t n_map1;
	int fd;

	fd = test_open(&map1, &n_map1);

	if (0 > fd)
		fail("test_open");

	if (0 > bus1_ioctl_peer_query(fd, &query))
		fail("bus1_ioctl_peer_query");

	printf("flags = %llu\n", query.flags);
	printf("peer_flags = %llu\n", query.peer_flags);
	printf("max_slices = %u\n", query.max_slices);
	printf("max_handles = %u\n", query.max_handles);
	printf("max_inflight_bytes = %u\n", query.max_inflight_bytes);
	printf("max_inflight_fds = %u\n", query.max_inflight_fds);

	test_close(fd, map1, n_map1);

	return EXIT_SUCCESS;
}

