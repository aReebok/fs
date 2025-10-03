#include <stdio.h>
#include <stdlib.h>
#include "cdllist.h"
#include "talloc.h"
#include "buffer.h"
#include "bufcache.h"
#include "inode.h"
#include "inocache.h"
#include "diskdrv.h"
#include "bfs_drv.h"
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
    struct BCache * buffer_cache = initialize_cache();
    ssd = initialize_fs(VFS);
    floppy = initialize_bfs("floppy.bfs");

    printf("size of sublock is %ld\n\n", sizeof(SuBlk));
    sublk* temp = create_sublk();
    write_sublk(temp, floppy);

    sublk* temp2 = read_sublk(floppy);
    printf("SuBlk fields from temp2:\n");
    printf("  file_system_size: %d\n", temp2->file_system_size);
    printf("  num_of_free_blocks: %d\n", temp2->num_of_free_blocks);
    printf("  free_block_list: ");
    for (int i = 0; i < 164; i++) {
        printf("%u ", temp2->free_block_list[i]);
    }
    printf("\n");
    printf("  inode_list_size: %d\n", temp2->inode_list_size);
    printf("  num_of_free_inodes: %d\n", temp2->num_of_free_inodes);
    printf("  free_inode_list: ");
    for (int i = 0; i < 64; i++) {
        printf("%u ", temp2->free_inode_list[i]);
    }
    printf("\n");
    printf("  index_of_next_free_inode: %d\n", temp2->index_of_next_free_inode);
    printf("  lock_fields: ");
    for (int i = 0; i < LF_COUNT; i++) {
        printf("%d ", temp2->lock_fields[i]);
    }
    printf("\n");
    printf("  super_block_modified: %d\n", temp2->super_block_modified);
    
    // printf("size of inode-disk %lu\n", sizeof(DiskInode)); 
    // printf("size of inode-incore %lu\n", sizeof(Inode));

    // InodeCache* inode_cache = initialize_icache();

    // for (int i = 0; i < 6; i++) { // populates with Inodes
    //     DiskInode* temp_dino = create_dinode(1, 1, time(NULL), time(NULL), time(NULL), 1, 0);
    //     Inode* temp_ino = create_inode(temp_dino, 0, 1, i); 
    //     icache_insert(temp_ino, inode_cache);
    // }

    // print_list_links(inode_cache->INO_FREE_LIST);
    // print_hash_queue(inode_cache);


    // print_list_links(inode_cache->INO_FREE_LIST);                   // print out links
    // print_hash_queue(inode_cache);
    // print_inode_info_free_list(inode_cache->INO_FREE_LIST);        // print out actual inode information
    
    // quick check on hash searching:
    // puts("Printing Inode hash search");
    // Inode* temp = search_ino_hq(4, inode_cache);
    // print_inode(temp);

    // puts("=====Exiting Main: Safe exiting. Deleting RAM=====");
    texit(0);
}


