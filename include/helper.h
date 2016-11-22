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
#include <linux/bus1.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "bus1-ioctl.h"

#define CHAR_DEVICE "/dev/bus1"

#define CLIENT_PID_FILE "/tmp/hello-client.pid"

static inline int test_open(const uint8_t **mapp, size_t *n_mapp)
{
	const size_t size = 16UL * 1024UL * 1024UL;
	int fd;

	fd = open(CHAR_DEVICE, O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);

	*mapp = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0);

	*n_mapp = size;
	return fd;
}

static inline void test_close(int fd, const uint8_t *map, size_t n_map)
{
	munmap((void *)map, n_map);
	close(fd);
}

void fail(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void must_save_pid(char *path)
{
	FILE *f;
	f = fopen(CLIENT_PID_FILE, "w");
	if (f == NULL)
		fail("fopen");
	fprintf(f, "%d", getpid());
	if(0 > fclose(f))
		perror("fclose");
}

void recv_pid(char *path, pid_t *pid)
{
  FILE *f = fopen (path, "r");
  if (f == NULL)
	  perror("fopen");
  fscanf (f, "%d", pid);
  fclose (f);
}
