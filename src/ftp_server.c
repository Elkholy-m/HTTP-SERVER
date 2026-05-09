#include "../include/common.h"
#include <pthread.h>
#include <stdio.h>

pthread_t thread_bool[THREAD_BOOL_MAX];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
Queue q = {0};

void* handle_request(void* args);
void* thread_func(void* args);

int main() {
    int client_counts = 0;
    struct sockaddr_in sock_addr;
    socklen_t sock_addr_len = sizeof(sock_addr);
    int sock_channel, conn_fd, n_bytes;
    int res;

    for (int i = 0; i < THREAD_BOOL_MAX; i++) {
        pthread_create(&thread_bool[i], NULL, thread_func, NULL);
    }

    sock_channel = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_channel == -1) {
        format_error("ERROR WHILE CREATING THE SOCKET");
    }

    memset(&sock_addr , 0, sock_addr_len);

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(SERVER_PORT);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    res = bind(sock_channel, (SockAddr*) &sock_addr, sock_addr_len);
    if (res < 0) {
        format_error("ERROR WHILE BINDING");
    }

    res = listen(sock_channel,  SERVER_BACKLOG);

    if (res < 0) {
        format_error("ERROR WHILE LISTENING");
    }

    printf("SERVER IS LISTNEING NOW WAITING FOR A CONNECTION\n");
    fflush(stdout);

    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        char client_ip[BUFFER_SIZE];

        conn_fd = accept(sock_channel, (SockAddr*)&client_addr, &client_addr_len);
        inet_ntop(AF_INET, (void*)&client_addr.sin_addr, client_ip, client_addr_len);
        client_counts++;
        printf("CONNECTED SUCCESSFULLY TO THE CLIENT WITH IP = ( %s )\n", client_ip);
        printf("COUNT OF CLIENTS: %d ", client_counts);
        fflush(stdout);

        // pthread_t thread;
        int* conn_fd_cp = malloc(sizeof(int));
        *conn_fd_cp = conn_fd;
        // pthread_create(&thread, NULL, handle_request, (void*)conn_fd_cp);


        // handle_request((void*) &conn_fd);
        
        pthread_mutex_lock(&mutex);
        enqueue(&q, conn_fd_cp);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void* handle_request(void* args) {
    int conn_fd = *(int*)args;
    size_t written = 0;

    char buffer[BUFFER_SIZE] = {0};
    size_t n_bytes = read(conn_fd, buffer, BUFFER_SIZE - 1);

    char path[BUFFER_SIZE] = {0};
    realpath(buffer, path);
    printf("FILE PATH: %s\n", path);
    fflush(stdout);


    FILE* fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("ERROR INVALID FILE PATH %s, OR SOMETHING WENT WRONG\n", path);
        fflush(stdout);
        return NULL;
    }


    while ((n_bytes = fread(buffer, 1, BUFFER_SIZE, fptr)) > 0) {
        written = write(conn_fd, buffer, n_bytes);
    }

    close(conn_fd);
    fclose(fptr);
    return NULL;
}

void* thread_func(void* args) {
    while (true) {
        pthread_mutex_lock(&mutex);
        int *conn_fd;
        if ((conn_fd = dequeue(&q)) == NULL) {
            pthread_cond_wait(&cond, &mutex);
            conn_fd = dequeue(&q);
        }

        pthread_mutex_unlock(&mutex);

        if (conn_fd != NULL) {
            handle_request((void*)conn_fd);
            free(conn_fd);
            conn_fd = NULL;
        }
    }
}
