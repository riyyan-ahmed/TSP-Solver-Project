// Generated with Claude Code (Claude Sonnet 5.5), 2026-10-14
#include <gtest/gtest.h>
#include <stdexcept>
#include "CoordinatesGraph.hpp"
#include "MatrixGraph.hpp"

using namespace tsp;

TEST(CoordinatesGraph, ComputesEuclideanDistance) {
    CoordinatesGraph graph({{0.0, 0.0}, {3.0, 4.0}});
    EXPECT_EQ(graph.size(), 2u);
    EXPECT_DOUBLE_EQ(graph.weight(0, 1), 5.0);
}

TEST(CoordinatesGraph, IsSymmetricWithZeroDiagonal) {
    CoordinatesGraph graph({{1.5, -2.0}, {7.25, 3.0}, {-4.0, 0.5}});
    for (std::size_t i = 0; i < graph.size(); ++i) {
        EXPECT_DOUBLE_EQ(graph.weight(i, i), 0.0);
        for (std::size_t j = 0; j < graph.size(); ++j) {
            EXPECT_DOUBLE_EQ(graph.weight(i, j), graph.weight(j, i));
        }
    }
}

TEST(CoordinatesGraph, SatisfiesTriangleInequality) {
    CoordinatesGraph graph({{0, 0}, {10, 1}, {4, 9}, {-3, 5}});
    for (std::size_t a = 0; a < graph.size(); ++a)
        for (std::size_t b = 0; b < graph.size(); ++b)
            for (std::size_t c = 0; c < graph.size(); ++c)
                EXPECT_LE(graph.weight(a, c), graph.weight(a, b) + graph.weight(b, c) + 1e-9);
}

TEST(CoordinatesGraph, RejectsEmptyInput) {
    EXPECT_THROW(CoordinatesGraph({}), std::invalid_argument);
}

TEST(CoordinatesGraph, RejectsOutOfRangeIndex) {
    CoordinatesGraph graph({{0, 0}, {1, 1}});
    EXPECT_THROW(graph.weight(2, 0), std::out_of_range);
    EXPECT_THROW(graph.weight(0, 2), std::out_of_range);
}

TEST(MatrixGraph, ReturnsStoredWeights) {
    MatrixGraph graph({{0, 5, 8}, {5, 0, 4}, {8, 4, 0}});
    EXPECT_EQ(graph.size(), 3u);
    EXPECT_DOUBLE_EQ(graph.weight(0, 2), 8.0);
    EXPECT_DOUBLE_EQ(graph.weight(1, 2), 4.0);
}

TEST(MatrixGraph, RejectsNonSquareMatrix) {
    EXPECT_THROW(MatrixGraph({{0, 1}, {1}}), std::invalid_argument);
}

TEST(MatrixGraph, RejectsEmptyMatrix) {
    EXPECT_THROW(MatrixGraph({}), std::invalid_argument);
}

TEST(MatrixGraph, RejectsOutOfRangeIndex) {
    MatrixGraph graph({{0, 1}, {1, 0}});
    EXPECT_THROW(graph.weight(2, 0), std::out_of_range);
    EXPECT_THROW(graph.weight(0, 2), std::out_of_range);
}

// Example instance from the project specification (figure 2)
TEST(TourLength, MatchesSpecificationExample) {
    MatrixGraph graph({{0, 5, 8, 6}, {5, 0, 4, 7}, {8, 4, 0, 3}, {6, 7, 3, 0}});
    EXPECT_DOUBLE_EQ(graph.tourLength({0, 1, 2, 3}), 18.0);
}

TEST(TourLength, ClosesTheCycle) {
    CoordinatesGraph graph({{0, 0}, {4, 0}, {4, 3}});
    // 4 + 3 + 5
    EXPECT_DOUBLE_EQ(graph.tourLength({0, 1, 2}), 12.0);
}

TEST(TourLength, IsIndependentOfStartAndDirection) {
    MatrixGraph graph({{0, 5, 8, 6}, {5, 0, 4, 7}, {8, 4, 0, 3}, {6, 7, 3, 0}});
    EXPECT_DOUBLE_EQ(graph.tourLength({0, 1, 2, 3}), graph.tourLength({2, 3, 0, 1}));
    EXPECT_DOUBLE_EQ(graph.tourLength({0, 1, 2, 3}), graph.tourLength({3, 2, 1, 0}));
}

TEST(TourLength, SingleCityAndEmptyTourHaveZeroLength) {
    CoordinatesGraph graph({{2, 2}});
    EXPECT_DOUBLE_EQ(graph.tourLength({0}), 0.0);
    EXPECT_DOUBLE_EQ(graph.tourLength({}), 0.0);
}

TEST(TourLength, RejectsInvalidVertex) {
    MatrixGraph graph({{0, 1}, {1, 0}});
    EXPECT_THROW(graph.tourLength({0, 2}), std::out_of_range);
}

TEST(TourLength, ChecksEveryVertexNotOnlyTheFirstOne) {
    MatrixGraph graph({{0, 1, 1}, {1, 0, 1}, {1, 1, 0}});
    EXPECT_THROW(graph.tourLength({0, 1, 7}), std::out_of_range);
}

TEST(TourLength, PrefersPerimeterOrderOnASquare) {
    // a unit square visited in perimeter order is shorter than the crossing order
    CoordinatesGraph graph({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    EXPECT_DOUBLE_EQ(graph.tourLength({0, 1, 2, 3}), 4.0);
    EXPECT_GT(graph.tourLength({0, 2, 1, 3}), 4.0);
}

TEST(CoordinatesGraph, CoincidentCitiesAreAtDistanceZero) {
    CoordinatesGraph graph({{2.5, 2.5}, {2.5, 2.5}});
    EXPECT_DOUBLE_EQ(graph.weight(0, 1), 0.0);
}
