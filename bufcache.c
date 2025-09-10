#include <stdio.h>
#include <unistd.h>
#include "bufcache.h"
#include "diskdrv.h"


extern diskdrv* ssd;

int bcache_insert(Buffer * const buf, struct BCache * bc) {
    int buf_h = hash_buffer(buf);
    // TODO: ERROR CHECKING and return 1
    insert_head(&buf -> fl_hook, bc -> BUF_FREE_LIST);
    insert_tail(&buf -> hq_hook, bc -> BUF_HASH_QUEUE + buf_h);
    return 0;
}

struct BCache * initialize_cache() {
    struct BCache * bc = talloc(sizeof(BCache));

    bc->BUF_FREE_LIST = talloc(sizeof(cdllist));
    bc->BUF_FREE_LIST->next = bc->BUF_FREE_LIST;
    bc->BUF_FREE_LIST->prev = bc->BUF_FREE_LIST;
    // TODO: lets add a few buffer free list in init code

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

extern void print_buffer_info_free_list(cdllist* list);

Buffer* getblk(int const blk_num, struct BCache *bc) {
    Buffer* ret = NULL;
    // 10 seconds... If buffer is not available in 10 seconds, return NULL...
    // TODO: Exception throwing maybe?
    int sleep_timer = 10 * 1000; 
    // may need to change this slightly (null check) to not stall as we have just one process rn
    while(ret == NULL) { 
        Buffer* buf = search_hq(blk_num, bc);
        if (buf != NULL) { // Buffer found in Hash Queue
            if (buf->status & B_LOCKED) { // Buffer is busy
                // printf("Buffer found in HQ; buffer is busy. Sleeping...\n");
                sleep(1000);
                sleep_timer -= 1000;

                if (sleep_timer < 0)
                    return NULL;
                continue;
            }
            
            buf->status |= B_LOCKED;
            remove_from_list(&buf -> fl_hook);
            ret = buf;
        }
        else {
            cdllist* node = remove_from_head(bc->BUF_FREE_LIST);
            if(node == NULL) {
                printf("sleeping...\n");
                sleep(5);
                continue;
            }

            Buffer* buf = container_of(node, Buffer, fl_hook);
            if (buf->status & B_DELWRI) {
                data_block_write(buf, ssd);
                continue;
            }
            
            buf->status |= B_LOCKED;
            remove_from_list(&buf -> hq_hook);

            buf->device_no = 0; // TODO: Need to bring in dev#... no device support till .vfs design is concrete
            buf->block_no = blk_num;
            buf->status |= B_WANTED; // TODO: Temp status... what's a good status here?
            
            bcache_insert(buf, bc);
            ret = buf;
        }
    }
    return ret;
}

Buffer* bread(int const blk_num, struct BCache *bc) {
    if (blk_num < 0) {
        printf("ERROR in bread: Invalid block number: %d\n", blk_num);
        return NULL;
    }
    Buffer* buf = getblk(blk_num, bc);
    if (buf == NULL) {
        puts("ERROR in bread: Buffer not found. getlblk failed");
        return NULL;
    }
    if (buf->status & B_VALID)
        return buf;

    data_block_read(buf, ssd);
    buf->status |= B_VALID;
    // process sleeps till read
    return buf;
}

void brelse(Buffer* locked_buf, BCache* bc) {
    // wakeup all procs: event, waiting for any buffer to become free;
    // wakeup all procs: event, waiting for this buffer to become free;
    // raise processor execution level to block interrupts; 
    if (locked_buf->status & B_VALID) // TODO: and buffer not old [textbook pseudocode]
        insert_tail(&locked_buf->fl_hook, bc->BUF_FREE_LIST);
    else
        insert_head(&locked_buf->fl_hook, bc->BUF_FREE_LIST);
    // lower processor execution level to allow interrupts;
    printf("Buffer 3 status:%d \n", (locked_buf->status & B_VALID));
    locked_buf->status &= ~B_LOCKED; // unlock(buffer)
}

void bwrite(Buffer* buf, struct BCache *bc) {
    // TODO: see textbook impl
    // initiate disk write;
    printf("Inside bwrite()\n");

    int len = data_block_write(buf, ssd);
    printf("%d\n", len);
    // if (I/O synchronous){
        // sleep(event I/O complete);
        // release buffer
        brelse(buf, bc);
    // }
    if (buf->status & B_DELWRI) {
        // mark buffer to put at head of free list: 
    };
}
