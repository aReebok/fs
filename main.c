#include <stdio.h>
#include <stdlib.h>
#include "free_list.h"
#include "talloc.h"
#include "buffer.h"
#include "bufcache.h"

// Define container_of macro if not already defined
// #ifndef container_of
// #define container_of(ptr, type, member)
//      ((type *)((char *)(ptr) - (size_t)&((type *)0)->member))
// #endif

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

int main() {
    struct BCache buffer_cache = initialize_cache();

    print_list_links(buffer_cache.BUF_FREE_LIST);
    print_hash_queue(buffer_cache);

    int block_nums[3] = {0, 257, 261};
    for(int i = 0; i < 3; i++) {
        Buffer* new_buffer1 = create_buf(1, block_nums[i], 0);
        BCache_Insert(new_buffer1, buffer_cache);
    }

    print_list_links(buffer_cache.BUF_FREE_LIST);                   // print out links
    print_hash_queue(buffer_cache);
    print_buffer_info_free_list(buffer_cache.BUF_FREE_LIST);        // print out actual buffer information

    return 0;
}
