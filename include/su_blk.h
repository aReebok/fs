#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bfs_drv.h"
#include "talloc.h"
#include "buffer.h"


#ifndef _SUPER_BLOCK
#define _SUPER_BLOCK

#define LF_COUNT 10

typedef struct sublk SuBlk;

/*
 * Struct definition for superblock...
 * May need to remove the cdllist in favor of a c_list instead
 * 
*/
typedef struct sublk sublk;
struct sublk {
    int file_system_size;
    
    int num_of_free_blocks;
    uint32_t free_block_list[164];

    int inode_list_size;
    int num_of_free_inodes;
    uint32_t free_inode_list[64];
    int index_of_next_free_inode; // why is this needed? Maybe inode list shouldn't be a cdllist...

    int lock_fields[LF_COUNT]; // what is this again
    char super_block_modified;

};

sublk* create_sublk();

int write_sublk(SuBlk * sublk, bfs * dev);

SuBlk * read_sublk(bfs * device);

#endif
// sblk = 512