#include <stdio.h>

#include <linux/bus1.h>

int main(int argc, const char *argv[])
{
	printf("BUS1_CMD_PEER_DISCONNECT=%d\n" ,BUS1_CMD_PEER_DISCONNECT);
	printf("BUS1_CMD_PEER_QUERY=%d\n", BUS1_CMD_PEER_QUERY);
	printf("BUS1_CMD_PEER_RESET=%d\n", BUS1_CMD_PEER_RESET);
	printf("BUS1_CMD_HANDLE_RELEASE=%d\n", BUS1_CMD_HANDLE_RELEASE);
	printf("BUS1_CMD_HANDLE_TRANSFER=%d\n", BUS1_CMD_HANDLE_TRANSFER);
	printf("BUS1_CMD_NODES_DESTROY=%d\n", BUS1_CMD_NODES_DESTROY);
	printf("BUS1_CMD_SLICE_RELEASE=%d\n", BUS1_CMD_SLICE_RELEASE);
	printf("BUS1_CMD_SEND=%d\n", BUS1_CMD_SEND);
	printf("BUS1_CMD_RECV=%d\n", BUS1_CMD_RECV);
}
