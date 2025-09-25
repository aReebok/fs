#include "gtest/gtest.h"

extern "C" {
    #include "talloc.h"
}

TEST(talloc, list_insertion) {
	int* a = (int *) talloc(sizeof(*a) * 10);
	ASSERT_NE(a, nullptr);
	float *b = (float *) talloc(sizeof(*b) * 1024);
	ASSERT_NE(b, nullptr);
	tfree();
}

TEST(talloc, allocate_bounds) {
	int * a = (int *) talloc(0);
	ASSERT_EQ(a, nullptr);

	a = (int *) talloc(-100);
	ASSERT_EQ(a, nullptr);

    size_t large_size = (size_t)-1; // or SIZE_MAX
    void * b = talloc(large_size);
	ASSERT_EQ(b, nullptr);

	tfree();
}