/*
 * Open image links from a subreddit.
 *
 * Using bus1 to communicate between the processes.  The URLs fetched from
 * Reddit are sent as messages in the child process and read by the parent. The
 * parent opens any link received by calling xdg-open on.
 *
 * JSON parsing from jansson example[0].
 * https://github.com/akheron/jansson/tree/master/doc
 */
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>

#include <curl/curl.h>
#include <jansson.h>
#include <linux/bus1.h>

#define SIZE (16UL * 1024UL * 1024UL)
#define BUFFER_SIZE  (1024 * 1024)
#define URL_FORMAT "https://www.reddit.com/r/%s/.json"

int bus1_send_string(int fd, uint64_t id, char *msg);

int open_links(int fd, const uint8_t *map, size_t n_map);
int fetch_links(int fd, uint64_t id, char *url);

static size_t write_res(void *ptr, size_t size, size_t nmemb, void *stream);
static char *request(const char *url);

struct write_result {
	char *data;
	int pos;
};

int main(int argc, char **argv)
{
	const size_t n_map = SIZE;
	uint64_t id = 0x100;
	const uint8_t *map;
	char *subreddit;
	int r, fd;

	subreddit = argc > 1 ? argv[1] : "cats";

	char url[strlen(subreddit)+strlen(URL_FORMAT)];
	sprintf(url, URL_FORMAT, subreddit);

	fd = open("/dev/bus1", O_RDWR | O_CLOEXEC | O_NONBLOCK | O_NOCTTY);
	map = mmap(NULL, n_map, PROT_READ, MAP_PRIVATE, fd, 0);

	if (0 > fd)
		perror("test_open");

	r = EXIT_SUCCESS;

	switch (fork()) {
	case 0:
		r = fetch_links(fd, id, url);
		if (r != EXIT_SUCCESS)
			perror("fetch_links");
		break;
	default:
		r = open_links(fd, map, n_map);
		break;
	}

	munmap((void *)map, n_map);
	close(fd);

	return r;
}

int open_links(int fd, const uint8_t *map, size_t n_map)
{
	struct bus1_cmd_recv cmd_recv;
	int count, read_count;
	char *msg;

	// First message should tell us how many links we will be reading.
	do {
		cmd_recv = (struct bus1_cmd_recv){
			.flags = 0,
			.max_offset = n_map,
		};
		if (ioctl(fd, BUS1_CMD_RECV, &cmd_recv) == -EAGAIN)
			continue;
		if (cmd_recv.msg.type == BUS1_MSG_NONE)
			continue;
		msg = (char *)map + cmd_recv.msg.offset;
	} while (cmd_recv.msg.type != BUS1_MSG_DATA);

	if (!strcmp(msg, "error")) {
		printf("%s: %s\n", __func__, msg);
		return EXIT_FAILURE;
	}

	count = atoi(msg);
	printf("will read %d messages\n",
	       count);

	read_count = 0;
	while (count > read_count) {
		cmd_recv = (struct bus1_cmd_recv){
			.flags = 0,
			.max_offset = n_map,
		};
		if (ioctl(fd, BUS1_CMD_RECV, &cmd_recv) == -EAGAIN)
			continue;
		if (cmd_recv.msg.type == BUS1_MSG_NONE)
			continue;
		read_count++;

		char *url = (char *)map + cmd_recv.msg.offset;

		if (strstr(url, "imgur") == NULL
		    && strstr(url, ".png") == NULL) {
			printf("does not contain imgur or .png will skip: %s\n", url);
			continue;
		}

		char  *suffix = " > /dev/null 2>/dev/null";
		char *prefix = "xdg-open ";
		char cmd[strlen(prefix) + strlen(url) + strlen(suffix)];
		sprintf(cmd, "%s '%s' %s", prefix, url, suffix);
		printf("system(\"%s\")\n", cmd);
		system(cmd);
	}

	printf("open_links END\n");
	return EXIT_SUCCESS;
}

int fetch_links(int fd, uint64_t id, char *url)
{
	json_t *root, *children;
	json_error_t error;
	char *text;
	size_t i, count;

	text = request(url);
	if(!text) {
		bus1_send_string(fd, id, "error");
		return EXIT_FAILURE;
	}

	root = json_loads(text, 0, &error);
	free(text);

	if(!root) {
		fprintf(stderr, "error: on line %d: %s\n", error.line,
			error.text);
		return EXIT_FAILURE;
	}

	children = json_object_get(json_object_get(root, "data"), "children");

	if(!json_is_array(children)) {
		fprintf(stderr, "error: root is not an array\n");
		json_decref(root);
		return EXIT_FAILURE;
	}

	// Sending the count to the bus
	count = json_array_size(children);
	char data_count[sizeof(int)];
	snprintf(data_count, sizeof(int), "%lu", count);
	bus1_send_string(fd, id, data_count);

	for(i = 0; i < count; i++) {
		json_t *url, *title, *data;
		data = json_object_get(json_array_get(children, i), "data");
		if(!json_is_object(data)) {
			fprintf(stderr, "error: child data %d is not an object\n", (int)(i + 1));
			json_decref(root);
			return EXIT_FAILURE;
		}

		url = json_object_get(data, "url");
		if(!json_is_string(url)) {
			fprintf(stderr, "error: child %d: url is not a string\n", (int)(i + 1));
			return EXIT_FAILURE;
		}

		title = json_object_get(data, "title");
		if(!json_is_string(title)) {
			fprintf(stderr, "error: title %d: title is not a string\n", (int)(i + 1));
			json_decref(root);
			return EXIT_FAILURE;
		}

		char *msg = (char *)json_string_value(url);
		if (0 > bus1_send_string(fd, id, msg))
			perror("cmd_send");
	}

	json_decref(root);

	return EXIT_SUCCESS;
}

int bus1_send_string(int fd, uint64_t id, char *msg) {
	printf("bus1_send_string(%d, %lu, %s)\n", fd, id, msg);
	struct bus1_cmd_send cmd_send;
	struct iovec vec = { msg, strlen(msg) };
	cmd_send = (struct bus1_cmd_send){
		.ptr_destinations	= (unsigned long)&id,
			.n_destinations	= 1,
			.ptr_vecs		= (unsigned long)&vec,
			.n_vecs		= 1,
	};
	return ioctl(fd, BUS1_CMD_SEND, &cmd_send);
}

static size_t write_res(void *ptr, size_t size, size_t nmemb, void *stream)
{
	struct write_result *result = (struct write_result *)stream;

	if(result->pos + size * nmemb >= BUFFER_SIZE - 1) {
		fprintf(stderr, "error: too small buffer\n");
		return 0;
	}

	memcpy(result->data + result->pos, ptr, size * nmemb);
	result->pos += size * nmemb;

	return size * nmemb;
}

static char *request(const char *url)
{
	CURL *curl = NULL;
	CURLcode status;
	struct curl_slist *headers = NULL;
	char *data = NULL;
	long code;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(!curl)
		goto error;

	data = malloc(BUFFER_SIZE);
	if(!data)
		goto error;

	struct write_result write_result = {
		.data = data,
		.pos = 0
	};

	curl_easy_setopt(curl, CURLOPT_URL, url);

	headers = curl_slist_append(headers, "User-Agent: rpd");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_res);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

	status = curl_easy_perform(curl);
	if(status != 0) {
		fprintf(stderr, "error: unable to request data from %s:\n",
			url);
		fprintf(stderr, "%s\n", curl_easy_strerror(status));
		goto error;
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
	if(code != 200) {
		fprintf(stderr, "error: server responded with code %ld\n",
			code);
		goto error;
	}

	curl_easy_cleanup(curl);
	curl_slist_free_all(headers);
	curl_global_cleanup();

	/* zero-terminate the result */
	data[write_result.pos] = '\0';

	return data;

error:
	if(data)
		free(data);
	if(curl)
		curl_easy_cleanup(curl);
	if(headers)
		curl_slist_free_all(headers);
	curl_global_cleanup();
	return NULL;
}
