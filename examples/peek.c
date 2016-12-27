#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/bus1.h>

struct peer_context {
	const uint8_t *map;
	size_t n_map;
	int fd;
};

int send_empty_msg(struct peer_context *context)
{
        struct bus1_cmd_send cmd_send;
	uint64_t id = 0x100;
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

	return ioctl(context->fd, BUS1_CMD_SEND, &cmd_send);
}

int recv_emtpy_msg(struct peer_context *context, struct bus1_cmd_recv *cmd_recv,
		   int flags)
{
	*cmd_recv = (struct bus1_cmd_recv){
		.flags = flags,
		.max_offset = context->n_map,
	};
	return ioctl(context->fd, BUS1_CMD_RECV, cmd_recv);
}

int main(int argc, const char *argv[])
{
        struct bus1_cmd_recv cmd_recv;
	struct peer_context *context = malloc(sizeof *context);

	context->fd = open("/dev/bus1", O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);
	context->n_map = 1024;
	context->map = mmap(NULL, context->n_map, PROT_READ, MAP_PRIVATE, context->fd, 0);

	if (0 > context->fd) {
		perror("open");
		free(context);
		goto error;
	}

	// BUS1_MSG_DATA
	assert(!send_empty_msg(context));
	assert(!recv_emtpy_msg(context, &cmd_recv, 0));
	if (BUS1_MSG_DATA == cmd_recv.msg.type)
		printf("BUS1_MSG_DATA\n");


	// BUS1_MSG_NONE
	assert(recv_emtpy_msg(context, &cmd_recv, 0));
	if (BUS1_MSG_NONE == cmd_recv.msg.type)
		printf("BUS1_MSG_NONE\n");

	// BUS1_MSG_DATA
	assert(!send_empty_msg(context));
	assert(!recv_emtpy_msg(context, &cmd_recv, BUS1_RECV_FLAG_PEEK));
	if (BUS1_MSG_DATA == cmd_recv.msg.type)
		printf("BUS1_MSG_DATA\n");
	assert(!recv_emtpy_msg(context, &cmd_recv, 0));
	if (BUS1_MSG_DATA == cmd_recv.msg.type)
		printf("BUS1_MSG_DATA\n");

	munmap((void *)context->map, context->n_map);
	close(context->fd);
	free(context);
	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}
