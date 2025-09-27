#include <stdio.h>
#include <stdlib.h>
#include "cdllist.h"

#ifndef _SUPER_BLOCK
#define _SUPER_BLOCK

#define LF_COUNT 10

typedef struct sublk SuBlk;

/*
 * Struct definition for superblock...
 * May need to remove the cdllist in favor of a c_list instead
 * 
*/

struct sublk {
    int file_system_size;
    
    int num_of_free_blocks;
    cdllist * free_block_list;

    int inode_list_size;
    int num_of_free_inodes;
    cdllist * free_inode_list;
    int index_of_next_free_inode; // why is this needed? Maybe inode list shouldn't be a cdllist...

    int lock_fields[LF_COUNT]; // what is this again
    char super_block_modified;

};

char * serialize_sublk(SuBlk * sblk);

int write_sublk(SuBlk * sblk);


#endif
// sblk = 512