#include "su_blk.h"
#include "util.h"

sublk* create_empty_sublk() {
    sublk* empty_sublk = talloc(sizeof(sublk));
    memset(empty_sublk, 0, sizeof(sublk));
    return empty_sublk;
}

sublk* create_sublk() {
    sublk* new_sublk = create_empty_sublk();
    new_sublk->file_system_size = 97;
    new_sublk->num_of_free_blocks = 10000;
    new_sublk->free_block_list[0] = 99;
    new_sublk->free_block_list[1] = 98;
    new_sublk->free_block_list[2] = 97;
    new_sublk->free_block_list[100] = 96;
    new_sublk->inode_list_size = 64;
    new_sublk->num_of_free_inodes = 64;
    for (int i = 0; i < 64; i++) {
        new_sublk->free_inode_list[i] = i;
    }
    new_sublk->index_of_next_free_inode = 0;
    new_sublk->lock_fields = 0;
    new_sublk->super_block_modified = 55;
    return new_sublk;
}

char* sublk_to_str(sublk* s) {
    char* block;
    
    if((block = talloc(BLOCK_SIZE)) == NULL) // TODO better error report: use the plog stuff probably right
        return NULL;

    int offset = 0;
    memcpy(block + offset, &(s->file_system_size), sizeof(s->file_system_size));
    offset += sizeof(s->file_system_size);

    memcpy(block + offset, &(s->num_of_free_blocks), sizeof(s->num_of_free_blocks));
    offset += sizeof(s->num_of_free_blocks);

    memcpy(block + offset, s->free_block_list, sizeof(s->free_block_list));
    offset += sizeof(s->free_block_list);

    memcpy(block + offset, &(s->next_free_block_index), sizeof(s->free_block_list));
    offset += sizeof(s->next_free_block_index);

    memcpy(block + offset, &(s->inode_list_size), sizeof(s->inode_list_size));
    offset += sizeof(s->inode_list_size);

    memcpy(block + offset, &(s->num_of_free_inodes), sizeof(s->num_of_free_inodes));
    offset += sizeof(s->num_of_free_inodes);

    memcpy(block + offset, s->free_inode_list, sizeof(s->free_inode_list));
    offset += sizeof(s->free_inode_list);

    memcpy(block + offset, &(s->index_of_next_free_inode), sizeof(s->index_of_next_free_inode));
    offset += sizeof(s->index_of_next_free_inode);

    memcpy(block + offset, &(s->lock_fields), sizeof(s->lock_fields));
    offset += sizeof(s->lock_fields);

    memcpy(block + offset, &(s->super_block_modified), sizeof(s->super_block_modified));
    offset += sizeof(s->super_block_modified);

    return block;
}

sublk* read_sublk(char store[]) {
    sublk* blk = (sublk*) talloc(sizeof(sublk));
    memcpy(blk, store, sizeof(sublk));
    return blk;
}

// github copilot generated - prints super block information
void print_sublk(const sublk* s) { 
    if (!s) {
        printf("sublk: (null)\n");
        return;
    }

    printf("Superblock (sublk):\n");
    printf("  file_system_size        : %d\n", s->file_system_size);
    printf("  num_of_free_blocks      : %d\n", s->num_of_free_blocks);

    printf("  free_block_list (%d)   :", SU_FREE_BLOCK_LIST_SIZE);
    for (int i = 0; i < SU_FREE_BLOCK_LIST_SIZE; ++i) {
        if (i % 16 == 0) printf("\n    ");
        printf("%6u", (unsigned)s->free_block_list[i]);
    }
    printf("\n");

    printf("  inode_list_size         : %d\n", s->inode_list_size);
    printf("  num_of_free_inodes      : %d\n", s->num_of_free_inodes);

    printf("  free_inode_list (%d)    :", SU_FREE_INODE_LIST_SIZE);
    for (int i = 0; i < SU_FREE_INODE_LIST_SIZE; ++i) {
        if (i % 16 == 0) printf("\n    ");
        printf("%6u", (unsigned)s->free_inode_list[i]);
    }
    printf("\n");

    printf("  index_of_next_free_inode: %d\n", s->index_of_next_free_inode);
    printf("  lock_fields             : %d\n", s->lock_fields);
    printf("  super_block_modified    : %d\n", s->super_block_modified);
    for (int i = 0; i < 125; ++i) putchar('=');
    putchar('\n');
}