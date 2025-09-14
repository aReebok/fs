#include "gtest/gtest.h"
extern "C" {
    #include "../helpercode.h"
}

int main(int argc, char **argv) {
    disable_std_print();
    printf("\n\n======================RUNNING UNIT TESTS======================\n\n");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
