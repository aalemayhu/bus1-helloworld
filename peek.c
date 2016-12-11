#include <stdio.h>

#include "include/helper.h"

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

	return bus1_ioctl_send(context->fd, &cmd_send);
}

int recv_emtpy_msg(struct peer_context *context, struct bus1_cmd_recv *cmd_recv,
		   int flags)
{
	*cmd_recv = (struct bus1_cmd_recv){
		.flags = flags,
		.max_offset = context->n_map,
	};
	return bus1_ioctl_recv(context->fd, cmd_recv);
}

int main(int argc, const char *argv[])
{
        struct bus1_cmd_recv cmd_recv;
	struct peer_context *context = malloc(sizeof *context);

	context->fd = test_open(&context->map, &context->n_map);

	if (0 > context->fd) {
		perror("open");
		goto error;
	}

	// BUS1_MSG_DATA
	assert(!send_empty_msg(context));
	assert(!recv_emtpy_msg(context, &cmd_recv, 0));
	log_msg_type(cmd_recv);


	// BUS1_MSG_NONE
	assert(recv_emtpy_msg(context, &cmd_recv, 0));
	log_msg_type(cmd_recv);

	// BUS1_MSG_DATA
	assert(!send_empty_msg(context));
	assert(!recv_emtpy_msg(context, &cmd_recv, BUS1_RECV_FLAG_PEEK));
	log_msg_type(cmd_recv);
	assert(!recv_emtpy_msg(context, &cmd_recv, 0));
	log_msg_type(cmd_recv);

	test_close(context->fd, context->map, context->n_map);
	free(context);
	return EXIT_SUCCESS;

error:
	return EXIT_FAILURE;
}
