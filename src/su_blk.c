#include "su_blk.h"
#include "bfs_drv.h"
#include "util.h"

sublk* create_sublk() {
    sublk* new_sublk = (sublk*) talloc(sizeof(sublk));
    new_sublk->file_system_size = 1;
    new_sublk->num_of_free_blocks = 10000;
    new_sublk->free_block_list[0] = 99;
    new_sublk->free_block_list[1] = 98;
    new_sublk->free_block_list[2] = 97;
    new_sublk->free_block_list[163] = 96;
    new_sublk->inode_list_size = 64;
    new_sublk->num_of_free_inodes = 64;
    for (int i = 0; i < 64; i++) {
        new_sublk->free_inode_list[i] = i;
    }
    new_sublk->index_of_next_free_inode = 0;
    for (int i = 0; i < LF_COUNT; i++) {
        new_sublk->lock_fields[i] = 0;
    }
    new_sublk->super_block_modified = 55;
    return new_sublk;
}

int write_sublk(SuBlk * sublk, bfs * dev) {
    char* block = talloc(BLOCK_SIZE);
    int offset = 0;
    memcpy(block + offset, &(sublk->file_system_size), sizeof(sublk->file_system_size));
    offset += sizeof(sublk->file_system_size);

    memcpy(block + offset, &(sublk->num_of_free_blocks), sizeof(sublk->num_of_free_blocks));
    offset += sizeof(sublk->num_of_free_blocks);

    memcpy(block + offset, sublk->free_block_list, sizeof(sublk->free_block_list));
    offset += sizeof(sublk->free_block_list);

    memcpy(block + offset, &(sublk->inode_list_size), sizeof(sublk->inode_list_size));
    offset += sizeof(sublk->inode_list_size);

    memcpy(block + offset, &(sublk->num_of_free_inodes), sizeof(sublk->num_of_free_inodes));
    offset += sizeof(sublk->num_of_free_inodes);

    memcpy(block + offset, sublk->free_inode_list, sizeof(sublk->free_inode_list));
    offset += sizeof(sublk->free_inode_list);

    memcpy(block + offset, &(sublk->index_of_next_free_inode), sizeof(sublk->index_of_next_free_inode));
    offset += sizeof(sublk->index_of_next_free_inode);

    memcpy(block + offset, sublk->lock_fields, sizeof(sublk->lock_fields));
    offset += sizeof(sublk->lock_fields);

    memcpy(block + offset, &(sublk->super_block_modified), sizeof(sublk->super_block_modified));
    offset += sizeof(sublk->super_block_modified);
    if (block_write(block, SUBLOCK_NUM, dev) == -1) {
        perr("Block Write has failed in writing the Super Block to the file.");
        return 1;
    }
    return 0;
}

SuBlk * read_sublk(bfs * dev) {
    char store[BLOCK_SIZE];
    sublk* blk = (sublk*) talloc(sizeof(sublk));
    if(block_read(store, SUBLOCK_NUM, dev) == -1) {
        perr("Block Read has failed in reading the Super Block from the file.");
        return NULL;
    }
    memcpy(blk, store, sizeof(sublk));
    return blk;
}