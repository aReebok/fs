#include "driver.h"

int block_read(char* r_data, int blk_no, bfs* device) {
    if (blk_no == SUBLOCK_NUM) {
        lseek(device->bfs, PRE_BLOCK_SIZE*BLOCK_SIZE, SEEK_SET);
        return read(device->bfs, r_data, BLOCK_SIZE);
    }
    return 0;
}

int block_write(char* w_data, int blk_no, bfs* device) {
    // TODO seek to the block number in the bfs file file 
    // TODO make sure alignment is correct and fwrite
    // return BLOCK

    // if blk_no = -1, then writing super block...
    if(blk_no == SUBLOCK_NUM) {
        lseek(device->bfs, PRE_BLOCK_SIZE*BLOCK_SIZE, SEEK_SET);
        return write(device->bfs, w_data, BLOCK_SIZE);
    }
    else if (blk_no >= 0) {
        //TODO: Find offset and write the block... 
        //TODO: read the block first if necessary
        lseek(device->bfs, (PRE_BLOCK_SIZE + SUPER_BLOCK_SIZE + \
                        INODE_BLOCK_SIZE + blk_no) * BLOCK_SIZE, SEEK_SET);
        return write(device->bfs, w_data, BLOCK_SIZE);
    }
    return 0;
}
