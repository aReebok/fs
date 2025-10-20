#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "talloc.h"
#include "buffer.h"


#ifndef _SUPER_BLOCK
#define _SUPER_BLOCK

#define LOCKED_SU_BLOCK_LIST    0x01    
#define LOCKED_SU_INODE_LIST    0x02
#define LF_COUNT 10
#define SU_FREE_BLOCK_LIST_SIZE 128
#define SU_FREE_INODE_LIST_SIZE 64 
#define FREE_ARRAY_BLOCK_SIZE (BLOCK_SIZE/sizeof(uint32_t))

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
    uint32_t free_block_list[SU_FREE_BLOCK_LIST_SIZE];
    int next_free_index;

    int inode_list_size;
    int num_of_free_inodes;
    uint32_t free_inode_list[SU_FREE_INODE_LIST_SIZE];
    int index_of_next_free_inode;

    int lock_fields;
    char super_block_modified;

};

sublk* create_empty_sublk();
sublk* create_sublk();

char * sublk_to_str(sublk * sublk);

sublk * read_sublk(char store[]);
void print_sublk(const sublk* s);

#endif