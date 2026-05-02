#include "../include/common.h"
#include <pthread.h>

void* handle_request(void* args);

int main() {
    int client_counts = 0;
    struct sockaddr_in sock_addr;
    socklen_t sock_addr_len = sizeof(sock_addr);
    int sock_channel, conn_channel, n_bytes;
    char read_buffer[BUFFER_SIZE], write_buffer[BUFFER_SIZE];
    int res;

    sock_channel = socket(AF_INET, SOCK_STREAM, 0);

    memset(&sock_addr , 0, sock_addr_len);
    memset(read_buffer , 0, BUFFER_SIZE);
    memset(write_buffer, 0, BUFFER_SIZE);

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(SERVER_PORT);      // CONVERT FROM LITTLE ENDIAN TO BIG ENDIAN
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); // CONVERT FROM LITTLE ENDIAN TO BIG ENDIAN

    res = bind(sock_channel, (SockAddr*) &sock_addr, sock_addr_len);
    res = listen(sock_channel,  100);
    printf("SERVER IS LISTNEING NOW WAITING FOR A CONNECTION\n");
    fflush(stdout);

    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        char client_ip[BUFFER_SIZE];

        conn_channel = accept(sock_channel, (SockAddr*)&client_addr, &client_addr_len);
        inet_ntop(AF_INET, (void*)&client_addr.sin_addr, client_ip, client_addr_len);
        client_counts++;
        printf("CONNECTED SUCCESSFULLY TO THE CLIENT WITH IP = ( %s )\n", client_ip);
        printf("COUNT OF CLIENTS: %d ", client_counts);
        fflush(stdout);

        n_bytes = read(conn_channel, read_buffer, BUFFER_SIZE);
        handle_request_args args = {
            .conn_channel = conn_channel,
            .read_buffer = read_buffer,
            .write_buffer = write_buffer,
        };

        pthread_t new_thread;
        pthread_create(&new_thread, NULL, handle_request, &args);
        // handle_request(&args);
        close(conn_channel);
    }

    return 0;
}

void* handle_request(void* args) {
    handle_request_args args_cp = *(handle_request_args*)args;
    printf("File Path: %s\n", args_cp.read_buffer);
    fflush(stdout);

    FILE* fptr = fopen(args_cp.read_buffer, "r");
    if (fptr == NULL) {
        printf("ERROR INVALID FILE PATH OR SOMETHING WENT WRONG\n");
        fflush(stdout);
        return NULL;
    }

    size_t n_bytes;
    while ((n_bytes = fread(args_cp.write_buffer, 1, BUFFER_SIZE, fptr)) > 0) {
        n_bytes = write(args_cp.conn_channel, args_cp.write_buffer, BUFFER_SIZE);
        memset(args_cp.write_buffer, 0, BUFFER_SIZE);
    }

    memset(args_cp.read_buffer, 0, BUFFER_SIZE);
    pthread_exit(NULL);
    return NULL;
}
