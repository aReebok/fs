#include "bfs.h"
#include "driver.h"

#define ERROR() \
    perr("Talloc failed: BFS_DEVICE could not be created");\
    return NULL;

bfs * floppy;

bfs * mkbfs(const char * bfs_path) {
    bfs * dev = talloc(sizeof(*dev));

    if(dev == NULL) {
        ERROR(); // TODO: Test in a safe environment whether the semi-colon or braces are necessary
    }

    int f = open(bfs_path, O_RDWR | O_CREAT | O_TRUNC, 0644); // #TODO: ADD checks to maybe not recreate the file everytime?

    // ftruncate takes the created file and shortens/lengthens it to size of BFS... 0 padded
    if (ftruncate(f, BFS_SIZE) == -1) {
        ERROR();
        printf("teste");
    }

    dev->bfs = f;

    // TODO: Setup super block
    dev->incore_sblk = create_sublk();

    // TODO: Setup inode lists and stuff

    // TODO: Setup free block list
    setup_free_block_list(dev);
    
    // printf("======Successfully created BFS of size: %d bytes=======\n", BFS_SIZE);
    return dev;
}

int setup_free_block_list(bfs * dev) {
    uint32_t * sblk_free_list_ptr = dev->incore_sblk->free_block_list;
    uint32_t linked_list_address_block[LINKED_LIST_DATA_BLK_SIZE];

    int free_data_blk_address = 1;

    for(int i = 0; i < SU_FREE_BLOCK_LIST_SIZE; i++) {
        sblk_free_list_ptr[i] = free_data_blk_address;
        free_data_blk_address++;
    }

    int next_linked_list_address = sblk_free_list_ptr[0];

    uint32_t ll_address_blk[LINKED_LIST_DATA_BLK_SIZE];

    while(free_data_blk_address < DATA_BLOCK_SIZE) {
        
        for(int ll_address_blk_index = 0; ll_address_blk_index < LINKED_LIST_DATA_BLK_SIZE; ll_address_blk_index++) {
            if (free_data_blk_address >= DATA_BLOCK_SIZE) {
                ll_address_blk[ll_address_blk_index] = 0;
            }
            else {
                ll_address_blk[ll_address_blk_index] = free_data_blk_address;
                free_data_blk_address++;
            }
        }
        block_write((void *) ll_address_blk, next_linked_list_address, dev);
        next_linked_list_address = ll_address_blk[0];
    }
}