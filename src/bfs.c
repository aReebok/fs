#include "bfs.h"
#include "driver.h"
#include "inode.h"

#define ERROR() \
    perr("Talloc failed: BFS_DEVICE could not be created");\
    return NULL;

extern bfs* floppy;

bfs* mkbfs(const char* bfs_path) {
    bfs* dev = talloc(sizeof(*dev));
    if(dev == NULL) { ERROR(); } // TODO: Test in a safe environment whether the semi-colon or braces are necessary

    int f = open(bfs_path, O_RDWR | O_CREAT | O_TRUNC, 0644); // #TODO: ADD checks to maybe not recreate the file everytime?

    // ftruncate takes the created file and shortens/lengthens it to size of BFS... 0 padded
    if (ftruncate(f, BFS_SIZE) == -1) {
        ERROR();
    }

    dev->bfs = f;

    // TODO: Setup super block
    dev->incore_sublk = create_empty_sublk();
    dev->incore_sublk->file_system_size = BFS_SIZE;
    dev->incore_sublk->super_block_modified = 1;
    // TODO: Setup inode lists and stuff
    setup_free_inode_list(dev);

    // TODO: Setup free block list
    setup_free_block_list(dev);
    
    // TODO: Write out incore super block to disk
    block_write(sublk_to_str(dev->incore_sublk), SUBLK_INDEX, dev);

    printf("======Successfully created BFS of size: %0.f bytes=======\n", BFS_SIZE);
    return dev;
}

int setup_free_inode_list(bfs* dev) {
    uint32_t* sblk_free_ino_list = dev->incore_sublk->free_inode_list;
    dev->incore_sublk->inode_list_size = INODE_BLOCK_COUNT * INODES_PER_BLOCK; // check if this is the right interp of this variable
    dev->incore_sublk->index_of_next_free_inode = SU_FREE_INODE_LIST_SIZE - 1;
    int i = SU_FREE_INODE_LIST_SIZE - 1;
    int inode_no = 1;
    while (i >= 0 && inode_no <= INODE_BLOCK_COUNT * INODES_PER_BLOCK) {
        sblk_free_ino_list[i--] = inode_no++;
        dev->incore_sublk->num_of_free_inodes++;
    }

    return 0;
}

int setup_free_block_list(bfs* dev) {
    uint32_t* sblk_free_blk_list = dev->incore_sublk->free_block_list;
    
    int curr_blk_no = 1;
    for (int i = 0; i < SU_FREE_BLOCK_LIST_SIZE; i++)
        sblk_free_blk_list[i] = curr_blk_no++; 
    
    dev->incore_sublk->num_of_free_blocks += SU_FREE_BLOCK_LIST_SIZE;

    uint32_t free_addrs[FREE_ADDRS_CAPACITY];
    int next_array_address = sblk_free_blk_list[0];
    while(curr_blk_no < DATA_BLOCK_COUNT) {
        
        for(int i = 0; i < FREE_ADDRS_CAPACITY; i++) {
            if (curr_blk_no >= DATA_BLOCK_COUNT) {
                free_addrs[i] = 0;
            } else {
                free_addrs[i] = curr_blk_no;
                dev->incore_sublk->num_of_free_blocks++;
                curr_blk_no++;
            }
        }
        block_write((void *) free_addrs, next_array_address, dev);
        next_array_address = free_addrs[0];
    }

    return 0;
}