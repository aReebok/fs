#include "buffer.h"
#include "talloc.h"

#define BCACHE_SIZE     16

typedef struct BCache BCache;
struct BCache {
    cdllist * BUF_HASH_QUEUE;
    cdllist * BUF_FREE_LIST;
};

int bcache_insert(Buffer * const buf, struct BCache *bc);
struct BCache * initialize_cache();
void print_hash_queue(struct BCache *bc);
Buffer * search_hq(int block_num, struct BCache *bc);

Buffer * getblk(int const blk_num, struct BCache *bc);
void brelse(Buffer* locked_buf, struct BCache *bc);
Buffer * bread(int const blk_num, struct BCache *bc);
Buffer * breada(int const blk_num, int read_ahead_count, struct BCache *bc); // TODO: need to draft this
void bwrite(Buffer* buf, struct BCache *bc);
