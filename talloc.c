#include <stdlib.h>
#include <stdio.h>
#include "talloc.h"

typedef struct Node {
    void* item; 
    struct Node* next;
} Node;

Node* ACTIV_LIST = NULL;

void display(Node *head){
    printf("(");
    while(head != NULL){
        printf(" %p,", head->item);
        head = head->next;
    }
    puts(")");
}

Node* insert_front(Node* head, void* item) {
    Node* new_node = malloc(sizeof(Node));
    new_node->item = item;
    new_node->next = head;
    return new_node;
}

void *talloc(size_t size) {
    void* alloc = malloc(size);
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

// int main(){
//     int* p = talloc(sizeof(int)*2);
//     p[0] = 100;
//     p[1] = 200;
//     int* p2 = talloc(sizeof(int)*2);
//     p2[0] = 150;
//     p2[1] = 250;

//     for (int i = 0; i < 2; i++) {
//         printf("%d %d %d\n", i, p[i], p2[i]);
//     }

//     display(ACTIV_LIST);
//     texit(0);
// }