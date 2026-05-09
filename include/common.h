#pragma once
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include "my_queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define CLIENT_PORT 80
#define SERVER_PORT 18000
#define SockAddr struct sockaddr
#define SERVER_BACKLOG 100
#define THREAD_BOOL_MAX 10

int format_error(const char* fmt, ...);
char* bin_to_hex(char* bin_fmt, size_t len);
