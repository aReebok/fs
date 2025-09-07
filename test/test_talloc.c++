#include "gtest/gtest.h"

extern "C" {
    #include "../talloc.h"
}

TEST(talloc, allocate_pass) {
	int* a = (int *) talloc(sizeof(*a) * 10);
	ASSERT_NE(a, (void*) NULL);
	float *b = (float *) talloc(sizeof(*b) * 1024);
	ASSERT_NE(b, (void*) NULL);
	tfree();
}

TEST(talloc, allocate_fail) {
	int * a = (int *) talloc(0);
	ASSERT_EQ(a, (void*) NULL);
	a = (int *) talloc(-100);
	ASSERT_EQ(a, (void*) NULL);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
