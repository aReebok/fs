#include <stdio.h>
#include "bufcache.h"


int bcache_insert(Buffer * const buf, struct BCache * bc) {
    printf("> Adding a buffer to buffer pool: ...\n");
    int buf_h = hash_buffer(buf);
    //ERROR CHECKING and return 1
    insert_head(&buf -> fl_hook, bc -> BUF_FREE_LIST);
    insert_tail(&buf -> hq_hook, bc -> BUF_HASH_QUEUE + buf_h);
    return 0;
}

struct BCache * initialize_cache() {
    struct BCache * bc = talloc(sizeof(*bc));

    bc -> BUF_FREE_LIST = talloc(sizeof(cdllist));
    bc -> BUF_FREE_LIST -> next = bc->BUF_FREE_LIST;
    bc -> BUF_FREE_LIST -> prev = bc->BUF_FREE_LIST;

    bc->BUF_HASH_QUEUE = talloc(sizeof(cdllist) * HASH_SIZE);
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