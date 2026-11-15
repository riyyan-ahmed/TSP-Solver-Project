// Partly edited by Claude (Claude Opus 5.5), 2026-11-15: the two iteration tests merged into one
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
    // bit 63 is the last of the 64 bits
    Subset s(0, 64);
    s.add(63);
    s.add(1);
    s.add(32);
    std::vector<std::size_t> elements;
    for (std::size_t element : s) {
        elements.push_back(element);
    }
    EXPECT_EQ(elements, std::vector<std::size_t>({1, 32, 63}));
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

TEST(SubsetIterator, FollowsTheGosperSequenceOfTheTP) {
    // n = 5 and k = 3, the ten bit strings listed in the TP
    const std::vector<unsigned long long> expected = {
        0b00111, 0b01011, 0b01101, 0b01110, 0b10011, 0b10101, 0b10110, 0b11001, 0b11010, 0b11100};
    std::vector<unsigned long long> seen;
    for (SubsetIterator it(5, 3); it != SubsetIterator::end(); ++it) {
        seen.push_back((*it).index());
    }
    EXPECT_EQ(seen, expected);
}

TEST(SubsetIterator, CountsBinomialCoefficients) {
    // C(10, 4) = 210 subsets of size 4 in a set of 10
    std::size_t count = 0;
    for (SubsetIterator it(10, 4); it != SubsetIterator::end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 210u);
}

TEST(SubsetIterator, HandlesTheEmptyAndTheFullSubset) {
    std::size_t empty = 0;
    for (SubsetIterator it(6, 0); it != SubsetIterator::end(); ++it) {
        empty++;
    }
    EXPECT_EQ(empty, 1u);

    std::size_t full = 0;
    for (SubsetIterator it(6, 6); it != SubsetIterator::end(); ++it) {
        full++;
    }
    EXPECT_EQ(full, 1u);
}
