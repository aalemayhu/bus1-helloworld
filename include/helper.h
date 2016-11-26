#ifndef __HELPER_H
#define __HELPER_H

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
#include <signal.h>
#include "bus1-ioctl.h"

#define CHAR_DEVICE "/dev/bus1"

#define CLIENT_PID_FILE "/tmp/hello-client.pid"

static inline int helper_open(char *path, const uint8_t **mapp, size_t *n_mapp)
{
	const size_t size = 16UL * 1024UL * 1024UL;
	int fd;

	fd = open(path, O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);

	*mapp = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);

	*n_mapp = size;
	return fd;
}

static inline int test_open(const uint8_t **mapp, size_t *n_mapp)
{
	return helper_open(CHAR_DEVICE, mapp, n_mapp);
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

void log_cmd_recv(struct bus1_cmd_recv cmd)
{
	printf("struct bus1_cmd_recv {\n");
	printf("        __u64 flags = %llu\n", cmd.flags);
	printf("        __u64 max_offset = %llu\n", cmd.max_offset);
	printf("        struct {\n");
	printf("                __u64 type = %llu\n", cmd.msg.type);
	printf("                __u64 flags = %llu\n", cmd.msg.flags);
	printf("                __u64 destination = %llu\n", cmd.msg.destination);
	printf("                __u32 uid = %u\n", cmd.msg.uid);
	printf("                __u32 gid = %u\n", cmd.msg.gid);
	printf("                __u32 pid = %u\n", cmd.msg.pid);
	printf("                __u32 tid = %u\n", cmd.msg.tid);
	printf("                __u64 offset = %llu\n", cmd.msg.offset);
	printf("                __u64 n_bytes = %llu\n", cmd.msg.n_bytes);
	printf("                __u64 n_handles = %llu\n", cmd.msg.n_handles);
	printf("                __u64 n_fds = %llu\n", cmd.msg.n_fds);
	printf("                __u64 n_secctx = %llu\n", cmd.msg.n_fds);
	printf("        } msg;\n");
	printf("};");
}

void log_msg_type(struct bus1_cmd_recv cmd)
{
	uint64_t type = cmd.msg.type;
	if (BUS1_MSG_NONE == type)
		printf("BUS1_MSG_NONE\n");
	else if (BUS1_MSG_DATA == type)
		printf("BUS1_MSG_DATA\n");
	if (BUS1_MSG_NODE_DESTROY == type)
		printf("BUS1_MSG_NODE_DESTROY\n");
	if (BUS1_MSG_NODE_RELEASE == type)
		printf("BUS1_MSG_NODE_RELEASE\n");
}

static void signal_handler(int s)
{
	printf("Got signal: %d, which means we waited too long.", s);
	exit(EXIT_FAILURE);
}

static inline void set_alarm(int seconds)
{
	signal(SIGALRM, signal_handler);
	alarm(2);

}
#endif /* __HELPER_H */
