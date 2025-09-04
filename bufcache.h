#include "buffer.h"
#include "talloc.h"

// struct definition
struct BCache {
    free_list * BUF_HASH_QUEUE;
    free_list * BUF_FREE_LIST;
};

int BCache_Insert(Buffer * const buf, struct BCache bc);
struct BCache initialize_cache();
void print_hash_queue(struct BCache bc);

Buffer getblk(int const blk_num);
int brelse(int const buf_num);
int bread(int const blk_num, int read_ahead_count);
int bwrite(Buffer *);

