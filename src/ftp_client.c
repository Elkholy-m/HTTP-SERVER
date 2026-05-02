#include "../include/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int rand_range(int min, int max) {
    srand(time(NULL));
    float r_num = rand() / (float)RAND_MAX;
    int range_num = r_num * max + min;

    return range_num;
}

int main() {
    const char* server_ip = "127.0.0.1";
    struct sockaddr_in server_address;
    socklen_t server_addr_len = sizeof(server_address);
    int sock_fd, n_bytes;
    char read_buffer[BUFFER_SIZE], write_buffer[BUFFER_SIZE];
    int res;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        printf("error while creating socket\n");
        fflush(stdout);
        return -1;
    }

    memset(&server_address  , 0, server_addr_len);
    memset(read_buffer , 0, BUFFER_SIZE);
    memset(write_buffer, 0, BUFFER_SIZE);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    res = inet_pton(AF_INET, server_ip, &server_address.sin_addr);
    if (res != 1) {
        printf("error while parsing the server_ip\n");
        fflush(stdout);
        return -1;
    }

    res = connect(sock_fd, (SockAddr*)&server_address, server_addr_len);

    if (res < 0) {
        printf("error while connecting to the server\n");
        fflush(stdout);
        return -1;
    }

    int rand_num = rand_range(1, 6);

    snprintf(
        write_buffer,
        BUFFER_SIZE,
        "../text_files/%d.txt",
        rand_num);

    n_bytes = write(sock_fd, write_buffer, BUFFER_SIZE);
    printf("write_buffer: %s", write_buffer);
    fflush(stdout);

    memset(write_buffer, 0, BUFFER_SIZE);

    while((n_bytes = read(sock_fd, read_buffer, BUFFER_SIZE)) > 0) {
        fprintf(stdout, "READ (%d.txt):\n%s", rand_num, read_buffer);
        fflush(stdout);
        memset(read_buffer, 0, BUFFER_SIZE);
    }

    close(sock_fd);
    return 0;
}
