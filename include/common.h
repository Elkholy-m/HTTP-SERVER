#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
    int conn_channel;
    char *read_buffer;
    char *write_buffer;
} handle_request_args;

#define BUFFER_SIZE 4096
#define CLIENT_PORT 80
#define SERVER_PORT 18000
#define SockAddr struct sockaddr

int format_error(const char* fmt, ...);
char* bin_to_hex(char* bin_fmt, size_t len);
