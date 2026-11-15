// Partly edited by Claude (Claude Opus 5.5), 2026-11-14: trailing spaces and empty lines
#include <gtest/gtest.h>
#include <stdexcept>
#include "CoordinatesGraph.hpp"
#include "MatrixGraph.hpp"

using namespace tsp;

TEST(CoordinatesGraph, IsSymmetricWithZeroDiagonal) {
    CoordinatesGraph graph({{1.5, -2.0}, {7.25, 3.0}, {-4.0, 0.5}});
    for (std::size_t i = 0; i < graph.size(); i++) {
        EXPECT_DOUBLE_EQ(graph.weight(i, i), 0.0);
        for (std::size_t j = 0; j < graph.size(); j++) {
            EXPECT_DOUBLE_EQ(graph.weight(i, j), graph.weight(j, i));
        }
    }
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

TEST(TourLength, MatchesSpecificationExample) {
    MatrixGraph graph({{0, 5, 8, 6}, {5, 0, 4, 7}, {8, 4, 0, 3}, {6, 7, 3, 0}});
    EXPECT_DOUBLE_EQ(graph.tourLength({0, 1, 2, 3}), 18.0);
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

TEST(CoordinatesGraph, Euc2dRoundsToTheNearestInteger) {
    CoordinatesGraph graph({{0, 0}, {3, 4}, {1, 1}, {2, 3}}, EdgeWeightType::Euc2d);
    EXPECT_DOUBLE_EQ(graph.weight(0, 1), 5.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 2), 1.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 3), 4.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 0), 0.0);
}

TEST(CoordinatesGraph, DefaultTypeKeepsThePlainDistance) {
    CoordinatesGraph graph({{0, 0}, {1, 1}});
    EXPECT_NEAR(graph.weight(0, 1), 1.41421356, 1e-6);
}

TEST(CoordinatesGraph, AttRoundsUpWhenNintRoundsDown) {
    CoordinatesGraph graph({{0, 0}, {10, 0}, {9, 3}, {4, 11}}, EdgeWeightType::Att);
    EXPECT_DOUBLE_EQ(graph.weight(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 2), 3.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 3), 4.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 0), 0.0);
}

TEST(CoordinatesGraph, GeoUsesLatitudeAndLongitudeInDegreesAndMinutes) {
    // first three cities of burma14
    CoordinatesGraph graph({{16.47, 96.10}, {16.47, 94.44}, {20.09, 92.54}}, EdgeWeightType::Geo);
    EXPECT_DOUBLE_EQ(graph.weight(0, 1), 153.0);
    EXPECT_DOUBLE_EQ(graph.weight(0, 2), 510.0);  // 560 if the degrees were rounded instead of truncated
    EXPECT_DOUBLE_EQ(graph.weight(1, 2), 422.0);
    EXPECT_DOUBLE_EQ(graph.weight(1, 1), 0.0);
}
