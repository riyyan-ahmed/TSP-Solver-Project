#include <gtest/gtest.h>
#include "utils.hpp"

TEST(AdditionTest, BasicCases) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(7, 5), 12);
    EXPECT_EQ(add(0, 5), 5);
}
