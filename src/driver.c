#include "driver.h"

int block_read(char* r_data, int blk_no, bfs* device) {
    lseek(device->bfs, (PRE_BLOCK_COUNT + blk_no) * BLOCK_SIZE, SEEK_SET);
    return read(device->bfs, r_data, BLOCK_SIZE);
}

int block_write(char* w_data, int blk_no, bfs* device) {
    lseek(device->bfs, (PRE_BLOCK_COUNT + blk_no) * BLOCK_SIZE, SEEK_SET);
    return write(device->bfs, w_data, BLOCK_SIZE);
}
