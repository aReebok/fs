#include <stdio.h>
#include "free_list.h"
#include "talloc.h"
#include "buffer.h"

// Define container_of macro if not already defined
// #ifndef container_of
// #define container_of(ptr, type, member) \
//     ((type *)((char *)(ptr) - (size_t)&((type *)0)->member))
// #endif

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

int main() {
    print_list_links(&BUF_FREE_LIST);
    Buffer* new_buffer = create_buf(1, 0, 0);
    insert_head(&new_buffer->fl_hook, &BUF_FREE_LIST);
    Buffer* new_buffer2 = create_buf(1, 256, 0);
    insert_head(&new_buffer2->fl_hook, &BUF_FREE_LIST);
    print_list_links(&BUF_FREE_LIST);                   // print out links
    print_buffer_info_free_list(&BUF_FREE_LIST);        // print out actual buffer information
    return 0;
}
