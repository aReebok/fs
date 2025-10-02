#include "su_blk.h"
#include "bfs_drv.h"
#include "util.h"

int write_sublk(SuBlk * sublk, bfs * dev) {
    Buffer * block = talloc(sizeof(BLOCK_SIZE));
    int offset = 0;

    memcpy(block + offset, sublk->file_system_size, sizeof(sublk->file_system_size));
    offset += sizeof(sublk->file_system_size);

    memcpy(block + offset, sublk->num_of_free_blocks, sizeof(sublk->num_of_free_blocks));
    offset += sizeof(sublk->num_of_free_blocks);

    memcpy(block + offset, sublk->free_block_list, sizeof(sublk->free_block_list));
    offset += sizeof(sublk->free_block_list);

    memcpy(block + offset, sublk->inode_list_size, sizeof(sublk->inode_list_size));
    offset += sizeof(sublk->inode_list_size);

    memcpy(block + offset, sublk->num_of_free_inodes, sizeof(sublk->num_of_free_inodes));
    offset += sizeof(sublk->num_of_free_inodes);

    memcpy(block + offset, sublk->free_inode_list, sizeof(sublk->free_inode_list));
    offset += sizeof(sublk->free_inode_list);

    memcpy(block + offset, sublk->index_of_next_free_inode, sizeof(sublk->index_of_next_free_inode));
    offset += sizeof(sublk->index_of_next_free_inode);

    memcpy(block + offset, sublk->lock_fields, sizeof(sublk->lock_fields));
    offset += sizeof(sublk->lock_fields);

    memcpy(block + offset, sublk->super_block_modified, sizeof(sublk->super_block_modified));
    offset += sizeof(sublk->super_block_modified);

    if (block_write(block, SUBLOCK_NUM, dev) == -1) {
        perr("Block Write has failed in writing the Super Block to the file.");
        return 1;
    }
    return 0;
}


SuBlk * read_sublk(bfs * dev) {
    return NULL;
}