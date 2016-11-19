#include <linux/bus1.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CHAR_DEVICE "/dev/bus1"

int main(int argc, const char *argv[])
{
	printf("hello client");

	int peer = open(CHAR_DEVICE, O_RDONLY);
	if (0 > peer) {
		fprintf(stderr, "failed to open the character device %s\n",
			CHAR_DEVICE);
		return EXIT_FAILURE;
	}

	bool incoming_message = false;
	while (incoming_message) {
		char *msg = "";
		// TODO: Read messages from CHAR_DEVICE
		if (!strcmp(msg, "quit"))
			break;
		printf("> %s", msg);
	}
	close(peer);

	return EXIT_SUCCESS;
}
