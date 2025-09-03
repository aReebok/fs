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

free_list *hashq = NULL;
free_list BUF_FREE_LIST = {&BUF_FREE_LIST, &BUF_FREE_LIST};
// TO DO create buffer_hash_queue -> then we have a full buffer cache

void print_list_links(free_list* list) { // for debugging
    free_list* tmp = list;
        if (is_empty(tmp)) {
            printf("Printing empty list\n");
        } else {
            printf("Printing non-empty list\n");
        };

    do {
        printf("\t{%p: next[%p] prev[%p]}\n", tmp, tmp->next, tmp->prev);
        tmp = tmp->next;
    } while(tmp != list);
}

void print_buffer_info_free_list(free_list* list) { // for debugging
    if (is_empty(list)) {
        printf("Empty list\n");
        return;
    } else {
        printf("Printing buffer info from free list...\n");
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
    int buf_h = hash_buffer(buf);
    // ERROR CHECKING and return 1
    insert_head(&buf->fl_hook, &BUF_FREE_LIST); //TODO:insert_head: Should return 1 or 0
    insert_head(&buf->fl_hook, &hashq[buf_h]);  //TODO:insert_tail: Should return 1 or 0
    return 0;
}

int main() {
    // Hash setup - HASH_SIZE in buffer.h
    hashq = malloc(sizeof(*hashq) * HASH_SIZE);
    for(int i = 0; i < HASH_SIZE; i++) {
        free_list temp_buf = {&temp_buf, &temp_buf};
        hashq[i] = temp_buf;
    }

    print_list_links(&BUF_FREE_LIST);
    Buffer* new_buffer = create_buf(1, 0, 0);
    insert_buffer_cache(new_buffer);

    Buffer* new_buffer2 = create_buf(1, 256, 0);
    insert_buffer_cache(new_buffer2);
    print_list_links(&BUF_FREE_LIST);                   // print out links
    print_buffer_info_free_list(&BUF_FREE_LIST);        // print out actual buffer information

    return 0;
}
