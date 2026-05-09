#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t Node;
typedef struct node_t {
    Node* next;
    int* value;
} Node;

typedef struct {
    Node* head;
    Node* tail;
} Queue;

void init_queue(Queue* q);
void enqueue(Queue* q, int* val);
int* dequeue(Queue* q);
