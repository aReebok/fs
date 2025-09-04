#include <stdio.h>
#include <stdlib.h>
#include "free_list.h"
#include "talloc.h"
#include "buffer.h"

// Define container_of macro if not already defined
// #ifndef container_of
// #define container_of(ptr, type, member)
//      ((type *)((char *)(ptr) - (size_t)&((type *)0)->member))
// #endif

free_list *BUF_HASH_QUEUE = NULL;
free_list BUF_FREE_LIST = {&BUF_FREE_LIST, &BUF_FREE_LIST};
// TO DO create buffer_hash_queue -> then we have a full buffer cache

void print_list_links(free_list* list) { // for debugging
    free_list* tmp = list;
        if (is_empty(tmp)) {
            printf("Printing empty list\n");
        } else {
            printf("Printing non-empty list\n");
        };
    printf("\tHEAD\n");
    do {
        printf("\t{%p: next[%p] prev[%p]}\n", tmp, tmp->next, tmp->prev);
        tmp = tmp->next;
    } while(tmp != list);
    printf("\tTAIL\n\n");
}

void print_hash_queue() {
    printf("Printing each HashQueue [HEAD NODE only]\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("Bucket %d: head=%p next=%p prev=%p size=%d\n",
               i, &BUF_HASH_QUEUE[i],
               BUF_HASH_QUEUE[i].next,
               BUF_HASH_QUEUE[i].prev,
            size(&BUF_HASH_QUEUE[i]));
    }
    printf("\n");
}

void print_buffer_info_free_list(free_list* list) { // for debugging
    if (is_empty(list)) {
        printf("Empty list\n");
        return;
    } else {
        printf("> Printing actual buffer content from free list...\n");
    };

    free_list* curr = list->next;
    Buffer* temp;
    do {
        temp = container_of(curr, Buffer, fl_hook);
        printf("    {Location dev %d blk %d \t| Status: %d},\n", \
                temp->device_no, temp->block_no, temp->status);
        curr = curr->next;
    } while(curr != list);
}

int insert_buffer_cache(Buffer * const buf) {
    printf("> Adding a buffer to buffer pool: ...\n");
    int buf_h = hash_buffer(buf);
    // ERROR CHECKING and return 1
    insert_head(&buf->fl_hook, &BUF_FREE_LIST); //TODO:insert_head: Should return 1 or 0
    insert_head(&buf->hq_hook, &BUF_HASH_QUEUE[buf_h]);  //TODO:insert_tail: Should return 1 or 0
    return 0;
}

int main() {
    // Hash setup - HASH_SIZE in buffer.h
    BUF_HASH_QUEUE = talloc(sizeof(free_list) * HASH_SIZE);
    for(int i = 0; i < HASH_SIZE; i++) {        
        BUF_HASH_QUEUE[i].next = &BUF_HASH_QUEUE[i];
        BUF_HASH_QUEUE[i].prev = &BUF_HASH_QUEUE[i];
    }

    print_list_links(&BUF_FREE_LIST);
    print_hash_queue();

    Buffer* new_buffer1 = create_buf(1, 0, 0);
    Buffer* new_buffer2 = create_buf(1, 257, 0);
    Buffer* new_buffer3 = create_buf(1, 261, 0);
    insert_buffer_cache(new_buffer1);
    insert_buffer_cache(new_buffer2);
    insert_buffer_cache(new_buffer3);

    print_list_links(&BUF_FREE_LIST);                   // print out links
    print_hash_queue();
    print_buffer_info_free_list(&BUF_FREE_LIST);        // print out actual buffer information

    return 0;
}
