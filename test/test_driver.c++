#include "gtest/gtest.h"

extern "C" {
    #include "driver.h"
}
TEST(driver, block_read_and_write) {
    floppy = mkbfs("test.bfs");
    const char* msg5 = "5 this is the block number 5 in the bfs";
    const char* msg6 = "6 this is the block number 6 in the bfs";
    const char* msg7 = "7 this is the block number 7 in the bfs";
    const char* msg100 = "100 this is the block number 100 in the bfs";

    char w_store[BLOCK_SIZE];
    memcpy(w_store, msg5, strlen(msg5) + 1); // make sure to add + 1 to copy null byte
    int w_len = block_write(w_store, 5, floppy);
    EXPECT_EQ(w_len, BLOCK_SIZE);

    memcpy(w_store, msg6, strlen(msg6) + 1);
    w_len = block_write(w_store, 6, floppy);
    memcpy(w_store, msg7, strlen(msg7) + 1);
    w_len = block_write(w_store, 7, floppy);
    memcpy(w_store, msg100, strlen(msg100) + 1);
    w_len = block_write(w_store, 100, floppy);

    char r_store[BLOCK_SIZE];
    int r_len = block_read(r_store, 100, floppy);
    EXPECT_EQ(r_len, BLOCK_SIZE);
    EXPECT_STREQ(r_store, msg100);

    r_len = block_read(r_store, 6, floppy);
    EXPECT_STREQ(r_store, msg6);

    r_len = block_read(r_store, 5, floppy);
    EXPECT_STREQ(r_store, msg5);

    r_len = block_read(r_store, 7, floppy);
    EXPECT_STREQ(r_store, msg7);

    tfree();
}