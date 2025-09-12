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
    ASSERT_EQ(search_hq(10, bc), (void *) NULL);
    ASSERT_EQ(search_hq(-1, bc), (void *) NULL);
    ASSERT_EQ(search_hq(1e8, bc), (void *) NULL);
    tfree();
}

TEST(bufcache, BC_insert_test) {
    //TODO
}
TEST(bufcache, getblk_test) {
    //TODO
}
TEST(bufcache, bread_test) {
    //TODO
}
TEST(bufcache, breada_test) {
    //TODO
}
TEST(bufcache, bwrite_test) {
    //TODO
}
TEST(bufcache, brelse_test) {
    //TODO
}
