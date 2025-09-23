#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"
#include "util.h"

typedef struct Node Node;

Node* ACTIV_LIST = NULL;

struct Node {
    void* item; 
    Node* next;
};

Node* insert_front(Node* head, void* item) {
    Node* new_node = malloc(sizeof(Node));
    new_node->item = item;
    new_node->next = head;
    return new_node;
}

void *talloc(size_t size) {
    if (size <= 0) return NULL;
    void* alloc = malloc(size);
    if (alloc == NULL)
        perr("talloc: malloc failed.");
    else
        ACTIV_LIST = insert_front(ACTIV_LIST, alloc);
    return alloc;
}

void tfree() {
    Node* curr;
    while (ACTIV_LIST) {
        curr = ACTIV_LIST;
        ACTIV_LIST = ACTIV_LIST->next;
        free(curr->item);
        free(curr);
        // printf("Freeing node...\n");
    }
}

void texit(int status) {
    tfree();
    exit(status);
}
