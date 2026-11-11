#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "Subset.hpp"

using namespace subset;

TEST(Subset, StartsWithTheKFirstElements) {
    Subset s(3, 5);
    EXPECT_TRUE(s.contains(0));
    EXPECT_TRUE(s.contains(2));
    EXPECT_FALSE(s.contains(3));
    EXPECT_EQ(s.index(), 7u);  // bits 00111
}

TEST(Subset, AddAndMinusChangeOneBit) {
    Subset s(0, 5);
    s.add(0);
    s.add(3);
    EXPECT_EQ(s.index(), 9u);  // bits 01001, the example of the TP
    s.minus(0);
    EXPECT_FALSE(s.contains(0));
    EXPECT_TRUE(s.contains(3));
}

TEST(Subset, PrintsItsElements) {
    Subset s(0, 5);
    s.add(0);
    s.add(3);
    std::ostringstream out;
    out << s;
    EXPECT_EQ(out.str(), "{0,3}");
}

TEST(Subset, IteratesOverItsElementsInOrder) {
    Subset s(0, 10);
    s.add(7);
    s.add(1);
    s.add(4);
    std::vector<std::size_t> elements;
    for (std::size_t element : s) {
        elements.push_back(element);
    }
    EXPECT_EQ(elements, std::vector<std::size_t>({1, 4, 7}));
}

TEST(Subset, RejectsInvalidSizesAndElements) {
    EXPECT_THROW(Subset(4, 3), std::invalid_argument);
    EXPECT_THROW(Subset(1, 65), std::invalid_argument);
    Subset s(1, 3);
    EXPECT_THROW(s.contains(3), std::out_of_range);
    EXPECT_THROW(s.add(3), std::out_of_range);
    EXPECT_THROW(s.minus(3), std::out_of_range);
}

TEST(Subset, HandlesTheFullSixtyFourBits) {
    Subset s(64, 64);
    EXPECT_TRUE(s.contains(63));
    EXPECT_EQ(s.index(), ~0ULL);
}
