#include "include/helper.h"

int main(int argc, const char *argv[])
{
	int fd, i, read_count, expected;
	struct bus1_cmd_send cmd_send;
	struct bus1_cmd_recv cmd_recv;
	uint64_t id = 0x100;
	const uint8_t *map;
	size_t size;

	set_alarm(3);

	expected = argc > 1 ? atoi(argv[1]) : 11;
	fd = test_open(&map, &size);
	read_count = 0;

	if (0 > fd)
		perror("test_open");

	switch(fork()) {
	case -1 : perror("fork failed\n");
	case 0:
		  while (expected > read_count) {
			  cmd_recv = (struct bus1_cmd_recv){
				  .flags = 0,
				  .max_offset = size,
			  };
			  if (ioctl(fd, BUS1_CMD_RECV, &cmd_recv) == -EAGAIN)
				  continue;
			  read_count++;
			  log_msg_type(cmd_recv);
			  printf("%s\n", (char *)map + cmd_recv.msg.offset);
		  }
		  break;
	default:
		  for (i = 0; i < expected; i++) {
			  char data[sizeof(int)];
			  snprintf(data, sizeof(int), "%d", i);
			  struct iovec vec = { data, strlen(data) };
			  cmd_send = (struct bus1_cmd_send){
				  .ptr_destinations	= (unsigned long)&id,
				  .n_destinations	= 1,
				  .ptr_vecs		= (unsigned long)&vec,
				  .n_vecs		= 1,
			  };
			  if (0 > ioctl(fd, BUS1_CMD_SEND, &cmd_send))
				  perror("send_msg");
		  }
		  break;
	}

	test_close(fd, map, size);

	return EXIT_SUCCESS;
}
