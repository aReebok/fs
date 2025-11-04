#include <stdio.h>
#include "buffer.h"
#include "talloc.h"
#include "util.h"
#include <string.h> 

// TODO make these hash funcs into #defines
int hash_buffer(const Buffer* const buf) {
    return buf->block_no % HASH_SIZE;
}

int hash_block_num(int const block_no) {
    return block_no % HASH_SIZE;
}

void set_buf_status(Buffer* const buf, const unsigned int status) {
    buf->status = status;
}

Buffer* create_empty_buf() {
    Buffer* empty_buf = talloc(sizeof(Buffer));
    memset(empty_buf, 0, sizeof(Buffer));
    return empty_buf; 
}

Buffer* create_buf(const int devno, const int blockno, const int status) {
    if (devno < 0 || blockno < 0 || status < 0) {
        plog("Warning [creat_buf] Invalid arguments: negative values not allowed\n");
        return NULL;
    }
    Buffer* new_buf = talloc(sizeof(Buffer));
    new_buf->device_no = devno;
    new_buf->block_no = blockno;
    new_buf->status = status;
    return new_buf;
}

void print_buffer(Buffer * const buf) {
    printf("    {Location dev %d blk %d \t| Status: %d},\n", \
        buf->device_no, buf->block_no, buf->status);
    printf("\tData: %s\n", buf->data);
    printf("------------------------------------------------------\n");
}

void print_buffer_info_free_list(cdllist* list) {
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