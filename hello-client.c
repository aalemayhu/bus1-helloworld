#include <linux/bus1.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char *argv[])
{
	printf("hello client");

	struct bus1_peer *peer = bus1_peer_new();

	bus1_peer_free(peer);

	return EXIT_SUCCESS;
}
