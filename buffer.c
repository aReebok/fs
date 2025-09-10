#include <stdio.h>
#include "buffer.h"
#include "talloc.h"

// member functions just for buffer struct
int hash_buffer(const Buffer* const buf) {
    return buf->block_no % HASH_SIZE;
}

int hash_block_num(int const block_no) {
    return block_no % HASH_SIZE;
}

void set_buf_status(Buffer* const buf, const unsigned int status) {
    buf->status = status;
}

Buffer* create_buf(const unsigned int devno, \
                    const unsigned int blockno, \
                    const unsigned int status) {
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