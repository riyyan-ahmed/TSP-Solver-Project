// everyStepGoesToTheNearestCity written by Claude (Claude Opus 5.5), 2026-11-07
#include <gtest/gtest.h>
#include "CoordinatesGraph.hpp"
#include "MinimumSpanningTree.hpp"
#include "NearestNeighbourConstructor.hpp"
#include "TestHelpers.hpp"
#include "TwoOpt.hpp"

using namespace tsp;
using namespace helpers;

namespace {
    // Independent check of the rule: at every step, no remaining city is strictly closer than
    // the chosen one, and a city at the same distance is never preferred over a smaller index
    bool everyStepGoesToTheNearestCity(const Graph& graph, const Tour& tour) {
        for (std::size_t k = 0; k + 1 < tour.size(); k++) {
            const double chosen = graph.weight(tour[k], tour[k + 1]);

            for (std::size_t m = k + 2; m < tour.size(); m++) {
                const double other = graph.weight(tour[k], tour[m]);
                if (other < chosen || (other == chosen && tour[m] < tour[k + 1])) {
                    return false;
                }
            }
        }
        return true;
    }
}

TEST(NearestNeighbourConstructor, BuildsTheTourOfTheFourCityExample) {
    MatrixGraph graph = fourCities();
    Tour tour = NearestNeighbourConstructor().build(graph);

    EXPECT_EQ(tour, Tour({0, 2, 1, 3}));                
    EXPECT_DOUBLE_EQ(graph.tourLength(tour), 15.0);
}

TEST(NearestNeighbourConstructor, FollowsTheClosestCityNotTheIndexOrder) {
    // cities on a line, numbered out of order
    CoordinatesGraph line({{0, 0}, {3, 0}, {1, 0}, {2, 0}});
    EXPECT_EQ(NearestNeighbourConstructor().build(line), Tour({0, 2, 3, 1}));
}

TEST(NearestNeighbourConstructor, BreaksTiesWithTheSmallestIndex) {
    // from city 0, cities 1 and 2 are both at distance 1
    CoordinatesGraph square({{0, 0}, {1, 0}, {0, 1}, {1, 1}});
    EXPECT_EQ(NearestNeighbourConstructor().build(square), Tour({0, 1, 3, 2}));
}

TEST(NearestNeighbourConstructor, HandlesOneTwoAndThreeCities) {
    EXPECT_EQ(NearestNeighbourConstructor().build(MatrixGraph({{0}})), Tour({0}));
    EXPECT_EQ(NearestNeighbourConstructor().build(MatrixGraph({{0, 7}, {7, 0}})), Tour({0, 1}));
    EXPECT_EQ(NearestNeighbourConstructor().build(MatrixGraph({{0, 5, 2}, {5, 0, 3}, {2, 3, 0}})), Tour({0, 2, 1}));
}

class NearestNeighbourOnProvidedInstances : public ::testing::TestWithParam<const char*> {};

TEST_P(NearestNeighbourOnProvidedInstances, BuildsAValidTourThatTwoOptCanOnlyShorten) {
    TspInstance instance = load(GetParam());
    const Graph& graph = instance.getGraph();

    const Tour tour = NearestNeighbourConstructor().build(graph);
    const Tour improved = TwoOpt().improve(graph, tour);

    EXPECT_TRUE(isPermutation(tour, graph.size()));
    EXPECT_EQ(tour.front(), NearestNeighbourConstructor::start);
    EXPECT_TRUE(everyStepGoesToTheNearestCity(graph, tour));

    EXPECT_TRUE(isPermutation(improved, graph.size()));
    EXPECT_LE(graph.tourLength(improved), graph.tourLength(tour));
    EXPECT_LE(MinimumSpanningTree(graph).weight(), graph.tourLength(improved));
}

INSTANTIATE_TEST_SUITE_P(AllFiles, NearestNeighbourOnProvidedInstances, ::testing::ValuesIn(instanceFiles));
