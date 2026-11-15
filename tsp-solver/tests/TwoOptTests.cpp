// Partly edited by Claude (Claude Opus 5.5), 2026-11-15: helpers from TestHelpers.hpp, removed IsDeterministic (the exact tours expected below already show it)
#include <gtest/gtest.h>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include "CoordinatesGraph.hpp"
#include "TestHelpers.hpp"
#include "TwoOpt.hpp"

using namespace tsp;
using namespace helpers;

namespace {
    Tour identityTour(std::size_t n) {
        Tour tour(n);
        std::iota(tour.begin(), tour.end(), 0);
        return tour;
    }

    // Independent check of the stopping condition: every 2-opt move is applied on a copy
    // and the full tour length is recomputed, without using the gain formula of TwoOpt
    bool noImprovingMoveLeft(const Graph& graph, const Tour& tour) {
        const std::size_t n = tour.size();
        const double length = graph.tourLength(tour);

        for (std::size_t i = 0; i + 2 < n; i++) {
            for (std::size_t j = i + 2; j < n; j++) {
                if (i == 0 && j == n - 1) {
                    continue;
                }
                Tour moved = tour;
                std::reverse(moved.begin() + i + 1, moved.begin() + j + 1);
                if (graph.tourLength(moved) < length - 1e-9) {
                    return false;
                }
            }
        }
        return true;
    }
}

TEST(TwoOpt, ImprovesTheMstTourOfTheFourCityExample) {
    MatrixGraph graph = fourCities();
    Tour tour = TwoOpt().improve(graph, {0, 1, 2, 3});   // A B C D, length 14

    EXPECT_EQ(tour, Tour({0, 1, 3, 2}));                 // A B D C
    EXPECT_DOUBLE_EQ(graph.tourLength(tour), 13.0);
}

TEST(TwoOpt, ImprovesTheNearestNeighbourTourOfTheFourCityExample) {
    MatrixGraph graph = fourCities();
    Tour tour = TwoOpt().improve(graph, {0, 2, 1, 3});   // A C B D, length 15

    EXPECT_TRUE(isPermutation(tour, 4));
    EXPECT_DOUBLE_EQ(graph.tourLength(tour), 13.0);
}

TEST(TwoOpt, KeepsAnOptimalTourUnchanged) {
    EXPECT_EQ(TwoOpt().improve(fourCities(), {0, 1, 3, 2}), Tour({0, 1, 3, 2}));
}

TEST(TwoOpt, RemovesTheCrossingOnASquare) {
    CoordinatesGraph square({{0, 0}, {1, 0}, {1, 1}, {0, 1}});
    Tour tour = TwoOpt().improve(square, {0, 2, 1, 3});  // the two diagonals cross

    EXPECT_DOUBLE_EQ(square.tourLength(tour), 4.0);
}

TEST(TwoOpt, ToursWithFewerThanFourCitiesAreUnchanged) {
    EXPECT_EQ(TwoOpt().improve(MatrixGraph({{0}}), {0}), Tour({0}));
    EXPECT_EQ(TwoOpt().improve(MatrixGraph({{0, 1}, {1, 0}}), {1, 0}), Tour({1, 0}));
    EXPECT_EQ(TwoOpt().improve(MatrixGraph({{0, 1, 2}, {1, 0, 3}, {2, 3, 0}}), {2, 0, 1}), Tour({2, 0, 1}));
}

TEST(TwoOpt, RejectsATourOfTheWrongSize) {
    EXPECT_THROW(TwoOpt().improve(fourCities(), {0, 1, 2}), std::invalid_argument);
}

TEST(TwoOpt, ZeroPassesLeavesTheTourUnchanged) {
    EXPECT_EQ(TwoOpt(0).improve(fourCities(), {0, 2, 1, 3}), Tour({0, 2, 1, 3}));
}

TEST(TwoOpt, LimitedPassesNeverMakeTheTourLonger) {
    TspInstance instance = load("coord/berlin52.tsp");
    const Graph& graph = instance.getGraph();
    const Tour start = identityTour(graph.size());

    Tour onePass = TwoOpt(1).improve(graph, start);
    Tour full = TwoOpt().improve(graph, start);

    EXPECT_TRUE(isPermutation(onePass, graph.size()));
    EXPECT_LE(graph.tourLength(onePass), graph.tourLength(start));
    EXPECT_LE(graph.tourLength(full), graph.tourLength(onePass));
}

class TwoOptOnProvidedInstances : public ::testing::TestWithParam<const char*> {};

TEST_P(TwoOptOnProvidedInstances, ImprovesTheTrivialTourUntilNoMoveIsLeft) {
    TspInstance instance = load(GetParam());
    const Graph& graph = instance.getGraph();
    const Tour start = identityTour(graph.size());

    Tour tour = TwoOpt().improve(graph, start);

    EXPECT_TRUE(isPermutation(tour, graph.size()));
    EXPECT_LE(graph.tourLength(tour), graph.tourLength(start));

    // the independent check recomputes whole tours, which is too slow for pr1002
    if (graph.size() <= 300) {
        EXPECT_TRUE(noImprovingMoveLeft(graph, tour));
    }
}

INSTANTIATE_TEST_SUITE_P(AllFiles, TwoOptOnProvidedInstances, ::testing::ValuesIn(instanceFiles));
