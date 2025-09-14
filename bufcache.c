#include <stdio.h>
#include "bufcache.h"
#include <unistd.h>

#define CHECK_NULL(var) \
    if ((var) == NULL) { \
        printf("%s\n", "Critical Talloc Failure. Halting Buffer Cache Initialization Process");\
        return NULL; \
    }

int bcache_insert(Buffer * const buf, struct BCache * bc) {
    printf("> Adding a buffer to buffer pool: ...\n");
    int buf_h = hash_buffer(buf);
    if (insert_head(&buf -> fl_hook, bc -> BUF_FREE_LIST)) {
        printf("Error: Could not add buffer to FREE LIST");
        return 1;
    }
    if (insert_tail(&buf -> hq_hook, bc -> BUF_HASH_QUEUE + buf_h)) {
        printf("Error: Could not add buffer to HASH QUEUE");
        return 1;
    }
    return 0;
}

struct BCache * initialize_cache() {
    struct BCache * bc = talloc(sizeof(*bc));
    CHECK_NULL(bc);

    bc -> BUF_FREE_LIST = talloc(sizeof(cdllist));
    CHECK_NULL(bc -> BUF_FREE_LIST);
    
    bc -> BUF_FREE_LIST -> next = bc->BUF_FREE_LIST;
    bc -> BUF_FREE_LIST -> prev = bc->BUF_FREE_LIST;

    bc->BUF_HASH_QUEUE = talloc(sizeof(cdllist) * HASH_SIZE);
    CHECK_NULL(bc -> BUF_HASH_QUEUE);

    for(int i = 0; i < HASH_SIZE; i++) {
        // Does this abomination work as I think it does?
        bc->BUF_HASH_QUEUE[i].next = bc->BUF_HASH_QUEUE + i;
        bc->BUF_HASH_QUEUE[i].prev = bc->BUF_HASH_QUEUE + i;
    }
    return bc;
}

void print_hash_queue(struct BCache *bc) {
    printf("Printing each HashQueue [HEAD NODE only]\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("Bucket %d: head=%p next=%p prev=%p size=%d\n",
               i, bc -> BUF_HASH_QUEUE + i,
               bc -> BUF_HASH_QUEUE[i].next,
               bc -> BUF_HASH_QUEUE[i].prev,
            size(bc -> BUF_HASH_QUEUE + i));
    }
    printf("\n");
}

Buffer * search_hq(int block_num, struct BCache *bc) {
    int buf_h = hash_block_num(block_num);
    cdllist * hque = bc -> BUF_HASH_QUEUE + buf_h;
    cdllist * iter = hque -> next;
    while (iter != hque) {
        Buffer * buf = container_of(iter, Buffer, hq_hook);
        if(buf -> block_no == block_num)
            return buf;
        iter = iter -> next;
    }
    return NULL;
}

Buffer * getblk(int const blk_num, struct BCache *bc) {
    Buffer * ret = NULL;
    // 10 seconds... If buffer is not available in 10 seconds, return NULL...
    int sleep_timer = 10 * 1000; 
    // may need to change this slightly (null check) to not stall as we have just one process rn
    while(ret == NULL) { 
        Buffer * buf = search_hq(blk_num, bc);
        if (buf != NULL) { // Buffer found in Hash Queue
            if (buf -> status == B_LOCKED) { // Buffer is busy
                sleep(1000);
                sleep_timer -= 1000;
                if (sleep_timer < 0)
                    return NULL;
                continue;
            }
            buf -> status = B_LOCKED;
            remove_from_list(&buf -> fl_hook);
            ret = buf;
        }
        else {
            // block not in hash queue
            cdllist * node = remove_from_head(bc->BUF_FREE_LIST);
            if(node == NULL) {
                sleep(5 * 1000);
                continue;
            }
            Buffer * buf = container_of(node, Buffer, fl_hook);
            if (buf -> status == B_DELWRI) {
                //  TODO: async write here to disk 
                continue;
            }
            buf -> status = B_LOCKED;
            remove_from_list(&buf -> hq_hook);

            buf -> device_no = 0; // TODO: Need to bring in dev#...
            buf -> block_no = blk_num;
            buf -> status = B_WANTED; // TODO: Temp status... what's a good status here?
            
            bcache_insert(buf, bc);
            ret = buf;
        }
    }
    return ret;
}

Buffer * bread(int const blk_num, struct BCache *bc) {
    Buffer * buf = getblk(blk_num, bc);
    if (buf == NULL) {
        puts("ERROR: Buffer not found. getlblk failed");
        return NULL;
    }
    if (buf -> status == B_WANTED) {
        // TODO: ASYNC_IO need to read data from disk to buffer
        buf->status = B_LOCKED;
    }
    return buf;
}
