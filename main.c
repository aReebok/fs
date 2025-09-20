#include <stdio.h>
#include <stdlib.h>
#include "cdllist.h"
#include "talloc.h"
#include "buffer.h"
#include "bufcache.h"
#include "diskdrv.h"

void print_list_links(cdllist* list) { // for debugging
    cdllist* tmp = list;
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

void print_buffer_info_free_list(cdllist* list) { // for debugging
    if (is_empty(list)) {
        printf("Empty list\n");
        return;
    } else {
        printf("> Printing actual buffer content from free list...\n");
    };

    cdllist* curr = list->next;
    Buffer* temp;
    do {
        temp = container_of(curr, Buffer, fl_hook);
        print_buffer(temp);
        curr = curr->next;
    } while(curr != list);
}

int main() {
    struct BCache * buffer_cache = initialize_cache();
    #define VFS "test.vfs"
    ssd = initialize_fs(VFS);

    // initializes with a few buffers in free list
    for (int i = 0; i < 7; i++) {
        Buffer* temp_buf = create_buf(1, i, 0); // status = 0; meaning invalid data
        bcache_insert(temp_buf, buffer_cache);
    }

    // Tests bread and getblk
    Buffer* third_line_buf = bread(3, buffer_cache); 
    // Buffer* third_line_buf_again = bread(3, buffer_cache); // buffer found in HQ, but buffer is busy [sleeping]
    brelse(third_line_buf, buffer_cache); 
    print_buffer_info_free_list(buffer_cache->BUF_FREE_LIST); // releases to tail end of free list
    
    Buffer* third_line_buf_again_again = bread(3, buffer_cache);
    print_buffer_info_free_list(buffer_cache->BUF_FREE_LIST); 
    
    // Test bwrite
    third_line_buf_again_again->data[10] = '$';
    bwrite(third_line_buf_again_again, buffer_cache);

    puts("=====Exiting Main: Safe exiting. Deleting RAM=====");
    texit(0);
}
