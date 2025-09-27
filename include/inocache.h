#ifndef _INOCACHE_H
#define _INOCACHE_H

#include "inode.h"
#include "talloc.h"

#define ICACHE_SIZE     8

typedef struct InodeCache InodeCache;
struct InodeCache {
    cdllist* INO_HASH_QUEUE;
    cdllist* INO_FREE_LIST;
};

int icache_insert(Inode* const ino, InodeCache *ic);
InodeCache* initialize_icache();
void print_ihash_queue(InodeCache *ic);
Inode* search_ino_hq(int ino_num, InodeCache *ic);

Inode* iget(int const ino_num, InodeCache *ic);
void iput(Inode* incore_ino, InodeCache *ic);


#endif // _INOCACHE_H