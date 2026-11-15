// Partly edited by Claude (Claude Opus 5.5), 2026-11-14: include order and trailing spaces
#include <gtest/gtest.h>
#include <algorithm>
#include <limits>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>
#include "CoordinatesGraph.hpp"
#include "HeldKarpSolver.hpp"
#include "TestHelpers.hpp"

using namespace tsp;
using namespace helpers;

namespace {
    TspInstance makeInstance(std::vector<std::pair<double, double>> points) {
        std::vector<int> ids(points.size());
        std::iota(ids.begin(), ids.end(), 1);
        return TspInstance("test", ids, std::make_unique<CoordinatesGraph>(std::move(points)));
    }

    // tries every tour that starts at city 0
    double bruteForceOptimum(const Graph& graph) {
        Tour tour(graph.size());
        std::iota(tour.begin(), tour.end(), 0);
        double best = std::numeric_limits<double>::infinity();
        do {
            best = std::min(best, graph.tourLength(tour));
        } while (std::next_permutation(tour.begin() + 1, tour.end()));
        return best;
    }
}

TEST(HeldKarpSolver, FindsTheTourOfTheSpecificationExample) {
    // figure 2 of the subject, the best tour A B C D has length 18
    TspInstance instance("example", {1, 2, 3, 4}, std::make_unique<MatrixGraph>(
        DistanceMatrix{{0, 5, 8, 6}, {5, 0, 4, 7}, {8, 4, 0, 3}, {6, 7, 3, 0}}));
    const Tour tour = HeldKarpSolver().solve(instance);
    EXPECT_TRUE(isPermutation(tour, 4));
    EXPECT_EQ(tour.front(), 0u);
    EXPECT_DOUBLE_EQ(instance.getGraph().tourLength(tour), 18.0);
}

TEST(HeldKarpSolver, HandlesOneTwoAndThreeCities) {
    EXPECT_EQ(HeldKarpSolver().solve(makeInstance({{0, 0}})), Tour({0}));
    EXPECT_EQ(HeldKarpSolver().solve(makeInstance({{0, 0}, {1, 0}})), Tour({0, 1}));
    EXPECT_TRUE(isPermutation(HeldKarpSolver().solve(makeInstance({{0, 0}, {1, 0}, {0, 1}})), 3));
}

TEST(HeldKarpSolver, MatchesBruteForceOnSmallInstances) {
    for (std::size_t n = 4; n <= 9; n++) {
        SCOPED_TRACE(n);
        // spread out points without randomness so the test always does the same thing
        std::vector<std::pair<double, double>> points;
        for (std::size_t i = 0; i < n; i++) {
            points.emplace_back((i * 37) % 101, (i * 59) % 97);
        }
        TspInstance instance = makeInstance(points);
        const Tour tour = HeldKarpSolver().solve(instance);
        EXPECT_TRUE(isPermutation(tour, n));
        EXPECT_NEAR(instance.getGraph().tourLength(tour), bruteForceOptimum(instance.getGraph()), 1e-9);
    }
}

TEST(HeldKarpSolver, FindsTheKnownOptimaOfTheSmallInstances) {
    // optimal tour lengths from TSPLIB, gr24 is left out because it needs 1.5 GB of memory
    const std::pair<const char*, double> known[] = {
        {"coord/burma14.tsp", 3323}, {"coord/ulysses16.tsp", 6859},
        {"lower_diag/gr17.tsp", 2085}, {"lower_diag/gr21.tsp", 2707}};

    for (const auto& [file, optimum] : known) {
        SCOPED_TRACE(file);
        TspInstance instance = load(file);
        const Tour tour = HeldKarpSolver().solve(instance);
        EXPECT_TRUE(isPermutation(tour, instance.size()));
        EXPECT_DOUBLE_EQ(instance.getGraph().tourLength(tour), optimum);
    }
}

TEST(HeldKarpSolver, RejectsTooManyCities) {
    std::vector<std::pair<double, double>> points;
    for (std::size_t i = 0; i <= HeldKarpSolver::maxCities; i++) {
        points.emplace_back(i, 0);
    }
    EXPECT_THROW(HeldKarpSolver().solve(makeInstance(points)), std::invalid_argument);
}
