#include "buffer.h"
#include "talloc.h"

// struct definition
struct BCache {
    cdllist * BUF_HASH_QUEUE;
    cdllist * BUF_FREE_LIST;
};

int bcache_insert(Buffer * const buf, struct BCache *bc);
struct BCache initialize_cache();
void print_hash_queue(struct BCache *bc);
Buffer * search_hq(int block_num, struct BCache *bc);

Buffer getblk(int const blk_num);
int brelse(int const buf_num);
int bread(int const blk_num, int read_ahead_count);
int bwrite(Buffer *);

