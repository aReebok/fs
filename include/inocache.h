/* In-core inode cache system
 *    Allows for fast retrival us recently used inodes
 *    Saves time in bread, if inode already in inode cache
 */

#ifndef _INOCACHE_H
#define _INOCACHE_H

#include "inode.h"
#include "talloc.h"

#define ICACHE_SIZE     8

typedef struct InodeCache {
    cdllist* INO_HASH_QUEUE;
    cdllist* INO_FREE_LIST;
} InodeCache;

InodeCache* initialize_icache();
int icache_insert(Inode* const ino, InodeCache *ic);
Inode* search_ino_hq(int ino_num, InodeCache *ic);
void print_ihash_queue(InodeCache *ic);

Inode* iget(int const ino_num, InodeCache *ic);
void iput(Inode* incore_ino, InodeCache *ic);

#endif // _INOCACHE_H