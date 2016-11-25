#include "include/helper.h"

int main()
{
	struct bus1_cmd_send cmd_send;
	struct bus1_cmd_recv cmd_recv;
	char *data[] = { "a", "b" };
	uint64_t id = 0x100;
	const uint8_t *map;
	size_t n_map;
	int fd, i;

	fd = test_open(&map, &n_map);
	if (0 > fd)
		fail("test_open");

	switch(fork()) {
	case -1 : fail("fork failed\n");
	case 0:
		  for (i = 0; i < sizeof(data) / sizeof(*data); ++i) {
			  cmd_recv = (struct bus1_cmd_recv){
				  .flags = 0,
					  .max_offset = n_map,
			  };

			  if (0 > bus1_ioctl_recv(fd, &cmd_recv))
				  fail("bus1_ioctl_recv");

			  printf("%s\n", (char *)map + cmd_recv.msg.offset);
		  }
		  break;
	default:
		  for (i = 0; i < sizeof(data) / sizeof(*data); ++i) {
			  struct iovec vec = { data[i], strlen(data[i]) };
			  cmd_send = (struct bus1_cmd_send){
				  .ptr_destinations	= (unsigned long)&id,
					  .n_destinations	= 1,
					  .ptr_vecs		= (unsigned long)&vec,
					  .n_vecs		= 1,
			  };
			  if (0 > bus1_ioctl_send(fd, &cmd_send))
				  fail("send_msg");
		  }
		  break;
	}

	test_close(fd, map, n_map);

	return EXIT_SUCCESS;
}
