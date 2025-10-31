#include "driver.h"

int block_read(char* r_data, int blk_no, int byte_offset, bfs* device) {
    lseek(device->bfs, ((PRE_BLOCK_COUNT + blk_no) * BLOCK_SIZE) + byte_offset, SEEK_SET);
    return read(device->bfs, r_data, BLOCK_SIZE);
}

int block_write(char* w_data, int blk_no, int byte_offset, bfs* device) {
    lseek(device->bfs, ((PRE_BLOCK_COUNT + blk_no) * BLOCK_SIZE) + byte_offset, SEEK_SET);
    return write(device->bfs, w_data, BLOCK_SIZE);
}
