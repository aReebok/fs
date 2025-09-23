#include "gtest/gtest.h"

extern "C" {
    #include "bufcache.h"
    #include "diskdrv.h"
}

TEST(bufcache, initialization_test) {
    BCache * bc = initialize_cache();
    ASSERT_NE(bc, nullptr);
    tfree();
}

TEST(bufcache, search_hq_empty) {
    struct BCache * bc = initialize_cache();
    ASSERT_EQ(search_hq(-1, NULL), nullptr);
    ASSERT_EQ(search_hq(-1, bc), nullptr);
    ASSERT_EQ(search_hq(10, bc), nullptr);
    ASSERT_EQ(search_hq(1e8, bc), nullptr);
    tfree();
}

TEST(bufcache, bufcache_insert_test) {
    BCache * bc = initialize_cache();

    Buffer * bad_buf = create_buf(-1, -1, 3);
    Buffer * good_buf = create_buf(0, 6 * BLOCK_SIZE, 0);
    
    EXPECT_EQ(bcache_insert(NULL, NULL), 1);
    EXPECT_EQ(bcache_insert(bad_buf, bc), 1);
    EXPECT_EQ(bcache_insert(good_buf, bc), 0);

    cdllist * tempList = bc->BUF_FREE_LIST;
    bc->BUF_FREE_LIST = NULL;
    EXPECT_EQ(bcache_insert(good_buf, bc), 1);
    bc->BUF_FREE_LIST = tempList;
    tempList = bc->BUF_HASH_QUEUE;
    bc->BUF_HASH_QUEUE = NULL;
    EXPECT_EQ(bcache_insert(good_buf, bc), 1);

    bc->BUF_HASH_QUEUE = tempList;
    EXPECT_EQ(bcache_insert(good_buf, bc), 0);
    tfree();
}

// TEST(bufcache, BC_insert_test_pass) {

// }

TEST(bufcache, getblk_test) {
    ssd = initialize_fs(VFS);
    // BCache* bc = initialize_cache();
    /* scenario 2 - blk not in HQ, take buffer off free list */
    // TODO fix below failing getblk call
    // Buffer* getblk_invalid = getblk(-100, bc);
    // Buffer* buf_blk3 = getblk(3, bc);
    // EXPECT_STREQ(buf_blk3->data, "test disk block 3 content here");

    // TODO test to make sure buf decresed in size, but it's still on HQ?
    // TODO test to make sure buf is locked

    /* TODO scenario 1 - block in HQ, block is ready */
    /* TODO scenario 3 - block NOT in HQ, block from FL is DELAY WRITE */
    /* TODO scenario 4 - block NOT in HQ, no buf avail on FL */
    /* TODO scenario 5 - block in HQ, buf marked locked */
    

    tfree();
}

TEST(bufcache, bread_test) {
    ssd = initialize_fs(VFS);
    BCache* bc = initialize_cache();

    Buffer* b_invalid = bread(-1, bc);
    EXPECT_EQ(b_invalid, nullptr);

    Buffer* not_in_bc = bread(4, bc);
    EXPECT_STREQ(not_in_bc->data, "test disk block 4 content here");
    EXPECT_EQ(not_in_bc->status & B_LOCKED, B_LOCKED);
    EXPECT_EQ(not_in_bc->status & B_VALID, B_VALID);
    brelse(not_in_bc, bc);

    /* hits the case where block from getblk is valid */
    Buffer* cached_buf = bread(4, bc); 
    EXPECT_STREQ(cached_buf->data, "test disk block 4 content here");


    // TODO should reading an unused block return NULL?
    // Buffer* no_block = bread(40, bc); 
    // EXPECT_EQ(no_block, nullptr);
    // print_buffer(no_block);

    tfree();
}

// TEST(bufcache, breada_test) {
// }

TEST(bufcache, bwrite_test) {
    ssd = initialize_fs(VFS);
    BCache* bc = initialize_cache();
    Buffer* valid_buf = bread(7, bc); 
    const char* overwrite_data = "Hello world";
    int c = 0;
    while(overwrite_data[c] != '\0' && c < BLOCK_SIZE) {
        valid_buf->data[c] = overwrite_data[c];
        c++;
    }
    valid_buf->data[c] = '\0';

    bwrite(valid_buf, bc);
    brelse(valid_buf, bc);
    Buffer* tmp_read = bread(7, bc); 
    EXPECT_STREQ(tmp_read->data, "Hello world");

    tfree();
}

TEST(bufcache, brelse_test) {
    ssd = initialize_fs(VFS);
    BCache* bc = initialize_cache();

    Buffer* valid_buf = bread(5, bc); 
    Buffer* invalid_buf = bread(6, bc); 
    invalid_buf->status &= ~B_VALID;  

    /* valid buffer -> free list tail (MRU) */
    brelse(valid_buf, bc);
    Buffer* tmp_tail = container_of(bc->BUF_FREE_LIST->prev, Buffer, fl_hook);
    EXPECT_STREQ(tmp_tail->data, "test disk block 5 content here");
    EXPECT_EQ(valid_buf->status & B_VALID, B_VALID);
    EXPECT_EQ(valid_buf->status & B_LOCKED, 0);

    /* invalid buffer -> free list head (LRU) */
    brelse(invalid_buf, bc);
    Buffer * tmp_head = container_of(bc->BUF_FREE_LIST->next, Buffer, fl_hook);
    EXPECT_STREQ(tmp_head->data, "test disk block 6 content here");

    tfree();
}
