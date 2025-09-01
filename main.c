#include <stdio.h>
#include "talloc.h"

#define BLOCK_SIZE  1024        // File System block size 
#define BUF_FL_SZ   10
#define B_LOCKED    0x01        // Buffer Status Flags
#define B_VALID     0x02
#define B_DELWRI    0x04
#define B_IOBUSY    0x08
#define B_WANTED    0x16

typedef struct Buffer { 
    unsigned int device_no;
    unsigned int block_no;
    unsigned int status;
    char data[BLOCK_SIZE]; // for now just chars
    struct Buffer *hq_prev; // Hash Queue
    struct Buffer *hq_next;
    struct Buffer *fl_prev; // Free List
    struct Buffer *fl_next;
} Buffer;

typedef struct Block {
    unsigned int device_no;
    unsigned int block_no;
    unsigned int status;

} Block;

typedef struct Free_List {
    Buffer* head;
    Buffer* tail;
} Free_List;

Free_List* BUF_FREE_LST = NULL;

void print_buffer_pool() {
    printf("[\n");
    const Buffer* temp = BUF_FREE_LST->head;
    do {
        printf("    {Location dev %d blk %d \t| Status: %d},\n", \
                temp->device_no, temp->block_no, temp->status);
        temp = temp->fl_next;
    } while (temp != BUF_FREE_LST->head);
    printf("]\n");
}

Buffer* insert_front_buf_free_list(Buffer* const buf) {
    if (BUF_FREE_LST->head) {
        BUF_FREE_LST->head->fl_prev = buf;
        buf->fl_next = BUF_FREE_LST->head;
        buf->fl_prev = BUF_FREE_LST->tail;
        BUF_FREE_LST->tail->fl_next = buf;
        return buf;
    } 
    
    buf->fl_next = buf->fl_prev = buf;
    BUF_FREE_LST->tail = buf;
    return buf;
}


void buffer_alloc(const unsigned int devno, const unsigned int blkno) {
    Buffer* new_buffer = talloc(sizeof(Buffer));
    new_buffer->device_no = devno;
    new_buffer->block_no = blkno;
    new_buffer->status = 0;

    // add to head of free list..
    BUF_FREE_LST->head = insert_front_buf_free_list(new_buffer);

    // add to hash queue

}


void init_buffer_pool() {
    BUF_FREE_LST = talloc(sizeof(Free_List));
    BUF_FREE_LST->head = NULL;
    BUF_FREE_LST->tail = NULL;

    // init free list 
    for (int i = 0; i < BUF_FL_SZ; i++)
        buffer_alloc((i / (BUF_FL_SZ/2))+1, (i % (BUF_FL_SZ/2)) * 256);

    // init hash queue
}

int main() {
    init_buffer_pool();
    print_buffer_pool();
    texit(0);
}