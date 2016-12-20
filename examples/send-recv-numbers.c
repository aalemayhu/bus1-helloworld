#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <linux/bus1.h>


int main(int argc, const char *argv[])
{
	int fd, i, read_count, expected;
	struct bus1_cmd_send cmd_send;
	struct bus1_cmd_recv cmd_recv;
	uint64_t id = 0x100;
	const uint8_t *map;
	const size_t size = 16UL * 1024UL * 1024UL;

	expected = argc > 1 ? atoi(argv[1]) : 11;
	fd = open("/dev/bus1", O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);
	map = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
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
			  if (BUS1_MSG_DATA == cmd_recv.msg.type)
				  printf("BUS1_MSG_DATA\n");
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

	munmap((void *)map, size);
	close(fd);

	return EXIT_SUCCESS;
}
