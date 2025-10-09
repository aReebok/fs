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

int setup_super_block();

int setup_free_block_list(bfs * dev) {
    // temp pointer for super-block free block list, will fill at the end
    uint32_t * free_list_pointer = dev -> incore_sblk -> free_block_list;
    dev->incore_sblk->num_of_free_blocks = 1;
    free_list_pointer[0] = 0; // points to block 0 in the block section, which \
                                    is the first node of free-blk linked list

    int i = 0; //   block number 0 will contain address for blocks from 1-256\
                    (256 numbers, 4 bytes each)... then block 256 will contain\
                    the continuation (address from block 257-512)... then 512...

    uint32_t blk[FREE_ARRAY_BLOCK_SIZE];

    while(i < DATA_BLOCK_SIZE) {
        if (DATA_BLOCK_SIZE - i < FREE_BLOCK_LIST_SIZE) {
            for(int j = 0; j < (DATA_BLOCK_SIZE - i); j++) {
                free_list_pointer[j+1] = i + j;
                dev->incore_sblk->num_of_free_blocks += 1;
            }
            break;
        }
        for(int j = 0; j < FREE_ARRAY_BLOCK_SIZE; j++) {
            blk[j] = i+j+1;
        }

        //  Flips the first and last element... this way the 0th index contains\
            the pointer to the next block in the free block linked list...
        int temp = blk[FREE_ARRAY_BLOCK_SIZE - 1 ];
        blk[FREE_ARRAY_BLOCK_SIZE - 1] = blk[0];
        blk[0] = temp;

        // WRITE BLOCK to the harddisk... 
        block_write((void *) blk, i, dev);

        // i += 256, skipping to the next node node in the linked list...
        i += FREE_ARRAY_BLOCK_SIZE; 
    }

    for(int i = 0; i < FREE_BLOCK_LIST_SIZE; i++) {

    }
}