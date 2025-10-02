// #include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "bfs_drv.h"
#include "util.h"
#include "talloc.h"

#define ERROR() \
    perr("Talloc failed: BFS_DEVICE could not be created");\
    return NULL;

bfs * floppy;

bfs * initialize_bfs(const char * bfs_path) {
    bfs * dev = talloc(sizeof(*dev));

    if(dev == NULL) {
        ERROR(); // TODO: Test in a safe environment whether the semi-colon or braces are necessary
    }

    int f = creat(bfs_path, 0644); // #TODO: ADD checks to maybe not recreate the file everytime?

    // ftruncate takes the created file and shortens/lengthens it to size of BFS... 0 padded
    if (ftruncate(f, BFS_SIZE) == -1) {
        ERROR();
    }

    dev->bfs = f;
    
    dev->super_block_start = PRE_BLOCK_SIZE; // leave a set number of blocks empty for future use
    dev->inode_table_start = dev->super_block_start + SUPER_BLOCK_SIZE;
    dev->data_block_start = dev->inode_table_start  + INODE_BLOCK_SIZE;

    printf("======Successfully created BFS of size: %d bytes=======\n", BFS_SIZE);
    return dev;
}

size_t block_write(Buffer* w_buf, int blk_no, bfs* device) {
    // TODO seek to the block number in the bfs file file 
    // TODO make sure alignment is correct and fwrite
    // return BLOCK

    // if blk_no = -1, then writing super block...
    if(blk_no == SUBLOCK_NUM) {
        lseek(device->bfs, PRE_BLOCK_SIZE*BLOCK_SIZE, SEEK_SET);
        return write(device->bfs, w_buf, BLOCK_SIZE);
    }
    else {
        //TODO: Find offset and write the block... it might be an indoe so 
        // read the block first if necessary
    }
}
