#include <stdio.h>
#include <stdlib.h>
#include "cdllist.h"
#include "talloc.h"
#include "buffer.h"
#include "bufcache.h"
#include "inode.h"
#include "inocache.h"
#include "diskdrv.h"
// #include "bfs_drv.h"
// #include "bfs.h"
#include "driver.h"
#include "inode.h"
#include "su_blk.h"

void print_list_links(cdllist* list) { // for debugging
    cdllist* tmp = list;
        if (is_empty(tmp)) {
            printf("Printing empty list\n");
        } else {
            printf("Printing non-empty list\n");
        };
    printf("\tHEAD\n");
    do {
        printf("\t{%p: next[%p] prev[%p]}\n", tmp, tmp->next, tmp->prev);
        tmp = tmp->next;
    } while(tmp != list);
    printf("\tTAIL\n\n");
}

void print_buffer_info_free_list(cdllist* list) { // for debugging
    if (is_empty(list)) {
        printf("Empty list\n");
        return;
    } else {
        printf("> Printing actual buffer content from free list...\n");
    };

    cdllist* curr = list->next;
    Buffer* temp;
    do {
        temp = container_of(curr, Buffer, fl_hook);
        print_buffer(temp);
        curr = curr->next;
    } while(curr != list);
}

void print_inode(Inode* const ino) {
    printf("    i{Location dev %d ino %d \t| Status: %d | Ref ct: %d},\n", \
        ino->logical_device_no, ino->inode_number, ino->status, ino->reference_count);
    printf("\tOn disk information:\n");

    long int inolastmod = (long int)ino->dino->inode_last_modified;
    long int filelastacc = (long int)ino->dino->last_accessed;
    long int filelastmod = (long int)ino->dino->last_modified;
    printf(".  inod last modified: \t%s", ctime( &inolastmod) );
    printf(".  file last accessed: \t%s", ctime(&filelastacc) );
    printf(".  file last modified: \t%s", ctime(&filelastmod) );
    printf("------------------------------------------------------\n");
}

void print_inode_info_free_list(cdllist* list) { // for debugging
    if (is_empty(list)) {
        printf("Empty free inode list \n");
        return;
    } else {
        printf("> Printing actual Inode content from free list...\n");
    };

    cdllist* curr = list->next;
    Inode* temp;
    do {
        temp = container_of(curr, Inode, fl_hook);
        print_inode(temp);
        curr = curr->next;
    } while(curr != list);
}


int main() {
    floppy = mkbfs("floppy.bfs");

    printf("size of sublock is %ld\n\n", sizeof(sublk));
    print_sublk(floppy->incore_sblk);
    block_write(sublk_to_str(floppy->incore_sblk), SUBLOCK_NUM, floppy);

    char store[BLOCK_SIZE];
    if (block_read(store, SUBLOCK_NUM, floppy) == -1) {
        perr("Block Read has failed in reading the Super Block from the file.");
        return 1;
    }

    sublk* temp2 = read_sublk(store);
    print_sublk(temp2);                 // TODO URGENT fix this, expected lock_fields = 0; returned lock_fields: 55 [super_block_modified should be 55........]

    puts("=====Exiting Main: Safe exiting. Deleting RAM=====");
    texit(0);
}


