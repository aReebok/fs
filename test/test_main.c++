#include "gtest/gtest.h"
extern "C" {
    #include "util.h"
}

int main(int argc, char **argv) {
    disable_std_print();
    printf("\n\n======================RUNNING UNIT TESTS======================\n\n");
    ::testing::InitGoogleTest(&argc, argv);
    int test_out = RUN_ALL_TESTS();
    printf("\n=====================COMPLETED UNIT TESTS=====================\n\n");
    return test_out;
}
