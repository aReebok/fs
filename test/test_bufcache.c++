#include "gtest/gtest.h"

extern "C" {
    #include "../bufcache.h"
}

TEST(bufcache, initialization_test) {
    struct BCache * bc = initialize_cache();
    ASSERT_NE(bc, (void*) NULL);
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

TEST(bufcache, BC_insert_test) {
    Buffer * bad_buf = create_buf(-1, -1, 3);
    Buffer * good_buf = create_buf(0, 6 * BLOCK_SIZE, 0);

    struct BCache * bc = initialize_cache();

    EXPECT_EQ(bcache_insert(NULL, NULL), 0);
    EXPECT_EQ(bcache_insert(bad_buf, bc), 0);
    EXPECT_EQ(bcache_insert(good_buf, bc), 0);

    cdllist * tempList = bc->BUF_FREE_LIST;
    bc -> BUF_FREE_LIST = NULL;
    EXPECT_EQ(bcache_insert(good_buf, bc), 1);
    bc -> BUF_FREE_LIST = tempList;
    tempList = bc -> BUF_HASH_QUEUE;
    bc -> BUF_HASH_QUEUE = NULL;
    EXPECT_EQ(bcache_insert(good_buf, bc), 1);

    bc -> BUF_HASH_QUEUE = tempList;
    EXPECT_EQ(bcache_insert(good_buf, bc), 0);
    tfree();
}

// TEST(bufcache, BC_insert_test_pass) {
// }
// TEST(bufcache, getblk_test) {
// }
// TEST(bufcache, bread_test) {
// }
// TEST(bufcache, breada_test) {
// }
// TEST(bufcache, bwrite_test) {
// }
// TEST(bufcache, brelse_test) {
// }
