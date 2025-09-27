#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include "inode.h"
#include "inocache.h"
#include "diskdrv.h"
#include "util.h"

int icache_insert(Inode* const ino, InodeCache *ic){
    if (ino == NULL || ic == NULL || ino->inode_number < 0 || ino->logical_device_no < 0) 
    return 1;

    plog("> Adding an Inode to Inode Cache pool: ...\n");
    int ino_h = hash_inode(ino);

    if (ino_h < 0) return 1;
    if (insert_head(&ino->fl_hook, ic->INO_FREE_LIST)) {
        perr("Error: Could not add inode to FREE LIST");
        return 1;
    }
    if (insert_tail(&ino->hq_hook, ic->INO_HASH_QUEUE + ino_h)) {
        perr("Error: Could not add inode to HASH QUEUE");
        return 1;
    }
    return 0;

}
InodeCache* initialize_icache() {
    InodeCache* ic = talloc(sizeof(*ic));
    // CHECK_NULL(ic);

    ic->INO_FREE_LIST = talloc(sizeof(cdllist));
    // CHECK_NULL(ic->INO_FREE_LIST);
    
    ic->INO_FREE_LIST->next = ic->INO_FREE_LIST;
    ic->INO_FREE_LIST->prev = ic->INO_FREE_LIST;

    ic->INO_HASH_QUEUE = talloc(sizeof(cdllist) * IHASH_SIZE);
    // CHECK_NULL(ic->INO_HASH_QUEUE);

    for(int i = 0; i < IHASH_SIZE; i++) {
        ic->INO_HASH_QUEUE[i].next = ic->INO_HASH_QUEUE + i;
        ic->INO_HASH_QUEUE[i].prev = ic->INO_HASH_QUEUE + i;
    }

    for (int ino_num = 0; ino_num < ICACHE_SIZE; ino_num++) { // initialize invalid inodes [status = 0]
        DiskInode* temp_dino = create_dinode(1, 1, time(NULL), time(NULL), time(NULL), 1, 0);
        Inode* temp_ino = create_inode(temp_dino, 0, 1, ino_num); 
        icache_insert(temp_ino, ic);
    }

    return ic;
}

void print_ihash_queue(InodeCache *ic) {
    plog("Printing Inode HashQueue head nodes");
    char str[200];
    for (int i = 0; i < IHASH_SIZE; i++) {
        sprintf(str, "Bucket %d: head=%p next=%p prev=%p size=%d\n",
               i, ic->INO_HASH_QUEUE + i,
               ic->INO_HASH_QUEUE[i].next,
               ic->INO_HASH_QUEUE[i].prev,
            size(ic->INO_HASH_QUEUE + i));
        plog(str);
    }
    plog("");
}

Inode* search_ino_hq(int ino_num, InodeCache *ic) {
    if (ino_num < 0 || ic == NULL)
        return NULL;
    int ino_h = hash_inonum(ino_num);
    cdllist* hque = ic->INO_HASH_QUEUE + ino_h;
    cdllist* iter = hque->next;
    while (iter != hque) {
        Inode* ino = container_of(iter, Inode, hq_hook);
        if(ino->inode_number == ino_num)
            return ino;
        iter = iter->next;
    }
    return NULL;
}

Inode* iget(int const ino_num, InodeCache *ic);
void iput(Inode* incore_ino, InodeCache *ic);


