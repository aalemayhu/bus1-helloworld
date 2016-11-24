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
	const uint8_t *map1;
	size_t n_map1;
	int fd;

	fd = test_open(&map1, &n_map1);
	printf("fd=%d\n", fd);
	if (0 > fd)
		perror("test_open");
	test_close(fd, map1, n_map1);

	return EXIT_SUCCESS;
}

