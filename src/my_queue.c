#include "../include/my_queue.h"

void enqueue(Queue* q, int* val) {
    Node* new_node = malloc(sizeof(Node)) ;
    if (new_node == NULL) { return; }
    new_node->value =  val;
    new_node->next = NULL;

    if (q->tail == NULL) {
        q->head = new_node;
    } else {
        q->tail->next = new_node;
    }

    q->tail = new_node;
}


int* dequeue(Queue* q) {
    if (!q || !q->head) { return NULL; }
    Node* headNode = q->head;
    int* res = headNode->value;

    q->head = headNode->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }

    headNode->next = NULL;
    free(headNode);
    return res;
}
